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
class Timesig{
    friend class TimesigList;

public:
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

protected:
    /**
     * @brief Tick 単位の時刻
     */
    tick_t clock;

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
     * @brief このイベントの tick 単位の時刻を取得する
     * @return tick 単位の時刻
     */
    tick_t getClock()const{
        return clock;
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
     * @return 比較対象がこのインスタンスより大であれば true を、そうでなければ false を返す
     */
    bool compareTo( const Timesig &item ) const{
        return (this->barCount - item.barCount) < 0;
    }

    /**
     * @brief コピーを作成する
     * @return このオブジェクトのコピー
     */
    Timesig clone() const{
        Timesig result( numerator, denominator, barCount );
        result.clock = clock;
        return result;
    }

    /**
     * @brief 順序を比較する
     * @return b が a より大であれば true を、そうでなければ false を返す
     */
    static bool compare( const Timesig &a, const Timesig &b ){
        return a.compareTo( b );
    }
};

VSQ_END_NAMESPACE

#endif
