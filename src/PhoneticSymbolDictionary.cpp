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
#include <algorithm>
#include <fstream>

#pragma execution_character_set("utf-8")

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
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidJp.inc"
		);
	}
	return &result;
}

PhoneticSymbolDictionary const* PhoneticSymbolDictionary::vocaloidEnDictionary()
{
	static PhoneticSymbolDictionary result;
	if (result.table.empty()) {
		// MSVC でも gcc でもコンパイルできる方法にしたらこうなった.
		// PhoneticSymbolDictionaryVocaloidEn.*.inc の元ファイルは resources/PhoneticSymbolDictionaryVocaloidEn.inc で,
		// これを resources/separate.rb を引数なしで実行すると自動生成される.
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.0.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.1.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.2.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.3.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.4.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.5.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.6.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.7.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.8.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.9.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.10.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.11.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.12.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.13.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.14.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.15.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.16.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.17.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.18.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.19.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.20.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.21.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.22.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.23.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.24.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.25.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.26.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.27.inc"
		);
		result.addAllFromText(
#			include "PhoneticSymbolDictionaryVocaloidEn.28.inc"
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

void PhoneticSymbolDictionary::addAllFromText(std::string const& source)
{
	std::string key[2];
	int index = 0;
	std::string::size_type offset = 0;
	size_t const size = source.size();
	for (size_t i = 0; i < size; ++i) {
		auto next = source.find('\n', offset);
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
