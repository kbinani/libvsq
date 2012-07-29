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
    static vector<int> getBytesUInt16BE( uint16_t value ){
        vector<int> result;
        result.push_back( ((0xff00 & value) >> 8) & 0xff );
        result.push_back( ((0x00ff & value)) & 0xff );
        return result;
    }

    static vector<int> getBytesUInt32BE( uint32_t value ){
        vector<int> result;
        result.push_back( ((0xff000000 & value) >> 24) & 0xff );
        result.push_back( ((0x00ff0000 & value) >> 16) & 0xff );
        result.push_back( ((0x0000ff00 & value) >> 8) & 0xff );
        result.push_back( ((0x000000ff & value)) & 0xff );
        return result;
    }
};

VSQ_END_NAMESPACE

#endif
