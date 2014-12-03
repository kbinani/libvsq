/**
 * Track.hpp
 * Copyright © 2012,2014 kbinani
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
#pragma once

#include "./Namespace.hpp"
#include "./Common.hpp"
#include "./Event.hpp"
#include "./BPList.hpp"
#include "./EventListIndexIterator.hpp"
#include "./EventListIndexIteratorKind.hpp"
#include <map>

LIBVSQ_BEGIN_NAMESPACE

/**
 * @todo _pit とかを一般化する.
 */
class Track
{
protected:
	/**
	 * @brief トラック名などの情報.
	 */
	Common _common;

	/**
	 * @brief イベントの一覧.
	 */
	Event::List _events;

private:
	/**
	 * @brief PIT. ピッチベンド(pitchBendBPList). default=0.
	 */
	BPList _pit;

	/**
	 * @brief PBS. ピッチベンドセンシティビティ(pitchBendSensBPList). default=2.
	 */
	BPList _pbs;

	/**
	 * @brief DYN. ダイナミクス(dynamicsBPList). default=64.
	 */
	BPList _dyn;

	/**
	 * @brief BRE. ブレシネス(epRResidualBPList). default=0.
	 */
	BPList _bre;

	/**
	 * @brief BRI. ブライトネス(epRESlopeBPList). default=64.
	 */
	BPList _bri;

	/**
	 * @brief CLE. クリアネス(epRESlopeDepthBPList). default=0.
	 */
	BPList _cle;

	BPList _reso1FreqBPList;

	BPList _reso2FreqBPList;

	BPList _reso3FreqBPList;

	BPList _reso4FreqBPList;

	BPList _reso1BWBPList;

	BPList _reso2BWBPList;

	BPList _reso3BWBPList;

	BPList _reso4BWBPList;

	BPList _reso1AmpBPList;

	BPList _reso2AmpBPList;

	BPList _reso3AmpBPList;

	BPList _reso4AmpBPList;

	/**
	 * @brief Harmonics. (EpRSineBPList)default = 64.
	 */
	BPList _harmonics;

	/**
	 * @brief Effect2 Depth.
	 */
	BPList _fx2depth;

	/**
	 * @brief GEN. ジェンダーファクター(genderFactorBPList). default=64.
	 */
	BPList _gen;

	/**
	 * @brief POR. ポルタメントタイミング(portamentoTimingBPList). default=64.
	 */
	BPList _por;

	/**
	 * @brief OPE. オープニング(openingBPList). default=127.
	 */
	BPList _ope;

	std::map<std::string, BPList*> curveNameMap;

public:
	/**
	 * @brief Master Trackを構築.
	 */
	Track();

	/**
	 * @brief Master Trackでないトラックを構築.
	 * @param name トラック名.
	 * @param singer トラックのデフォルトの歌手名.
	 */
	Track(std::string const& name, std::string const& singer);

	Track(Track const& value);

	virtual ~Track()
	{}

	Track& operator = (Track const& value);

	/**
	 * @brief トラックの名前を取得する.
	 * @return トラック名.
	 */
	std::string name() const;

	/**
	 * @brief トラックの名前を設定する.
	 * @param value トラック名.
	 */
	void name(std::string const& value);

	/**
	 * @brief 指定された種類のイベントのインデクスを順に返す反復子を取得する.
	 * @param iteratorKind 反復子の種類.
	 * @return 反復子.
	 */
	EventListIndexIterator getIndexIterator(EventListIndexIteratorKind iteratorKind) const;

	/**
	 * @brief 指定したゲートタイムにおいて, 歌唱を担当している歌手の歌手変更イベントを取得する.
	 * @param tick ゲートタイム.
	 * @return 歌手イベント. 存在しなければ null を返す.
	 * @todo Change to const method.
	 */
	Event const* singerEventAt(tick_t tick) const;

	BPList const* curve(std::string const& curveName) const;

	/**
	 * @brief 指定された名前のカーブを取得する.
	 * @param curve カーブ名.
	 * @return カーブ.
	 */
	BPList* curve(std::string const& curveName);

	/**
	 * @brief コピーを作成する.
	 * @return このオブジェクトのコピー.
	 */
	Track clone() const;

	/**
	 * @brief イベントリストを取得する.
	 * @return イベントリストのポインタ.
	 */
	Event::List& events();

	Event::List const& events() const;

	/**
	 * @brief Get an instance of Common.
	 */
	Common& common();

	/**
	 * @brief Get an instance of Common.
	 */
	Common const& common() const;

	/**
	 * @brief Get a list of curve name for this track.
	 * @return A list of curve name.
	 */
	virtual std::vector<std::string> const* curveNameList() const;

protected:
	/**
	 * @brief Get a map of section name in vsq and curve name.
	 */
	virtual std::map<std::string, std::string> getSectionNameMap() const;

private:
	/**
	 * @brief 初期化を行う.
	 * @param name
	 * @param singer
	 */
	void _initCor(std::string const& name, std::string const& singer);

	/**
	 * @brief Deep copy instance.
	 * @param destination Copy destination
	 */
	void deepCopy(Track* destination) const;

	void setupCurveNameMap();

	void addCurveNameTo(std::vector<std::string>& vocaloid1CurveNameList,
						std::vector<std::string>& vocaloid2CurveNameList,
						std::string const& name,
						bool addToVocaloid1, bool addToVocaloid2) const;
};

LIBVSQ_END_NAMESPACE
