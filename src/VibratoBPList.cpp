/**
 * @file VibratoBPList.cpp
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
#include "../include/libvsq/VibratoBPList.hpp"
#include "../include/libvsq/StringUtil.hpp"
#include <algorithm>
#include <sstream>

LIBVSQ_BEGIN_NAMESPACE

VibratoBPList::VibratoBPList(std::string const& textNum, std::string const& textBPX, std::string const& textBPY)
{
	int num = 0;
	try {
		num = StringUtil::parseInt<int>(textNum);
	} catch (StringUtil::IntegerParseException const&) {
		num = 0;
	}
	auto bpx = StringUtil::explode(",", textBPX);
	auto bpy = StringUtil::explode(",", textBPY);
	int actNum = std::min({num, (int)bpx.size(), (int)bpy.size()});
	if (actNum > 0) {
		std::vector<double> x(actNum);
		std::vector<int> y(actNum);
		try {
			for (int i = 0; i < actNum; i++) {
				x[i] = StringUtil::parseFloat<double>(bpx[i]);
			}
		} catch (StringUtil::FloatParseException const&) {
			x.clear();
		}
		try {
			for (int i = 0; i < actNum; i++) {
				y[i] = StringUtil::parseInt<int>(bpy[i]);
			}
		} catch (StringUtil::IntegerParseException const&) {
			y.clear();
		}

		if (x.size() == actNum && y.size() == actNum) {
			for (int i = 0; i < actNum; i++) {
				_list.push_back(VibratoBP(x[i], y[i]));
			}
			std::stable_sort(_list.begin(), _list.end(), VibratoBP::compare);
		}
	}
}

VibratoBPList::VibratoBPList(std::vector<double> const& x, std::vector<int> const& y)
{
	int len = std::min(x.size(), y.size());
	for (int i = 0; i < len; i++) {
		_list.push_back(VibratoBP(x[i], y[i]));
	}
	std::stable_sort(_list.begin(), _list.end(), VibratoBP::compare);
}

int VibratoBPList::getValueAt(double x, int defaultValue) const
{
	if (_list.size() <= 0) {
		return defaultValue;
	}
	int index = -1;
	for (int i = 0; i < _list.size(); i++) {
		if (x < _list[i].x) {
			break;
		}
		index = i;
	}
	if (index == -1) {
		return defaultValue;
	} else {
		return _list[index].y;
	}
}

VibratoBPList VibratoBPList::clone() const
{
	VibratoBPList ret;
	for (int i = 0; i < _list.size(); i++) {
		ret._list.push_back(VibratoBP(_list[i].x, _list[i].y));
	}
	return ret;
}

int VibratoBPList::size() const
{
	return (int)_list.size();
}

VibratoBP VibratoBPList::get(int index) const
{
	return _list[index];
}

void VibratoBPList::set(int index, VibratoBP const& value)
{
	_list[index] = value;
}

std::string VibratoBPList::data() const
{
	std::ostringstream ret;
	for (int i = 0; i < _list.size(); i++) {
		if (0 < i) {
			ret << ",";
		}
		ret << _list[i].x << "=" << _list[i].y;
	}
	return ret.str();
}

void VibratoBPList::data(std::string const& value)
{
	_list.clear();
	auto spl = StringUtil::explode(",", value);
	for (auto const& token : spl) {
		auto spl2 = StringUtil::explode("=", token);
		if (spl2.size() >= 2) {
			double x;
			int y;
			try {
				x = StringUtil::parseFloat<double>(spl2[0]);
			} catch (StringUtil::FloatParseException const&) {
				continue;
			}
			try {
				y = StringUtil::parseInt<int>(spl2[1]);
			} catch (StringUtil::IntegerParseException const&) {
				continue;
			}
			_list.emplace_back(x, y);
		}
	}
	std::stable_sort(_list.begin(), _list.end(), VibratoBP::compare);
}

LIBVSQ_END_NAMESPACE
