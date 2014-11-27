/**
 * Event.hpp
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

VSQ_BEGIN_NAMESPACE

/**
 * VSQ ファイルのメタテキスト内に記述されるイベントを表すクラス
 * @class table
 * @name Event
 */
class Event
{
	friend class VSQFileReader; //TODO:Track::Track(TextStream &, ...)の機能がVSQFIleREaderに移動したら、ここは不要になるので消す
public:
	class ListIterator;
	class ListConstIterator;

	/**
	 * @brief 固有 ID 付きの {@link Event} のリストを取り扱うクラス
	 */
	class List
	{
		friend class ListIterator;
		friend class ListConstIterator;

	protected:
		/**
		 * @brief イベントのリスト
		 */
		std::vector<Event*> _events;

		/**
		 * @brief イベントの ID のリスト
		 */
		std::vector<int> _ids;

	public:
		List();

		List(const List& list);

		List& operator = (const List& list);

		~List();

		/**
		 * @brief イベント ID を基にイベントを検索し、そのインデックスを返す
		 * @param internalId 検索するイベント ID
		 * @return 検索結果のインデックス(最初のインデックスは0)。イベントが見つからなければ負の値を返す
		 */
		int findIndexFromId(int internalId) const;

		/**
		 * @brief イベント ID を基にイベントを検索し、そのオブジェクトを返す
		 * @param internalId 検索するイベント ID
		 * @return 検索結果のイベント。イベントが見つからなければ <code>nil</code> を返す
		 */
		const Event* findFromId(int internalId) const;

		/**
		 * @brief 指定されたイベント ID をもつイベントのオブジェクトを置き換える。イベントが見つからなければ何もしない
		 * @param internalId 検索するイベント ID
		 * @param value 置換するオブジェクト
		 */
		void setForId(int internalId, const Event& value);

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
		ListIterator iterator();

		const ListConstIterator iterator() const;

		/**
		 * @brief イベントを追加する
		 * @param item 追加するオブジェクト
		 * @return 追加したオブジェクトに割り振られたイベント ID
		 */
		int add(const Event& item);

		/**
		 * @brief イベントを追加する
		 * @param item 追加するオブジェクト
		 * @param internalId 追加するオブジェクトに割り振るイベント ID
		 * @return オブジェクトに割り振られたイベント ID
		 */
		int add(const Event& item, int internalId);

		/**
		 * @brief イベントを削除する
		 * @param index 削除するイベントのインデックス(最初のインデックスは0)
		 */
		void removeAt(int index);

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
		const Event* get(int index) const;

		/**
		 * @brief 指定したインデックスのイベントを設定する
		 * @param index インデックス(最初のインデックスは0)
		 * @param value 設定するイベント
		 */
		void set(int index, const Event& value);

		/**
		 * @brief リスト内部のイベント ID のデータを更新する
		 */
		void updateIdList();

	private:
		/**
		 * @brief イベントを追加する
		 * @param item 追加するオブジェクト
		 * @param internal_id 追加するオブジェクトに割り振るイベント ID
		 */
		void _addCor(const Event& item, int internalId);

		/**
		 * @brief イベントに割り振る ID を取得する
		 * @param next
		 * @return
		 */
		int _getNextId(int next);

		/**
		 * @brief Deep copy member fields.
		 * @param list Copy source.
		 */
		void copy(const List& list);
	};

	/**
	 * @brief イベントリストのアイテムを順に返す反復子
	 */
	class ListConstIterator
	{
	protected:
		/**
		 * @brief 反復子の元になるリスト
		 */
		const List* _list;

		/**
		 * @brief 反復子の現在の位置
		 */
		int _pos;

	public:
		/**
		 * @brief 初期化を行う
		 * @param list 反復子の元になるリスト
		 */
		ListConstIterator(const List* list);

		/**
		 * @brief 反復子が次の要素を持つ場合に <code>true</code> を返す
		 * @return 反復子がさらに要素を持つ場合は <code>true</code> を、そうでなければ <code>false</code> を返す
		 */
		bool hasNext();

		/**
		 * @brief 反復子の次の要素を返す
		 * @return 次の要素
		 */
		Event* next();
	};

	class ListIterator : public ListConstIterator
	{
	private:
		List* _nonConstList;

	public:
		explicit ListIterator(List* list);

		/**
		 * @brief 反復子によって最後に返された要素を削除する
		 */
		void remove();
	};

	/**
	 * @brief Maximum length of note event in milli second.
	 */
	static const int MAX_NOTE_MILLISEC_LENGTH = 16383;

	/**
	 * @brief Maximum note number.
	 */
	static const int MAX_NOTE_NUMBER = 127;

	/**
	 * @brief Minimum note number.
	 */
	static const int MIN_NOTE_NUMBER = 0;

	/**
	 * イベントに付けるタグ文字列
	 * @var string
	 */
	std::string tag;

