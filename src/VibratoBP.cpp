/**
 * @file VibratoBP.cpp
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
#include "../include/libvsq/VibratoBP.hpp"

LIBVSQ_BEGIN_NAMESPACE

VibratoBP::VibratoBP()
{
	x = 0.0;
	y = 0;
}

VibratoBP::VibratoBP(double x, int y)
{
	this->x = x;
	this->y = y;
}

int VibratoBP::compareTo(VibratoBP const& item) const
{
	return compareCore(*this, item);
}

bool VibratoBP::compare(VibratoBP const& a, VibratoBP const& b)
{
	return compareCore(a, b) < 0;
}

int VibratoBP::compareCore(VibratoBP const& a, VibratoBP const& b)
{
	double v = a.x - b.x;
	if (v > 0.0) {
		return 1;
	} else if (v < 0.0) {
		return -1;
	}
	return 0;
}

LIBVSQ_END_NAMESPACE
