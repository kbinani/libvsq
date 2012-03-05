/**
 * VibratoBP.cpp
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
#include "VibratoBP.h"

using namespace VSQ_NS;

VibratoBP::VibratoBP()
{
    x = 0.0;
    y = 0;
}

VibratoBP::VibratoBP( double x, int y )
{
    this->x = x;
    this->y = y;
}

int VibratoBP::compareTo( const VibratoBP &item )
{
    return compareCore( *this, item );
}

bool VibratoBP::compare( const VibratoBP &a, const VibratoBP &b )
{
    return compareCore( a, b ) < 0;
}

int VibratoBP::compareCore( const VibratoBP &a, const VibratoBP &b )
{
    double v = a.x - b.x;
    if( v > 0.0 ){
        return 1;
    }else if( v < 0.0 ){
        return -1;
    }
    return 0;
}
