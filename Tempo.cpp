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

const std::string Tempo::toString() const
{
	std::ostringstream oss;
	oss << "{Clock=" << clock << ", Tempo=" << tempo << ", Time=" << _time << "}";
	return oss.str();
}

Tempo::Tempo(tick_t clock, int tempo)
{
	this->clock = clock;
	this->tempo = tempo;
	_time = 0.0;
}

Tempo::Tempo()
{
	clock = 0;
	tempo = 0;
	_time = 0.0;
}

int Tempo::compareTo(const Tempo& entry) const
{
	return compareCore(*this, entry);
}

bool Tempo::equals(const Tempo& entry) const
{
	if (clock == entry.clock) {
		return true;
	} else {
		return false;
	}
}

double Tempo::getTime() const
{
	return _time;
}

Tempo Tempo::clone() const
{
	Tempo result;
	result.clock = clock;
	result.tempo = tempo;
	result._time = _time;
	return result;
}

bool Tempo::compare(const Tempo& a, const Tempo& b)
{
	return compareCore(a, b) < 0;
}

int Tempo::compareCore(const Tempo& a, const Tempo& b)
{
	return (int)(a.clock - b.clock);
}

VSQ_END_NAMESPACE
