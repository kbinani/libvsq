/**
 * HandleType.h
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
#ifndef __HandleType_h__
#define __HandleType_h__

#include "vsqglobal.h"

VSQ_BEGIN_NAMESPACE

namespace HandleType
{

    /**
     * @brief {@link Handle} の種類を表現する列挙子
     * @class table
     * @name HandleTypeEnum
     */
    enum HandleTypeEnum
    {
        UNKNOWN = 0,

        /**
         * 歌詞ハンドル
         * @var int
         */
        LYRIC = 1,

        /**
         * ビブラートハンドル
         * @var int
         */
        VIBRATO = 2,

        /**
         * 歌手ハンドル
         * @var int
         */
        SINGER = 3,

        /**
         * アタックハンドル
         * @var int
         */
        NOTE_HEAD = 4,

        /**
         * Dynamics ハンドル
         * @var int
         */
        DYNAMICS = 5
    };

}

VSQ_END_NAMESPACE

#endif
