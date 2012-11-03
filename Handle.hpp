/**
 * Handle.h
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
#ifndef __Handle_h__
#define __Handle_h__

#include <string>
#include <boost/format.hpp>
#include "vsqglobal.hpp"
#include "HandleType.hpp"
#include "ArticulationType.hpp"
#include "VibratoBPList.hpp"
#include "Lyric.hpp"
#include "TextStream.hpp"

VSQ_BEGIN_NAMESPACE

/**
 * @brief ハンドルを取り扱います。ハンドルにはLyricHandle、VibratoHandle、SingerHandleおよびNoteHeadHandleがある
 * @todo get, set両方あるプロパティについては、fieldにしてよいのでは
 */
class Handle
{
public:
    /**
     * @brief メタテキストに出力されるこのオブジェクトの ID
     * @todo VSQへの書き出し時にのみ使っているので、うまいことして削除する
     */
    int index;

    /**
     * @brief ハンドルを特定するための文字列
     */
    std::string iconId;

    /**
     * @brief ハンドルの名前
     */
    std::string ids;

    /**
     * @brief ハンドルのオリジナル
     */
    int original;

    /**
     * @brief 歌手の歌唱言語を表す番号(バンクセレクト)。歌手ハンドルでのみ使われる
     */
    int language;

    /**
     * @brief 歌手の種類を表す番号(プログラムチェンジ)。歌手ハンドルでのみ使われる
     */
    int program ;

    /**
     * @brief 歌詞・発音記号列の前後にクォーテーションマークを付けるかどうか
     */
    bool addQuotationMark;

    /**
     * @brief キャプション
     */
    std::string caption;

    /**
     * @brief Dyn の開始値
     */
    int startDyn;

    /**
     * @brief Dyn の終了値
     */
    int endDyn;

    /**
     * @brief Depth
     */
    int depth;

    /**
     * @brief Duration
     */
    int duration;

    /**
     * @brief Rate の開始値
     */
    int startRate;

    /**
     * @brief Depth の開始値
     */
    int startDepth;

    /**
     * @brief Depth のカーブ
     */
    VSQ_NS::VibratoBPList depthBP;

    /**
     * @brief Rate のカーブ
     */
    VSQ_NS::VibratoBPList rateBP;

    /**
     * @brief Dyn のカーブ
     */
    VSQ_NS::VibratoBPList dynBP;

protected:
    VSQ_NS::HandleType::HandleTypeEnum _type;

    VSQ_NS::ArticulationType::ArticulationTypeEnum _articulation;

    /**
     * @brief 歌詞一覧
     */
    std::vector<VSQ_NS::Lyric> _lyrics;

    /**
     * @brief 長さ
     */
    int _length;

public:
    /**
     * @brief 強弱記号の場合の、IconId の最初の5文字。
     * @return IconId の接頭辞
     */
    inline static const std::string getIconIdPrefixDynaff(){
        return "$0501";
    }

    /**
     * @brief クレッシェンドの場合の、IconId の最初の5文字。
     * @return IconId の接頭辞
     */
    inline static const std::string getIconIdPrefixCrescend(){
        return "$0502";
    }

    /**
     * @brief デクレッシェンドの場合の、IconId の最初の5文字。
     * @return IconId の接頭辞
     */
    inline static const std::string getIconIdPrefixDecrescend(){
        return "$0503";
    }

    /**
     * @param type ハンドルの種類
     */
    explicit Handle( VSQ_NS::HandleType::HandleTypeEnum type = VSQ_NS::HandleType::UNKNOWN ){
        init();
        _type = type;
        if( _type == HandleType::DYNAMICS ){
            _init_icon_dynamics();
        }else if( type == HandleType::NOTE_HEAD ){
            _articulation = ArticulationType::NOTE_ATTACK;
        }else if( type == HandleType::VIBRATO ){
            _init_vibrato();
        }else if( type == HandleType::LYRIC ){
            _init_lyric();
        }
    }

    /**
     * @brief articulation の種類を取得する
     * @return articulation の種類
     */
    VSQ_NS::ArticulationType::ArticulationTypeEnum getArticulation() const{
        return _articulation;
    }

    /**
     * @brief このハンドルが強弱記号を表すものかどうかを表すブール値を取得する
     * @return このオブジェクトが強弱記号を表すものであれば <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    bool isDynaffType() const{
        return iconId.find( Handle::getIconIdPrefixDynaff() ) == 0;
    }

    /**
     * @brief このハンドルがクレッシェンドを表すものかどうかを表すブール値を取得する
     * @return このオブジェクトがクレッシェンドを表すものであれば <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    bool isCrescendType() const{
        return iconId.find( Handle::getIconIdPrefixCrescend() ) == 0;
    }

    /**
     * @brief このハンドルがデクレッシェンドを表すものかどうかを表すブール値を取得する
     * @return このオブジェクトがデクレッシェンドを表すものであれば <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    bool isDecrescendType() const{
        return iconId.find( Handle::getIconIdPrefixDecrescend() ) == 0;
    }

    /**
     * @brief Tick 単位の長さを取得する
     * @return
     */
    VSQ_NS::tick_t getLength() const{
        return _length;
    }

    /**
     * @brief 長さを設定する
     * @param value Tick単位の長さ
     */
    void setLength( VSQ_NS::tick_t value ){
        _length = value;
    }

