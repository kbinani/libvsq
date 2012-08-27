/**
 * VSQFileWriter.hpp
 * Copyright © 2012 kbinani
 *
 * This file is part of libvsq.
 *
 * libvsq is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * libvsq is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#ifndef __VSQFileWriter_hpp__
#define __VSQFileWriter_hpp__

#include "vsqglobal.hpp"
#include "Sequence.hpp"
#include "VocaloidMidiEventListFactory.hpp"

VSQ_BEGIN_NAMESPACE

class VSQFileWriter{
public:
    /**
     * @brief ストリームに出力する
     * @param stream (? extends OutputStream) 出力先のストリーム
     * @param msPreSend (int) ミリ秒単位のプリセンドタイム
     * @param encoding (string) マルチバイト文字のテキストエンコーディング(現在は Shift_JIS 固定で、引数は無視される)
     * @param printPitch (boolean) pitch を含めて出力するかどうか(現在は <code>false</code> 固定で、引数は無視される)
     */
    void write( Sequence *sequence, OutputStream *stream, int msPreSend, const string &encoding, bool printPitch = false ){
        sequence->updateTotalClocks();
        int64_t first_position; //チャンクの先頭のファイル位置

        // ヘッダ
        // チャンクタイプ
        char mthd[] = { 0x4d, 0x54, 0x68, 0x64 };
        stream->write( mthd, 0, 4 );
        // データ長
        stream->write( 0x00 );
        stream->write( 0x00 );
        stream->write( 0x00 );
        stream->write( 0x06 );
        // フォーマット
        stream->write( 0x00 );
        stream->write( 0x01 );
        // トラック数
        writeUnsignedShort( stream, sequence->track.size() );
        // 時間単位
        stream->write( 0x01 );
        stream->write( 0xe0 );

        // Master Track
        // チャンクタイプ
        string mtrk = getTrackHeader();
        stream->write( mtrk.c_str(), 0, 4 );
        // データ長。とりあえず0を入れておく
        char empty[] = { 0x00, 0x00, 0x00, 0x00 };
        stream->write( empty, 0, 4 );
        first_position = stream->getPointer();
        // トラック名
        const int masterTrackNameLength = 12;
        char masterTrackName[masterTrackNameLength] = { 0x4D, 0x61, 0x73, 0x74, 0x65, 0x72, 0x20, 0x54, 0x72, 0x61, 0x63, 0x6B };
        MidiEvent::writeDeltaClock( stream, 0 );// デルタタイム
        stream->write( 0xff );// ステータスタイプ
        stream->write( 0x03 );// イベントタイプSequence/Track Name
        stream->write( masterTrackNameLength );// トラック名の文字数。これは固定
        stream->write( masterTrackName, 0, masterTrackNameLength );

        vector<MidiEvent> events;
        for( int i = 0; i < sequence->timesigList.size(); i++ ){
            Timesig entry = sequence->timesigList.get( i );
            events.push_back( MidiEvent::generateTimeSigEvent( entry.clock, entry.numerator, entry.denominator ) );
        }
        TempoList::Iterator itr = sequence->tempoList.iterator();
        while( itr.hasNext() ){
            Tempo entry = itr.next();
            events.push_back( MidiEvent::generateTempoChangeEvent( entry.clock, entry.tempo ) );
        }
        std::sort( events.begin(), events.end(), MidiEvent::compare );
        tick_t last = 0;
        for( int i = 0; i < events.size(); i++ ){
            MidiEvent midiEvent = events[i];
            MidiEvent::writeDeltaClock( stream, midiEvent.clock - last );
            midiEvent.writeData( stream );
            last = midiEvent.clock;
        }

        MidiEvent::writeDeltaClock( stream, 0 );
        stream->write( 0xff );
        stream->write( 0x2f );// イベントタイプEnd of Track
        stream->write( 0x00 );
        int64_t pos = stream->getPointer();
        stream->seek( first_position - 4 );
        writeUnsignedInt( stream, pos - first_position );
        stream->seek( pos );

        // トラック
        Sequence temp = sequence->clone();
        _printTrack( &temp, 1, stream, msPreSend, encoding, printPitch, &sequence->master, &sequence->mixer );
        int count = sequence->track.size();
        for( int track = 2; track < count; track++ ){
            _printTrack( sequence, track, stream, msPreSend, encoding, printPitch, 0, 0 );
        }
    }