	/**
	 * 内部で使用するオブジェクト固有の ID
	 * @var int
	 */
	int id;

	/**
	 * Tick 単位の時刻
	 * @var int
	 */
	tick_t clock;

	/**
	 * イベントの種類
	 * @var EventTypeEnum
	 * @todo private|protectedにするべきでは
	 */
	EventType::EventTypeEnum type;

	/**
	 * @brief 歌手ハンドル
	 */
	Handle singerHandle;

	/**
	 * ノート番号
	 * @var int
	 */
	int note;

	/**
	 * ベロシティ
	 * @var int
	 */
	int dynamics;

	/**
	 * ベンド深さ
	 * @var int
	 */
	int pmBendDepth;

	/**
	 * ベンド長さ
	 * @var int
	 */
	int pmBendLength;

	/**
	 * ポルタメント
	 * @var int
	 */
	int pmbPortamentoUse;

	/**
	 * ディケイ
	 * @var int
	 */
	int demDecGainRate;

	/**
	 * アクセント
	 * @var int
	 */
	int demAccent ;

	/**
	 * @brief 歌詞ハンドル
	 */
	Handle lyricHandle;

	/**
	 * @brief ビブラートハンドル
	 */
	Handle vibratoHandle;

	/**
	 * イベント先頭から測った、ビブラートの開始位置(Tick 単位)
	 * @var int
	 */
	int vibratoDelay ;

	/**
	 * @brief アタックハンドル
	 */
	Handle noteHeadHandle;

	/**
	 * @var int
	 */
	int pMeanOnsetFirstNote;

	/**
	 * @var int
	 */
	int vMeanNoteTransition;

	/**
	 * @var int
	 */
	int d4mean;

	/**
	 * @var int
	 */
	int pMeanEndingNote;

	/**
	 * @brief 強弱記号ハンドル
	 * @todo 型をHandle*に変える
	 */
	Handle iconDynamicsHandle;

protected:
	/**
	 * @brief EOSイベントかどうか
	 */
	bool isEos;

private:
	/**
	 * Tick 単位のイベント長さ
	 * @var int
	 * @access private
	 */
	tick_t _length;

	/**
	 * @todo 未実装
	 * @var UstEvent
	UstEvent ustEvent = nil;
	 */

public:
	/**
	 * @brief 初期化を行う
	 * @param line (string) VSQ メタテキスト中の [EventList] セクション内のイベント宣言文字列(ex."480=ID#0001")
	 */
	explicit Event(std::string const& line);

	/**
	 * @brief 初期化を行う。この初期化メソッドは末尾のイベントリストを表すインスタンスを初期化する
	 */
	Event();

	/**
	 * @brief 初期化を行う
	 * @param clock (int) Tick 単位の時刻
	 * @param eventType (EventTypeEnum) イベントの種類
	 */
	Event(tick_t clock, EventType::EventTypeEnum eventType);

	/**
	 * @brief 長さを取得する
	 * @return (int) 長さ
	 */
	tick_t getLength() const;

	/**
	 * @brief 長さを設定する
	 * @param value (int) 長さ
	 */
	void setLength(tick_t value);

	/**
	 * @brief コピーを作成する
	 * @return (Event) このインスタンスのコピー
	 */
	Event clone() const;

	/**
	 * このオブジェクトがイベントリストの末尾の要素( EOS )かどうかを取得する
	 * @return (boolean) このオブジェクトが EOS 要素であれば <code>true</code> を、そうでなければ <code>false</code> を返す
	 */
	bool isEOS() const;

	/**
	 * @brief 順序を比較する
	 * @param item (Event) 比較対象のアイテム
	 * @return (int) このインスタンスが比較対象よりも小さい場合は負の整数、等しい場合は 0、大きい場合は正の整数を返す
	 */
	int compareTo(const Event& item) const;

	/**
	 * @brief 2 つの {@link Event} を比較する
	 * @param a (Event) 比較対象のオブジェクト
	 * @param b (Event) 比較対象のオブジェクト
	 * @return (boolean) <code>a</code> が <code>b</code> よりも小さい場合は <code>true</code>、そうでない場合は <code>false</code> を返す
	 */
	static bool compare(Event const& a, Event const& b);

	/**
	 * @brief Compare 2 Event objects.
	 * @param a Compare target.
	 * @param b Compare target.
	 */
	static bool comp(Event const* a, Event const* b);

	/**
	 * @brief イベントリストの末尾の要素を表すオブジェクトを取得する
	 * @return (Event) オブジェクト
	 */
	static const Event getEOS();

	/*
	    -- @param item [VsqEvent]
	    -- @return [bool]
	    -- @todo 実装できたら、TrackTest::testGetIndexIteratorNote, testGetIndexIteratorDynamics
	             の中のequals使うassertionを復活させること
	bool equals(Event const& other) const;
	 */

private:
	void init();
};

VSQ_END_NAMESPACE
