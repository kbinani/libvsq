/**
 * VibratoBPList.hpp
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
#include "./VibratoBP.hpp"
#include <vector>

VSQ_BEGIN_NAMESPACE

/**
 * @brief ビブラートデータポイントのリストを表すクラス.
 * @todo ビブラート用とは限らないので, クラス名を良いものに変える. 例えば, NoteHeadHandleのDyn.
 */
class VibratoBPList
{
private:
	/**
	 * @brief カーブのデータ点を保持するリスト.
	 */
	std::vector<VibratoBP> _list;

public:
	/**
	 * @brief 初期化を行う.
	 * @param textNum データ点の個数の文字列表現.
	 * @param textBPX x 軸のデータ点の値をカンマ区切りで繋げた文字列.
	 * @param textBPY y 軸のデータ点の値をカンマ区切りで繋げた文字列.
	 */
	VibratoBPList(std::string const& textNum, std::string const& textBPX, std::string const& textBPY);

	/**
	 * @brief 初期化を行う.
	 * @param x x 軸の値のリスト.
	 * @param y y 軸の値のリスト.
	 */
	explicit VibratoBPList(std::vector<double> const& x = std::vector<double>(), std::vector<int> const& y = std::vector<int>());

	/**
	 * @brief 指定した位置のビブラートカーブの値を取得する.
	 * @param x 取得したい x 軸の値.
	 * @param defaultValue ビブラートカーブのデフォルト値.
	 * @return ビブラートカーブの値.
	 */
	int getValueAt(double x, int defaultValue) const;

	/**
	 * @brief コピーを作成する.
	 * @return このオブジェクトのコピー.
	 */
	VibratoBPList clone() const;

	/**
	 * @brief データ点の個数を返す.
	 * @return データ点の個数.
	 */
	int size() const;

	/**
	 * @brief 指定したインデックスのデータ点を取得する.
	 * @param index 0 から始まるインデックス.
	 * @return データ点.
	 */
	const VibratoBP get(int index) const;

	/**
	 * @brief 指定したインデックスのデータ点を設定する.
	 * @param index インデックス(最初のインデックスは0).
	 * @param value 設定するデータ点.
	 */
	void set(int index, const VibratoBP value);

	/**
	 * @brief データ点のリストを, 文字列に変換する. 例えば "key1=value1,key2=value2" のように変換される.
	 * @return 変換後の文字列.
	 */
	const std::string getData() const;

	/**
	 * @brief "key1=value=1,key2=value2" のような文字列から, データ点のリストを設定する.
	 * @param value データ点の文字列形式.
	 */
	void setData(std::string const& value);

private:
	explicit VibratoBPList(std::vector<double> const&);

};

VSQ_END_NAMESPACE
