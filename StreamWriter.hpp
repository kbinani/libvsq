/**
 * StreamWriter.hpp
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
#include "./TextOutputStream.hpp"
#include <string>

VSQ_BEGIN_NAMESPACE

class OutputStream;

/**
 * @brief ファイルへの出力を行う TextOutputStream の実装
 */
class StreamWriter : public TextOutputStream
{
private:
	OutputStream* stream;
	OutputStream* deleteInDestructor;

public:
	/**
	 * @brief 出力先のファイルパスを指定して初期化する
	 * @param filePath 出力先のファイルパス
	 */
	explicit StreamWriter(std::string const& filePath);

	/**
	 * @brief Initialize writer by stream.
	 * @param  stream  A stream. This stream is automatically closed
	 *     when 'close' is called (however, not to be deleted)
	 */
	explicit StreamWriter(OutputStream* stream);

	~StreamWriter();

	void close();

	void write(std::string const& text) override;

	void writeLine(std::string const& text) override;
};

VSQ_END_NAMESPACE
