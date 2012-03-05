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
#ifndef __VoiceLanguageEnum_h__
#define __VoiceLanguageEnum_h__

#include <string>
#include <algorithm>
#include <cctype>
#include "vsqglobal.h"

VSQ_BEGIN_NAMESPACE

/**
 * 歌手の歌唱言語を表す列挙子
 * @class table
 * @name VoiceLanguageEnum
 */
class VoiceLanguage
{
public:
    enum VoiceLanguageEnum
    {
        /**
         * 日本語
         * @var int
         */
        JAPANESE = 0,

        /**
         * 英語
         * @var int
         */
        ENGLISH = 1
    };

    /**
     * 歌手の名前から、その歌手の歌唱言語を取得する
     * @param name (string) 歌手の名前
     * @return (VoiceLanguageEnum) 歌唱言語
     * @access static
     */
    static VoiceLanguageEnum valueFromSingerName( const std::string &name )
    {
        std::string search = name;
        std::transform( search.begin(), search.end(), search.begin(), tolower );
        if( search == "meiko" ||
            search == "kaito" ||
            search == "miku" ||
            search == "rin" ||
            search == "len" ||
            search == "rin_act2" ||
            search == "len_act2" ||
            search == "gackpoid" ||
            search == "luka_jpn" ||
            search == "megpoid" ||
            search == "sfa2_miki" ||
            search == "yuki" ||
            search == "kiyoteru" ||
            search == "miku_sweet" ||
            search == "miku_dark" ||
            search == "miku_soft" ||
            search == "miku_light" ||
            search == "miku_vivid" ||
            search == "miku_solid" ){
            return VoiceLanguage::JAPANESE;
        }else if( search == "sweet_ann" ||
            search == "prima" ||
            search == "luka_eng" ||
            search == "sonika" ||
            search == "lola" ||
            search == "leon" ||
            search == "miriam" ||
            search == "big_al" ){
            return VoiceLanguage::ENGLISH;
        }
        return VoiceLanguage::JAPANESE;
    }
};

VSQ_END_NAMESPACE

#endif
