/**
 * VocaloidMidiEventListFactory.hpp
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
#ifndef __VocaloidMidiEventListFactory_hpp__
#define __VocaloidMidiEventListFactory_hpp__

#include "vsqglobal.hpp"
#include "NrpnEvent.hpp"
#include "Track.hpp"
#include "TempoList.hpp"
#include "MidiParameterType.hpp"

VSQ_BEGIN_NAMESPACE

using namespace std;

/**
 * @brief
 */
class VocaloidMidiEventListFactory{
protected:
    /**
     * @brief トラックの Expression(DYN) の NRPN リストを作成する
     * @param track 出力するトラック
     * @param tempoList テンポ情報
     * @param preSendMilliseconds ミリ秒単位のプリセンド時間
     * @return NrpnEvent の配列
     */
    static vector<NrpnEvent> generateExpressionNRPN( Track *track, TempoList *tempoList, int preSendMilliseconds ){
        vector<NrpnEvent> ret;
        BPList *dyn = track->getCurve( "DYN" );
        size_t count = dyn->size();
        int lastDelay = 0;
        for( int i = 0; i < count; i++ ){
            tick_t clock = dyn->getKeyClock( i );
            tick_t actualClock;
            int delay;
            _getActualClockAndDelay( tempoList, clock, preSendMilliseconds, &actualClock, &delay );
            if( actualClock >= 0 ){
                if( lastDelay != delay ){
                    int delayMsb, delayLsb;
                    _getMsbAndLsb( delay, &delayMsb, &delayLsb );
                    NrpnEvent delayNrpn( actualClock, MidiParameterType::CC_E_DELAY, delayMsb, delayLsb );
                    ret.push_back( delayNrpn );
                }
                lastDelay = delay;

                NrpnEvent add(
                    actualClock,
                    MidiParameterType::CC_E_EXPRESSION,
                    dyn->getValue( i )
                );
                ret.push_back( add );
            }
        }
        return ret;
    }

    /**
     * @brief 指定した時刻における、プリセンド込の時刻と、ディレイを取得する
     * @param tempoList テンポ情報
     * @param clock (int) Tick 単位の時刻
     * @param msPreSend (int) ミリ秒単位のプリセンド時間
     * @return (int) プリセンド分のクロックを引いた Tick 単位の時刻
     * @return (int) ミリ秒単位のプリセンド時間
     */
    static void _getActualClockAndDelay( TempoList *tempoList, tick_t clock, int msPreSend, tick_t *actualClock, int *delay ){
        double clock_msec = tempoList->getSecFromClock( clock ) * 1000.0;

        if( clock_msec - msPreSend <= 0 ){
            *actualClock = 0;
        }else{
            double draft_clock_sec = (clock_msec - msPreSend) / 1000.0;
            *actualClock = (tick_t)::floor( tempoList->getClockFromSec( draft_clock_sec ) );
        }
        *delay = (int)::floor( clock_msec - tempoList->getSecFromClock( (double)*actualClock ) * 1000.0 );
    }

    /**
     * @brief DATA の値を MSB と LSB に分解する
     * @param value (int) 分解する値
     * @param (int) MSB の値
     * @param (int) LSB の値
     */
    static void _getMsbAndLsb( int value, int *msb, int *lsb ){
        if( 0x3fff < value ){
            *msb = 0x7f;
            *lsb = 0x7f;
        }else{
            *msb = 0xff & (value >> 7);
            *lsb = value - (*msb << 7);
        }
    }
};

VSQ_END_NAMESPACE

#endif
