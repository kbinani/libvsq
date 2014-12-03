/**
 * MeasureLineIterator.cpp
 * Copyright © 2014 kbinani
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
#include "../include/libvsq/MeasureLineIterator.hpp"
#include "../include/libvsq/TimesigList.hpp"

LIBVSQ_BEGIN_NAMESPACE

MeasureLineIterator::MeasureLineIterator(TimesigList const* list, tick_t assistLineStep)
{
	this->list = list;
	endTick = 0;
	i = 0;
	temporaryEndTick = -1;
	tick = 0;
	if (assistLineStep < 0 || assistLineStep % MIN_ASSIST_LINE_STEP != 0) {
		throw InvalidAssistLineStep();
	}
	this->assistLineStep = assistLineStep;
	this->reset(endTick);
}

bool MeasureLineIterator::hasNext() const
{
	if (tick <= endTick) {
		return true;
	} else {
		return false;
	}
}

MeasureLine MeasureLineIterator::next()
{
	int mod = stepLength * currentNumerator;
	if (tick < temporaryEndTick) {
		if ((tick - currentTick) % mod == 0) {
			return returnBorder();
		} else {
			return returnOther();
		}
	}

	if (i < list->size()) {
		currentDenominator = list->get(i).denominator;
		currentNumerator = list->get(i).numerator;
		currentTick = list->get(i).tick();
		int local_bar_count = list->get(i).barCount;
		int denom = currentDenominator;
		if (denom <= 0) {
			denom = 4;
		}
		stepLength = 480 * 4 / denom;
		if (0 < assistLineStep && assistLineStep < stepLength) {
			actualStepLength = assistLineStep;
		} else {
			actualStepLength = stepLength;
		}
		mod = stepLength * currentNumerator;
		barCount = local_bar_count - 1;
		temporaryEndTick = endTick;
		if (i + 1 < list->size()) {
			temporaryEndTick = list->get(i + 1).tick();
		}
		i++;
		tick = currentTick;
		if (tick < temporaryEndTick) {
			if ((tick - currentTick) % mod == 0) {
				return returnBorder();
			} else {
				return returnOther();
			}
		}
	} else {
		if ((tick - currentTick) % mod == 0) {
			return returnBorder();
		} else {
			return returnOther();
		}
	}
	return MeasureLine(0, 0, 4, 4, true, false);
}

void MeasureLineIterator::reset(tick_t endTick)
{
	this->endTick = endTick;
	this->i = 0;
	this->temporaryEndTick = -1;
	this->tick = 0;
	this->currentDenominator = 0;
	this->currentNumerator = 0;
	this->actualStepLength = 0;
	this->currentTick = 0;
	this->barCount = 0;
	this->stepLength = 0;
}

MeasureLine MeasureLineIterator::returnBorder()
{
	barCount++;
	MeasureLine ret(tick, barCount, currentNumerator, currentDenominator, true, false);
	tick += actualStepLength;
	return ret;
}

MeasureLine MeasureLineIterator::returnOther()
{
	MeasureLine ret(tick, barCount, currentNumerator, currentDenominator, false, (tick - currentTick) % stepLength != 0);
	tick += actualStepLength;
	return ret;
}

LIBVSQ_END_NAMESPACE
