/**
 * Master.cpp
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
#include "./Master.hpp"
#include "./TextStream.hpp"
#include "./StringUtil.hpp"
#include <sstream>

VSQ_BEGIN_NAMESPACE

Master::Master()
{
	this->preMeasure = 1;
}

Master::Master(int preMeasure)
{
	this->preMeasure = preMeasure;
}

Master::Master(TextStream& stream, std::string& lastLine)
{
	this->preMeasure = 0;
	lastLine = stream.readLine();
	while (lastLine.find("[") == std::string::npos) {
		std::vector<std::string> spl = StringUtil::explode("=", lastLine);
		if (spl[0] == "PreMeasure") {
			this->preMeasure = StringUtil::parseInt<int>(spl[1]);
		}
		if (!stream.ready()) {
			break;
		}
		lastLine = stream.readLine();
	}
}

Master Master::clone() const
{
	return Master(this->preMeasure);
}

void Master::write(TextStream& stream) const
{
	stream.writeLine("[Master]");
	std::ostringstream oss;
	oss << "PreMeasure=" << this->preMeasure;
	stream.writeLine(oss.str());
}

VSQ_END_NAMESPACE
