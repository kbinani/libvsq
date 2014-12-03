/**
 * @file StringUtil.cpp
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
#include "../include/libvsq/StringUtil.hpp"
#include <algorithm>
#include <functional>
#include <sstream>
#include <iomanip>

LIBVSQ_BEGIN_NAMESPACE

std::vector<std::string>
StringUtil::explode(std::string const& delimiter, std::string const& text, std::string::size_type limit, std::string const& escape)
{
	std::vector<std::string> result;
	std::string::size_type searchFrom = 0;
	std::string::size_type delimiterIndex = getDelimiterIndex(text, delimiter, escape, searchFrom);
	while (delimiterIndex != std::string::npos) {
		std::string token = text.substr(searchFrom, delimiterIndex - searchFrom);
		result.push_back(token);
		searchFrom = delimiterIndex + delimiter.length();
		if (result.size() + 1 == limit) {
			break;
		}
		delimiterIndex = getDelimiterIndex(text, delimiter, escape, searchFrom);
	}
	result.push_back(text.substr(searchFrom));
	return result;
}

std::string StringUtil::replace(std::string const& text, std::string const& search, std::string const& replace)
{
	if (search == replace) {
		return text;
	}
	std::string result = text;
	std::string::size_type index = result.find(search, 0);
	int searchLength = search.length();
	int replaceLength = replace.length();

	while (std::string::npos != index) {
		result.replace(index, searchLength, replace);
		index = result.find(search, index - searchLength + replaceLength + 1);
	}
	return result;
}

std::string StringUtil::toString(int value, int baseNumber)
{
	std::ostringstream oss;
	oss << std::uppercase << std::setbase(baseNumber) << value;
	return oss.str();
}

std::string StringUtil::toLower(std::string const& value)
{
	std::string result = value;
	transform(result.begin(), result.end(), result.begin(), _toLower);
	return result;
}

std::string StringUtil::repeat(std::string const& value, int count)
{
	std::ostringstream result;
	for (int i = 0; i < count; i++) {
		result << value;
	}
	return result.str();
}

std::string StringUtil::trim(std::string const& s)
{
	std::string::const_iterator left
		= std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun(::isspace)));
	std::string::const_reverse_iterator right
		= std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun(::isspace)));
	return (left < right.base()) ? std::string(left, right.base()) : std::string();
}

StringUtil::StringUtil()
{}

std::string::size_type StringUtil::getDelimiterIndex(std::string const& text, std::string const& delimiter, std::string const& escape, std::string::size_type searchFrom)
{
	if (escape.length() == 0) {
		return (int)text.find(delimiter, searchFrom);
	} else {
		while (searchFrom < text.length()) {
			int draft = (int)text.find(delimiter, searchFrom);
			if (draft == std::string::npos) {
				return std::string::npos;
			}
			int expectedEscapeIndex = draft - escape.length();
			if (0 <= expectedEscapeIndex) {
				// エスケープ文字が存在する可能性がある
				if (text.substr(expectedEscapeIndex, escape.length()) == escape) {
					// エスケープ文字だった場合
					searchFrom = draft + 1;
				} else {
					return draft;
				}
			} else {
				// エスケープ文字が存在し得ない
				return draft;
			}
		}
		return std::string::npos;
	}
}

char StringUtil::_toLower(char c)
{
	return tolower(c);
}

LIBVSQ_END_NAMESPACE
