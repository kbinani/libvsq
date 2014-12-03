/**
 * MusicXmlWriter.hpp
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
#include "./Tempo.hpp"
#include <string>
#include <vector>
#include <map>

VSQ_BEGIN_NAMESPACE

class Sequence;
class TextOutputStream;

class MusicXmlWriter
{
public:
	/**
	 * @brief シーケンスを MusicXML として, 指定されたストリームに出力する.
	 * @param sequence 出力するシーケンス.
	 * @param stream 出力先のストリーム.
	 * @todo change_tempo を changeTempo に名前を変える.
	 * @todo sw を stream に名前変える.
	 * @todo vsq を sequence に名前変える.
	 * @todo tempo と change_tempo を渡せるようにする.
	 */
	void write(Sequence const* sequence, TextOutputStream* sw, std::string const& software);

protected:
	void printStyledNote(
		TextOutputStream* writer,
		int clock_start,
		int clock_length,
		int note,
		std::vector<Tempo>& tempoInsert,
		std::string lyric,
		std::map<std::string, bool>& altered_context,
		bool tie_start_required,
		bool tie_stop_required);

	void printStyledNoteCor(
		TextOutputStream* writer,
		int clock_length,
		int note,
		std::string lyric,
		std::map<std::string, bool>& altered_context,
		bool tie_start_required,
		bool tie_stop_required,
		std::string type);
};

VSQ_END_NAMESPACE
