/**
 * Track.hpp
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
#ifndef __Track_hpp__
#define __Track_hpp__

#include "vsqglobal.hpp"
#include "Event.hpp"
#include "Common.hpp"
#include "Master.hpp"
#include "Mixer.hpp"
#include "BPList.hpp"
#include "EventListIndexIterator.hpp"
#include "MidiEvent.hpp"
#include "CP932Converter.hpp"

VSQ_BEGIN_NAMESPACE

/**
 * @todo _pit とかを一般化する
 */
class Track
{
protected:
    /**
     * @brief トラック名などの情報
     * @todo Rename to _common.
     */
    Common _common;

    /**
     * @brief イベントの一覧
     */
    Event::List _events;

private:
    /**
     * @brief PIT。ピッチベンド(pitchBendBPList)。default=0
     */
    BPList _pit;

    /**
     * @brief PBS。ピッチベンドセンシティビティ(pitchBendSensBPList)。default=2
     */
    BPList _pbs;

    /**
     * @brief DYN。ダイナミクス(dynamicsBPList)。default=64
     */
    BPList _dyn;

    /**
     * @brief BRE。ブレシネス(epRResidualBPList)。default=0
     */
    BPList _bre;

    /**
     * @brief BRI。ブライトネス(epRESlopeBPList)。default=64
     */
    BPList _bri;

    /**
     * @brief CLE。クリアネス(epRESlopeDepthBPList)。default=0
     */
    BPList _cle;

    /**
     */
    BPList _reso1FreqBPList;

    /**
     */
    BPList _reso2FreqBPList;

    /**
     */
    BPList _reso3FreqBPList;

    /**
     */
    BPList _reso4FreqBPList;

    /**
     */
    BPList _reso1BWBPList;

    /**
     */
    BPList _reso2BWBPList;

    /**
     */
    BPList _reso3BWBPList;

    /**
     */
    BPList _reso4BWBPList;

    /**
     */
    BPList _reso1AmpBPList;

    /**
     */
    BPList _reso2AmpBPList;

    /**
     */
    BPList _reso3AmpBPList;

    /**
     */
    BPList _reso4AmpBPList;

    /**
     * @brief Harmonics。(EpRSineBPList)default = 64
     */
    BPList _harmonics;

    /**
     * @brief Effect2 Depth。
     */
    BPList _fx2depth;

    /**
     * @brief GEN。ジェンダーファクター(genderFactorBPList)。default=64
     */
    BPList _gen;

    /**
     * @brief POR。ポルタメントタイミング(portamentoTimingBPList)。default=64
     */
    BPList _por;

    /**
     * @brief OPE。オープニング(openingBPList)。default=127
     */
    BPList _ope;

    std::map<std::string, VSQ_NS::BPList *> curveNameMap;

public:
    /**
     * @brief Master Trackを構築
     */
    explicit Track(){
        this->_initCor( "Track1", "Miku" );
    }

    /**
     * @brief Master Trackでないトラックを構築
     * @param name (string) トラック名
     * @param singer (string) トラックのデフォルトの歌手名
     */
    explicit Track( std::string name, std::string singer ){
        this->_initCor( name, singer );
    }

    Track( const Track &value ){
        value.deepCopy( this );
    }

    Track &operator = ( const Track &value ){
        value.deepCopy( this );
        return *this;
    }

    /**
     * @brief トラックの名前を取得する
     * @return (string) トラック名
     */
    const std::string getName() const{
        //if( common == nil ){
        //    return "Master Track";
        //}else{
            return _common.name;
        //}
    }

    /**
     * @brief トラックの名前を設定する
     * @param value (string) トラック名
     */
    void setName( const std::string &value ){
        _common.name = value;
    }

    /**
     * @brief 指定された種類のイベントのインデクスを順に返す反復子を取得する
     * @param iteratorKind 反復子の種類
     * @return 反復子
     */
    EventListIndexIterator getIndexIterator( EventListIndexIteratorKind::EventListIndexIteratorKindEnum iteratorKind ){
        return EventListIndexIterator( &this->_events, (int)iteratorKind );
    }

