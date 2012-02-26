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
        /**
         * 歌詞ハンドル
         * @var int
         */
        LYRIC = 0,

        /**
         * ビブラートハンドル
         * @var int
         */
        VIBRATO = 1,

        /**
         * 歌手ハンドル
         * @var int
         */
        SINGER = 2,

        /**
         * アタックハンドル
         * @var int
         */
        NOTE_HEAD = 3,

        /**
         * Dynamics ハンドル
         * @var int
         */
        DYNAMICS = 4
    };

}

VSQ_END_NAMESPACE

#endif
