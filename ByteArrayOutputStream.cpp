/**
 * ByteArrayOutputStream.cpp
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
#include "./ByteArrayOutputStream.hpp"
#include <cstdlib>

VSQ_BEGIN_NAMESPACE

ByteArrayOutputStream::ByteArrayOutputStream()
{
	_pointer = 0;
	_arrayLength = UNIT_BUFFER_LENGTH;
	_array = (char*)::calloc(_arrayLength, sizeof(char));
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

std::string ByteArrayOutputStream::toString()
{
	std::string result(_array, _length);
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
	if (_array) {
		free(_array);
		_array = NULL;
	}
	_length = 0;
	_arrayLength = 0;
}

void ByteArrayOutputStream::ensureBufferLength(int length)
{
	int amount = length - _arrayLength;
	if (0 < amount) {
		char* temporary = (char*)realloc(_array, length * sizeof(char));
		if (!temporary) {
			close();
			return;
		}
		_array = temporary;
		for (int i = _arrayLength; i < _arrayLength + amount; i++) {
			_array[i] = 0;
		}
		_arrayLength = length;
	}
}

VSQ_END_NAMESPACE
