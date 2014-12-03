/**
 * Tempo.cpp
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
#include "./Tempo.hpp"
#include <sstream>

VSQ_BEGIN_NAMESPACE

std::string Tempo::toString() const
{
	std::ostringstream oss;
	oss << "{Tick=" << tick << ", Tempo=" << tempo << ", Time=" << _time << "}";
	return oss.str();
}

Tempo::Tempo(tick_t tick, int tempo)
{
	this->tick = tick;
	this->tempo = tempo;
	_time = 0.0;
}

Tempo::Tempo()
{
	tick = 0;
	tempo = 0;
	_time = 0.0;
}

int Tempo::compareTo(Tempo const& entry) const
{
	return compareCore(*this, entry);
}

bool Tempo::equals(Tempo const& entry) const
{
	if (tick == entry.tick) {
		return true;
	} else {
		return false;
	}
}

double Tempo::time() const
{
	return _time;
}

Tempo Tempo::clone() const
{
	Tempo result;
	result.tick = tick;
	result.tempo = tempo;
	result._time = _time;
	return result;
}

bool Tempo::compare(Tempo const& a, Tempo const& b)
{
	return compareCore(a, b) < 0;
}

int Tempo::compareCore(Tempo const& a, Tempo const& b)
{
	return (int)(a.tick - b.tick);
}

VSQ_END_NAMESPACE
