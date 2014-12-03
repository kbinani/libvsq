/**
 * Timesig.hpp
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
#include <string>

LIBVSQ_BEGIN_NAMESPACE

/**
 * @brief 拍子変更情報テーブル内の要素を表現するためのクラス.
 */
class Timesig
{
	friend class TimesigList;

public:
	/**
	 * @brief 拍子の分子.
	 */
	int numerator;

	/**
	 * @brief 拍子の分母.
	 */
	int denominator;

	/**
	 * @brief 何小節目か.
	 */
	int barCount;

protected:
	/**
	 * @brief Tick 単位の時刻.
	 */
	tick_t tick_;

public:
	Timesig();

	/**
	 * @brief 初期化を行う.
	 * @param numerator 拍子の分子の値.
	 * @param denominator 拍子の分母値.
	 * @param barCount 小節数.
	 */
	Timesig(int numerator, int denominator, int barCount);

	/**
	 * @brief このイベントの tick 単位の時刻を取得する.
	 * @return tick 単位の時刻.
	 */
	tick_t tick() const;

	/**
	 * @brief 文字列に変換する.
	 * @return 変換後の文字列.
	 */
	std::string toString() const;

	/**
	 * @brief 順序を比較する.
	 * @param item 比較対象のアイテム.
	 * @return 比較対象がこのインスタンスより大であれば true を, そうでなければ false を返す.
	 */
	bool compareTo(Timesig const& item) const;

	/**
	 * @brief コピーを作成する.
	 * @return このオブジェクトのコピー.
	 */
	Timesig clone() const;

	/**
	 * @brief 順序を比較する.
	 * @return b が a より大であれば true を, そうでなければ false を返す.
	 */
	static bool compare(Timesig const& a, Timesig const& b);
};

LIBVSQ_END_NAMESPACE
