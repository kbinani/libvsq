/**
 * @file Event.hpp
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

#include "./BasicTypes.hpp"
#include "./EventType.hpp"
#include "./Handle.hpp"
#include <vector>
#include <string>
#include <memory>

LIBVSQ_BEGIN_NAMESPACE

/**
 * @brief VSQ ファイルのメタテキスト内に記述されるイベントを表すクラス.
 */
class Event
{
public:
	class ListIterator;
	class ListConstIterator;

	/**
	 * @brief 固有 ID 付きの {@link Event} のリストを取り扱うクラス.
	 */
	class List
	{
		friend class ListIterator;
		friend class ListConstIterator;

	protected:
		/**
		 * @brief イベントのリスト.
		 */
		std::vector<std::unique_ptr<Event>> _events;

		/**
		 * @brief イベントの ID のリスト.
		 */
		std::vector<int> _ids;

	public:
		List();

		/**
		 * @brief コピーコンストラクタ. @a list のコピーを作成する.
		 * @param list コピー元のリスト.
		 */
		List(List const& list);

		/**
		 * @brief コピー演算子. @a list のコピーを作成する.
		 * @param list コピー元のリスト.
		 * @return @a list のコピー.
		 */
		List& operator = (List const& list);

		~List();

		/**
		 * @brief イベント ID を基にイベントを検索し, そのインデックスを返す.
		 * @param internalId 検索するイベント ID.
		 * @return 検索結果のインデックス(最初のインデックスは0). イベントが見つからなければ負の値を返す.
		 */
		int findIndexFromId(int internalId) const;

		/**
		 * @brief イベント ID を基にイベントを検索し, そのオブジェクトを返す.
		 * @param internalId 検索するイベント ID.
		 * @return 検索結果のイベント. イベントが見つからなければ <code>nil</code> を返す.
		 */
		Event const* findFromId(int internalId) const;

		/**
		 * @brief 指定されたイベント ID をもつイベントのオブジェクトを置き換える. イベントが見つからなければ何もしない.
		 * @param internalId 検索するイベント ID.
		 * @param value 置換するオブジェクト.
		 */
		void setForId(int internalId, Event const& value);

		/**
		 * @brief イベントを並べ替える.
		 */
		void sort();

		/**
		 * @brief 全てのイベントを削除する.
		 */
		void clear();

		/**
		 * @brief リスト内のイベントを順に返す反復子を取得する.
		 * @return 反復子.
		 */
		ListIterator iterator();

		/**
		 * @copydoc List::iterator
		 */
		ListConstIterator iterator() const;

		/**
		 * @brief イベントを追加する.
		 * @param item 追加するオブジェクト.
		 * @return 追加したオブジェクトに割り振られたイベント ID.
		 */
		int add(Event const& item);

		/**
		 * @brief イベントを追加する.
		 * @param item 追加するオブジェクト.
		 * @param internalId 追加するオブジェクトに割り振るイベント ID.
		 * @return オブジェクトに割り振られたイベント ID.
		 */
		int add(Event const& item, int internalId);

		/**
		 * @brief イベントを削除する.
		 * @param index 削除するイベントのインデックス(最初のインデックスは0).
		 */
		void removeAt(int index);

		/**
		 * @brief イベントの個数を返す.
		 * @return データ点の個数.
		 */
		int size() const;

		/**
		 * @brief 指定したインデックスのイベントを取得する.
		 * @param index インデックス(最初のインデックスは0).
		 * @return イベント.
		 */
		Event const* get(int index) const;

		/**
		 * @brief 指定したインデックスのイベントを設定する.
		 * @param index インデックス(最初のインデックスは0).
		 * @param value 設定するイベント.
		 */
		void set(int index, Event const& value);

		/**
		 * @brief リスト内部のイベント ID のデータを更新する.
		 */
		void updateIdList();

	private:
		/**
		 * @brief イベントを追加する.
		 * @param item 追加するオブジェクト.
		 * @param internal_id 追加するオブジェクトに割り振るイベント ID.
		 */
		void _addCor(Event const& item, int internalId);

		/**
		 * @brief イベントに割り振る ID を取得する.
		 * @param next
		 * @return
		 */
		int _getNextId(int next);

		/**
		 * @brief Deep copy member fields.
		 * @param list Copy source.
		 */
		void copy(List const& list);
	};

	/**
	 * @brief イベントリストのアイテムを順に返す反復子.
	 */
	class ListConstIterator
	{
	protected:
		/**
		 * @brief 反復子の元になるリスト.
		 */
		List const* _list;

		/**
		 * @brief 反復子の現在の位置.
		 */
		int _pos;

	public:
		/**
		 * @brief 初期化を行う.
		 * @param list 反復子の元になるリスト.
		 */
		ListConstIterator(List const* list);

		virtual ~ListConstIterator()
		{}

		/**
		 * @brief 反復子が次の要素を持つ場合に <code>true</code> を返す.
		 * @return 反復子がさらに要素を持つ場合は <code>true</code> を, そうでなければ <code>false</code> を返す.
		 */
		bool hasNext();

		/**
		 * @brief 反復子の次の要素を返す.
		 * @return 次の要素.
		 */
		Event* next();
	};

	/**
	 * @brief イベントリストのアイテムを順に返す反復子.
	 */
	class ListIterator : public ListConstIterator
	{
	private:
		List* _nonConstList;

	public:
		/**
		 * @copydoc ListConstIterator::ListConstIterator
		 */
		explicit ListIterator(List* list);

