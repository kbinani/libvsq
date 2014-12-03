/**
 * BPList.cpp
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
#include "./BPList.hpp"
#include "./StringUtil.hpp"
#include "./TextStream.hpp"
#include <sstream>
#include <cmath>

VSQ_BEGIN_NAMESPACE

BPList::KeyClockIterator::KeyClockIterator(BPList* list)
{
	_list = list;
	_pos = -1;
}

bool BPList::KeyClockIterator::hasNext()
{
	if (_list) {
		return (_pos + 1 < _list->size());
	} else {
		return false;
	}
}

tick_t BPList::KeyClockIterator::next()
{
	_pos++;
	return _list->getKeyClock(_pos);
}

void BPList::KeyClockIterator::remove()
{
	if (0 <= _pos && _pos < _list->size()) {
		for (int i = _pos; i < _list->size() - 1; i++) {
			_list->_clocks[i] = _list->_clocks[i + 1];
			_list->_items[i].value = _list->_items[i + 1].value;
			_list->_items[i].id = _list->_items[i + 1].id;
		}
		_list->_length--;
	}
}

BPList::BPList()
{
	_init();
}

BPList::BPList(std::string const& name, int defaultValue, int minimum, int maximum)
{
	_init();
	_name = name;
	_defaultValue = defaultValue;
	_maxValue = maximum;
	_minValue = minimum;
	_maxId = 0;
}

std::string BPList::getName() const
{
	return _name;
}

void BPList::setName(std::string const& value)
{
	_name = value;
}

int BPList::getMaxId() const
{
	return _maxId;
}

int BPList::getDefault() const
{
	return _defaultValue;
}

void BPList::setDefault(int value)
{
	_defaultValue = value;
}

void BPList::renumberIds()
{
	_maxId = 0;
	for (int i = 0; i < _length; i++) {
		_maxId++;
		_items[i].id = _maxId;
	}
}

std::string BPList::getData() const
{
	std::ostringstream ret;
	for (int i = 0; i < _length; i++) {
		if (0 < i) {
			ret << ",";
		}
		ret << _clocks[i] << "=" << _items[i].value;
	}
	return ret.str();
}

void BPList::setData(std::string const& value)
{
	_length = 0;
	_maxId = 0;
	std::vector<std::string> spl = StringUtil::explode(",", value);
	for (int i = 0; i < spl.size(); i++) {
		std::vector<std::string> spl2 = StringUtil::explode("=", spl[i]);
		if (spl2.size() < 2) {
			continue;
		}
		tick_t clock;
		int value;
		try {
			clock = StringUtil::parseInt<tick_t>(spl2[0]);
			value = StringUtil::parseInt<int>(spl2[1]);
		} catch (StringUtil::IntegerParseException&) {
			continue;
		}
		if (value < _minValue) {
			value = _minValue;
		}
		if (_maxValue < value) {
			value = _maxValue;
		}
		_ensureBufferLength(_length + 1);
		_clocks[_length] = clock;
		_items[_length] = BP(value, _maxId + 1);
		_maxId++;
		_length++;
	}
}

BPList BPList::clone() const
{
	BPList res(_name, _defaultValue, _minValue, _maxValue);
	res._ensureBufferLength(_length);
	for (int i = 0; i < _length; i++) {
		res.addWithoutSort(_clocks[i], _items[i].value);
		res._items[i].id = _items[i].id;
	}
	res._length = _length;
	res._maxId = _maxId;
	return res;
}

int BPList::getMaximum() const
{
	return _maxValue;
}

void BPList::setMaximum(int value)
{
	_maxValue = value;
}

int BPList::getMinimum() const
{
	return _minValue;
}

void BPList::setMinimum(int value)
{
	_minValue = value;
}

void BPList::remove(tick_t clock)
{
	_ensureBufferLength(_length);
	int index = _find(clock);
	removeElementAt(index);
}

void BPList::removeElementAt(int index)
{
	if (0 <= index && index < _length) {
		for (int i = index; i < _length - 1; i++) {
			_clocks[i] = _clocks[i + 1];
			_items[i].value = _items[i + 1].value;
			_items[i].id = _items[i + 1].id;
		}
		_length--;
	}
}

bool BPList::isContainsKey(tick_t clock) const
{
	return (_find(clock) >= 0);
}

void BPList::move(tick_t clock, tick_t new_clock, int new_value)
{
	_ensureBufferLength(_length);
	int index = _find(clock);
	if (index < 0) {
		return;
	}
	BP item = _items[index];
	for (int i = index; i < _length - 1; i++) {
		_clocks[i] = _clocks[i + 1];
		_items[i].value = _items[i + 1].value;
		_items[i].id = _items[i + 1].id;
	}
	_length--;
	int index_new = _find(new_clock);
	if (index_new >= 0) {
		_items[index_new].value = new_value;
		_items[index_new].id = item.id;
	} else {
		_length++;
		_ensureBufferLength(_length);
		_clocks[_length - 1] = new_clock;
		std::stable_sort(_clocks.begin(), _clocks.begin() + _length);
		index_new = _find(new_clock);
		for (int i = _length - 1; i >= index_new + 1; i--) {
			_items[i].value = _items[i - 1].value;
			_items[i].id = _items[i - 1].id;
		}
		_items[index_new].value = new_value;
		_items[index_new].id = item.id;
	}
}

void BPList::clear()
{
	_length = 0;
}

int BPList::getValue(int index) const
{
	return _items[index].value;
}

BP BPList::get(int index) const
{
	return _items[index];
}

tick_t BPList::getKeyClock(int index) const
{
	return _clocks[index];
}

int BPList::findValueFromId(int id) const
{
	for (int i = 0; i < _length; i++) {
		BP item = _items[i];
		if (item.id == id) {
			return item.value;
		}
	}
	return _defaultValue;
}

BPListSearchResult BPList::findElement(int id) const
{
	BPListSearchResult context;
	for (int i = 0; i < _length; i++) {
		BP item = _items[i];
		if (item.id == id) {
			context.clock = _clocks[i];
			context.index = i;
			context.point = item;
			return context;
		}
	}
	context.clock = -1;
	context.index = -1;
	context.point = BP(_defaultValue, -1);
	return context;
}

void BPList::setValueForId(int id, int value)
{
	for (int i = 0; i < _length; i++) {
		if (_items[i].id == id) {
			_items[i].value = value;
			break;
		}
	}
}

void BPList::print(TextStream& stream, tick_t startClock, std::string const& header) const
{
	stream.writeLine(header);
	int lastvalue = _defaultValue;
	int value_at_start_written = false;
	for (int i = 0; i < _length; i++) {
		tick_t key = _clocks[i];
		if (startClock == key) {
			stream.write(StringUtil::toString(key, "%d"));
			stream.write("=");
			stream.writeLine(StringUtil::toString(_items[i].value, "%d"));
			value_at_start_written = true;
		} else if (startClock < key) {
			if ((!value_at_start_written) && (lastvalue != _defaultValue)) {
				stream.write(StringUtil::toString(startClock, "%ld"));
				stream.write("=");
				stream.writeLine(StringUtil::toString(lastvalue, "%d"));
				value_at_start_written = true;
			}
			int val = _items[i].value;
			stream.write(StringUtil::toString(key, "%d"));
			stream.write("=");
			stream.writeLine(StringUtil::toString(val, "%d"));
		} else {
			lastvalue = _items[i].value;
		}
	}
	if ((!value_at_start_written) && (lastvalue != _defaultValue)) {
		stream.write(StringUtil::toString(startClock, "%ld"));
		stream.write("=");
		stream.writeLine(StringUtil::toString(lastvalue, "%d"));
	}
}

std::string BPList::appendFromText(TextStream& reader)
{
	tick_t clock = 0;
	int value = 0;
	int minus = 1;
	int mode = 0; // 0: clockを読んでいる, 1: valueを読んでいる
	while (reader.ready()) {
		std::string ch = reader.get();
		if (ch == "\n") {
			if (mode == 1) {
				addWithoutSort(clock, value * minus);
				mode = 0;
				clock = 0;
				value = 0;
				minus = 1;
			}
		} else {
			if (ch == "[") {
				if (mode == 1) {
					addWithoutSort(clock, value * minus);
					mode = 0;
					clock = 0;
					value = 0;
					minus = 1;
				}
				reader.setPointer(reader.getPointer() - 1);
				break;
			}
			if (ch == "=") {
				mode = 1;
			} else if (ch == "-") {
				minus = -1;
			} else {
				int num = -1;
				if (ch == "0") {
					num = 0;
				} else if (ch == "1") {
					num = 1;
				} else if (ch == "2") {
					num = 2;
				} else if (ch == "3") {
					num = 3;
				} else if (ch == "4") {
					num = 4;
				} else if (ch == "5") {
					num = 5;
				} else if (ch == "6") {
					num = 6;
				} else if (ch == "7") {
					num = 7;
				} else if (ch == "8") {
					num = 8;
				} else if (ch == "9") {
					num = 9;
				}
				if (num >= 0) {
					if (mode == 0) {
						clock = clock * 10 + num;
					} else {
						value = value * 10 + num;
					}
				}
			}
		}
	}
	return reader.readLine();
}

int BPList::size() const
{
	return _length;
}

BPList::KeyClockIterator BPList::keyClockIterator()
{
	return KeyClockIterator(this);
}

int BPList::add(tick_t clock, int value)
{
	_ensureBufferLength(_length);
	int index = _find(clock);
	if (index >= 0) {
		_items[index].value = value;
		return _items[index].id;
	} else {
		_length++;
		_ensureBufferLength(_length);
		_clocks[_length - 1] = clock;
		std::stable_sort(_clocks.begin(), _clocks.begin() + _length);
		index = _find(clock);
		_maxId++;
		for (int i = _length - 1; i >= index + 1; i--) {
			_items[i].value = _items[i - 1].value;
			_items[i].id = _items[i - 1].id;
		}
		_items[index].value = value;
		_items[index].id = _maxId;
		return _maxId;
	}
}

int BPList::addWithId(tick_t clock, int value, int id)
{
	_ensureBufferLength(_length);
	int index = _find(clock);
	if (index >= 0) {
		_items[index].value = value;
		_items[index].id = id;
	} else {
		_length++;
		_ensureBufferLength(_length);
		_clocks[_length - 1] = clock;
		std::stable_sort(_clocks.begin(), _clocks.begin() + _length);
		index = _find(clock);
		for (int i = _length - 1; i >= index + 1; i--) {
			_items[i].value = _items[i - 1].value;
			_items[i].id = _items[i - 1].id;
		}
		_items[index].value = value;
		_items[index].id = id;
	}
	_maxId = std::max(_maxId, id);
	return id;
}

void BPList::removeWithId(int id)
{
	for (int i = 0; i < _length; i++) {
		if (_items[i].id == id) {
			for (int j = i; j < _length - 1; j++) {
				_items[j].value = _items[j + 1].value;
				_items[j].id = _items[j + 1].id;
				_clocks[j] = _clocks[j + 1];
			}
			_length--;
			break;
		}
	}
}

int BPList::getValueAt(tick_t clock) const
{
	int index = _find(clock);
	if (index >= 0) {
		return _items[index].value;
	} else {
		if (_length <= 0) {
			return _defaultValue;
		} else {
			int draft = -1;
			for (int i = 0; i < _length; i++) {
				int c = _clocks[i];
				if (clock < c) {
					break;
				}
				draft = i;
			}
			if (draft == -1) {
				return _defaultValue;
			} else {
				return _items[draft].value;
			}
		}
	}
}

int BPList::getValueAt(tick_t clock, int* index) const
{
	if (_length == 0) {
		return _defaultValue;
	} else {
		if (*index < 0 || _length <= *index) {
			*index = 0;
		}
		if (0 <= *index && *index < _length) {
			if (clock < _clocks[*index]) {
				*index = 0;
			}
		}
		for (int i = *index; i < _length; i++) {
			tick_t keyclock = _clocks[i];
			if (clock < keyclock) {
				if (1 <= i) {
					*index = i - 1;
					return _items[i - 1].value;
				} else {
					*index = 0;
					return _defaultValue;
				}
			}
		}
		*index = _length - 1;
		return _items[_length - 1].value;
	}
}

void BPList::_init()
{
	_length = 0;
	_defaultValue = 0;
	_maxValue = 127;
	_minValue = 0;
	_maxId = 0;
	_name = "";
}

void BPList::_ensureBufferLength(int length)
{
	if (length > _clocks.size()) {
		int newLength = length;
		if (_clocks.size() <= 0) {
			newLength = (int)::floor(length * 1.2);
		} else {
			int order = length / _clocks.size();
			if (order <= 1) {
				order = 2;
			}
			newLength = _clocks.size() * order;
		}
		int delta = newLength - _clocks.size();
		for (int i = 0; i < delta; i++) {
			_clocks.push_back(0);
			_items.push_back(BP(0, 0));
		}
	}
}

int BPList::_find(tick_t value) const
{
	for (int i = 0; i < _length; i++) {
		if (_clocks[i] == value) {
			return i;
		}
	}
	return -1;
}

void BPList::addWithoutSort(tick_t clock, int value)
{
	_ensureBufferLength(_length + 1);
	_clocks[_length] = clock;
	_maxId++;
	_items[_length].value = value;
	_items[_length].id = _maxId;
	_length++;
}

VSQ_END_NAMESPACE
