/**
 * FileInputStream.hpp
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
#ifndef __FileInputStream_hpp__
#define __FileInputStream_hpp__

#include "vsqglobal.hpp"
#include "InputStream.hpp"
#include <stdio.h>
#include <string>

VSQ_BEGIN_NAMESPACE

/**
 * @brief ファイルからの読み込みストリーム
 */
class FileInputStream : public InputStream{
private:
    FILE *fileHandle;

public:
    explicit FileInputStream( const std::string &filePath ){
        fileHandle = fopen( filePath.c_str(), "rb" );
    }

    /**
     * @brief 1 バイトを読み込む
     * @return 読み込んだバイト値
     */
    int read(){
        return fgetc( fileHandle );
    }

    /**
     * @brief バッファーに読み込む
     * @param buffer 読み込んだデータを格納するバッファー
     * @param startIndex 読み込んだデータを格納するオフセット
     * @param length 読み込む長さ
     */
    void read( char *buffer, int64_t startIndex, int64_t length ){
        fread( buffer + sizeof( char ) * startIndex, sizeof( char ), length, fileHandle );
    }

    /**
     * @brief ファイルポインターを移動する
     * @param position ファイルポインター
     */
    void seek( int64_t position ){
        fpos_t pos = position;
        fsetpos( fileHandle, &pos );
    }

    /**
     * @brief ファイルポインターを取得する
     * @return ファイルポインター
     */
    int64_t getPointer(){
        fpos_t result;
        fgetpos( fileHandle, &result );
        return (int64_t)result;
    }

    /**
     * @brief ストリームを閉じる
     */
    void close(){
        fclose( fileHandle );
        fileHandle = 0;
    }
};

VSQ_END_NAMESPACE

#endif // __FileInputStream_hpp__
