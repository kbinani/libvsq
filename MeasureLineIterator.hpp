/**
 * MeasureLineIterator.h
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
#ifndef __MeasureLineIterator_h__
#define __MeasureLineIterator_h__

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
    TimesigList *m_list;
    int m_end_clock;
    int i;
    tick_t clock;
    int local_denominator;
    int local_numerator;
    tick_t clock_step;
    int t_end;
    tick_t local_clock;
    int bar_counter;

public:
    /**
     * @brief 小節線の情報を取得する区間を指定し、初期化する
     * @param list テンポ変更リスト
     */
    explicit MeasureLineIterator( TimesigList *list ){
        m_list = list;
        m_end_clock = 0;
        i = 0;
        t_end = -1;
        clock = 0;
        this->reset( m_end_clock );
    }

    /**
     * @brief 次の小節線が取得可能かどうかを取得する
     * @return 取得可能であれば true を返す
     */
    bool hasNext(){
        if( clock <= m_end_clock ){
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
        int mod = clock_step * local_numerator;
        if( clock < t_end ){
            if( (clock - local_clock) % mod == 0 ){
                bar_counter++;
                MeasureLine ret;
                ret.tick = clock;
                ret.isBorder = true;
                clock += clock_step;
                return ret;
            }else{
                MeasureLine ret;
                ret.tick = clock;
                ret.isBorder = false;
                clock += clock_step;
                return ret;
            }
        }

        if( i < m_list->size() ){
            local_denominator = m_list->get( i ).denominator;
            local_numerator = m_list->get( i ).numerator;
            local_clock = m_list->get( i ).clock;
            int local_bar_count = m_list->get( i ).barCount;
            int denom = local_denominator;
            if( denom <= 0 ){
                denom = 4;
            }
            clock_step = 480 * 4 / denom;
            mod = clock_step * local_numerator;
            bar_counter = local_bar_count - 1;
            t_end = m_end_clock;
            if( i + 1 < m_list->size() ){
                t_end = m_list->get( i + 1 ).clock;
            }
            i++;
            clock = local_clock;
            if( clock < t_end ){
                if( (clock - local_clock) % mod == 0 ){
                    bar_counter++;
                    MeasureLine ret;
                    ret.tick = clock;
                    ret.isBorder = true;
                    clock += clock_step;
                    return ret;
                }else{
                    MeasureLine ret;
                    ret.tick = clock;
                    ret.isBorder = false;
                    clock += clock_step;
                    return ret;
                }
            }
        }else{
            if( (clock - local_clock) % mod == 0 ){
                bar_counter++;
                MeasureLine ret;
                ret.tick = clock;
                ret.isBorder = true;
                clock += clock_step;
                return ret;
            }else{
                MeasureLine ret;
                ret.tick = clock;
                ret.isBorder = false;
                clock += clock_step;
                return ret;
            }
        }
        return MeasureLine();
    }

    /**
     * @brief 反復子をリセットする
     * @todo startTick を指定できるようにする
     */
    void reset( tick_t end_clock ){
        this->m_end_clock = end_clock;
        this->i = 0;
        this->t_end = -1;
        this->clock = 0;
        this->local_denominator = 0;
        this->local_numerator = 0;
        this->clock_step = 0;
        this->local_clock = 0;
        this->bar_counter = 0;
    }
};

VSQ_END_NAMESPACE

#endif
