/**
 * MeasureLine.h
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
#ifndef __MeasureLine_h__
#define __MeasureLine_h__

#include "vsqglobal.hpp"

VSQ_BEGIN_NAMESPACE

/**
 * 小節を区切る線を表現するクラス
 */
class MeasureLine
{
public:
    /**
     * @brief tick 単位の時刻
     */
    tick_t tick;

    /**
     * @brief 小節の境界かどうか
     */
    bool isBorder;

    /**
     * @brief 小節数
     */
    int barCount;

    /**
     * @brief 拍子の分子
     */
    int numerator;

    /**
     * @brief 拍子の分母
     */
    int denominator;

public:
    explicit MeasureLine(){
        isBorder = true;
        tick = 0;
        barCount = 0;
        numerator = 4;
        denominator = 4;
    }
};

VSQ_END_NAMESPACE

#endif
