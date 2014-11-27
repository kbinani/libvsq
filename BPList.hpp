/**
 * BPList.h
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
#include "./BP.hpp"
#include "./BPListSearchResult.hpp"
#include <vector>
#include <string>

VSQ_BEGIN_NAMESPACE

class TextStream;

/**
 * @brief コントロールカーブのデータ点リストを表すクラス
 */
class BPList
{
public:
	/**
	 * @brief コントロールカーブのデータ点の Tick 単位の時刻を順に返す反復子
	 */
	class KeyClockIterator
	{
	private:
		/**
		 * @brief 反復子の元になるリスト
		 */
		BPList* _list;

		/**
		 * 反復子の現在の位置
		 */
		int _pos;

	public:
		/**
		 * @brief 初期化を行う
		 * @param list (BPList) 反復子の元になるリスト
		 */
		explicit KeyClockIterator(BPList* list = NULL);

		/**
		 * @brief 反復子が次の要素を持つ場合に <code>true</code> を返す
		 * @return (boolean) 反復子がさらに要素を持つ場合は <code>true</code> を、そうでなければ <code>false</code> を返す
		 */
		bool hasNext();

		/**
		 * @brief 反復子の次の要素を返す
		 * @return (int) 次の要素
		 */
		tick_t next();

		/**
		 * @brief 反復子によって最後に返された要素を削除する
		 */
		void remove();
	};

	static const int INIT_BUFLEN = 512;

private:
	/**
	 * Tick 単位の時刻を格納したリスト
	 * @var table
	 * @access private
	 */
	std::vector<tick_t> _clocks;

	/**
	 * データ点の値と id のセットを格納した {@link BP} のリスト
	 * @var table
	 * @access private
	 */
	std::vector<BP> _items;

	/**
	 * 現在のリストの長さ
	 * @var int
	 * @access private
	 */
	int _length;

	/**
	 * コントロールカーブのデフォルト値
	 * @var int
	 * @access private
	 */
	int _defaultValue;

	/**
	 * コントロールカーブの最大値
	 * @var int
	 * @access private
	 */
	int _maxValue;

	/**
	 * コントロールカーブの最小値
	 * @var int
	 * @access private
	 */
	int _minValue;

	/**
	 * このリスト内で使用されている ID の最大値
	 * @var int
	 * @access private
	 */
	int _maxId;

	/**
	 * コントロールカーブの名前
	 * @var string
	 * @access private
	 */
	std::string _name;

public:
	BPList();

	/**
	 * @brief 初期化を行う
	 * @param name コントロールカーブの名前
	 * @param defaultValue コントロールカーブのデフォルト値
	 * @param minimum コントロールカーブの最小値
	 * @param maximum コントロールカーブの最大値
	 */
	BPList(std::string const& name, int defaultValue, int minimum, int maximum);

	/**
	 * @brief コントロールカーブの名前を取得する
	 * @return コントロールカーブの名前
	 */
	const std::string getName() const;

	/**
	 * @brief コントロールカーブの名前を設定する
	 * @param value コントロールカーブの名前
	 */
	void setName(std::string const& value);

	/**
	 * @brief このリスト内で使用されている ID の最大値を取得する
	 * @return 使用されている ID の最大値
	 */
	int getMaxId() const;

	/**
	 * @brief コントロールカーブのデフォルト値を取得する
	 * @return コントロールカーブのデフォルト値
	 */
	int getDefault() const;

	/**
	 * @brief コントロールカーブのデフォルト値を設定する
	 * @param value コントロールカーブのデフォルト値
	 */
	void setDefault(int value);

	/**
	 * @brief データ点の ID を一度クリアし，新たに番号付けを行います．
	 * ID は，Redo, Undo 用コマンドが使用するため，このメソッドを呼ぶと Redo, Undo 操作が破綻する．XML からのデシリアライズ直後のみ使用するべき．
	 */
	void renumberIds();

	/**
	 * @return
	 */
	const std::string getData() const;

	/**
	 * @param value [string]
	 * @return [void]
	 */
	void setData(std::string const& value);

	/**
	 * @brief コピーを作成する
	 * @return (BPList) このオブジェクトのコピー
	 */
	BPList clone() const;

	/**
	 * @brief コントロールカーブの最大値を取得する
	 * @return (int) コントロールカーブの最大値
	 */
	int getMaximum() const;

	/**
	 * @brief コントロールカーブの最大値を設定する
	 * @param value (int) コントロールカーブの最大値
	 */
	void setMaximum(int value);

	/**
	 * @brief コントロールカーブの最小値を取得する
	 * @return (int) コントロールカーブの最小値
	 */
	int getMinimum() const;

	/**
	 * @brief コントロールカーブの最小値を設定する
	 * @param value (int) コントロールカーブの最小値
	 */
	void setMinimum(int value);

	/**
	 * @param clock (int)
	 * @return [void]
	 */
	void remove(tick_t clock);

	/**
	 * @param index (int)
	 * @return [void]
	 */
	void removeElementAt(int index);

	/**
	 * @brief 指定された時刻にデータ点が存在するかどうかを調べる
	 * @param clock (int) Tick 単位の時刻
	 * @return (boolean) データ点が存在すれば <code>ture</code> を、そうでなければ <code>false</code> を返す
	 */
	bool isContainsKey(tick_t clock) const;

