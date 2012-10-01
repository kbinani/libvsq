/**
 * BitConverter.hpp
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
#ifndef __BitConverter_h__
#define __BitConverter_h__

#include "vsqglobal.hpp"
#include <vector>
#include <stdint.h>

VSQ_BEGIN_NAMESPACE

using namespace std;

class BitConverter{
public:
    static vector<char> getBytesUInt16BE( uint16_t value ){
        vector<char> result;
        result.push_back( ((0xff00 & value) >> 8) & 0xff );
        result.push_back( ((0x00ff & value)) & 0xff );
        return result;
    }

    static vector<char> getBytesUInt32BE( uint32_t value ){
        vector<char> result;
        result.push_back( ((0xff000000 & value) >> 24) & 0xff );
        result.push_back( ((0x00ff0000 & value) >> 16) & 0xff );
        result.push_back( ((0x0000ff00 & value) >> 8) & 0xff );
        result.push_back( ((0x000000ff & value)) & 0xff );
        return result;
    }

    /**
     * @brief 4 バイトのデータを little endian とみなし、uint32 の整数値を取得する
     * @param bytes 変換元のデータ
     * @return 変換後の整数値
     */
    static uint32_t makeUInt32LE( char bytes[4] ){
        return (0xff000000 & (bytes[3] << 24)) |
               (0x00ff0000 & (bytes[2] << 16)) |
               (0x0000ff00 & (bytes[1] << 8)) |
               (0x000000ff & bytes[0]);
    }

    /**
     * @brief 4 バイトのデータを big endian とみなし、uint32 の整数値を取得する
     * @param bytes 変換元のデータ
     * @return 変換後の整数値
     */
    static uint32_t makeUInt32BE( char bytes[4] ){
        return (0xff000000 & (bytes[0] << 24)) |
               (0x00ff0000 & (bytes[1] << 16)) |
               (0x0000ff00 & (bytes[2] << 8)) |
               (0x000000ff & bytes[3]);
    }

    /**
     * @brief 2 バイトのデータを big endian とみなし、uint16 の整数値を取得する
     * @param bytes 変換元のデータ
     * @return 変換後の整数値
     */
    static uint16_t makeUInt16BE( char bytes[2] ){
        return (0xff00 & (bytes[0] << 8)) |
               (0x00ff & bytes[1]);
    }
};

VSQ_END_NAMESPACE

#endif
