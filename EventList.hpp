/**
 * EventList.h
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
#ifndef __EventList_h__
#define __EventList_h__

#include <vector>
#include "vsqglobal.hpp"
#include "Event.hpp"

VSQ_BEGIN_NAMESPACE

class EventListIterator;

/**
 * @brief 固有 ID 付きの {@link Event} のリストを取り扱うクラス
 */
class EventList
{
    friend class EventListIterator;

protected:
    /**
     * @brief イベントのリスト
     */
    std::vector<VSQ_NS::Event> _events;

    /**
     * @brief イベントの ID のリスト
     */
    std::vector<int> _ids;

public:
    /**
     * @brief イベント ID を基にイベントを検索し、そのインデックスを返す
     * @param internalId 検索するイベント ID
     * @return 検索結果のインデックス(最初のインデックスは0)。イベントが見つからなければ負の値を返す
     */
    int findIndexFromId( int internalId ) const;

    /**
     * @brief イベント ID を基にイベントを検索し、そのオブジェクトを返す
     * @param internalId 検索するイベント ID
     * @return 検索結果のイベント。イベントが見つからなければ <code>nil</code> を返す
     */
    const VSQ_NS::Event *findFromId( int internalId );

    /**
     * @brief 指定されたイベント ID をもつイベントのオブジェクトを置き換える。イベントが見つからなければ何もしない
     * @param internalId 検索するイベント ID
     * @param value 置換するオブジェクト
     */
    void setForId( int internalId, const VSQ_NS::Event &value );

    /**
     * @brief イベントを並べ替える
     */
    void sort();

    /**
     * @brief 全てのイベントを削除する
     */
    void clear();

    /**
     * @brief リスト内のイベントを順に返す反復子を取得する
     * @return 反復子
     */
    VSQ_NS::EventListIterator iterator();

    /**
     * @brief イベントを追加する
     * @param item 追加するオブジェクト
     * @return 追加したオブジェクトに割り振られたイベント ID
     */
    int add( const VSQ_NS::Event &item );

    /**
     * @brief イベントを追加する
     * @param item 追加するオブジェクト
     * @param internalId 追加するオブジェクトに割り振るイベント ID
     * @return オブジェクトに割り振られたイベント ID
     */
    int add( const VSQ_NS::Event &item, int internalId );

    /**
     * @brief イベントを削除する
     * @param index 削除するイベントのインデックス(最初のインデックスは0)
     */
    void removeAt( int index );

    /**
     * @brief イベントの個数を返す
     * @return データ点の個数
     */
    int size() const;

    /**
     * @brief 指定したインデックスのイベントを取得する
     * @param index インデックス(最初のインデックスは0)
     * @return イベント
     */
    const VSQ_NS::Event get( int index ) const;

    /**
     * @brief 指定したインデックスのイベントを設定する
     * @param index インデックス(最初のインデックスは0)
     * @param value 設定するイベント
     */
    void set( int index, const VSQ_NS::Event &value );

    /**
     * @brief リスト内部のイベント ID のデータを更新する
     */
    void updateIdList();

    /**
     * @brief イベントリストをテキストストリームに出力する
     * @param stream 出力先のストリーム
     * @param eos EOS として出力する Tick 単位の時刻
     * @return リスト中のイベントに含まれるハンドルの一覧
     */
    std::vector<VSQ_NS::Handle> write( TextStream &stream, VSQ_NS::tick_t eos );

private:
    /**
     * @brief イベントを追加する
     * @param item 追加するオブジェクト
     * @param internal_id 追加するオブジェクトに割り振るイベント ID
     */
    void _addCor( const VSQ_NS::Event &item, int internalId );

    /**
     * @brief イベントに割り振る ID を取得する
     * @param next
     * @return
     */
    int _getNextId( int next );

    /**
     * @brief リスト内のイベントから、ハンドルの一覧を作成する。同時に、各イベント、ハンドルの番号を設定する
     * @return (table<Handle>) ハンドルの一覧
     */
    const std::vector<Handle> _buildHandleList();

};

VSQ_END_NAMESPACE

#endif
