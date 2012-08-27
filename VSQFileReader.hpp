/**
 * VSQFileReader.hpp
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
#ifndef __VSQFileReader_hpp__
#define __VSQFileReader_hpp__

#include "vsqglobal.hpp"
#include "Sequence.hpp"
#include "InputStream.hpp"
#include "SMFReader.hpp"

VSQ_BEGIN_NAMESPACE

using namespace std;
using namespace VSQ_NS;

/**
 * @brief VSQ ファイルからのシーケンス読み込みを行うクラス
 */
class VSQFileReader{
protected:
    class TentativeHandle : public Handle{
    public:
        explicit TentativeHandle( HandleType::HandleTypeEnum type ) :
            Handle( type )
        {
        }

        void setHandleType( HandleType::HandleTypeEnum type ){
            _type = type;
        }

        void setLyrics( const vector<Lyric> &lyrics ){
            _lyrics = lyrics;
        }
    };

    class TentativeEvent : public Event{
    };

    class TentativeTrack : public Track{
    public:
        void setCommon( const Common &value ){
            common = value;
        }
    };

public:
    /**
     * @brief 読み込みストリームから、VSQ ファイルを読み込む
     * @param sequence 読み込み結果の格納先
     * @param stream 読み込みストリーム
     * @param encoding メタテキストのテキストエンコーディング(無視される。現在はShift_JIS固定)
     * @todo メタテキストに記録されず、NRPNにのみ出力される値がなんらかあったはずなので、それの読み込みもサポートするかどうか検討すること。
     */
    void read( Sequence &sequence, InputStream *stream, const std::string &encoding ){
        vector<vector<MidiEvent> > events;
        SMFReader reader;
        int format, timeFormat;
        reader.read( stream, events, format, timeFormat );

        int num_track = events.size();
        sequence.track.clear();
        for( int i = 0; i < num_track; i++ ){
            TextStream textStream;
            string trackName;
            getMetatextByMidiEventList( events[i], encoding, textStream, trackName );
            Track track;
            if( i == 1 ){
                Master master;
                Mixer mixer;
                track = getTrackByTextStream( textStream, &master, &mixer );
                sequence.master = master;
                sequence.mixer = mixer;
            }else{
                track = getTrackByTextStream( textStream );
            }
            track.setName( trackName );
            sequence.track.push_back( track );
        }

        parseTempoList( events[0], sequence.tempoList );
        parseTimesigList( events[0], sequence.timesigList );

        // 曲の長さを計算
        sequence.tempoList.updateTempoInfo();
        sequence.timesigList.updateTimesigInfo();
        sequence.updateTotalClocks();
    }

protected:
    /**
     * @brief テキストストリームからイベントの内容を読み込み初期化する
     * @param sr [TextStream] 読み込み対象
     * @param value [int]
     * @param last_line [ByRef<string>] 読み込んだ最後の行が返されます
     * @todo boost::lexical_cast使っている箇所はStringUtil使うようにする
     */
    static TentativeEvent parseEvent( TextStream &sr, int value, std::string &lastLine ){
        TentativeEvent result;
        result.index = value;
        result.type = EventType::UNKNOWN;
        result._singerHandleIndex = -2;
        result._lyricHandleIndex = -1;
        result._vibratoHandleIndex = -1;
        result._noteHeadHandleIndex = -1;
        result.setLength( 0 );
        result.note = 0;
        result.dynamics = 64;
        result.pmBendDepth = 8;
        result.pmBendLength = 0;
        result.pmbPortamentoUse = 0;
        result.demDecGainRate = 50;
        result.demAccent = 50;
        result.vibratoDelay = 0;
        lastLine = sr.readLine();
        while( lastLine.find( "[" ) != 0 ){
            vector<string> spl = StringUtil::explode( "=", lastLine );
            string search = spl[0];
            if( search == "Type" ){
                if( spl[1] == "Anote" ){
                    result.type = EventType::NOTE;
                }else if( spl[1] == "Singer" ){
                    result.type = EventType::SINGER;
                }else if( spl[1] == "Aicon" ){
                    result.type = EventType::ICON;
                }else{
                    result.type = EventType::UNKNOWN;
                }
            }else if( search == "Length" ){
                result.setLength( boost::lexical_cast<tick_t>( spl[1] ) );
            }else if( search == "Note#" ){
                result.note = boost::lexical_cast<int>( spl[1] );
            }else if( search == "Dynamics" ){
                result.dynamics = boost::lexical_cast<int>( spl[1] );
            }else if( search == "PMBendDepth" ){
                result.pmBendDepth = boost::lexical_cast<int>( spl[1] );
            }else if( search == "PMBendLength" ){
                result.pmBendLength = boost::lexical_cast<int>( spl[1] );
            }else if( search == "DEMdecGainRate" ){
                result.demDecGainRate = boost::lexical_cast<int>( spl[1] );
            }else if( search ==  "DEMaccent" ){
                result.demAccent = boost::lexical_cast<int>( spl[1] );
            }else if( search == "LyricHandle" ){
                result._lyricHandleIndex = Handle::getHandleIndexFromString( spl[1] );
            }else if( search == "IconHandle" ){
                result._singerHandleIndex = Handle::getHandleIndexFromString( spl[1] );
            }else if( search == "VibratoHandle" ){
                result._vibratoHandleIndex = Handle::getHandleIndexFromString( spl[1] );
            }else if( search == "VibratoDelay" ){
                result.vibratoDelay = boost::lexical_cast<int>( spl[1] );
            }else if( search == "PMbPortamentoUse" ){
                result.pmbPortamentoUse = boost::lexical_cast<int>( spl[1] );
            }else if( search == "NoteHeadHandle" ){
                result._noteHeadHandleIndex = Handle::getHandleIndexFromString( spl[1] );
            }
            if( ! sr.ready() ){
                break;
            }
            lastLine = sr.readLine();
        }
        return result;
    }

