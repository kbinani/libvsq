/**
 * FileInputStream.hpp
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
#include "./InputStream.hpp"
#include <fstream>

VSQ_BEGIN_NAMESPACE

/**
 * @brief ファイルからの読み込みストリーム.
 */
class FileInputStream : public InputStream
{
private:
	std::ifstream stream;

public:
	explicit FileInputStream(std::string const& filePath);

	/**
	 * @brief 1 バイトを読み込む.
	 * @return 読み込んだバイト値.
	 */
	int read();

	/**
	 * @brief バッファーに読み込む.
	 * @param buffer 読み込んだデータを格納するバッファー.
	 * @param startIndex 読み込んだデータを格納するオフセット.
	 * @param length 読み込む長さ.
	 */
	int read(char* buffer, int64_t startIndex, int64_t length);

	/**
	 * @brief ファイルポインターを移動する.
	 * @param position ファイルポインター.
	 */
	void seek(int64_t position);

	/**
	 * @brief ファイルポインターを取得する.
	 * @return ファイルポインター.
	 */
	int64_t getPointer();

	/**
	 * @brief ストリームを閉じる.
	 */
	void close();
};

VSQ_END_NAMESPACE
