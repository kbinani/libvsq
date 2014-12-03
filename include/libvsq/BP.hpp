/**
 * BP.hpp
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
 * @brief コントロールカーブのデータ点を表現するクラス.
 */
class BP
{
public:
	/**
	 * @brief コンストラクタ.
	 * @param[in] value データ点の値.
	 * @param[in] id データ点のユニーク ID.
	 */
	BP(int value, int id);

	/**
	 * @brief コピーを作成する.
	 * @return このインスタンスのコピー.
	 */
	BP clone() const;

private:
	BP();

public:
	/**
	 * @brief データ点の値.
	 */
	int value;

	/**
	 * @brief データ点のユニーク ID.
	 */
	int id;
};

VSQ_END_NAMESPACE
