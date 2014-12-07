/**
 * @file VSQFileReader.hpp
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
#include "./Handle.hpp"
#include "./Event.hpp"
#include "./Track.hpp"
#include "./MidiEvent.hpp"
#include "./PublicForUnitTest.hpp"

LIBVSQ_BEGIN_NAMESPACE

class Sequence;
class InputStream;
class TimesigList;
class TempoList;
class Master;
class Mixer;

/**
 * @brief VSQ ファイルのシーケンスを読み込み, Sequence オブジェクトを作成するクラス.
 */
class VSQFileReader
{
public:
	VSQFileReader();

	~VSQFileReader();

	/**
	 * @brief 読み込みストリームから, VSQ ファイルを読み込む.
	 * @param sequence 読み込み結果の格納先.
	 * @param stream 読み込みストリーム.
	 * @param encoding メタテキストのテキストエンコーディング(無視される. 現在はShift_JIS固定).
	 * @todo メタテキストに記録されず, NRPNにのみ出力される値がなんらかあったはずなので, それの読み込みもサポートするかどうか検討すること.
	 */
	void read(Sequence& sequence, InputStream& stream, std::string const& encoding);

LIBVSQ_PRIVATE_BUT_PUBLIC_FOR_UNITTEST:
	/**
	 * @brief テキストストリームからイベントの内容を読み込み初期化する.
	 * @param sr 読み込み対象.
	 * @param last_line 読み込んだ最後の行が格納される.
	 * @param[out] lyricHandleIndex
	 * @param[out] singerHandleIndex
	 * @param[out] vibratoHandleIndex
	 * @param[out] noteHeadHandleIndex
	 */
	Event parseEvent(TextStream& stream, std::string& lastLine, EventType& type, int& lyricHandleIndex, int& singerHandleIndex, int& vibratoHandleIndex, int& noteHeadHandleIndex);

	/**
	 * @brief テキストストリームからハンドルの内容を読み込み初期化する.
	 * @param stream 読み込み元のテキストストリーム.
	 * @param index <code>index</code> フィールドの値.
	 * @param lastLine 読み込んだ最後の行. テーブルの ["value"] に文字列が格納される.
	 */
	Handle parseHandle(TextStream& stream, int index, std::string& lastLine);

private:
	class Impl;
	std::unique_ptr<Impl> _impl;
};

LIBVSQ_END_NAMESPACE
