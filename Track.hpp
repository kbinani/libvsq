--[[
  Track.lua
  Copyright © 2011 kbinani

  This file is part of luavsq.

  luavsq is free software; you can redistribute it and/or
  modify it under the terms of the BSD License.

  luavsq is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
]]

local type = type;

module( "luavsq" );

---
-- トラックを表すクラス
-- @class table
-- @name Track
Track = {};

--
-- 初期化を行う
-- @return (Track)
function Track.new( ... )
    local this = {};
    local arguments = { ... };

    ---
    -- トラックに付属するタグ情報
    -- @var ?
    this.tag = "";

    ---
    -- トラック名などの情報
    -- @var Common
    this.common = nil;

    ---
    -- マスター情報。Master Track のみが保持する
    -- @var Master
    this.master = nil;

    ---
    -- ミキサー情報。Master Track のみが保持する
    -- @var Mixer
    this.mixer = nil;

    ---
    -- イベントの一覧
    -- @var EventList
    this.events = nil;

    ---
    --  PIT。ピッチベンド(pitchBendBPList)。default=0
    -- @var BPList
    -- @access private
    this._pit = nil;

    ---
    --  PBS。ピッチベンドセンシティビティ(pitchBendSensBPList)。dfault=2
    -- @var BPList
    -- @access private
    this._pbs = nil;

    ---
    --  DYN。ダイナミクス(dynamicsBPList)。default=64
    -- @var BPList
    -- @access private
    this._dyn = nil;

    ---
    --  BRE。ブレシネス(epRResidualBPList)。default=0
    -- @var BPList
    -- @access private
    this._bre = nil;

    ---
    --  BRI。ブライトネス(epRESlopeBPList)。default=64
    -- @var BPList
    -- @access private
    this._bri = nil;

    ---
    --  CLE。クリアネス(epRESlopeDepthBPList)。default=0
    -- @var BPList
    -- @access private
    this._cle = nil;

    ---
    -- @var BPList
    -- @access private
    this._reso1FreqBPList = nil;

    ---
    -- @var BPList
    -- @access private
    this._reso2FreqBPList = nil;

    ---
    -- @var BPList
    -- @access private
    this._reso3FreqBPList = nil;

    ---
    -- @var BPList
    -- @access private
    this._reso4FreqBPList = nil;

    ---
    -- @var BPList
    -- @access private
    this._reso1BWBPList = nil;

    ---
    -- @var BPList
    -- @access private
    this._reso2BWBPList = nil;

    ---
    -- @var BPList
    -- @access private
    this._reso3BWBPList = nil;

    ---
    -- @var BPList
    -- @access private
    this._reso4BWBPList = nil;

    ---
    -- @var BPList
    -- @access private
    this._reso1AmpBPList = nil;

    ---
    -- @var BPList
    -- @access private
    this._reso2AmpBPList = nil;

    ---
    -- @var BPList
    -- @access private
    this._reso3AmpBPList = nil;

    ---
    -- @var BPList
    -- @access private
    this._reso4AmpBPList = nil;

    ---
    --  Harmonics。(EpRSineBPList)default = 64
    -- @var BPList
    -- @access private
    this._harmonics = nil;

    ---
    --  Effect2 Depth。
    -- @var BPList
    -- @access private
    this._fx2depth = nil;

    ---
    --  GEN。ジェンダーファクター(genderFactorBPList)。default=64
    -- @var BPList
    -- @access private
    this._gen = nil;

    ---
    -- POR。ポルタメントタイミング(portamentoTimingBPList)。default=64
    -- @var BPList
    -- @access private
    this._por = nil;

    ---
    --  OPE。オープニング(openingBPList)。default=127
    -- @var BPList
    -- @access private
    this._ope = nil;

    ---
    -- cent単位のピッチベンド。vsqに保存するときは、VsqFile#reflectPitchによってPIT, PBSに落とし込む。それらの範囲をオーバーしてたら知らん(cutoff)
    -- @var BPList
    -- @access private
    this._pitch = nil;

    ---
    -- Master Trackを構築
    -- @return (Track)
    -- @name new<!--1-->
    -- @access static ctor
    function this:_init_0()
    end

    ---
    -- Master Trackでないトラックを構築
    -- @param name (string) トラック名
    -- @param singer (string) トラックのデフォルトの歌手名
    -- @return (Track)
    -- @name new<!--2-->
    -- @access static ctor
    function this:_init_2a( name, singer )
        self:_initCor( name, singer );
    end

    --[[
        -- @param midi_event [Array<MidiEvent>]
        -- @param encoding [string]
        function this:_init_2b( midi_event, encoding )
            local track_name = "";

            local sw = nil;
            sw = TextStream.new();
            local count = #midi_event;
            local buffer = Array.new(); -- Vector<Integer>();
            local i;
            for i = 0; i < count; i++
                local item = midi_event[i];
                if( item.firstByte == 0xff and #item.data > 0 )then
                    -- meta textを抽出
                    local type = item.data[0];
                    if( type == 0x01 or type == 0x03 )then
                        if( type == 0x01 )then
                            local colon_count = 0;
                            local j;
                            for j = 0; j < #item.data - 1; j++
                                local d = item.data[j + 1];
                                if( d == 0x3a )then
                                    colon_count++;
                                    if( colon_count <= 2 )then
                                        continue;
                                    end
                                end
                                if( colon_count < 2 )then
                                    continue;
                                end
                                buffer.push( d );
                            end

                            local index_0x0a = org.kbinani.PortUtil.arrayIndexOf( buffer, 0x0a );
                            while( index_0x0a >= 0 )do
                                local cpy = Array.new( index_0x0a );
                                local j;
                                for j = 0; j < index_0x0a; j++
                                    cpy[j] = 0xff & buffer[0];
                                    buffer.shift();
                                end

                                local line = org.kbinani.Cp932.convertToUTF8( cpy );
--alert( "VsqTrack#_init_2b; line=" + line );
                                sw:writeLine( line );
                                buffer.shift();
                                index_0x0a = org.kbinani.PortUtil.arrayIndexOf( buffer, 0x0a );
                            end
                        else
                            local j;
                            for j = 0; j < #item.data - 1; j++
                                buffer.push( item.data[j + 1] );
                            end
                            local c = #buffer;
                            local d = Array.new( c );
                            local j;
                            for j = 0; j < c; j++
                                d[j] = 0xff & buffer[j];
                            end
                            track_name = org.kbinani.Cp932.convertToUTF8( d );
                            buffer.splice( 0, #buffer );
                        end
                    end
                else
                    continue;
                end
            end

            local remain = #buffer;
            if( remain > 0 )then
                local cpy = Array.new( remain );
                local j;
                for j = 0; j < remain; j++
                    cpy[j] = 0xff & buffer[j];
                end
                local line = org.kbinani.Cp932.convertToUTF8( cpy );
                sw:writeLine( line );
            end

            sw:setPointer( -1 );
            self.MetaText = MetaText.new( sw );
            self.setName( track_name );
        end]]

    ---
    -- 初期化を行う
    -- @param name (string)
    -- @param singer (string)
    -- @access private
    function this:_initCor( name, singer )
        self.common = Common.new( name, 179, 181, 123, 1, 1 );
        self._pit = BPList.new( "pit", 0, -8192, 8191 );
        self._pbs = BPList.new( "pbs", 2, 0, 24 );
        self._dyn = BPList.new( "dyn", 64, 0, 127 );
        self._bre = BPList.new( "bre", 0, 0, 127 );
        self._bri = BPList.new( "bri", 64, 0, 127 );
        self._cle = BPList.new( "cle", 0, 0, 127 );
        self._reso1FreqBPList = BPList.new( "reso1freq", 64, 0, 127 );
        self._reso2FreqBPList = BPList.new( "reso2freq", 64, 0, 127 );
        self._reso3FreqBPList = BPList.new( "reso3freq", 64, 0, 127 );
        self._reso4FreqBPList = BPList.new( "reso4freq", 64, 0, 127 );
        self._reso1BWBPList = BPList.new( "reso1bw", 64, 0, 127 );
        self._reso2BWBPList = BPList.new( "reso2bw", 64, 0, 127 );
        self._reso3BWBPList = BPList.new( "reso3bw", 64, 0, 127 );
        self._reso4BWBPList = BPList.new( "reso4bw", 64, 0, 127 );
        self._reso1AmpBPList = BPList.new( "reso1amp", 64, 0, 127 );
        self._reso2AmpBPList = BPList.new( "reso2amp", 64, 0, 127 );
        self._reso3AmpBPList = BPList.new( "reso3amp", 64, 0, 127 );
        self._reso4AmpBPList = BPList.new( "reso4amp", 64, 0, 127 );
        self._harmonics = BPList.new( "harmonics", 64, 0, 127 );
        self._fx2depth = BPList.new( "fx2depth", 64, 0, 127 );
        self._gen = BPList.new( "gen", 64, 0, 127 );
        self._por = BPList.new( "por", 64, 0, 127 );
        self._ope = BPList.new( "ope", 127, 0, 127 );
        self._pitch = BPList.new( "pitch", 0, -15000, 15000 );
        --[[if ( is_first_track ) {
                self.master = Master.new( pre_measure );
            } else {
                self.master = null;
            }]]
        self.events = EventList.new();

        local event = Event.new( 0, EventTypeEnum.SINGER );
        local ish = Handle.new( HandleTypeEnum.SINGER );
        ish.iconId = "$07010000";
        ish.ids = singer;
        ish.original = 0;
        ish.caption = "";
        ish:setLength( 1 );
        ish.language = 0;
        ish.program = 0;
        event.singerHandle = ish;
        self.events:add( event );
    end

    ---
    -- 指定された種類のイベントのインデクスを順に返す反復子を取得する
    -- @param iteratorKind (EventList.IndexIteratorKindEnum) 反復子の種類
    -- @return (EventList.IndexIterator) 反復子
    function this:getIndexIterator( iteratorKind )
        return EventList.IndexIterator.new( self.events, iteratorKind );
    end

    --[[
        -- このトラックの再生モードを取得します．
        --
        -- @return [int] PlayMode.PlayAfterSynthまたはPlayMode.PlayWithSynth
        function this:getPlayMode()
            if( self.common == nil )then
                return PlayModeEnum.PLAY_WITH_SYNTH;
            end
            if( self.common.lastPlayMode ~= PlayModeEnum.PLAY_AFTER_SYNTH and
                 self.common.lastPlayMode ~= PlayModeEnum.PLAY_WITH_SYNTH )then
                self.common.lastPlayMode = PlayModeEnum.PLAY_WITH_SYNTH;
            end
            return self.common.lastPlayMode;
        end]]

    --[[
        -- このトラックの再生モードを設定します．
        --
        -- @param value [int] PlayMode.PlayAfterSynth, PlayMode.PlayWithSynth, またはPlayMode.Offのいずれかを指定します
        -- @return [void]
        function this:setPlayMode( value )
            if( self.MetaText == nil ) return;
            if( self.common == nil )then
                self.common = Common.new( "Miku", 128, 128, 128, DynamicsModeEnum.EXPERT, value );
                return;
            end
            if( value == PlayModeEnum.OFF )then
                if( self.common.playMode ~= PlayModeEnum.OFF )then
                    self.common.lastPlayMode = self.common.playMode;
                end
            else
                self.common.lastPlayMode = value;
            end
            self.common.playMode = value;
        end]]

    --[[
        -- このトラックがレンダリングされるかどうかを取得します．
        --
        -- @return [bool]
        function this:isTrackOn()
            if( self.MetaText == nil ) return true;
            if( self.common == nil ) return true;
            return self.common.playMode ~= PlayModeEnum.OFF;
        end]]

    --[[
        -- このトラックがレンダリングされるかどうかを設定します，
        --
        -- @param value [bool]
        function this:setTrackOn( value )
            if( self.MetaText == nil ) return;
            if( self.common == nil )then
                self.common = Common.new( "Miku", 128, 128, 128, DynamicsModeEnum.EXPERT, value ? PlayModeEnum.PLAY_WITH_SYNTH : PlayModeEnum.OFF );
            end
            if( value )then
                if( self.common.lastPlayMode ~= PlayModeEnum.PLAY_AFTER_SYNTH and
                     self.common.lastPlayMode ~= PlayModeEnum.PLAY_WITH_SYNTH )then
                    self.common.lastPlayMode = PlayModeEnum.PLAY_WITH_SYNTH;
                end
                self.common.playMode = self.common.lastPlayMode;
            else
                if( self.common.playMode == PlayModeEnum.PLAY_AFTER_SYNTH or
                     self.common.playMode == PlayModeEnum.PLAY_WITH_SYNTH )then
                    self.common.lastPlayMode = self.common.playMode;
                end
                self.common.playMode = PlayModeEnum.OFF;
            end
        end]]

    ---
    -- トラックの名前を取得する
    -- @return (string) トラック名
    function this:getName()
        if( self.common == nil )then
            return "Master Track";
        else
            return self.common.name;
        end
    end

    ---
    -- トラックの名前を設定する
    -- @param value (string) トラック名
    function this:setName( value )
        if( self.common ~= nil )then
            self.common.name = value;
        end
    end

    --[[
        -- このトラックの，指定したゲートタイムにおけるピッチベンドを取得します．単位はCentです．
        --
        -- @param clock [int] ピッチベンドを取得するゲートタイム
        -- @return [double]
        function this:getPitchAt( clock )
            local inv2_13 = 1.0 / 8192.0;
            local pit = self.PIT.getValue( clock );
            local pbs = self.PBS.getValue( clock );
            return pit * pbs * inv2_13 * 100.0;
        end]]

    --[[
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
                if( handle == nil )then
                    continue;
                end
                local clock = item.Clock;
                local length = item.getLength();

                if( handle.isDynaffType() )then
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
                        if( clock <= c and c <= clock + length )then
                            dyn.removeElementAt( i );
                        elseif( c < clock )then
                            break;
                        end
                    end

                    local bplist = handle.getDynBP();
                    if( bplist == nil or (bplist ~= nil and bplist.size() <= 0) )then
                        -- カーブデータが無い場合
                        local a = 0.0;
                        if( length > 0 )then
                            a = (handle.getEndDyn() - handle.getStartDyn()) / length;
                        end
                        local last_val = start_dyn;
                        local i;
                        for i = clock; i < clock + length; i++
                            local val = start_dyn + org.kbinani.PortUtil.castToInt( a * (i - clock) );
                            if( val < dyn.getMinimum() )then
                                val = dyn.getMinimum();
                            elseif( dyn.getMaximum() < val )then
                                val = dyn.getMaximum();
                            end
                            if( last_val ~= val )then
                                dyn.add( i, val );
                                last_val = val;
                            end
                        end
                    else
                        -- カーブデータがある場合
                        local last_val = handle.getStartDyn();
                        local last_clock = clock;
                        local bpnum = bplist:size();
                        local last = start_dyn;

                        -- bplistに指定されている分のデータ点を追加
                        local i;
                        for i = 0; i < bpnum; i++
                            local point = bplist.getElement( i );
                            local pointClock = clock + org.kbinani.PortUtil.castToInt( length * point.X );
                            if( pointClock <= last_clock )then
                                continue;
                            end
                            local pointValue = point.Y;
                            local a = (pointValue - last_val) / (pointClock - last_clock);
                            local j;
                            for j = last_clock; j <= pointClock; j++
                                local val = start_dyn + org.kbinani.PortUtil.castToInt( (j - last_clock) * a );
                                if( val < dyn.getMinimum() )then
                                    val = dyn.getMinimum();
                                elseif( dyn.getMaximum() < val )then
                                    val = dyn.getMaximum();
                                end
                                if( val ~= last )then
                                    dyn.add( j, val );
                                    last = val;
                                end
                            end
                            last_val = point.Y;
                            last_clock = pointClock;
                        end

                        -- bplistの末尾から，clock => clock + lengthまでのデータ点を追加
                        local last2 = last;
                        if( last_clock < clock + length )then
                            local a = (handle.getEndDyn() - last_val) / (clock + length - last_clock);
                            local j;
                            for j = last_clock; j < clock + length; j++
                                local val = last2 + org.kbinani.PortUtil.castToInt( (j - last_clock) * a );
                                if( val < dyn.getMinimum() )then
                                    val = dyn.getMinimum();
                                elseif( dyn.getMaximum() < val )then
                                    val = dyn.getMaximum();
                                end
                                if( val ~= last )then
                                    dyn.add( j, val );
                                    last = val;
                                end
                            end
                        end
                    end
                end
            end
        end]]

    --[[
        -- 指定したゲートタイムにおいて、歌唱を担当している歌手のVsqEventを取得します．
        --
        -- @param clock [int]
        -- @return [VsqEvent]
        function this:getSingerEventAt( clock )
            local last = nil;
            local itr;
            for itr = self.getSingerEventIterator(); itr.hasNext();
                local item = itr.next();
                if( clock < item.Clock )then
                    return last;
                end
                last = item;
            end
            return last;
        end]]

    --[[
        -- このトラックに設定されているイベントを，ゲートタイム順に並べ替えます．
        --
        -- @reutrn [void]
        function this:sortEvent()
            self.events:sort();
        end]]

    --
    -- トラックのメタテキストを、テキストストリームに出力する
    function this:printMetaText( ... )
        local arguments = { ... };
        if( #arguments == 3 )then
            self:_printMetaText_3( arguments[1], arguments[2], arguments[3] );
        elseif( #arguments == 4 )then
            self:_printMetaText_4( arguments[1], arguments[2], arguments[3], arguments[4] );
        end
    end

    ---
    -- トラックのメタテキストを、テキストストリームに出力する
    -- @param stream (TextStream) 出力先のストリーム
    -- @param eos (int) イベントリストの末尾を表す番号
    -- @param start (int) Tick 単位の出力開始時刻
    -- @name printMetaText<!--1-->
    function this:_printMetaText_3( stream, eos, start )
        self:_printMetaText_4( stream, eos, start, false );
    end

    ---
    -- トラックのメタテキストを、テキストストリームに出力する
    -- @param stream (TextStream) 出力先のストリーム
    -- @param eos (int) イベントリストの末尾を表す番号
    -- @param start (int) Tick 単位の出力開始時刻
    -- @param printPitch (boolean) pitch を含めて出力するかどうか(現在は <code>false</code> 固定で、引数は無視される)
    -- @name printMetaText<!--2-->
    function this:_printMetaText_4( stream, eos, start, printPitch )
        if( self.common ~= nil )then
            self.common:write( stream );
        end
        if( self.master ~= nil )then
            self.master:write( stream );
        end
        if( self.mixer ~= nil )then
            self.mixer:write( stream );
        end
        local handle = self.events:write( stream, eos );
        local itr = self.events:iterator()
        while( itr:hasNext() )do
            local item = itr:next();
            item:write( stream );
        end
        local i;
        for i = 1, #handle, 1 do
            handle[i]:write( stream );
        end
        local version = self.common.version;
        if( self._pit:size() > 0 )then
            self._pit:print( stream, start, "[PitchBendBPList]" );
        end
        if( self._pbs:size() > 0 )then
            self._pbs:print( stream, start, "[PitchBendSensBPList]" );
        end
        if( self._dyn:size() > 0 )then
            self._dyn:print( stream, start, "[DynamicsBPList]" );
        end
        if( self._bre:size() > 0 )then
            self._bre:print( stream, start, "[EpRResidualBPList]" );
        end
        if( self._bri:size() > 0 )then
            self._bri:print( stream, start, "[EpRESlopeBPList]" );
        end
        if( self._cle:size() > 0 )then
            self._cle:print( stream, start, "[EpRESlopeDepthBPList]" );
        end
        if( version:sub( 1, 4 ) == "DSB2" )then
            if( self._harmonics:size() > 0 )then
                self._harmonics:print( stream, start, "[EpRSineBPList]" );
            end
            if( self._fx2depth:size() > 0 )then
                self._fx2depth:print( stream, start, "[VibTremDepthBPList]" );
            end

            if( self._reso1FreqBPList:size() > 0 )then
                self._reso1FreqBPList:print( stream, start, "[Reso1FreqBPList]" );
            end
            if( self._reso2FreqBPList:size() > 0 )then
                self._reso2FreqBPList:print( stream, start, "[Reso2FreqBPList]" );
            end
            if( self._reso3FreqBPList:size() > 0 )then
                self._reso3FreqBPList:print( stream, start, "[Reso3FreqBPList]" );
            end
            if( self._reso4FreqBPList:size() > 0 )then
                self._reso4FreqBPList:print( stream, start, "[Reso4FreqBPList]" );
            end

            if( self._reso1BWBPList:size() > 0 )then
                self._reso1BWBPList:print( stream, start, "[Reso1BWBPList]" );
            end
            if( self._reso2BWBPList:size() > 0 )then
                self._reso2BWBPList:print( stream, start, "[Reso2BWBPList]" );
            end
            if( self._reso3BWBPList:size() > 0 )then
                self._reso3BWBPList:print( stream, start, "[Reso3BWBPList]" );
            end
            if( self._reso4BWBPList:size() > 0 )then
                self._reso4BWBPList:print( stream, start, "[Reso4BWBPList]" );
            end

            if( self._reso1AmpBPList:size() > 0 )then
                self._reso1AmpBPList:print( stream, start, "[Reso1AmpBPList]" );
            end
            if( self._reso2AmpBPList:size() > 0 )then
                self._reso2AmpBPList:print( stream, start, "[Reso2AmpBPList]" );
            end
            if( self._reso3AmpBPList:size() > 0 )then
                self._reso3AmpBPList:print( stream, start, "[Reso3AmpBPList]" );
            end
            if( self._reso4AmpBPList:size() > 0 )then
                self._reso4AmpBPList:print( stream, start, "[Reso4AmpBPList]" );
            end
        end

        if( self._gen:size() > 0 )then
            self._gen:print( stream, start, "[GenderFactorBPList]" );
        end
        if( self._por:size() > 0 )then
            self._por:print( stream, start, "[PortamentoTimingBPList]" );
        end
        if( version:sub( 1, 4 ) == "DSB3" )then
            if( self._ope:size() > 0 )then
                self._ope:print( stream, start, "[OpeningBPList]" );
            end
        end
    end

    --[[
        -- レンダラーを変更します
        --
        -- @param new_renderer [string]
        -- @param singers [Array<VsqID>]
        function this:changeRenderer( new_renderer, singers )
            local default_id = nil;
            local singers_size = #singers;
            if( singers_size <= 0 )then
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
            end

            local itr;
            for ( itr = self.getSingerEventIterator(); itr.hasNext();
                local ve = itr.next();
                local singer_handle = ve.IconHandle;
                local program = singer_handle.Program;
                local found = false;
                local i;
                for i = 0; i < singers_size; i++
                    local id = singers[i];
                    if( program == singer_handle.Program )then
                        ve.id = id:clone();
                        found = true;
                        break;
                    end
                end
                if( !found )then
                    local add = default_id:clone();
                    add.IconHandle.Program = program;
                    ve.id = add;
                end
            end
            self.common.Version = new_renderer;
        end]]

    ---
    -- 指定された名前のカーブを取得します
    -- @param curve (string) カーブ名
    -- @return (BPList) カーブ
    function this:getCurve( curve )
        local search = curve:lower();
        if( search == "bre" )then
            return self._bre;
        elseif( search == "bri" )then
            return self._bri;
        elseif( search == "cle" )then
            return self._cle;
        elseif( search == "dyn" )then
            return self._dyn;
        elseif( search == "gen" )then
            return self._gen;
        elseif( search == "ope" )then
            return self._ope;
        elseif( search == "pbs" )then
            return self._pbs;
        elseif( search == "pit" )then
            return self._pit;
        elseif( search == "por" )then
            return self._por;
        elseif( search == "harmonics" )then
            return self._harmonics;
        elseif( search == "fx2depth" )then
            return self._fx2depth;
        elseif( search == "reso1amp" )then
            return self._reso1AmpBPList;
        elseif( search == "reso1bw" )then
            return self._reso1BWBPList;
        elseif( search == "reso1freq" )then
            return self._reso1FreqBPList;
        elseif( search == "reso2amp" )then
            return self._reso2AmpBPList;
        elseif( search == "reso2bw" )then
            return self._reso2BWBPList;
        elseif( search == "reso2freq" )then
            return self._reso2FreqBPList;
        elseif( search == "reso3amp" )then
            return self._reso3AmpBPList;
        elseif( search == "reso3bw" )then
            return self._reso3BWBPList;
        elseif( search == "reso3freq" )then
            return self._reso3FreqBPList;
        elseif( search == "reso4amp" )then
            return self._reso4AmpBPList;
        elseif( search == "reso4bw" )then
            return self._reso4BWBPList;
        elseif( search == "reso4freq" )then
            return self._reso4FreqBPList;
        elseif( search == "pitch" )then
            return self._pitch;
        else
            return nil;
        end
    end

    ---
    -- 指定された名前のカーブを設定する
    -- @param curve (string) カーブ名
    -- @param value (BPList) 設定するカーブ
    function this:setCurve( curve, value )
        local search = curve:lower();
        if( search == "bre" )then
            self._bre = value;
        elseif( search == "bri" )then
            self._bri = value;
        elseif( search == "cle" )then
            self._cle = value;
        elseif( search == "dyn" )then
            self._dyn = value;
        elseif( search == "gen" )then
            self._gen = value;
        elseif( search == "ope" )then
            self._ope = value;
        elseif( search == "pbs" )then
            self._pbs = value;
        elseif( search == "pit" )then
            self._pit = value;
        elseif( search == "por" )then
            self._por = value;
        elseif( search == "harmonics" )then
            self._harmonics = value;
        elseif( search == "fx2depth" )then
            self._fx2depth = value;
        elseif( search == "reso1amp" )then
            self._reso1AmpBPList = value;
        elseif( search == "reso1bw" )then
            self._reso1BWBPList = value;
        elseif( search == "reso1freq" )then
            self._reso1FreqBPList = value;
        elseif( search == "reso2amp" )then
            self._reso2AmpBPList = value;
        elseif( search == "reso2bw" )then
            self._reso2BWBPList = value;
        elseif( search == "reso2freq" )then
            self._reso2FreqBPList = value;
        elseif( search == "reso3amp" )then
            self._reso3AmpBPList = value;
        elseif( search == "reso3bw" )then
            self._reso3BWBPList = value;
        elseif( search == "reso3freq" )then
            self._reso3FreqBPList = value;
        elseif( search == "reso4amp" )then
            self._reso4AmpBPList = value;
        elseif( search == "reso4bw" )then
            self._reso4BWBPList = value;
        elseif( search == "reso4freq" )then
            self._reso4FreqBPList = value;
        elseif( search == "pitch" )then
            self._pitch = value;
        end
    end

    ---
    -- コピーを作成する
    -- @return (Track) このオブジェクトのコピー
    function this:clone()
        local res = Track.new();
        res:setName( self:getName() );

        if( self.common ~= nil )then
            res.common = self.common:clone();
        end
        if( self.master ~= nil )then
            res.master = self.master:clone();
        end
        if( self.mixer ~= nil )then
            res.mixer = self.mixer:clone();
        end
        if( self.events ~= nil )then
            res.events = EventList.new();
            local i;
            for i = 0, self.events:size() - 1, 1 do
                local item = self.events:get( i );
                res.events:add( item:clone(), item.internalID );
            end
        end
        if( self._pit ~= nil )then
            res._pit = self._pit:clone();
        end
        if( self._pbs ~= nil )then
            res._pbs = self._pbs:clone();
        end
        if( self._dyn ~= nil )then
            res._dyn = self._dyn:clone();
        end
        if( self._bre ~= nil )then
            res._bre = self._bre:clone();
        end
        if( self._bri ~= nil )then
            res._bri = self._bri:clone();
        end
        if( self._cle ~= nil )then
            res._cle = self._cle:clone();
        end
        if( self._reso1FreqBPList ~= nil )then
            res._reso1FreqBPList = self._reso1FreqBPList:clone();
        end
        if( self._reso2FreqBPList ~= nil )then
            res._reso2FreqBPList = self._reso2FreqBPList:clone();
        end
        if( self._reso3FreqBPList ~= nil )then
            res._reso3FreqBPList = self._reso3FreqBPList:clone();
        end
        if( self._reso4FreqBPList ~= nil )then
            res._reso4FreqBPList = self._reso4FreqBPList:clone();
        end
        if( self._reso1BWBPList ~= nil )then
            res._reso1BWBPList = self._reso1BWBPList:clone();
        end
        if( self._reso2BWBPList ~= nil )then
            res._reso2BWBPList = self._reso2BWBPList:clone();
        end
        if( self._reso3BWBPList ~= nil )then
            res._reso3BWBPList = self._reso3BWBPList:clone();
        end
        if( self._reso4BWBPList ~= nil )then
            res._reso4BWBPList = self._reso4BWBPList:clone();
        end
        if( self._reso1AmpBPList ~= nil )then
            res._reso1AmpBPList = self._reso1AmpBPList:clone();
        end
        if( self._reso2AmpBPList ~= nil )then
            res._reso2AmpBPList = self._reso2AmpBPList:clone();
        end
        if( self._reso3AmpBPList ~= nil )then
            res._reso3AmpBPList = self._reso3AmpBPList:clone();
        end
        if( self._reso4AmpBPList ~= nil )then
            res._reso4AmpBPList = self._reso4AmpBPList:clone();
        end
        if( self._harmonics ~= nil )then
            res._harmonics = self._harmonics:clone();
        end
        if( self._fx2depth ~= nil )then
            res._fx2depth = self._fx2depth:clone();
        end
        if( self._gen ~= nil )then
            res._gen = self._gen:clone();
        end
        if( self._por ~= nil )then
            res._por = self._por:clone();
        end
        if( self._ope ~= nil )then
            res._ope = self._ope:clone();
        end
        if( self._pitch ~= nil )then
            res._pitch = self._pitch:clone();
        end
        res.tag = self.tag;
        return res;
    end

    --[[
        -- 歌詞の文字数を調べます
        -- @return [int]
        function this:getLyricLength()
            local counter = 0;
            local i;
            for i = 0; i < self.events:size(); i++
                if( self.events:getElement( i ).type == EventTypeEnum.NOTE )then
                    counter++;
                end
            end
            return counter;
        end]]

    if( #arguments == 0 )then
        this:_init_0();
    elseif( #arguments == 2 )then
        if( type( arguments[1] ) == "string" )then
            this:_init_2a( arguments[1], arguments[2] );
        else
            this:_init_2b( arguments[1], arguments[2] );
        end
    end

    return this;
end
