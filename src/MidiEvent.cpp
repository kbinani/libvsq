/**
 * MidiEvent.cpp
 * Copyright © 2014 kbinani
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
#include "../include/libvsq/MidiEvent.hpp"
#include "../include/libvsq/OutputStream.hpp"
#include "../include/libvsq/InputStream.hpp"
#include "../include/libvsq/StringUtil.hpp"
#include <cmath>

LIBVSQ_BEGIN_NAMESPACE

MidiEvent::MidiEvent()
{
	tick = 0;
	firstByte = 0;
}

void MidiEvent::writeData(OutputStream& stream) const
{
	stream.write(firstByte);
	int size = (int)data.size();
	if (0 < size) {
		std::vector<char> buffer(size);
		for (int i = 0; i < size; i++) {
			buffer[i] = (char)data[i];
		}
		if (firstByte == 0xff) {
			stream.write(buffer[0]);
			writeDeltaTick(stream, size - 1);
			stream.write(buffer.data(), 1, size - 1);
		} else {
			stream.write(buffer.data(), 0, size);
		}
	}
}

int MidiEvent::compareTo(MidiEvent const& item) const
{
	if (tick != item.tick) {
		return tick - item.tick;
	} else {
		int first_this = firstByte & 0xf0;
		int first_item = item.firstByte & 0xf0;

		if ((first_this == 0x80 || first_this == 0x90) && (first_item == 0x80 || first_item == 0x90)) {
			if (!data.empty() && data.size() >= 2 && !item.data.empty() && item.data.size() >= 2) {
				if (first_item == 0x90 && item.data[1] == 0) {
					first_item = 0x80;
				}
				if (first_this == 0x90 && data[1] == 0) {
					first_this = 0x80;
				}
				if (data[0] == item.data[0]) {
					if (first_this == 0x90) {
						if (first_item == 0x80) {
							// ON -> OFF
							return 1;
						} else {
							// ON -> ON
							return 0;
						}
					} else {
						if (first_item == 0x80) {
							// OFF -> OFF
							return 0;
						} else {
							// OFF -> ON
							return -1;
						}
					}
				}
			}
		}
		return tick - item.tick;
	}
}

MidiEvent MidiEvent::generateTimeSigEvent(tick_t tick, int numerator, int denominator)
{
	MidiEvent ret;
	ret.tick = tick;
	ret.firstByte = 0xff;
	int b_numer = (int)::floor(::log((double)denominator) / ::log(2.0) + 0.1);
	ret.data.push_back(0x58);
	ret.data.push_back(numerator);
	ret.data.push_back(b_numer);
	ret.data.push_back(0x18);
	ret.data.push_back(0x08);
	return ret;
}

MidiEvent MidiEvent::generateTempoChangeEvent(tick_t tick, int tempo)
{
	MidiEvent ret;
	ret.tick = tick;
	ret.firstByte = 0xff;
	int b1 = tempo & 0xff;
	tempo = tempo >> 8;
	int b2 = tempo & 0xff;
	tempo = tempo >> 8;
	int b3 = tempo & 0xff;
	ret.data.push_back(0x51);
	ret.data.push_back(b3);
	ret.data.push_back(b2);
	ret.data.push_back(b1);
	return ret;
}

void MidiEvent::writeDeltaTick(OutputStream& stream, int number)
{
	std::vector<bool> bits;
	uint64_t val = 0x1;
	bits.push_back((number & val) == val);
	for (int i = 1; i < 64; i++) {
		val = val << 1;
		bits.push_back((number & val) == val);
	}
	int first = 0;
	for (int i = 63; i >= 0; i--) {
		if (bits[i]) {
			first = i;
			break;
		}
	}
	// 何バイト必要か？
	int bytes = first / 7 + 1;
	for (int i = 1; i <= bytes; i++) {
		int num = 0;
		long count = 0x80;
		for (int j = (bytes - i + 1) * 7 - 1; j >= (bytes - i + 1) * 7 - 6 - 1; j--) {
			count = count >> 1;
			if (bits[j]) {
				num += count;
			}
		}
		if (i != bytes) {
			num += 0x80;
		}
		stream.write(num);
	}
}

tick_t MidiEvent::readDeltaTick(InputStream& stream)
{
	tick_t ret = 0;
	const tick_t mask = ~(tick_t)0 - (tick_t)0xFF + (tick_t)0x80;
	while (1) {
		int i = stream.read();
		if (i < 0) {
			break;
		}
		int d = i;
		ret = (mask & (ret << 7)) | (d & 0x7f);
		if ((d & 0x80) == 0x00) {
			break;
		}
	}
	return ret;
}

MidiEvent MidiEvent::read(InputStream& stream, tick_t& last_tick, uint8_t& last_status_byte)
{
	tick_t delta_tick = readDeltaTick(stream);
	last_tick += delta_tick;
	int first_byte = stream.read();
	if (first_byte < 0x80) {
		// ランニングステータスが適用される
		int64_t pos = stream.getPointer();
		stream.seek(pos - 1);
		first_byte = last_status_byte;
	} else {
		last_status_byte = first_byte;
	}
	int ctrl = first_byte & 0xf0;
	if (ctrl == 0x80 || ctrl == 0x90 || ctrl == 0xA0 || ctrl == 0xB0 || ctrl == 0xE0 || first_byte == 0xF2) {
		// 3byte使用するチャンネルメッセージ：
		//     0x8*: ノートオフ
		//     0x9*: ノートオン
		//     0xA*: ポリフォニック・キープレッシャ
		//     0xB*: コントロールチェンジ
		//     0xE*: ピッチベンドチェンジ
		// 3byte使用するシステムメッセージ
		//     0xF2: ソングポジション・ポインタ
		MidiEvent me;
		me.tick = last_tick;
		me.firstByte = first_byte;
		me.data.clear();
		me.data.push_back(0xff & stream.read());
		me.data.push_back(0xff & stream.read());
		return me;
	} else if (ctrl == 0xC0 || ctrl == 0xD0 || first_byte == 0xF1 || first_byte == 0xF3) {
		// 2byte使用するチャンネルメッセージ
		//     0xC*: プログラムチェンジ
		//     0xD*: チャンネルプレッシャ
		// 2byte使用するシステムメッセージ
		//     0xF1: クォータフレーム
		//     0xF3: ソングセレクト
		MidiEvent me;
		me.tick = last_tick;
		me.firstByte = first_byte;
		me.data.clear();
		me.data.push_back(0xff & stream.read());
		return me;
	} else if (first_byte == 0xF6) {
		// 1byte使用するシステムメッセージ
		//     0xF6: チューンリクエスト
		//     0xF7: エンドオブエクスクルーシブ（このクラスではF0ステータスのSysExの一部として取り扱う）
		//     0xF8: タイミングクロック
		//     0xFA: スタート
		//     0xFB: コンティニュー
		//     0xFC: ストップ
		//     0xFE: アクティブセンシング
		//     0xFF: システムリセット
		MidiEvent me;
		me.tick = last_tick;
		me.firstByte = first_byte;
		me.data.clear();
		return me;
	} else if (first_byte == 0xff) {
		// メタイベント
		int meta_event_type = stream.read();
		tick_t meta_event_length = readDeltaTick(stream);
		MidiEvent me;
		me.tick = last_tick;
		me.firstByte = first_byte;
		me.data.clear();
		me.data.push_back(meta_event_type);
		for (int i = 0; i < meta_event_length; i++) {
			me.data.push_back(stream.read());
		}
		return me;
	} else if (first_byte == 0xf0) {
		// f0ステータスのSysEx
		MidiEvent me;
		me.tick = last_tick;
		me.firstByte = first_byte;
		int sysex_length = (int)readDeltaTick(stream);
		me.data.clear();
		for (int i = 0; i < sysex_length + 1; i++) {
			me.data.push_back(stream.read());
		}
		return me;
	} else if (first_byte == 0xf7) {
		// f7ステータスのSysEx
		MidiEvent me;
		me.tick = last_tick;
		me.firstByte = first_byte;
		int sysex_length = (int)readDeltaTick(stream);
		me.data.clear();
		for (int i = 0; i < sysex_length; i++) {
			me.data.push_back(stream.read());
		}
		return me;
	} else {
		throw ParseException("don't know how to process first_byte: 0x" + StringUtil::toString(first_byte, 16));
	}
}

bool MidiEvent::compare(MidiEvent const& a, MidiEvent const& b)
{
	return (a.compareTo(b) < 0);
}

LIBVSQ_END_NAMESPACE
