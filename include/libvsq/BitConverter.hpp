/**
 * @file BitConverter.hpp
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
#include <cstdint>
#include <vector>

LIBVSQ_BEGIN_NAMESPACE

/**
 * @brief バイト列と整数との相互変換を行うクラス.
 */
class BitConverter
{
public:
	/**
	 * @brief uint16 の整数値から, big endian のバイト列を取得する.
	 * @param value 変換する整数値.
	 * @return 変換後のバイト列.
	 */
	static std::vector<char> getBytesUInt16BE(uint16_t value);

	/**
	 * @brief uint32 の整数値から, big endian のバイト列を取得する.
	 * @param value 変換する整数値.
	 * @return 変換後のバイト列.
	 */
	static std::vector<char> getBytesUInt32BE(uint32_t value);

	/**
	 * @brief 4 バイトのバイト列を little endian とみなし, uint32 の整数値を取得する.
	 * @param bytes 変換元のバイト列.
	 * @return 変換後の整数値.
	 */
	static uint32_t makeUInt32LE(char bytes[4]);

	/**
	 * @brief 4 バイトのバイト列を big endian とみなし, uint32 の整数値を取得する.
	 * @param bytes 変換元のバイト列.
	 * @return 変換後の整数値.
	 */
	static uint32_t makeUInt32BE(char bytes[4]);

	/**
	 * @brief 2 バイトのバイト列を little endian とみなし, int16 の整数値を取得する.
	 * @param bytes 変換元のバイト列.
	 * @return 変換後の整数値.
	 */
	static int16_t makeInt16LE(char bytes[2]);

	/**
	 * @brief 2 バイトのバイト列を big endian とみなし, uint16 の整数値を取得する.
	 * @param bytes 変換元のバイト列.
	 * @return 変換後の整数値.
	 */
	static uint16_t makeUInt16BE(char bytes[2]);
};

LIBVSQ_END_NAMESPACE
