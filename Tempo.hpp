/**
 * Tempo.h
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
#ifndef __Tempo_h__
#define __Tempo_h__

#include "vsqglobal.hpp"
#include <string>
#include <sstream>

VSQ_BEGIN_NAMESPACE

/**
 * @brief テンポ情報テーブル内の要素を表現するクラス
 */
class Tempo{
    friend class TempoList;

public:
    /**
     * Tick 単位の時刻
     * @var int
     */
    tick_t clock;

    /**
     * テンポ値。四分音符の長さをマイクロ秒単位で表した値
     * @var int
     */
    int tempo;

protected:
    /**
     * テンポ変更の秒単位の時刻
     * @var double
     * @access private
     */
    double _time;

public:
    /**
     * @brief 文字列に変換する
     * @return (string) 変換後の文字列
     */
    const std::string toString(){
        std::ostringstream oss;
        oss << "{Clock=" << clock << ", Tempo=" << tempo << ", Time=" << _time << "}";
        return oss.str();
    }

    /**
     * @brief 初期化を行う
     * @param clock (int) Tick 単位の時刻
     * @param tempo (int) テンポ値。四分音符の長さをマイクロ秒単位で表した値
     */
    explicit Tempo( VSQ_NS::tick_t clock, int tempo ){
        this->clock = clock;
        this->tempo = tempo;
        _time = 0.0;
    }

    explicit Tempo(){
        clock = 0;
        tempo = 0;
        _time = 0.0;
    }


    /**
     * @brief 順序を比較する
     * @param entry (Tempo) 比較対象のアイテム
     * @return (int) このインスタンスが比較対象よりも小さい場合は負の整数、等しい場合は 0、大きい場合は正の整数を返す
     */
    int compareTo( const Tempo &entry ){
        return compareCore( *this, entry );
    }

    /**
     * @brief このオブジェクトのインスタンスと、指定されたオブジェクトが同じかどうかを調べる
     * @param entry (Tempo) 比較対象のオブジェクト
     * @return (boolean) 比較対象と同じであれば <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    bool equals( const Tempo &entry ){
        if( clock == entry.clock ){
            return true;
        }else{
            return false;
        }
    }

    /**
     * @brief 秒単位の時刻を取得する
     * @return (double) 秒単位の時刻
     */
    double getTime(){
        return _time;
    }

    /**
     * @brief コピーを作成する
     * @return このオブジェクトのコピー
     */
    Tempo clone() const{
        Tempo result;
        result.clock = clock;
        result.tempo = tempo;
        result._time = _time;
        return result;
    }

    /**
     * @brief 2 つの {@link Tempo} を比較する
     * @param a (Tempo) 比較対象のオブジェクト
     * @param b (Tempo) 比較対象のオブジェクト
     * @return (boolean) <code>a</code> が <code>b</code> よりも小さい場合は <code>true</code>、そうでない場合は <code>false</code> を返す
     */
    static bool compare( const Tempo &a, const Tempo &b ){
        return compareCore( a, b ) < 0;
    }

private:
    static int compareCore( const Tempo &a, const Tempo &b ){
        return (int)(a.clock - b.clock);
    }

};

VSQ_END_NAMESPACE

#endif