    /**
        -- このトラックの再生モードを取得します．
        --
        -- @return [int] PlayMode.PlayAfterSynthまたはPlayMode.PlayWithSynth
        function this:getPlayMode()
            if( self.common == nil ){
                return PlayModeEnum.PLAY_WITH_SYNTH;
            }
            if( self.common.lastPlayMode ~= PlayModeEnum.PLAY_AFTER_SYNTH and
                 self.common.lastPlayMode ~= PlayModeEnum.PLAY_WITH_SYNTH ){
                self.common.lastPlayMode = PlayModeEnum.PLAY_WITH_SYNTH;
            }
            return self.common.lastPlayMode;
        }
        */

    /**
        -- このトラックの再生モードを設定します．
        --
        -- @param value [int] PlayMode.PlayAfterSynth, PlayMode.PlayWithSynth, またはPlayMode.Offのいずれかを指定します
        -- @return [void]
        function this:setPlayMode( value )
            if( self.MetaText == nil ) return;
            if( self.common == nil ){
                self.common = Common.new( "Miku", 128, 128, 128, DynamicsModeEnum.EXPERT, value );
                return;
            }
            if( value == PlayModeEnum.OFF ){
                if( self.common.playMode ~= PlayModeEnum.OFF ){
                    self.common.lastPlayMode = self.common.playMode;
                }
            else
                self.common.lastPlayMode = value;
            }
            self.common.playMode = value;
        }
        */

    /**
        -- このトラックがレンダリングされるかどうかを取得します．
        --
        -- @return [bool]
        function this:isTrackOn()
            if( self.MetaText == nil ) return true;
            if( self.common == nil ) return true;
            return self.common.playMode ~= PlayModeEnum.OFF;
        }
        */

    /**
        -- このトラックがレンダリングされるかどうかを設定します，
        --
        -- @param value [bool]
        function this:setTrackOn( value )
            if( self.MetaText == nil ) return;
            if( self.common == nil ){
                self.common = Common.new( "Miku", 128, 128, 128, DynamicsModeEnum.EXPERT, value ? PlayModeEnum.PLAY_WITH_SYNTH : PlayModeEnum.OFF );
            }
            if( value ){
                if( self.common.lastPlayMode ~= PlayModeEnum.PLAY_AFTER_SYNTH and
                     self.common.lastPlayMode ~= PlayModeEnum.PLAY_WITH_SYNTH ){
                    self.common.lastPlayMode = PlayModeEnum.PLAY_WITH_SYNTH;
                }
                self.common.playMode = self.common.lastPlayMode;
            else
                if( self.common.playMode == PlayModeEnum.PLAY_AFTER_SYNTH or
                     self.common.playMode == PlayModeEnum.PLAY_WITH_SYNTH ){
                    self.common.lastPlayMode = self.common.playMode;
                }
                self.common.playMode = PlayModeEnum.OFF;
            }
        }
        */

    /**
        -- このトラックの，指定したゲートタイムにおけるピッチベンドを取得します．単位はCentです．
        --
        -- @param clock [int] ピッチベンドを取得するゲートタイム
        -- @return [double]
        function this:getPitchAt( clock )
            local inv2_13 = 1.0 / 8192.0;
            local pit = self.PIT.getValue( clock );
            local pbs = self.PBS.getValue( clock );
            return pit * pbs * inv2_13 * 100.0;
        }
        */

