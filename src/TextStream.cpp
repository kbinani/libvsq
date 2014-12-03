/**
 * @file TextStream.cpp
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
#include "../include/libvsq/TextStream.hpp"
#include <algorithm>
#include <sstream>

LIBVSQ_BEGIN_NAMESPACE

TextStream::TextStream()
{
	_arrayLength = 0;
	_length = 0;
	_position = -1;
}

TextStream::~TextStream()
{
	close();
}

int TextStream::getPointer()
{
	return _position;
}

void TextStream::setPointer(int value)
{
	_position = value;
}

std::string TextStream::get()
{
	++_position;
	std::string ret;
	ret += _array[_position];
	return ret;
}

std::string TextStream::readLine()
{
	std::ostringstream sb;
	// '\n'が来るまで読み込み
	while (_position + 1 < _length) {
		_position++;
		char c = _array[_position];
		if (c == (char)0x0A || c == 0) {
			break;
		}
		sb << c;
	}
	return sb.str();
}

bool TextStream::ready() const
{
	if (0 <= _position + 1 && _position + 1 < _length) {
		return true;
	} else {
		return false;
	}
}

void TextStream::write(std::string const& str)
{
	int len = str.size();
	int newSize = _position + 1 + len;
	int offset = _position + 1;
	_ensureCapacity(newSize);
	char const* ptr = str.c_str();
	for (int i = 0; i < len; ++i) {
		_array[offset + i] = ptr[i];
	}
	_position += len;
	_length = std::max(_length, newSize);
}

void TextStream::writeLine(std::string const& str)
{
	int len = str.size();
	int offset = _position + 1;
	int newSize = offset + len + 1;
	_ensureCapacity(newSize);
	for (int i = 0; i < len; ++i) {
		_array[offset + i] = str[i];
	}
	_array[offset + len] = (char)0x0A;
	_position += len + 1;
	_length = std::max(_length, newSize);
}

void TextStream::close()
{
	_length = 0;
}

std::string TextStream::toString() const
{
	std::string ret;
	if (!_array.empty()) {
		ret += _array.data();
	}
	return ret;
}

void TextStream::_ensureCapacity(int length)
{
	if (length > _arrayLength) {
		_array.resize(length + 1);
		for (int i = _arrayLength; i <= length; ++i) {
			_array[i] = 0;
		}
		_arrayLength = length;
	}
}

LIBVSQ_END_NAMESPACE
