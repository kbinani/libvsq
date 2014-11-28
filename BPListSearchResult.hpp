/**
 * BPListSearchResult.hpp
 * Copyright © 2012 kbinani
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
#include "./BP.hpp"

VSQ_BEGIN_NAMESPACE

/**
 * @brief コントロールパラメータのデータ点検索結果を格納するクラス.
 */
class BPListSearchResult
{
public:
	/**
	 * @brief データ点の Tick 単位の時刻.
	 */
	tick_t clock;

	/**
	 * @brief データ点のインデックス. 最初のインデックスは 0.
	 */
	int index;

	/**
	 * @brief データ点のオブジェクト.
	 */
	BP point;

	/**
	 * @brief 初期化を行う.
	 */
	BPListSearchResult()
		: clock(0)
		, index(0)
		, point(0, 0)
	{}
};

VSQ_END_NAMESPACE