protected:
    /**
     * @brief トラックのメタテキストを、テキストストリームに出力する
     * @param track 出力するトラック
     * @param stream 出力先のストリーム
     * @param eos イベントリストの末尾を表す番号
     * @param start Tick 単位の出力開始時刻
     * @param printPitch pitch を含めて出力するかどうか(現在は <code>false</code> 固定で、引数は無視される)
     * @param master 出力する Master 情報。出力しない場合は NULL を指定する
     * @param mixer 出力する Mixer 情報。出力しない場合は NULL を指定する
     */
    void printMetaText( const Track &t, TextStream &stream, int eos, tick_t start, bool printPitch = false, Master *master = 0, Mixer *mixer = 0 ){
        Track track = t;
        //TODO: commonの型を Common* にする
        //if( common ~= nil ){
            track.getCommon()->write( stream );
        //}
        if( master ){
            master->write( stream );
        }
        if( mixer ){
            mixer->write( stream );
        }
        vector<Handle> handle = track.getEvents()->write( stream, eos );
        Event::ListIterator itr = track.getEvents()->iterator();
        while( itr.hasNext() ){
            Event *item = itr.next();
            item->write( stream );
        }
        for( int i = 0; i < handle.size(); ++i ){
            handle[i].write( stream );
        }
        string version = track.getCommon()->version;
        if( track.getCurve( "pit" )->size() > 0 ){
            track.getCurve( "pit" )->print( stream, start, "[PitchBendBPList]" );
        }
        if( track.getCurve( "pbs" )->size() > 0 ){
            track.getCurve( "pbs" )->print( stream, start, "[PitchBendSensBPList]" );
        }
        if( track.getCurve( "dyn" )->size() > 0 ){
            track.getCurve( "dyn" )->print( stream, start, "[DynamicsBPList]" );
        }
        if( track.getCurve( "bre" )->size() > 0 ){
            track.getCurve( "bre" )->print( stream, start, "[EpRResidualBPList]" );
        }
        if( track.getCurve( "bri" )->size() > 0 ){
            track.getCurve( "bri" )->print( stream, start, "[EpRESlopeBPList]" );
        }
        if( track.getCurve( "cle" )->size() > 0 ){
            track.getCurve( "cle" )->print( stream, start, "[EpRESlopeDepthBPList]" );
        }
        if( version.substr( 0, 4 ) == "DSB2" ){
            if( track.getCurve( "harmonics" )->size() > 0 ){
                track.getCurve( "harmonics" )->print( stream, start, "[EpRSineBPList]" );
            }
            if( track.getCurve( "fx2depth" )->size() > 0 ){
                track.getCurve( "fx2depth" )->print( stream, start, "[VibTremDepthBPList]" );
            }

            if( track.getCurve( "reso1Freq" )->size() > 0 ){
                track.getCurve( "reso1Freq" )->print( stream, start, "[Reso1FreqBPList]" );
            }
            if( track.getCurve( "reso2Freq" )->size() > 0 ){
                track.getCurve( "reso2Freq" )->print( stream, start, "[Reso2FreqBPList]" );
            }
            if( track.getCurve( "reso3Freq" )->size() > 0 ){
                track.getCurve( "reso3Freq" )->print( stream, start, "[Reso3FreqBPList]" );
            }
            if( track.getCurve( "reso4Freq" )->size() > 0 ){
                track.getCurve( "reso4Freq" )->print( stream, start, "[Reso4FreqBPList]" );
            }

            if( track.getCurve( "reso1BW" )->size() > 0 ){
                track.getCurve( "reso1BW" )->print( stream, start, "[Reso1BWBPList]" );
            }
            if( track.getCurve( "reso2BW" )->size() > 0 ){
                track.getCurve( "reso2BW" )->print( stream, start, "[Reso2BWBPList]" );
            }
            if( track.getCurve( "reso3BW" )->size() > 0 ){
                track.getCurve( "reso3BW" )->print( stream, start, "[Reso3BWBPList]" );
            }
            if( track.getCurve( "reso4BW" )->size() > 0 ){
                track.getCurve( "reso4BW" )->print( stream, start, "[Reso4BWBPList]" );
            }

            if( track.getCurve( "reso1Amp" )->size() > 0 ){
                track.getCurve( "reso1Amp" )->print( stream, start, "[Reso1AmpBPList]" );
            }
            if( track.getCurve( "reso2Amp" )->size() > 0 ){
                track.getCurve( "reso2Amp" )->print( stream, start, "[Reso2AmpBPList]" );
            }
            if( track.getCurve( "reso3Amp" )->size() > 0 ){
                track.getCurve( "reso3Amp" )->print( stream, start, "[Reso3AmpBPList]" );
            }
            if( track.getCurve( "reso4Amp" )->size() > 0 ){
                track.getCurve( "reso4Amp" )->print( stream, start, "[Reso4AmpBPList]" );
            }
        }

        if( track.getCurve( "gen" )->size() > 0 ){
            track.getCurve( "gen" )->print( stream, start, "[GenderFactorBPList]" );
        }
        if( track.getCurve( "por" )->size() > 0 ){
            track.getCurve( "por" )->print( stream, start, "[PortamentoTimingBPList]" );
        }
        if( version.substr( 0, 4 ) == "DSB3" ){
            if( track.getCurve( "ope" )->size() > 0 ){
                track.getCurve( "ope" )->print( stream, start, "[OpeningBPList]" );
            }
        }
    }

    /**
     * @brief トラックをストリームに出力する
     * @param sequence (Sequence) 出力するシーケンス
     * @param track (int) 出力するトラックの番号
     * @param stream (? extends OutputStream) 出力先のストリーム
     * @param msPreSend (int) ミリ秒単位のプリセンド時間
     * @param encoding (string) マルチバイト文字のテキストエンコーディング(現在は Shift_JIS 固定で、引数は無視される)
     * @param printPitch (boolean) pitch を含めて出力するかどうか(現在は false 固定で、引数は無視される)
     * @access static private
     */
    void _printTrack( Sequence *sequence, int track, OutputStream *stream, int msPreSend, const string &encoding, bool printPitch, Master *master = 0, Mixer *mixer = 0 ){
        // ヘッダ
        string mtrk = getTrackHeader();
        stream->write( mtrk.c_str(), 0, 4 );
        // データ長。とりあえず0
        char empty[] = { 0x00, 0x00, 0x00, 0x00 };
        stream->write( empty, 0, 4 );
        int64_t first_position = stream->getPointer();
        // トラック名
        MidiEvent::writeDeltaClock( stream, 0x00 );// デルタタイム
        stream->write( 0xff );// ステータスタイプ
        stream->write( 0x03 );// イベントタイプSequence/Track Name
        string seq_name = CP932Converter::convertFromUTF8( sequence->track[track].getName() );
        MidiEvent::writeDeltaClock( stream, seq_name.size() ); // seq_nameの文字数
        //TODO: 第3引数の型キャスト要らなくなるかも
        stream->write( seq_name.c_str(), 0, (int)seq_name.size() );

        // Meta Textを準備
        TextStream textStream;
        printMetaText( sequence->track[track], textStream, sequence->getTotalClocks() + 120, 0, printPitch, master, mixer );
        tick_t lastClock = 0;
        vector<MidiEvent> meta = getMidiEventsFromMetaText( &textStream, encoding );
        for( int i = 0; i < meta.size(); i++ ){
            MidiEvent::writeDeltaClock( stream, meta[i].clock - lastClock );
            meta[i].writeData( stream );
            lastClock = meta[i].clock;
        }
        tick_t maxClock = lastClock;

        lastClock = 0;
        vector<MidiEvent> nrpns =
            VocaloidMidiEventListFactory::generateMidiEventList(
                &sequence->track[track], &sequence->tempoList,
                sequence->getTotalClocks(), sequence->getPreMeasureClocks(), msPreSend
            );
        for( int i = 0; i < nrpns.size(); i++ ){
            MidiEvent item = nrpns[i];
            MidiEvent::writeDeltaClock( stream, item.clock - lastClock );
            item.writeData( stream );
            lastClock = item.clock;
        }
        maxClock = ::max( maxClock, lastClock );

        // トラックエンド
        lastClock = maxClock;
        Event last_event = sequence->track[track].getEvents()->get( sequence->track[track].getEvents()->size() - 1 );
        maxClock = ::max( maxClock, last_event.clock + last_event.getLength() );
        tick_t lastDeltaClock = maxClock - lastClock;
        if( lastDeltaClock < 0 ){
            lastDeltaClock = 0;
        }
        MidiEvent::writeDeltaClock( stream, lastDeltaClock );
        stream->write( 0xff );
        stream->write( 0x2f );
        stream->write( 0x00 );
        int64_t pos = stream->getPointer();
        stream->seek( first_position - 4 );
        writeUnsignedInt( stream, pos - first_position );
        stream->seek( pos );
    }

    /**
     * @brief 文字列を MIDI メタイベントにしたものを取得する
     * @param sr (TextStream) MIDI イベント生成元の文字列が出力されたストリーム
     * @param encoding (string) マルチバイト文字のテキストエンコーディング(現在は Shift_JIS 固定で、引数は無視される)
     */
    vector<MidiEvent> getMidiEventsFromMetaText( TextStream *sr, const string &encoding ){
        string _NL = "\n";// + (char)0x0a;
        vector<MidiEvent> ret;
        sr->setPointer( -1 );
        string tmp = "";
        if( sr->ready() ){
            int line_count = -1;
            vector<int> buffer;
            bool first = true;
            while( sr->ready() ){
                if( first ){
                    tmp = sr->readLine();
                    first = false;
                }else{
                    tmp = _NL + sr->readLine();
                }
                string line = CP932Converter::convertFromUTF8( tmp );
                for( int i = 0; i < line.size(); i++ ){
                    buffer.push_back( 0xff & line[i] );
                }
                vector<int> prefix = getLinePrefixBytes( line_count + 1 );
                while( prefix.size() + buffer.size() >= 127 ){
                    line_count++;
                    prefix = getLinePrefixBytes( line_count );
                    MidiEvent add;
                    add.clock = 0;
                    add.firstByte = 0xff;
                    add.data.push_back( 0x01 );
                    for( int i = 0; i < prefix.size(); i++ ){
                        add.data.push_back( prefix[i] );
                    }
                    int remain = 127;
                    for( int i = 0; !buffer.empty() && prefix.size() + i < remain; i++ ){
                        add.data.push_back( buffer[0] );
                        buffer.erase( buffer.begin() );
                    }
                    ret.push_back( add );
                    prefix = getLinePrefixBytes( line_count + 1 );
                }
            }
            if( false == buffer.empty() ){
                vector<int> prefix = getLinePrefixBytes( line_count + 1 );
                while( prefix.size() + buffer.size() >= 127 ){
                    line_count = line_count + 1;
                    prefix = getLinePrefixBytes( line_count );
                    MidiEvent add;
                    add.clock = 0;
                    add.firstByte = 0xff;
                    add.data.push_back( 0x01 );
                    int remain = 127;
                    for( int i = 0; i < prefix.size(); i++ ){
                        add.data.push_back( prefix[i] );
                    }
                    for( int i = 0; !buffer.empty() && prefix.size() + i < remain; i++ ){
                        add.data.push_back( buffer[0] );
                        buffer.erase( buffer.begin() );
                    }
                    ret.push_back( add );
                    prefix = getLinePrefixBytes( line_count + 1 );
                }
                if( false == buffer.empty() ){
                    line_count = line_count + 1;
                    prefix = getLinePrefixBytes( line_count );
                    MidiEvent add;
                    add.clock = 0;
                    add.firstByte = 0xff;
                    int remain = prefix.size() + buffer.size();
                    add.data.push_back( 0x01 );
                    for( int i = 0; i < prefix.size(); i++ ){
                        add.data.push_back( prefix[i] );
                    }
                    for( int i = 0; !buffer.empty() && prefix.size() + i < remain; i++ ){
                        add.data.push_back( buffer[0] );
                        buffer.erase( buffer.begin() );
                    }
                    ret.push_back( add );
                }
            }
        }

        return ret;
    }

    /**
     * @brief "DM:0001:"といった、VSQメタテキストの行の先頭につくヘッダー文字列のバイト列表現を取得する
     * @param count (int) ヘッダーの番号
     * @return (table<int>) バイト列
     */
    vector<int> getLinePrefixBytes( int count ){
        int digits = getHowManyDigits( count );
        int c = (int)::floor( (digits - 1) / 4 ) + 1;
        ostringstream format;
        format << "DM:%0" << (c * 4) << "d:";
        char *str = new char[1024];
        ::memset( str, 0, 1024 );
        sprintf( str, format.str().c_str(), count );

        string resultString = str;
        delete [] str;

        vector<int> result;
        for( int i = 0; i < resultString.size(); i++ ){
            result.push_back( 0xff & resultString[i] );
        }
        return result;
    }

    /**
     * @brief 数値の 10 進数での桁数を取得する
     * @param number (int) 検査対象の数値
     * @return (int) 数値の 10 進数での桁数
     */
    int getHowManyDigits( int number ){
        number = ::abs( number );
        if( number == 0 ){
            return 1;
        }else{
            return (int)::floor( ::log10( number ) ) + 1;
        }
    }

    /**
     * @brief 16 ビットの unsigned int 値をビッグエンディアンでストリームに書きこむ
     * @param stream (? extends OutputStream) 出力先のストリーム
     * @param data (int) 出力する値
     */
    void writeUnsignedShort( OutputStream *stream, int data ){
        vector<char> dat = BitConverter::getBytesUInt16BE( data );
        stream->write( dat.data(), 0, dat.size() );
    }

    /**
     * @brief 32 ビットの unsigned int 値をビッグエンディアンでストリームに書きこむ
     * @param stream (? extends OutputStram) 出力先のストリーム
     * @param data (int) 出力する値
     */
    void writeUnsignedInt( OutputStream *stream, int data ){
        vector<char> dat = BitConverter::getBytesUInt32BE( data );
        stream->write( dat.data(), 0, dat.size() );
    }

private:
    /**
     * @brief SMF のトラックヘッダー文字列を取得する
     */
    string getTrackHeader(){
        return string( "MTrk" );
    }
};

VSQ_END_NAMESPACE

#endif
