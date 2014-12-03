/**
 * @file MusicXmlWriter.hpp
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
#include <map>
#include <memory>
#include <string>
#include <vector>

LIBVSQ_BEGIN_NAMESPACE

class Sequence;
class TextOutputStream;

/**
 * @brief Sequence を MusicXML ファイルに書き込む処理を行うクラス.
 */
class MusicXmlWriter
{
public:
	MusicXmlWriter();

	~MusicXmlWriter();

	/**
	 * @brief シーケンスを MusicXML として, 指定されたストリームに出力する.
	 * @param sequence 出力するシーケンス.
	 * @param stream 出力先のストリーム.
	 * @todo tempo と change_tempo を渡せるようにする.
	 */
	void write(Sequence const& sequence, TextOutputStream& stream, std::string const& software) const;

private:
	class Impl;
	std::unique_ptr<Impl> _impl;
};

LIBVSQ_END_NAMESPACE
