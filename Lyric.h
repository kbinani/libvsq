/**
 * Lyric.h
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
#ifndef __Lyric_h__
#define __Lyric_h__

#include <string>
#include <vector>
#include "vsqglobal.h"

VSQ_BEGIN_NAMESPACE

/**
 * 歌詞ハンドルに格納する歌詞情報を保持するクラス
 * @class table
 * @name Lyric
 */
class Lyric
{
public:
    /**
     * 歌詞
     * @var string
     */
    std::string phrase;// = "a";

    /**
     * 歌詞ハンドル内に複数の歌詞が入る場合の、この歌詞の長さ分率。デフォルトは 1.0
     * @var double
     */
    double lengthRatio;// = 1.0;

    /**
     * 発音記号がプロテクトされた状態かどうか
     * @var boolean
     */
    bool isProtected;// = false;

    /**
     * 発音記号のリスト
     * @var table
     * @access private
     */
    std::vector<std::string> _phoneticSymbol;// = { "a" };

    /**
     * Consonant Adjustment のリスト
     * @var table
     * @access private
     */
    std::vector<int> _consonantAdjustment;// = { 0 };

    /**
     * 文字列を元に初期化を行う
     * @param line 「"あ","a",0.0000,0.0」などのような文字列
     */
    explicit Lyric( const std::string &line );

    /**
     * 歌詞、発音記号を指定して初期化を行う
     * @param phrase 歌詞
     * @param phoneticSymbol 発音記号
     */
    explicit Lyric( const std::string &phrase, const std::string &phoneticSymbol );

    /**
     * このオブジェクトと、指定されたオブジェクトが同じかどうかを調べる。
     * 音声合成したときに影響のある範囲のフィールドしか比較されない。
     * たとえば、<code>isProtected</code> が <code>this</code> と <code>item</code> で違っていても、他が同一であれば <code>true</code> が返る。
     * @param item 比較対象のオブジェクト
     * @return 比較対象と同じであれば <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    bool equalsForSynth( Lyric &item ) const;

    /**
     * このオブジェクトのインスタンスと、指定されたオブジェクトが同じかどうかを調べる
     * @param item (Lyric) 比較対象のオブジェクト
     * @return (boolean) 比較対象と同じであれば <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    bool equals( Lyric &item ) const;

    /**
     * Consonant Adjustmentの文字列形式を取得する
     * @return Consonant Adjustment を空白区切りで連結した文字列
     */
    const std::string getConsonantAdjustment() const;

    /**
     * Consonant Adjustmentを文字列形式で設定する
     * @param value Consonant Adjustment を空白区切りで連結した文字列
     */
    void setConsonantAdjustment( const std::string &value );

    /**
     * Consonant Adjustment を、整数配列で取得する
     * @return Consonant Adjustment を格納した整数の配列
     */
    const std::vector<int> getConsonantAdjustmentList() const;

    /**
     * Consonant Adjustment を、整数配列形式で設定する
     * @param value Consonant Adjustment を格納した整数の配列
     */
    void setConsonantAdjustmentList( const std::vector<int> &value );

    /**
     * この歌詞の発音記号を取得する
     * @return 発音記号
     */
    const std::string getPhoneticSymbol() const;

    /**
     * この歌詞の発音記号を設定する
     * @param value 発音記号
     */
    void setPhoneticSymbol( const std::string &value );

    /**
     * この歌詞の発音記号の配列を取得する
     * @return 発音記号の配列
     */
    const std::vector<std::string> getPhoneticSymbolList() const;

    /**
     * このインスタンスを文字列に変換する
     * @param addQuateMark (boolean) 歌詞、発音記号の前後に引用符(")を追加するかどうか
     * @return (string) 変換後の文字列
     */
    const std::string toString( bool addQuateMark = false ) const;
};

VSQ_END_NAMESPACE

#endif