    /**
     * @brief 指定した位置にある歌詞を取得する
     * @param index 取得する要素のインデックス(最初のインデックスは0)
     * @return 歌詞
     */
    const VSQ_NS::Lyric getLyricAt( int index ) const{
        return _lyrics[index];
    }

    /**
     * @brief 指定した位置にある歌詞を指定した要素で置き換える
     * @param index 置き換える要素のインデックス(最初のインデックスは0)
     * @param value 置き換える要素
     */
    void setLyricAt( int index, const VSQ_NS::Lyric &value ){
        if( _lyrics.size() < index + 1 ){
            int remain = index + 1 - _lyrics.size();
            for( int i = 0; i < remain; i++ ){
                _lyrics.push_back( Lyric( "", "" ) );
            }
        }
        _lyrics[index] = value;
    }

    /**
     * @brief 歌詞を追加する
     * @param lyric 追加する歌詞
     */
    void addLyric( Lyric lyric ){
        _lyrics.push_back( lyric );
    }

    /**
     * @brief 歌詞の個数を返す
     * @return 歌詞の個数
     */
    int getLyricCount() const{
        return _lyrics.size();
    }

    /**
     * @brief Display String 値を取得する
     * @return Display String 値
     */
    const std::string getDisplayString() const{
        return ids + caption;
    }

    /**
     * @brief ハンドルのタイプを取得する
     * @return ハンドルのタイプ
     */
    VSQ_NS::HandleType::HandleTypeEnum getHandleType() const{
        return _type;
    }

    /**
     * @brief コピーを作成する
     * @return (Handle) このオブジェクトのコピー
     */
    Handle clone() const{
        if( _type == HandleType::DYNAMICS ){
            Handle ret( HandleType::DYNAMICS );
            ret.iconId = iconId;
            ret.ids = ids;
            ret.original = original;
            ret.caption = caption;
            ret.startDyn = startDyn;
            ret.endDyn = endDyn;
            //if( 0 != _dynBP ){
                ret.dynBP = dynBP.clone();
            //}
            ret.setLength( getLength() );
            return ret;
        }else if( _type == HandleType::LYRIC ){
            Handle result( HandleType::LYRIC );
            result.index = index;
            for( int i = 0; i < _lyrics.size(); i++ ){
                Lyric buf = _lyrics[i].clone();
                result._lyrics.push_back( buf );
            }
            return result;
        }else if( _type == HandleType::NOTE_HEAD ){
            Handle result( HandleType::NOTE_HEAD );
            result.index = index;
            result.iconId = iconId;
            result.ids = ids;
            result.original = original;
            result.caption = caption;
            result.setLength( getLength() );
            result.duration = duration;
            result.depth = depth;
            return result;
        }else if( _type == HandleType::SINGER ){
            Handle ret( HandleType::SINGER );
            ret.caption = caption;
            ret.iconId = iconId;
            ret.ids = ids;
            ret.index = index;
            ret.language = language;
            ret.setLength( _length );
            ret.original = original;
            ret.program = program;
            return ret;
        }else if( _type == HandleType::VIBRATO ){
            Handle result( HandleType::VIBRATO );
            result.index = index;
            result.iconId = iconId;
            result.ids = ids;
            result.original = original;
            result.caption = caption;
            result.setLength( getLength() );
            result.startDepth = startDepth;
            //if( 0 != _depthBP ){
                result.depthBP = depthBP.clone();
            //}
            result.startRate = startRate;
            //if( 0 != _rateBP ){
                result.rateBP = rateBP.clone();
            //}
            return result;
        }else{
            return Handle( HandleType::UNKNOWN );
        }
    }

    /**
     * @brief ハンドル指定子（例えば"h#0123"という文字列）からハンドル番号を取得する
     * @param s ハンドル指定子
     * @return ハンドル番号
     */
    static int getHandleIndexFromString( const std::string &s ){
        vector<string> spl = StringUtil::explode( "#", s );
        return StringUtil::parseInt<int>( spl[1] );
    }

protected:
    void init(){
        _type = HandleType::UNKNOWN;
        _articulation = ArticulationType::NONE;
        index = 0;
        iconId = "";
        ids = "";
        original = 0;
        caption = "";
        _length = 0;
        startDepth = 0;
        startRate = 0;
        language = 0;
        program = 0;
        duration = 0;
        depth = 0;
        startDyn = 0;
        endDyn = 0;
        addQuotationMark = true;
    }

    /**
     * @brief ビブラートハンドルとして初期化を行う
     */
    void _init_vibrato(){
        _articulation = ArticulationType::VIBRATO;
        index = 0;
        iconId = "$04040000";
        ids = "";
        original = 0;
        startRate = 64;
        startDepth = 64;
        rateBP = VibratoBPList();
        depthBP = VibratoBPList();
    }

    /**
     * @brief 強弱記号ハンドルとして初期化を行う
     */
    void _init_icon_dynamics(){
        _articulation = ArticulationType::DYNAFF;
        iconId = "";
        ids = "";
        original = 0;
    }

    /**
     * @brief 歌詞ハンドルとして初期化を行う
     */
    void _init_lyric(){
        index = 0;
        _lyrics.clear();
    }
};

VSQ_END_NAMESPACE

#endif
