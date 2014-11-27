/**
 * TempoList.hpp
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
#include "./Tempo.hpp"
#include <vector>

VSQ_BEGIN_NAMESPACE

/**
 * @brief テンポ情報を格納したテーブルを表すクラス
 */
class TempoList
{
public:
	static const int gatetimePerQuater = 480;
	static const int baseTempo = 500000;

	class Iterator
	{
	private:
		const std::vector<Tempo>* list;
		std::vector<Tempo>::const_iterator iterator;

	public:
		/**
		 * @todo このコンストラクタは隠蔽すべき
		 */
		explicit Iterator(const std::vector<Tempo>* list);

		bool hasNext();

		Tempo next();
	};

private:
	/**
	 * @brief テンポ変更情報のリスト
	 */
	std::vector<Tempo> _array;

public:
	/**
	 * @brief リスト内のテンポ変更イベントを順に返す反復子を取得する
	 * @return (List.Iterator<Tempo>) 反復子
	 */
	TempoList::Iterator iterator() const;

	/**
	 * @brief データ点を時刻順に並べ替える
	 */
	void sort();

	/**
	 * @brief データ点を追加する
	 * @param value (Tempo) 追加するテンポ変更情報
	 */
	void push(const Tempo& value);

	/**
	 * @brief リスト内のテンポ変更情報の個数を取得する
	 * @return (int) テンポ変更情報の個数
	 */
	int size() const;

	/**
	 * @brief 指定したインデックスのテンポ変更情報を取得する
	 * @param index (int) インデックス(最初のインデックスは0)
	 * @return (Tempo) テンポ変更情報
	 */
	Tempo get(int index) const;

	/**
	 * @brief 指定したインデックスのテンポ変更情報を設定する
	 * @param index (int) インデックス(最初のインデックスは0)
	 * @param value (Tempo) 設定するイベント
	 */
	void set(int index, const Tempo& value);

	/**
	 * @brief 時刻の単位を、秒単位から Tick 単位に変換する
	 * @param time (double) 秒単位の時刻
	 * @return (double) Tick 単位の時刻
	 */
	double getClockFromSec(double time) const;

	/**
	 * @brief リスト内のテンポ変更情報の秒単位の時刻部分を更新する
	 */
	void updateTempoInfo();

	/**
	 * @brief 時刻の単位を、Tick 単位から秒単位に変換する
	 * @param clock (double) Tick 単位の時刻
	 * @return (double) 秒単位の時刻
	 */
	double getSecFromClock(double clock) const;

	/**
	 * @brief 指定した時刻におけるテンポを取得する
	 * @param clock (int) Tick 単位の時刻
	 * @return (int) テンポ値。四分音符の長さをマイクロ秒単位で表した値
	 * @todo clock の型をtick_tに修正
	 */
	int getTempoAt(int clock) const;

	/**
	 * @brief リストをクリアする
	 */
	void clear();
};

VSQ_END_NAMESPACE
