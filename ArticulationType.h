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
