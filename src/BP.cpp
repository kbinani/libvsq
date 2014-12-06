/**
 * @file BP.cpp
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
#include "../include/libvsq/BP.hpp"

LIBVSQ_BEGIN_NAMESPACE

BP::BP(int aValue, int aId)
	: value(aValue)
	, id(aId)
{}


BP BP::clone() const
{
	return BP(value, id);
}

LIBVSQ_END_NAMESPACE
