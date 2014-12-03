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
	 * @todo tempo と change_tempo を渡せるようにする.
	 */
	void write(Sequence const& sequence, TextOutputStream& stream, std::string const& software);

protected:
	void printStyledNote(
		TextOutputStream& writer,
		int tickStart,
		int tickLength,
		int note,
		std::vector<Tempo>& tempoInsert,
		std::string lyric,
		std::map<std::string, bool>& alteredContext,
		bool tieStartRequired,
		bool tieStopRequired);

	void printStyledNoteCor(
		TextOutputStream& writer,
		int tickLength,
		int note,
		std::string lyric,
		std::map<std::string, bool>& alteredContext,
		bool tieStartRequired,
		bool tieStopRequired,
		std::string type);
};

VSQ_END_NAMESPACE
