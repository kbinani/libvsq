/**
 * EventListIterator.h
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
#ifndef __EventListIterator_h__
#define __EventListIterator_h__

#include "vsqglobal.hpp"
#include "EventList.hpp"

VSQ_BEGIN_NAMESPACE

/**
 * @brief イベントリストのアイテムを順に返す反復子
 */
class EventListIterator
{
private:
    /**
     * @brief 反復子の元になるリスト
     */
    VSQ_NS::EventList *_list;

    /**
     * @brief 反復子の現在の位置
     */
    int _pos;

public:
    /**
     * @brief 初期化を行う
     * @param list 反復子の元になるリスト
     */
    EventListIterator( VSQ_NS::EventList *list ) :
        _list( list ),
        _pos( -1 )
    {
    }

    /**
     * @brief 反復子が次の要素を持つ場合に <code>true</code> を返す
     * @return 反復子がさらに要素を持つ場合は <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    bool hasNext()
    {
        if( 0 <= _pos + 1 && _pos + 1 < _list->size() ){
            return true;
        }
        return false;
    }

    /**
     * @brief 反復子の次の要素を返す
     * @return 次の要素
     */
    VSQ_NS::Event *next()
    {
        _pos++;
        return &_list->_events[_pos];
    }

    /**
     * @brief 反復子によって最後に返された要素を削除する
     */
    void remove()
    {
        if( 0 <= _pos && _pos < _list->size() ){
            _list->removeAt( _pos );
            _pos--;
        }
    }
};

VSQ_END_NAMESPACE

#endif
