/**
 * BPListKeyClockIterator.h
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
#ifndef __BPListKeyClockIterator_h__
#define __BPListKeyClockIterator_h__

#include "vsqglobal.h"
#include "BPList.h"

VSQ_BEGIN_NAMESPACE

/**
 * @brief コントロールカーブのデータ点の Tick 単位の時刻を順に返す反復子
 */
class BPListKeyClockIterator
{
private:
    /**
     * @brief 反復子の元になるリスト
     */
    BPList *_list;

    /**
     * 反復子の現在の位置
     */
    int _pos;

public:
    /**
     * @brief 初期化を行う
     * @param list (BPList) 反復子の元になるリスト
     */
    explicit BPListKeyClockIterator( BPList *list = NULL )
    {
        _list = list;
        _pos = -1;
    }

    /**
     * @brief 反復子が次の要素を持つ場合に <code>true</code> を返す
     * @return (boolean) 反復子がさらに要素を持つ場合は <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    bool hasNext()
    {
        if( _list ){
            return (_pos + 1 < _list->size());
        }else{
            return false;
        }
    }

    /**
     * @brief 反復子の次の要素を返す
     * @return (int) 次の要素
     */
    VSQ_NS::tick_t next()
    {
        _pos++;
        return _list->getKeyClock( _pos );
    }

    /**
     * @brief 反復子によって最後に返された要素を削除する
     */
    void remove()
    {
        if( 0 <= _pos && _pos < _list->size() ){
            for( int i = _pos; i < _list->size() - 1; i++ ){
                _list->_clocks[i] = _list->_clocks[i + 1];
                _list->_items[i].value = _list->_items[i + 1].value;
                _list->_items[i].id = _list->_items[i + 1].id;
            }
            _list->_length--;
        }
    }
};

VSQ_END_NAMESPACE

#endif
