/**
 * @file Master.cpp
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
#include "../include/libvsq/Master.hpp"
#include "../include/libvsq/TextStream.hpp"
#include "../include/libvsq/StringUtil.hpp"
#include <sstream>

LIBVSQ_BEGIN_NAMESPACE

Master::Master()
{
	this->preMeasure = 1;
}

Master::Master(int preMeasure)
{
	this->preMeasure = preMeasure;
}

Master Master::clone() const
{
	return Master(this->preMeasure);
}

LIBVSQ_END_NAMESPACE
