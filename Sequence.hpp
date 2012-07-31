/**
 * Sequence.hpp
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
#ifndef __Sequence_hpp__
#define __Sequence_hpp__

#include "vsqglobal.hpp"
#include "Track.hpp"
#include "TempoList.hpp"
#include "TimesigList.hpp"
#include "OutputStream.hpp"
#include "MidiEvent.hpp"
#include "CP932Converter.hpp"
#include "NrpnEvent.hpp"
#include "BitConverter.hpp"
#include <vector>
#include <string.h>

VSQ_BEGIN_NAMESPACE

using namespace std;

/**
 * @brief VSQ ファイルのシーケンスを保持するクラス
 */
class Sequence{
public:
    /**
     * @brief トラックのリスト。最初のトラックは MasterTrack であり、通常の音符が格納されるトラックはインデックス 1 以降となる
     */
    vector<Track> track;

    /**
     * @brief テンポ情報を保持したテーブル
     */
    TempoList tempoList;

    /**
     * @brief 拍子情報を保持したテーブル
     */
    TimesigList timesigList;

    /**
     * @brief プリメジャーを保持する
     */
    Master master;

    /**
     * @brief ミキサー情報
     */
    Mixer mixer;

    /**
     * @brief シーケンスに付属するタグ情報
     */
    string tag;

private:
    /**
     * @brief テンポが省略された際の、基準となるテンポ値
     */
    static const int baseTempo = 500000;

    /**
     * @brief VOCALOID の NRPN を出力するかどうか
    -- @todo NRPN 出力関連の メソッドのテストが書けたらデフォルトで true にする
     */
    static const bool _WRITE_NRPN = false;

    /**
     * @brief 四分音符 1 個あたりの Tick 数
     */
    static const int _tickPerQuarter = 480;

    /**
     * @brief Tick 単位の曲の長さ
     */
    tick_t _totalClocks;

public:
    /**
     * @brief 初期化を行う
     * @param singer (string) 歌手名
     * @param preMeasure (int) 小節単位のプリメジャー
     * @param numerator (int) 拍子の分子の値
     * @param denominator (int) 拍子の分母の値
     * @param tempo (int) テンポ値。四分音符の長さのマイクロ秒単位の長さ
     */
    explicit Sequence( const string &singer, int preMeasure, int numerator, int denominator, int tempo ){
        _totalClocks = preMeasure * 480 * 4 / denominator * numerator;

        track.push_back( Track() );
        track.push_back( Track( "Voice1", singer ) );
        master = Master( preMeasure );
        mixer = Mixer( 0, 0, 0, 0 );
        mixer.slave.push_back( MixerItem( 0, 0, 0, 0 ) );
        timesigList.push( Timesig( numerator, denominator, 0 ) );
        tempoList.push( Tempo( 0, tempo ) );
    }

    /**
     * @brief コピーを作成する
     * @return (Sequence) オブジェクトのコピー
     */
    Sequence clone() const{
        Sequence ret( "Miku", 1, 4, 4, baseTempo );
        ret.track.clear();
        for( int i = 0; i < track.size(); i++ ){
            ret.track.push_back( track[i].clone() );
        }

        ret.tempoList = TempoList();
        for( int i = 0; i < tempoList.size(); i++ ){
            ret.tempoList.push( tempoList.get( i ).clone() );
        }

        for( int i = 0; i < timesigList.size(); i++ ){
            ret.timesigList.push( timesigList.get( i ).clone() );
        }
        ret._totalClocks = _totalClocks;
        ret.master = master.clone();
        ret.mixer = mixer.clone();
        return ret;
    }

    /**
     * @brief テンポが一つも指定されていない場合の、基本テンポ値を取得する
     * @return (int) テンポ値。四分音符の長さのマイクロ秒単位の長さ
     */
    int getBaseTempo() const{
        return Sequence::baseTempo;
    }

    /**
     * @brief Tick 単位の曲の長さを取得する
     * シーケンスに変更を加えた場合、<code><a href="#updateTotalClocks">updateTotalClocks</a></code> を呼んでからこのメソッドを呼ぶこと
     * @return (int) Tick 単位の曲の長さ
     */
    tick_t getTotalClocks() const{
        return _totalClocks;
    }

    /**
     * @brief プリメジャー値を取得する
     * @return (int) 小節単位のプリメジャー長さ
     */
    int getPreMeasure() const{
        return master.preMeasure;
    }

    /**
     * @brief Tick 単位のプリメジャー部分の長さを取得する
     * @return (int) Tick 単位のプリメジャー長さ
     */
    tick_t getPreMeasureClocks() const{
        return _calculatePreMeasureInClock();
    }

    /**
     * @brief 四分音符あたりの Tick 数を取得する
     * @return (int) 四分音符一つあたりの Tick 数
     */
    tick_t getTickPerQuarter() const{
        return _tickPerQuarter;
    }

    /**
     * @brief カーブ名のリストを取得する
     * @return カーブ名のリスト
     */
    static const vector<string> getCurveNameList(){
        vector<string> result;
        result.push_back( "VEL" );
        result.push_back( "DYN" );
        result.push_back( "BRE" );
        result.push_back( "BRI" );
        result.push_back( "CLE" );
        result.push_back( "OPE" );
        result.push_back( "GEN" );
        result.push_back( "POR" );
        result.push_back( "PIT" );
        result.push_back( "PBS" );
        return result;
    }

