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
    public:
        int singerHandleIndex;
        int lyricHandleIndex;
        int vibratoHandleIndex;
        int noteHeadHandleIndex;
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
    TentativeEvent parseEvent( TextStream &stream, std::string &lastLine ){
        TentativeEvent result;
        result.type = EventType::UNKNOWN;
        result.singerHandleIndex = -2;
        result.lyricHandleIndex = -1;
        result.vibratoHandleIndex = -1;
        result.noteHeadHandleIndex = -1;
        result.setLength( 0 );
        result.note = 0;
        result.dynamics = 64;
        result.pmBendDepth = 8;
        result.pmBendLength = 0;
        result.pmbPortamentoUse = 0;
        result.demDecGainRate = 50;
        result.demAccent = 50;
        result.vibratoDelay = 0;
        lastLine = stream.readLine();
        while( lastLine.find( "[" ) != 0 ){
            vector<string> parameters = StringUtil::explode( "=", lastLine );
            string search = parameters[0];
            if( search == "Type" ){
                if( parameters[1] == "Anote" ){
                    result.type = EventType::NOTE;
                }else if( parameters[1] == "Singer" ){
                    result.type = EventType::SINGER;
                }else if( parameters[1] == "Aicon" ){
                    result.type = EventType::ICON;
                }else{
                    result.type = EventType::UNKNOWN;
                }
            }else if( search == "Length" ){
                result.setLength( boost::lexical_cast<tick_t>( parameters[1] ) );
            }else if( search == "Note#" ){
                result.note = boost::lexical_cast<int>( parameters[1] );
            }else if( search == "Dynamics" ){
                result.dynamics = boost::lexical_cast<int>( parameters[1] );
            }else if( search == "PMBendDepth" ){
                result.pmBendDepth = boost::lexical_cast<int>( parameters[1] );
            }else if( search == "PMBendLength" ){
                result.pmBendLength = boost::lexical_cast<int>( parameters[1] );
            }else if( search == "DEMdecGainRate" ){
                result.demDecGainRate = boost::lexical_cast<int>( parameters[1] );
            }else if( search ==  "DEMaccent" ){
                result.demAccent = boost::lexical_cast<int>( parameters[1] );
            }else if( search == "LyricHandle" ){
                result.lyricHandleIndex = Handle::getHandleIndexFromString( parameters[1] );
            }else if( search == "IconHandle" ){
                result.singerHandleIndex = Handle::getHandleIndexFromString( parameters[1] );
            }else if( search == "VibratoHandle" ){
                result.vibratoHandleIndex = Handle::getHandleIndexFromString( parameters[1] );
            }else if( search == "VibratoDelay" ){
                result.vibratoDelay = boost::lexical_cast<int>( parameters[1] );
            }else if( search == "PMbPortamentoUse" ){
                result.pmbPortamentoUse = boost::lexical_cast<int>( parameters[1] );
            }else if( search == "NoteHeadHandle" ){
                result.noteHeadHandleIndex = Handle::getHandleIndexFromString( parameters[1] );
            }
            if( ! stream.ready() ){
                break;
            }
            lastLine = stream.readLine();
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
    Handle parseHandle( VSQ_NS::TextStream &stream, int index, std::string &lastLine ){
        TentativeHandle result( HandleType::UNKNOWN );
        result.index = index;

        // default値で埋める
        result.setHandleType( HandleType::VIBRATO );
        result.iconId = "";
        result.ids = "normal";
        result.setLyrics( vector<Lyric>() );
        result.addLyric( Lyric( "" ) );
        result.original = 0;
        result.caption = "";
        result.setLength( 0 );
        result.startDepth = 0;
        result.startRate = 0;
        result.language = 0;
        result.program = 0;
        result.duration = 0;
        result.depth = 64;

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
            vector<string> parameters = StringUtil::explode( "=", lastLine );
            string search = parameters[0];
            if( search == "Language" ){
                result.setHandleType( HandleType::SINGER );
                result.language = boost::lexical_cast<int>( parameters[1] );
            }else if( search == "Program" ){
                result.program = boost::lexical_cast<int>( parameters[1] );
            }else if( search == "IconID" ){
                result.iconId = parameters[1];
            }else if( search == "IDS" ){
                result.ids = parameters[1];
                for( int i = 2; i < parameters.size(); i++ ){
                    result.ids = result.ids + "=" + parameters[i];
                }
            }else if( search == "Original" ){
                result.original = boost::lexical_cast<int>( parameters[1] );
            }else if( search == "Caption" ){
                result.caption = parameters[1];
                for( int i = 2; i < parameters.size(); i++ ){
                    result.caption = result.caption + "=" + parameters[i];
                }
            }else if( search == "Length" ){
                result.setLength( boost::lexical_cast<tick_t>( parameters[1] ) );
            }else if( search == "StartDepth" ){
                result.startDepth = boost::lexical_cast<int>( parameters[1] );
            }else if( search == "DepthBPNum" ){
                tmpDepthBPNum = parameters[1];
            }else if( search == "DepthBPX" ){
                tmpDepthBPX = parameters[1];
            }else if( search == "DepthBPY" ){
                tmpDepthBPY = parameters[1];
            }else if( search == "StartRate" ){
                result.setHandleType( HandleType::VIBRATO );
                result.startRate = boost::lexical_cast<int>( parameters[1] );
            }else if( search == "RateBPNum" ){
                tmpRateBPNum = parameters[1];
            }else if( search == "RateBPX" ){
                tmpRateBPX = parameters[1];
            }else if( search == "RateBPY" ){
                tmpRateBPY = parameters[1];
            }else if( search == "Duration" ){
                result.setHandleType( HandleType::NOTE_HEAD );
                result.duration = boost::lexical_cast<int>( parameters[1] );
            }else if( search == "Depth" ){
                result.depth = boost::lexical_cast<int>( parameters[1] );
            }else if( search == "StartDyn" ){
                result.setHandleType( HandleType::DYNAMICS );
                result.startDyn = boost::lexical_cast<int>( parameters[1] );
            }else if( search == "EndDyn" ){
                result.setHandleType( HandleType::DYNAMICS );
                result.endDyn = boost::lexical_cast<int>( parameters[1] );
            }else if( search == "DynBPNum" ){
                tmpDynBPNum = parameters[1];
            }else if( search == "DynBPX" ){
                tmpDynBPX = parameters[1];
            }else if( search == "DynBPY" ){
                tmpDynBPY = parameters[1];
            }else if( search.find( "L" ) == 0 && search.size() >= 2 ){
                int index = boost::lexical_cast<int>( search.substr( 1, 1 ) );
                Lyric lyric( parameters[1] );
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
                result.rateBP = VibratoBPList( tmpRateBPNum, tmpRateBPX, tmpRateBPY );
            }else{
                result.rateBP = VibratoBPList();
            }

            // DepthBPX, DepthBPYの設定
            if( tmpDepthBPNum != "" ){
                result.depthBP = VibratoBPList( tmpDepthBPNum, tmpDepthBPX, tmpDepthBPY );
            }else{
                result.depthBP = VibratoBPList();
            }
        }else{
            result.depthBP = VibratoBPList();
            result.rateBP = VibratoBPList();
        }

        if( tmpDynBPNum != "" ){
            result.dynBP = VibratoBPList( tmpDynBPNum, tmpDynBPX, tmpDynBPY );
        }else{
            result.dynBP = VibratoBPList();
        }
        return result;
    }

private:
    /**
     * @brief MIDIイベントリストから拍子変更イベントを取り出し、TimesigList のインスタンスを初期化する
     */
    void parseTimesigList( const vector<MidiEvent> &midiEventList, TimesigList &timesigList ){
        timesigList.clear();
        int dnominator = 4;
        int numerator = 4;
        int count = -1;
        for( int j = 0; j < midiEventList.size(); j++ ){
            if( midiEventList[j].firstByte == 0xff && midiEventList[j].data.size() >= 5 && midiEventList[j].data[0] == 0x58 ){
                count++;
                numerator = midiEventList[j].data[1];
                dnominator = 1;
                for( int i = 0; i < midiEventList[j].data[2]; i++ ){
                    dnominator = dnominator * 2;
                }
                if( count == 0 ){
                    if( midiEventList[j].clock == 0 ){
                        timesigList.push( Timesig( numerator, dnominator, 0 ) );
                    }else{
                        timesigList.push( Timesig( 4, 4, 0 ) );
                        timesigList.push( Timesig( numerator, dnominator, (int)midiEventList[j].clock / (480 * 4) ) );
                        count++;
                    }
                }else{
                    int lastNumerator = timesigList.get( count - 1 ).numerator;
                    int lastDenominator = timesigList.get( count - 1 ).denominator;
                    tick_t clock = timesigList.get( count - 1 ).getClock();
                    int barCount = timesigList.get( count - 1 ).barCount;
                    int dif = 480 * 4 / lastDenominator * lastNumerator;//1小節が何クロックか？
                    barCount += ((int)midiEventList[j].clock - clock) / dif;
                    timesigList.push( Timesig( numerator, dnominator, barCount ) );
                }
            }
        }
    }

    /**
     * @brief MIDIイベントリストからテンポ変更イベントを取り出し、TempoList のインスタンスを初期化する
     */
    static void parseTempoList( const vector<MidiEvent> &midiEventList, TempoList &tempoList ){
        tempoList.clear();
        int lastTempo = 500000;
        tick_t lastClock = 0;
        int count = -1;
        int listSize = midiEventList.size();
        for( int j = 0; j < listSize; j++ ){
            MidiEvent item = midiEventList[j];
            if( item.firstByte == 0xff && item.data.size() >= 4 && item.data[0] == 0x51 ){
                count++;
                if( count == 0 && item.clock != 0 ){
                    tempoList.push( Tempo( 0, lastTempo ) );
                }
                int currentTempo = item.data[1] << 16 | item.data[2] << 8 | item.data[3];
                tick_t currentClock = midiEventList[j].clock;
                tempoList.push( Tempo( currentClock, currentTempo ) );
                lastTempo = currentTempo;
                lastClock = currentClock;
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
     */
    void getMetatextByMidiEventList( const std::vector<VSQ_NS::MidiEvent> &midiEventList, const std::string &encoding, TextStream &stream, string &trackName ){
        int count = midiEventList.size();
        vector<int> buffer;
        for( int i = 0; i < count; i++ ){
            MidiEvent item = midiEventList[i];
            if( item.firstByte == 0xff && item.data.size() > 0 ){
                // meta textを抽出
                int type = item.data[0];
                if( type == 0x01 || type == 0x03 ){
                    if( type == 0x01 ){
                        int colonCount = 0;
                        for( int j = 1; j < item.data.size(); j++ ){
                            int d = item.data[j];
                            if( d == 0x3a ){
                                colonCount++;
                                if( colonCount <= 2 ){
                                    continue;
                                }
                            }
                            if( colonCount < 2 ){
                                continue;
                            }
                            buffer.push_back( d );
                        }

                        std::vector<int>::iterator lineFeedPosition = std::find( buffer.begin(), buffer.end(), 0x0a );
                        while( lineFeedPosition != buffer.end() ){
                            std::ostringstream copy;
                            int lineFeedIndex = lineFeedPosition - buffer.begin();
                            for( int j = 0; j < lineFeedIndex; j++ ){
                                copy << (char)(0xff & buffer[0]);
                                buffer.erase( buffer.begin() );
                            }

                            string line = CP932Converter::convertToUTF8( copy.str() );
                            stream.writeLine( line );
                            buffer.erase( buffer.begin() );
                            lineFeedPosition = std::find( buffer.begin(), buffer.end(), 0x0a );
                        }
                    }else{
                        for( int j = 1; j < item.data.size(); j++ ){
                            buffer.push_back( item.data[j] );
                        }
                        int c = buffer.size();
                        ostringstream copy;
                        for( int j = 0; j < c; j++ ){
                            copy << (char)(0xff & buffer[j]);
                        }
                        trackName = CP932Converter::convertToUTF8( copy.str() );
                        buffer.clear();
                    }
                }
            }else{
                continue;
            }
        }

        int remain = buffer.size();
        if( 0 < remain ){
            std::ostringstream copy;
            for( int j = 0; j < remain; j++ ){
                copy << (char)(0xff & buffer[j]);
            }
            std::string line = VSQ_NS::CP932Converter::convertToUTF8( copy.str() );
            stream.writeLine( line );
        }

        stream.setPointer( -1 );
    }

    /**
     * @brief メタテキストが格納されたストリームから、トラックを初期化する
     * @param 読み込むストリーム
     * @param [out] master 読み込まれた Master 情報
     * @param [out] mixer 読み込まれた Mixer 情報
     */
    Track getTrackByTextStream( TextStream &stream, Master *master = 0, Mixer *mixer = 0 ){
        std::map<int, tick_t> eventClockMap;
        std::map<int, TentativeEvent *> eventIdMap;
        std::map<int, VSQ_NS::Handle> handleIdMap;
        vector<VSQ_NS::Event *> temporaryEventList;

        TentativeTrack result;

        std::string lastLine = stream.readLine();
        while( 1 ){
            // TextMemoryStreamから順次読込み
            if( lastLine.length() == 0 ){
                break;
            }
            if( lastLine == "[Common]" ){
                result.setCommon( VSQ_NS::Common( stream, lastLine ) );
            }else if( lastLine == "[Master]" && master != 0 ){
                *master = VSQ_NS::Master( stream, lastLine );
            }else if( lastLine == "[Mixer]" && mixer != 0 ){
                *mixer = VSQ_NS::Mixer( stream, lastLine );
            }else if( lastLine == "[EventList]" ){
                lastLine = stream.readLine();
                while( lastLine.find( "[" ) != 0 ){
                    std::vector<std::string> parameters = StringUtil::explode( "=", lastLine );
                    tick_t clock = StringUtil::parseInt( parameters[0] );
                    int id = -1;
                    if( parameters[1] != "EOS" ){
                        std::vector<std::string> idList = StringUtil::explode( ",", parameters[1] );
                        for( int i = 0; i < idList.size(); i++ ){
                            std::vector<std::string> idParameters = StringUtil::explode( "#", idList[i] );
                            id = StringUtil::parseInt( idParameters[1] );
                            eventClockMap.insert( make_pair( id, clock ) );
                        }
                    }else{
                        eventClockMap.insert( make_pair( -1, clock ) );
                    }
                    if( ! stream.ready() ){
                        break;
                    }else{
                        lastLine = stream.readLine();
                    }
                }
            }else if( lastLine == "[PitchBendBPList]" ){
                lastLine = result.getCurve( "pit" )->appendFromText( stream );
            }else if( lastLine == "[PitchBendSensBPList]" ){
                lastLine = result.getCurve( "pbs" )->appendFromText( stream );
            }else if( lastLine == "[DynamicsBPList]" ){
                lastLine = result.getCurve( "dyn" )->appendFromText( stream );
            }else if( lastLine == "[EpRResidualBPList]" ){
                lastLine = result.getCurve( "bre" )->appendFromText( stream );
            }else if( lastLine == "[EpRESlopeBPList]" ){
                lastLine = result.getCurve( "bri" )->appendFromText( stream );
            }else if( lastLine == "[EpRESlopeDepthBPList]" ){
                lastLine = result.getCurve( "cle" )->appendFromText( stream );
            }else if( lastLine == "[EpRSineBPList]" ){
                lastLine = result.getCurve( "harmonics" )->appendFromText( stream );
            }else if( lastLine == "[VibTremDepthBPList]" ){
                lastLine = result.getCurve( "fx2depth" )->appendFromText( stream );
            }else if( lastLine == "[Reso1FreqBPList]" ){
                lastLine = result.getCurve( "reso1FreqBPList" )->appendFromText( stream );
            }else if( lastLine == "[Reso2FreqBPList]" ){
                lastLine = result.getCurve( "reso2FreqBPList" )->appendFromText( stream );
            }else if( lastLine == "[Reso3FreqBPList]" ){
                lastLine = result.getCurve( "reso3FreqBPList" )->appendFromText( stream );
            }else if( lastLine == "[Reso4FreqBPList]" ){
                lastLine = result.getCurve( "reso4FreqBPList" )->appendFromText( stream );
            }else if( lastLine == "[Reso1BWBPList]" ){
                lastLine = result.getCurve( "reso1BWBPList" )->appendFromText( stream );
            }else if( lastLine == "[Reso2BWBPList]" ){
                lastLine = result.getCurve( "reso2BWBPList" )->appendFromText( stream );
            }else if( lastLine == "[Reso3BWBPList]" ){
                lastLine = result.getCurve( "reso3BWBPList" )->appendFromText( stream );
            }else if( lastLine == "[Reso4BWBPList]" ){
                lastLine = result.getCurve( "reso4BWBPList" )->appendFromText( stream );
            }else if( lastLine == "[Reso1AmpBPList]" ){
                lastLine = result.getCurve( "reso1AmpBPList" )->appendFromText( stream );
            }else if( lastLine == "[Reso2AmpBPList]" ){
                lastLine = result.getCurve( "reso2AmpBPList" )->appendFromText( stream );
            }else if( lastLine == "[Reso3AmpBPList]" ){
                lastLine = result.getCurve( "reso3AmpBPList" )->appendFromText( stream );
            }else if( lastLine == "[Reso4AmpBPList]" ){
                lastLine = result.getCurve( "reso4AmpBPList" )->appendFromText( stream );
            }else if( lastLine == "[GenderFactorBPList]" ){
                lastLine = result.getCurve( "gen" )->appendFromText( stream );
            }else if( lastLine == "[PortamentoTimingBPList]" ){
                lastLine = result.getCurve( "por" )->appendFromText( stream );
            }else if( lastLine == "[OpeningBPList]" ){
                lastLine = result.getCurve( "ope" )->appendFromText( stream );
            }else{
                std::string buffer = lastLine;
                buffer = StringUtil::replace( buffer, "[", "" );
                buffer = StringUtil::replace( buffer, "]", "" );
                std::vector<std::string> parameters = StringUtil::explode( "#", buffer );
                int index = StringUtil::parseInt( parameters[1] );
                if( lastLine.find( "[ID#" ) == 0 ){
                    TentativeEvent *item = new TentativeEvent();
                    *item = parseEvent( stream, lastLine );
                    temporaryEventList.push_back( item );
                    eventIdMap.insert( make_pair( index, item ) );
                }else if( lastLine.find( "[h#" ) == 0 ){
                    handleIdMap.insert( make_pair( index, parseHandle( stream, index, lastLine ) ) );
                }
            }

            if( ! stream.ready() ){
                break;
            }
        }

        // まずhandleをidに埋め込み
        for( map<int, TentativeEvent *>::iterator i = eventIdMap.begin(); i != eventIdMap.end(); ++i ){
            TentativeEvent *id = i->second;
            if( handleIdMap.find( id->singerHandleIndex ) != handleIdMap.end() ){
                if( id->type == VSQ_NS::EventType::SINGER ){
                    id->singerHandle = handleIdMap[id->singerHandleIndex];
                }else if( id->type == VSQ_NS::EventType::ICON ){
                    id->iconDynamicsHandle = handleIdMap[id->singerHandleIndex];
                }
            }
            if( handleIdMap.find( id->lyricHandleIndex ) != handleIdMap.end() ){
                id->lyricHandle = handleIdMap[id->lyricHandleIndex];
            }
            if( handleIdMap.find( id->vibratoHandleIndex ) != handleIdMap.end() ){
                id->vibratoHandle = handleIdMap[id->vibratoHandleIndex];
            }
            if( handleIdMap.find( id->noteHeadHandleIndex ) != handleIdMap.end() ){
                id->noteHeadHandle = handleIdMap[id->noteHeadHandleIndex];
            }
        }

        // idをeventListに埋め込み
        Event::List *events = result.getEvents();
        events->clear();
        int count = 0;
        for( map<int, tick_t>::iterator i = eventClockMap.begin(); i != eventClockMap.end(); ++i ){
            int id = i->first;
            tick_t clock = i->second;
            if( eventIdMap.find( id ) != eventIdMap.end() ){
                count++;
                Event item = *eventIdMap[id];
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
