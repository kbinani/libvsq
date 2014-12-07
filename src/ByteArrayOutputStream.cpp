/**
 * @file ByteArrayOutputStream.cpp
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
#include "../include/libvsq/ByteArrayOutputStream.hpp"
#include <cstdlib>
#include <algorithm>

LIBVSQ_BEGIN_NAMESPACE

ByteArrayOutputStream::ByteArrayOutputStream()
{
	_pointer = 0;
	_arrayLength = UNIT_BUFFER_LENGTH;
	_array.resize(_arrayLength);
	_length = 0;
}

ByteArrayOutputStream::~ByteArrayOutputStream()
{
	close();
}

void ByteArrayOutputStream::write(int byte)
{
	ensureBufferLength(_pointer + 1);
	_array[_pointer] = (char)byte;
	_pointer++;
	_length = std::max(_length, _pointer);
}

void ByteArrayOutputStream::write(char const* array, int64_t startIndex, int64_t length)
{
	ensureBufferLength(_pointer + length);
	for (int i = 0; i < length; i++) {
		_array[_pointer + i] = array[startIndex + i];
	}
	_pointer += length;
	_length = std::max(_length, _pointer);
}

std::string ByteArrayOutputStream::toString() const
{
	std::string result(_array.data(), _length);
	return result;
}

int64_t ByteArrayOutputStream::getPointer()
{
	return _pointer;
}

void ByteArrayOutputStream::seek(int64_t position)
{
	_pointer = (position < 0) ? 0 : position;
}

void ByteArrayOutputStream::close()
{
	_length = 0;
	_arrayLength = 0;
}

void ByteArrayOutputStream::ensureBufferLength(int length)
{
	int amount = length - _arrayLength;
	if (0 < amount) {
		_arrayLength = length;
		_array.resize(_arrayLength);
	}
}

LIBVSQ_END_NAMESPACE
