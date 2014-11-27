/**
 * TimesigList.hpp
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

#include <vector>
#include <math.h>
#include <algorithm>
#include "Timesig.hpp"

VSQ_BEGIN_NAMESPACE

/**
 * @brief 拍子情報を格納したテーブルを表すクラス
 */
class TimesigList
{
private:
	/**
	 * @brief 拍子情報を格納したリスト。tick の昇順で格納する
	 */
	std::vector<Timesig> list;

public:
	TimesigList();

	~TimesigList();

	/**
	 * @brief 指定したインデックスの拍子変更情報を取得する
	 * @param index 取得するデータ点のインデックス(0から始まる)
	 * @return 拍子変更情報
	 */
	Timesig get(int index) const;

	/**
	 * @brief データ点を追加する
	 * @param item 追加する拍子変更情報
	 */
	void push(const Timesig& item);

	/**
	 * @brief データ点の個数を返す
	 * @return データ点の個数
	 */
	int size() const;

	/**
	 * @brief 指定された時刻における拍子情報を取得する
	 * @param clock Tick 単位の時刻
	 * @return 指定された時刻での拍子情報
	 */
	Timesig getTimesigAt(tick_t clock) const;

	/**
	 * @brief 指定した小節の開始クロックを取得する。
	 * ここで使用する小節数は、プリメジャーを考慮しない。即ち、曲頭の小節が 0 となる
	 * @param barCount 小節数
	 * @return Tick 単位の時刻
	 */
	tick_t getClockFromBarCount(int barCount) const;

	/**
	 * @brief リストをクリアする
	 */
	void clear();

	/**
	 * @brief 指定したクロックが、曲頭から何小節目に属しているかを調べる
	 * ここで使用する小節数は、プリメジャーを考慮しない。即ち、曲頭の小節が 0 となる
	 * @param clock Tick 単位の時刻
	 * @return 小節数
	 */
	int getBarCountFromClock(tick_t clock) const;

protected:
	/**
	 * @brief データ点を追加する。追加後のソートは行わない
	 * push を頻繁に行い、速度の改善を行いたい場合は、TimesigList をオーバーライドし、
	 * pushWithoutSort を必要回呼んだ後最後に updateTimesigInfo を呼ぶような実装にすると良いだろう
	 * @param item 追加する拍子変更情報
	 */
	void pushWithoutSort(const Timesig& item);

	/**
	 * @brief リスト内の拍子変更情報の clock の部分を更新する
	 */
	void updateTimesigInfo();
};

VSQ_END_NAMESPACE
