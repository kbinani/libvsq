/**
 * EventList.cpp
 * Copyright © 2012 kbinani
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
#include <sstream>
#include <boost/format.hpp>
#include <algorithm>
#include "EventList.hpp"
#include "EventListIterator.hpp"
#include "VoiceLanguageEnum.hpp"

using namespace std;
using namespace VSQ_NS;

int EventList::findIndexFromId( int internalId ) const
{
    int c = _events.size();
    for( int i = 0; i < c; i++ ){
        Event item = _events[i];
        if( item.id == internalId ){
            return i;
        }
    }
    return -1;
}

const Event *EventList::findFromId( int internalId )
{
    int index = findIndexFromId( internalId );
    if( 0 <= index && index < _events.size() ){
        return &_events[index];
    }else{
        return NULL;
    }
}

void EventList::setForId( int internalId, const Event &value )
{
    int c = _events.size();
    for( int i = 0; i < c; i++ ){
        if( _events[i].id == internalId ){
            _events[i] = value;
            _events[i].id = internalId;
            break;
        }
    }
}

void EventList::sort()
{
    std::sort( _events.begin(), _events.end(), Event::compare );
    updateIdList();
}

void EventList::clear()
{
    _events.clear();
    _ids.clear();
}

EventListIterator EventList::iterator()
{
    updateIdList();
    return EventListIterator( this );
}

int EventList::add(const Event &item)
{
    int id = _getNextId( 0 );
    _addCor( item, id );
    std::sort( _events.begin(), _events.end(), Event::compare );
    int count = _events.size();
    for( int i = 0; i < count; i++ ){
        _ids[i] = _events[i].id;
    }
    return id;
}

int EventList::add( const Event &item, int internalId )
{
    _addCor( item, internalId );
    std::sort( _events.begin(), _events.end(), Event::compare );
    return internalId;
}

void EventList::_addCor( const Event &item, int internalId )
{
    updateIdList();
    Event add = item;
    add.id = internalId;
    _events.push_back( add );
    _ids.push_back( internalId );
}

void EventList::removeAt( int index )
{
    updateIdList();
    _events.erase( _events.begin() + index );
    _ids.erase( _ids.begin() + index );
}

int EventList::_getNextId( int next )
{
    updateIdList();
    int max = -1;
    for( int i = 0; i < _ids.size(); i++ ){
        max = ::max( max, _ids[i] );
    }
    return max + 1 + next;
}

int EventList::size() const
{
    return _events.size();
}

const Event EventList::get( int index ) const
{
    return _events[index];
}

void EventList::set( int index, const Event &value )
{
    int id = _events[index].id;
    _events[index] = value;
    _events[index].id = id;
}

void EventList::updateIdList()
{
    if( _ids.size() != _events.size() ){
        _ids.clear();
    }
    int count = _events.size();
    for( int i = 0; i < count; i++ ){
        _ids.push_back( _events[i].id );
    }
}

vector<Handle> EventList::write( TextStream &stream, tick_t eos )
{
    vector<Handle> handles = _buildHandleList();
    stream.writeLine( "[EventList]" );
    vector<Event> temp;
    EventListIterator itr = iterator();
    while( itr.hasNext() ){
        temp.push_back( *itr.next() );
    }
    std::sort( temp.begin(), temp.end(), Event::compare );
    int i = 0;
    while( i < temp.size() ){
        Event item = temp[i];
        if( !item.isEOS() ){
            ostringstream ids;
            ids << "ID#" << (boost::format( "%04d" ) % item.index).str();
            tick_t clock = temp[i].clock;
            while( i + 1 < temp.size() && clock == temp[i + 1].clock ){
                i++;
                ids << ",ID#" << (boost::format( "%04d" ) % temp[i].index).str();
            }
            ostringstream oss;
            oss << clock << "=" << ids.str();
            stream.writeLine( oss.str() );
        }
        i++;
    }
    stream.write( (boost::format( "%d" ) % eos).str() ).writeLine( "=EOS" );
    return handles;
}

const vector<Handle> EventList::_buildHandleList()
{
    vector<Handle> handle;
    int current_id = -1;
    int current_handle = -1;
    bool add_quotation_mark = true;
    EventListIterator itr = iterator();
    while( itr.hasNext() ){
        Event *item = itr.next();
        current_id = current_id + 1;
        item->index = current_id;
        // SingerHandle
        if( item->singerHandle.getHandleType() == HandleType::SINGER ){
            current_handle = current_handle + 1;
            item->singerHandle.index = current_handle;
            handle.push_back( item->singerHandle );
            item->_singerHandleIndex = current_handle;
            VoiceLanguage::VoiceLanguageEnum lang = VoiceLanguage::valueFromSingerName( item->singerHandle.ids );
            add_quotation_mark = lang == VoiceLanguage::JAPANESE;
        }
        // LyricHandle
        if( item->lyricHandle.getHandleType() == HandleType::LYRIC ){
            current_handle = current_handle + 1;
            item->lyricHandle.index = current_handle;
            item->lyricHandle.addQuotationMark = add_quotation_mark;
            handle.push_back( item->lyricHandle );
            item->_lyricHandleIndex = current_handle;
        }
        // VibratoHandle
        if( item->vibratoHandle.getHandleType() == HandleType::VIBRATO ){
            current_handle = current_handle + 1;
            item->vibratoHandle.index = current_handle;
            handle.push_back( item->vibratoHandle );
            item->_vibratoHandleIndex = current_handle;
        }
        // NoteHeadHandle
        if( item->noteHeadHandle.getHandleType() == HandleType::NOTE_HEAD ){
            current_handle = current_handle + 1;
            item->noteHeadHandle.index = current_handle;
            handle.push_back( item->noteHeadHandle );
            item->_noteHeadHandleIndex = current_handle;
        }
        // IconDynamicsHandle
        if( item->iconDynamicsHandle.getHandleType() == HandleType::DYNAMICS ){
            current_handle = current_handle + 1;
            item->iconDynamicsHandle.index = current_handle;
            item->iconDynamicsHandle.setLength( item->getLength() );
            handle.push_back( item->iconDynamicsHandle );
            // IconDynamicsHandleは、歌手ハンドルと同じ扱いなので
            // _singerHandleIndexでよい
            item->_singerHandleIndex = current_handle;
        }
    }
    return handle;
}
