/**
 * VibratoBPList.cpp
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
#include <sstream>

VSQ_BEGIN_NAMESPACE

VibratoBPList::VibratoBPList(std::string const& textNum, std::string const& textBPX, std::string const& textBPY)
{
	int num = 0;
	try {
		num = StringUtil::parseInt<int>(textNum);
	} catch (StringUtil::IntegerParseException&) {
		num = 0;
	}
	std::vector<std::string> bpx = StringUtil::explode(",", textBPX);
	std::vector<std::string> bpy = StringUtil::explode(",", textBPY);
	int actNum = std::min(num, std::min((int)bpx.size(), (int)bpy.size()));
	if (actNum > 0) {
		double* x = new double[actNum]();
		int* y = new int[actNum]();
		try {
			for (int i = 0; i < actNum; i++) {
				x[i] = StringUtil::parseFloat<double>(bpx[i]);
			}
		} catch (StringUtil::FloatParseException&) {
			delete [] x;
			x = 0;
		}
		try {
			for (int i = 0; i < actNum; i++) {
				y[i] = StringUtil::parseInt<int>(bpy[i]);
			}
		} catch (StringUtil::IntegerParseException&) {
			delete [] y;
			y = 0;
		}

		if (x && y) {
			for (int i = 0; i < actNum; i++) {
				_list.push_back(VibratoBP(x[i], y[i]));
			}
			std::stable_sort(_list.begin(), _list.end(), VibratoBP::compare);
		}
		if (x) { delete [] x; }
		if (y) { delete [] y; }
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
	std::vector<std::string> spl = StringUtil::explode(",", value);
	std::vector<std::string>::iterator i;
	for (i = spl.begin(); i != spl.end(); ++i) {
		std::vector<std::string> spl2 = StringUtil::explode("=", (*i));
		if (spl2.size() >= 2) {
			double x;
			int y;
			try {
				x = StringUtil::parseFloat<double>(spl2[0]);
			} catch (StringUtil::FloatParseException&) {
				continue;
			}
			try {
				y = StringUtil::parseInt<int>(spl2[1]);
			} catch (StringUtil::IntegerParseException&) {
				continue;
			}
			_list.push_back(VibratoBP(x, y));
		}
	}
	std::stable_sort(_list.begin(), _list.end(), VibratoBP::compare);
}

VSQ_END_NAMESPACE
