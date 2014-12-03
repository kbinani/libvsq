/**
 * NoteNumberUtil.hpp
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
#include <string>

VSQ_BEGIN_NAMESPACE

class NoteNumberUtil
{
public:
	/**
	 * @brief ノートのオクターブ, 変化記号を除いた部分の文字列表記を調べる.
	 * 例：C4 => "C", D#4 => "D"
	 * @param note ノート番号.
	 */
	static std::string getNoteStringBase(int note);

	/**
	 * @brief ノート#のオクターブ部分の表記を調べる.
	 * 例：C4 => 4, D#4 => 4
	 * @param note ノート番号.
	 */
	static int getNoteOctave(int note);

	/**
	 * @brief C#4なら+1, C4なら0, Cb4なら-1.
	 * @param note ノート番号.
	 */
	static int getNoteAlter(int note);

private:
	NoteNumberUtil(); /* = delete */
};

VSQ_END_NAMESPACE
