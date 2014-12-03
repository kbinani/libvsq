/**
 * VSQFileReader.hpp
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

LIBVSQ_BEGIN_NAMESPACE

class Sequence;
class InputStream;
class TimesigList;
class TempoList;
class Master;
class Mixer;

/**
 * @brief VSQ ファイルからのシーケンス読み込みを行うクラス.
 */
class VSQFileReader
{
protected:
	class TentativeHandle : public Handle
	{
	public:
		explicit TentativeHandle(HandleType type) :
			Handle(type)
		{}

		void setHandleType(HandleType type)
		{
			_type = type;
		}

		void setLyrics(std::vector<Lyric> const& lyrics)
		{
			_lyrics = lyrics;
		}
	};

	class TentativeEvent : public Event
	{
		typedef Event base_type;
	public:
		int singerHandleIndex;
		int lyricHandleIndex;
		int vibratoHandleIndex;
		int noteHeadHandleIndex;

	public:
		void setEOS(bool value)
		{
			isEos = value;
		}

		void setType(EventType type)
		{
			base_type::type(type);
		}
	};

	class TentativeTrack : public Track
	{
	public:
		TentativeTrack() : Track()
		{}

		void setCommon(Common const& value)
		{
			_common = value;
		}

		std::map<std::string, std::string> getSectionNameMap() const
		{
			return Track::getSectionNameMap();
		}
	};

public:
	/**
	 * @brief 読み込みストリームから, VSQ ファイルを読み込む.
	 * @param sequence 読み込み結果の格納先.
	 * @param stream 読み込みストリーム.
	 * @param encoding メタテキストのテキストエンコーディング(無視される. 現在はShift_JIS固定).
	 * @todo メタテキストに記録されず, NRPNにのみ出力される値がなんらかあったはずなので, それの読み込みもサポートするかどうか検討すること.
	 */
	void read(Sequence& sequence, InputStream& stream, std::string const& encoding);

protected:
	/**
	 * @brief テキストストリームからイベントの内容を読み込み初期化する.
	 * @param sr 読み込み対象.
	 * @param value
	 * @param last_line 読み込んだ最後の行が格納される.
	 */
	TentativeEvent parseEvent(TextStream& stream, std::string& lastLine);

	/**
	 * @brief テキストストリームからハンドルの内容を読み込み初期化する.
	 * @param stream 読み込み元のテキストストリーム.
	 * @param index <code>index</code> フィールドの値.
	 * @param lastLine 読み込んだ最後の行. テーブルの ["value"] に文字列が格納される.
	 */
	Handle parseHandle(TextStream& stream, int index, std::string& lastLine);

private:
	/**
	 * @brief MIDIイベントリストから拍子変更イベントを取り出し, TimesigList のインスタンスを初期化する.
	 */
	void parseTimesigList(std::vector<MidiEvent> const& midiEventList, TimesigList& timesigList);

	/**
	 * @brief MIDIイベントリストからテンポ変更イベントを取り出し, TempoList のインスタンスを初期化する.
	 */
	static void parseTempoList(std::vector<MidiEvent> const& midiEventList, TempoList& tempoList);

	/**
	 * @brief MIDI イベントのリストから, VOCALOIDメタテキストとトラック名を取得する.
	 * @param midi_event
	 * @param encoding
	 * @param[out] sw 取得したメタテキストの格納先.
	 * @param[out] track_name 取得したトラック名.
	 */
	void getMetatextByMidiEventList(std::vector<MidiEvent> const& midiEventList, std::string const& encoding, TextStream& stream, std::string& trackName);

	/**
	 * @brief メタテキストが格納されたストリームから, トラックを初期化する.
	 * @param 読み込むストリーム.
	 * @param[out] master 読み込まれた Master 情報.
	 * @param[out] mixer 読み込まれた Mixer 情報.
	 */
	Track getTrackByTextStream(TextStream& stream, Master* master = 0, Mixer* mixer = 0);
};

LIBVSQ_END_NAMESPACE
