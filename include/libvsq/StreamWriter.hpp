/**
 * @file StreamWriter.hpp
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
#include <memory>
#include <string>

LIBVSQ_BEGIN_NAMESPACE

class OutputStream;

/**
 * @brief ファイルへの出力を行う TextOutputStream の実装.
 */
class StreamWriter : public TextOutputStream
{
private:
	std::unique_ptr<OutputStream> stream;

public:
	/**
	 * @brief 出力先のファイルパスを指定して初期化する.
	 * @param filePath 出力先のファイルパス.
	 */
	explicit StreamWriter(std::string const& filePath);

	/**
	 * @brief \~japanese-en 出力先のストリームを指定して初期化する.
	 *        \~english Initialize writer by stream.
	 * @param stream \~japanese-en 出力先のストリーム.
	 *                             このストリームは StreamWriter::close が呼ばれる時自動で OutputStream::close される.
	 *                             また, このストリームの所有権は StreamWriter に移動する.
	 *               \~english A stream. This stream is automatically OutputStream::close 'ed
	 *                         when StreamWriter::close is called.
	 *                         The ownership of @a stream object will be moved to this object.
	 */
	explicit StreamWriter(OutputStream* stream);

	~StreamWriter();

	void close() override;

	void write(std::string const& text) override;

	void writeLine(std::string const& text) override;
};

LIBVSQ_END_NAMESPACE
