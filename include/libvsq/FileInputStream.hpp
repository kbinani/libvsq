/**
 * @file FileInputStream.hpp
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

LIBVSQ_BEGIN_NAMESPACE

/**
 * @brief ファイルからの読み込みストリーム.
 */
class FileInputStream : public InputStream
{
private:
	std::ifstream stream;

public:
	/**
	 * @brief ファイルパスを指定してストリームを開く.
	 * @param filePath 開くファイルのファイルパス.
	 */
	explicit FileInputStream(std::string const& filePath);

	/**
	 * @brief 1 バイトを読み込む.
	 * @return 読み込んだバイト値.
	 */
	int read() override;

	/**
	 * @brief バッファーに読み込む.
	 * @param buffer 読み込んだデータを格納するバッファー.
	 * @param startIndex 読み込んだデータを格納するオフセット.
	 * @param length 読み込む長さ.
	 */
	int read(char* buffer, int64_t startIndex, int64_t length) override;

	/**
	 * @brief ファイルポインターを移動する.
	 * @param position ファイルポインター.
	 */
	void seek(int64_t position) override;

	/**
	 * @brief ファイルポインターを取得する.
	 * @return ファイルポインター.
	 */
	int64_t getPointer() override;

	/**
	 * @brief ストリームを閉じる.
	 */
	void close() override;
};

LIBVSQ_END_NAMESPACE