    /**
        -- クレッシェンド，デクレッシェンド，および強弱記号をダイナミクスカーブに反映させます．
        -- この操作によって，ダイナミクスカーブに設定されたデータは全て削除されます．
        -- @return [void]
        function this:reflectDynamics()
            local dyn = self.getCurve( "dyn" );
            dyn.clear();
            local itr;
            for itr = self.getDynamicsEventIterator(); itr.hasNext();
                local item = itr.next();
                local handle = item.IconDynamicsHandle;
                if( handle == nil ){
                    continue;
                }
                local clock = item.Clock;
                local length = item.getLength();

                if( handle.isDynaffType() ){
                    -- 強弱記号
                    dyn.add( clock, handle.getStartDyn() );
                else
                    -- クレッシェンド，デクレッシェンド
                    local start_dyn = dyn.getValue( clock );

                    -- 範囲内のアイテムを削除
                    local count = dyn.size();
                    local i;
                    for i = count - 1; i >= 0; i--
                        local c = dyn.getKeyClock( i );
                        if( clock <= c and c <= clock + length ){
                            dyn.removeElementAt( i );
                        }else if( c < clock ){
                            break;
                        }
                    }

                    local bplist = handle.getDynBP();
                    if( bplist == nil or (bplist ~= nil and bplist.size() <= 0) ){
                        -- カーブデータが無い場合
                        local a = 0.0;
                        if( length > 0 ){
                            a = (handle.getEndDyn() - handle.getStartDyn()) / length;
                        }
                        local last_val = start_dyn;
                        local i;
                        for i = clock; i < clock + length; i++
                            local val = start_dyn + org.kbinani.PortUtil.castToInt( a * (i - clock) );
                            if( val < dyn.getMinimum() ){
                                val = dyn.getMinimum();
                            }else if( dyn.getMaximum() < val ){
                                val = dyn.getMaximum();
                            }
                            if( last_val ~= val ){
                                dyn.add( i, val );
                                last_val = val;
                            }
                        }
                    else
                        -- カーブデータがある場合
                        local last_val = handle.getStartDyn();
                        local last_clock = clock;
                        local bpnum = bplist.size();
                        local last = start_dyn;

                        -- bplistに指定されている分のデータ点を追加
                        local i;
                        for i = 0; i < bpnum; i++
                            local point = bplist.getElement( i );
                            local pointClock = clock + org.kbinani.PortUtil.castToInt( length * point.X );
                            if( pointClock <= last_clock ){
                                continue;
                            }
                            local pointValue = point.Y;
                            local a = (pointValue - last_val) / (pointClock - last_clock);
                            local j;
                            for j = last_clock; j <= pointClock; j++
                                local val = start_dyn + org.kbinani.PortUtil.castToInt( (j - last_clock) * a );
                                if( val < dyn.getMinimum() ){
                                    val = dyn.getMinimum();
                                }else if( dyn.getMaximum() < val ){
                                    val = dyn.getMaximum();
                                }
                                if( val ~= last ){
                                    dyn.add( j, val );
                                    last = val;
                                }
                            }
                            last_val = point.Y;
                            last_clock = pointClock;
                        }

                        -- bplistの末尾から，clock => clock + lengthまでのデータ点を追加
                        local last2 = last;
                        if( last_clock < clock + length ){
                            local a = (handle.getEndDyn() - last_val) / (clock + length - last_clock);
                            local j;
                            for j = last_clock; j < clock + length; j++
                                local val = last2 + org.kbinani.PortUtil.castToInt( (j - last_clock) * a );
                                if( val < dyn.getMinimum() ){
                                    val = dyn.getMinimum();
                                }else if( dyn.getMaximum() < val ){
                                    val = dyn.getMaximum();
                                }
                                if( val ~= last ){
                                    dyn.add( j, val );
                                    last = val;
                                }
                            }
                        }
                    }
                }
            }
        }
        */

    /**
     * @brief 指定したゲートタイムにおいて、歌唱を担当している歌手の歌手変更イベントを取得する
     * @param clock ゲートタイム
     * @return 歌手イベント。存在しなければ null を返す
     */
    const VSQ_NS::Event *getSingerEventAt( VSQ_NS::tick_t clock ){
        const VSQ_NS::Event *last = 0;
        VSQ_NS::Event::List *events = this->events();
        VSQ_NS::EventListIndexIterator itr = getIndexIterator( VSQ_NS::EventListIndexIteratorKind::SINGER );
        while( itr.hasNext() ){
            int index = itr.next();
            const VSQ_NS::Event *item = events->get( index );
            if( clock < item->clock ){
                return last;
            }
            last = item;
        }
        return last;
    }


