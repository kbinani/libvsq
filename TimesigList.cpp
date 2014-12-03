/**
 * TimesigList.cpp
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
#include "./TimesigList.hpp"

VSQ_BEGIN_NAMESPACE

TimesigList::TimesigList()
{}

TimesigList::~TimesigList()
{
	clear();
}

Timesig TimesigList::get(int index) const
{
	return list[index];
}

void TimesigList::push(Timesig const& item)
{
	pushWithoutSort(item);
	updateTimesigInfo();
}

int TimesigList::size() const
{
	return list.size();
}

Timesig TimesigList::getTimesigAt(tick_t clock) const
{
	Timesig ret;
	ret.numerator = 4;
	ret.denominator = 4;
	ret.barCount = 0;

	int index = -1;
	int count = list.size();
	if (0 < count) {
		for (int i = count - 1; i >= 0; i--) {
			index = i;
			if (list[i].getClock() <= clock) {
				break;
			}
		}
	}

	if (0 <= index) {
		ret.numerator = list[index].numerator;
		ret.denominator = list[index].denominator;
		int tickPerBar = 480 * 4 / ret.denominator * ret.numerator;
		int deltaBar = (int)::floor((double)((clock - list[index].getClock()) / tickPerBar));
		ret.barCount = list[index].barCount + deltaBar;
	} else {
		int tickPerBar = 480 * 4 / ret.denominator * ret.numerator;
		int deltaBar = (int)::floor((double)(clock / tickPerBar));
		ret.barCount = deltaBar;
	}

	return ret;
}

tick_t TimesigList::getClockFromBarCount(int barCount) const
{
	int index = 0;
	for (int i = list.size() - 1; i >= 0; i--) {
		index = i;
		if (list[i].barCount <= barCount) {
			break;
		}
	}
	Timesig item = list[index];
	int numerator = item.numerator;
	int denominator = item.denominator;
	tick_t initClock = item.getClock();
	int initBarCount = item.barCount;
	int clockPerBar = numerator * 480 * 4 / denominator;
	return initClock + (barCount - initBarCount) * clockPerBar;
}

void TimesigList::clear()
{
	list.clear();
}

int TimesigList::getBarCountFromClock(tick_t clock) const
{
	int index = 0;
	int c = list.size();
	for (int i = c - 1; i >= 0; i--) {
		index = i;
		if (list[i].getClock() <= clock) {
			break;
		}
	}
	int bar_count = 0;
	if (index >= 0) {
		Timesig item = list[index];
		tick_t last_clock = item.getClock();
		int t_bar_count = item.barCount;
		int numerator = item.numerator;
		int denominator = item.denominator;
		tick_t clock_per_bar = numerator * 480 * 4 / denominator;
		bar_count = t_bar_count + (clock - last_clock) / clock_per_bar;
	}
	return bar_count;
}

void TimesigList::pushWithoutSort(Timesig const& item)
{
	int index = -1;
	int count = list.size();
	for (int i = 0; i < count; i++) {
		if (list[i].barCount == item.barCount) {
			index = i;
			break;
		}
	}

	if (0 <= index) {
		list[index] = item;
	} else {
		list.push_back(item);
	}
}

void TimesigList::updateTimesigInfo()
{
	if (! list.empty()) {
		std::stable_sort(list.begin(), list.end(), Timesig::compare);

		int count = list.size();
		for (int j = 1; j < count; j++) {
			Timesig item = list[j - 1];
			int numerator = item.numerator;
			int denominator = item.denominator;
			tick_t clock = item.getClock();
			int bar_count = item.barCount;
			int diff = (int)::floor((double)(480 * 4 / denominator * numerator));
			clock = clock + (list[j].barCount - bar_count) * diff;
			list[j].clock = clock;
		}
	}
}

VSQ_END_NAMESPACE
