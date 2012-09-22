/**
 * MeasureLineIterator.hpp
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
#ifndef __MeasureLineIterator_hpp__
#define __MeasureLineIterator_hpp__

#include "vsqglobal.hpp"
#include "TimesigList.hpp"
#include "MeasureLine.hpp"

VSQ_BEGIN_NAMESPACE

/**
 * @brief 小節を区切る線の情報を順に返す反復子
 */
class MeasureLineIterator
{
private:
    /**
     * @brief 補助線の間隔として利用可能な最小時間(tick単位)
     */
    const static tick_t MIN_ASSIST_LINE_STEP = 15;
    TimesigList *list;
    tick_t endTick;
    int i;
    tick_t tick;
    int currentDenominator;
    int currentNumerator;
    tick_t stepLength;
    tick_t temporaryEndTick;
    tick_t currentTick;
    int barCounter;
    tick_t assistLineStep;
    tick_t originalStepLength;

public:
    class InvalidAssistLineStep : public std::exception{
    };

public:
    /**
     * @brief 小節線の情報を取得する区間を指定し、初期化する
     * @param list テンポ変更リスト
     */
    explicit MeasureLineIterator( TimesigList *list, tick_t assistLineStep = 0 ){
        this->list = list;
        endTick = 0;
        i = 0;
        temporaryEndTick = -1;
        tick = 0;
        if( assistLineStep < 0 || assistLineStep % MIN_ASSIST_LINE_STEP != 0 ){
            throw InvalidAssistLineStep();
        }
        this->assistLineStep = assistLineStep;
        this->reset( endTick );
    }

    /**
     * @brief 次の小節線が取得可能かどうかを取得する
     * @return 取得可能であれば true を返す
     */
    bool hasNext(){
        if( tick <= endTick ){
            return true;
        }else{
            return false;
        }
    }

    /**
     * @brief 次の小節線を取得する
     * @return 次の小節線の情報
     */
    MeasureLine next(){
        int mod = stepLength * currentNumerator;
        if( tick < temporaryEndTick ){
            if( (tick - currentTick) % mod == 0 ){
                return returnBorder();
            }else{
                return returnOther();
            }
        }

        if( i < list->size() ){
            currentDenominator = list->get( i ).denominator;
            currentNumerator = list->get( i ).numerator;
            currentTick = list->get( i ).getClock();
            int local_bar_count = list->get( i ).barCount;
            int denom = currentDenominator;
            if( denom <= 0 ){
                denom = 4;
            }
            originalStepLength = 480 * 4 / denom;
            if( 0 < assistLineStep && assistLineStep < originalStepLength ){
                stepLength = assistLineStep;
            }else{
                stepLength = originalStepLength;
            }
            mod = stepLength * currentNumerator;
            barCounter = local_bar_count - 1;
            temporaryEndTick = endTick;
            if( i + 1 < list->size() ){
                temporaryEndTick = list->get( i + 1 ).getClock();
            }
            i++;
            tick = currentTick;
            if( tick < temporaryEndTick ){
                if( (tick - currentTick) % mod == 0 ){
                    return returnBorder();
                }else{
                    return returnOther();
                }
            }
        }else{
            if( (tick - currentTick) % mod == 0 ){
                return returnBorder();
            }else{
                return returnOther();
            }
        }
        return MeasureLine( 0, 0, 4, 4, true, false );
    }

    /**
     * @brief 反復子をリセットする
     * @param endTick 反復を行う最大の時刻(tick単位)を指定する
     * @todo startTick を指定できるようにする
     */
    void reset( tick_t endTick ){
        this->endTick = endTick;
        this->i = 0;
        this->temporaryEndTick = -1;
        this->tick = 0;
        this->currentDenominator = 0;
        this->currentNumerator = 0;
        this->stepLength = 0;
        this->currentTick = 0;
        this->barCounter = 0;
        this->originalStepLength = 0;
    }

private:
    /**
     * @brief 小節の境界を表す MeasureLine のインスタンスを返す
     */
    MeasureLine returnBorder(){
        barCounter++;
        MeasureLine ret( tick, barCounter, currentNumerator, currentDenominator, true, false );
        tick += stepLength;
        return ret;
    }

    /**
     * @brief 小節の境界でない MeasureLine のインスタンスを返す
     */
    MeasureLine returnOther(){
        MeasureLine ret( tick, barCounter, currentNumerator, currentDenominator, false, (tick - currentTick) % originalStepLength != 0 );
        tick += stepLength;
        return ret;
    }
};

VSQ_END_NAMESPACE

#endif
