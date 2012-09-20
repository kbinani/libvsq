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

#include "vsqglobal.hpp"
#include <string>
#include "StringUtil.hpp"

VSQ_BEGIN_NAMESPACE

using namespace std;
using namespace VSQ_NS;

/**
 * VSQ で使用される発音記号のためのユーティリティ
 * @class table
 * @name PhoneticSymbol
 */
class PhoneticSymbol
{
public:
    /**
     * @brief 指定した文字列が子音を表す発音記号かどうかを判定する
     * @param symbol (string) 判定対象の発音記号
     * @return (boolean) 子音であれば <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    static bool isConsonant( const string &symbol ){
        string search = "\t" + symbol + "\t";
        return isConsonantEN( search ) || isConsonantJP( search );
    }

    /**
     * @brief 指定した文字列が母音を表す発音記号かどうかを判定する
     * @param symbol (string) 判定対象の発音記号
     * @return (boolean) 母音であれば <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    static bool isVowel( const string &symbol ){
        string search = "\t" + symbol + "\t";
        return isVowelEN( search ) || isVowelJP( search );
    }

    /**
     * @brief 指定した文字列が発音記号として有効かどうかを判定する
     * @param symbol (string) 判定対象の発音記号
     * @return (boolean) 有効であれば <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    static bool isValidSymbol( const string &symbol ){
        bool vowel = isVowel( symbol );
        bool consonant = isConsonant( symbol );
        if( vowel || consonant ){
            return true;
        }

        // ブレスの判定
        int symbolCharacterCount = symbol.size();
        if( symbol.find( "br" ) == 0 && symbolCharacterCount > 2 ){
            // br001とかをfalseにするためのチェック
            string s = symbol.substr( 2 );
            try{
                StringUtil::parseInt<int>( s );
                return true;
            }catch( StringUtil::IntegerParseException & ){
                return false;
            }
        }
        return false;
    }

private:
    PhoneticSymbol()
    {
    }

    static bool isVowelJP( const string search ){
        const string symbolVowelJP = "\ta\ti\tM\te\to\t";
        return symbolVowelJP.find( search ) != string::npos;
    }

    static bool isConsonantJP( const string search ){
        const string symbolConsonantJP = "\tk\tk'\tg\tg'\tN\tN'\ts\tS\tz\tZ\tdz\tdZ\tt\tt'\tts\ttS\td\td'\tn\tJ\th\th\\\tC\tp\\\tp\\'\tb\tb'\tp\tp'\tm\tm'\tj\t4\t4'\tw\tN\\\t";
        return symbolConsonantJP.find( search ) != string::npos;
    }

    static bool isVowelEN( const string search ){
        const string symbolVowelEN = "\t@\tV\te\te\tI\ti:\t{\tO:\tQ\tU\tu:\t@r\teI\taI\tOI\t@U\taU\tI@\te@\tU@\tO@\tQ@\t";
        return symbolVowelEN.find( search ) != string::npos;
    }

    static bool isConsonantEN( const string search ){
        const string symbolConsonantEN = "\tw\tj\tb\td\tg\tbh\tdh\tgh\tdZ\tv\tD\tz\tZ\tm\tn\tN\tr\tl\tl0\tp\tt\tk\tph\tth\tkh\ttS\tf\tT\ts\tS\th\tSil\tAsp\t";
        return symbolConsonantEN.find( search ) != string::npos;
    }
};

VSQ_END_NAMESPACE

#endif