    /**
        -- このトラックに設定されているイベントを，ゲートタイム順に並べ替えます．
        --
        -- @reutrn [void]
        function this:sortEvent()
            self.events:sort();
        }
        */

    /**
        -- レンダラーを変更します
        --
        -- @param new_renderer [string]
        -- @param singers [Array<VsqID>]
        function this:changeRenderer( new_renderer, singers )
            local default_id = nil;
            local singers_size = #singers;
            if( singers_size <= 0 ){
                default_id = Id.new();
                default_id.type = EventTypeEnum.SINGER;
                local singer_handle = Handle.new( HandleTypeEnum.SINGER );
                singer_handle.IconID = "$0701" + org.kbinani.PortUtil.sprintf( "%04X", 0 );
                singer_handle.ids = "Unknown";
                singer_handle.Index = 0;
                singer_handle.Language = 0;
                singer_handle.setLength( 1 );
                singer_handle.Original = 0;
                singer_handle.Program = 0;
                singer_handle.Caption = "";
                default_id.singerHandle = singer_handle;
            else
                default_id = singers[0];
            }

            local itr;
            for ( itr = self.getSingerEventIterator(); itr.hasNext();
                local ve = itr.next();
                local singer_handle = ve.IconHandle;
                local program = singer_handle.Program;
                local found = false;
                local i;
                for i = 0; i < singers_size; i++
                    local id = singers[i];
                    if( program == singer_handle.Program ){
                        ve.id = id:clone();
                        found = true;
                        break;
                    }
                }
                if( !found ){
                    local add = default_id:clone();
                    add.IconHandle.Program = program;
                    ve.id = add;
                }
            }
            self.common.Version = new_renderer;
        }
        */

    const BPList *curve(const std::string &curveName)const {
        string search = StringUtil::toLower(curveName);
        std::map<std::string, BPList *>::const_iterator index
                = curveNameMap.find(search);
        if (index == curveNameMap.end()) {
            return 0;
        } else {
            return index->second;
        }
    }

    /**
     * @brief 指定された名前のカーブを取得します
     * @param curve (string) カーブ名
     * @return (BPList) カーブ
     */
    BPList *curve(const std::string &curveName) {
        string search = StringUtil::toLower(curveName);
        std::map<std::string, BPList *>::const_iterator index
                = curveNameMap.find(search);
        if (index == curveNameMap.end()) {
            return 0;
        } else {
            return index->second;
        }
    }

    /*
     * @brief 指定された名前のカーブを設定する
     * @param curve (string) カーブ名
     * @param value (BPList) 設定するカーブ
     */
    /*void setCurve( const std::string curve, BPList value ){
        string search = StringUtil::toLower( curve );
        if( search == "bre" ){
            _bre = value;
        }else if( search == "bri" ){
            _bri = value;
        }else if( search == "cle" ){
            _cle = value;
        }else if( search == "dyn" ){
            _dyn = value;
        }else if( search == "gen" ){
            _gen = value;
        }else if( search == "ope" ){
            _ope = value;
        }else if( search == "pbs" ){
            _pbs = value;
        }else if( search == "pit" ){
            _pit = value;
        }else if( search == "por" ){
            _por = value;
        }else if( search == "harmonics" ){
            _harmonics = value;
        }else if( search == "fx2depth" ){
            _fx2depth = value;
        }else if( search == "reso1amp" ){
            _reso1AmpBPList = value;
        }else if( search == "reso1bw" ){
            _reso1BWBPList = value;
        }else if( search == "reso1freq" ){
            _reso1FreqBPList = value;
        }else if( search == "reso2amp" ){
            _reso2AmpBPList = value;
        }else if( search == "reso2bw" ){
            _reso2BWBPList = value;
        }else if( search == "reso2freq" ){
            _reso2FreqBPList = value;
        }else if( search == "reso3amp" ){
            _reso3AmpBPList = value;
        }else if( search == "reso3bw" ){
            _reso3BWBPList = value;
        }else if( search == "reso3freq" ){
            _reso3FreqBPList = value;
        }else if( search == "reso4amp" ){
            _reso4AmpBPList = value;
        }else if( search == "reso4bw" ){
            _reso4BWBPList = value;
        }else if( search == "reso4freq" ){
            _reso4FreqBPList = value;
        }
    }*/

