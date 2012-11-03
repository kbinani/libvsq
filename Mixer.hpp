/**
 * Mixer.hpp
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
#ifndef __Mixer_hpp__
#define __Mixer_hpp__

#include "vsqglobal.hpp"
#include "MixerItem.hpp"
#include "StringUtil.hpp"
#include "TextStream.hpp"
#include <vector>

VSQ_BEGIN_NAMESPACE

/**
 * @brief VSQ ファイルのメタテキストの [Mixer] セクションに記録される内容を取り扱うクラス
 */
class Mixer{
public:
    /**
     * @brief MasterFader 値
     */
    int masterFeder;// = 0;

    /**
     * @brief MasterMute 値
     */
    int masterMute;// = 0;

    /**
     * @brief MasterPanpot 値
     */
    int masterPanpot;// = 0;

    /**
     * @brief OutputMode 値
     */
    int outputMode;// = 0;

    /**
     * @brief 各トラックのミキサー情報
     */
    std::vector<MixerItem> slave;

    /**
     * @brief 初期化を行う
     */
    explicit Mixer(){
        this->masterFeder = 0;
        this->masterMute = 0;
        this->masterPanpot = 0;
        this->outputMode = 0;
    }

    /**
     * @brief 各パラメータを指定し、初期化を行う
     * @param masterFeder MasterFader 値
     * @param masterPanpot MasterPanpot 値
     * @param masterMute MasterMute 値
     * @param outputMode OutputMode 値
     */
    explicit Mixer( int masterFeder, int masterPanpot, int masterMute, int outputMode ){
        this->masterFeder = masterFeder;
        this->masterMute = masterMute;
        this->masterPanpot = masterPanpot;
        this->outputMode = outputMode;
    }

    /**
     * @brief テキストストリームから読み込みを行い、初期化を行う
     * @param stream (TextStream) 読み込むテキストストリーム
     * @param lastLine (table) 読み込んだ最後の行。テーブルの ["value"] に文字列が格納される
     */
    explicit Mixer( TextStream &stream, std::string &lastLine ){
        this->masterFeder = 0;
        this->masterPanpot = 0;
        this->masterMute = 0;
        this->outputMode = 0;
        int tracks = 0;
        std::string buffer = "";
        lastLine = stream.readLine();
        while( lastLine.at( 0 ) != '[' ){
            std::vector<std::string> params = StringUtil::explode( "=", lastLine );
            if( params[0] == "MasterFeder" ){
                this->masterFeder = StringUtil::parseInt<int>( params[1] );
            }else if( params[0] == "MasterPanpot" ){
                this->masterPanpot = StringUtil::parseInt<int>( params[1] );
            }else if( params[0] == "MasterMute" ){
                this->masterMute = StringUtil::parseInt<int>( params[1] );
            }else if( params[0] == "OutputMode" ){
                this->outputMode = StringUtil::parseInt<int>( params[1] );
            }else if( params[0] == "Tracks" ){
                tracks = StringUtil::parseInt<int>( params[1] );
            }else{
                if( params[0].find( "Feder" ) == 0 ||
                    params[0].find( "Panpot" ) == 0 ||
                    params[0].find( "Mute" ) == 0  ||
                    params[0].find( "Solo" ) == 0 )
                {
                    buffer = buffer + params[0] + "=" + params[1] + "\n";
                }
            }
            if( !stream.ready() ){
                break;
            }
            lastLine = stream.readLine();
        }

        for( int i = 0; i < tracks; i++ ){
            this->slave.push_back( MixerItem( 0, 0, 0, 0 ) );
        }
        std::vector<std::string> spl = StringUtil::explode( "\n", buffer );
        for( int i = 0; i < spl.size(); i++ ){
            std::string ind = "";
            int index;
            std::vector<std::string> spl2 = StringUtil::explode( "=", spl[i] );
            if( spl2[0].find( "Feder" ) == 0 ){
                ind = spl2[0].substr( std::string( "Feder" ).size() );
                index = StringUtil::parseInt<int>( ind );
                this->slave[index].feder = StringUtil::parseInt<int>( spl2[1] );
            }else if( spl2[0].find( "Panpot" ) == 0 ){
                ind = spl2[0].substr( std::string( "Panpot" ).size() );
                index = StringUtil::parseInt<int>( ind );
                this->slave[index].panpot = StringUtil::parseInt<int>( spl2[1] );
            }else if( spl2[0].find( "Mute" ) == 0 ){
                ind = spl2[0].substr( std::string( "Mute" ).size() );
                index = StringUtil::parseInt<int>( ind );
                this->slave[index].mute = StringUtil::parseInt<int>( spl2[1] );
            }else if( spl2[0].find( "Solo" ) == 0 ){
                ind = spl2[0].substr( std::string( "Solo" ).size() );
                index = StringUtil::parseInt<int>( ind );
                this->slave[index].solo = StringUtil::parseInt<int>( spl2[1] );
            }
        }
    }

    /**
     * @brief コピーを作成する
     * @return このオブジェクトのコピー
     */
    Mixer clone() const{
        Mixer res( this->masterFeder, this->masterPanpot, this->masterMute, this->outputMode );
        for( int i = 0; i < this->slave.size(); i++ ){
            MixerItem item = this->slave[i];
            res.slave.push_back( item.clone() );
        }
        return res;
    }

    /**
     * @brief テキストストリームに出力する
     * @param stream (TextStream) 出力先のストリーム
     */
    void write( TextStream &stream ) const{
        stream.writeLine( "[Mixer]" );
        stream.writeLine( "MasterFeder=" + StringUtil::toString( this->masterFeder ) );
        stream.writeLine( "MasterPanpot=" + StringUtil::toString( this->masterPanpot ) );
        stream.writeLine( "MasterMute=" + StringUtil::toString( this->masterMute ) );
        stream.writeLine( "OutputMode=" + StringUtil::toString( this->outputMode ) );
        stream.writeLine( "Tracks=" + StringUtil::toString( this->slave.size() ) );
        for( int i = 0; i < this->slave.size(); ++i ){
            MixerItem item = this->slave[i];
            stream.writeLine( "Feder" + StringUtil::toString( i ) + "=" + StringUtil::toString( item.feder ) );
            stream.writeLine( "Panpot" + StringUtil::toString( i ) + "=" + StringUtil::toString( item.panpot ) );
            stream.writeLine( "Mute" + StringUtil::toString( i ) + "=" + StringUtil::toString( item.mute ) );
            stream.writeLine( "Solo" + StringUtil::toString( i ) + "=" + StringUtil::toString( item.solo ) );
        }
    }
};

VSQ_END_NAMESPACE

#endif
