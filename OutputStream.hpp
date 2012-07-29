/**
 * OutputStream.h
 * Copyright ﾂｩ 2012 kbinani
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
#ifndef OUTPUTSTREAM_H
#define OUTPUTSTREAM_H

#include "vsqglobal.hpp"

VSQ_BEGIN_NAMESPACE

class OutputStream
{
public:
    virtual void write( int value ) = 0;

    virtual void write( const char *buffer, int startIndex, int length ) = 0;

    virtual void seek( int position ) = 0;

    virtual int getPointer() = 0;

    virtual void close() = 0;
};

VSQ_END_NAMESPACE

#endif // OUTPUTSTREAM_H
