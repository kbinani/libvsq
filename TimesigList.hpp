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
    Timesig **list;

    /**
     * @brief リストのサイズ
     */
    int listSize;

    /**
     * @brief updateTimesigInfo メソッドが呼ばれたかどうか
     */
    bool updated;

public:
    explicit TimesigList(){
        this->listSize = 0;
        this->list = NULL;
        this->updated = false;
    }

    ~TimesigList(){
        if( this->list ){
            this->listSize = 0;
            for( int i = 0; i < this->listSize; i++ ){
                Timesig *item = this->list[i];
                delete item;
            }
            free( this->list );
            this->list = NULL;
        }
    }

    /**
     * @brief 指定したインデックスの拍子変更情報を取得する
     * @param index 取得するデータ点のインデックス(0から始まる)
     * @return 拍子変更情報
     */
    Timesig get( int index ){
        Timesig *item = this->list[index];
        Timesig result( item->numerator, item->denominator, item->barCount );
        result.clock = item->clock;
        return result;
    }

    /**
     * @brief リスト内の拍子変更情報の clock の部分を更新する
     */
    void updateTimesigInfo(){
        if( 0 < this->listSize ){
            qsort( this->list, this->listSize, sizeof( Timesig * ), Timesig::compare );

            int count = this->listSize;
            for( int j = 1; j < count; j++ ){
                Timesig *item = this->list[j - 1];
                int numerator = item->numerator;
                int denominator = item->denominator;
                tick_t clock = item->clock;
                int bar_count = item->barCount;
                int diff = (int)::floor( (double)(480 * 4 / denominator * numerator) );
                clock = clock + (this->list[j]->barCount - bar_count) * diff;
                this->list[j]->clock = clock;
            }

            this->updated = true;
        }
    }

    /**
     * @brief データ点を追加する
     * @param item 追加する拍子変更情報
     */
    void push( Timesig item ){
        int index = -1;
        for( int i = 0; i < listSize; i++ ){
            if( list[i]->barCount == item.barCount ){
                index = i;
                break;
            }
        }

        if( 0 <= index ){
            list[index]->barCount = item.barCount;
            list[index]->numerator = item.numerator;
            list[index]->denominator = item.denominator;
        }else{
            Timesig *add = new Timesig( item.numerator, item.denominator, item.barCount );
            add->clock = item.clock;
            this->list = (Timesig **)realloc( this->list, sizeof( Timesig * ) * (this->listSize + 1) );
            this->list[this->listSize] = add;
            this->listSize++;
        }
        this->updated = false;
    }

    /**
     * @brief データ点の個数を返す
     */
    int size(){
        return this->listSize;
    }

    /**
     * @brief updateTimesigInfo メソッドが呼ばれたかどうかを取得する
     */
    bool isUpdated(){
        return this->updated;
    }

    /**
     * @brief 指定された時刻における拍子情報を取得する
     * @param clock Tick 単位の時刻
     * @return 指定された時刻での拍子情報
     */
    Timesig getTimesigAt( tick_t clock ){
        if( !updated ){
            updateTimesigInfo();
        }
        Timesig ret;
        ret.numerator = 4;
        ret.denominator = 4;
        ret.barCount = 0;

        int index = -1;
        if( 0 < listSize ){
            for( int i = listSize - 1; i >= 0; i-- ){
                index = i;
                if( list[i]->clock <= clock ){
                    break;
                }
            }
        }

        if( 0 <= index ){
            ret.numerator = list[index]->numerator;
            ret.denominator = list[index]->denominator;
            int tickPerBar = 480 * 4 / ret.denominator * ret.numerator;
            int deltaBar = (int)::floor( (double)((clock - list[index]->clock) / tickPerBar) );
            ret.barCount = list[index]->barCount + deltaBar;
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
    tick_t getClockFromBarCount( int barCount ){
        if( !updated ){
            updateTimesigInfo();
        }
        int index = 0;
        for( int i = listSize - 1; i >= 0; i-- ){
            index = i;
            if( list[i]->barCount <= barCount ){
                break;
            }
        }
        Timesig *item = list[index];
        int numerator = item->numerator;
        int denominator = item->denominator;
        tick_t initClock = item->clock;
        int initBarCount = item->barCount;
        int clockPerBar = numerator * 480 * 4 / denominator;
        return initClock + (barCount - initBarCount) * clockPerBar;
    }
};

VSQ_END_NAMESPACE

#endif
