/**
 * EventListIndexIterator.hpp
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
#ifndef __EventListIndexIterator_hpp__
#define __EventListIndexIterator_hpp__

#include "vsqglobal.hpp"
#include "Event.hpp"
#include "EventListIndexIteratorKind.hpp"

VSQ_BEGIN_NAMESPACE

/**
 * @brief イベントリスト中の、インデックスを順に返す反復子
 */
class EventListIndexIterator
{
private:
	/**
	 * @brief 反復子の元になるリスト
	 */
	const Event::List* _list;

	/**
	 * @brief next メソッドで最後に返したインデックス
	 */
	int _pos;

	/**
	 * @brief 歌手イベントを対象に含めるかどうか
	 */
	bool _kindSinger;

	/**
	 * @brief 音符イベントを対象に含めるかどうか
	 */
	bool _kindNote;

	/**
	 * @brief クレッシェンドイベントを対象に含めるかどうか
	 */
	bool _kindCrescend;

	/**
	 * @brief デクレッシェンドイベントを対象に含めるかどうか
	 */
	bool _kindDecrescend;

	/**
	 * @brief 強弱記号イベントを対象に含めるかどうか
	 */
	bool _kindDynaff;

public:
	/**
	 * @brief 初期化を行う
	 * @param list (EventList) 反復子の元になるリスト
	 * @param iteratorKind (EventList.IndexIteratorKindEnum) 反復子の種類
	 *
	 * @todo iteratorKind の型を EventListIndexIteratorKind にできないか？
	 */
	explicit EventListIndexIterator(const Event::List* list, int iteratorKind)
	{
		this->_list = list;
		this->_pos = -1;
		this->_kindSinger = (iteratorKind & EventListIndexIteratorKind::SINGER) == EventListIndexIteratorKind::SINGER;
		this->_kindNote = (iteratorKind & EventListIndexIteratorKind::NOTE) == EventListIndexIteratorKind::NOTE;
		this->_kindCrescend = (iteratorKind & EventListIndexIteratorKind::CRESCENDO) == EventListIndexIteratorKind::CRESCENDO;
		this->_kindDecrescend = (iteratorKind & EventListIndexIteratorKind::DECRESCENDO) == EventListIndexIteratorKind::DECRESCENDO;
		this->_kindDynaff = (iteratorKind & EventListIndexIteratorKind::DYNAFF) == EventListIndexIteratorKind::DYNAFF;
	}

	/**
	 * @brief 反復子の次の要素を返す
	 * @return (int) 次の要素
	 */
	int next()
	{
		int nextPosition = _nextPosition();
		if (0 <= nextPosition) {
			_pos = nextPosition;
			return nextPosition;
		} else {
			return -1;
		}
	}

	/**
	 * @brief 反復子が次の要素を持つ場合に <code>true</code> を返す
	 * @return (boolean) 反復子がさらに要素を持つ場合は <code>true</code> を、そうでなければ <code>false</code> を返す
	 */
	bool hasNext() const
	{
		return (0 <= this->_nextPosition());
	}

private:
	/**
	 * @brief 反復子の次の要素を探索する
	 * @return (int) 次のインデックス
	 */
	int _nextPosition() const
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
};

VSQ_END_NAMESPACE

#endif
