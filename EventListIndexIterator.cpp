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
#include "./EventListIndexIterator.hpp"
#include "./EventListIndexIteratorKind.hpp"

VSQ_BEGIN_NAMESPACE

EventListIndexIterator::EventListIndexIterator(const Event::List* list, int iteratorKind)
{
	this->_list = list;
	this->_pos = -1;
	this->_kindSinger = (iteratorKind & EventListIndexIteratorKind::SINGER) == EventListIndexIteratorKind::SINGER;
	this->_kindNote = (iteratorKind & EventListIndexIteratorKind::NOTE) == EventListIndexIteratorKind::NOTE;
	this->_kindCrescend = (iteratorKind & EventListIndexIteratorKind::CRESCENDO) == EventListIndexIteratorKind::CRESCENDO;
	this->_kindDecrescend = (iteratorKind & EventListIndexIteratorKind::DECRESCENDO) == EventListIndexIteratorKind::DECRESCENDO;
	this->_kindDynaff = (iteratorKind & EventListIndexIteratorKind::DYNAFF) == EventListIndexIteratorKind::DYNAFF;
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
			if (item->type == EventType::SINGER) {
				return i;
			}
		}
		if (_kindNote) {
			if (item->type == EventType::NOTE) {
				return i;
			}
		}
		if (_kindDynaff || _kindCrescend || _kindDecrescend) {
			if (item->type == EventType::ICON
				&& item->iconDynamicsHandle.getHandleType() != HandleType::UNKNOWN) {
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
