/**
 * @file TextStream.hpp
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

#include "./TextOutputStream.hpp"
#include <vector>

LIBVSQ_BEGIN_NAMESPACE

/**
 * @brief 文字列への読み書きストリーム.
 */
class TextStream : public TextOutputStream
{
private:
	/**
	 * @brief 書き込まれたデータを保持するバッファー.
	 */
	std::vector<char> _array;

	/**
	 * @brief _array の現在の長さ.
	 */
	int _arrayLength;

	/**
	 * @brief データの現在の長さ.
	 */
	int _length;

	/**
	 * @brief 現在の読み書き位置.
	 */
	int _position;

public:
	TextStream();

	~TextStream();

	/**
	 * @brief 現在の読み書き位置を取得する.
	 * @return 現在の読み書き位置.
	 */
	int getPointer();

	/**
	 * @brief 現在の読み書き位置を設定する.
	 * @param value 設定する読み書き位置.
	 */
	void setPointer(int value);

	/**
	 * @brief 現在の読み込み位置から 1 文字を読み込み, 読み書き位置を一つ進める.
	 * @return 読み込んだ文字.
	 */
	std::string get();

	/**
	 * @brief 現在の読み込み位置から, 改行またはファイル末端まで読み込む.
	 * @return 読み込んだ文字列.
	 */
	std::string readLine();

	/**
	 * @brief テキストストリームが読み込み可能な状態かどうかを返す.
	 * @return 読み込み可能であれば <code>true</code> を, そうでなければ <code>false</code> を返す.
	 */
	bool ready() const;

	/**
	 * @brief 文字列をストリームに書きこむ.
	 * @param str 書きこむ文字列.
	 */
	void write(std::string const& str) override;

	/**
	 * @brief 文字列をストリームに書きこむ. 末尾に改行文字を追加する.
	 * @param str 書きこむ文字列.
	 */
	void writeLine(std::string const& str) override;

	/**
	 * @brief ストリームを閉じる.
	 */
	void close() override;

	/**
	 * @brief ストリームに書きこまれた文字列を連結し, 返す.
	 * @return 文字列.
	 */
	std::string toString() const;

private:
	/**
	 * @brief 内部のバッファー容量を確保する.
	 * @param length 確保したいバッファー容量.
	 */
	void _ensureCapacity(int length);
};

LIBVSQ_END_NAMESPACE