    /**
     * @brief コピーを作成する
     * @return (Track) このオブジェクトのコピー
     */
    Track clone()const{
        Track res;
        deepCopy( &res );
        return res;
    }

    /*
        -- 歌詞の文字数を調べます
        -- @return [int]
        function this:getLyricLength()
            local counter = 0;
            local i;
            for i = 0; i < self.events.size(); i++
                if( self.events:getElement( i ).type == EventTypeEnum.NOTE ){
                    counter++;
                }
            }
            return counter;
        }
        */

    /**
     * @brief イベントリストを取得する
     * @return イベントリストのポインタ
     */
    Event::List *events(){
        return &_events;
    }

    const Event::List *events()const{
        return &_events;
    }

    /**
     * @brief Get an instance of Common.
     * @todo Rename to common.
     */
    Common *common() {
        return &_common;
    }

    /**
     * @brief Get an instance of Common.
     * @todo Rename to common.
     */
    const Common *common()const {
        return &_common;
    }

    /**
     * @brief Get a list of curve name for this track.
     * @return A list of curve name.
     */
    virtual const std::vector<std::string> *curveNameList()const {
        static std::vector<std::string> vocaloidCurveNameList;
        if (vocaloidCurveNameList.empty()) {
            vocaloidCurveNameList.push_back("Accent");
            vocaloidCurveNameList.push_back("Decay");
            vocaloidCurveNameList.push_back("Env");
            vocaloidCurveNameList.push_back("DYN");
            vocaloidCurveNameList.push_back("BRE");
            vocaloidCurveNameList.push_back("BRI");
            vocaloidCurveNameList.push_back("CLE");
            vocaloidCurveNameList.push_back("OPE");
            vocaloidCurveNameList.push_back("GEN");
            vocaloidCurveNameList.push_back("POR");
            vocaloidCurveNameList.push_back("PIT");
            vocaloidCurveNameList.push_back("PBS");
            vocaloidCurveNameList.push_back("VibratoRate");
            vocaloidCurveNameList.push_back("VibratoDepth");
            vocaloidCurveNameList.push_back("harmonics");
            vocaloidCurveNameList.push_back("fx2depth");
            vocaloidCurveNameList.push_back("reso1freq");
            vocaloidCurveNameList.push_back("reso1bw");
            vocaloidCurveNameList.push_back("reso1amp");
            vocaloidCurveNameList.push_back("reso2freq");
            vocaloidCurveNameList.push_back("reso2bw");
            vocaloidCurveNameList.push_back("reso2amp");
            vocaloidCurveNameList.push_back("reso3freq");
            vocaloidCurveNameList.push_back("reso3bw");
            vocaloidCurveNameList.push_back("reso3amp");
            vocaloidCurveNameList.push_back("reso4freq");
            vocaloidCurveNameList.push_back("reso4bw");
            vocaloidCurveNameList.push_back("reso4amp");
        }
        return &vocaloidCurveNameList;
    }

protected:
    /**
     * @brief Get a map of section name in vsq and curve name.
     */
    virtual std::map<std::string, std::string> getSectionNameMap()const {
        static std::map<std::string, std::string> result;
        if (result.empty()) {
            result.insert(std::make_pair("pit", "[PitchBendBPList]"));
            result.insert(std::make_pair("pbs", "[PitchBendSensBPList]"));
            result.insert(std::make_pair("dyn", "[DynamicsBPList]"));
            result.insert(std::make_pair("bre", "[EpRResidualBPList]"));
            result.insert(std::make_pair("bri", "[EpRESlopeBPList]"));
            result.insert(std::make_pair("cle", "[EpRESlopeDepthBPList]"));
            result.insert(std::make_pair("harmonics", "[EpRSineBPList]"));
            result.insert(std::make_pair("fx2depth", "[VibTremDepthBPList]"));
            result.insert(std::make_pair("reso1Freq", "[Reso1FreqBPList]"));
            result.insert(std::make_pair("reso2Freq", "[Reso2FreqBPList]"));
            result.insert(std::make_pair("reso3Freq", "[Reso3FreqBPList]"));
            result.insert(std::make_pair("reso4Freq", "[Reso4FreqBPList]"));
            result.insert(std::make_pair("reso1BW", "[Reso1BWBPList]"));
            result.insert(std::make_pair("reso2BW", "[Reso2BWBPList]"));
            result.insert(std::make_pair("reso3BW", "[Reso3BWBPList]"));
            result.insert(std::make_pair("reso4BW", "[Reso4BWBPList]"));
            result.insert(std::make_pair("reso1Amp", "[Reso1AmpBPList]"));
            result.insert(std::make_pair("reso2Amp", "[Reso2AmpBPList]"));
            result.insert(std::make_pair("reso3Amp", "[Reso3AmpBPList]"));
            result.insert(std::make_pair("reso4Amp", "[Reso4AmpBPList]"));
            result.insert(std::make_pair("gen", "[GenderFactorBPList]"));
            result.insert(std::make_pair("por", "[PortamentoTimingBPList]"));
            result.insert(std::make_pair("ope", "[OpeningBPList]"));
        }
        return result;
    }

private:
    /**
     * @brief 初期化を行う
     * @param name (string)
     * @param singer (string)
     */
    void _initCor( std::string name, std::string singer ){
        this->_common = Common(name, 179, 181, 123, DynamicsMode::EXPERT, PlayMode::PLAY_WITH_SYNTH);
        this->_pit = BPList( "pit", 0, -8192, 8191 );
        this->_pbs = BPList( "pbs", 2, 0, 24 );
        this->_dyn = BPList( "dyn", 64, 0, 127 );
        this->_bre = BPList( "bre", 0, 0, 127 );
        this->_bri = BPList( "bri", 64, 0, 127 );
        this->_cle = BPList( "cle", 0, 0, 127 );
        this->_reso1FreqBPList = BPList( "reso1freq", 64, 0, 127 );
        this->_reso2FreqBPList = BPList( "reso2freq", 64, 0, 127 );
        this->_reso3FreqBPList = BPList( "reso3freq", 64, 0, 127 );
        this->_reso4FreqBPList = BPList( "reso4freq", 64, 0, 127 );
        this->_reso1BWBPList = BPList( "reso1bw", 64, 0, 127 );
        this->_reso2BWBPList = BPList( "reso2bw", 64, 0, 127 );
        this->_reso3BWBPList = BPList( "reso3bw", 64, 0, 127 );
        this->_reso4BWBPList = BPList( "reso4bw", 64, 0, 127 );
        this->_reso1AmpBPList = BPList( "reso1amp", 64, 0, 127 );
        this->_reso2AmpBPList = BPList( "reso2amp", 64, 0, 127 );
        this->_reso3AmpBPList = BPList( "reso3amp", 64, 0, 127 );
        this->_reso4AmpBPList = BPList( "reso4amp", 64, 0, 127 );
        this->_harmonics = BPList( "harmonics", 64, 0, 127 );
        this->_fx2depth = BPList( "fx2depth", 64, 0, 127 );
        this->_gen = BPList( "gen", 64, 0, 127 );
        this->_por = BPList( "por", 64, 0, 127 );
        this->_ope = BPList( "ope", 127, 0, 127 );

        setupCurveNameMap();

        Event event( 0, EventType::SINGER );
        Handle ish( HandleType::SINGER );
        ish.iconId = "$07010000";
        ish.ids = singer;
        ish.original = 0;
        ish.setLength( 1 );
        ish.language = 0;
        ish.program = 0;
        event.singerHandle = ish;
        this->_events.add( event );
    }

