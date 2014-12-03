/**
 * BPList.h
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
#ifndef __BPList_h__
#define __BPList_h__

#include "vsqglobal.hpp"
#include "BP.hpp"
#include "BPListSearchResult.hpp"
#include "TextStream.hpp"
#include "StringUtil.hpp"
#include <sstream>
#include <cmath>
#include <vector>
#include <boost/format.hpp>

VSQ_BEGIN_NAMESPACE

using namespace std;
using namespace VSQ_NS;

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
		explicit KeyClockIterator(BPList* list = NULL)
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
			if (_list) {
				return (_pos + 1 < _list->size());
			} else {
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
			return _list->getKeyClock(_pos);
		}

		/**
		 * @brief 反復子によって最後に返された要素を削除する
		 */
		void remove()
		{
			if (0 <= _pos && _pos < _list->size()) {
				for (int i = _pos; i < _list->size() - 1; i++) {
					_list->_clocks[i] = _list->_clocks[i + 1];
					_list->_items[i].value = _list->_items[i + 1].value;
					_list->_items[i].id = _list->_items[i + 1].id;
				}
				_list->_length--;
			}
		}
	};

	static const int INIT_BUFLEN = 512;

private:
	/**
	 * Tick 単位の時刻を格納したリスト
	 * @var table
	 * @access private
	 */
	std::vector<VSQ_NS::tick_t> _clocks;

	/**
	 * データ点の値と id のセットを格納した {@link BP} のリスト
	 * @var table
	 * @access private
	 */
	std::vector<VSQ_NS::BP> _items;

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
	explicit BPList()
	{
		_init();
	}

	/**
	 * @brief 初期化を行う
	 * @param name コントロールカーブの名前
	 * @param defaultValue コントロールカーブのデフォルト値
	 * @param minimum コントロールカーブの最小値
	 * @param maximum コントロールカーブの最大値
	 */
	explicit BPList(const string& name, int defaultValue, int minimum, int maximum)
	{
		_init();
		_name = name;
		_defaultValue = defaultValue;
		_maxValue = maximum;
		_minValue = minimum;
		_maxId = 0;
	}


	/**
	 * @brief コントロールカーブの名前を取得する
	 * @return コントロールカーブの名前
	 */
	const std::string getName() const
	{
		return _name;
	}

	/**
	 * @brief コントロールカーブの名前を設定する
	 * @param value コントロールカーブの名前
	 */
	void setName(const std::string& value)
	{
		_name = value;
	}

	/**
	 * @brief このリスト内で使用されている ID の最大値を取得する
	 * @return 使用されている ID の最大値
	 */
	int getMaxId() const
	{
		return _maxId;
	}

	/**
	 * @brief コントロールカーブのデフォルト値を取得する
	 * @return コントロールカーブのデフォルト値
	 */
	int getDefault() const
	{
		return _defaultValue;
	}

	/**
	 * @brief コントロールカーブのデフォルト値を設定する
	 * @param value コントロールカーブのデフォルト値
	 */
	void setDefault(int value)
	{
		_defaultValue = value;
	}

	/**
	 * @brief データ点の ID を一度クリアし，新たに番号付けを行います．
	 * ID は，Redo, Undo 用コマンドが使用するため，このメソッドを呼ぶと Redo, Undo 操作が破綻する．XML からのデシリアライズ直後のみ使用するべき．
	 */
	void renumberIds()
	{
		_maxId = 0;
		for (int i = 0; i < _length; i++) {
			_maxId++;
			_items[i].id = _maxId;
		}
	}

	/**
	 * @return
	 */
	const std::string getData() const
	{
		ostringstream ret;
		for (int i = 0; i < _length; i++) {
			if (0 < i) {
				ret << ",";
			}
			ret << _clocks[i] << "=" << _items[i].value;
		}
		return ret.str();
	}

	/**
	 * @param value [string]
	 * @return [void]
	 */
	void setData(const std::string& value)
	{
		_length = 0;
		_maxId = 0;
		vector<string> spl = StringUtil::explode(",", value);
		for (int i = 0; i < spl.size(); i++) {
			vector<string> spl2 = StringUtil::explode("=", spl[i]);
			if (spl2.size() < 2) {
				continue;
			}
			tick_t clock;
			int value;
			try {
				clock = StringUtil::parseInt<tick_t>(spl2[0]);
				value = StringUtil::parseInt<int>(spl2[1]);
			} catch (StringUtil::IntegerParseException&) {
				continue;
			}
			if (value < _minValue) {
				value = _minValue;
			}
			if (_maxValue < value) {
				value = _maxValue;
			}
			_ensureBufferLength(_length + 1);
			_clocks[_length] = clock;
			_items[_length] = BP(value, _maxId + 1);
			_maxId++;
			_length++;
		}
	}

	/**
	 * @brief コピーを作成する
	 * @return (BPList) このオブジェクトのコピー
	 */
	BPList clone() const
	{
		BPList res(_name, _defaultValue, _minValue, _maxValue);
		res._ensureBufferLength(_length);
		for (int i = 0; i < _length; i++) {
			res.addWithoutSort(_clocks[i], _items[i].value);
			res._items[i].id = _items[i].id;
		}
		res._length = _length;
		res._maxId = _maxId;
		return res;
	}

	/**
	 * @brief コントロールカーブの最大値を取得する
	 * @return (int) コントロールカーブの最大値
	 */
	int getMaximum() const
	{
		return _maxValue;
	}

	/**
	 * @brief コントロールカーブの最大値を設定する
	 * @param value (int) コントロールカーブの最大値
	 */
	void setMaximum(int value)
	{
		_maxValue = value;
	}

	/**
	 * @brief コントロールカーブの最小値を取得する
	 * @return (int) コントロールカーブの最小値
	 */
	int getMinimum() const
	{
		return _minValue;
	}

	/**
	 * @brief コントロールカーブの最小値を設定する
	 * @param value (int) コントロールカーブの最小値
	 */
	void setMinimum(int value)
	{
		_minValue = value;
	}

	/**
	 * @param clock (int)
	 * @return [void]
	 */
	void remove(VSQ_NS::tick_t clock)
	{
		_ensureBufferLength(_length);
		int index = _find(clock);
		removeElementAt(index);
	}

	/**
	 * @param index (int)
	 * @return [void]
	 */
	void removeElementAt(int index)
	{
		if (0 <= index && index < _length) {
			for (int i = index; i < _length - 1; i++) {
				_clocks[i] = _clocks[i + 1];
				_items[i].value = _items[i + 1].value;
				_items[i].id = _items[i + 1].id;
			}
			_length--;
		}
	}

	/**
	 * @brief 指定された時刻にデータ点が存在するかどうかを調べる
	 * @param clock (int) Tick 単位の時刻
	 * @return (boolean) データ点が存在すれば <code>ture</code> を、そうでなければ <code>false</code> を返す
	 */
	bool isContainsKey(VSQ_NS::tick_t clock) const
	{
		return (_find(clock) >= 0);
	}

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
	void move(VSQ_NS::tick_t clock, VSQ_NS::tick_t new_clock, int new_value)
	{
		_ensureBufferLength(_length);
		int index = _find(clock);
		if (index < 0) {
			return;
		}
		BP item = _items[index];
		for (int i = index; i < _length - 1; i++) {
			_clocks[i] = _clocks[i + 1];
			_items[i].value = _items[i + 1].value;
			_items[i].id = _items[i + 1].id;
		}
		_length--;
		int index_new = _find(new_clock);
		if (index_new >= 0) {
			_items[index_new].value = new_value;
			_items[index_new].id = item.id;
		} else {
			_length++;
			_ensureBufferLength(_length);
			_clocks[_length - 1] = new_clock;
			std::sort(_clocks.begin(), _clocks.begin() + _length);
			index_new = _find(new_clock);
			for (int i = _length - 1; i >= index_new + 1; i--) {
				_items[i].value = _items[i - 1].value;
				_items[i].id = _items[i - 1].id;
			}
			_items[index_new].value = new_value;
			_items[index_new].id = item.id;
		}
	}

	/**
	 * @brief 全てのデータ点を削除する
	 */
	void clear()
	{
		_length = 0;
	}

	/**
	 * @brief データ点の値を取得する
	 * @param index (int) 取得するデータ点のインデックス(最初のインデックスは0)
	 * @return (int) データ点の値
	 */
	int getValue(int index) const
	{
		return _items[index].value;
	}

	/**
	 * @brief データ点を取得する
	 * @param index (int) 取得するデータ点のインデックス(最初のインデックスは0)
	 * @return (BP) データ点のインスタンス
	 */
	const VSQ_NS::BP get(int index) const
	{
		return _items[index];
	}

	/**
	 * @brief データ点の時刻を取得する
	 * @param index (int) 取得するデータ点のインデックス(最初のインデックスは0)
	 * @return (int) データ点の Tick 単位の時刻
	 */
	VSQ_NS::tick_t getKeyClock(int index) const
	{
		return _clocks[index];
	}

	/**
	 * @brief ID を基にデータ点の値を取得する
	 * @param id (int) データ点の ID
	 * @return (int) データ点の値
	 */
	int findValueFromId(int id) const
	{
		for (int i = 0; i < _length; i++) {
			BP item = _items[i];
			if (item.id == id) {
				return item.value;
			}
		}
		return _defaultValue;
	}

	/**
	 * @brief ID を基にデータ点を検索し、検索結果を取得する
	 * @param id (int) データ点の ID
	 * @return (BPListSearchResult) 検索結果を格納したオブジェクト
	 */
	const VSQ_NS::BPListSearchResult findElement(int id) const
	{
		BPListSearchResult context;
		for (int i = 0; i < _length; i++) {
			BP item = _items[i];
			if (item.id == id) {
				context.clock = _clocks[i];
				context.index = i;
				context.point = item;
				return context;
			}
		}
		context.clock = -1;
		context.index = -1;
		context.point = BP(_defaultValue, -1);
		return context;
	}

	/**
	 * @brief 指定した ID のデータ点の値を設定する
	 * @param id (int) データ点の ID
	 * @param value (int) 設定するデータ点の値
	 */
	void setValueForId(int id, int value)
	{
		for (int i = 0; i < _length; i++) {
			if (_items[i].id == id) {
				_items[i].value = value;
				break;
			}
		}
	}

	/**
	 * @brief コントロールカーブをテキストストリームに出力する
	 * @param stream (TextStream) 出力先のストリーム
	 * @param startClock (int) Tick 単位の出力開始時刻
	 * @param header (string) 最初に出力するヘッダー文字列
	 */
	void print(VSQ_NS::TextStream& stream, VSQ_NS::tick_t startClock, const std::string& header) const
	{
		stream.writeLine(header);
		int lastvalue = _defaultValue;
		int value_at_start_written = false;
		for (int i = 0; i < _length; i++) {
			tick_t key = _clocks[i];
			if (startClock == key) {
				stream.write((boost::format("%d") % key).str());
				stream.write("=");
				stream.writeLine((boost::format("%d") % _items[i].value).str());
				value_at_start_written = true;
			} else if (startClock < key) {
				if ((!value_at_start_written) && (lastvalue != _defaultValue)) {
					stream.write((boost::format("%ld") % startClock).str());
					stream.write("=");
					stream.writeLine((boost::format("%d") % lastvalue).str());
					value_at_start_written = true;
				}
				int val = _items[i].value;
				stream.write((boost::format("%d") % key).str());
				stream.write("=");
				stream.writeLine((boost::format("%d") % val).str());
			} else {
				lastvalue = _items[i].value;
			}
		}
		if ((!value_at_start_written) && (lastvalue != _defaultValue)) {
			stream.write((boost::format("%ld") % startClock).str());
			stream.write("=");
			stream.writeLine((boost::format("%d") % lastvalue).str());
		}
	}

	/**
	 * @brief テキストファイルからデータ点を読込み、現在のリストに追加します
	 * @param reader [TextStream]
	 * @return [string]
	 */
	const std::string appendFromText(VSQ_NS::TextStream& reader)
	{
		tick_t clock = 0;
		int value = 0;
		int minus = 1;
		int mode = 0; // 0: clockを読んでいる, 1: valueを読んでいる
		while (reader.ready()) {
			string ch = reader.get();
			if (ch == "\n") {
				if (mode == 1) {
					addWithoutSort(clock, value * minus);
					mode = 0;
					clock = 0;
					value = 0;
					minus = 1;
				}
			} else {
				if (ch == "[") {
					if (mode == 1) {
						addWithoutSort(clock, value * minus);
						mode = 0;
						clock = 0;
						value = 0;
						minus = 1;
					}
					reader.setPointer(reader.getPointer() - 1);
					break;
				}
				if (ch == "=") {
					mode = 1;
				} else if (ch == "-") {
					minus = -1;
				} else {
					int num = -1;
					if (ch == "0") {
						num = 0;
					} else if (ch == "1") {
						num = 1;
					} else if (ch == "2") {
						num = 2;
					} else if (ch == "3") {
						num = 3;
					} else if (ch == "4") {
						num = 4;
					} else if (ch == "5") {
						num = 5;
					} else if (ch == "6") {
						num = 6;
					} else if (ch == "7") {
						num = 7;
					} else if (ch == "8") {
						num = 8;
					} else if (ch == "9") {
						num = 9;
					}
					if (num >= 0) {
						if (mode == 0) {
							clock = clock * 10 + num;
						} else {
							value = value * 10 + num;
						}
					}
				}
			}
		}
		return reader.readLine();
	}

	/**
	 * @brief データ点の個数を返す
	 * @return (int) データ点の個数
	 */
	int size() const
	{
		return _length;
	}

	/**
	 * @brief データ点の Tick 単位の時刻を昇順に返す反復子を取得する
	 * @return 反復子のインスタンス
	 */
	KeyClockIterator keyClockIterator()
	{
		return KeyClockIterator(this);
	}

	/**
	 * @brief データ点を追加する。指定された時刻に既にデータ点がある場合、データ点の値を上書きする
	 * @param clock (int) データ点を追加する Tick 単位の時刻
	 * @param value (int) データ点の値
	 * @return (int) データ点の ID
	 */
	int add(VSQ_NS::tick_t clock, int value)
	{
		_ensureBufferLength(_length);
		int index = _find(clock);
		if (index >= 0) {
			_items[index].value = value;
			return _items[index].id;
		} else {
			_length++;
			_ensureBufferLength(_length);
			_clocks[_length - 1] = clock;
			std::sort(_clocks.begin(), _clocks.begin() + _length);
			index = _find(clock);
			_maxId++;
			for (int i = _length - 1; i >= index + 1; i--) {
				_items[i].value = _items[i - 1].value;
				_items[i].id = _items[i - 1].id;
			}
			_items[index].value = value;
			_items[index].id = _maxId;
			return _maxId;
		}
	}

	/**
	 * @brief データ点を、ID 指定したうえで追加する。指定された時刻に既にデータ点がある場合、データ点の値を上書きする
	 * @param clock (int) データ点を追加する Tick 単位の時刻
	 * @param value (int) データ点の値
	 * @param id (int) データ点の ID
	 * @return (int) データ点の ID
	 */
	int addWithId(VSQ_NS::tick_t clock, int value, int id)
	{
		_ensureBufferLength(_length);
		int index = _find(clock);
		if (index >= 0) {
			_items[index].value = value;
			_items[index].id = id;
		} else {
			_length++;
			_ensureBufferLength(_length);
			_clocks[_length - 1] = clock;
			std::sort(_clocks.begin(), _clocks.begin() + _length);
			index = _find(clock);
			for (int i = _length - 1; i >= index + 1; i--) {
				_items[i].value = _items[i - 1].value;
				_items[i].id = _items[i - 1].id;
			}
			_items[index].value = value;
			_items[index].id = id;
		}
		_maxId = ::max(_maxId, id);
		return id;
	}

	/**
	 * @param id [long]
	 * @return [void]
	 */
	void removeWithId(int id)
	{
		for (int i = 0; i < _length; i++) {
			if (_items[i].id == id) {
				for (int j = i; j < _length - 1; j++) {
					_items[j].value = _items[j + 1].value;
					_items[j].id = _items[j + 1].id;
					_clocks[j] = _clocks[j + 1];
				}
				_length--;
				break;
			}
		}
	}

	/**
	 * @brief 指定された Tick 単位の時刻における、コントロールパラメータの値を取得する
	 * @param clock (int) 値を取得する Tick 単位の時刻
	 * @return (int) コントロールパラメータの値
	 * @name getValueAt<! *1 *>
	 */
	int getValueAt(VSQ_NS::tick_t clock) const
	{
		int index = _find(clock);
		if (index >= 0) {
			return _items[index].value;
		} else {
			if (_length <= 0) {
				return _defaultValue;
			} else {
				int draft = -1;
				for (int i = 0; i < _length; i++) {
					int c = _clocks[i];
					if (clock < c) {
						break;
					}
					draft = i;
				}
				if (draft == -1) {
					return _defaultValue;
				} else {
					return _items[draft].value;
				}
			}
		}
	}

	/**
	 * @brief 指定された Tick 単位の時刻における、コントロールパラメータの値を取得する
	 * @param clock (int) 値を取得する Tick 単位の時刻
	 * @param index (table) 値の取得に使用したインデックス(最初のインデックスは0)
	 * @return (int) コントロールパラメータの値
	 * @name getValueAt<! *2 *>
	 */
	int getValueAt(VSQ_NS::tick_t clock, int* index) const
	{
		if (_length == 0) {
			return _defaultValue;
		} else {
			if (*index < 0 || _length <= *index) {
				*index = 0;
			}
			if (0 <= *index && *index < _length) {
				if (clock < _clocks[*index]) {
					*index = 0;
				}
			}
			for (int i = *index; i < _length; i++) {
				tick_t keyclock = _clocks[i];
				if (clock < keyclock) {
					if (1 <= i) {
						*index = i - 1;
						return _items[i - 1].value;
					} else {
						*index = 0;
						return _defaultValue;
					}
				}
			}
			*index = _length - 1;
			return _items[_length - 1].value;
		}
	}

