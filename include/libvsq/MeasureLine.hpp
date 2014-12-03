/**
 * MeasureLine.hpp
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

LIBVSQ_BEGIN_NAMESPACE

/**
 * @brief 小節を区切る線を表現するクラス.
 */
class MeasureLine
{
public:
	/**
	 * @brief tick 単位の時刻.
	 */
	tick_t tick;

	/**
	 * @brief 小節の境界かどうか.
	 */
	bool isBorder;

	/**
	 * @brief 補助線かどうか.
	 */
	bool isAssistLine;

	/**
	 * @brief 小節数.
	 */
	int barCount;

	/**
	 * @brief 拍子の分子.
	 */
	int numerator;

	/**
	 * @brief 拍子の分母.
	 */
	int denominator;

public:
	MeasureLine(tick_t tick_, int barCount_, int numerator_, int denominator_, bool isBorder_, bool isAssistLine_)
		: tick(tick_)
		, isBorder(isBorder_)
		, isAssistLine(isAssistLine_)
		, barCount(barCount_)
		, numerator(numerator_)
		, denominator(denominator_)
	{}
};

LIBVSQ_END_NAMESPACE
