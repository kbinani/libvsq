/**
 * MidiEvent.hpp
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
#include <exception>
#include <string>
#include <vector>

VSQ_BEGIN_NAMESPACE

class OutputStream;
class InputStream;

/**
 * @brief MIDI イベントを表現するクラス.
 * メタイベントは, メタイベントのデータ長をData[1]に格納せず, 生のデータをDataに格納するので, 注意が必要.
 */
class MidiEvent
{
public:
	class ParseException : public std::exception
	{
		std::string message;
	public:
		explicit ParseException(std::string const& message)
			: message(message)
		{}

		~ParseException()
		{}

		std::string getMessage() const
		{
			return message;
		}
	};

	/**
	 * @brief Tick 単位の時刻.
	 */
	tick_t tick;

	/**
	 * @brief MIDI イベントの先頭バイト.
	 */
	int firstByte;

	/**
	 * @brief MIDI イベントのデータ.
	 * メタイベントについては長さ値を保持せず, 出力時に <code>data</code> フィールドの長さに応じた値を自動的に出力する.
	 */
	std::vector<int> data;

	MidiEvent();

	/**
	 * @brief MIDI データをストリームに出力する.
	 * @param stream 出力先のストリーム.
	 */
	void writeData(OutputStream& stream) const;

	/**
	 * @brief 順序を比較する.
	 * @param item 比較対象のアイテム.
	 * @return このインスタンスが比較対象よりも小さい場合は負の整数, 等しい場合は 0, 大きい場合は正の整数を返す.
	 */
	int compareTo(MidiEvent const& item) const;

	/**
	 * @brief 拍子イベントを作成する.
	 * @param tick Tick 単位の時刻.
	 * @param numerator 拍子の分子の値.
	 * @param denominator 表紙の分母の値.
	 * @return 拍子イベント.
	 */
	static MidiEvent generateTimeSigEvent(tick_t tick, int numerator, int denominator);

	/**
	 * @brief テンポイベントを作成する.
	 * @param tick Tick 単位の時刻.
	 * @param tempo 四分音符のマイクロ秒単位の長さ.
	 * @return テンポイベント.
	 */
	static MidiEvent generateTempoChangeEvent(tick_t tick, int tempo);

	/**
	 * @brief 可変長のデルタタイムをストリームに出力する.
	 * @param stream 出力先のストリーム.
	 * @param number デルタタイム.
	 */
	static void writeDeltaTick(OutputStream& stream, int number);

	/**
	 * @brief ストリームから, デルタタイムを読み込む.
	 * @param stream 読み込み元のストリーム.
	 * @return デルタタイム.
	 */
	static tick_t readDeltaTick(InputStream& stream);

	/**
	 * @brief ストリームから MIDI イベントを一つ読み込む.
	 * @param stream 読み込み元のストリーム.
	 * @param last_tick
	 * @param last_status_byte
	 */
	static MidiEvent read(InputStream& stream, tick_t& last_tick, uint8_t& last_status_byte);

	/**
	 * @brief 2 つの {@link MidiEvent} を比較する.
	 * @param a 比較対象のオブジェクト.
	 * @param b 比較対象のオブジェクト.
	 * @return <code>a</code> が <code>b</code> よりも小さい場合は <code>true</code>, そうでない場合は <code>false</code> を返す.
	 */
	static bool compare(MidiEvent const& a, MidiEvent const& b);
};

VSQ_END_NAMESPACE
