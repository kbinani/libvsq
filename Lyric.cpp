/**
 * Lyric.cpp
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
#include "./Lyric.hpp"
#include "./StringUtil.hpp"
#include "./PhoneticSymbol.hpp"
#include <cstdlib>
#include <sstream>

VSQ_BEGIN_NAMESPACE

Lyric::Lyric(std::string const& line)
{
	if (line.size() == 0) {
		phrase = "a";
		setPhoneticSymbol("a");
		lengthRatio = 1.0;
		isProtected = false;
		setConsonantAdjustment("0");
		return;
	}
	int len = line.size();
	int indx = 0;
	int dquote_count = 0;
	std::string work = "";
	std::string consonantAdjustment = "";
	for (int i = 0; i < len; i++) {
		char c = line[i];
		if (c == ',' || i + 1 == len) {
			if (i + 1 == len) {
				work = work + c;
			}
			if (dquote_count % 2 == 0) {
				// ,の左側に偶数個の"がある場合→,は区切り文字
				indx = indx + 1;
				std::string search = "\"";
				if (indx == 1) {
					// phrase
					work = StringUtil::replace(work, "\"\"", "\"");   // "は""として保存される
					if (work.find(search) == 0 && work.find_last_of(search) == (work.size() - search.size())) {
						int l = work.size();
						if (l > search.size() * 2) {
							phrase = work.substr(search.size(), l - search.size() * 2);
						} else {
							phrase = "a";
						}
					} else {
						phrase = work;
					}
					work = "";
				} else if (indx == 2) {
					// symbols
					std::string symbols = "";
					if ((work.find(search) == 0) && (work.find_last_of(search) == (work.size() - search.size()))) {
						int l = work.size();
						if (l > search.size() * 2) {
							symbols = work.substr(search.size(), l - search.size() * 2);
						} else {
							symbols = "a";
						}
					} else {
						symbols = work;
					}
					setPhoneticSymbol(symbols);
					work = "";
				} else if (indx == 3) {
					// lengthRatio
					lengthRatio = atof(work.c_str());
					work = "";
				} else {
					if (indx - 3 <= _phoneticSymbol.size()) {
						// consonant adjustment
						if (indx - 3 == 1) {
							consonantAdjustment = consonantAdjustment + work;
						} else {
							consonantAdjustment = consonantAdjustment + "," + work;
						}
					} else {
						// protected
						isProtected = (work == "1");
					}
					work = "";
				}
			} else {
				// ,の左側に奇数個の"がある場合→,は歌詞等の一部
				work = work + "" + c;
			}
		} else {
			work = work + "" + c;
			if (c == '"') {
				dquote_count = dquote_count + 1;
			}
		}
	}
	setConsonantAdjustment(consonantAdjustment);
}

Lyric::Lyric(std::string const& phrase, std::string const& phoneticSymbol)
{
	this->phrase = phrase;
	_consonantAdjustment.clear();
	this->setPhoneticSymbol(phoneticSymbol);
	lengthRatio = 1.0;
	isProtected = false;
}

bool Lyric::equalsForSynth(Lyric const& item) const
{
	if (getPhoneticSymbol() != item.getPhoneticSymbol()) {
		return false;
	}
	if (getConsonantAdjustment() != item.getConsonantAdjustment()) {
		return false;
	}
	return true;
}

bool Lyric::equals(Lyric const& item) const
{
	if (false == equalsForSynth(item)) {
		return false;
	}
	if (isProtected != item.isProtected) {
		return false;
	}
	if (phrase != item.phrase) {
		return false;
	}
	if (lengthRatio != item.lengthRatio) {
		return false;
	}
	return true;
}

std::string Lyric::getConsonantAdjustment() const
{
	const std::vector<int> arr = getConsonantAdjustmentList();
	if (arr.empty()) {
		return "";
	}
	std::vector<int>::const_iterator i;
	std::ostringstream ret;
	for (i = arr.begin(); i != arr.end(); ++i) {
		ret << (i == arr.begin() ? "" : ",") << (*i);
	}
	return ret.str();
}

void Lyric::setConsonantAdjustment(std::string const& value)
{
	std::vector<std::string> spl = StringUtil::explode(",", value);
	std::vector<int> arr;
	std::vector<std::string>::iterator i;
	for (i = spl.begin(); i != spl.end(); ++i) {
		arr.push_back((int)atoi((*i).c_str()));
	}
	setConsonantAdjustmentList(arr);
}

std::vector<int> Lyric::getConsonantAdjustmentList() const
{
	std::vector<int> _consonantAdjustment = this->_consonantAdjustment;
	if (_consonantAdjustment.empty()) {
		if (_phoneticSymbol.empty()) {
			_consonantAdjustment.clear();
		} else {
			_consonantAdjustment.clear();
			std::vector<std::string>::const_iterator i;
			for (i = _phoneticSymbol.begin(); i != _phoneticSymbol.end(); ++i) {
				int consonantAdjustment;
				if (PhoneticSymbol::isConsonant((*i))) {
					consonantAdjustment = 64;
				} else {
					consonantAdjustment = 0;
				}
				_consonantAdjustment.push_back(consonantAdjustment);
			}
		}
	}
	return _consonantAdjustment;
}

void Lyric::setConsonantAdjustmentList(std::vector<int> const& value)
{
	_consonantAdjustment.clear();
	std::vector<int>::const_iterator i;
	for (i = value.begin(); i != value.end(); ++i) {
		_consonantAdjustment.push_back((*i));
	}
}

Lyric Lyric::clone() const
{
	Lyric result("", "");
	result.phrase = phrase;

	result._phoneticSymbol.clear();
	for (int i = 0; i < _phoneticSymbol.size(); i++) {
		result._phoneticSymbol.push_back(_phoneticSymbol[i]);
	}

	result.lengthRatio = lengthRatio;

	result._consonantAdjustment.clear();
	for (int i = 0; i < _consonantAdjustment.size(); i++) {
		result._consonantAdjustment.push_back(_consonantAdjustment[i]);
	}
	result.isProtected = isProtected;
	return result;
}

std::string Lyric::getPhoneticSymbol() const
{
	const std::vector<std::string> symbol = getPhoneticSymbolList();
	if (symbol.empty()) {
		return std::string("");
	}
	std::ostringstream result;
	std::vector<std::string>::const_iterator i;
	for (i = symbol.begin(); i != symbol.end(); ++i) {
		result << (i == symbol.begin() ? "" : " ") << (*i);
	}
	return result.str();
}

void Lyric::setPhoneticSymbol(std::string const& value)
{
	std::string s = StringUtil::replace(value, "  ", " ");
	_phoneticSymbol = StringUtil::explode(" ", s);
	for (int i = 0; i < _phoneticSymbol.size(); i++) {
		_phoneticSymbol[i] = StringUtil::replace(_phoneticSymbol[i], "\\"  "\\", "\\");
	}
}

std::vector<std::string> Lyric::getPhoneticSymbolList() const
{
	std::vector<std::string> ret;
	std::vector<std::string>::const_iterator i;
	for (i = _phoneticSymbol.begin(); i != _phoneticSymbol.end(); ++i) {
		ret.push_back((*i));
	}
	return ret;
}

std::string Lyric::toString(bool addQuateMark) const
{
	std::string quot;
	if (addQuateMark) {
		quot = "\"";
	} else {
		quot = "";
	}
	std::ostringstream result;
	result << quot << phrase << quot << ",";
	std::vector<std::string> symbol = getPhoneticSymbolList();
	std::string strSymbol = getPhoneticSymbol();
	if (false == addQuateMark) {
		if (strSymbol.size() == 0) {
			strSymbol = "u:";
		}
	}
	result << quot << strSymbol << quot << "," << lengthRatio;

	std::string escaped = StringUtil::replace(result.str(), "\\"  "\\", "\\");
	result.str("");
	result.clear(std::ostringstream::goodbit);
	result << escaped;
	std::vector<int> consonantAdjustment = _consonantAdjustment;
	if (consonantAdjustment.empty()) {
		std::vector<std::string>::iterator i;
		for (i = symbol.begin(); i != symbol.end(); ++i) {
			int adjustment;
			if (PhoneticSymbol::isConsonant((*i))) {
				adjustment = 64;
			} else {
				adjustment = 0;
			}
			consonantAdjustment.push_back(adjustment);
		}
	}
	std::vector<int>::iterator i;
	for (i = consonantAdjustment.begin(); i != consonantAdjustment.end(); ++i) {
		result << "," << (*i);
	}
	if (isProtected) {
		result << ",1";
	} else {
		result <<  ",0";
	}
	return result.str();
}

VSQ_END_NAMESPACE
