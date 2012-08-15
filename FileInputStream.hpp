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
#include <fstream>
#include <string>

VSQ_BEGIN_NAMESPACE

/**
 * @brief ファイルからの読み込みストリーム
 */
class FileInputStream : public InputStream{
private:
    std::ifstream stream;

public:
    explicit FileInputStream( const std::string &filePath )
        : stream( filePath.c_str(), std::ios::binary )
    {
    }

    /**
     * @brief 1 バイトを読み込む
     * @return 読み込んだバイト値
     */
    int read(){
        return stream.get();
    }

    /**
     * @brief バッファーに読み込む
     * @param buffer 読み込んだデータを格納するバッファー
     * @param startIndex 読み込んだデータを格納するオフセット
     * @param length 読み込む長さ
     */
    void read( char *buffer, int64_t startIndex, int64_t length ){
        stream.get( buffer + sizeof( char ) * startIndex, length + 1 );
    }

    /**
     * @brief ファイルポインターを移動する
     * @param position ファイルポインター
     */
    void seek( int64_t position ){
        stream.seekg( position );
    }

    /**
     * @brief ファイルポインターを取得する
     * @return ファイルポインター
     */
    int64_t getPointer(){
        return (int64_t)stream.tellg();
    }

    /**
     * @brief ストリームを閉じる
     */
    void close(){
        stream.close();
    }
};

VSQ_END_NAMESPACE

#endif // __FileInputStream_hpp__