    /**
     * @brief Deep copy instance.
     * @param destination Copy destination
     */
    void deepCopy( Track *destination )const{
        destination->setName( getName() );

        destination->_common = _common.clone();
        destination->_events.clear();
        for( int i = 0; i < _events.size(); i++ ){
            const Event *item = _events.get( i );
            destination->_events.add( item->clone(), item->id );
        }
        destination->_pit = _pit.clone();
        destination->_pbs = _pbs.clone();
        destination->_dyn = _dyn.clone();
        destination->_bre = _bre.clone();
        destination->_bri = _bri.clone();
        destination->_cle = _cle.clone();
        destination->_reso1FreqBPList = _reso1FreqBPList.clone();
        destination->_reso2FreqBPList = _reso2FreqBPList.clone();
        destination->_reso3FreqBPList = _reso3FreqBPList.clone();
        destination->_reso4FreqBPList = _reso4FreqBPList.clone();
        destination->_reso1BWBPList = _reso1BWBPList.clone();
        destination->_reso2BWBPList = _reso2BWBPList.clone();
        destination->_reso3BWBPList = _reso3BWBPList.clone();
        destination->_reso4BWBPList = _reso4BWBPList.clone();
        destination->_reso1AmpBPList = _reso1AmpBPList.clone();
        destination->_reso2AmpBPList = _reso2AmpBPList.clone();
        destination->_reso3AmpBPList = _reso3AmpBPList.clone();
        destination->_reso4AmpBPList = _reso4AmpBPList.clone();
        destination->_harmonics = _harmonics.clone();
        destination->_fx2depth = _fx2depth.clone();
        destination->_gen = _gen.clone();
        destination->_por = _por.clone();
        destination->_ope = _ope.clone();

        destination->setupCurveNameMap();
    }

