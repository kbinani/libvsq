/**
 * NoteNumberUtil.h
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
#ifndef __NoteNumberUtil_hpp__

#include "vsqglobal.hpp"
#include <string>

VSQ_BEGIN_NAMESPACE

class NoteNumberUtil{
public:
    /**
     * @brief ノートのオクターブ，変化記号を除いた部分の文字列表記を調べます．
     * 例：C4 => "C", D#4 => "D"
     * @param note ノート番号
     */
    static std::string getNoteStringBase( int note ){
        int odd = note % 12;
        switch ( odd ) {
            case 0:
            case 1:{
                return "C";
            }
            case 2:{
                return "D";
            }
            case 3:
            case 4:{
                return "E";
            }
            case 5:
            case 6:{
                return "F";
            }
            case 7:
            case 8:{
                return "G";
            }
            case 9:{
                return "A";
            }
            case 10:
            case 11:{
                return "B";
            }
            default:{
                return "";
            }
        }
    }

    /**
     * @brief ノート#のオクターブ部分の表記を調べます．
     * 例：C4 => 4, D#4 => 4
     * @param note ノート番号
     */
    static int getNoteOctave( int note ){
        int odd = note % 12;
        return (note - odd) / 12 - 2;
    }

    /**
     * @brief C#4なら+1, C4なら0, Cb4なら-1
     * @param note ノート番号
     */
    static int getNoteAlter( int note ){
        static int ALTER[13] = { 0, 1, 0, -1, 0, 0, 1, 0, 1, 0, -1, 0, 0 };
        note = note % 12 + 12;
        return ALTER[note % 12];
    }
};

VSQ_END_NAMESPACE

#endif
