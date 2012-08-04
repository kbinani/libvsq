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
 * @brief Track のインスタンスから、VOCALOID で使用される NRPN リストを作成するためのクラス
 */
class VocaloidMidiEventListFactory{
private:
    /**
     * @brief BPList のインスタンスから NrpnEvent のリストを作成する際の、
     * ディレイ指定 NrpnEvent と、値 NrpnEvent を提供するためのクラス
     */
    class NrpnEventProvider{
    protected:
        MidiParameterType::MidiParameterTypeEnum delayNrpn;
        MidiParameterType::MidiParameterTypeEnum nrpn;

    public:
        explicit NrpnEventProvider(
            MidiParameterType::MidiParameterTypeEnum delayNrpnType,
            MidiParameterType::MidiParameterTypeEnum nrpnType )
        {
            this->delayNrpn = delayNrpnType;
            this->nrpn = nrpnType;
        }

        virtual NrpnEvent getDelayNrpnEvent( tick_t actualClock, int delay ){
            int delayMsb, delayLsb;
            VocaloidMidiEventListFactory::_getMsbAndLsb( delay, &delayMsb, &delayLsb );
            return NrpnEvent( actualClock, delayNrpn, delayMsb, delayLsb );
        }

        virtual NrpnEvent getNrpnEvent( tick_t actualClock, int value ){
            return NrpnEvent( actualClock, nrpn, value );
        }
    };

    /**
     * @brief PitchBend カーブ用の NrpnEventProvider 実装
     */
    class PitchBendNrpnEventProvider : public NrpnEventProvider{
    public:
        explicit PitchBendNrpnEventProvider()
            : NrpnEventProvider( MidiParameterType::PB_DELAY, MidiParameterType::PB_PITCH_BEND )
        {
        }