    /**
     * @brief totalClock の値を更新する
     */
    void updateTotalClocks(){
        tick_t max = getPreMeasureClocks();
        vector<string> curveNameList = getCurveNameList();
        for( int i = 1; i < track.size(); i++ ){
            Track *track = &(this->track[i]);
            int numEvents = track->getEvents()->size();
            if( 0 < numEvents ){
                Event lastItem = track->getEvents()->get( numEvents - 1 );
                max = ::max( max, lastItem.clock + lastItem.getLength() );
            }
            for( int j = 0; j < curveNameList.size(); j++ ){
                string vct = curveNameList[j];
                BPList *list = track->getCurve( vct );
                if( list ){
                    int size = list->size();
                    if( size > 0 ){
                        tick_t last_key = list->getKeyClock( size - 1 );
                        max = ::max( max, last_key );
                    }
                }
            }
        }
        _totalClocks = max;
    }


    //TODO:
    /*
        --
        -- 指定したクロックにおける、音符長さ(ゲートタイム単位)の最大値を調べます
        -- @param clock [int]
        -- @return [int]
        function this:getMaximumNoteLengthAt( clock )
            local secAtStart = self.tempoList:getSecFromClock( clock );
            local secAtEnd = secAtStart + Id.MAX_NOTE_MILLISEC_LENGTH / 1000.0;
            local clockAtEnd = math.floor( self.tempoList:getClockFromSec( secAtEnd ) - 1 );
            secAtEnd = self.tempoList:getSecFromClock( clockAtEnd );
            while ( math.floor( secAtEnd * 1000.0 ) - math.floor( secAtStart * 1000.0 ) <= Id.MAX_NOTE_MILLISEC_LENGTH )do
                clockAtEnd = clockAtEnd + 1;
                secAtEnd = self.tempoList:getSecFromClock( clockAtEnd );
            end
            clockAtEnd = clockAtEnd - 1;
            return clockAtEnd - clock;
        end
*/

    /**
     * @brief ストリームに出力する
     * @param stream (? extends OutputStream) 出力先のストリーム
     * @param msPreSend (int) ミリ秒単位のプリセンドタイム
     * @param encoding (string) マルチバイト文字のテキストエンコーディング(現在は Shift_JIS 固定で、引数は無視される)
     * @param printPitch (boolean) pitch を含めて出力するかどうか(現在は <code>false</code> 固定で、引数は無視される)
     */
    void write( OutputStream *stream, int msPreSend, const string &encoding, bool printPitch = false ){
        updateTotalClocks();
        //TODO: 型を要検討
        size_t first_position; //チャンクの先頭のファイル位置

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
        _writeUnsignedShort( stream, track.size() );
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
        for( int i = 0; i < timesigList.size(); i++ ){
            Timesig entry = timesigList.get( i );
            events.push_back( MidiEvent::generateTimeSigEvent( entry.clock, entry.numerator, entry.denominator ) );
        }
        TempoList::Iterator itr = tempoList.iterator();
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
        size_t pos = stream->getPointer();
        stream->seek( first_position - 4 );
        _writeUnsignedInt( stream, pos - first_position );
        stream->seek( pos );

        // トラック
        Sequence temp = clone();
        _printTrack( &temp, 1, stream, msPreSend, encoding, printPitch, &master, &mixer );
        int count = track.size();
        for( int track = 2; track < count; track++ ){
            _printTrack( this, track, stream, msPreSend, encoding, printPitch, 0, 0 );
        }
    }

    //TODO:
/*
        --
        -- @param vsq [VsqFile]
        -- @param track [int]
        -- @param msPreSend [int]
        -- @return [VsqNrpn[] ]
        function Sequence.generateFx2DepthNRPN( vsq, track, msPreSend )
            local ret = {};--Array.new();--Vector<VsqNrpn>();
            local fx2depth = vsq.track:get( track ):getCurve( "fx2depth" );
            local count = fx2depth:size();
            local i;
            for i = 0, count - 1, do
                local clock = fx2depth:getKeyClock( i );
                local c = clock - vsq:getPresendClockAt( clock, msPreSend );
                if( c >= 0 )then
                    local add = NrpnEvent.new(
                        c,
                        MidiParameterEnum.CC_FX2_EFFECT2_DEPTH,
                        fx2depth:getValue( i )
                    );
                    table.insert( ret, add );
                end
            end
            return ret;
        end
*/

/*
        --
        --  指定したトラックのデータから，NRPNを作成します
        -- @param vsq [VsqFile]
        -- @param track [int]
        -- @param msPreSend [int]
        -- @param clock_start [int]
        -- @param clock_end [int]
        -- @return [VsqNrpn[] ]
        function Sequence._generateNRPN_5( vsq, track, msPreSend, clock_start, clock_end )
            local temp = vsq:clone();
            temp.removePart( clock_end, vsq.TotalClocks );
            if( 0 < clock_start )then
                temp.removePart( 0, clock_start );
            end
            temp.Master.PreMeasure = 1;
            --temp.m_premeasure_clocks = temp.getClockFromBarCount( 1 );
            local ret = Sequence._generateNRPN_3( temp, track, msPreSend );
            temp = nil;
            return ret;
        end
*/

