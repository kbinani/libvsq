/**
 * TimesigList.h
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
#ifndef __TimesigList_h__
#define __TimesigList_h__

#include <vector>
#include <math.h>
#include <algorithm>
#include "Timesig.hpp"

VSQ_BEGIN_NAMESPACE

/**
 * @brief 拍子情報を格納したテーブルを表すクラス
 */
class TimesigList{
private:
    /**
     * @brief 拍子情報を格納したリスト。tick の昇順で格納する
     */
    vector<Timesig> list;

public:
    explicit TimesigList(){
    }

    ~TimesigList(){
        clear();
    }

    /**
     * @brief 指定したインデックスの拍子変更情報を取得する
     * @param index 取得するデータ点のインデックス(0から始まる)
     * @return 拍子変更情報
     */
    Timesig get( int index )const{
        return list[index];
    }

    /**
     * @brief データ点を追加する
     * @param item 追加する拍子変更情報
     */
    void push( const Timesig &item ){
        pushWithoutSort( item );
        updateTimesigInfo();
    }

    /**
     * @brief データ点の個数を返す
     * @return データ点の個数
     */
    int size() const{
        return list.size();
    }

    /**
     * @brief 指定された時刻における拍子情報を取得する
     * @param clock Tick 単位の時刻
     * @return 指定された時刻での拍子情報
     */
    Timesig getTimesigAt( tick_t clock )const{
        Timesig ret;
        ret.numerator = 4;
        ret.denominator = 4;
        ret.barCount = 0;

        int index = -1;
        int count = list.size();
        if( 0 < count ){
            for( int i = count - 1; i >= 0; i-- ){
                index = i;
                if( list[i].getClock() <= clock ){
                    break;
                }
            }
        }

        if( 0 <= index ){
            ret.numerator = list[index].numerator;
            ret.denominator = list[index].denominator;
            int tickPerBar = 480 * 4 / ret.denominator * ret.numerator;
            int deltaBar = (int)::floor( (double)((clock - list[index].getClock()) / tickPerBar) );
            ret.barCount = list[index].barCount + deltaBar;
        }else{
            int tickPerBar = 480 * 4 / ret.denominator * ret.numerator;
            int deltaBar = (int)::floor( (double)(clock / tickPerBar) );
            ret.barCount = deltaBar;
        }

        return ret;
    }

    /**
     * @brief 指定した小節の開始クロックを取得する。
     * ここで使用する小節数は、プリメジャーを考慮しない。即ち、曲頭の小節が 0 となる
     * @param barCount 小節数
     * @return Tick 単位の時刻
     */
    tick_t getClockFromBarCount( int barCount )const{
        int index = 0;
        for( int i = list.size() - 1; i >= 0; i-- ){
            index = i;
            if( list[i].barCount <= barCount ){
                break;
            }
        }
        Timesig item = list[index];
        int numerator = item.numerator;
        int denominator = item.denominator;
        tick_t initClock = item.getClock();
        int initBarCount = item.barCount;
        int clockPerBar = numerator * 480 * 4 / denominator;
        return initClock + (barCount - initBarCount) * clockPerBar;
    }

    /**
     * @brief リストをクリアする
     */
    void clear(){
        list.clear();
    }

    /**
     * @brief 指定したクロックが、曲頭から何小節目に属しているかを調べる
     * ここで使用する小節数は、プリメジャーを考慮しない。即ち、曲頭の小節が 0 となる
     * @param clock Tick 単位の時刻
     * @return 小節数
     */
    int getBarCountFromClock( VSQ_NS::tick_t clock )const{
        int index = 0;
        int c = list.size();
        for( int i = c - 1; i >= 0; i-- ){
            index = i;
            if( list[i].getClock() <= clock ){
                break;
            }
        }
        int bar_count = 0;
        if( index >= 0 ){
            Timesig item = list[index];
            VSQ_NS::tick_t last_clock = item.getClock();
            int t_bar_count = item.barCount;
            int numerator = item.numerator;
            int denominator = item.denominator;
            VSQ_NS::tick_t clock_per_bar = numerator * 480 * 4 / denominator;
            bar_count = t_bar_count + (clock - last_clock) / clock_per_bar;
        }
        return bar_count;
    }

protected:
    /**
     * @brief データ点を追加する。追加後のソートは行わない
     * push を頻繁に行い、速度の改善を行いたい場合は、TimesigList をオーバーライドし、
     * pushWithoutSort を必要回呼んだ後最後に updateTimesigInfo を呼ぶような実装にすると良いだろう
     * @param item 追加する拍子変更情報
     */
    void pushWithoutSort( const Timesig &item ){
        int index = -1;
        int count = list.size();
        for( int i = 0; i < count; i++ ){
            if( list[i].barCount == item.barCount ){
                index = i;
                break;
            }
        }

        if( 0 <= index ){
            list[index] = item;
        }else{
            list.push_back( item );
        }
    }

    /**
     * @brief リスト内の拍子変更情報の clock の部分を更新する
     */
    void updateTimesigInfo(){
        if( ! list.empty() ){
            std::sort( list.begin(), list.end(), Timesig::compare );

            int count = list.size();
            for( int j = 1; j < count; j++ ){
                Timesig item = list[j - 1];
                int numerator = item.numerator;
                int denominator = item.denominator;
                tick_t clock = item.getClock();
                int bar_count = item.barCount;
                int diff = (int)::floor( (double)(480 * 4 / denominator * numerator) );
                clock = clock + (list[j].barCount - bar_count) * diff;
                list[j].clock = clock;
            }
        }
    }
};

VSQ_END_NAMESPACE

#endif
