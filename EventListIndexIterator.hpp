/**
 * EventListIndexIterator.hpp
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
#pragma once

#include "./Namespace.hpp"
#include "./Event.hpp"
#include "./EventListIndexIteratorKind.hpp"

VSQ_BEGIN_NAMESPACE

/**
 * @brief イベントリスト中の, インデックスを順に返す反復子.
 */
class EventListIndexIterator
{
private:
	/**
	 * @brief 反復子の元になるリスト.
	 */
	Event::List const* _list;

	/**
	 * @brief next メソッドで最後に返したインデックス.
	 */
	int _pos;

	/**
	 * @brief 歌手イベントを対象に含めるかどうか.
	 */
	bool _kindSinger;

	/**
	 * @brief 音符イベントを対象に含めるかどうか.
	 */
	bool _kindNote;

	/**
	 * @brief クレッシェンドイベントを対象に含めるかどうか.
	 */
	bool _kindCrescend;

	/**
	 * @brief デクレッシェンドイベントを対象に含めるかどうか.
	 */
	bool _kindDecrescend;

	/**
	 * @brief 強弱記号イベントを対象に含めるかどうか.
	 */
	bool _kindDynaff;

public:
	/**
	 * @brief 初期化を行う.
	 * @param list 反復子の元になるリスト.
	 * @param iteratorKind 反復子の種類.
	 *
	 * @todo iteratorKind の型を EventListIndexIteratorKind にできないか?
	 */
	EventListIndexIterator(Event::List const* list, EventListIndexIteratorKind iteratorKind);

	/**
	 * @brief 反復子の次の要素を返す.
	 * @return 次の要素.
	 */
	int next();

	/**
	 * @brief 反復子が次の要素を持つ場合に <code>true</code> を返す.
	 * @return 反復子がさらに要素を持つ場合は <code>true</code> を, そうでなければ <code>false</code> を返す.
	 */
	bool hasNext() const;

private:
	/**
	 * @brief 反復子の次の要素を探索する.
	 * @return 次のインデックス.
	 */
	int _nextPosition() const;
};

VSQ_END_NAMESPACE