    /**
     * @brief テキストストリームからハンドルの内容を読み込み初期化する
     * @param stream 読み込み元のテキストストリーム
     * @param index <code>index</code> フィールドの値
     * @param lastLine 読み込んだ最後の行。テーブルの ["value"] に文字列が格納される
     * @todo boostでstring->intの変換をやっている箇所を、StringUtilを使うよう変更
     */
    static Handle parseHandle( VSQ_NS::TextStream &stream, int index, std::string &lastLine ){
        TentativeHandle result( HandleType::UNKNOWN );
        result.index = index;

        // default値で埋める
        result.setHandleType( HandleType::VIBRATO );
        result.iconId = "";
        result.ids = "normal";
        result.setLyrics( vector<Lyric>() );
        result.addLyric( Lyric( "" ) );
        result.original = 0;
        result.setCaption( "" );
        result.setLength( 0 );
        result.setStartDepth( 0 );
        result.setStartRate( 0 );
        result.language = 0;
        result.program = 0;
        result.setDuration( 0 );
        result.setDepth( 64 );

        string tmpDepthBPX = "";
        string tmpDepthBPY = "";
        string tmpDepthBPNum = "";

        string tmpRateBPX = "";
        string tmpRateBPY = "";
        string tmpRateBPNum = "";

        string tmpDynBPX = "";
        string tmpDynBPY = "";
        string tmpDynBPNum = "";

        // "["にぶち当たるまで読込む
        lastLine = stream.readLine();
        while( lastLine.find( "[" ) != 0 ){
            vector<string> spl = StringUtil::explode( "=", lastLine );
            string search = spl[0];
            if( search == "Language" ){
                result.setHandleType( HandleType::SINGER );
                result.language = boost::lexical_cast<int>( spl[1] );
            }else if( search == "Program" ){
                result.program = boost::lexical_cast<int>( spl[1] );
            }else if( search == "IconID" ){
                result.iconId = spl[1];
            }else if( search == "IDS" ){
                result.ids = spl[1];
                for( int i = 2; i < spl.size(); i++ ){
                    result.ids = result.ids + "=" + spl[i];
                }
            }else if( search == "Original" ){
                result.original = boost::lexical_cast<int>( spl[1] );
            }else if( search == "Caption" ){
                result.setCaption( spl[1] );
                for( int i = 2; i < spl.size(); i++ ){
                    result.setCaption( result.getCaption() + "=" + spl[i] );
                }
            }else if( search == "Length" ){
                result.setLength( boost::lexical_cast<tick_t>( spl[1] ) );
            }else if( search == "StartDepth" ){
                result.setStartDepth( boost::lexical_cast<int>( spl[1] ) );
            }else if( search == "DepthBPNum" ){
                tmpDepthBPNum = spl[1];
            }else if( search == "DepthBPX" ){
                tmpDepthBPX = spl[1];
            }else if( search == "DepthBPY" ){
                tmpDepthBPY = spl[1];
            }else if( search == "StartRate" ){
                result.setHandleType( HandleType::VIBRATO );
                result.setStartRate( boost::lexical_cast<int>( spl[1] ) );
            }else if( search == "RateBPNum" ){
                tmpRateBPNum = spl[1];
            }else if( search == "RateBPX" ){
                tmpRateBPX = spl[1];
            }else if( search == "RateBPY" ){
                tmpRateBPY = spl[1];
            }else if( search == "Duration" ){
                result.setHandleType( HandleType::NOTE_HEAD );
                result.setDuration( boost::lexical_cast<int>( spl[1] ) );
            }else if( search == "Depth" ){
                result.setDepth( boost::lexical_cast<int>( spl[1] ) );
            }else if( search == "StartDyn" ){
                result.setHandleType( HandleType::DYNAMICS );
                result.setStartDyn( boost::lexical_cast<int>( spl[1] ) );
            }else if( search == "EndDyn" ){
                result.setHandleType( HandleType::DYNAMICS );
                result.setEndDyn( boost::lexical_cast<int>( spl[1] ) );
            }else if( search == "DynBPNum" ){
                tmpDynBPNum = spl[1];
            }else if( search == "DynBPX" ){
                tmpDynBPX = spl[1];
            }else if( search == "DynBPY" ){
                tmpDynBPY = spl[1];
            }else if( search.find( "L" ) == 0 && search.size() >= 2 ){
                int index = boost::lexical_cast<int>( search.substr( 1, 1 ) );
                Lyric lyric( spl[1] );
                result.setHandleType( HandleType::LYRIC );
                if( result.getLyricCount() <= index + 1 ){
                    int amount = index + 1 - result.getLyricCount();
                    for( int i = 0; i < amount; i++ ){
                        result.addLyric( Lyric( "", "" ) );
                    }
                }
                result.setLyricAt( index, lyric );
            }
            if( ! stream.ready() ){
                break;
            }
            lastLine = stream.readLine();
        }

        // RateBPX, RateBPYの設定
        if( result.getHandleType() == HandleType::VIBRATO ){
            if( tmpRateBPNum != "" ){
                result.setRateBP( VibratoBPList( tmpRateBPNum, tmpRateBPX, tmpRateBPY ) );
            }else{
                result.setRateBP( VibratoBPList() );
            }

            // DepthBPX, DepthBPYの設定
            if( tmpDepthBPNum != "" ){
                result.setDepthBP( VibratoBPList( tmpDepthBPNum, tmpDepthBPX, tmpDepthBPY ) );
            }else{
                result.setDepthBP( VibratoBPList() );
            }
        }else{
            result.setDepthBP( VibratoBPList() );
            result.setRateBP( VibratoBPList() );
        }

        if( tmpDynBPNum != "" ){
            result.setDynBP( VibratoBPList( tmpDynBPNum, tmpDynBPX, tmpDynBPY ) );
        }else{
            result.setDynBP( VibratoBPList() );
        }
        return result;
    }

private:
    /**
     * @brief MIDIイベントリストから拍子変更イベントを取り出し、TimesigList のインスタンスを初期化する
     */
    static void parseTimesigList( const vector<MidiEvent> &midi_event, TimesigList &timesigList ){
        timesigList.clear();
        int dnomi = 4;
        int numer = 4;
        int count = -1;
        for( int j = 0; j < midi_event.size(); j++ ){
            if( midi_event[j].firstByte == 0xff && midi_event[j].data.size() >= 5 && midi_event[j].data[0] == 0x58 ){
                count++;
                numer = midi_event[j].data[1];
                dnomi = 1;
                for( int i = 0; i < midi_event[j].data[2]; i++ ){
                    dnomi = dnomi * 2;
                }
                if( count == 0 ){
                    if( midi_event[j].clock == 0 ){
                        timesigList.push( Timesig( numer, dnomi, 0 ) );
                    }else{
                        timesigList.push( Timesig( 4, 4, 0 ) );
                        timesigList.push( Timesig( numer, dnomi, (int)midi_event[j].clock / (480 * 4) ) );
                        count++;
                    }
                }else{
                    int numerator = timesigList.get( count - 1 ).numerator;
                    int denominator = timesigList.get( count - 1 ).denominator;
                    int clock = timesigList.get( count - 1 ).clock;
                    int bar_count = timesigList.get( count - 1 ).barCount;
                    int dif = 480 * 4 / denominator * numerator;//1小節が何クロックか？
                    bar_count += ((int)midi_event[j].clock - clock) / dif;
                    timesigList.push( Timesig( numer, dnomi, bar_count ) );
                }
            }
        }
        timesigList.updateTimesigInfo();
    }

