/**
 * OutputStream.hpp
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
#ifndef __OutputStream_hpp__
#define __OutputStream_hpp__

#include "vsqglobal.hpp"
#include <exception>
#include <stdint.h>

VSQ_BEGIN_NAMESPACE

/**
 * @brief 書き込みストリーム
 */
class OutputStream
{
public:
	/**
	 * @brief IO 例外
	 */
	class IOException : public std::exception
	{
	};

public:
	/**
	 * @brief 1 バイト書き込む
	 * @param value 書き込む値
	 */
	virtual void write(int value) = 0;

	/**
	 * @brief バッファーを書き込む
	 * @param buffer 書き込むバッファー
	 * @param startIndex 書き込みを開始するインデックス
	 * @param length 書き込む長さ
	 */
	virtual void write(const char* buffer, int64_t startIndex, int64_t length) = 0;

	/**
	 * @brief ポインターを移動する
	 * @param position ポインターの位置。ファイル先頭からの位置を指定する
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

#endif // __OutputStream_hpp__
