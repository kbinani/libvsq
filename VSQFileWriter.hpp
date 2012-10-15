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
protected:
    class TempEvent : public Event{
    public:
        /**
         * VSQ メタテキストに出力されるこのオブジェクトの ID
         */
        int index;

        int singerHandleIndex;
        int lyricHandleIndex;
        int vibratoHandleIndex;
        int noteHeadHandleIndex;

        explicit TempEvent( const Event &item ) :
            Event( item )
        {
            index = -1;
            singerHandleIndex = -1;
            lyricHandleIndex = -1;
            vibratoHandleIndex = -1;
            noteHeadHandleIndex = -1;
        }
    };

public:
    /**
     * @brief ストリームに出力する
     * @param 出力するシーケンス
     * @param stream 出力先のストリーム
     * @param msPreSend ミリ秒単位のプリセンドタイム
     * @param encoding マルチバイト文字のテキストエンコーディング(現在は Shift_JIS 固定で、引数は無視される)
     * @param printPitch pitch を含めて出力するかどうか(現在は <code>false</code> 固定で、引数は無視される)
     */
    void write( const Sequence *sequence, OutputStream *stream, int msPreSend, const string &encoding, bool printPitch = false ){
        Sequence copyOfSequence = *sequence;
        Sequence *targetSequence = &copyOfSequence;
        targetSequence->updateTotalClocks();
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
        writeUnsignedShort( stream, targetSequence->track.size() + 1 );
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
        for( int i = 0; i < targetSequence->timesigList.size(); i++ ){
            Timesig entry = targetSequence->timesigList.get( i );
            events.push_back( MidiEvent::generateTimeSigEvent( entry.getClock(), entry.numerator, entry.denominator ) );
        }
        TempoList::Iterator itr = targetSequence->tempoList.iterator();
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
        Sequence temp = targetSequence->clone();
        _printTrack( &temp, 0, stream, msPreSend, encoding, printPitch, &targetSequence->master, &targetSequence->mixer );
        int count = targetSequence->track.size();
        for( int track = 1; track < count; track++ ){
            _printTrack( targetSequence, track, stream, msPreSend, encoding, printPitch, 0, 0 );
        }
    }

