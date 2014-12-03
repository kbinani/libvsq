/**
 * EventWriteOption.hpp
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

#include "./Namespace.hpp"

VSQ_BEGIN_NAMESPACE

class EventWriteOption
{
public:
	bool length;
	bool note;
	bool dynamics;
	bool pmBendDepth;
	bool pmBendLength;
	bool pmbPortamentoUse;
	bool demDecGainRate;
	bool demAccent;

	EventWriteOption()
		: length(true)
		, note(true)
		, dynamics(true)
		, pmBendDepth(true)
		, pmBendLength(true)
		, pmbPortamentoUse(true)
		, demDecGainRate(true)
		, demAccent(true)
	{}
};

VSQ_END_NAMESPACE
