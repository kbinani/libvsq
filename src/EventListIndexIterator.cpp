/**
 * EventListIndexIterator.cpp
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
#include "../include/libvsq/EventListIndexIterator.hpp"
#include "../include/libvsq/EventListIndexIteratorKind.hpp"

VSQ_BEGIN_NAMESPACE

EventListIndexIterator::EventListIndexIterator(Event::List const* list, EventListIndexIteratorKind iteratorKind)
{
	this->_list = list;
	this->_pos = -1;
	this->_kindSinger = (static_cast<int>(iteratorKind) & static_cast<int>(EventListIndexIteratorKind::SINGER)) == static_cast<int>(EventListIndexIteratorKind::SINGER);
	this->_kindNote = (static_cast<int>(iteratorKind) & static_cast<int>(EventListIndexIteratorKind::NOTE)) == static_cast<int>(EventListIndexIteratorKind::NOTE);
	this->_kindCrescend = (static_cast<int>(iteratorKind) & static_cast<int>(EventListIndexIteratorKind::CRESCENDO)) == static_cast<int>(EventListIndexIteratorKind::CRESCENDO);
	this->_kindDecrescend = (static_cast<int>(iteratorKind) & static_cast<int>(EventListIndexIteratorKind::DECRESCENDO)) == static_cast<int>(EventListIndexIteratorKind::DECRESCENDO);
	this->_kindDynaff = (static_cast<int>(iteratorKind) & static_cast<int>(EventListIndexIteratorKind::DYNAFF)) == static_cast<int>(EventListIndexIteratorKind::DYNAFF);
}

int EventListIndexIterator::next()
{
	int nextPosition = _nextPosition();
	if (0 <= nextPosition) {
		_pos = nextPosition;
		return nextPosition;
	} else {
		return -1;
	}
}

bool EventListIndexIterator::hasNext() const
{
	return (0 <= this->_nextPosition());
}

int EventListIndexIterator::_nextPosition() const
{
	int count = _list->size();
	for (int i = _pos + 1; i < count; ++i) {
		const Event* item = _list->get(i);
		if (_kindSinger) {
			if (item->type() == EventType::SINGER) {
				return i;
			}
		}
		if (_kindNote) {
			if (item->type() == EventType::NOTE) {
				return i;
			}
		}
		if (_kindDynaff || _kindCrescend || _kindDecrescend) {
			if (item->type() == EventType::ICON
				&& item->iconDynamicsHandle.type() != HandleType::UNKNOWN) {
				if (_kindDynaff) {
					if (item->iconDynamicsHandle.isDynaffType()) {
						return i;
					}
				}
				if (_kindCrescend) {
					if (item->iconDynamicsHandle.isCrescendType()) {
						return i;
					}
				}
				if (_kindDecrescend) {
					if (item->iconDynamicsHandle.isDecrescendType()) {
						return i;
					}
				}
			}
		}
	}
	return -1;
}

VSQ_END_NAMESPACE
