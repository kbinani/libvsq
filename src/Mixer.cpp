/**
 * Mixer.cpp
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

Mixer::Mixer(TextStream& stream, std::string& lastLine)
{
	this->masterFeder = 0;
	this->masterPanpot = 0;
	this->masterMute = 0;
	this->outputMode = 0;
	int tracks = 0;
	std::string buffer = "";
	lastLine = stream.readLine();
	while (lastLine.at(0) != '[') {
		std::vector<std::string> params = StringUtil::explode("=", lastLine);
		if (params[0] == "MasterFeder") {
			this->masterFeder = StringUtil::parseInt<int>(params[1]);
		} else if (params[0] == "MasterPanpot") {
			this->masterPanpot = StringUtil::parseInt<int>(params[1]);
		} else if (params[0] == "MasterMute") {
			this->masterMute = StringUtil::parseInt<int>(params[1]);
		} else if (params[0] == "OutputMode") {
			this->outputMode = StringUtil::parseInt<int>(params[1]);
		} else if (params[0] == "Tracks") {
			tracks = StringUtil::parseInt<int>(params[1]);
		} else {
			if (params[0].find("Feder") == 0 ||
				params[0].find("Panpot") == 0 ||
				params[0].find("Mute") == 0  ||
				params[0].find("Solo") == 0) {
				buffer = buffer + params[0] + "=" + params[1] + "\n";
			}
		}
		if (!stream.ready()) {
			break;
		}
		lastLine = stream.readLine();
	}

	for (int i = 0; i < tracks; i++) {
		this->slave.push_back(MixerItem(0, 0, 0, 0));
	}
	std::vector<std::string> spl = StringUtil::explode("\n", buffer);
	for (std::string const& s : spl) {
		std::string ind = "";
		int index;
		std::vector<std::string> spl2 = StringUtil::explode("=", s);
		if (spl2[0].find("Feder") == 0) {
			ind = spl2[0].substr(std::string("Feder").size());
			index = StringUtil::parseInt<int>(ind);
			this->slave[index].feder = StringUtil::parseInt<int>(spl2[1]);
		} else if (spl2[0].find("Panpot") == 0) {
			ind = spl2[0].substr(std::string("Panpot").size());
			index = StringUtil::parseInt<int>(ind);
			this->slave[index].panpot = StringUtil::parseInt<int>(spl2[1]);
		} else if (spl2[0].find("Mute") == 0) {
			ind = spl2[0].substr(std::string("Mute").size());
			index = StringUtil::parseInt<int>(ind);
			this->slave[index].mute = StringUtil::parseInt<int>(spl2[1]);
		} else if (spl2[0].find("Solo") == 0) {
			ind = spl2[0].substr(std::string("Solo").size());
			index = StringUtil::parseInt<int>(ind);
			this->slave[index].solo = StringUtil::parseInt<int>(spl2[1]);
		}
	}
}

Mixer Mixer::clone() const
{
	Mixer res(this->masterFeder, this->masterPanpot, this->masterMute, this->outputMode);
	for (MixerItem const& item : this->slave) {
		res.slave.push_back(item.clone());
	}
	return res;
}

void Mixer::write(TextStream& stream) const
{
	stream.writeLine("[Mixer]");
	stream.writeLine("MasterFeder=" + StringUtil::toString(this->masterFeder));
	stream.writeLine("MasterPanpot=" + StringUtil::toString(this->masterPanpot));
	stream.writeLine("MasterMute=" + StringUtil::toString(this->masterMute));
	stream.writeLine("OutputMode=" + StringUtil::toString(this->outputMode));
	stream.writeLine("Tracks=" + StringUtil::toString(this->slave.size()));
	for (int i = 0; i < this->slave.size(); ++i) {
		MixerItem item = this->slave[i];
		stream.writeLine("Feder" + StringUtil::toString(i) + "=" + StringUtil::toString(item.feder));
		stream.writeLine("Panpot" + StringUtil::toString(i) + "=" + StringUtil::toString(item.panpot));
		stream.writeLine("Mute" + StringUtil::toString(i) + "=" + StringUtil::toString(item.mute));
		stream.writeLine("Solo" + StringUtil::toString(i) + "=" + StringUtil::toString(item.solo));
	}
}

LIBVSQ_END_NAMESPACE
