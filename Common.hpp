/**
 * Common.hpp
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
#ifndef __Common_hpp__
#define __Common_hpp__

#include "vsqglobal.hpp"
#include <string>
#include <vector>
#include "DynamicsMode.hpp"
#include "PlayMode.hpp"
#include "TextStream.hpp"
#include "StringUtil.hpp"

VSQ_BEGIN_NAMESPACE

/**
 * @brief VSQ ファイルのメタテキストの [Common] セクションに記録される内容を格納するクラス
 */
class Common{
public:
    /**
     * @brief トラックの歌声合成エンジンのバージョンを表す文字列
     */
    std::string version;// = "DSB301";

    /**
     * @brief トラックの名前
     */
    std::string name;// = "Miku";

    /**
     * @brief ダイナミクスモード
     */
    DynamicsMode::DynamicsModeEnum dynamicsMode;// = DynamicsMode::EXPERT;

    /**
     * @brief 再生モード
     */
    PlayMode::PlayModeEnum playMode;// = PlayMode::PLAY_WITH_SYNTH;

    /**
     * @brief (不明)
     */
    std::string color;// = "179,181,123";

private:
    /**
     * @brief PlayModeがOff(-1)にされる直前に，PlayAfterSynthかPlayWithSynthのどちらが指定されていたかを記憶しておく．
     */
    PlayMode::PlayModeEnum _lastPlayMode;// = PlayMode::PLAY_WITH_SYNTH;

public:
    /**
     * @brief 初期化を行う
     */
    explicit Common(){
        init();
    }

    /**
     * @brief 初期化を行う
     * @param stream 読み込み元のテキストストリーム
     * @param lastLine 読み込んだ最後の行。テーブルの ["value"] に文字列が格納される
     */
    explicit Common( TextStream &stream, std::string &lastLine ){
        init();
        this->version = "";
        this->name = "";
        this->color = "0,0,0";
        this->dynamicsMode = DynamicsMode::STANDARD;
        this->playMode = PlayMode::PLAY_WITH_SYNTH;
        lastLine = stream.readLine();
        while( lastLine.find( "[" ) != 0 ){
            std::vector<std::string> spl = StringUtil::explode( "=", lastLine );
            std::string search = spl[0];
            if( search == "Version" ){
                this->version = spl[1];
            }else if( search == "Name" ){
                this->name = spl[1];
            }else if( search == "Color" ){
                this->color = spl[1];
            }else if( search == "DynamicsMode" ){
                this->dynamicsMode = (DynamicsMode::DynamicsModeEnum)StringUtil::parseInt( spl[1] );
            }else if( search == "PlayMode" ){
                this->playMode = (PlayMode::PlayModeEnum)StringUtil::parseInt( spl[1] );
            }
            if( !stream.ready() ){
                break;
            }
            lastLine = stream.readLine();
        }
    }

    /**
     * @brief 初期化を行う
     * @param name トラック名
     * @param r 赤(意味は不明)
     * @param g 緑(意味は不明)
     * @param b 青(意味は不明)
     * @param dynamicsMode シーケンスの Dynamics モード
     * @param playMode シーケンスの Play モード
     */
    explicit Common( std::string name, int r, int g, int b, DynamicsMode::DynamicsModeEnum dynamicsMode, PlayMode::PlayModeEnum playMode ){
        init();
        this->version = "DSB301";
        this->name = name;
        ostringstream oss;
        oss << r << "," << g << "," << b;
        this->color = oss.str();
        this->dynamicsMode = dynamicsMode;
        this->playMode = playMode;
    }

    /**
     * @brief コピーを作成する
     * @return このインスタンスのコピー
     */
    Common clone() const{
        std::vector<std::string> spl = StringUtil::explode( ",", this->color );
        int r = StringUtil::parseInt( spl[0] );
        int g = StringUtil::parseInt( spl[1] );
        int b = StringUtil::parseInt( spl[2] );
        Common result( this->name, r, g, b, this->dynamicsMode, this->playMode );
        result.version = this->version;
        result._lastPlayMode = this->_lastPlayMode;
        return result;
    }

    /**
     * @brief テキストストリームに出力する
     * @param stream (TextStream) 出力先のストリーム
     */
    void write( TextStream &stream ) const{
        stream.writeLine( "[Common]" );
        stream.writeLine( "Version=" + this->version );
        stream.writeLine( "Name=" + this->name );
        stream.writeLine( "Color=" + this->color );
        ostringstream oss;
        oss << "DynamicsMode=" << (int)this->dynamicsMode;
        stream.writeLine( oss.str() );

        oss.str( "" );
        oss << "PlayMode=" << (int)this->playMode;
        stream.writeLine( oss.str() );
    }

private:
    void init(){
        this->version = "DSB301";
        this->name = "Miku";
        this->dynamicsMode = DynamicsMode::EXPERT;
        this->playMode = PlayMode::PLAY_WITH_SYNTH;
        this->color = "179,181,123";
        this->_lastPlayMode = PlayMode::PLAY_WITH_SYNTH;
    }
};

VSQ_END_NAMESPACE

#endif