protected:
    /**
     * @brief ハンドルをストリームに書き込む
     * @param item 書き込むハンドル
     * @param stream 書き込み先のストリーム
     */
    void writeHandle( const Handle &item, VSQ_NS::TextStream &stream ){
        stream.writeLine( string( "[h#" ) + StringUtil::toString( item.index, "%04d" ) + string( "]" ) );
        if( item.getHandleType() == HandleType::LYRIC ){
            for( int i = 0; i < item.getLyricCount(); i++ ){
                stream.writeLine( string( "L" ) + StringUtil::toString( i ) + "=" + item.getLyricAt( i ).toString( item.addQuotationMark ) );
            }
        }else if( item.getHandleType() == HandleType::VIBRATO ){
            stream.writeLine( string( "IconID=" ) + item.iconId );
            stream.writeLine( string( "IDS=" ) + item.ids );
            stream.writeLine( string( "Original=" ) + StringUtil::toString( item.original ) );
            stream.writeLine( string( "Caption=" ) + item.caption );
            stream.writeLine( string( "Length=" ) + StringUtil::toString( item.getLength() ) );
            stream.writeLine( string( "StartDepth=" ) + StringUtil::toString( item.startDepth ) );
            stream.writeLine( string( "DepthBPNum=" ) + StringUtil::toString( item.depthBP.size() ) );
            if( item.depthBP.size() > 0 ){
                stream.write( string( "DepthBPX=" ) + StringUtil::toString( item.depthBP.get( 0 ).x, "%.6f" ) );
                for( int i = 1; i < item.depthBP.size(); i++ ){
                    stream.write( string( "," ) + StringUtil::toString( item.depthBP.get( i ).x, "%.6f" ) );
                }
                stream.writeLine( "" );
                stream.write( string( "DepthBPY=" ) + StringUtil::toString( item.depthBP.get( 0 ).y ) );
                for( int i = 1; i < item.depthBP.size(); i++ ){
                    stream.write( string( "," ) + StringUtil::toString( item.depthBP.get( i ).y ) );
                }
                stream.writeLine( "" );
            }
            stream.writeLine( string( "StartRate=" ) + StringUtil::toString( item.startRate ) );
            stream.writeLine( string( "RateBPNum=" ) + StringUtil::toString( item.rateBP.size() ) );
            if( item.rateBP.size() > 0 ){
                stream.write( string( "RateBPX=" ) + StringUtil::toString( item.rateBP.get( 0 ).x, "%.6f" ) );
                for( int i = 1; i < item.rateBP.size(); i++ ){
                    stream.write( string( "," ) + StringUtil::toString( item.rateBP.get( i ).x, "%.6f" ) );
                }
                stream.writeLine( "" );
                stream.write( string( "RateBPY=" ) + StringUtil::toString( item.rateBP.get( 0 ).y ) );
                for( int i = 1; i < item.rateBP.size(); i++ ){
                    stream.write( string( "," ) + StringUtil::toString( item.rateBP.get( i ).y ) );
                }
                stream.writeLine( "" );
            }
        }else if( item.getHandleType() == HandleType::SINGER ){
            stream.writeLine( string( "IconID=" ) + item.iconId );
            stream.writeLine( string( "IDS=" ) + item.ids );
            stream.writeLine( string( "Original=" ) + StringUtil::toString( item.original ) );
            stream.writeLine( string( "Caption=" ) + item.caption );
            stream.writeLine( string( "Length=" ) + StringUtil::toString( item.getLength() ) );
            stream.writeLine( string( "Language=" ) + StringUtil::toString( item.language ) );
            stream.writeLine( string( "Program=" ) + StringUtil::toString( item.program ) );
        }else if( item.getHandleType() == HandleType::NOTE_HEAD ){
            stream.writeLine( string( "IconID=" ) + item.iconId );
            stream.writeLine( string( "IDS=" ) + item.ids );
            stream.writeLine( string( "Original=" ) + StringUtil::toString( item.original ) );
            stream.writeLine( string( "Caption=" ) + item.caption );
            stream.writeLine( string( "Length=" ) + StringUtil::toString( item.getLength() ) );
            stream.writeLine( string( "Duration=" ) + StringUtil::toString( item.duration ) );
            stream.writeLine( string( "Depth=" ) + StringUtil::toString( item.depth ) );
        }else if( item.getHandleType() == HandleType::DYNAMICS ){
            stream.writeLine( string( "IconID=" ) + item.iconId );
            stream.writeLine( string( "IDS=" ) + item.ids );
            stream.writeLine( string( "Original=" ) + StringUtil::toString( item.original ) );
            stream.writeLine( string( "Caption=" ) + item.caption );
            stream.writeLine( string( "StartDyn=" ) + StringUtil::toString( item.startDyn ) );
            stream.writeLine( string( "EndDyn=" ) + StringUtil::toString( item.endDyn ) );
            stream.writeLine( string( "Length=" ) + StringUtil::toString( item.getLength() ) );
            if( item.dynBP.size() <= 0 ){
                stream.writeLine( "DynBPNum=0" );
            }else{
                int c = item.dynBP.size();
                stream.writeLine( string( "DynBPNum=" ) + StringUtil::toString( c ) );
                stream.write( string( "DynBPX=" ) + StringUtil::toString( item.dynBP.get( 0 ).x, "%.6f" ) );
                for( int i = 1; i < c; i++ ){
                    stream.write( string( "," ) + StringUtil::toString( item.dynBP.get( i ).x, "%.6f" ) );
                }
                stream.writeLine( "" );
                stream.write( string( "DynBPY=" ) + StringUtil::toString( item.dynBP.get( 0 ).y ) );
                for( int i = 1; i < c; i++ ){
                    stream.write( string( "," ) + StringUtil::toString( item.dynBP.get( i ).y ) );
                }
                stream.writeLine( "" );
            }
        }
    }

    /**
     * @brief テキストストリームにイベントを書き出す
     * @param stream (TextStream) 出力先
     * @param printTargets (table) 出力するアイテムのリスト
     * @todo boost使ってる箇所をStringUtil使うよう変更
     */
    void writeEvent( const TempEvent &item, VSQ_NS::TextStream &stream, VSQ_NS::EventWriteOption printTargets = EventWriteOption() ) const{
        stream.write( "[ID#" );
        stream.write( (boost::format( "%04d" ) % item.index).str() );
        stream.writeLine( "]" );
        stream.write( "Type=" );
        stream.writeLine( EventType::toString( item.type ) );
        if( item.type == EventType::NOTE ){
            if( printTargets.length ){
                stream.write( "Length=" );
                stream.writeLine( (boost::format( "%ld" ) % item.getLength()).str() );
            }
            if( printTargets.note ){
                stream.write( "Note#=" );
                stream.writeLine( (boost::format( "%d" ) % item.note).str() );
            }
            if( printTargets.dynamics ){
                stream.write( "Dynamics=" );
                stream.writeLine( (boost::format( "%d" ) % item.dynamics).str() );
            }
            if( printTargets.pmBendDepth ){
                stream.write( "PMBendDepth=" );
                stream.writeLine( (boost::format( "%d" ) % item.pmBendDepth).str() );
            }
            if( printTargets.pmBendLength ){
                stream.write( "PMBendLength=" );
                stream.writeLine( (boost::format( "%d" ) % item.pmBendLength).str() );
            }
            if( printTargets.pmbPortamentoUse ){
                stream.write( "PMbPortamentoUse=" );
                stream.writeLine( (boost::format( "%d" ) % item.pmbPortamentoUse).str() );
            }
            if( printTargets.demDecGainRate ){
                stream.write( "DEMdecGainRate=" );
                stream.writeLine( (boost::format( "%d" ) % item.demDecGainRate).str() );
            }
            if( printTargets.demAccent ){
                stream.write( "DEMaccent=" );
                stream.writeLine( (boost::format( "%d" ) % item.demAccent).str() );
            }
            if( printTargets.preUtterance ){
                //TODO:
    //            stream.writeLine( "PreUtterance=" + ustEvent.preUtterance );
            }
            if( printTargets.voiceOverlap ){
                //TODO:
    //            stream.writeLine( "VoiceOverlap=" + ustEvent.voiceOverlap );
            }
            if( item.lyricHandle.getHandleType() == HandleType::LYRIC ){
                stream.write( "LyricHandle=h#" );
                stream.writeLine( (boost::format( "%04d" ) % item.lyricHandle.index).str() );
            }
            if( item.vibratoHandle.getHandleType() == HandleType::VIBRATO ){
                stream.write( "VibratoHandle=h#" );
                stream.writeLine( (boost::format( "%04d" ) % item.vibratoHandle.index).str() );
                stream.write( "VibratoDelay=" );
                stream.writeLine( (boost::format( "%d" ) % item.vibratoDelay).str() );
            }
            if( item.noteHeadHandle.getHandleType() == HandleType::NOTE_HEAD ){
                stream.write( "NoteHeadHandle=h#" );
                stream.writeLine( (boost::format( "%04d" ) % item.noteHeadHandle.index).str() );
            }
        }else if( item.type == EventType::SINGER ){
            stream.write( "IconHandle=h#" );
            stream.writeLine( (boost::format( "%04d" ) % item.singerHandle.index).str() );
        }else if( item.type == EventType::ICON ){
            stream.write( "IconHandle=h#" );
            stream.writeLine( (boost::format( "%04d" ) % item.iconDynamicsHandle.index).str() );
            stream.write( "Note#=" );
            stream.writeLine( (boost::format( "%d" ) % item.note).str() );
        }
    }

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
        //if( common ~= nil ){
            track.getCommon()->write( stream );
        //}
        if( master ){
            master->write( stream );
        }
        if( mixer ){
            mixer->write( stream );
        }

        vector<Handle> handle;
        {
            vector<TempEvent *> eventList;
            Event::ListIterator itr = track.getEvents()->iterator();
            while( itr.hasNext() ){
                Event *item = itr.next();
                eventList.push_back( new TempEvent( *item ) );
            }

            handle = writeEventList( eventList, stream, eos );
            for( vector<TempEvent *>::iterator itr = eventList.begin(); itr != eventList.end(); ++itr ){
                TempEvent *item = *itr;
                writeEvent( *item, stream );
                delete item;
            }
        }
        for( int i = 0; i < handle.size(); ++i ){
            writeHandle( handle[i], stream );
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
        const Event *last_event = sequence->track[track].getEvents()->get( sequence->track[track].getEvents()->size() - 1 );
        maxClock = ::max( maxClock, last_event->clock + last_event->getLength() );
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
     * @brief イベントリストをテキストストリームに出力する
     * @param stream 出力先のストリーム
     * @param eos EOS として出力する Tick 単位の時刻
     * @return リスト中のイベントに含まれるハンドルの一覧
     */
    std::vector<VSQ_NS::Handle> writeEventList( vector<TempEvent *> &eventList, TextStream &stream, VSQ_NS::tick_t eos ){
        vector<Handle> handles = getHandleList( eventList );
        stream.writeLine( "[EventList]" );
        vector<TempEvent> temp;
        for( vector<TempEvent *>::iterator itr = eventList.begin(); itr != eventList.end(); ++itr ){
            temp.push_back( **itr );
        }
        std::sort( temp.begin(), temp.end(), Event::compare );
        int i = 0;
        while( i < temp.size() ){
            TempEvent item = temp[i];
            if( ! item.isEOS() ){
                ostringstream ids;
                ids << "ID#" << (boost::format( "%04d" ) % item.index).str();
                tick_t clock = temp[i].clock;
                while( i + 1 < temp.size() && clock == temp[i + 1].clock ){
                    i++;
                    ids << ",ID#" << (boost::format( "%04d" ) % temp[i].index).str();
                }
                ostringstream oss;
                oss << clock << "=" << ids.str();
                stream.writeLine( oss.str() );
            }
            i++;
        }
        stream.write( (boost::format( "%d" ) % eos).str() );
        stream.writeLine( "=EOS" );
        return handles;
    }

    /**
     * @brief リスト内のイベントから、ハンドルの一覧を作成する。同時に、各イベント、ハンドルの番号を設定する
     * @return (table<Handle>) ハンドルの一覧
     */
    const std::vector<Handle> getHandleList( vector<TempEvent *> &eventList ){
        vector<Handle> handle;
        int current_id = -1;
        int current_handle = -1;
        bool add_quotation_mark = true;
        for( vector<TempEvent *>::iterator itr = eventList.begin(); itr != eventList.end(); ++itr ){
            TempEvent *item = *itr;
            current_id = current_id + 1;
            item->index = current_id;
            // SingerHandle
            if( item->singerHandle.getHandleType() == HandleType::SINGER ){
                current_handle = current_handle + 1;
                item->singerHandle.index = current_handle;
                handle.push_back( item->singerHandle );
                item->singerHandleIndex = current_handle;
                VoiceLanguage::VoiceLanguageEnum lang = VoiceLanguage::valueFromSingerName( item->singerHandle.ids );
                add_quotation_mark = lang == VoiceLanguage::JAPANESE;
            }
            // LyricHandle
            if( item->lyricHandle.getHandleType() == HandleType::LYRIC ){
                current_handle = current_handle + 1;
                item->lyricHandle.index = current_handle;
                item->lyricHandle.addQuotationMark = add_quotation_mark;
                handle.push_back( item->lyricHandle );
                item->lyricHandleIndex = current_handle;
            }
            // VibratoHandle
            if( item->vibratoHandle.getHandleType() == HandleType::VIBRATO ){
                current_handle = current_handle + 1;
                item->vibratoHandle.index = current_handle;
                handle.push_back( item->vibratoHandle );
                item->vibratoHandleIndex = current_handle;
            }
            // NoteHeadHandle
            if( item->noteHeadHandle.getHandleType() == HandleType::NOTE_HEAD ){
                current_handle = current_handle + 1;
                item->noteHeadHandle.index = current_handle;
                handle.push_back( item->noteHeadHandle );
                item->noteHeadHandleIndex = current_handle;
            }
            // IconDynamicsHandle
            if( item->iconDynamicsHandle.getHandleType() == HandleType::DYNAMICS ){
                current_handle = current_handle + 1;
                item->iconDynamicsHandle.index = current_handle;
                item->iconDynamicsHandle.setLength( item->getLength() );
                handle.push_back( item->iconDynamicsHandle );
                // IconDynamicsHandleは、歌手ハンドルと同じ扱いなので
                // _singerHandleIndexでよい
                item->singerHandleIndex = current_handle;
            }
        }
        return handle;
    }

    /**
     * @brief SMF のトラックヘッダー文字列を取得する
     */
    string getTrackHeader(){
        return string( "MTrk" );
    }
};

VSQ_END_NAMESPACE

#endif