    /**
     * @brief MIDIイベントリストからテンポ変更イベントを取り出し、TempoList のインスタンスを初期化する
     */
    static void parseTempoList( const vector<MidiEvent> &midi_event, TempoList &tempoList ){
        tempoList.clear();
        int prev_tempo = 500000;
        tick_t prev_index = 0;
        int count = -1;
        int midi_event_size = midi_event.size();
        for( int j = 0; j < midi_event_size; j++ ){
            MidiEvent itemj = midi_event[j];
            if( itemj.firstByte == 0xff && itemj.data.size() >= 4 && itemj.data[0] == 0x51 ){
                count++;
                if( count == 0 && itemj.clock != 0 ){
                    tempoList.push( Tempo( 0, prev_tempo ) );
                }
                int current_tempo = itemj.data[1] << 16 | itemj.data[2] << 8 | itemj.data[3];
                tick_t current_index = midi_event[j].clock;
                tempoList.push( Tempo( current_index, current_tempo ) );
                prev_tempo = current_tempo;
                prev_index = current_index;
            }
        }
        tempoList.updateTempoInfo();
    }

    /**
     * @brief MIDI イベントのリストから、VOCALOIDメタテキストとトラック名を取得する
     * @param midi_event [Array<MidiEvent>]
     * @param encoding [string]
     * @param [out] sw 取得したメタテキストの格納先
     * @param [out] track_name 取得したトラック名
     * @todo midi_event を midiEventList に名前を変える
     * @todo swをstreamに名前変える
     */
    static void getMetatextByMidiEventList( const std::vector<VSQ_NS::MidiEvent> &midi_event, const std::string &encoding, TextStream &sw, string &track_name ){
        int count = midi_event.size();
        vector<int> buffer;
        for( int i = 0; i < count; i++ ){
            MidiEvent item = midi_event[i];
            if( item.firstByte == 0xff && item.data.size() > 0 ){
                // meta textを抽出
                int type = item.data[0];
                if( type == 0x01 || type == 0x03 ){
                    if( type == 0x01 ){
                        int colon_count = 0;
                        for( int j = 1; j < item.data.size(); j++ ){
                            int d = item.data[j];
                            if( d == 0x3a ){
                                colon_count++;
                                if( colon_count <= 2 ){
                                    continue;
                                }
                            }
                            if( colon_count < 2 ){
                                continue;
                            }
                            buffer.push_back( d );
                        }

                        std::vector<int>::iterator itr_0x0a = std::find( buffer.begin(), buffer.end(), 0x0a );
                        while( itr_0x0a != buffer.end() ){
                            std::ostringstream cpy;
                            int index_0x0a = itr_0x0a - buffer.begin();
                            for( int j = 0; j < index_0x0a; j++ ){
                                cpy << (char)(0xff & buffer[0]);
                                buffer.erase( buffer.begin() );
                            }

                            string line = CP932Converter::convertToUTF8( cpy.str() );
                            sw.writeLine( line );
                            buffer.erase( buffer.begin() );
                            itr_0x0a = std::find( buffer.begin(), buffer.end(), 0x0a );
                        }
                    }else{
                        for( int j = 1; j < item.data.size(); j++ ){
                            buffer.push_back( item.data[j] );
                        }
                        int c = buffer.size();
                        ostringstream d;
                        for( int j = 0; j < c; j++ ){
                            d << (char)(0xff & buffer[j]);
                        }
                        track_name = CP932Converter::convertToUTF8( d.str() );
                        buffer.clear();
                    }
                }
            }else{
                continue;
            }
        }

        int remain = buffer.size();
        if( 0 < remain ){
            std::ostringstream cpy;
            for( int j = 0; j < remain; j++ ){
                cpy << (char)(0xff & buffer[j]);
            }
            std::string line = VSQ_NS::CP932Converter::convertToUTF8( cpy.str() );
            sw.writeLine( line );
        }

        sw.setPointer( -1 );
    }

