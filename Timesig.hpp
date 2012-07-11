/**
 * Timesig.h
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
#ifndef __Timesig_h__
#define __Timesig_h__

#include <string>
#include <sstream>
#include "vsqglobal.hpp"

VSQ_BEGIN_NAMESPACE

using namespace std;

/**
 * 拍子変更情報テーブル内の要素を表現するためのクラス
 */
class Timesig
{
public:
    /**
     * @brief Tick 単位の時刻
     */
    tick_t clock;

    /**
     * @brief 拍子の分子
     */
    int numerator;

    /**
     * @brief 拍子の分母
     */
    int denominator;

    /**
     * @brief 何小節目か
     */
    int barCount;

public:
    explicit Timesig(){
        this->clock = 0;
        this->numerator = 4;
        this->denominator = 4;
        this->barCount = 0;
    }

    /**
     * @brief 初期化を行う
     * @param numerator 拍子の分子の値
     * @param denominator 拍子の分母値
     * @param barCount 小節数
     */
    explicit Timesig( int numerator, int denominator, int barCount ){
        this->clock = 0;
        this->numerator = numerator;
        this->denominator = denominator;
        this->barCount = barCount;
    }

    /**
     * @brief 文字列に変換する
     * @return (string) 変換後の文字列
     */
    const std::string toString() const{
        ostringstream oss;
        oss << "{Clock=" << this->clock << ", Numerator=" << this->numerator << ", Denominator=" << this->denominator << ", BarCount=" << this->barCount << "}";
        return oss.str();
    }

    /**
     * @brief 順序を比較する
     * @param item 比較対象のアイテム
     * @return このインスタンスが比較対象よりも小さい場合は負の整数、等しい場合は 0、大きい場合は正の整数を返す
     */
    int compareTo( Timesig &item ) const{
        return this->barCount - item.barCount;
    }

    /**
     * @brief 順序を比較する
     * @return b が a より大であれば true を、そうでなければ false を返す
     */
    static int compare( const void *a, const void *b ){
        Timesig *castedA = *(Timesig **)a;
        Timesig *castedB = *(Timesig **)b;
        return castedA->compareTo( *castedB );
    }
};

VSQ_END_NAMESPACE

#endif
