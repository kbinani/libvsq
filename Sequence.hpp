/**
 * Sequence.hpp
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

VSQ_BEGIN_NAMESPACE

/**
 * @brief VSQ ファイルのシーケンスを保持するクラス
 * @todo PreMeasureの取得方法が２通り以上ある。Sequence::master.preMeasureとSequence::getPreMeasure
 */
class Sequence
{
public:
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
	std::string tag;

protected:
	/**
	 * @brief トラックのリスト
	 */
	std::vector<Track> _track;

private:
	/**
	 * @brief テンポが省略された際の、基準となるテンポ値
	 */
	static const int baseTempo = 500000;

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
	 */
	Sequence();

	/**
	 * @brief 初期化を行う
	 * @param singer (string) 歌手名
	 * @param preMeasure (int) 小節単位のプリメジャー
	 * @param numerator (int) 拍子の分子の値
	 * @param denominator (int) 拍子の分母の値
	 * @param tempo (int) テンポ値。四分音符の長さのマイクロ秒単位の長さ
	 */
	Sequence(std::string const& singer, int preMeasure, int numerator, int denominator, int tempo);

	/**
	 * @brief コピーを作成する
	 * @return (Sequence) オブジェクトのコピー
	 */
	Sequence clone() const;

	/**
	 * @brief Get an instance of Track.
	 */
	const Track* track(int trackIndex) const;

	/**
	 * @brief Get an instance of Track.
	 */
	Track* track(int trackIndex);

	const std::vector<Track>* tracks() const;

	std::vector<Track>* tracks();

	/**
	 * @brief テンポが一つも指定されていない場合の、基本テンポ値を取得する
	 * @return (int) テンポ値。四分音符の長さのマイクロ秒単位の長さ
	 */
	int getBaseTempo() const;

	/**
	 * @brief Tick 単位の曲の長さを取得する
	 * シーケンスに変更を加えた場合、<code><a href="#updateTotalClocks">updateTotalClocks</a></code> を呼んでからこのメソッドを呼ぶこと
	 * @return (int) Tick 単位の曲の長さ
	 */
	tick_t getTotalClocks() const;

	/**
	 * @brief プリメジャー値を取得する
	 * @return (int) 小節単位のプリメジャー長さ
	 */
	int getPreMeasure() const;

	/**
	 * @brief Tick 単位のプリメジャー部分の長さを取得する
	 * @return (int) Tick 単位のプリメジャー長さ
	 */
	tick_t getPreMeasureClocks() const;

	/**
	 * @brief 四分音符あたりの Tick 数を取得する
	 * @return (int) 四分音符一つあたりの Tick 数
	 */
	tick_t getTickPerQuarter() const;

	/**
	 * @brief カーブ名のリストを取得する
	 * @return カーブ名のリスト
	 */
	static const std::vector<std::string> getCurveNameList();

	/**
	 * @brief totalClock の値を更新する
	 */
	void updateTotalClocks();

private:
	/**
	 * @brief プリメジャーの Tick 単位の長さを計算する
	 * @return (int) Tick 単位のプリメジャー長さ
	 */
	tick_t _calculatePreMeasureInClock() const;

	/**
	 * @brief 初期化を行う
	 * @param singer (string) 歌手名
	 * @param preMeasure (int) 小節単位のプリメジャー
	 * @param numerator (int) 拍子の分子の値
	 * @param denominator (int) 拍子の分母の値
	 * @param tempo (int) テンポ値。四分音符の長さのマイクロ秒単位の長さ
	 */
	void init(std::string const& singer, int preMeasure, int numerator, int denominator, int tempo);
};

VSQ_END_NAMESPACE