    /**
     * @brief メタテキストが格納されたストリームから、トラックを初期化する
     * @param 読み込むストリーム
     * @param [out] master 読み込まれた Master 情報
     * @param [out] mixer 読み込まれた Mixer 情報
     */
    static Track getTrackByTextStream( TextStream &stream, Master *master = 0, Mixer *mixer = 0 ){
        std::map<int, tick_t> t_event_list;
        std::map<int, VSQ_NS::Event *> eventList;
        std::map<int, VSQ_NS::Handle> handleList;
        vector<VSQ_NS::Event *> temporaryEventList;

        TentativeTrack result;

        std::string last_line = stream.readLine();
        while( 1 ){
            // TextMemoryStreamから順次読込み
            if( last_line.length() == 0 ){
                break;
            }
            if( last_line == "[Common]" ){
                result.setCommon( VSQ_NS::Common( stream, last_line ) );
            }else if( last_line == "[Master]" && master != 0 ){
                *master = VSQ_NS::Master( stream, last_line );
            }else if( last_line == "[Mixer]" && mixer != 0 ){
                *mixer = VSQ_NS::Mixer( stream, last_line );
            }else if( last_line == "[EventList]" ){
                last_line = stream.readLine();
                while( last_line.find( "[" ) != 0 ){
                    std::vector<std::string> spl2 = StringUtil::explode( "=", last_line );
                    tick_t clock = StringUtil::parseInt( spl2[0] );
                    int id_number = -1;
                    if( spl2[1] != "EOS" ){
                        std::vector<std::string> ids = StringUtil::explode( ",", spl2[1] );
                        for( int i = 0; i < ids.size(); i++ ){
                            std::vector<std::string> spl3 = StringUtil::explode( "#", ids[i] );
                            id_number = StringUtil::parseInt( spl3[1] );
                            t_event_list.insert( make_pair( id_number, clock ) );
                        }
                    }else{
                        t_event_list.insert( make_pair( -1, clock ) );
                    }
                    if( ! stream.ready() ){
                        break;
                    }else{
                        last_line = stream.readLine();
                    }
                }
            }else if( last_line == "[PitchBendBPList]" ){
                last_line = result.getCurve( "pit" )->appendFromText( stream );
            }else if( last_line == "[PitchBendSensBPList]" ){
                last_line = result.getCurve( "pbs" )->appendFromText( stream );
            }else if( last_line == "[DynamicsBPList]" ){
                last_line = result.getCurve( "dyn" )->appendFromText( stream );
            }else if( last_line == "[EpRResidualBPList]" ){
                last_line = result.getCurve( "bre" )->appendFromText( stream );
            }else if( last_line == "[EpRESlopeBPList]" ){
                last_line = result.getCurve( "bri" )->appendFromText( stream );
            }else if( last_line == "[EpRESlopeDepthBPList]" ){
                last_line = result.getCurve( "cle" )->appendFromText( stream );
            }else if( last_line == "[EpRSineBPList]" ){
                last_line = result.getCurve( "harmonics" )->appendFromText( stream );
            }else if( last_line == "[VibTremDepthBPList]" ){
                last_line = result.getCurve( "fx2depth" )->appendFromText( stream );
            }else if( last_line == "[Reso1FreqBPList]" ){
                last_line = result.getCurve( "reso1FreqBPList" )->appendFromText( stream );
            }else if( last_line == "[Reso2FreqBPList]" ){
                last_line = result.getCurve( "reso2FreqBPList" )->appendFromText( stream );
            }else if( last_line == "[Reso3FreqBPList]" ){
                last_line = result.getCurve( "reso3FreqBPList" )->appendFromText( stream );
            }else if( last_line == "[Reso4FreqBPList]" ){
                last_line = result.getCurve( "reso4FreqBPList" )->appendFromText( stream );
            }else if( last_line == "[Reso1BWBPList]" ){
                last_line = result.getCurve( "reso1BWBPList" )->appendFromText( stream );
            }else if( last_line == "[Reso2BWBPList]" ){
                last_line = result.getCurve( "reso2BWBPList" )->appendFromText( stream );
            }else if( last_line == "[Reso3BWBPList]" ){
                last_line = result.getCurve( "reso3BWBPList" )->appendFromText( stream );
            }else if( last_line == "[Reso4BWBPList]" ){
                last_line = result.getCurve( "reso4BWBPList" )->appendFromText( stream );
            }else if( last_line == "[Reso1AmpBPList]" ){
                last_line = result.getCurve( "reso1AmpBPList" )->appendFromText( stream );
            }else if( last_line == "[Reso2AmpBPList]" ){
                last_line = result.getCurve( "reso2AmpBPList" )->appendFromText( stream );
            }else if( last_line == "[Reso3AmpBPList]" ){
                last_line = result.getCurve( "reso3AmpBPList" )->appendFromText( stream );
            }else if( last_line == "[Reso4AmpBPList]" ){
                last_line = result.getCurve( "reso4AmpBPList" )->appendFromText( stream );
            }else if( last_line == "[GenderFactorBPList]" ){
                last_line = result.getCurve( "gen" )->appendFromText( stream );
            }else if( last_line == "[PortamentoTimingBPList]" ){
                last_line = result.getCurve( "por" )->appendFromText( stream );
            }else if( last_line == "[OpeningBPList]" ){
                last_line = result.getCurve( "ope" )->appendFromText( stream );
            }else{
                std::string buffer = last_line;
                buffer = StringUtil::replace( buffer, "[", "" );
                buffer = StringUtil::replace( buffer, "]", "" );
                std::vector<std::string> spl = StringUtil::explode( "#", buffer );
                int index = StringUtil::parseInt( spl[1] );
                if( last_line.find( "[ID#" ) == 0 ){
                    Event *item = new Event();
                    *item = parseEvent( stream, index, last_line );
                    temporaryEventList.push_back( item );
                    eventList.insert( make_pair( index, item ) );
                }else if( last_line.find( "[h#" ) == 0 ){
                    handleList.insert( make_pair( index, parseHandle( stream, index, last_line ) ) );
                }
            }

            if( ! stream.ready() ){
                break;
            }
        }

        // まずhandleをidに埋め込み
        for( map<int, Event *>::iterator i = eventList.begin(); i != eventList.end(); ++i ){
            VSQ_NS::Event *id = i->second;
            if( handleList.find( id->_singerHandleIndex ) != handleList.end() ){
                if( id->type == VSQ_NS::EventType::SINGER ){
                    id->singerHandle = handleList[id->_singerHandleIndex];
                }else if( id->type == VSQ_NS::EventType::ICON ){
                    id->iconDynamicsHandle = handleList[id->_singerHandleIndex];
                }
            }
            if( handleList.find( id->_lyricHandleIndex ) != handleList.end() ){
                id->lyricHandle = handleList[id->_lyricHandleIndex];
            }
            if( handleList.find( id->_vibratoHandleIndex ) != handleList.end() ){
                id->vibratoHandle = handleList[id->_vibratoHandleIndex];
            }
            if( handleList.find( id->_noteHeadHandleIndex ) != handleList.end() ){
                id->noteHeadHandle = handleList[id->_noteHeadHandleIndex];
            }
        }

        // idをeventListに埋め込み
        Event::List *events = result.getEvents();
        events->clear();
        int count = 0;
        for( map<int, tick_t>::iterator i = t_event_list.begin(); i != t_event_list.end(); ++i ){
            int id_number = i->first;
            tick_t clock = i->second;
            if( eventList.find( id_number ) != eventList.end() ){
                count++;
                Event item = *eventList[id_number];
                item.clock = clock;
                events->add( item, count );
            }
        }
        events->sort();

        for( vector<Event *>::iterator i = temporaryEventList.begin(); i != temporaryEventList.end(); ++i ){
            delete (*i);
        }

        return (Track)result;
    }
};

VSQ_END_NAMESPACE

#endif
