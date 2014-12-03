/**
 * BitConverter.hpp
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
#include <vector>

VSQ_BEGIN_NAMESPACE

class BitConverter
{
public:
	static std::vector<char> getBytesUInt16BE(uint16_t value);

	static std::vector<char> getBytesUInt32BE(uint32_t value);

	/**
	 * @brief 4 バイトのデータを little endian とみなし, uint32 の整数値を取得する.
	 * @param bytes 変換元のデータ.
	 * @return 変換後の整数値.
	 */
	static uint32_t makeUInt32LE(char bytes[4]);

	/**
	 * @brief 4 バイトのデータを big endian とみなし, uint32 の整数値を取得する.
	 * @param bytes 変換元のデータ.
	 * @return 変換後の整数値.
	 */
	static uint32_t makeUInt32BE(char bytes[4]);

	/**
	 * @brief 2 バイトのデータを little endian とみなし, int16 の整数値を取得する.
	 * @param bytes 変換元のデータ.
	 * @return 変換後の整数値.
	 */
	static int16_t makeInt16LE(char bytes[2]);

	/**
	 * @brief 2 バイトのデータを big endian とみなし, uint16 の整数値を取得する.
	 * @param bytes 変換元のデータ.
	 * @return 変換後の整数値.
	 */
	static uint16_t makeUInt16BE(char bytes[2]);
};

VSQ_END_NAMESPACE
