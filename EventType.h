#ifndef __EventType_h__
#define __EventType_h__

#include <string>
#include "vsqglobal.h"

VSQ_BEGIN_NAMESPACE

class EventType
{
public:
    /**
     * {@link Event} の種類を表現する列挙子
     */
    enum EventTypeEnum
    {
        /**
         * @brief 不明
         */
        UNKNOWN = 0,

        /**
         * @brief 歌手
         */
        SINGER = 1,

        /**
         * @brief 歌詞
         */
        NOTE = 2,

        /**
         * @brief アイコン
         */
        ICON = 3
    };

    /**
     * @brief 文字列に変換する
     * @param value 指定された列挙子の文字列表現
     * @return 変換後の文字列
     */
    static const std::string toString( EventTypeEnum value )
    {
        if( value == EventType::SINGER ){
            return "Singer";
        }else if( value == EventType::NOTE ){
            return "Anote";
        }else if( value == EventType::ICON ){
            return "Aicon";
        }else{
            return "Unknown";
        }
    }

private:
    EventType()
    {
    }

};

VSQ_END_NAMESPACE

#endif
