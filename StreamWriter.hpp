/**
 * StreamWriter.hpp
 * Copyright © 2012 kbinani
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
#ifndef __StreamWriter_hpp__
#define __StreamWriter_hpp__

#include "vsqglobal.hpp"
#include "TextOutputStream.hpp"
#include "OutputStream.hpp"
#include "FileOutputStream.hpp"
#include <fstream>

VSQ_BEGIN_NAMESPACE

/**
 * @brief ファイルへの出力を行う TextOutputStream の実装
 */
class StreamWriter : public TextOutputStream
{
private:
	VSQ_NS::OutputStream* stream;
	VSQ_NS::OutputStream* deleteInDestructor;

public:
	/**
	 * @brief 出力先のファイルパスを指定して初期化する
	 * @param filePath 出力先のファイルパス
	 */
	explicit StreamWriter(const std::string& filePath)
	{
		try {
			stream = new FileOutputStream(filePath);
		} catch (OutputStream::IOException) {
			throw TextOutputStream::IOException();
		}
		deleteInDestructor = stream;
	}

	/**
	 * @brief Initialize writer by stream.
	 * @param  stream  A stream. This stream is automatically closed
	 *     when 'close' is called (however, not to be deleted)
	 */
	explicit StreamWriter(OutputStream* stream)
	{
		this->stream = stream;
		this->deleteInDestructor = 0;
	}

	~StreamWriter()
	{
		close();
	}

	void close()
	{
		if (stream) {
			stream->close();
			stream = 0;
		}
		if (deleteInDestructor) {
			delete deleteInDestructor;
			deleteInDestructor = 0;
		}
	}

	void write(const std::string& text)
	{
		if (stream) { stream->write(text.c_str(), 0, text.length()); }
	}

	void writeLine(const std::string& text)
	{
		write(text);
		if (stream) { stream->write(0x0A); }
	}
};

VSQ_END_NAMESPACE

#endif
