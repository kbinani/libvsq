/**
 * @file Mixer.cpp
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
#include "../include/libvsq/Mixer.hpp"
#include "../include/libvsq/TextStream.hpp"
#include "../include/libvsq/StringUtil.hpp"

LIBVSQ_BEGIN_NAMESPACE

Mixer::Mixer()
{
	this->masterFeder = 0;
	this->masterMute = 0;
	this->masterPanpot = 0;
	this->outputMode = 0;
}

Mixer::Mixer(int masterFeder, int masterPanpot, int masterMute, int outputMode)
{
	this->masterFeder = masterFeder;
	this->masterMute = masterMute;
	this->masterPanpot = masterPanpot;
	this->outputMode = outputMode;
}

Mixer Mixer::clone() const
{
	Mixer res(this->masterFeder, this->masterPanpot, this->masterMute, this->outputMode);
	for (MixerItem const& item : this->slave) {
		res.slave.push_back(item.clone());
	}
	return res;
}

LIBVSQ_END_NAMESPACE
