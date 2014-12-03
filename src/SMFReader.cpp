/**
 * SMFReader.cpp
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
#include "../include/libvsq/SMFReader.hpp"
#include "../include/libvsq/InputStream.hpp"
#include "../include/libvsq/BitConverter.hpp"

LIBVSQ_BEGIN_NAMESPACE

void SMFReader::read(InputStream& stream, std::vector<std::vector<MidiEvent>>& dest, int& format, int& timeFormat)
{
	// ヘッダ
	char byte4[4] = { 0 };
	stream.read(byte4, 0, 4);
	if (BitConverter::makeUInt32BE(byte4) != 0x4d546864) {
		throw ParseException();
	}

	// データ長
	stream.read(byte4, 0, 4);

	// フォーマット
	stream.read(byte4, 0, 2);
	format = BitConverter::makeUInt16BE(byte4);

	// トラック数
	stream.read(byte4, 0, 2);
	int tracks = (int)BitConverter::makeUInt16BE(byte4);

	// 時間分解能
	stream.read(byte4, 0, 2);
	timeFormat = BitConverter::makeUInt16BE(byte4);

	// 各トラックを読込み
	dest.clear();
	for (int track = 0; track < tracks; track++) {
		dest.push_back(std::vector<MidiEvent>());

		// ヘッダー
		stream.read(byte4, 0, 4);
		if (BitConverter::makeUInt32BE(byte4) != 0x4d54726b) {
			throw ParseException();// "header error; MTrk" );
		}

		// チャンクサイズ
		stream.read(byte4, 0, 4);
		long size = (long)BitConverter::makeUInt32BE(byte4);
		int64_t startpos = stream.getPointer();

		// チャンクの終わりまで読込み
		tick_t tick = 0;
		uint8_t last_status_byte = 0x00;
		while (stream.getPointer() < startpos + size) {
			MidiEvent mi = MidiEvent::read(stream, tick, last_status_byte);
			dest[track].push_back(mi);
		}
	}
}

LIBVSQ_END_NAMESPACE
