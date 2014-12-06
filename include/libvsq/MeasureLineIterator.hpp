/**
 * @file MeasureLineIterator.hpp
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
#include "./MeasureLine.hpp"
#include <exception>

LIBVSQ_BEGIN_NAMESPACE

class TimesigList;

/**
 * @brief 小節を区切る線の情報を順に返す反復子.
 */
class MeasureLineIterator
{
private:
	/**
	 * @brief 補助線の間隔として利用可能な最小時間(tick単位).
	 */
	static tick_t const MIN_ASSIST_LINE_STEP = 15;

	TimesigList const* list;
	tick_t endTick;
	int i;
	tick_t tick;
	int currentDenominator;
	int currentNumerator;
	tick_t actualStepLength;
	tick_t temporaryEndTick;
	tick_t currentTick;
	int barCount;
	tick_t assistLineStep;
	tick_t stepLength;

public:
	/**
	 * @brief MeasureLineIterator のコンストラクタに不正な値が指定された時スローされる例外.
	 * @sa MeasureLineIterator
	 */
	class InvalidAssistLineStep : public std::exception
	{};

public:
	/**
	 * @brief 小節線の情報を取得する区間を指定し, 初期化する.
	 * @param list テンポ変更リスト.
	 * @throw InvalidAssistLineStep @a assistLineStep に不正な値が指定された時スローされる.
	 */
	explicit MeasureLineIterator(TimesigList const* list, tick_t assistLineStep = 0);

	/**
	 * @brief 次の小節線が取得可能かどうかを取得する.
	 * @return 取得可能であれば true を返す.
	 */
	bool hasNext() const;

	/**
	 * @brief 次の小節線を取得する.
	 * @return 次の小節線の情報.
	 */
	MeasureLine next();

	/**
	 * @brief 反復子をリセットする.
	 * @param endTick 反復を行う最大の時刻(tick単位)を指定する.
	 * @todo startTick を指定できるようにする.
	 */
	void reset(tick_t endTick);

private:
	/**
	 * @brief 小節の境界を表す MeasureLine のインスタンスを返す.
	 */
	MeasureLine returnBorder();

	/**
	 * @brief 小節の境界でない MeasureLine のインスタンスを返す.
	 */
	MeasureLine returnOther();
};

LIBVSQ_END_NAMESPACE
