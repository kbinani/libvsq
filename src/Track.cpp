/**
 * @file Track.cpp
 * Copyright © 2014 kbinani
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
#include "../include/libvsq/Track.hpp"
#include "../include/libvsq/StringUtil.hpp"
#include <memory>

LIBVSQ_BEGIN_NAMESPACE

Track::Track()
{
	this->_initCor("Track1", "Miku");
}

Track::Track(std::string const& name, std::string const& singer)
{
	this->_initCor(name, singer);
}

Track::Track(Track const& value)
{
	value.deepCopy(this);
}

Track& Track::operator = (Track const& value)
{
	value.deepCopy(this);
	return *this;
}

std::string Track::name() const
{
	//if( common == nil ){
	//    return "Master Track";
	//}else{
	return _common.name;
	//}
}

void Track::name(std::string const& value)
{
	_common.name = value;
}

EventListIndexIterator Track::getIndexIterator(EventListIndexIteratorKind iteratorKind) const
{
	return EventListIndexIterator(&this->_events, iteratorKind);
}

/**
	-- このトラックの再生モードを取得します.
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
	-- このトラックの再生モードを設定します.
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
	-- このトラックがレンダリングされるかどうかを取得します.
	--
	-- @return [bool]
	function this:isTrackOn()
		if( self.MetaText == nil ) return true;
		if( self.common == nil ) return true;
		return self.common.playMode ~= PlayModeEnum.OFF;
	}
	*/

