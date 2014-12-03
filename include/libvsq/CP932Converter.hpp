/**
 * CP932Converter.hpp
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
#include <string>
#include <vector>

VSQ_BEGIN_NAMESPACE

/**
 * @brief UTF8 から CP932 への文字コード変換ユーティリティ.
 */
class CP932Converter
{
public:
	/**
	 * @brief UTF8 の文字列を CP932 の文字列に変換する.
	 * @param utf8 変換前の文字列.
	 * @return 変換後の文字列.
	 */
	static std::string convertFromUTF8(std::string const& utf8);

	/**
	 * @brief CP932のバイト列をUTF8の文字列に変換します.
	 * @param cp932 変換する CP932 のバイト列.
	 * @return 変換後の UTF8 文字列.
	 */
	static std::string convertToUTF8(std::string const& cp932);

protected:
	/**
	 * @brief UTF8 の文字列を unicode のバイト列に変換する.
	 * @param s 変換前の文字列.
	 * @return 変換後の unicode のバイト列.
	 */
	static std::vector<std::vector<int>> _getUnicodeBytesFromUTF8String(std::string const& s);

	/**
	 * @brief UTF8 のバイト列を Unicode のバイト列に変換する.
	 * @param utf8 UTF8のバイト列.
	 * @return Unicode のバイト列.
	 */
	static std::vector<int> _getUnicodeBytesFromUTF8Bytes(std::vector<int> const& utf8);

private:
	/**
	 * @brief Unicode から CP932 への変換テーブルを初期化する.
	 */
	static void initializeUnicodeToCp932Dictionary(int dict[256][256][2]);

	/**
	 * @brief CP932 から UTF8 への変換テーブルを初期化する.
	 */
	static void initializeCP932ToUTF8Dictionary(int dict[0xFFFF]);
};

VSQ_END_NAMESPACE
