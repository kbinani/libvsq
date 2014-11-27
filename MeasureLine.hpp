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

VSQ_BEGIN_NAMESPACE

/**
 * 小節を区切る線を表現するクラス
 */
class MeasureLine
{
public:
	/**
	 * @brief tick 単位の時刻
	 */
	tick_t tick;

	/**
	 * @brief 小節の境界かどうか
	 */
	bool isBorder;

	/**
	 * @brief 補助線かどうか
	 */
	bool isAssistLine;

	/**
	 * @brief 小節数
	 */
	int barCount;

	/**
	 * @brief 拍子の分子
	 */
	int numerator;

	/**
	 * @brief 拍子の分母
	 */
	int denominator;

public:
	MeasureLine(tick_t tick, int barCount, int numerator, int denominator, bool isBorder, bool isAssistLine)
	{
		this->tick = tick;
		this->barCount = barCount;
		this->numerator = numerator;
		this->denominator = denominator;
		this->isBorder = isBorder;
		this->isAssistLine = isAssistLine;
	}
};

VSQ_END_NAMESPACE