/**
	-- このトラックがレンダリングされるかどうかを設定します,
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
	-- このトラックの, 指定したゲートタイムにおけるピッチベンドを取得します. 単位はCentです.
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
	-- クレッシェンド, デクレッシェンド, および強弱記号をダイナミクスカーブに反映させます.
	-- この操作によって, ダイナミクスカーブに設定されたデータは全て削除されます.
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
				-- クレッシェンド, デクレッシェンド
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

					-- bplistの末尾から, clock => clock + lengthまでのデータ点を追加
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

Event const* Track::singerEventAt(tick_t tick) const
{
	Event const* last = nullptr;
	Event::List const& events = this->events();
	EventListIndexIterator itr = getIndexIterator(EventListIndexIteratorKind::SINGER);
	while (itr.hasNext()) {
		int index = itr.next();
		Event const* item = events.get(index);
		if (tick < item->tick) {
			return last;
		}
		last = item;
	}
	return last;
}


/**
	-- このトラックに設定されているイベントを, ゲートタイム順に並べ替えます.
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

BPList const* Track::curve(std::string const& curveName) const
{
	std::string search = StringUtil::toLower(curveName);
	auto index = curveNameMap.find(search);
	if (index == curveNameMap.end()) {
		return nullptr;
	} else {
		return index->second.get();
	}
}

BPList* Track::curve(std::string const& curveName)
{
	std::string search = StringUtil::toLower(curveName);
	auto index = curveNameMap.find(search);
	if (index == curveNameMap.end()) {
		return nullptr;
	} else {
		return index->second.get();
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

Track Track::clone() const
{
	Track res;
	deepCopy(&res);
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

Event::List& Track::events()
{
	return _events;
}

Event::List const& Track::events() const
{
	return _events;
}

Common& Track::common()
{
	return _common;
}

Common const& Track::common() const
{
	return _common;
}

std::vector<std::string> const* Track::curveNameList() const
{
	static std::vector<std::string> vocaloid1;
	static std::vector<std::string> vocaloid2;
	if (vocaloid1.empty() || vocaloid2.empty()) {
		vocaloid1.clear();
		vocaloid2.clear();

		addCurveNameTo(vocaloid1, vocaloid2, "PIT", true, true);
		addCurveNameTo(vocaloid1, vocaloid2, "PBS", true, true);
		addCurveNameTo(vocaloid1, vocaloid2, "DYN", true, true);
		addCurveNameTo(vocaloid1, vocaloid2, "BRE", true, true);
		addCurveNameTo(vocaloid1, vocaloid2, "BRI", true, true);
		addCurveNameTo(vocaloid1, vocaloid2, "CLE", true, true);

		addCurveNameTo(vocaloid1, vocaloid2, "harmonics", true, false);
		addCurveNameTo(vocaloid1, vocaloid2, "fx2depth", true, false);
		addCurveNameTo(vocaloid1, vocaloid2, "reso1Freq", true, false);
		addCurveNameTo(vocaloid1, vocaloid2, "reso2Freq", true, false);
		addCurveNameTo(vocaloid1, vocaloid2, "reso3Freq", true, false);
		addCurveNameTo(vocaloid1, vocaloid2, "reso4Freq", true, false);
		addCurveNameTo(vocaloid1, vocaloid2, "reso1BW", true, false);
		addCurveNameTo(vocaloid1, vocaloid2, "reso2BW", true, false);
		addCurveNameTo(vocaloid1, vocaloid2, "reso3BW", true, false);
		addCurveNameTo(vocaloid1, vocaloid2, "reso4BW", true, false);
		addCurveNameTo(vocaloid1, vocaloid2, "reso1Amp", true, false);
		addCurveNameTo(vocaloid1, vocaloid2, "reso2Amp", true, false);
		addCurveNameTo(vocaloid1, vocaloid2, "reso3Amp", true, false);
		addCurveNameTo(vocaloid1, vocaloid2, "reso4Amp", true, false);

		addCurveNameTo(vocaloid1, vocaloid2, "GEN", true, true);
		addCurveNameTo(vocaloid1, vocaloid2, "POR", true, true);

		addCurveNameTo(vocaloid1, vocaloid2, "OPE", false, true);
	}

	if (common().version.substr(0, 4) == "DSB2") {
		return &vocaloid1;
	} else {
		return &vocaloid2;
	}
}

std::map<std::string, std::string> Track::getSectionNameMap() const
{
	static std::map<std::string, std::string> const result = {
		{"[PitchBendBPList]",			"pit"},
		{"[PitchBendSensBPList]",		"pbs"},
		{"[DynamicsBPList]",			"dyn"},
		{"[EpRResidualBPList]",			"bre"},
		{"[EpRESlopeBPList]",			"bri"},
		{"[EpRESlopeDepthBPList]",		"cle"},
		{"[EpRSineBPList]",				"harmonics"},
		{"[VibTremDepthBPList]",		"fx2depth"},
		{"[Reso1FreqBPList]",			"reso1Freq"},
		{"[Reso2FreqBPList]",			"reso2Freq"},
		{"[Reso3FreqBPList]",			"reso3Freq"},
		{"[Reso4FreqBPList]",			"reso4Freq"},
		{"[Reso1BWBPList]",				"reso1BW"},
		{"[Reso2BWBPList]",				"reso2BW"},
		{"[Reso3BWBPList]",				"reso3BW"},
		{"[Reso4BWBPList]",				"reso4BW"},
		{"[Reso1AmpBPList]",			"reso1Amp"},
		{"[Reso2AmpBPList]",			"reso2Amp"},
		{"[Reso3AmpBPList]",			"reso3Amp"},
		{"[Reso4AmpBPList]",			"reso4Amp"},
		{"[GenderFactorBPList]",		"gen"},
		{"[PortamentoTimingBPList]",	"por"},
		{"[OpeningBPList]",				"ope"},
	};
	return result;
}

void Track::_initCor(std::string const& name, std::string const& singer)
{
	this->_common = Common(name, 179, 181, 123, DynamicsMode::EXPERT, PlayMode::PLAY_WITH_SYNTH);
	curveNameMap["pit"] = std::move(std::unique_ptr<BPList>(new BPList("pit", 0, -8192, 8191)));
	curveNameMap["pbs"] = std::move(std::unique_ptr<BPList>(new BPList("pbs", 2, 0, 24)));
	curveNameMap["dyn"] = std::move(std::unique_ptr<BPList>(new BPList("dyn", 64, 0, 127)));
	curveNameMap["bre"] = std::move(std::unique_ptr<BPList>(new BPList("bre", 0, 0, 127)));
	curveNameMap["bri"] = std::move(std::unique_ptr<BPList>(new BPList("bri", 64, 0, 127)));
	curveNameMap["cle"] = std::move(std::unique_ptr<BPList>(new BPList("cle", 0, 0, 127)));
	curveNameMap["reso1freq"] = std::move(std::unique_ptr<BPList>(new BPList("reso1freq", 64, 0, 127)));
	curveNameMap["reso2freq"] = std::move(std::unique_ptr<BPList>(new BPList("reso2freq", 64, 0, 127)));
	curveNameMap["reso3freq"] = std::move(std::unique_ptr<BPList>(new BPList("reso3freq", 64, 0, 127)));
	curveNameMap["reso4freq"] = std::move(std::unique_ptr<BPList>(new BPList("reso4freq", 64, 0, 127)));
	curveNameMap["reso1bw"] = std::move(std::unique_ptr<BPList>(new BPList("reso1bw", 64, 0, 127)));
	curveNameMap["reso2bw"] = std::move(std::unique_ptr<BPList>(new BPList("reso2bw", 64, 0, 127)));
	curveNameMap["reso3bw"] = std::move(std::unique_ptr<BPList>(new BPList("reso3bw", 64, 0, 127)));
	curveNameMap["reso4bw"] = std::move(std::unique_ptr<BPList>(new BPList("reso4bw", 64, 0, 127)));
	curveNameMap["reso1amp"] = std::move(std::unique_ptr<BPList>(new BPList("reso1amp", 64, 0, 127)));
	curveNameMap["reso2amp"] = std::move(std::unique_ptr<BPList>(new BPList("reso2amp", 64, 0, 127)));
	curveNameMap["reso3amp"] = std::move(std::unique_ptr<BPList>(new BPList("reso3amp", 64, 0, 127)));
	curveNameMap["reso4amp"] = std::move(std::unique_ptr<BPList>(new BPList("reso4amp", 64, 0, 127)));
	curveNameMap["harmonics"] = std::move(std::unique_ptr<BPList>(new BPList("harmonics", 64, 0, 127)));
	curveNameMap["fx2depth"] = std::move(std::unique_ptr<BPList>(new BPList("fx2depth", 64, 0, 127)));
	curveNameMap["gen"] = std::move(std::unique_ptr<BPList>(new BPList("gen", 64, 0, 127)));
	curveNameMap["por"] = std::move(std::unique_ptr<BPList>(new BPList("por", 64, 0, 127)));
	curveNameMap["ope"] = std::move(std::unique_ptr<BPList>(new BPList("ope", 127, 0, 127)));

	Event event(0, EventType::SINGER);
	Handle ish(HandleType::SINGER);
	ish.iconId = "$07010000";
	ish.ids = singer;
	ish.original = 0;
	ish.length(1);
	ish.language = 0;
	ish.program = 0;
	event.singerHandle = ish;
	this->_events.add(event);
}

void Track::deepCopy(Track* destination) const
{
	destination->name(name());

	destination->_common = _common.clone();
	destination->_events.clear();
	for (int i = 0; i < _events.size(); i++) {
		Event const* item = _events.get(i);
		destination->_events.add(item->clone(), item->id);
	}
	destination->curveNameMap.clear();
    for (auto const& item : curveNameMap) {
        BPList* list = new BPList(item.second->name(),
                                  item.second->defaultValue(),
                                  item.second->minimum(),
                                  item.second->maximum());
		*list = item.second->clone();
        destination->curveNameMap[item.first] = std::move(std::unique_ptr<BPList>(list));
    }
}

void Track::addCurveNameTo(std::vector<std::string>& vocaloid1CurveNameList,
						   std::vector<std::string>& vocaloid2CurveNameList,
						   std::string const& name,
						   bool addToVocaloid1, bool addToVocaloid2) const
{
	if (addToVocaloid1) { vocaloid1CurveNameList.push_back(name); }
	if (addToVocaloid2) { vocaloid2CurveNameList.push_back(name); }
}

LIBVSQ_END_NAMESPACE
