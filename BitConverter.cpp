/**
 * BitConverter.cpp
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
#include "./BitConverter.hpp"

VSQ_BEGIN_NAMESPACE

std::vector<char>
BitConverter::getBytesUInt16BE(uint16_t value)
{
	std::vector<char> result;
	result.push_back(((0xff00 & value) >> 8) & 0xff);
	result.push_back(((0x00ff & value)) & 0xff);
	return result;
}

std::vector<char>
BitConverter::getBytesUInt32BE(uint32_t value)
{
	std::vector<char> result;
	result.push_back(((0xff000000 & value) >> 24) & 0xff);
	result.push_back(((0x00ff0000 & value) >> 16) & 0xff);
	result.push_back(((0x0000ff00 & value) >> 8) & 0xff);
	result.push_back(((0x000000ff & value)) & 0xff);
	return result;
}

uint32_t BitConverter::makeUInt32LE(char bytes[4])
{
	return (0xff000000 & (bytes[3] << 24)) |
		   (0x00ff0000 & (bytes[2] << 16)) |
		   (0x0000ff00 & (bytes[1] << 8)) |
		   (0x000000ff & bytes[0]);
}

uint32_t BitConverter::makeUInt32BE(char bytes[4])
{
	return (0xff000000 & (bytes[0] << 24)) |
		   (0x00ff0000 & (bytes[1] << 16)) |
		   (0x0000ff00 & (bytes[2] << 8)) |
		   (0x000000ff & bytes[3]);
}

int16_t BitConverter::makeInt16LE(char bytes[2])
{
	return (0xff00 & (bytes[1] << 8)) |
		   (0x00ff & bytes[0]);
}

uint16_t BitConverter::makeUInt16BE(char bytes[2])
{
	return (0xff00 & (bytes[0] << 8)) |
		   (0x00ff & bytes[1]);
}

VSQ_END_NAMESPACE
