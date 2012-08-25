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
private:
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
     * @todo private メソッド。VSQFileReaderに移動する
     * @todo sr を stream にリネームする
     */
    static Track getTrackByTextStream( TextStream &sr, Master *master = 0, Mixer *mixer = 0 ){
        std::map<int, tick_t> t_event_list;  //Vector<ValuePair<Integer, Integer>> t_event_list = new Vector<ValuePair<Integer, Integer>>();
        //TODO: __id -> __eventListなどと名前を変える
        std::map<int, VSQ_NS::Event *> __id;//TreeMap<Integer, VsqID> __id = new TreeMap<Integer, VsqID>();
        std::map<int, VSQ_NS::Handle> __handle; //TreeMap<Integer, VsqHandle> __handle = new TreeMap<Integer, VsqHandle>();
        vector<VSQ_NS::Event *> temporaryEventList;

        TentativeTrack result;

        std::string last_line = sr.readLine();
        while( 1 ){
            // TextMemoryStreamから順次読込み
            if( last_line.length() == 0 ){
                break;
            }
            if( last_line == "[Common]" ){
                result.setCommon( VSQ_NS::Common( sr, last_line ) );
            }else if( last_line == "[Master]" && master != 0 ){
                *master = VSQ_NS::Master( sr, last_line );
            }else if( last_line == "[Mixer]" && mixer != 0 ){
                *mixer = VSQ_NS::Mixer( sr, last_line );
            }else if( last_line == "[EventList]" ){
                last_line = sr.readLine();
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
                    if( ! sr.ready() ){
                        break;
                    }else{
                        last_line = sr.readLine();
                    }
                }
            }else if( last_line == "[PitchBendBPList]" ){
                last_line = result.getCurve( "pit" )->appendFromText( sr );
            }else if( last_line == "[PitchBendSensBPList]" ){
                last_line = result.getCurve( "pbs" )->appendFromText( sr );
            }else if( last_line == "[DynamicsBPList]" ){
                last_line = result.getCurve( "dyn" )->appendFromText( sr );
            }else if( last_line == "[EpRResidualBPList]" ){
                last_line = result.getCurve( "bre" )->appendFromText( sr );
            }else if( last_line == "[EpRESlopeBPList]" ){
                last_line = result.getCurve( "bri" )->appendFromText( sr );
            }else if( last_line == "[EpRESlopeDepthBPList]" ){
                last_line = result.getCurve( "cle" )->appendFromText( sr );
            }else if( last_line == "[EpRSineBPList]" ){
                last_line = result.getCurve( "harmonics" )->appendFromText( sr );
            }else if( last_line == "[VibTremDepthBPList]" ){
                last_line = result.getCurve( "fx2depth" )->appendFromText( sr );
            }else if( last_line == "[Reso1FreqBPList]" ){
                last_line = result.getCurve( "reso1FreqBPList" )->appendFromText( sr );
            }else if( last_line == "[Reso2FreqBPList]" ){
                last_line = result.getCurve( "reso2FreqBPList" )->appendFromText( sr );
            }else if( last_line == "[Reso3FreqBPList]" ){
                last_line = result.getCurve( "reso3FreqBPList" )->appendFromText( sr );
            }else if( last_line == "[Reso4FreqBPList]" ){
                last_line = result.getCurve( "reso4FreqBPList" )->appendFromText( sr );
            }else if( last_line == "[Reso1BWBPList]" ){
                last_line = result.getCurve( "reso1BWBPList" )->appendFromText( sr );
            }else if( last_line == "[Reso2BWBPList]" ){
                last_line = result.getCurve( "reso2BWBPList" )->appendFromText( sr );
            }else if( last_line == "[Reso3BWBPList]" ){
                last_line = result.getCurve( "reso3BWBPList" )->appendFromText( sr );
            }else if( last_line == "[Reso4BWBPList]" ){
                last_line = result.getCurve( "reso4BWBPList" )->appendFromText( sr );
            }else if( last_line == "[Reso1AmpBPList]" ){
                last_line = result.getCurve( "reso1AmpBPList" )->appendFromText( sr );
            }else if( last_line == "[Reso2AmpBPList]" ){
                last_line = result.getCurve( "reso2AmpBPList" )->appendFromText( sr );
            }else if( last_line == "[Reso3AmpBPList]" ){
                last_line = result.getCurve( "reso3AmpBPList" )->appendFromText( sr );
            }else if( last_line == "[Reso4AmpBPList]" ){
                last_line = result.getCurve( "reso4AmpBPList" )->appendFromText( sr );
            }else if( last_line == "[GenderFactorBPList]" ){
                last_line = result.getCurve( "gen" )->appendFromText( sr );
            }else if( last_line == "[PortamentoTimingBPList]" ){
                last_line = result.getCurve( "por" )->appendFromText( sr );
            }else if( last_line == "[OpeningBPList]" ){
                last_line = result.getCurve( "ope" )->appendFromText( sr );
            }else{
                std::string buffer = last_line;
                buffer = StringUtil::replace( buffer, "[", "" );
                buffer = StringUtil::replace( buffer, "]", "" );
                std::vector<std::string> spl = StringUtil::explode( "#", buffer );
                int index = StringUtil::parseInt( spl[1] );
                if( last_line.find( "[ID#" ) == 0 ){
                    Event *item = new VSQ_NS::Event( sr, index, last_line );
                    temporaryEventList.push_back( item );
                    __id.insert( make_pair( index, item ) );
                }else if( last_line.find( "[h#" ) == 0 ){
                    __handle.insert( make_pair( index, VSQ_NS::Handle( sr, index, last_line ) ) );
                }
            }

            if( ! sr.ready() ){
                break;
            }
        }

        // まずhandleをidに埋め込み
        for( map<int, Event *>::iterator i = __id.begin(); i != __id.end(); ++i ){
            VSQ_NS::Event *id = i->second;
            if( __handle.find( id->_singerHandleIndex ) != __handle.end() ){
                if( id->type == VSQ_NS::EventType::SINGER ){
                    id->singerHandle = __handle[id->_singerHandleIndex];
                }else if( id->type == VSQ_NS::EventType::ICON ){
                    id->iconDynamicsHandle = __handle[id->_singerHandleIndex];
                }
            }
            if( __handle.find( id->_lyricHandleIndex ) != __handle.end() ){
                id->lyricHandle = __handle[id->_lyricHandleIndex];
            }
            if( __handle.find( id->_vibratoHandleIndex ) != __handle.end() ){
                id->vibratoHandle = __handle[id->_vibratoHandleIndex];
            }
            if( __handle.find( id->_noteHeadHandleIndex ) != __handle.end() ){
                id->noteHeadHandle = __handle[id->_noteHeadHandleIndex];
            }
        }

        // idをeventListに埋め込み
        Event::List *events = result.getEvents();
        events->clear();
        int count = 0;
        for( map<int, tick_t>::iterator i = t_event_list.begin(); i != t_event_list.end(); ++i ){
            int id_number = i->first;
            tick_t clock = i->second;
            if( __id.find( id_number ) != __id.end() ){
                count++;
                Event item = *__id[id_number];
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
