/**
 * EventType.cpp
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
#include "./EventType.hpp"

VSQ_BEGIN_NAMESPACE

const std::string EventType::toString(EventTypeEnum value)
{
	if (value == EventType::SINGER) {
		return "Singer";
	} else if (value == EventType::NOTE) {
		return "Anote";
	} else if (value == EventType::ICON) {
		return "Aicon";
	} else {
		return "Unknown";
	}
}

EventType::EventType()
{}

VSQ_END_NAMESPACE
