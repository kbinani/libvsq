/**
 * Common.cpp
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
#include "./Common.hpp"
#include "./TextStream.hpp"
#include "./StringUtil.hpp"
#include <vector>
#include <sstream>

VSQ_BEGIN_NAMESPACE

Common::Common()
{
	init();
}

Common::Common(TextStream& stream, std::string& lastLine)
{
	init();
	this->version = "";
	this->name = "";
	this->color = "0,0,0";
	this->dynamicsMode = DynamicsMode::STANDARD;
	this->_playMode = PlayMode::PLAY_WITH_SYNTH;
	lastLine = stream.readLine();
	while (lastLine.find("[") != 0) {
		std::vector<std::string> spl = StringUtil::explode("=", lastLine);
		std::string search = spl[0];
		if (search == "Version") {
			this->version = spl[1];
		} else if (search == "Name") {
			this->name = spl[1];
		} else if (search == "Color") {
			this->color = spl[1];
		} else if (search == "DynamicsMode") {
			this->dynamicsMode = (DynamicsMode::DynamicsModeEnum)StringUtil::parseInt<int>(spl[1]);
		} else if (search == "PlayMode") {
			this->_playMode = (PlayMode::PlayModeEnum)StringUtil::parseInt<int>(spl[1]);
		}
		if (!stream.ready()) {
			break;
		}
		lastLine = stream.readLine();
	}
}

Common::Common(std::string const& name, int r, int g, int b, DynamicsMode::DynamicsModeEnum dynamicsMode, PlayMode::PlayModeEnum playMode)
{
	init();
	this->version = "DSB301";
	this->name = name;
	std::ostringstream oss;
	oss << r << "," << g << "," << b;
	this->color = oss.str();
	this->dynamicsMode = dynamicsMode;
	this->_playMode = playMode;
}

Common Common::clone() const
{
	std::vector<std::string> spl = StringUtil::explode(",", this->color);
	int r = StringUtil::parseInt<int>(spl[0]);
	int g = StringUtil::parseInt<int>(spl[1]);
	int b = StringUtil::parseInt<int>(spl[2]);
	Common result(this->name, r, g, b, this->dynamicsMode, this->_playMode);
	result.version = this->version;
	result._lastPlayMode = this->_lastPlayMode;
	return result;
}

PlayMode::PlayModeEnum Common::playMode() const
{
	return _playMode;
}

void Common::setPlayMode(PlayMode::PlayModeEnum mode)
{
	_playMode = mode;
	if (mode != PlayMode::OFF) {
		_lastPlayMode = mode;
	}
}

PlayMode::PlayModeEnum Common::lastPlayMode() const
{
	return _lastPlayMode;
}

void Common::write(TextStream& stream) const
{
	stream.writeLine("[Common]");
	stream.writeLine("Version=" + this->version);
	stream.writeLine("Name=" + this->name);
	stream.writeLine("Color=" + this->color);
	std::ostringstream oss;
	oss << "DynamicsMode=" << (int)this->dynamicsMode;
	stream.writeLine(oss.str());

	oss.str("");
	oss << "PlayMode=" << (int)this->_playMode;
	stream.writeLine(oss.str());
}

void Common::init()
{
	this->version = "DSB301";
	this->name = "Miku";
	this->dynamicsMode = DynamicsMode::EXPERT;
	this->_playMode = PlayMode::PLAY_WITH_SYNTH;
	this->color = "179,181,123";
	this->_lastPlayMode = PlayMode::PLAY_WITH_SYNTH;
}

VSQ_END_NAMESPACE
