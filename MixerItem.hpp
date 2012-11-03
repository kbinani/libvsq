/**
 * MixerItem.hpp
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
#ifndef __MixerItem_hpp__
#define __MixerItem_hpp__

#include "vsqglobal.hpp"

VSQ_BEGIN_NAMESPACE

/**
 * @brief @link Mixer の <code>slave</code> 要素に格納されるアイテムを表すクラス
 */
class MixerItem{
public:
    /**
     * @brief Feder値
     */
    int feder;

    /**
     * @brief Panpot値
     */
    int panpot;

    /**
     * @brief Mute値
     */
    int mute;

    /**
     * @brief Solo値
     */
    int solo;

    /**
     * @brief 各パラメータを指定し、初期化を行う
     * @param feder (int) Feder値
     * @param panpot (int) Panpot値
     * @param mute (int) Mute値
     * @param solo (int) Solo値
     */
    explicit MixerItem( int feder, int panpot, int mute, int solo ){
        this->feder = feder;
        this->panpot = panpot;
        this->mute = mute;
        this->solo = solo;
    }

    /**
     * @brief コピーを作成する
     * @return このオブジェクトのコピー
     */
    MixerItem clone() const{
        return MixerItem( this->feder, this->panpot, this->mute, this->solo );
    }
};

VSQ_END_NAMESPACE

#endif
