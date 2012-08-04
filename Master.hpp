/**
 * Master.hpp
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
#ifndef __Master_hpp__
#define __Master_hpp__

#include "vsqglobal.hpp"
#include "TextStream.hpp"
#include "StringUtil.hpp"
#include <vector>
#include <sstream>

VSQ_BEGIN_NAMESPACE

/**
 * @brief VSQ ファイルのメタテキストの [Master] に記録される内容を取り扱うクラス
 */
class Master{
public:
    /**
     * @brief プリメジャーの長さ(小節数)
     */
    int preMeasure;

    explicit Master(){
        this->preMeasure = 1;
    }

    /**
     * @brief プリメジャーを指定し、初期化を行う
     * @param preMeasure プリメジャーの長さ(小節数)
     */
    explicit Master( int preMeasure ){
        this->preMeasure = preMeasure;
    }

    /**
     * @brief テキストストリームから読み込むことで初期化を行う
     * @param stream 読み込むテキストストリーム
     * @param lastLine 読み込んだ最後の行。
     */
    explicit Master( TextStream &stream, std::string &lastLine ){
        this->preMeasure = 0;
        lastLine = stream.readLine();
        while( lastLine.find( "[" ) == std::string::npos ){
            std::vector<std::string> spl = StringUtil::explode( "=", lastLine );
            if( spl[0] == "PreMeasure" ){
                this->preMeasure = StringUtil::parseInt( spl[1] );
            }
            if( !stream.ready() ){
                break;
            }
            lastLine = stream.readLine();
        }
    }

    /**
     * @brief コピーを作成する
     * @return このオブジェクトのコピー
     */
    Master clone() const{
        return Master( this->preMeasure );
    }

    /**
     * @brief テキストストリームに出力する
     * @param stream (TextStream) 出力先
     */
    void write( TextStream &stream ) const{
        stream.writeLine( "[Master]" );
        std::ostringstream oss;
        oss << "PreMeasure=" << this->preMeasure;
        stream.writeLine( oss.str() );
    }
};

VSQ_END_NAMESPACE

#endif