		/**
		 * @brief 反復子によって最後に返された要素を削除する.
		 */
		void remove();
	};

	/**
	 * @brief \~japanese-en ノートの最大長さ(ミリ秒).
	 *        \~english Maximum length of note event in milli second.
	 */
	static const int MAX_NOTE_MILLISEC_LENGTH = 16383;

	/**
	 * @brief \~japanese-en ノートナンバーの最大値.
	 *        \~english Maximum note number.
	 */
	static const int MAX_NOTE_NUMBER = 127;

	/**
	 * @brief \~japanese-en ノートナンバーの最小値.
	 *        \~english Minimum note number.
	 */
	static const int MIN_NOTE_NUMBER = 0;

	/**
	 * @brief イベントに付けるタグ文字列.
	 */
	std::string tag;

	/**
	 * @brief 内部で使用するオブジェクト固有の ID.
	 */
	int id;

	/**
	 * @brief Tick 単位の時刻.
	 */
	tick_t tick;

	/**
	 * @brief 歌手ハンドル.
	 */
	Handle singerHandle;

	/**
	 * @brief ノート番号.
	 */
	int note;

	/**
	 * @brief ベロシティ.
	 */
	int dynamics;

	/**
	 * @brief ベンド深さ.
	 */
	int pmBendDepth;

	/**
	 * @brief ベンド長さ.
	 */
	int pmBendLength;

	/**
	 * @brief ポルタメント.
	 */
	int pmbPortamentoUse;

	/**
	 * @brief ディケイ.
	 */
	int demDecGainRate;

	/**
	 * @brief アクセント.
	 */
	int demAccent ;

	/**
	 * @brief 歌詞ハンドル.
	 */
	Handle lyricHandle;

	/**
	 * @brief ビブラートハンドル.
	 */
	Handle vibratoHandle;

	/**
	 * @brief イベント先頭から測った, ビブラートの開始位置(Tick 単位).
	 */
	int vibratoDelay;

	/**
	 * @brief アタックハンドル.
	 */
	Handle noteHeadHandle;

	/**
	 * @brief pMeanOnsetFirstNote の値.
	 */
	int pMeanOnsetFirstNote;

	/**
	 * @brief vMeanNoteTransition の値.
	 */
	int vMeanNoteTransition;

	/**
	 * @brief d4mean の値.
	 */
	int d4mean;

	/**
	 * @brief pMeanEndingNote の値.
	 */
	int pMeanEndingNote;

	/**
	 * @brief 強弱記号ハンドル.
	 */
	Handle iconDynamicsHandle;

protected:
	/**
	 * @brief EOSイベントかどうか.
	 */
	bool isEos;

private:
	/**
	 * @brief イベントの種類.
	 */
	EventType _type;

	/**
	 * @brief Tick 単位のイベント長さ.
	 */
	tick_t _length;

public:
	/**
	 * @brief 初期化を行う.
	 * @param tick Tick 単位の時刻.
	 * @param eventType イベントの種類.
	 */
	Event(tick_t tick, EventType eventType);

	virtual ~Event()
	{}

	/**
	 * @brief 長さを取得する.
	 * @return 長さ.
	 */
	tick_t length() const;

	/**
	 * @brief 長さを設定する.
	 * @param value 長さ.
	 */
	void length(tick_t value);

	/**
	 * @brief イベントの種類を返す.
	 */
	EventType type() const;

	/**
	 * @brief コピーを作成する.
	 * @return このインスタンスのコピー.
	 */
	Event clone() const;

	/**
	 * @brief このオブジェクトがイベントリストの末尾の要素( EOS )かどうかを取得する.
	 * @return このオブジェクトが EOS 要素であれば <code>true</code> を, そうでなければ <code>false</code> を返す.
	 */
	bool isEOS() const;

	/**
	 * @brief 順序を比較する.
	 * @param item 比較対象のアイテム.
	 * @return このインスタンスが比較対象よりも小さい場合は負の整数, 等しい場合は 0, 大きい場合は正の整数を返す.
	 */
	int compareTo(Event const& item) const;

	/**
	 * @brief \~japanese-en 2 つの Event を比較する.
	 *        \~english Compare two Event objects.
	 *
	 * @param a \~japanese-en 比較対象のオブジェクト.
	 *          \~english Compare target.
	 *
	 * @param b \~japanese-en 比較対象のオブジェクト.
	 *          \~english Compare target.
	 *
	 * @return \~japanese-en @a a が @a b よりも小さい場合は <code>true</code>, そうでない場合は <code>false</code> を返す.
	 *         \~english Return <code>true</code> if @a a is less than @a b, <code>false</code> if else.
	 */
	static bool compare(Event const& a, Event const& b);

	/**
	 * @copydoc Event::compare
	 */
	static bool comp(Event const* a, Event const* b);

	/**
	 * @brief イベントリストの末尾の要素を表すオブジェクトを取得する.
	 * @return オブジェクト.
	 */
	static Event eos();

	/*
	 * @param item [VsqEvent]
	 * @return [bool]
	 * @todo 実装できたら, TrackTest::testGetIndexIteratorNote, testGetIndexIteratorDynamics
	 *       の中のequals使うassertionを復活させること
	bool equals(Event const& other) const;
	 */

protected:
	/**
	 * @brief イベントの種類を設定する.
	 */
	void type(EventType type);

private:
	/**
	 * @brief 初期化を行う. この初期化メソッドは末尾のイベントリストを表すインスタンスを初期化する.
	 */
	Event();

	void init();
};

LIBVSQ_END_NAMESPACE
