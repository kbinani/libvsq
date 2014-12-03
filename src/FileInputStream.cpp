/**
 * @file FileInputStream.cpp
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
#include "../include/libvsq/FileInputStream.hpp"

LIBVSQ_BEGIN_NAMESPACE

FileInputStream::FileInputStream(std::string const& filePath)
	: stream(filePath.c_str(), std::ios::binary)
{}

int FileInputStream::read()
{
	int result = stream.get();
	if (stream.eof()) {
		return -1;
	} else {
		return result;
	}
}

int FileInputStream::read(char* buffer, int64_t startIndex, int64_t length)
{
	return stream.read(buffer + sizeof(char) * startIndex, length).gcount();
}

void FileInputStream::seek(int64_t position)
{
	stream.clear();
	stream.seekg(position);
}

int64_t FileInputStream::getPointer()
{
	return (int64_t)stream.tellg();
}

void FileInputStream::close()
{
	stream.close();
}

LIBVSQ_END_NAMESPACE
