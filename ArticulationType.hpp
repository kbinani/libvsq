/**
 * ArticulationType.h
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
#ifndef __ArticulationTypeEnum_h__
#define __ArticulationTypeEnum_h__

#include "vsqglobal.hpp"

VSQ_BEGIN_NAMESPACE

namespace ArticulationType
{

    /**
     * @brief アイコン設定の種類を表す
     */
    enum ArticulationTypeEnum
    {
        /**
         * @brief アイコン設定がない
         */
        NONE = 0,

        /**
         * @brief ビブラート
         */
        VIBRATO = 1,

        /**
         * @brief クレッシェンド、またはデクレッシェンド
         */
        CRESCENDO = 2,

        /**
         * @brief ピアノ、フォルテ等の強弱記号
         */
        DYNAFF = 3,

        /**
         * @brief アタック
         */
        NOTE_ATTACK = 4,

        /**
         * @brief NoteTransition(詳細不明)
         */
        NOTE_TRANSITION = 5
    };
}

VSQ_END_NAMESPACE

#endif