	/**
	 * @brief 時刻clockのデータを時刻new_clockに移動します。
	 * 時刻clockにデータがなければ何もしない。
	 * 時刻new_clockに既にデータがある場合、既存のデータは削除される。
	 *
	 * @param clock (int)
	 * @param new_clock (int)
	 * @param new_value (int)
	 * @return [void]
	 */
	void move(tick_t clock, tick_t new_clock, int new_value);

	/**
	 * @brief 全てのデータ点を削除する
	 */
	void clear();

	/**
	 * @brief データ点の値を取得する
	 * @param index (int) 取得するデータ点のインデックス(最初のインデックスは0)
	 * @return (int) データ点の値
	 */
	int getValue(int index) const;

	/**
	 * @brief データ点を取得する
	 * @param index (int) 取得するデータ点のインデックス(最初のインデックスは0)
	 * @return (BP) データ点のインスタンス
	 */
	const BP get(int index) const;

	/**
	 * @brief データ点の時刻を取得する
	 * @param index (int) 取得するデータ点のインデックス(最初のインデックスは0)
	 * @return (int) データ点の Tick 単位の時刻
	 */
	tick_t getKeyClock(int index) const;

	/**
	 * @brief ID を基にデータ点の値を取得する
	 * @param id (int) データ点の ID
	 * @return (int) データ点の値
	 */
	int findValueFromId(int id) const;

	/**
	 * @brief ID を基にデータ点を検索し、検索結果を取得する
	 * @param id (int) データ点の ID
	 * @return (BPListSearchResult) 検索結果を格納したオブジェクト
	 */
	const BPListSearchResult findElement(int id) const;

	/**
	 * @brief 指定した ID のデータ点の値を設定する
	 * @param id (int) データ点の ID
	 * @param value (int) 設定するデータ点の値
	 */
	void setValueForId(int id, int value);

	/**
	 * @brief コントロールカーブをテキストストリームに出力する
	 * @param stream (TextStream) 出力先のストリーム
	 * @param startClock (int) Tick 単位の出力開始時刻
	 * @param header (string) 最初に出力するヘッダー文字列
	 */
	void print(TextStream& stream, tick_t startClock, std::string const& header) const;

	/**
	 * @brief テキストファイルからデータ点を読込み、現在のリストに追加します
	 * @param reader [TextStream]
	 * @return [string]
	 */
	const std::string appendFromText(TextStream& reader);

	/**
	 * @brief データ点の個数を返す
	 * @return (int) データ点の個数
	 */
	int size() const;

	/**
	 * @brief データ点の Tick 単位の時刻を昇順に返す反復子を取得する
	 * @return 反復子のインスタンス
	 */
	KeyClockIterator keyClockIterator();

	/**
	 * @brief データ点を追加する。指定された時刻に既にデータ点がある場合、データ点の値を上書きする
	 * @param clock (int) データ点を追加する Tick 単位の時刻
	 * @param value (int) データ点の値
	 * @return (int) データ点の ID
	 */
	int add(tick_t clock, int value);

	/**
	 * @brief データ点を、ID 指定したうえで追加する。指定された時刻に既にデータ点がある場合、データ点の値を上書きする
	 * @param clock (int) データ点を追加する Tick 単位の時刻
	 * @param value (int) データ点の値
	 * @param id (int) データ点の ID
	 * @return (int) データ点の ID
	 */
	int addWithId(tick_t clock, int value, int id);

	/**
	 * @param id [long]
	 * @return [void]
	 */
	void removeWithId(int id);

	/**
	 * @brief 指定された Tick 単位の時刻における、コントロールパラメータの値を取得する
	 * @param clock (int) 値を取得する Tick 単位の時刻
	 * @return (int) コントロールパラメータの値
	 * @name getValueAt<! *1 *>
	 */
	int getValueAt(tick_t clock) const;

	/**
	 * @brief 指定された Tick 単位の時刻における、コントロールパラメータの値を取得する
	 * @param clock (int) 値を取得する Tick 単位の時刻
	 * @param index (table) 値の取得に使用したインデックス(最初のインデックスは0)
	 * @return (int) コントロールパラメータの値
	 * @name getValueAt<! *2 *>
	 */
	int getValueAt(tick_t clock, int* index) const;

private:
	void _init();

	/**
	 * @brief データ点を格納するバッファを確保する
	 * @param length (int) 確保するバッファの最小長さ
	 */
	void _ensureBufferLength(int length);

	/**
	 * @brief 指定された時刻値を持つデータ点のインデックスを検索する
	 * @param value (int) Tick 単位の時刻
	 * @return (int) データ点のインデックス(最初のインデックスは0)。データ点が見つからなかった場合は負の値を返す
	 */
	int _find(tick_t value) const;

	/**
	 * @brief 並べ替え、既存の値との重複チェックを行わず、リストの末尾にデータ点を追加する
	 * @param clock (int) Tick 単位の時刻
	 * @param value (int) データ点の値
	 */
	void addWithoutSort(tick_t clock, int value);
};

VSQ_END_NAMESPACE
