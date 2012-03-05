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

#include "vsqglobal.h"
#include <string>

VSQ_BEGIN_NAMESPACE

/**
 * 文字列への読み書きストリーム
 * @class table
 * @name TextStream
 */
class TextStream
{
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
    TextStream();

    ~TextStream();

    /**
     * 現在の読み書き位置を取得する
     * @return (int) 現在の読み書き位置
     */
    int getPointer();

    /**
     * 現在の読み書き位置を設定する
     * @param value (int) 設定する読み書き位置
     */
    void setPointer( int value );

    /**
     * 現在の読み込み位置から 1 文字を読み込み、読み書き位置を一つ進める
     * @return (string) 読み込んだ文字
     */
    const std::string get();

    /**
     * 現在の読み込み位置から、改行またはファイル末端まで読み込む
     * @return (string) 読み込んだ文字列
     */
    const std::string readLine();

    /**
     * テキストストリームが読み込み可能な状態かどうかを返す
     * @return (boolean) 読み込み可能であれば <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    bool ready();

    /**
     * 文字列をストリームに書きこむ
     * @param str (string) 書きこむ文字列
     */
    TextStream &write( const std::string &str );

    /**
     * 文字列をストリームに書きこむ。末尾に改行文字を追加する
     * @param str (string) 書きこむ文字列
     */
    TextStream &writeLine( const std::string &str );

    /**
     * ストリームを閉じる
     */
    void close();

    /**
     * ストリームに書きこまれた文字列を連結し、返す
     * @return (string) 文字列
     */
    const std::string toString();

private:
    /**
     * 内部のバッファー容量を確保する
     * @param length (int) 確保したいバッファー容量
     * @access private
     */
    void _ensureCapacity( int _length );
};

VSQ_END_NAMESPACE

#endif
