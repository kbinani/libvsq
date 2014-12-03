/**
 * @file PhoneticSymbolDictionary.cpp
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
#include "../include/libvsq/PhoneticSymbolDictionary.hpp"
#include "../include/libvsq/StringUtil.hpp"
#include <fstream>

LIBVSQ_BEGIN_NAMESPACE

PhoneticSymbolDictionary::Element::Element(std::string const& word, std::string const& symbol)
{
	_word = word;
	_rawSymbol = symbol;
	_symbol = StringUtil::replace(symbol, "\t", " ");
}

std::string PhoneticSymbolDictionary::Element::word() const
{
	return _word;
}

std::string PhoneticSymbolDictionary::Element::rawSymbol() const
{
	return _rawSymbol;
}

std::string PhoneticSymbolDictionary::Element::symbol() const
{
	return _symbol;
}

PhoneticSymbolDictionary::PhoneticSymbolDictionary(std::string const& filePath)
{
	_maxDivisions = 1;
	read(filePath);
}

PhoneticSymbolDictionary::PhoneticSymbolDictionary()
{
	_maxDivisions = 1;
}

PhoneticSymbolDictionary::Element const* PhoneticSymbolDictionary::attach(std::string const& word) const
{
	std::string key = StringUtil::toLower(word);
	auto index = table.find(key);
	if (index == table.end()) {
		return 0;
	} else {
		return &index->second;
	}
}

int PhoneticSymbolDictionary::maxDivisions() const
{
	return _maxDivisions;
}

PhoneticSymbolDictionary const* PhoneticSymbolDictionary::vocaloidJpDictionary()
{
	static PhoneticSymbolDictionary result;
	if (result.table.empty()) {
		result.init(
#			include "PhoneticSymbolDictionaryVocaloidJp.inc"
		);
	}
	return &result;
}

PhoneticSymbolDictionary const* PhoneticSymbolDictionary::vocaloidEnDictionary()
{
	static PhoneticSymbolDictionary result;
	if (result.table.empty()) {
		result.init(
#			include "PhoneticSymbolDictionaryVocaloidEn.inc"
		);
	}
	return &result;
}

void PhoneticSymbolDictionary::read(std::string const& filePath)
{
	std::ifstream stream(filePath.c_str());
	std::string line;
	while (stream && std::getline(stream, line)) {
		line = StringUtil::trim(line);
		std::vector<std::string> parameters = StringUtil::explode("\t\t", line);
		if (parameters.size() == 2) {
			add(parameters[0], parameters[1]);
		}
	}
}

void PhoneticSymbolDictionary::add(std::string const& word, std::string const& symbol)
{
	std::string key = StringUtil::toLower(StringUtil::replace(word, "-\t", ""));
	if (table.find(key) == table.end()) {
		table.insert(std::make_pair(key, Element(word, symbol)));

		std::vector<std::string> splittedWord = StringUtil::explode("\t", symbol);
		_maxDivisions = std::max(_maxDivisions, (int)splittedWord.size());
	}
}

void PhoneticSymbolDictionary::init(std::string const& source)
{
	std::string key[2];
	int index = 0;
	std::string::size_type offset = 0;
	size_t const size = source.size();
	for (size_t i = 0; i < size; ++i) {
		std::string::size_type next = source.find('\n', offset);
		if (next == std::string::npos) {
			break;
		}
		key[index] = source.substr(offset, next - offset);
		offset = next + 1;
		++index;
		if (index == 2) {
			index = 0;
			add(key[0], key[1]);
		}
	}
}

LIBVSQ_END_NAMESPACE
