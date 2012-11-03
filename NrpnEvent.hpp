/**
 * NrpnEvent.hpp
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
#ifndef __NrpnEvent_hpp__
#define __NrpnEvent_hpp__

#include "vsqglobal.hpp"
#include "MidiEvent.hpp"
#include "MidiParameterType.hpp"
#include <vector>

VSQ_BEGIN_NAMESPACE

using namespace std;

/**
 * @brief NRPN イベントを表すクラス
 */
class NrpnEvent{
public:
    /**
     * @brief Tick 単位の時刻
     */
    tick_t clock;// = 0;

    /**
     * @brief NRPN の値
     */
    MidiParameterType::MidiParameterTypeEnum nrpn;// = 0;

    /**
     * @brief DATA MSB
     */
    int dataMSB;// = 0;

    /**
     * @brief DATA LSB
     */
    int dataLSB;// = 0;

    /**
     * @brief DATA LSB 値を持っているかどうか
     */
    bool hasLSB;// = false;

    /**
     * @brief MSB の出力を省略するかどうか
     */
    bool isMSBOmittingRequired;// = false;

protected:
    /**
     * @brief 子イベントのリスト
     */
    vector<NrpnEvent> _list;

public:
    /**
     * @brief 時刻、NRPN、DATA MSB を指定し、初期化を行う
     * @param clock (int) Tick 単位の時刻
     * @param nrpn (int) NRPN
     * @param dataMsb (int) DATA MSB
     */
    explicit NrpnEvent( tick_t clock, MidiParameterType::MidiParameterTypeEnum nrpn, int dataMsb ){
        this->clock = clock;
        this->nrpn = nrpn;
        this->dataMSB = dataMsb;
        this->dataLSB = 0x0;
        this->hasLSB = false;
        this->isMSBOmittingRequired = false;
    }

    /**
     * @brief 時刻、NRPN、DATA MSB、DATA LSB を指定し、初期化を行う
     * @param clock (int) Tick 単位の時刻
     * @param nrpn (int) NRPN
     * @param dataMsb (int) DATA MSB
     * @param dataLsb (int) DATA LSB
     */
    explicit NrpnEvent( tick_t clock, MidiParameterType::MidiParameterTypeEnum nrpn, int dataMsb, int dataLsb ){
        this->clock = clock;
        this->nrpn = nrpn;
        this->dataMSB = dataMsb;
        this->dataLSB = dataLsb;
        this->hasLSB = true;
        this->isMSBOmittingRequired = false;
    }

    /**
     * @brief 親子関係によって入れ子になっている NRPN イベントを展開し、配列に変換する
     * @return (table<NrpnEvent>) 展開後の NRPN イベントの配列
     */
    vector<NrpnEvent> expand(){
        vector<NrpnEvent> ret;
        if( hasLSB ){
            NrpnEvent v( clock, nrpn, dataMSB, dataLSB );
            v.isMSBOmittingRequired = isMSBOmittingRequired;
            ret.push_back( v );
        }else{
            NrpnEvent v( clock, nrpn, dataMSB );
            v.isMSBOmittingRequired = isMSBOmittingRequired;
            ret.push_back( v );
        }
        for( int i = 0; i < _list.size(); i++ ){
            vector<NrpnEvent> add = _list[i].expand();
            for( int j = 0; j < add.size(); j++ ){
                ret.push_back( add[j] );
            }
        }
        return ret;
    }

    /**
     * @brief 順序を比較する
     * @param item (NrpnEvent) 比較対象のアイテム
     * @return (int) このインスタンスが比較対象よりも小さい場合は負の整数、等しい場合は 0、大きい場合は正の整数を返す
     */
    int compareTo( const NrpnEvent &item ) const{
        if( clock == item.clock ){
            int thisNrpnMsb = (nrpn - (nrpn % 0x100)) / 0x100;
            int itemNrpnMsb = (item.nrpn - (item.nrpn % 0x100)) / 0x100;
            return itemNrpnMsb - thisNrpnMsb;
        }else{
            return clock - item.clock;
        }
    }

    /**
     * @brief NRPN、DATA MSB を指定し、イベントを追加する
     * @param nrpn (int) NRPN
     * @param dataMsb (int) DATA MSB
     */
    void append( MidiParameterType::MidiParameterTypeEnum nrpn, int dataMsb ){
        _list.push_back( NrpnEvent( clock, nrpn, dataMsb ) );
    }

    /**
     * @brief NRPN、DATA MSB、DATA LSB を指定し、イベントを追加する
     * @param nrpn (int) NRPN
     * @param dataMsb (int) DATA MSB
     * @param dataLsb (int) DATA LSB
     */
    void append( MidiParameterType::MidiParameterTypeEnum nrpn, int dataMsb, int dataLsb ){
        _list.push_back( NrpnEvent( clock, nrpn, dataMsb, dataLsb ) );
    }

