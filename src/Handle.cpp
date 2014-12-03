/**
 * Handle.cpp
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
#include "../include/libvsq/Handle.hpp"
#include "../include/libvsq/StringUtil.hpp"

VSQ_BEGIN_NAMESPACE

Handle::Handle(HandleType type)
{
	init();
	_type = type;
	if (_type == HandleType::DYNAMICS) {
		_init_icon_dynamics();
	} else if (type == HandleType::NOTE_HEAD) {
		_articulation = ArticulationType::NOTE_ATTACK;
	} else if (type == HandleType::VIBRATO) {
		_init_vibrato();
	} else if (type == HandleType::LYRIC) {
		_init_lyric();
	}
}

ArticulationType Handle::articulation() const
{
	return _articulation;
}

bool Handle::isDynaffType() const
{
	return iconId.find(Handle::getIconIdPrefixDynaff()) == 0;
}

bool Handle::isCrescendType() const
{
	return iconId.find(Handle::getIconIdPrefixCrescend()) == 0;
}

bool Handle::isDecrescendType() const
{
	return iconId.find(Handle::getIconIdPrefixDecrescend()) == 0;
}

tick_t Handle::length() const
{
	return _length;
}

void Handle::length(tick_t value)
{
	_length = value;
}

Lyric const& Handle::get(int index) const
{
	return _lyrics[index];
}

Lyric& Handle::get(int index)
{
	return _lyrics[index];
}

void Handle::set(int index, Lyric const& value)
{
	if (_lyrics.size() < index + 1) {
		int remain = index + 1 - _lyrics.size();
		for (int i = 0; i < remain; i++) {
			_lyrics.push_back(Lyric("", ""));
		}
	}
	_lyrics[index] = value;
}

void Handle::add(Lyric const& lyric)
{
	_lyrics.push_back(lyric);
}

int Handle::size() const
{
	return _lyrics.size();
}

std::string Handle::displayString() const
{
	return ids + caption;
}

HandleType Handle::type() const
{
	return _type;
}

Handle Handle::clone() const
{
	if (_type == HandleType::DYNAMICS) {
		Handle ret(HandleType::DYNAMICS);
		ret.iconId = iconId;
		ret.ids = ids;
		ret.original = original;
		ret.caption = caption;
		ret.startDyn = startDyn;
		ret.endDyn = endDyn;
		//if( 0 != _dynBP ){
		ret.dynBP = dynBP.clone();
		//}
		ret.length(length());
		return ret;
	} else if (_type == HandleType::LYRIC) {
		Handle result(HandleType::LYRIC);
		result.index = index;
		for (int i = 0; i < _lyrics.size(); i++) {
			Lyric buf = _lyrics[i].clone();
			result._lyrics.push_back(buf);
		}
		return result;
	} else if (_type == HandleType::NOTE_HEAD) {
		Handle result(HandleType::NOTE_HEAD);
		result.index = index;
		result.iconId = iconId;
		result.ids = ids;
		result.original = original;
		result.caption = caption;
		result.length(length());
		result.duration = duration;
		result.depth = depth;
		return result;
	} else if (_type == HandleType::SINGER) {
		Handle ret(HandleType::SINGER);
		ret.caption = caption;
		ret.iconId = iconId;
		ret.ids = ids;
		ret.index = index;
		ret.language = language;
		ret.length(_length);
		ret.original = original;
		ret.program = program;
		return ret;
	} else if (_type == HandleType::VIBRATO) {
		Handle result(HandleType::VIBRATO);
		result.index = index;
		result.iconId = iconId;
		result.ids = ids;
		result.original = original;
		result.caption = caption;
		result.length(length());
		result.startDepth = startDepth;
		//if( 0 != _depthBP ){
		result.depthBP = depthBP.clone();
		//}
		result.startRate = startRate;
		//if( 0 != _rateBP ){
		result.rateBP = rateBP.clone();
		//}
		return result;
	} else {
		return Handle(HandleType::UNKNOWN);
	}
}

int Handle::getHandleIndexFromString(std::string const& s)
{
	std::vector<std::string> spl = StringUtil::explode("#", s);
	return StringUtil::parseInt<int>(spl[1]);
}

void Handle::init()
{
	_type = HandleType::UNKNOWN;
	_articulation = ArticulationType::NONE;
	index = 0;
	iconId = "";
	ids = "";
	original = 0;
	caption = "";
	_length = 0;
	startDepth = 0;
	startRate = 0;
	language = 0;
	program = 0;
	duration = 0;
	depth = 0;
	startDyn = 0;
	endDyn = 0;
	addQuotationMark = true;
}

void Handle::_init_vibrato()
{
	_articulation = ArticulationType::VIBRATO;
	index = 0;
	iconId = "$04040000";
	ids = "";
	original = 0;
	startRate = 64;
	startDepth = 64;
	rateBP = VibratoBPList();
	depthBP = VibratoBPList();
}

void Handle::_init_icon_dynamics()
{
	_articulation = ArticulationType::DYNAFF;
	iconId = "";
	ids = "";
	original = 0;
}

void Handle::_init_lyric()
{
	index = 0;
	_lyrics.clear();
}

VSQ_END_NAMESPACE
