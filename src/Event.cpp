/**
 * @file Event.cpp
 * Copyright © 2012,2014 kbinani
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
#include "../include/libvsq/Event.hpp"
#include "../include/libvsq/StringUtil.hpp"
#include <algorithm>

LIBVSQ_BEGIN_NAMESPACE

Event::List::List()
{}

Event::List::List(List const& list)
{
	copy(list);
}

Event::List& Event::List::operator = (List const& list)
{
	copy(list);
	return *this;
}

Event::List::~List()
{
	clear();
}

int Event::List::findIndexFromId(int internalId) const
{
	int c = _events.size();
	for (int i = 0; i < c; i++) {
		auto const& item = _events[i];
		if (item->id == internalId) {
			return i;
		}
	}
	return -1;
}

Event const* Event::List::findFromId(int internalId) const
{
	int index = findIndexFromId(internalId);
	if (0 <= index && index < _events.size()) {
		return _events[index].get();
	} else {
		return 0;
	}
}

void Event::List::setForId(int internalId, Event const& value)
{
	for (auto const& item : _events) {
		if (item->id == internalId) {
			*item = value;
			item->id = internalId;
			break;
		}
	}
}

void Event::List::sort()
{
	std::stable_sort(_events.begin(), _events.end(), [](std::unique_ptr<Event> const & a, std::unique_ptr<Event> const & b) {
		return Event::comp(a.get(), b.get());
	});
	updateIdList();
}

void Event::List::clear()
{
	_events.clear();
	_ids.clear();
}

Event::ListIterator
Event::List::iterator()
{
	updateIdList();
	return ListIterator(this);
}

Event::ListConstIterator Event::List::iterator() const
{
	return ListConstIterator(this);
}

int Event::List::add(Event const& item)
{
	int id = _getNextId(0);
	_addCor(item, id);
	std::stable_sort(_events.begin(), _events.end(), [](std::unique_ptr<Event> const & a, std::unique_ptr<Event> const & b) {
		return Event::comp(a.get(), b.get());
	});
	int count = _events.size();
	for (int i = 0; i < count; i++) {
		_ids[i] = _events[i]->id;
	}
	return id;
}

int Event::List::add(Event const& item, int internalId)
{
	_addCor(item, internalId);
	std::stable_sort(_events.begin(), _events.end(), [](std::unique_ptr<Event> const & a, std::unique_ptr<Event> const & b) {
		return Event::comp(a.get(), b.get());
	});
	return internalId;
}

void Event::List::removeAt(int index)
{
	updateIdList();

	_events.erase(_events.begin() + index);
	_ids.erase(_ids.begin() + index);
}

int Event::List::size() const
{
	return _events.size();
}

Event const* Event::List::get(int index) const
{
	return _events[index].get();
}

void Event::List::set(int index, Event const& value)
{
	int id = _events[index]->id;
	*_events[index] = value;
	_events[index]->id = id;
}

void Event::List::updateIdList()
{
	if (_ids.size() != _events.size()) {
		_ids.clear();
	}
	for (auto const& item : _events) {
		_ids.push_back(item->id);
	}
}

void Event::List::_addCor(Event const& item, int internalId)
{
	updateIdList();
	auto add = std::unique_ptr<Event>(new Event);
	*add = item;
	add->id = internalId;

	_events.push_back(std::move(add));
	_ids.push_back(internalId);
}

int Event::List::_getNextId(int next)
{
	updateIdList();
	int max = -1;
	auto max_element = std::max_element(_ids.begin(), _ids.end());
	if (max_element != _ids.end()) {
		max = *max_element;
	}
	return max + 1 + next;
}

void Event::List::copy(List const& list)
{
	_events.clear();
	_ids.clear();
	for (auto const& item : list._events) {
		_addCor(*item, item->id);
	}
}

Event::ListConstIterator::ListConstIterator(List const* list) :
	_list(list),
	_pos(-1)
{}

bool Event::ListConstIterator::hasNext()
{
	if (0 <= _pos + 1 && _pos + 1 < _list->size()) {
		return true;
	}
	return false;
}

Event* Event::ListConstIterator::next()
{
	++_pos;
	return _list->_events[_pos].get();
}

Event::ListIterator::ListIterator(List* list)
	: ListConstIterator(list)
{
	_nonConstList = list;
}

void Event::ListIterator::remove()
{
	if (0 <= _pos && _pos < _nonConstList->size()) {
		_nonConstList->removeAt(_pos);
		--_pos;
	}
}

Event::Event(std::string const& line)
	: vibratoHandle()
{
	init();
	std::vector<std::string> spl = StringUtil::explode("=", line);
	tick = StringUtil::parseInt<tick_t>(spl[0]);
	isEos = (spl[1] == "EOS");
}

Event::Event()
{
	init();
	tick = 0;
	isEos = true;
	id = 0;
}

Event::Event(tick_t tick, EventType eventType)
{
	init();
	this->tick = tick;
	this->_type = eventType;
	if (eventType == EventType::SINGER) {
		singerHandle = Handle(HandleType::SINGER);
	} else if (eventType == EventType::NOTE) {
		lyricHandle = Handle(HandleType::LYRIC);
		lyricHandle.set(0, Lyric("a", "a"));
	}
	id = 0;
}

tick_t Event::length() const
{
	return _length;
}

void Event::length(tick_t value)
{
	_length = value;
}

EventType Event::type() const
{
	return _type;
}

void Event::type(EventType type)
{
	_type = type;
}

Event Event::clone() const
{
	Event result(tick, type());

	result._type = type();
	result.singerHandle = singerHandle.clone();
	result.length(length());
	result.note = note;
	result.dynamics = dynamics;
	result.pmBendDepth = pmBendDepth;
	result.pmBendLength = pmBendLength;
	result.pmbPortamentoUse = pmbPortamentoUse;
	result.demDecGainRate = demDecGainRate;
	result.demAccent = demAccent;
	result.d4mean = d4mean;
	result.pMeanOnsetFirstNote = pMeanOnsetFirstNote;
	result.vMeanNoteTransition = vMeanNoteTransition;
	result.pMeanEndingNote = pMeanEndingNote;
	result.lyricHandle = lyricHandle.clone();
	result.vibratoHandle = vibratoHandle.clone();
	result.vibratoDelay = vibratoDelay;
	result.noteHeadHandle = noteHeadHandle.clone();
	result.iconDynamicsHandle = iconDynamicsHandle.clone();
	result.isEos = isEos;

	result.id = id;
	result.tag = tag;
	return result;
}

bool Event::isEOS() const
{
	return isEos;
}

int Event::compareTo(Event const& item) const
{
	tick_t ret = tick - item.tick;
	if (ret == 0) {
		return static_cast<int>(type()) - static_cast<int>(item.type());
	} else {
		return (int)ret;
	}
}

bool Event::compare(Event const& a, Event const& b)
{
	return (a.compareTo(b) < 0);
}

bool Event::comp(Event const* a, Event const* b)
{
	return (a->compareTo(*b) < 0);
}

Event Event::eos()
{
	return Event();
}

/**
	-- @param item [VsqEvent]
	-- @return [bool]
	-- @todo 実装できたら, TrackTest::testGetIndexIteratorNote, testGetIndexIteratorDynamics
			 の中のequals使うassertionを復活させること
	function this:equals( item )
		if( self.tick ~= item.tick )then
			return false;
		end
		if( self.type ~= item.type )then
			return false;
		end
		if( self.type == idType.Anote )then
			if( self.note ~= item.note )then
				return false;
			end
			if( self:getLength() ~= item:getLength() )then
				return false;
			end
			if( self.d4mean ~= item.d4mean )then
				return false;
			end
			if( self.demAccent ~= item.demAccent )then
				return false;
			end
			if( self.demDecGainRate ~= item.demDecGainRate )then
				return false;
			end
			if( self.dynamics ~= item.dynamics )then
				return false;
			end
			if( self.lyricHandle ~= nil and item.lyricHandle ~= nil )then
				return false;
			end
			if( self.lyricHandle ~= nil and item.lyricHandle == nil )then
				return false;
			end
			if( self.lyricHandle ~= nil and item.lyricHandle ~= nil )then
				if( self.lyricHandle:size() ~= item.lyricHandle:size() )then
					return false;
				end
				local count = self.lyricHandle:size();
				local k;
				for k = 0, count - 1, 1 do
					if( not self.lyricHandle:getLyricAt( k ):equalsForSynth( item.lyricHandle:getLyricAt( k ) ) )then
						return false;
					end
				end
			end
			if( self.noteHeadHandle == nil and item.noteHeadHandle ~= nil )then
				return false;
			end
			if( self.noteHeadHandle ~= nil and item.noteHeadHandle == nil )then
				return false;
			end
			if( self.noteHeadHandle ~= nil and item.noteHeadHandle ~= nil )then
				if( self.NoteHeadHandle.iconId ~= item.noteHeadHandle.iconId )then
					return false;
				end
				if( self.noteHeadHandle:getDepth() ~= item.noteHeadHandle:getDepth() )then
					return false;
				end
				if( self.noteHeadHandle:getDuration() ~= item.noteHeadHandle:getDuration() )then
					return false;
				end
				if( self.noteHeadHandle:getLength() ~= item.noteHeadHandle:getLength() )then
					return false;
				end
			end
			if( self.pmBendDepth ~= item.pmBendDepth )then
				return false;
			end
			if( self.pmBendLength ~= item.pmBendLength )then
				return false;
			end
			if( self.pmbPortamentoUse ~= item.pmbPortamentoUse )then
				return false;
			end
			if( self.pMeanEndingNote ~= item.pMeanEndingNote )then
				return false;
			end
			if( self.pMeanOnsetFirstNote ~= item.pMeanOnsetFirstNote )then
				return false;
			end
			local hVibratoThis = self.vibratoHandle;
			local hVibratoItem = item.vibratoHandle;
			if( hVibratoThis == nil and hVibratoItem ~= nil )then
				return false;
			end
			if( hVibratoThis ~= nil and hVibratoItem == nil )then
				return false;
			end
			if( hVibratoThis ~= nil and hVibratoItem ~= nil )then
				if( self.vibratoDelay ~= item.vibratoDelay )then
					return false;
				end
				if( hVibratoThis.iconId ~= hVibratoItem.iconId )then
					return false;
				end
				if( hVibratoThis:getStartDepth() ~= hVibratoItem:getStartDepth() )then
					return false;
				end
				if( hVibratoThis:getStartRate() ~= hVibratoItem:getStartRate() )then
					return false;
				end
				local vibRateThis = hVibratoThis:getRateBP();
				local vibRateItem = hVibratoItem:getRateBP();
				if( vibRateThis == nil and vibRateItem ~= nil )then
					return false;
				end
				if( vibRateThis ~= nil and vibRateItem == nil )then
					return false;
				end
				if( vibRateThis ~= nil and vibRateItem ~= nil )then
					local numRateCount = vibRateThis:size();
					if( numRateCount ~= vibRateItem:size() )then
						return false;
					end
					local k;
					for k = 0, numRateCount - 1, 1 do
						local pThis = vibRateThis:getElement( k );
						local pItem = vibRateItem:getElement( k );
						if( pThis.x ~= pItem.x )then
							return false;
						end
						if( pThis.y ~= pItem.y )then
							return false;
						end
					end
				end
				local vibDepthThis = hVibratoThis:getDepthBP();
				local vibDepthItem = hVibratoItem:getDepthBP();
				if( vibDepthThis == nil and vibDepthItem ~= nil )then
					return false;
				end
				if( vibDepthThis ~= nil and vibDepthItem == nil )then
					return false;
				end
				if( vibDepthThis ~= nil and vibDepthItem ~= nil )then
					local numDepthCount = vibDepthThis:size();
					if( numDepthCount ~= vibDepthItem:size() )then
						return false;
					end
					local k;
					for k = 0, numDepthCount - 1, 1 do
						local pThis = vibDepthThis:getElement( k );
						local pItem = vibDepthItem:getElement( k );
						if( pThis.x ~= pItem.x )then
							return false;
						end
						if( pThis.y ~= pItem.y )then
							return false;
						end
					end
				end
			end
			if( self.vMeanNoteTransition ~= item.vMeanNoteTransition )then
				return false;
			end
		elseif( self.type == EventTypeEnum.SINGER )then
			-- シンガーイベントの比較
			if( self.singerHandle.program ~= item.singerHandle.program )then
				return false;
			end
		elseif( self.type == EventTypeEnum.ICON )then
			if( self.iconDynamicsHandle.iconId ~= item.iconDynamicsHandle.iconId )then
				return false;
			end
			if( self.iconDynamicsHandle:isDynaffType() )then
				-- 強弱記号
			else
				-- クレッシェンド・デクレッシェンド
				if( self:getLength() ~= item:getLength() )then
					return false;
				end
			end
		end

		return true;
	end*/

void Event::init()
{
	tag = "";
	id = -1;
	tick = 0;
	isEos = false;
	_type = EventType::NOTE;
	_length = 0;
	note = 0;
	dynamics = 0;
	pmBendDepth = 0;
	pmBendLength = 0;
	pmbPortamentoUse = 0;
	demDecGainRate = 0;
	demAccent = 0;
	vibratoDelay = 0;
	pMeanOnsetFirstNote = 10;
	vMeanNoteTransition = 12;
	d4mean = 24;
	pMeanEndingNote = 12;
}

LIBVSQ_END_NAMESPACE
