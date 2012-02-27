/**
 * PhoneticSymbol.h
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
#ifndef __PhoneticSymbol_h__
#define __PhoneticSymbol_h__

#include <string>
#include "vsqglobal.h"

VSQ_BEGIN_NAMESPACE

/**
 * VSQ で使用される発音記号のためのユーティリティ
 * @class table
 * @name PhoneticSymbol
 */
class PhoneticSymbol
{
private:
    /**
     * 日本語の母音発音記号
     * @var string
     * @access private static
     */
    static const std::string _SYMBOL_VOWEL_JP;

    /**
     * 日本語の子音発音記号
     * @var string
     * @access private static
     */
    static const std::string _SYMBOL_CONSONANT_JP;

    /**
     * 英語の母音発音記号
     * @var string
     * @access private static
     */
    static const std::string _SYMBOL_VOWEL_EN;

    /**
     * 英語の子音発音記号
     * @var string
     * @access private static
     */
    static const std::string _SYMBOL_CONSONANT_EN;

public:
    /**
     * 指定した文字列が子音を表す発音記号かどうかを判定する
     * @param symbol (string) 判定対象の発音記号
     * @return (boolean) 子音であれば <code>true</code> を、そうでなければ <code>false</code> を返す
     * @access static
     */
    static bool isConsonant( const std::string &symbol );

    /**
     * 指定した文字列が母音を表す発音記号かどうかを判定する
     * @param symbol (string) 判定対象の発音記号
     * @return (boolean) 母音であれば <code>true</code> を、そうでなければ <code>false</code> を返す
     * @access static
     */
    static bool isVowel( const std::string &symbol );

    /**
     * 指定した文字列が発音記号として有効かどうかを判定する
     * @param symbol (string) 判定対象の発音記号
     * @return (boolean) 有効であれば <code>true</code> を、そうでなければ <code>false</code> を返す
     * @access static
     */
    static bool isValidSymbol( const std::string &symbol );

private:
    PhoneticSymbol()
    {
    }
};

VSQ_END_NAMESPACE

#endif