    void setupCurveNameMap() {
        curveNameMap.clear();
        curveNameMap.insert(std::make_pair("bre", &_bre));
        curveNameMap.insert(std::make_pair("bri", &_bri));
        curveNameMap.insert(std::make_pair("cle", &_cle));
        curveNameMap.insert(std::make_pair("dyn", &_dyn));
        curveNameMap.insert(std::make_pair("gen", &_gen));
        curveNameMap.insert(std::make_pair("ope", &_ope));
        curveNameMap.insert(std::make_pair("pbs", &_pbs));
        curveNameMap.insert(std::make_pair("pit", &_pit));
        curveNameMap.insert(std::make_pair("por", &_por));
        curveNameMap.insert(std::make_pair("harmonics", &_harmonics));
        curveNameMap.insert(std::make_pair("fx2depth", &_fx2depth));
        curveNameMap.insert(std::make_pair("reso1amp", &_reso1AmpBPList));
        curveNameMap.insert(std::make_pair("reso1bw", &_reso1BWBPList));
        curveNameMap.insert(std::make_pair("reso1freq", &_reso1FreqBPList));
        curveNameMap.insert(std::make_pair("reso2amp", &_reso2AmpBPList));
        curveNameMap.insert(std::make_pair("reso2bw", &_reso2BWBPList));
        curveNameMap.insert(std::make_pair("reso2freq", &_reso2FreqBPList));
        curveNameMap.insert(std::make_pair("reso3amp", &_reso3AmpBPList));
        curveNameMap.insert(std::make_pair("reso3bw", &_reso3BWBPList));
        curveNameMap.insert(std::make_pair("reso3freq", &_reso3FreqBPList));
        curveNameMap.insert(std::make_pair("reso4amp", &_reso4AmpBPList));
        curveNameMap.insert(std::make_pair("reso4bw", &_reso4BWBPList));
        curveNameMap.insert(std::make_pair("reso4freq", &_reso4FreqBPList));
    }
};

VSQ_END_NAMESPACE

#endif
