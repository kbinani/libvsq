/**
 * @file MixerItem.cpp
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
#include "../include/libvsq/MixerItem.hpp"

LIBVSQ_BEGIN_NAMESPACE

MixerItem::MixerItem(int feder, int panpot, int mute, int solo)
{
	this->feder = feder;
	this->panpot = panpot;
	this->mute = mute;
	this->solo = solo;
}

MixerItem MixerItem::clone() const
{
	return MixerItem(this->feder, this->panpot, this->mute, this->solo);
}

LIBVSQ_END_NAMESPACE
