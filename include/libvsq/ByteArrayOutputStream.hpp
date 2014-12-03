/**
 * ByteArrayOutputStream.hpp
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

#include "./BasicTypes.hpp"
#include "./OutputStream.hpp"
#include <cstdint>
#include <string>
#include <vector>

LIBVSQ_BEGIN_NAMESPACE

/**
 * @brief データがバイト配列に書き込まれるストリームの実装.
 */
class ByteArrayOutputStream : public OutputStream
{
protected:
	/**
	 * @brief 確保するバッファー・ブロックのサイズ(バイト単位).
	 */
	static const int UNIT_BUFFER_LENGTH = 512;

private:
	/**
	 * @brief 現在のファイルポインタ.
	 */
	int64_t _pointer;

	/**
	 * @brief 書き込み先のバイト列.
	 */
	std::vector<char> _array;

	/**
	 * @brief 確保されたバイト列の現在の長さ.
	 */
	int64_t _arrayLength;

	/**
	 * @brief 書き込み済みバイト数.
	 */
	int64_t _length;

public:
	/**
	 * @brief 初期化を行う.
	 */
	ByteArrayOutputStream();

	~ByteArrayOutputStream();

	/**
	 * @brief 指定されたバイト値をストリームに書きこむ.
	 * @param byte 書きこむバイト値.
	 */
	void write(int byte) override;

	/**
	 * @brief 指定された配列の, 指定した範囲のバイト値をストリームに書きこむ.
	 * @param array 書きこむバイト列が格納された配列.
	 * @param startIndex 書き込み開始位置.
	 * @param length 書き込むバイト値の個数.
	 */
	void write(char const* array, int64_t startIndex, int64_t length) override;

	/**
	 * @brief バイト列を文字列に変換する.
	 * @return 変換された文字列.
	 */
	std::string toString() const;

	/**
	 * @brief 現在のファイルポインタを取得する.
	 * @return 現在のファイルポインタ.
	 */
	int64_t getPointer() override;

	/**
	 * @brief ファイルポインタを指定した位置に変更する.
	 * @param position 新しいポインタ値.
	 */
	void seek(int64_t position) override;

	/**
	 * @brief ストリームを閉じる.
	 */
	void close() override;

private:
	/**
	 * @brief バッファーを指定した長さまで確保する.
	 * @param length 確保するバッファーの長さ.
	 */
	void ensureBufferLength(int length);
};

LIBVSQ_END_NAMESPACE
