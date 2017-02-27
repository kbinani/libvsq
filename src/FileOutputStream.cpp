/**
 * @file FileOutputStream.cpp
 * Copyright © 2014,2017 kbinani
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
#include <libvsq/FileOutputStream.hpp>
#include <libvsq/FileUtil.hpp>

LIBVSQ_BEGIN_NAMESPACE

FileOutputStream::FileOutputStream(std::string const& filePath)
{
	stream.open(filePath.c_str(), std::ios::binary);
	if (!stream.is_open()) { throw OutputStream::IOException(); }
}

FileOutputStream::FileOutputStream(std::wstring const& filePath)
{
	FileUtil::open(stream, filePath, std::ios::out | std::ios::binary);
	if (!stream.is_open()) { throw OutputStream::IOException(); }
}

FileOutputStream::~FileOutputStream()
{
	if (stream.is_open()) { stream.close(); }
}

void FileOutputStream::write(char const* buffer, int64_t startIndex, int64_t length)
{
	stream.write(buffer + sizeof(char) * startIndex, length);
}

void FileOutputStream::write(int value)
{
	char c = 0xFF & value;
	stream.write(&c, 1);
}

void FileOutputStream::seek(int64_t position)
{
	stream.seekp(position, std::ios::beg);
}

int64_t FileOutputStream::getPointer()
{
	return (int64_t)stream.tellp();
}

void FileOutputStream::close()
{
	stream.close();
}

LIBVSQ_END_NAMESPACE