    //TODO: 実装
/*
    /**
     * @brief 指定したシーケンスの指定したトラックから、NRPN のリストを作成する
     * @param sequence (Sequence) 出力元のシーケンス
     * @param track (int) 出力するトラックの番号
     * @param msPreSend (int) ミリ秒単位のプリセンド時間
     * @return (table<NrpnEvent>) NrpnEvent の配列
     *
    static vector<NrpnEvent> _generateNRPN_3( Sequence *sequence, int track, int msPreSend ){
        local list = {};

        local target = sequence.track:get( track );
        local version = target.common.version;

        local count = target.events:size();
        local note_start = 0;
        local note_end = target.events:size() - 1;
        local i;
        for i = 0, count - 1, 1 do
            if( 0 <= target.events:get( i ).clock )then
                note_start = i;
                break;
            end
            note_start = i;
        end
        for i = target.events:size() - 1, 0, -1 do
            if( target.events:get( i ).clock <= sequence._totalClocks )then
                note_end = i;
                break;
            end
        end

        -- 最初の歌手を決める
        local singer_event = -1;
        for i = note_start, 0, -1 do
            if( target.events:get( i ).type == EventTypeEnum.SINGER )then
                singer_event = i;
                break;
            end
        end
        if( singer_event >= 0 )then --見つかった場合
            Sequence._array_add_all( list, Sequence._generateSingerNRPN( sequence, target.events:get( singer_event ), 0 ) );
        else                   --多分ありえないと思うが、歌手が不明の場合。
            --throw new Exception( "first singer was not specified" );
            table.insert( list, NrpnEvent.new( 0, MidiParameterEnum.CC_BS_LANGUAGE_TYPE, 0x0 ) );
            table.insert( list, NrpnEvent.new( 0, MidiParameterEnum.PC_VOICE_TYPE, 0x0 ) );
        end

        Sequence._array_add_all( list, Sequence._generateVoiceChangeParameterNRPN( sequence, track, msPreSend ) );
        if( version:sub( 1, 4 ) == "DSB2" )then
            Sequence._array_add_all( list, Sequence.generateFx2DepthNRPN( sequence, track, msPreSend ) );
        end

        local ms_presend = msPreSend;
        local dyn = target:getCurve( "dyn" );
        if( dyn:size() > 0 )then
            local listdyn = Sequence._generateExpressionNRPN( sequence, track, ms_presend );
            if( #listdyn > 0 )then
                Sequence._array_add_all( list, listdyn );
            end
        end
        local pbs = target:getCurve( "pbs" );
        if( pbs:size() > 0 )then
            local listpbs = Sequence._generatePitchBendSensitivityNRPN( sequence, track, ms_presend );
            if( #listpbs > 0 )then
                Sequence._array_add_all( list, listpbs );
            end
        end
        local pit = target:getCurve( "pit" );
        if( pit:size() > 0 )then
            local listpit = Sequence._generatePitchBendNRPN( sequence, track, ms_presend );
            if( #listpit > 0 )then
                Sequence._array_add_all( list, listpit );
            end
        end

        local lastDelay = nil;
        local last_note_end = 0;
        for i = note_start, note_end, 1 do
            local item = target.events:get( i );
            if( item.type == EventTypeEnum.NOTE )then
                local note_loc = 0x03;
                if( item.clock == last_note_end )then
                    note_loc = note_loc - 0x02;
                end

                -- 次に現れる音符イベントを探す
                local nextclock = item.clock + item:getLength() + 1;
                local event_count = target.events:size();
                local j;
                for j = i + 1, event_count - 1, 1 do
                    local itemj = target.events:get( j );
                    if( itemj.type == EventTypeEnum.NOTE )then
                        nextclock = itemj.clock;
                        break;
                    end
                end
                if( item.clock + item:getLength() == nextclock )then
                    note_loc = note_loc - 0x01;
                end

                local noteNrpn;
                noteNrpn, lastDelay = Sequence._generateNoteNRPN(
                    sequence,
                    track,
                    item,
                    msPreSend,
                    note_loc,
                    lastDelay
                );

                table.insert( list, noteNrpn );
                Sequence._array_add_all(
                    list,
                    Sequence._generateVibratoNRPN( sequence, item, msPreSend )
                );
                last_note_end = item.clock + item:getLength();
            elseif( item.type == EventTypeEnum.SINGER )then
                if( i > note_start and i ~= singer_event )then
                    Sequence._array_add_all(
                        list,
                        Sequence._generateSingerNRPN( sequence, item, msPreSend )
                    );
                end
            end
        end

        table.sort( list, NrpnEvent.compare );
        local merged = {};
        for i = 1, #list, 1 do
            Sequence._array_add_all( merged, list[i]:expand() );
        end
        return merged;
    }
*/

