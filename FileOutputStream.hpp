/**
 * FileOutputStream.hpp
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
#ifndef __libvsq_FileOutputStream_hpp__
#define __libvsq_FileOutputStream_hpp__
#include "OutputStream.hpp"
#include <string>
#include <fstream>

VSQ_BEGIN_NAMESPACE

/**
 * @brief ファイルへの出力を行う、OutputStream の実装
 */
class FileOutputStream : public OutputStream{
private:
    /**
     * @brief 出力先のストリーム
     */
    std::ofstream stream;

public:
    /**
     * @brief 出力先のファイルパスを指定し、初期化する
     */
    explicit FileOutputStream( const std::string &filePath ){
        stream.open( filePath.c_str(), std::ios::binary );
    }

    ~FileOutputStream(){
        if( stream.is_open() ) stream.close();
    }

    void write( const char *buffer, int64_t startIndex, int64_t length ){
        stream.write( buffer + sizeof( char ) * startIndex, length );
    }

    void write( int value ){
        char c = 0xFF & value;
        stream.write( &c, 1 );
    }

    void seek( int64_t position ){
        stream.seekp( position, std::ios::beg );
    }

    int64_t getPointer(){
        return (int64_t)stream.tellp();
    }

    void close(){
        stream.close();
    }
};

VSQ_END_NAMESPACE

#endif
