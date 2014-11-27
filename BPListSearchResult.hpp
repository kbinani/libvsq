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
 * コントロールパラメータのデータ点検索結果を格納するクラス
 * @class table
 * @name BPListSearchResult
 */
class BPListSearchResult
{
public:
	/**
	 * データ点の Tick 単位の時刻
	 * @var int
	 */
	tick_t clock;

	/**
	 * データ点のインデックス。最初のインデックスは 0
	 * @var int
	 */
	int index;

	/**
	 * データ点のオブジェクト
	 * @var BP
	 */
	BP point;

	/**
	 * 初期化を行う
	 * @return (BPListSearchResult)
	 * @access static ctor
	 */
	BPListSearchResult() :
		clock(0),
		index(0),
		point(0, 0)
	{
	}
};

VSQ_END_NAMESPACE
