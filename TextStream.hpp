/**
 * TextStream.h
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
#ifndef __TextStream_h__
#define __TextStream_h__

#include "vsqglobal.hpp"
#include "TextOutputStream.hpp"
#include <string>
#include <cmath>
#include <sstream>

VSQ_BEGIN_NAMESPACE

using namespace std;

/**
 * 文字列への読み書きストリーム
 * @class table
 * @name TextStream
 */
class TextStream : public TextOutputStream{
private:
    /**
     * 書き込まれたデータを保持するバッファー
     * @var table
     * @access private
     */
    char *_array;

    /**
     * _array の現在の長さ
     */
    int _arrayLength;

    /**
     * データの現在の長さ
     * @var int
     * @access private
     */
    int _length;// = 0;

    /**
     * 現在の読み書き位置
     * @var int
     * @access private
     */
    int _position;// = -1;

public:
    explicit TextStream(){
        _array = NULL;
        _arrayLength = 0;
        _length = 0;
        _position = -1;
    }

    ~TextStream(){
        close();
    }

    /**
     * @brief 現在の読み書き位置を取得する
     * @return (int) 現在の読み書き位置
     */
    int getPointer(){
        return _position;
    }

    /**
     * @brief 現在の読み書き位置を設定する
     * @param value (int) 設定する読み書き位置
     */
    void setPointer( int value ){
        _position = value;
    }

    /**
     * @brief 現在の読み込み位置から 1 文字を読み込み、読み書き位置を一つ進める
     * @return (string) 読み込んだ文字
     */
    const std::string get(){
        _position++;
        string ret;
        ret += _array[_position];
        return ret;
    }

    /**
     * @brief 現在の読み込み位置から、改行またはファイル末端まで読み込む
     * @return (string) 読み込んだ文字列
     */
    const std::string readLine(){
        ostringstream sb;
        // '\n'が来るまで読み込み
        while( _position + 1 < _length ){
            _position++;
            char c = _array[_position];
            if( c == (char)0x0A || c == 0 ){
                break;
            }
            sb << c;
        }
        return sb.str();
    }

    /**
     * @brief テキストストリームが読み込み可能な状態かどうかを返す
     * @return (boolean) 読み込み可能であれば <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    bool ready(){
        if( 0 <= _position + 1 && _position + 1 < _length ){
            return true;
        }else{
            return false;
        }
    }

    /**
     * @brief 文字列をストリームに書きこむ
     * @param str (string) 書きこむ文字列
     */
    void write( const std::string &str ){
        int len = str.size();
        int newSize = _position + 1 + len;
        int offset = _position + 1;
        _ensureCapacity( newSize );
        const char *ptr = str.c_str();
        for( int i = 0; i < len; i++ ){
            _array[offset + i] = ptr[i];
        }
        _position += len;
        _length = std::max( _length, newSize );
    }

    /**
     * @brief 文字列をストリームに書きこむ。末尾に改行文字を追加する
     * @param str (string) 書きこむ文字列
     */
    void writeLine( const std::string &str ){
        int len = str.size();
        int offset = _position + 1;
        int newSize = offset + len + 1;
        _ensureCapacity( newSize );
        for( int i = 0; i < len; i++ ){
            _array[offset + i] = str[i];
        }
        _array[offset + len] = (char)0x0A;
        _position += len + 1;
        _length = std::max( _length, newSize );
    }

    /**
     * @brief ストリームを閉じる
     */
    void close(){
        if( _array ){
            free( _array );
            _array = NULL;
        }
        _length = 0;
    }

    /**
     * @brief ストリームに書きこまれた文字列を連結し、返す
     * @return (string) 文字列
     */
    const std::string toString(){
        string ret;
        if( _array ){
            ret += _array;
        }
        return ret;
    }

private:
    /**
     * @brief 内部のバッファー容量を確保する
     * @param length (int) 確保したいバッファー容量
     */
    void _ensureCapacity( int length ){
        if( length > _arrayLength ){
            _array = (char *)::realloc( _array, (length + 1) * sizeof( char ) );
            for( int i = _arrayLength; i <= length; i++ ){
                _array[i] = 0;
            }
            _arrayLength = length;
        }
    }
};

VSQ_END_NAMESPACE

#endif
