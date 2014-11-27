/**
 * InputStream.hpp
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
#include <cstdint>

VSQ_BEGIN_NAMESPACE

/**
 * @brief 読み込みストリーム
 */
class InputStream
{
public:
	virtual ~InputStream()
	{}

	/**
	 * @brief 1 バイトを読み込む
	 * @return 読み込んだバイト値。ストリームの末尾に達した場合は負の値を返す
	 */
	virtual int read() = 0;

	/**
	 * @brief バッファーに読み込む
	 * @param buffer 読み込んだデータを格納するバッファー
	 * @param startIndex 読み込んだデータを格納するオフセット
	 * @param length 読み込む長さ
	 * @return 読み込んだ長さ
	 */
	virtual int read(char* buffer, int64_t startIndex, int64_t length) = 0;

	/**
	 * @brief ファイルポインターを移動する
	 * @param position ファイルポインター
	 */
	virtual void seek(int64_t position) = 0;

	/**
	 * @brief ファイルポインターを取得する
	 * @return ファイルポインター
	 */
	virtual int64_t getPointer() = 0;

	/**
	 * @brief ストリームを閉じる
	 */
	virtual void close() = 0;
};

VSQ_END_NAMESPACE
