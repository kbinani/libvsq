/**
 * StreamWriter.hpp
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
#ifndef __StreamWriter_hpp__
#define __StreamWriter_hpp__

#include "vsqglobal.hpp"
#include "TextOutputStream.hpp"
#include <fstream>

VSQ_BEGIN_NAMESPACE

class StreamWriter : public TextOutputStream{
private:
    std::ofstream stream;

public:
    explicit StreamWriter( const std::string &filePath ){
        stream.open( filePath.c_str(), std::ios::binary );
    }

    void close(){
        stream.close();
    }

    void write( const std::string &text ){
        stream << text;
    }

    void writeLine( const std::string &text ){
        static char lf = (char)0x0A;
        stream << text << lf;
    }
};

VSQ_END_NAMESPACE

#endif
