#include "TextStream.h"
#include <sstream>
#include <iostream>

using namespace std;
using namespace VSQ_NS;

TextStream::TextStream()
{
    _array = NULL;
    _arrayLength = 0;
    _length = 0;
    _position = -1;
}

TextStream::~TextStream()
{
    close();
}

int TextStream::getPointer()
{
    return _position;
}

void TextStream::setPointer( int value )
{
    _position = value;
}

const string TextStream::get()
{
    _position++;
    string ret;
    ret += _array[_position];
    return ret;
}

const std::string TextStream::readLine()
{
    ostringstream sb;
    // '\n'が来るまで読み込み
    while( _position + 1 < _length ){
        _position++;
        char c = _array[_position];
        if( c == '\n' || c == 0 ){
            break;
        }
        sb << c;
    }
    return sb.str();
}

bool TextStream::ready()
{
    if( 0 <= _position + 1 && _position + 1 < _length ){
        return true;
    }else{
        return false;
    }
}

void TextStream::_ensureCapacity( int length )
{
    if( length > _arrayLength ){
        _array = (char *)::realloc( _array, (length + 1) * sizeof( char ) );
        for( int i = _arrayLength; i <= length; i++ ){
            _array[i] = 0;
        }
        _arrayLength = length;
    }
}

TextStream &TextStream::write( const string &str )
{
    int len = str.size();
    int newSize = _position + 1 + len;
    int offset = _position + 1;
    _ensureCapacity( newSize );
    const char *ptr = str.c_str();
    for( int i = 0; i < len; i++ ){
        _array[offset + i] = ptr[i];
    }
    _position += len;
    _length = ::max( _length, newSize );
    return *this;
}

TextStream &TextStream::writeLine( const string &str )
{
    int len = str.size();
    int offset = _position + 1;
    int newSize = offset + len + 1;
    _ensureCapacity( newSize );
    for( int i = 0; i < len; i++ ){
        _array[offset + i] = str[i];
    }
    _array[offset + len] = '\n';
    _position += len + 1;
    _length = ::max( _length, newSize );
    return *this;
}

void TextStream::close()
{
    if( _array ){
        free( _array );
        _array = NULL;
    }
    _length = 0;
}

const string TextStream::toString()
{
    string ret;
    if( _array ){
        ret += _array;
    }
    return ret;
}
