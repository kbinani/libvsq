/**
 * VibratoBP.hpp
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

VSQ_BEGIN_NAMESPACE

/**
 * @brief ビブラートのデータポイント.
 */
class VibratoBP
{
public:
	/**
	 * @brief データ点の X 値.
	 */
	double x;

	/**
	 * @brief データ点の Y 値.
	 */
	int y;

public:
	VibratoBP();

	/**
	 * @brief 初期化を行う.
	 * @param x x 軸の値.
	 * @param y y 軸の値.
	 */
	VibratoBP(double x, int y);

	/**
	 * @brief 順序を比較する.
	 * @param item 比較対象のアイテム.
	 * @return このインスタンスが比較対象よりも小さい場合は負の整数, 等しい場合は 0, 大きい場合は正の整数を返す.
	 */
	int compareTo(VibratoBP const& item) const;

	/**
	 * @brief 2 つの {@link VibratoBP} を比較する.
	 * @param a 比較対象のオブジェクト.
	 * @param b 比較対象のオブジェクト.
	 * @return <code>a</code> が <code>b</code> よりも小さい場合は <code>true</code>, そうでない場合は <code>false</code> を返す.
	 */
	static bool compare(VibratoBP const& a, VibratoBP const& b);

private:
	static int compareCore(VibratoBP const& a, VibratoBP const& b);
};

VSQ_END_NAMESPACE
