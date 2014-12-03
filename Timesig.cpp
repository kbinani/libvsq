/**
 * Timesig.cpp
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
#include "./Timesig.hpp"
#include <sstream>

VSQ_BEGIN_NAMESPACE

Timesig::Timesig()
{
	this->tick_ = 0;
	this->numerator = 4;
	this->denominator = 4;
	this->barCount = 0;
}

Timesig::Timesig(int numerator, int denominator, int barCount)
{
	this->tick_ = 0;
	this->numerator = numerator;
	this->denominator = denominator;
	this->barCount = barCount;
}

tick_t Timesig::tick() const
{
	return tick_;
}

std::string Timesig::toString() const
{
	std::ostringstream oss;
	oss << "{Tick=" << this->tick_ << ", Numerator=" << this->numerator << ", Denominator=" << this->denominator << ", BarCount=" << this->barCount << "}";
	return oss.str();
}

bool Timesig::compareTo(Timesig const& item) const
{
	return (this->barCount - item.barCount) < 0;
}

Timesig Timesig::clone() const
{
	Timesig result(numerator, denominator, barCount);
	result.tick_ = tick_;
	return result;
}

bool Timesig::compare(Timesig const& a, Timesig const& b)
{
	return a.compareTo(b);
}

VSQ_END_NAMESPACE