        NrpnEvent getNrpnEvent( tick_t actualClock, int value ){
            int actualValue = value + 0x2000;
            int msb, lsb;
            VocaloidMidiEventListFactory::_getMsbAndLsb( actualValue, &msb, &lsb );
            return NrpnEvent( actualClock, nrpn, msb, lsb );
        }
    };

protected:
    /**
     * @brief データ点のリストから、NRPN のリストを作成する
     * @param result 作成した NRPN のリストの格納先
     * @param tempoList テンポ情報
     * @param preSendMilliseconds ミリ秒単位のプリセンド秒
     * @param list NRPN リストの元になるデータのリスト
     * @param delayNrpnType delay を指定する際の NRPN のタイプ
     * @param nrpnType データ点の値を指定する際の NRPN のタイプ
     */
    static void generateNRPNByBPList(
        vector<NrpnEvent> &result,
        TempoList *tempoList, int preSendMilliseconds,
        BPList *list, NrpnEventProvider *provider
    ){
        size_t count = list->size();
        int lastDelay = 0;
        for( int i = 0; i < count; i++ ){
            tick_t clock = list->getKeyClock( i );
            tick_t actualClock;
            int delay;
            _getActualClockAndDelay( tempoList, clock, preSendMilliseconds, &actualClock, &delay );
            if( actualClock >= 0 ){
                if( lastDelay != delay ){
                    result.push_back( provider->getDelayNrpnEvent( actualClock, delay ) );
                }
                lastDelay = delay;
                result.push_back( provider->getNrpnEvent( actualClock, list->getValue( i ) ) );
            }
        }
    }

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
        NrpnEventProvider *provider = new NrpnEventProvider( MidiParameterType::CC_E_DELAY, MidiParameterType::CC_E_EXPRESSION );
        generateNRPNByBPList( ret, tempoList, preSendMilliseconds, dyn, provider );
        delete provider;
        return ret;
    }

    /**
     * @brief トラックの先頭に記録される NRPN を作成する
     * @return NRPNイベント
     */
    static NrpnEvent generateHeaderNRPN(){
        NrpnEvent ret( 0, MidiParameterType::CC_BS_VERSION_AND_DEVICE, 0x00, 0x00 );
        ret.append( MidiParameterType::CC_BS_DELAY, 0x00, 0x00 );
        ret.append( MidiParameterType::CC_BS_LANGUAGE_TYPE, 0x00 );
        return ret;
    }

    /**
     * @brief 歌手変更イベントの NRPN リストを作成する。
     * トラック先頭の歌手変更イベントについては、このメソッドで作成してはいけない。
     * トラック先頭のgenerateNRPN メソッドが担当する
     * @param sequence (Sequence) 出力元のシーケンス
     * @param singerEvent (Event) 出力する歌手変更イベント
     * @param msPreSend (int) ミリ秒単位のプリセンド時間
     * @return (table<NrpnEvent>) NrpnEvent の配列
     */
    static vector<NrpnEvent> generateSingerNRPN( TempoList *tempoList, Event *singerEvent, int preSendMilliseconds ){
        tick_t clock = singerEvent->clock;
        Handle singer_handle;

        double clock_msec = tempoList->getSecFromClock( clock ) * 1000.0;

        double msEnd = tempoList->getSecFromClock( singerEvent->clock + singerEvent->getLength() ) * 1000.0;
        int duration = (int)::floor( ::ceil( msEnd - clock_msec ) );

        int duration0, duration1;
        _getMsbAndLsb( duration, &duration0, &duration1 );

        tick_t actualClock;
        int delay;
        _getActualClockAndDelay( tempoList, clock, preSendMilliseconds, &actualClock, &delay );
        int delayMsb, delayLsb;
        _getMsbAndLsb( delay, &delayMsb, &delayLsb );

        NrpnEvent add( actualClock, MidiParameterType::CC_BS_VERSION_AND_DEVICE, 0x00, 0x00 );
        add.append( MidiParameterType::CC_BS_DELAY, delayMsb, delayLsb, true );
        add.append( MidiParameterType::CC_BS_LANGUAGE_TYPE, singer_handle.language, true );
        add.append( MidiParameterType::PC_VOICE_TYPE, singer_handle.program );

        vector<NrpnEvent> ret;
        ret.push_back( add );
        return ret;
    }

    /**
     * @brief トラックの音符イベントから NRPN のリストを作成する
     * @param sequence (Sequence) 出力元のシーケンス
     * @param track (int) 出力するトラックの番号
     * @param noteEvent (Event) 出力する音符イベント
     * @param msPreSend (int) ミリ秒単位のプリセンド時間
     * @param noteLocation (int) <ul>
     *                               <li>00:前後共に連続した音符がある
     *                               <li>01:後ろにのみ連続した音符がある
     *                               <li>02:前にのみ連続した音符がある
     *                               <li>03:前後どちらにも連続した音符が無い
     *                           </ul>
     * @param lastDelay (int) 直前の音符イベントに指定された、ミリ秒単位のディレイ値。最初の音符イベントの場合は nil を指定する
     * @return (NrpnEvent) NrpnEvent
     * @return (int) この音符に対して設定された、ミリ秒単位のディレイ値
     */
    static NrpnEvent generateNoteNRPN( Track *track, TempoList *tempoList, Event *noteEvent, int msPreSend, int noteLocation, int *lastDelay, int *delay ){
        tick_t clock = noteEvent->clock;
        NrpnEvent add( 0, MidiParameterType::CC_BS_DELAY, 0, 0 );

        tick_t actualClock;
        _getActualClockAndDelay( tempoList, clock, msPreSend, &actualClock, delay );

        //lastDelay と delayがポインタなので注意
        bool addInitialized = false;
        int lastDelayValue;
        if( 0 == lastDelay ){
            add = NrpnEvent(
                actualClock,
                MidiParameterType::CVM_NM_VERSION_AND_DEVICE,
                0x00, 0x00
            );
            lastDelayValue = 0;
            addInitialized = true;
        }else{
            lastDelayValue = *lastDelay;
        }

        if( lastDelayValue != *delay ){
            int delayMsb, delayLsb;
            _getMsbAndLsb( *delay, &delayMsb, &delayLsb );
            if( false == addInitialized ){
                add = NrpnEvent( actualClock, MidiParameterType::CVM_NM_DELAY, delayMsb, delayLsb );
                addInitialized = true;
            }else{
                add.append( MidiParameterType::CVM_NM_DELAY, delayMsb, delayLsb, true );
            }
        }

        if( false == addInitialized ){
            add = NrpnEvent( actualClock, MidiParameterType::CVM_NM_NOTE_NUMBER, noteEvent->note );
        }else{
            add.append( MidiParameterType::CVM_NM_NOTE_NUMBER, noteEvent->note, true );
        }

        // Velocity
        add.append( MidiParameterType::CVM_NM_VELOCITY, noteEvent->dynamics, true );

        // Note Duration
        double msEnd = tempoList->getSecFromClock( clock + noteEvent->getLength() ) * 1000.0;
        double clock_msec = tempoList->getSecFromClock( clock ) * 1000.0;
        int duration = (int)::floor( msEnd - clock_msec );
        int duration0, duration1;
        _getMsbAndLsb( duration, &duration0, &duration1 );
        add.append( MidiParameterType::CVM_NM_NOTE_DURATION, duration0, duration1, true );

        // Note Location
        add.append( MidiParameterType::CVM_NM_NOTE_LOCATION, noteLocation, true );

        if( noteEvent->vibratoHandle.getHandleType() != HandleType::UNKNOWN ){
            add.append( MidiParameterType::CVM_NM_INDEX_OF_VIBRATO_DB, 0x00, 0x00, true );
            string icon_id = noteEvent->vibratoHandle.iconId;
            string num = icon_id.substr( icon_id.length() - 3 );
            int vibrato_type = (int)::floor( StringUtil::parseInt( num, 16 ) );
            int note_length = noteEvent->getLength();
            int vibrato_delay = noteEvent->vibratoDelay;
            int bVibratoDuration = (int)::floor( (note_length - vibrato_delay) / (double)note_length * 127.0 );
            int bVibratoDelay = 0x7f - bVibratoDuration;
            add.append( MidiParameterType::CVM_NM_VIBRATO_CONFIG, vibrato_type, bVibratoDuration, true );
            add.append( MidiParameterType::CVM_NM_VIBRATO_DELAY, bVibratoDelay, true );
        }

        vector<string> spl = noteEvent->lyricHandle.getLyricAt( 0 ).getPhoneticSymbolList();
        ostringstream os;
        for( int j = 0; j < spl.size(); j++ ){
            os << spl[j];
        }
        string s = os.str();
        vector<string> symbols;
        for( int i = 0; i < s.length(); i++ ){
            symbols.push_back( s.substr( i, 1 ) );
        }

        string renderer = track->getCommon()->version;
        if( renderer.substr( 0, 4 ) == string( "DSB2" ) ){
            add.append( (MidiParameterType::MidiParameterTypeEnum)0x5011, 0x01, true );//TODO: (byte)0x5011の意味は解析中
        }

        add.append( MidiParameterType::CVM_NM_PHONETIC_SYMBOL_BYTES, symbols.size(), true );// (byte)0x12(Number of phonetic symbols in bytes)
        int count = -1;
        vector<int> consonantAdjustment = noteEvent->lyricHandle.getLyricAt( 0 ).getConsonantAdjustmentList();
        for( int j = 0; j < spl.size(); j++ ){
            string chars = spl[j];
            for( int k = 0; k < chars.length(); k++ ){
                count = count + 1;
                if( k == 0 ){
                    add.append( (MidiParameterType::MidiParameterTypeEnum)((0x50 << 8) | (0x13 + count)), chars[k], consonantAdjustment[j], true ); // Phonetic symbol j
                }else{
                    add.append( (MidiParameterType::MidiParameterTypeEnum)((0x50 << 8) | (0x13 + count)), chars[k], true ); // Phonetic symbol j
                }
            }
        }
        if( renderer.substr( 0, 4 ) != string( "DSB2" ) ){
            add.append( MidiParameterType::CVM_NM_PHONETIC_SYMBOL_CONTINUATION, 0x7f, true ); // End of phonetic symbols
        }
        if( renderer.substr( 0, 4 ) == string( "DSB3" ) ){
            int v1mean = (int)::floor( noteEvent->pmBendDepth * 60 / 100 );
            if( v1mean < 0 ){
                v1mean = 0;
            }
            if( 60 < v1mean ){
                v1mean = 60;
            }
            int d1mean = (int)::floor( 0.3196 * noteEvent->pmBendLength + 8.0 );
            int d2mean = (int)::floor( 0.92 * noteEvent->pmBendLength + 28.0 );
            add.append( MidiParameterType::CVM_NM_V1MEAN, v1mean, true );// (byte)0x50(v1mean)
            add.append( MidiParameterType::CVM_NM_D1MEAN, d1mean, true );// (byte)0x51(d1mean)
            add.append( MidiParameterType::CVM_NM_D1MEAN_FIRST_NOTE, 0x14, true );// (byte)0x52(d1meanFirstNote)
            add.append( MidiParameterType::CVM_NM_D2MEAN, d2mean, true );// (byte)0x53(d2mean)
            add.append( MidiParameterType::CVM_NM_D4MEAN, noteEvent->d4mean, true );// (byte)0x54(d4mean)
            add.append( MidiParameterType::CVM_NM_PMEAN_ONSET_FIRST_NOTE, noteEvent->pMeanOnsetFirstNote, true ); // 055(pMeanOnsetFirstNote)
            add.append( MidiParameterType::CVM_NM_VMEAN_NOTE_TRNSITION, noteEvent->vMeanNoteTransition, true ); // (byte)0x56(vMeanNoteTransition)
            add.append( MidiParameterType::CVM_NM_PMEAN_ENDING_NOTE, noteEvent->pMeanEndingNote, true );// (byte)0x57(pMeanEndingNote)
            add.append( MidiParameterType::CVM_NM_ADD_PORTAMENTO, noteEvent->pmbPortamentoUse, true );// (byte)0x58(AddScoopToUpInternals&AddPortamentoToDownIntervals)
            int decay = (int)::floor( noteEvent->demDecGainRate / 100.0 * 0x64 );
            add.append( MidiParameterType::CVM_NM_CHANGE_AFTER_PEAK, decay, true );// (byte)0x59(changeAfterPeak)
            int accent = (int)::floor( 0x64 * noteEvent->demAccent / 100.0 );
            add.append( MidiParameterType::CVM_NM_ACCENT, accent, true );// (byte)0x5a(Accent)
        }
        add.append( MidiParameterType::CVM_NM_NOTE_MESSAGE_CONTINUATION, 0x7f, true );// (byte)0x7f(Note message continuation)
        return add;
    }

    /**
     * @brief 指定したシーケンスの指定したトラックから、PitchBend の NRPN リストを作成する
     * @param sequence (Sequence) 出力元のシーケンス
     * @param track (int) 出力するトラックの番号
     * @param msPreSend (int) ミリ秒単位のプリセンド時間
     * @return (table<NrpnEvent>) NrpnEvent の配列
     */
    static vector<NrpnEvent> generatePitchBendNRPN( Track *track, TempoList *tempoList, int msPreSend ){
        vector<NrpnEvent> ret;
        BPList *pit = track->getCurve( "PIT" );
        PitchBendNrpnEventProvider *provider = new PitchBendNrpnEventProvider();
        generateNRPNByBPList( ret, tempoList, msPreSend, pit, provider );
        delete provider;
        return ret;
    }

    /**
     * @brief 指定したシーケンスの指定したトラックから、PitchBendSensitivity の NRPN リストを作成する
     * @param sequence (Sequence) 出力元のシーケンス
     * @param track (int) 出力するトラックの番号
     * @param msPreSend (int) ミリ秒単位のプリセンド時間
     * @return (table<NrpnEvent>) NrpnEvent の配列
     */
    static vector<NrpnEvent> generatePitchBendSensitivityNRPN( Track *track, TempoList *tempoList, int msPreSend ){
        vector<NrpnEvent> ret;
        BPList *pbs = track->getCurve( "PBS" );
        int count = pbs->size();
        int lastDelay = 0;
        for( int i = 0; i < count; i++ ){
            tick_t clock = pbs->getKeyClock( i );
            tick_t actualClock;
            int delay;
            _getActualClockAndDelay( tempoList, clock, msPreSend, &actualClock, &delay );
            if( actualClock >= 0 ){
                if( lastDelay != delay ){
                    int delayMsb, delayLsb;
                    _getMsbAndLsb( delay, &delayMsb, &delayLsb );
                    ret.push_back( NrpnEvent( actualClock, MidiParameterType::CC_PBS_DELAY, delayMsb, delayLsb ) );
                }
                lastDelay = delay;

                NrpnEvent add(
                    actualClock,
                    MidiParameterType::CC_PBS_PITCH_BEND_SENSITIVITY,
                    pbs->getValue( i ),
                    0x00
                );
                ret.push_back( add );
            }
        }
        return ret;
    }

    /**
     * @brief トラックの音符イベントから、ビブラート出力用の NRPN のリストを作成する
     * @param sequence (Sequence) 出力元のシーケンス
     * @param noteEvent (Event) 出力する音符イベント
     * @param msPreSend (int) ミリ秒単位のプリセンド時間
     * @return (table<NrpnEvent>) NrpnEvent の配列
     */
    static vector<NrpnEvent> generateVibratoNRPN( TempoList *tempoList, Event *noteEvent, int msPreSend ){
        vector<NrpnEvent> ret;
        if( noteEvent->vibratoHandle.getHandleType() != HandleType::UNKNOWN ){
            tick_t vclock = noteEvent->clock + noteEvent->vibratoDelay;
            tick_t actualClock;
            int delay;
            _getActualClockAndDelay( tempoList, vclock, msPreSend, &actualClock, &delay );
            int delayMsb, delayLsb;
            _getMsbAndLsb( delay, &delayMsb, &delayLsb );
            NrpnEvent add2( actualClock, MidiParameterType::CC_VD_VERSION_AND_DEVICE, 0x00, 0x00 );
            add2.append( MidiParameterType::CC_VR_VERSION_AND_DEVICE, 0x00, 0x00 );
            add2.append( MidiParameterType::CC_VD_DELAY, delayMsb, delayLsb );
            add2.append( MidiParameterType::CC_VR_DELAY, delayMsb, delayLsb );
            // CC_VD_VIBRATO_DEPTH, CC_VR_VIBRATO_RATE では、NRPN の MSB を省略してはいけない
            add2.append( MidiParameterType::CC_VD_VIBRATO_DEPTH, noteEvent->vibratoHandle.getStartDepth() );
            add2.append( MidiParameterType::CC_VR_VIBRATO_RATE, noteEvent->vibratoHandle.getStartRate() );
            ret.push_back( add2 );
            tick_t vlength = noteEvent->getLength() - noteEvent->vibratoDelay;

            VibratoBPList depthBP = noteEvent->vibratoHandle.getDepthBP();
            int count = depthBP.size();
            if( count > 0 ){
                int lastDelay = 0;
                for( int i = 0; i < count; i++ ){
                    VibratoBP itemi = depthBP.get( i );
                    double percent = itemi.x;
                    tick_t cl = vclock + (tick_t) ::floor( percent * vlength );
                    _getActualClockAndDelay( tempoList, cl, msPreSend, &actualClock, &delay );
                    NrpnEvent nrpnEvent( 0, MidiParameterType::CC_BS_DELAY, 0 );
                    if( lastDelay != delay ){
                        _getMsbAndLsb( delay, &delayMsb, &delayLsb );
                        nrpnEvent = NrpnEvent( actualClock, MidiParameterType::CC_VD_DELAY, delayMsb, delayLsb );
                        nrpnEvent.append( MidiParameterType::CC_VD_VIBRATO_DEPTH, itemi.y );
                    }else{
                        nrpnEvent = NrpnEvent( actualClock, MidiParameterType::CC_VD_VIBRATO_DEPTH, itemi.y );
                    }
                    lastDelay = delay;
                    ret.push_back( nrpnEvent );
                }
            }

            VibratoBPList rateBP = noteEvent->vibratoHandle.getRateBP();
            count = rateBP.size();
            if( count > 0 ){
                int lastDelay = 0;
                for( int i = 0; i < count; i++ ){
                    VibratoBP itemi = rateBP.get( i );
                    double percent = itemi.x;
                    tick_t cl = vclock + (tick_t)::floor( percent * vlength );
                    _getActualClockAndDelay( tempoList, cl, msPreSend, &actualClock, &delay );
                    NrpnEvent nrpnEvent( 0, MidiParameterType::CC_BS_DELAY, 0 );
                    if( lastDelay != delay ){
                        _getMsbAndLsb( delay, &delayMsb, &delayLsb );
                        nrpnEvent = NrpnEvent( actualClock, MidiParameterType::CC_VR_DELAY, delayMsb, delayLsb );
                        nrpnEvent.append( MidiParameterType::CC_VR_VIBRATO_RATE, itemi.y );
                    }else{
                        nrpnEvent = NrpnEvent( actualClock, MidiParameterType::CC_VR_VIBRATO_RATE, itemi.y );
                    }
                    lastDelay = delay;
                    ret.push_back( nrpnEvent );
                }
            }
        }
        std::sort( ret.begin(), ret.end(), NrpnEvent::compare );
        return ret;
    }

    /**
     * @brief 指定したシーケンスの指定したトラックから、VoiceChangeParameter の NRPN リストを作成する
     * @param sequence (Sequence) 出力元のシーケンス
     * @param track (int) 出力するトラックの番号
     * @param msPreSend (int) ミリ秒単位のプリセンド時間
     * @return (table<NrpnEvent>) NrpnEvent の配列
     */
    static vector<NrpnEvent> generateVoiceChangeParameterNRPN( Track *track, TempoList *tempoList, int msPreSend, tick_t premeasure_clock ){
        string renderer = track->getCommon()->version;
        vector<NrpnEvent> res;

        vector<string> curves;
        if( renderer.substr( 0, 4 ) == "DSB3" ){
            curves.push_back( "BRE" ); curves.push_back( "BRI" ); curves.push_back( "CLE" );
            curves.push_back( "POR" ); curves.push_back( "OPE" ); curves.push_back( "GEN" );
        }else if( renderer.substr( 0, 4 ) == "DSB2" ){
            curves.push_back( "BRE" ); curves.push_back( "BRI" ); curves.push_back( "CLE" );
            curves.push_back( "POR" ); curves.push_back( "GEN" ); curves.push_back( "harmonics" );
            curves.push_back( "reso1amp" ); curves.push_back( "reso1bw" ); curves.push_back( "reso1freq" );
            curves.push_back( "reso2amp" ); curves.push_back( "reso2bw" ); curves.push_back( "reso2freq" );
            curves.push_back( "reso3amp" ); curves.push_back( "reso3bw" ); curves.push_back( "reso3freq" );
            curves.push_back( "reso4amp" ); curves.push_back( "reso4bw" ); curves.push_back( "reso4freq" );
        }else{
            curves.push_back( "BRE" ); curves.push_back( "BRI" ); curves.push_back( "CLE" );
            curves.push_back( "POR" ); curves.push_back( "GEN" );
        }

        int lastDelay = 0;
        for( int i = 0; i < curves.size(); i++ ){
            BPList *list = track->getCurve( curves[i] );
            if( list->size() > 0 ){
                lastDelay = addVoiceChangeParameters( res, list, tempoList, msPreSend, lastDelay );
            }
        }
        std::sort( res.begin(), res.end(), NrpnEvent::compare );
        return res;
    }

    /**
     * @param vsq [VsqFile]
     * @param track [int]
     * @param msPreSend [int]
     * @return [VsqNrpn[] ]
     */
    static vector<NrpnEvent> generateFx2DepthNRPN( Track *track, TempoList *tempoList, int preSendMilliseconds ){
        vector<NrpnEvent> ret;
        BPList *fx2depth = track->getCurve( "fx2depth" );
        NrpnEventProvider *provider = new NrpnEventProvider( MidiParameterType::CC_FX2_DELAY, MidiParameterType::CC_FX2_EFFECT2_DEPTH );
        generateNRPNByBPList( ret, tempoList, preSendMilliseconds, fx2depth, provider );
        delete provider;
        return ret;
    }

    /**
     * @brief Voice Change Parameter の NRPN を追加する
     * @param dest (table) 追加先のテーブル
     * @param list (BPList) Voice Change Parameter のデータ点が格納された BPList
     * @param sequence (Sequence) シーケンス
     * @param msPreSend (int) ミリ秒単位のプリセンド時間
     * @param lastDelay (int) 直前の delay 値(ミリ秒単位)
     * @return (int) delay 値(ミリ秒単位)
     */
    static int addVoiceChangeParameters( vector<NrpnEvent> &dest, BPList *list, TempoList *tempoList, int msPreSend, int lastDelay ){
        int id = MidiParameterType::getVoiceChangeParameterId( list->getName() );
        for( int j = 0; j < list->size(); j++ ){
            tick_t clock = list->getKeyClock( j );
            int value = list->getValue( j );
            tick_t actualClock;
            int delay;
            _getActualClockAndDelay( tempoList, clock, msPreSend, &actualClock, &delay );

            if( actualClock >= 0 ){
                NrpnEvent add( 0, MidiParameterType::CC_BS_DELAY, 0 );
                bool initialized = false;
                if( lastDelay != delay ){
                    int delayMsb, delayLsb;
                    _getMsbAndLsb( delay, &delayMsb, &delayLsb );
                    add = NrpnEvent( actualClock, MidiParameterType::VCP_DELAY, delayMsb, delayLsb );
                    initialized = true;
                }
                lastDelay = delay;

                if( false == initialized ){
                    add = NrpnEvent( actualClock, MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID, id );
                }else{
                    add.append( MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID, id );
                }
                add.append( MidiParameterType::VCP_VOICE_CHANGE_PARAMETER, value, true );
                dest.push_back( add );
            }
        }
        return lastDelay;
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
