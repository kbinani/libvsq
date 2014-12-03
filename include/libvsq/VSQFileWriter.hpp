/**
 * VSQFileWriter.hpp
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

#include "./Event.hpp"
#include "./Track.hpp"
#include "./MidiEvent.hpp"
#include "./PublicForUnitTest.hpp"
#include <memory>

LIBVSQ_BEGIN_NAMESPACE

class Sequence;
class OutputStream;
class Master;
class Mixer;

class VSQFileWriter
{
public:
	VSQFileWriter();

	~VSQFileWriter();

	/**
	 * @brief ストリームに出力する.
	 * @param 出力するシーケンス.
	 * @param stream 出力先のストリーム.
	 * @param msPreSend ミリ秒単位のプリセンドタイム.
	 * @param encoding マルチバイト文字のテキストエンコーディング(現在は Shift_JIS 固定で, 引数は無視される).
	 * @param printPitch pitch を含めて出力するかどうか(現在は <code>false</code> 固定で, 引数は無視される).
	 */
	void write(Sequence const& sequence, OutputStream& stream, int msPreSend, std::string const& encoding, bool printPitch = false);

LIBVSQ_PRIVATE_BUT_PUBLIC_FOR_UNITTEST:
	/**
	 * @brief ハンドルをストリームに書き込む.
	 * @param item 書き込むハンドル.
	 * @param stream 書き込み先のストリーム.
	 */
	void _writeHandle(Handle const& item, TextStream& stream);

	/**
	 * @brief テキストストリームにイベントを書き出す.
	 * @param stream 出力先.
	 * @param printTargets 出力するアイテムのリスト.
	 */
	void _writeEvent(Event const* item, int event_index, TextStream& stream) const;

	/**
	 * @brief トラックのメタテキストを, テキストストリームに出力する.
	 * @param track 出力するトラック.
	 * @param stream 出力先のストリーム.
	 * @param eos イベントリストの末尾を表す番号.
	 * @param start Tick 単位の出力開始時刻.
	 * @param printPitch pitch を含めて出力するかどうか(現在は <code>false</code> 固定で, 引数は無視される).
	 * @param master 出力する Master 情報. 出力しない場合は NULL を指定する.
	 * @param mixer 出力する Mixer 情報. 出力しない場合は NULL を指定する.
	 */
	void _printMetaText(Track const& track, TextStream& stream, int eos, tick_t start, bool printPitch = false, Master* master = 0, Mixer* mixer = 0);

	/**
	 * @brief 文字列を MIDI メタイベントにしたものを取得する.
	 * @param sr MIDI イベント生成元の文字列が出力されたストリーム.
	 * @param encoding マルチバイト文字のテキストエンコーディング(現在は Shift_JIS 固定で, 引数は無視される).
	 */
	std::vector<MidiEvent> _getMidiEventsFromMetaText(TextStream* sr, std::string const& encoding);

	/**
	 * @brief "DM:0001:"といった, VSQメタテキストの行の先頭につくヘッダー文字列のバイト列表現を取得する.
	 * @param count ヘッダーの番号.
	 * @return バイト列.
	 */
	std::vector<int> _getLinePrefixBytes(int count);

	/**
	 * @brief 数値の 10 進数での桁数を取得する.
	 * @param number 検査対象の数値.
	 * @return 数値の 10 進数での桁数.
	 */
	int _getHowManyDigits(int number);

	/**
	 * @brief 16 ビットの unsigned int 値をビッグエンディアンでストリームに書きこむ.
	 * @param stream 出力先のストリーム.
	 * @param data 出力する値.
	 */
	void _writeUnsignedShort(OutputStream& stream, int data);

	/**
	 * @brief 32 ビットの unsigned int 値をビッグエンディアンでストリームに書きこむ.
	 * @param stream 出力先のストリーム.
	 * @param data 出力する値.
	 */
	void _writeUnsignedInt(OutputStream& stream, int data);

private:
	class Impl;
	std::unique_ptr<Impl> _impl;
};

LIBVSQ_END_NAMESPACE
