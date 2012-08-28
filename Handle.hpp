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
 */
class Handle
{
public:
    /**
     * @brief メタテキストに出力されるこのオブジェクトの ID
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

protected:
    VSQ_NS::HandleType::HandleTypeEnum _type;

    VSQ_NS::ArticulationType::ArticulationTypeEnum _articulation;

    /**
     * @brief 歌詞一覧
     */
    std::vector<VSQ_NS::Lyric> _lyrics;

    /**
     * @brief キャプション
     */
    std::string _caption;

    /**
     * @brief 長さ
     */
    int _length;

    /**
     * @brief Depth の開始値
     */
    int _startDepth;

    /**
     * @brief Depth のカーブ
     */
    VSQ_NS::VibratoBPList _depthBP;

    /**
     * @brief Rate の開始値
     */
    int _startRate;

    /**
     * @brief Rate のカーブ
     */
    VSQ_NS::VibratoBPList _rateBP;

    /**
     * @brief Duration
     */
    int _duration;

    /**
     * @brief Depth
     */
    int _depth;

    /**
     * @brief Dyn の開始値
     */
    int _startDyn;

    /**
     * @brief Dyn の終了値
     */
    int _endDyn;

    /**
     * @brief Dyn のカーブ
     */
    VSQ_NS::VibratoBPList _dynBP;

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
     * @brief キャプションを取得する
     * @return キャプション
     */
    const std::string getCaption() const{
        return _caption;
    }

    /**
     * @brief キャプションを設定する
     * @param value キャプション
     */
    void setCaption( const std::string &value ){
        _caption = value;
    }

    /**
     * @brief DYN の開始値を取得する
     * @return DYN の開始値
     */
    int getStartDyn() const{
        return _startDyn;
    }

    /**
     * @brief DYN の開始値を設定する
     * @param value DYN の開始値
     */
    void setStartDyn( int value ){
        _startDyn = value;
    }

    /**
     * @brief DYN の終了値を取得する
     * @return DYN の終了値
     */
    int getEndDyn() const{
        return _endDyn;
    }

    /**
     * @brief DYN の終了値を設定する
     * @param value DYN の終了値
     */
    void setEndDyn( int value ){
        _endDyn = value;
    }

    /**
     * @brief DYN カーブを取得する
     * @return DYN カーブ
     */
    const VSQ_NS::VibratoBPList getDynBP() const{
        return _dynBP;
    }

    /**
     * @brief DYN カーブを設定する
     * @param value DYN カーブ
     */
    void setDynBP( const VSQ_NS::VibratoBPList &value ){
        _dynBP = value;
    }

    /**
     * @brief Depth 値を取得する
     * @return  Depth 値
     */
    int getDepth() const{
        return _depth;
    }

    /**
     * @brief Depth 値を設定する
     * @param value Depth 値
     */
    void setDepth( int value ){
        _depth = value;
    }

    /**
     * @brief Duration 値を取得する
     * @return Duration 値
     */
    int getDuration() const{
        return _duration;
    }

    /**
     * @brief Duration 値を設定する
     * @param value Duration 値
     */
    void setDuration( int value ){
        _duration = value;
    }

    /**
     * @brief Rate のビブラートカーブを取得する
     * @return Rate のビブラートカーブ
     */
    const VSQ_NS::VibratoBPList getRateBP() const{
        return _rateBP;
    }

    /**
     * @brief Rate のビブラートカーブを設定する
     * @param value 設定するビブラートカーブ
     */
    void setRateBP( const VSQ_NS::VibratoBPList &value ){
        _rateBP = value;
    }

    /**
     * @brief Depth のビブラートカーブを取得する
     * @return Depth のビビラートカーブ
     */
    const VSQ_NS::VibratoBPList getDepthBP() const{
        return _depthBP;
    }

    /**
     * @brief Depth のビブラートカーブを設定する
     * @param value 設定するビブラートカーブ
     */
    void setDepthBP( const VSQ_NS::VibratoBPList &value ){
        _depthBP = value;
    }

    /**
     * @brief Rate の開始値を取得する
     * @return Rate の開始値
     */
    int getStartRate() const{
        return _startRate;
    }

    /**
     * @brief Rate の開始値を設定する
     * @param value Rate の開始値
     */
    void setStartRate( int value ){
        _startRate = value;
    }

    /**
     * @brief Depth の開始値を取得する
     * @return Depth の開始値
     */
    int getStartDepth() const{
        return _startDepth;
    }

    /**
     * @brief Depth の開始値を設定する
     * @param value Depth の開始値
     */
    void setStartDepth( int value ){
        _startDepth = value;
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
        return ids + _caption;
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
            ret.setCaption( getCaption() );
            ret.setStartDyn( getStartDyn() );
            ret.setEndDyn( getEndDyn() );
            //TODO: _dynBPの型をVibratoBPList*に変える
            //if( 0 != _dynBP ){
                ret.setDynBP( _dynBP.clone() );
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
            result.setCaption( getCaption() );
            result.setLength( getLength() );
            result.setDuration( getDuration() );
            result.setDepth( getDepth() );
            return result;
        }else if( _type == HandleType::SINGER ){
            Handle ret( HandleType::SINGER );
            ret._caption = _caption;
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
            result.setCaption( _caption );
            result.setLength( getLength() );
            result.setStartDepth( _startDepth );
            //TODO: _depthBPの型をVibratoBPList*に変える
            //if( 0 != _depthBP ){
                result.setDepthBP( _depthBP.clone() );
            //}
            result.setStartRate( _startRate );
            //TODO: _rateBPの型をVibratoBPList*に変える
            //if( 0 != _rateBP ){
                result.setRateBP( _rateBP.clone() );
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
        return boost::lexical_cast<int>( spl[1] );
    }

protected:
    void init(){
        _type = HandleType::UNKNOWN;
        _articulation = ArticulationType::NONE;
        index = 0;
        iconId = "";
        ids = "";
        original = 0;
        _caption = "";
        _length = 0;
        _startDepth = 0;
        _startRate = 0;
        language = 0;
        program = 0;
        _duration = 0;
        _depth = 0;
        _startDyn = 0;
        _endDyn = 0;
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
        _startRate = 64;
        _startDepth = 64;
        _rateBP = VibratoBPList();
        _depthBP = VibratoBPList();
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