    /**
     * @brief NRPN、DATA MSB、MSB 省略フラグを指定し、イベントを追加する
     * @param nrpn (int) NRPN
     * @param dataMsb (int) DATA MSB
     * @param isMsbOmittingRequired (boolean) NRPN MSB を省略する場合は <code>true</code> を、そうでない場合は <code>false</code> を指定する
     */
    void append( MidiParameterType::MidiParameterTypeEnum nrpn, int dataMsb, bool isMsbOmittingRequired ){
        NrpnEvent v( this->clock, nrpn, dataMsb );
        v.isMSBOmittingRequired = isMsbOmittingRequired;
        _list.push_back( v );
    }

    /**
     * @brief NRPN、DATA MSB、DATA LSB、MSB 省略フラグを指定し、イベントを追加する
     * @param nrpn (int) NRPN
     * @param dataMsb (int) DATA MSB
     * @param dataLsb (int) DATA LSB
     * @param isMsbOmittingRequired (boolean) NRPN MSB を省略する場合は <code>true</code> を、そうでない場合は <code>false</code> を指定する
     */
    void append( MidiParameterType::MidiParameterTypeEnum nrpn, int dataMsb, int dataLsb, bool isMsbOmittingRequired ){
        NrpnEvent v( this->clock, nrpn, dataMsb, dataLsb );
        v.isMSBOmittingRequired = isMsbOmittingRequired;
        _list.push_back( v );
    }

    /**
     * @brief 2 つの {@link NrpnEvent} を比較する
     * @param a (NrpnEvent) 比較対象のオブジェクト
     * @param b (NrpnEvent) 比較対象のオブジェクト
     * @return (boolean) <code>a</code> が <code>b</code> よりも小さい場合は <code>true</code>、そうでない場合は <code>false</code> を返す
     */
    static bool compare( const NrpnEvent &a, const NrpnEvent &b ){
        if( a.compareTo( b ) < 0 ){
            return true;
        }else{
            return false;
        }
    }

    /*
        --
        -- @param src1 [NrpnEvent[] ]
        -- @param src2 [NrpnEvent[] ]
        -- @return [NrpnEvent[] ]
        function NrpnEvent.merge( src1, src2 )
            local ret = [];--new Vector<NrpnEvent>();
            for ( local i = 0; i < src1.length; i++ ) {
                ret.push( src1[i] );
            end
            for ( local i = 0; i < src2.length; i++ ) {
                ret.push( src2[i] );
            end
            ret.sort( NrpnEvent.compare );--Collections.sort( ret );
            return ret;
        end
    */

    /**
     * @brief NRPN イベントの配列を、{@link MidiEvent} の配列に変換する
     * @param source (table<NrpnEvent>) NRPN イベントの配列
     * @return (table) {@link MidiEvent} の配列
     */
    static vector<MidiEvent> convert( const vector<NrpnEvent> &source ){
        int nrpn = source[0].nrpn;
        int msb = 0xff & (nrpn >> 8);
        int lsb = nrpn - (0xff00 & (msb << 8));
        vector<MidiEvent> ret;
        MidiEvent e;

        e = MidiEvent();
        e.clock = source[0].clock;
        e.firstByte = 0xb0;
        e.data.push_back( 0x63 );
        e.data.push_back( msb );
        ret.push_back( e );

        e = MidiEvent();
        e.clock = source[0].clock;
        e.firstByte = 0xb0;
        e.data.push_back( 0x62 );
        e.data.push_back( lsb );
        ret.push_back( e );

        e = MidiEvent();
        e.clock = source[0].clock;
        e.firstByte = 0xb0;
        e.data.push_back( 0x06 );
        e.data.push_back( source[0].dataMSB );
        ret.push_back( e );

        if( source[0].hasLSB ){
            e = MidiEvent();
            e.clock = source[0].clock;
            e.firstByte = 0xb0;
            e.data.push_back( 0x26 );
            e.data.push_back( source[0].dataLSB );
            ret.push_back( e );
        }

        for( int i = 1; i < source.size(); i++ ){
            NrpnEvent item = source[i];
            int tnrpn = item.nrpn;
            msb = 0xff & (tnrpn >> 8);
            lsb = tnrpn - (0xff00 & (msb << 8));
            if( false == item.isMSBOmittingRequired ){
                e = MidiEvent();
                e.clock = item.clock;
                e.firstByte = 0xb0;
                e.data.push_back( 0x63 );
                e.data.push_back( msb );
                ret.push_back( e );
            }

            e = MidiEvent();
            e.clock = item.clock;
            e.firstByte = 0xb0;
            e.data.push_back( 0x62 );
            e.data.push_back( lsb );
            ret.push_back( e );

            e = MidiEvent();
            e.clock = item.clock;
            e.firstByte = 0xb0;
            e.data.push_back( 0x06 );
            e.data.push_back( item.dataMSB );
            ret.push_back( e );
            if( item.hasLSB ){
                e = MidiEvent();
                e.clock = item.clock;
                e.firstByte = 0xb0;
                e.data.push_back( 0x26 );
                e.data.push_back( item.dataLSB );
                ret.push_back( e );
            }
        }
        return ret;
    }

protected:
    explicit NrpnEvent(){
        this->clock = 0;
        this->dataLSB = 0;
        this->dataMSB = 0;
        this->hasLSB = false;
        this->isMSBOmittingRequired = false;
        this->nrpn = (MidiParameterType::MidiParameterTypeEnum)0;
    }
};

VSQ_END_NAMESPACE

#endif