private:
	void _init()
	{
		_length = 0;
		_defaultValue = 0;
		_maxValue = 127;
		_minValue = 0;
		_maxId = 0;
		_name = "";
	}

	/**
	 * @brief データ点を格納するバッファを確保する
	 * @param length (int) 確保するバッファの最小長さ
	 */
	void _ensureBufferLength(int length)
	{
		if (length > _clocks.size()) {
			int newLength = length;
			if (_clocks.size() <= 0) {
				newLength = (int)::floor(length * 1.2);
			} else {
				int order = length / _clocks.size();
				if (order <= 1) {
					order = 2;
				}
				newLength = _clocks.size() * order;
			}
			int delta = newLength - _clocks.size();
			for (int i = 0; i < delta; i++) {
				_clocks.push_back(0);
				_items.push_back(BP(0, 0));
			}
		}
	}

	/**
	 * @brief 指定された時刻値を持つデータ点のインデックスを検索する
	 * @param value (int) Tick 単位の時刻
	 * @return (int) データ点のインデックス(最初のインデックスは0)。データ点が見つからなかった場合は負の値を返す
	 */
	int _find(VSQ_NS::tick_t value) const
	{
		for (int i = 0; i < _length; i++) {
			if (_clocks[i] == value) {
				return i;
			}
		}
		return -1;
	}

	/**
	 * @brief 並べ替え、既存の値との重複チェックを行わず、リストの末尾にデータ点を追加する
	 * @param clock (int) Tick 単位の時刻
	 * @param value (int) データ点の値
	 */
	void addWithoutSort(VSQ_NS::tick_t clock, int value)
	{
		_ensureBufferLength(_length + 1);
		_clocks[_length] = clock;
		_maxId++;
		_items[_length].value = value;
		_items[_length].id = _maxId;
		_length++;
	}

};

VSQ_END_NAMESPACE

#endif
