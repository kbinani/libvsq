/**
 * @file Sequence.hpp
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

#include "./BasicTypes.hpp"
#include "./TempoList.hpp"
#include "./TimesigList.hpp"
#include "./Master.hpp"
#include "./Mixer.hpp"
#include "./Track.hpp"

LIBVSQ_BEGIN_NAMESPACE

/**
 * @brief VSQ ファイルのシーケンスを保持するクラス.
 * @todo PreMeasure の取得方法が 2 通り以上ある. Sequence::master.preMeasureとSequence::getPreMeasure.
 */
class Sequence
{
public:
	/**
	 * @brief テンポ情報を保持したテーブル.
	 */
	TempoList tempoList;

	/**
	 * @brief 拍子情報を保持したテーブル.
	 */
	TimesigList timesigList;

	/**
	 * @brief プリメジャーを保持する.
	 */
	Master master;

	/**
	 * @brief ミキサー情報.
	 */
	Mixer mixer;

	/**
	 * @brief シーケンスに付属するタグ情報.
	 */
	std::string tag;

protected:
	/**
	 * @brief トラックのリスト.
	 */
	std::vector<Track> _track;

private:
	/**
	 * @brief テンポが省略された際の, 基準となるテンポ値.
	 */
	static const int _baseTempo = 500000;

	/**
	 * @brief 四分音符 1 個あたりの Tick 数.
	 */
	static const int _tickPerQuarter = 480;

	/**
	 * @brief Tick 単位の曲の長さ.
	 */
	tick_t _totalTicks;

public:
	/**
	 * @brief 初期化を行う.
	 */
	Sequence();

	/**
	 * @brief 初期化を行う.
	 * @param singer 歌手名.
	 * @param preMeasure 小節単位のプリメジャー.
	 * @param numerator 拍子の分子の値.
	 * @param denominator 拍子の分母の値.
	 * @param tempo テンポ値. 四分音符の長さのマイクロ秒単位の長さ.
	 */
	Sequence(std::string const& singer, int preMeasure, int numerator, int denominator, int tempo);

	/**
	 * @brief コピーを作成する.
	 * @return オブジェクトのコピー.
	 */
	Sequence clone() const;

	/**
	 * @brief 先頭から @a trackIndex 番目のトラックを取得する.
	 */
	Track const& track(int trackIndex) const;

	/**
	 * @copydoc Sequence::track
	 */
	Track& track(int trackIndex);

	/**
	 * @brief 全トラックのリストを取得する.
	 */
	std::vector<Track> const& tracks() const;

	/**
	 * @copydoc Sequence::tracks
	 */
	std::vector<Track>& tracks();

	/**
	 * @brief テンポが一つも指定されていない場合の, 基本テンポ値を取得する.
	 * @return テンポ値. 四分音符の長さのマイクロ秒単位の長さ.
	 */
	int baseTempo() const;

	/**
	 * @brief Tick 単位の曲の長さを取得する.
	 * @details シーケンスに変更を加えた場合, Sequence::updateTotalTicks を呼んでからこのメソッドを呼ぶこと.
	 * @return Tick 単位の曲の長さ.
	 */
	tick_t totalTicks() const;

	/**
	 * @brief プリメジャー値を取得する
	 * @return 小節単位のプリメジャー長さ.
	 */
	int preMeasure() const;

	/**
	 * @brief Tick 単位のプリメジャー部分の長さを取得する.
	 * @return Tick 単位のプリメジャー長さ.
	 */
	tick_t preMeasureTicks() const;

	/**
	 * @brief 四分音符あたりの Tick 数を取得する.
	 * @return 四分音符一つあたりの Tick 数.
	 */
	tick_t tickPerQuarter() const;

	/**
	 * @brief カーブ名のリストを取得する.
	 * @return カーブ名のリスト.
	 */
	static std::vector<std::string> curveNameList();

	/**
	 * @brief totalTick の値を更新する.
	 */
	void updateTotalTicks();

private:
	/**
	 * @brief プリメジャーの Tick 単位の長さを計算する.
	 * @return Tick 単位のプリメジャー長さ.
	 */
	tick_t _calculatePreMeasureInTick() const;

	/**
	 * @brief 初期化を行う.
	 * @param singer 歌手名.
	 * @param preMeasure 小節単位のプリメジャー.
	 * @param numerator 拍子の分子の値.
	 * @param denominator 拍子の分母の値.
	 * @param tempo テンポ値. 四分音符の長さのマイクロ秒単位の長さ.
	 */
	void init(std::string const& singer, int preMeasure, int numerator, int denominator, int tempo);
};

LIBVSQ_END_NAMESPACE
