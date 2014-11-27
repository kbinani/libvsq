/**
 * TextOutputStream.hpp
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
#include <exception>
#include <string>

VSQ_BEGIN_NAMESPACE

/**
 * @brief テキスト出力のためのストリーム
 */
class TextOutputStream
{
public:
	/**
	 * @brief IO 例外
	 */
	class IOException : std::exception
	{};

public:
	virtual ~TextOutputStream()
	{}

	/**
	 * @brief ストリームを閉じる
	 */
	virtual void close() = 0;

	/**
	 * @brief 文字列をストリームに書きこむ
	 * @param text 書きこむ文字列
	 */
	virtual void write(std::string const& text) = 0;

	/**
	 * @brief 文字列をストリームに書きこむ。末尾に改行文字を追加する
	 * @param line 書きこむ文字列
	 */
	virtual void writeLine(std::string const& line) = 0;
};

VSQ_END_NAMESPACE
