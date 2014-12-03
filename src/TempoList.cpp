/**
 * TempoList.cpp
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
#include "../include/libvsq/TempoList.hpp"
#include <algorithm>

LIBVSQ_BEGIN_NAMESPACE

TempoList::Iterator::Iterator(std::vector<Tempo> const* list)
{
	this->list = list;
	this->iterator = list->begin();
}

bool TempoList::Iterator::hasNext()
{
	return iterator != this->list->end();
}

Tempo TempoList::Iterator::next()
{
	Tempo result = *iterator;
	++iterator;
	return result;
}

TempoList::Iterator TempoList::iterator() const
{
	return Iterator(&_array);
}

void TempoList::sort()
{
	std::stable_sort(_array.begin(), _array.end(), Tempo::compare);
}

void TempoList::push(Tempo const& value)
{
	_array.push_back(value);
}

int TempoList::size() const
{
	return _array.size();
}

Tempo const& TempoList::get(int index) const
{
	return _array[index];
}

void TempoList::set(int index, Tempo const& value)
{
	_array[index] = value;
}

double TempoList::tickFromTime(double time) const
{
	// timeにおけるテンポを取得
	int tempo = TempoList::baseTempo;
	tick_t base_tick = 0;
	double base_time = 0.0;
	int c = _array.size();
	if (c == 0) {
		tempo = TempoList::baseTempo;
		base_tick = 0;
		base_time = 0.0;
	} else if (c == 1) {
		tempo = _array[0].tempo;
		base_tick = _array[0].tick;
		base_time = _array[0]._time;
	} else {
		for (int i = c - 1; i >= 0; --i) {
			Tempo item = _array[i];
			if (item._time < time) {
				return item.tick + (time - item._time) * TempoList::gatetimePerQuater * 1000000.0 / item.tempo;
			}
		}
	}
	double dt = time - base_time;
	return base_tick + dt * TempoList::gatetimePerQuater * 1000000.0 / tempo;
}

void TempoList::updateTempoInfo()
{
	int c = _array.size();
	if (c == 0) {
		_array.push_back(Tempo(0, TempoList::baseTempo));
	}
	std::stable_sort(_array.begin(), _array.end(), Tempo::compare);
	Tempo item0 = _array[0];
	if (item0.tick != 0) {
		item0._time = TempoList::baseTempo * item0.tick / (TempoList::gatetimePerQuater * 1000000.0);
	} else {
		item0._time = 0.0;
	}
	double prev_time = item0._time;
	tick_t prev_tick = item0.tick;
	int prev_tempo = item0.tempo;
	double inv_tpq_sec = 1.0 / (TempoList::gatetimePerQuater * 1000000.0);
	for (int i = 1; i < c; ++i) {
		_array[i]._time = prev_time + prev_tempo * (_array[i].tick - prev_tick) * inv_tpq_sec;

		Tempo itemi = _array[i];
		prev_time = itemi._time;
		prev_tempo = itemi.tempo;
		prev_tick = itemi.tick;
	}
}

double TempoList::timeFromTick(double tick) const
{
	int c = _array.size();
	for (int i = c - 1; i >= 0; --i) {
		Tempo item = _array[i];
		if (item.tick < tick) {
			double init = item.time();
			tick_t dtick = tick - item.tick;
			double sec_per_tick1 = item.tempo * 1e-6 / 480.0;
			return init + dtick * sec_per_tick1;
		}
	}

	double sec_per_tick = TempoList::baseTempo * 1e-6 / 480.0;
	return tick * sec_per_tick;
}

int TempoList::tempoAt(tick_t tick) const
{
	int index = 0;
	int c = size();
	for (int i = c - 1; i >= 0; --i) {
		index = i;
		if (_array[i].tick <= tick) {
			break;
		}
	}
	return _array[index].tempo;
}

void TempoList::clear()
{
	_array.clear();
}

LIBVSQ_END_NAMESPACE
