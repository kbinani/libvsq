/**
 * @file CP932Converter.hpp
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

#include <libvsq/Namespace.hpp>
#include <string>

LIBVSQ_BEGIN_NAMESPACE

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
};

LIBVSQ_END_NAMESPACE
