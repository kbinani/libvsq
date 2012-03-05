/**
 * VibratoBPList.cpp
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
#include <boost/lexical_cast.hpp>
#include <math.h>
#include <algorithm>
#include "StringUtil.h"
#include "VibratoBPList.h"

using namespace std;
using namespace VSQ_NS;

VibratoBPList::VibratoBPList( const string &textNum, const string &textBPX, const string &textBPY )
{
    int num = 0;
    try{
        num = boost::lexical_cast<int>( textNum );
    }catch( boost::bad_lexical_cast & ){
        num = 0;
    }
    vector<string> bpx = StringUtil::explode( ",", textBPX );
    vector<string> bpy = StringUtil::explode( ",", textBPY );
    int actNum = ::min( num, ::min( (int)bpx.size(), (int)bpy.size() ) );
    if( actNum > 0 ){
        double *x = new double[actNum]();
        int *y = new int[actNum]();
        try{
            for( int i = 0; i < actNum; i++ ){
                x[i] = boost::lexical_cast<double>( bpx[i] );
                y[i] = boost::lexical_cast<int>( bpy[i] );
            }
        }catch( boost::bad_lexical_cast & ){
            delete [] x;
            delete [] y;
            x = NULL;
            y = NULL;
        }

        if( x && y ){
            for( int i = 0; i < actNum; i++ ){
                _list.push_back( VibratoBP( x[i], y[i] ) );
            }
            std::sort( _list.begin(), _list.end(), VibratoBP::compare );
            delete [] x;
            delete [] y;
        }
    }
}

VibratoBPList::VibratoBPList( const vector<double> &x, const vector<int> &y )
{
    int len = ::min( x.size(), y.size() );
    for( int i = 0; i < len; i++ ){
        _list.push_back( VibratoBP( x[i], y[i] ) );
    }
    std::sort( _list.begin(), _list.end(), VibratoBP::compare );
}

int VibratoBPList::getValueAt( double x, int defaultValue ) const
{
    if( _list.size() <= 0 ){
        return defaultValue;
    }
    int index = -1;
    for( int i = 0; i < _list.size(); i++ ){
        if( x < _list[i].x ){
            break;
        }
        index = i;
    }
    if( index == -1 ){
        return defaultValue;
    }else{
        return _list[index].y;
    }
}

int VibratoBPList::size() const
{
    return (int)_list.size();
}

const VibratoBP VibratoBPList::get( int index ) const
{
    return _list[index];
}

void VibratoBPList::set(int index, const VibratoBP value)
{
    _list[index] = value;
}

const string VibratoBPList::getData() const
{
    ostringstream ret;
    for( int i = 0; i < _list.size(); i++ ){
        if( 0 < i ){
            ret << ",";
        }
        ret << _list[i].x << "=" << _list[i].y;
    }
    return ret.str();
}

void VibratoBPList::setData( const string &value )
{
    _list.clear();
    vector<string> spl = StringUtil::explode( ",", value );
    vector<string>::iterator i;
    for( i = spl.begin(); i != spl.end(); ++i ){
        vector<string> spl2 = StringUtil::explode( "=", (*i) );
        if( spl2.size() >= 2 ){
            double x;
            int y;
            try{
                x = boost::lexical_cast<double>( spl2[0] );
                y = boost::lexical_cast<int>( spl2[1] );
            }catch( boost::bad_lexical_cast & ){
                continue;
            }
            _list.push_back( VibratoBP( x, y ) );
        }
    }
    std::sort( _list.begin(), _list.end(), VibratoBP::compare );
}
