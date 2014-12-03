/**
 * Tempo.hpp
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

VSQ_BEGIN_NAMESPACE

/**
 * @brief テンポ情報テーブル内の要素を表現するクラス.
 */
class Tempo
{
	friend class TempoList;

public:
	/**
	 * @brief Tick 単位の時刻.
	 */
	tick_t clock;

	/**
	 * @brief テンポ値. 四分音符の長さをマイクロ秒単位で表した値.
	 */
	int tempo;

protected:
	/**
	 * @brief テンポ変更の秒単位の時刻.
	 */
	double _time;

public:
	/**
	 * @brief 文字列に変換する.
	 * @return 変換後の文字列.
	 */
	std::string toString() const;

	/**
	 * @brief 初期化を行う.
	 * @param clock Tick 単位の時刻.
	 * @param tempo テンポ値. 四分音符の長さをマイクロ秒単位で表した値.
	 */
	Tempo(tick_t clock, int tempo);

	Tempo();

	/**
	 * @brief 順序を比較する.
	 * @param entry 比較対象のアイテム.
	 * @return このインスタンスが比較対象よりも小さい場合は負の整数, 等しい場合は 0, 大きい場合は正の整数を返す.
	 */
	int compareTo(Tempo const& entry) const;

	/**
	 * @brief このオブジェクトのインスタンスと, 指定されたオブジェクトが同じかどうかを調べる.
	 * @param entry 比較対象のオブジェクト.
	 * @return 比較対象と同じであれば <code>true</code> を, そうでなければ <code>false</code> を返す.
	 */
	bool equals(Tempo const& entry) const;

	/**
	 * @brief 秒単位の時刻を取得する.
	 * @return 秒単位の時刻.
	 */
	double getTime() const;

	/**
	 * @brief コピーを作成する.
	 * @return このオブジェクトのコピー.
	 */
	Tempo clone() const;

	/**
	 * @brief 2 つの {@link Tempo} を比較する.
	 * @param a 比較対象のオブジェクト.
	 * @param b 比較対象のオブジェクト.
	 * @return <code>a</code> が <code>b</code> よりも小さい場合は <code>true</code>, そうでない場合は <code>false</code> を返す.
	 */
	static bool compare(Tempo const& a, Tempo const& b);

private:
	static int compareCore(Tempo const& a, Tempo const& b);
};

VSQ_END_NAMESPACE
