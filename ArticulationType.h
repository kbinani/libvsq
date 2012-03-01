#ifndef __ArticulationTypeEnum_h__
#define __ArticulationTypeEnum_h__

#include "vsqglobal.h"

VSQ_BEGIN_NAMESPACE

namespace ArticulationType
{

    /**
     * @brief アイコン設定の種類を表す
     */
    enum ArticulationTypeEnum
    {
        /**
         * @brief ビブラート
         */
        VIBRATO = 0,

        /**
         * @brief クレッシェンド、またはデクレッシェンド
         */
        CRESCENDO = 1,

        /**
         * @brief ピアノ、フォルテ等の強弱記号
         */
        DYNAFF = 2,

        /**
         * @brief アタック
         */
        NOTE_ATTACK = 3,

        /**
         * @brief NoteTransition(詳細不明)
         */
        NOTE_TRANSITION = 4
    };
}

VSQ_END_NAMESPACE

#endif