    //TODO: lua用の実装なので、後で消す
/*
    ---
    -- 配列を連結する
    -- @access private
    -- @param src_array (table) 連結先のテーブル。参照として更新される
    -- @param add_array (table) 追加される要素が格納されたテーブル
    -- @return (table) src_array と同じインスタンス
    -- @access static private
    function Sequence._array_add_all( src_array, add_array )
        local i;
        for i = 1, #add_array, 1 do
            table.insert( src_array, add_array[i] );
        end
        return src_array;
    end
*/
protected:
    /**
     * @brief 文字列を MIDI メタイベントにしたものを取得する
     * @param sr (TextStream) MIDI イベント生成元の文字列が出力されたストリーム
     * @param encoding (string) マルチバイト文字のテキストエンコーディング(現在は Shift_JIS 固定で、引数は無視される)
     */
    static vector<MidiEvent> _getMidiEventsFromMetaText( TextStream *sr, const string &encoding ){
        string _NL = "" + (char)0x0a;
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
                vector<int> prefix = _getLinePrefixBytes( line_count + 1 );
                while( prefix.size() + buffer.size() >= 127 ){
                    line_count++;
                    prefix = _getLinePrefixBytes( line_count );
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
                    prefix = _getLinePrefixBytes( line_count + 1 );
                }
            }
            if( false == buffer.empty() ){
                vector<int> prefix = _getLinePrefixBytes( line_count + 1 );
                while( prefix.size() + buffer.size() >= 127 ){
                    line_count = line_count + 1;
                    prefix = _getLinePrefixBytes( line_count );
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
                    prefix = _getLinePrefixBytes( line_count + 1 );
                }
                if( false == buffer.empty() ){
                    line_count = line_count + 1;
                    prefix = _getLinePrefixBytes( line_count );
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
    static vector<int> _getLinePrefixBytes( int count ){
        int digits = _getHowManyDigits( count );
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
    static int _getHowManyDigits( int number ){
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
    static void _writeUnsignedShort( OutputStream *stream, int data ){
        vector<char> dat = BitConverter::getBytesUInt16BE( data );
        stream->write( dat.data(), 0, dat.size() );
    }

    /**
     * @brief 32 ビットの unsigned int 値をビッグエンディアンでストリームに書きこむ
     * @param stream (? extends OutputStram) 出力先のストリーム
     * @param data (int) 出力する値
     */
    static void _writeUnsignedInt( OutputStream *stream, int data ){
        vector<char> dat = BitConverter::getBytesUInt32BE( data );
        stream->write( dat.data(), 0, dat.size() );
    }

private:
    /**
     * @brief プリメジャーの Tick 単位の長さを計算する
     * @return (int) Tick 単位のプリメジャー長さ
     */
    tick_t _calculatePreMeasureInClock() const{
        int pre_measure = master.preMeasure;
        int last_bar_count = timesigList.get( 0 ).barCount;
        tick_t last_clock = timesigList.get( 0 ).clock;
        int last_denominator = timesigList.get( 0 ).denominator;
        int last_numerator = timesigList.get( 0 ).numerator;
        for( int i = 1; i < timesigList.size(); i++ ){
            if( timesigList.get( i ).barCount >= pre_measure ){
                break;
            }else{
                last_bar_count = timesigList.get( i ).barCount;
                last_clock = timesigList.get( i ).clock;
                last_denominator = timesigList.get( i ).denominator;
                last_numerator = timesigList.get( i ).numerator;
            }
        }

        int remained = pre_measure - last_bar_count;// プリメジャーの終わりまでの残り小節数
        return last_clock + (int)::floor( remained * last_numerator * 480 * 4 / last_denominator );
    }

    /**
     * @brief SMF のトラックヘッダー文字列を取得する
     */
    static string getTrackHeader(){
        return string( "MTrk" );
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
    static void _printTrack( Sequence *sequence, int track, OutputStream *stream, int msPreSend, const string &encoding, bool printPitch, Master *master = 0, Mixer *mixer = 0 ){
        char _NL = 0x0a;
        // ヘッダ
        string mtrk = getTrackHeader();
        stream->write( mtrk.c_str(), 0, 4 );
        // データ長。とりあえず0
        char empty[] = { 0x00, 0x00, 0x00, 0x00 };
        stream->write( empty, 0, 4 );
        size_t first_position = stream->getPointer();
        // トラック名
        MidiEvent::writeDeltaClock( stream, 0x00 );// デルタタイム
        stream->write( 0xff );// ステータスタイプ
        stream->write( 0x03 );// イベントタイプSequence/Track Name
        string seq_name = CP932Converter::convertFromUTF8( sequence->track[track].getName() );
        MidiEvent::writeDeltaClock( stream, seq_name.size() ); // seq_nameの文字数
        //TODO: 第3引数の型キャスト要らなくなるかも
        stream->write( seq_name.c_str(), 1, (int)seq_name.size() );

        // Meta Textを準備
        TextStream textStream;
        sequence->track[track].printMetaText( textStream, sequence->getTotalClocks() + 120, 0, printPitch );
        tick_t lastClock = 0;
        vector<MidiEvent> meta = _getMidiEventsFromMetaText( &textStream, encoding );
        for( int i = 0; i < meta.size(); i++ ){
            MidiEvent::writeDeltaClock( stream, meta[i].clock - lastClock );
            meta[i].writeData( stream );
            lastClock = meta[i].clock;
        }
        tick_t maxClock = lastClock;

        if( Sequence::_WRITE_NRPN ){
            //TODO:
/*
            lastClock = 0;
            vector<NrpnEvent> data = Sequence::generateNRPN( sequence, track, msPreSend );
            vector<MidiEvent> nrpns = NrpnEvent::convert( data );
            for( int i = 0; i < nrpns.size(); i++ ){
                MidiEvent item = nrpns[i];
                MidiEvent::writeDeltaClock( stream, item.clock - lastClock );
                item.writeData( stream );
                lastClock = item.clock;
            }
            maxClock = ::max( maxClock, lastClock );
*/
        }

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
        int pos = stream->getPointer();
        stream->seek( first_position - 4 );
        _writeUnsignedInt( stream, pos - first_position );
        stream->seek( pos );
    }

    //TODO: 実装
/*
    ---
    -- 歌手変更イベントの NRPN リストを作成する。
    -- トラック先頭の歌手変更イベントについては、このメソッドで作成してはいけない。
    -- トラック先頭のgenerateNRPN メソッドが担当する
    -- @param sequence (Sequence) 出力元のシーケンス
    -- @param singerEvent (Event) 出力する歌手変更イベント
    -- @param msPreSend (int) ミリ秒単位のプリセンド時間
    -- @return (table<NrpnEvent>) NrpnEvent の配列
    -- @access static private
    function Sequence._generateSingerNRPN( sequence, singerEvent, msPreSend )
        local clock = singerEvent.clock;
        local singer_handle = nil;
        if( singerEvent.singerHandle ~= nil )then
            singer_handle = singerEvent.singerHandle;
        end
        if( singer_handle == nil )then
            return {};
        end

        local clock_msec = sequence.tempoList:getSecFromClock( clock ) * 1000.0;

        local msEnd = sequence.tempoList:getSecFromClock( singerEvent.clock + singerEvent:getLength() ) * 1000.0;
        local duration = math.floor( math.ceil( msEnd - clock_msec ) );

        local duration0, duration1 = Sequence._getMsbAndLsb( duration );

        local actualClock, delay;
        actualClock, delay = sequence:_getActualClockAndDelay( clock, msPreSend );
        local delayMsb, delayLsb = Sequence._getMsbAndLsb( delay );
        local ret = {};

        local add = NrpnEvent.new( actualClock, MidiParameterEnum.CC_BS_VERSION_AND_DEVICE, 0x00, 0x00 );
        add:append( MidiParameterEnum.CC_BS_DELAY, delayMsb, delayLsb, true );
        add:append( MidiParameterEnum.CC_BS_LANGUAGE_TYPE, singer_handle.language, true );
        add:append( MidiParameterEnum.PC_VOICE_TYPE, singer_handle.program );
        local arr = {};
        table.insert( arr, add );
        return arr;
    end
*/

    //TODO:実装
/*
    ---
    -- トラックの音符イベントから NRPN のリストを作成する
    -- @param sequence (Sequence) 出力元のシーケンス
    -- @param track (int) 出力するトラックの番号
    -- @param noteEvent (Event) 出力する音符イベント
    -- @param msPreSend (int) ミリ秒単位のプリセンド時間
    -- @param noteLocation (int) <ul>
    --                               <li>00:前後共に連続した音符がある
    --                               <li>01:後ろにのみ連続した音符がある
    --                               <li>02:前にのみ連続した音符がある
    --                               <li>03:前後どちらにも連続した音符が無い
    --                           </ul>
    -- @param lastDelay (int) 直前の音符イベントに指定された、ミリ秒単位のディレイ値。最初の音符イベントの場合は nil を指定する
    -- @return (NrpnEvent) NrpnEvent
    -- @return (int) この音符に対して設定された、ミリ秒単位のディレイ値
    -- @access static private
    function Sequence._generateNoteNRPN( sequence, track, noteEvent, msPreSend, noteLocation, lastDelay )
        local clock = noteEvent.clock;
        local add = nil;

        local actualClock, delay;
        actualClock, delay = sequence:_getActualClockAndDelay( clock, msPreSend );

        if( lastDelay == nil )then
            add = NrpnEvent.new(
                actualClock,
                MidiParameterEnum.CVM_NM_VERSION_AND_DEVICE,
                0x00, 0x00
            );
            lastDelay = 0;
        end

        if( lastDelay ~= delay )then
            local delayMsb, delayLsb = Sequence._getMsbAndLsb( delay );
            if( add == nil )then
                add = NrpnEvent.new( actualClock, MidiParameterEnum.CVM_NM_DELAY, delayMsb, delayLsb );
            else
                add:append( MidiParameterEnum.CVM_NM_DELAY, delayMsb, delayLsb, true );
            end
        end

        if( add == nil )then
            add = NrpnEvent.new( actualClock, MidiParameterEnum.CVM_NM_NOTE_NUMBER, noteEvent.note );
        else
            add:append( MidiParameterEnum.CVM_NM_NOTE_NUMBER, noteEvent.note, true );
        end

        -- Velocity
        add:append( MidiParameterEnum.CVM_NM_VELOCITY, noteEvent.dynamics, true );

        -- Note Duration
        local msEnd = sequence.tempoList:getSecFromClock( clock + noteEvent:getLength() ) * 1000.0;
        local clock_msec = sequence.tempoList:getSecFromClock( clock ) * 1000.0;
        local duration = math.floor( msEnd - clock_msec );
        local duration0, duration1 = Sequence._getMsbAndLsb( duration );
        add:append( MidiParameterEnum.CVM_NM_NOTE_DURATION, duration0, duration1, true );

        -- Note Location
        add:append( MidiParameterEnum.CVM_NM_NOTE_LOCATION, noteLocation, true );

        if( noteEvent.vibratoHandle ~= nil )then
            add:append( MidiParameterEnum.CVM_NM_INDEX_OF_VIBRATO_DB, 0x00, 0x00, true );
            local icon_id = noteEvent.vibratoHandle.iconId;
            local num = icon_id:sub( icon_id:len() - 3 );
            local vibrato_type = math.floor( tonumber( num, 16 ) );
            local note_length = noteEvent:getLength();
            local vibrato_delay = noteEvent.vibratoDelay;
            local bVibratoDuration = math.floor( (note_length - vibrato_delay) / note_length * 127.0 );
            local bVibratoDelay = 0x7f - bVibratoDuration;
            add:append( MidiParameterEnum.CVM_NM_VIBRATO_CONFIG, vibrato_type, bVibratoDuration, true );
            add:append( MidiParameterEnum.CVM_NM_VIBRATO_DELAY, bVibratoDelay, true );
        end

        local spl = noteEvent.lyricHandle:getLyricAt( 0 ):getPhoneticSymbolList();
        local s = "";
        local j;
        for j = 1, #spl, 1 do
            s = s .. spl[j];
        end
        local symbols = {};
        local i;
        for i = 1, s:len(), 1 do
            symbols[i] = s:sub( i, i );
        end

        local renderer = sequence.track:get( track ).common.version;
        if( renderer:sub( 1, 4 ) == "DSB2" )then
            add:append( 0x5011, 0x01, true );--TODO: (byte)0x5011の意味は解析中
        end

        add:append( MidiParameterEnum.CVM_NM_PHONETIC_SYMBOL_BYTES, #symbols, true );-- (byte)0x12(Number of phonetic symbols in bytes)
        local count = -1;
        local consonantAdjustment = noteEvent.lyricHandle:getLyricAt( 0 ):getConsonantAdjustmentList();
        for j = 1, #spl, 1 do
            local chars = Util.stringToArray( spl[j] );--Array.new();
            local k;
            for k = 1, #chars, 1 do
                count = count + 1;
                if( k == 1 )then
                    add:append( Util.bor( Util.lshift( 0x50, 8 ), (0x13 + count) ), chars[k], consonantAdjustment[j], true ); -- Phonetic symbol j
                else
                    add:append( Util.bor( Util.lshift( 0x50, 8 ), (0x13 + count) ), chars[k], true ); -- Phonetic symbol j
                end
            end
        end
        if( renderer:sub( 1, 4 ) ~= "DSB2" )then
            add:append( MidiParameterEnum.CVM_NM_PHONETIC_SYMBOL_CONTINUATION, 0x7f, true ); -- End of phonetic symbols
        end
        if( renderer:sub( 1, 4 ) == "DSB3" )then
            local v1mean = math.floor( noteEvent.pmBendDepth * 60 / 100 );
            if( v1mean < 0 )then
                v1mean = 0;
            end
            if( 60 < v1mean )then
                v1mean = 60;
            end
            local d1mean = math.floor( 0.3196 * noteEvent.pmBendLength + 8.0 );
            local d2mean = math.floor( 0.92 * noteEvent.pmBendLength + 28.0 );
            add:append( MidiParameterEnum.CVM_NM_V1MEAN, v1mean, true );-- (byte)0x50(v1mean)
            add:append( MidiParameterEnum.CVM_NM_D1MEAN, d1mean, true );-- (byte)0x51(d1mean)
            add:append( MidiParameterEnum.CVM_NM_D1MEAN_FIRST_NOTE, 0x14, true );-- (byte)0x52(d1meanFirstNote)
            add:append( MidiParameterEnum.CVM_NM_D2MEAN, d2mean, true );-- (byte)0x53(d2mean)
            add:append( MidiParameterEnum.CVM_NM_D4MEAN, noteEvent.d4mean, true );-- (byte)0x54(d4mean)
            add:append( MidiParameterEnum.CVM_NM_PMEAN_ONSET_FIRST_NOTE, noteEvent.pMeanOnsetFirstNote, true ); -- 055(pMeanOnsetFirstNote)
            add:append( MidiParameterEnum.CVM_NM_VMEAN_NOTE_TRNSITION, noteEvent.vMeanNoteTransition, true ); -- (byte)0x56(vMeanNoteTransition)
            add:append( MidiParameterEnum.CVM_NM_PMEAN_ENDING_NOTE, noteEvent.pMeanEndingNote, true );-- (byte)0x57(pMeanEndingNote)
            add:append( MidiParameterEnum.CVM_NM_ADD_PORTAMENTO, noteEvent.pmbPortamentoUse, true );-- (byte)0x58(AddScoopToUpInternals&AddPortamentoToDownIntervals)
            local decay = math.floor( noteEvent.demDecGainRate / 100.0 * 0x64 );
            add:append( MidiParameterEnum.CVM_NM_CHANGE_AFTER_PEAK, decay, true );-- (byte)0x59(changeAfterPeak)
            local accent = math.floor( 0x64 * noteEvent.demAccent / 100.0 );
            add:append( MidiParameterEnum.CVM_NM_ACCENT, accent, true );-- (byte)0x5a(Accent)
        end
        add:append( MidiParameterEnum.CVM_NM_NOTE_MESSAGE_CONTINUATION, 0x7f, true );-- (byte)0x7f(Note message continuation)
        return add, delay;
    end
*/

    //TODO:実装
/*
    ---
    -- 指定したシーケンスの指定したトラックから、PitchBend の NRPN リストを作成する
    -- @param sequence (Sequence) 出力元のシーケンス
    -- @param track (int) 出力するトラックの番号
    -- @param msPreSend (int) ミリ秒単位のプリセンド時間
    -- @return (table<NrpnEvent>) NrpnEvent の配列
    -- @access static private
    function Sequence._generatePitchBendNRPN( sequence, track, msPreSend )
        local ret = {};
        local pit = sequence.track:get( track ):getCurve( "PIT" );
        local count = pit:size();
        local i, lastDelay;
        for i = 0, count - 1, 1 do
            local clock = pit:getKeyClock( i );

            local actualClock, delay;
            actualClock, delay = sequence:_getActualClockAndDelay( clock, msPreSend );
            if( actualClock >= 0 )then
                if( lastDelay ~= delay )then
                    local delayMsb, delayLsb;
                    delayMsb, delayLsb = Sequence._getMsbAndLsb( delay );
                    table.insert(
                        ret,
                        NrpnEvent.new( actualClock, MidiParameterEnum.PB_DELAY, delayMsb, delayLsb )
                    );
                end
                lastDelay = delay;

                local value = pit:getValue( i ) + 0x2000;
                local msb, lsb = Sequence._getMsbAndLsb( value );
                table.insert(
                    ret,
                    NrpnEvent.new( actualClock, MidiParameterEnum.PB_PITCH_BEND, msb, lsb )
                );
            end
        end
        return ret;
    end
*/

    //TODO:実装
/*
    ---
    -- 指定したシーケンスの指定したトラックから、PitchBendSensitivity の NRPN リストを作成する
    -- @param sequence (Sequence) 出力元のシーケンス
    -- @param track (int) 出力するトラックの番号
    -- @param msPreSend (int) ミリ秒単位のプリセンド時間
    -- @return (table<NrpnEvent>) NrpnEvent の配列
    -- @access static private
    function Sequence._generatePitchBendSensitivityNRPN( sequence, track, msPreSend )
        local ret = {};
        local pbs = sequence.track:get( track ):getCurve( "PBS" );
        local count = pbs:size();
        local i;
        local lastDelay = 0;
        for i = 0, count - 1, 1 do
            local clock = pbs:getKeyClock( i );
            local actualClock, delay;
            actualClock, delay = sequence:_getActualClockAndDelay( clock, msPreSend );
            if( actualClock >= 0 )then
                if( lastDelay ~= delay )then
                    local delayMsb, delayLsb;
                    delayMsb, delayLsb = Sequence._getMsbAndLsb( delay );
                    table.insert(
                        ret,
                        NrpnEvent.new( actualClock, MidiParameterEnum.CC_PBS_DELAY, delayMsb, delayLsb )
                    );
                end
                lastDelay = delay;

                local add = NrpnEvent.new(
                    actualClock,
                    MidiParameterEnum.CC_PBS_PITCH_BEND_SENSITIVITY,
                    pbs:getValue( i ),
                    0x00
                );
                table.insert( ret, add );
            end
        end
        return ret;
    end
*/

    //TODO:実装
/*
    ---
    -- トラックの音符イベントから、ビブラート出力用の NRPN のリストを作成する
    -- @param sequence (Sequence) 出力元のシーケンス
    -- @param noteEvent (Event) 出力する音符イベント
    -- @param msPreSend (int) ミリ秒単位のプリセンド時間
    -- @return (table<NrpnEvent>) NrpnEvent の配列
    -- @access static private
    function Sequence._generateVibratoNRPN( sequence, noteEvent, msPreSend )
        local ret = {};
        if( noteEvent.vibratoHandle ~= nil )then
            local vclock = noteEvent.clock + noteEvent.vibratoDelay;
            local actualClock, delay;
            actualClock, delay = sequence:_getActualClockAndDelay( vclock, msPreSend );
            local delayMsb, delayLsb = Sequence._getMsbAndLsb( delay );
            local add2 = NrpnEvent.new( actualClock, MidiParameterEnum.CC_VD_VERSION_AND_DEVICE, 0x00, 0x00 );
            add2:append( MidiParameterEnum.CC_VR_VERSION_AND_DEVICE, 0x00, 0x00 );
            add2:append( MidiParameterEnum.CC_VD_DELAY, delayMsb, delayLsb );
            add2:append( MidiParameterEnum.CC_VR_DELAY, delayMsb, delayLsb );
            -- CC_VD_VIBRATO_DEPTH, CC_VR_VIBRATO_RATE では、NRPN の MSB を省略してはいけない
            add2:append( MidiParameterEnum.CC_VD_VIBRATO_DEPTH, noteEvent.vibratoHandle:getStartDepth() );
            add2:append( MidiParameterEnum.CC_VR_VIBRATO_RATE, noteEvent.vibratoHandle:getStartRate() );
            table.insert( ret, add2 );
            local vlength = noteEvent:getLength() - noteEvent.vibratoDelay;

            local depthBP = noteEvent.vibratoHandle:getDepthBP();
            local count = depthBP:size();
            if( count > 0 )then
                local i, lastDelay;
                lastDelay = 0;
                for i = 0, count - 1, 1 do
                    local itemi = depthBP:get( i );
                    local percent = itemi.x;
                    local cl = vclock + math.floor( percent * vlength );
                    actualClock, delay = sequence:_getActualClockAndDelay( cl, msPreSend );
                    local nrpnEvent = nil;
                    if( lastDelay ~= delay )then
                        delayMsb, delayLsb = Sequence._getMsbAndLsb( delay );
                        nrpnEvent = NrpnEvent.new( actualClock, MidiParameterEnum.CC_VD_DELAY, delayMsb, delayLsb );
                        nrpnEvent:append( MidiParameterEnum.CC_VD_VIBRATO_DEPTH, itemi.y );
                    else
                        nrpnEvent = NrpnEvent.new( actualClock, MidiParameterEnum.CC_VD_VIBRATO_DEPTH, itemi.y );
                    end
                    lastDelay = delay;
                    table.insert( ret, nrpnEvent );
                end
            end

            local rateBP = noteEvent.vibratoHandle:getRateBP();
            count = rateBP:size();
            if( count > 0 )then
                local i, lastDelay;
                lastDelay = 0;
                for i = 0, count - 1, 1 do
                    local itemi = rateBP:get( i );
                    local percent = itemi.x;
                    local cl = vclock + math.floor( percent * vlength );
                    actualClock, delay = sequence:_getActualClockAndDelay( cl, msPreSend );
                    local nrpnEvent = nil;
                    if( lastDelay ~= delay )then
                        delayMsb, delayLsb = Sequence._getMsbAndLsb( delay );
                        nrpnEvent = NrpnEvent.new( actualClock, MidiParameterEnum.CC_VR_DELAY, delayMsb, delayLsb );
                        nrpnEvent:append( MidiParameterEnum.CC_VR_VIBRATO_RATE, itemi.y );
                    else
                        nrpnEvent = NrpnEvent.new( actualClock, MidiParameterEnum.CC_VR_VIBRATO_RATE, itemi.y );
                    end
                    lastDelay = delay;
                    table.insert( ret, nrpnEvent );
                end
            end
        end
        table.sort( ret, NrpnEvent.compare );
        return ret;
    end
*/

    //TODO:実装
/*
    ---
    -- 指定したシーケンスの指定したトラックから、VoiceChangeParameter の NRPN リストを作成する
    -- @param sequence (Sequence) 出力元のシーケンス
    -- @param track (int) 出力するトラックの番号
    -- @param msPreSend (int) ミリ秒単位のプリセンド時間
    -- @return (table<NrpnEvent>) NrpnEvent の配列
    -- @access static private
    function Sequence._generateVoiceChangeParameterNRPN( sequence, track, msPreSend )
        local premeasure_clock = sequence:getPreMeasureClocks();
        local renderer = sequence.track:get( track ).common.version;
        local res = {};

        local curves;
        if( renderer:sub( 1, 4 ) == "DSB3" )then
            curves = { "BRE", "BRI", "CLE", "POR", "OPE", "GEN" };
        elseif( renderer:sub( 1, 4 ) == "DSB2" )then
            curves = { "BRE", "BRI", "CLE", "POR", "GEN", "harmonics",
                       "reso1amp", "reso1bw", "reso1freq",
                       "reso2amp", "reso2bw", "reso2freq",
                       "reso3amp", "reso3bw", "reso3freq",
                       "reso4amp", "reso4bw", "reso4freq" };
        else
            curves = { "BRE", "BRI", "CLE", "POR", "GEN" };
        end

        local i, lastDelay;
        lastDelay = 0;
        for i = 1, #curves, 1 do
            local list = sequence.track:get( track ):getCurve( curves[i] );
            if( list:size() > 0 )then
                lastDelay = Sequence._addVoiceChangeParameters( res, list, sequence, msPreSend, lastDelay );
            end
        end
        table.sort( res, NrpnEvent.compare );
        return res;
    end
*/

    //TODO:実装
/*
    ---
    -- Voice Change Parameter の NRPN を追加する
    -- @access private
    -- @param dest (table) 追加先のテーブル
    -- @param list (BPList) Voice Change Parameter のデータ点が格納された BPList
    -- @param sequence (Sequence) シーケンス
    -- @param msPreSend (int) ミリ秒単位のプリセンド時間
    -- @param lastDelay (int) 直前の delay 値(ミリ秒単位)
    -- @return (int) delay 値(ミリ秒単位)
    -- @access static private
    function Sequence._addVoiceChangeParameters( dest, list, sequence, msPreSend, lastDelay )
        local id = MidiParameterEnum.getVoiceChangeParameterId( list:getName() );
        local count = list:size();
        local j;
        for j = 0, count - 1, 1 do
            local clock = list:getKeyClock( j );
            local value = list:getValue( j );
            local actualClock, delay;
            actualClock, delay = sequence:_getActualClockAndDelay( clock, msPreSend );

            if( actualClock >= 0 )then
                local add = nil;
                if( lastDelay ~= delay )then
                    local delayMsb, delayLsb;
                    delayMsb, delayLsb = Sequence._getMsbAndLsb( delay );
                    add = NrpnEvent.new( actualClock, MidiParameterEnum.VCP_DELAY, delayMsb, delayLsb );
                end
                lastDelay = delay;

                if( add == nil )then
                    add = NrpnEvent.new( actualClock, MidiParameterEnum.VCP_VOICE_CHANGE_PARAMETER_ID, id );
                else
                    add:append( MidiParameterEnum.VCP_VOICE_CHANGE_PARAMETER_ID, id );
                end
                add:append( MidiParameterEnum.VCP_VOICE_CHANGE_PARAMETER, value, true );
                table.insert( dest, add );
            end
        end
        return lastDelay;
    end
*/
};

VSQ_END_NAMESPACE

#endif
