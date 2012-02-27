/**
 * PhoneticSymbol.cpp
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
#include <string>
#include <boost/lexical_cast.hpp>
#include "PhoneticSymbol.h"

using namespace std;
using namespace VSQ_NS;

const string PhoneticSymbol::_SYMBOL_VOWEL_JP = "\ta\ti\tM\te\to\t";

const string PhoneticSymbol::_SYMBOL_CONSONANT_JP = "\tk\tk'\tg\tg'\tN\tN'\ts\tS\tz\tZ\tdz\tdZ\tt\tt'\tts\ttS\td\td'\tn\tJ\th\th\\\tC\tp\\\tp\\'\tb\tb'\tp\tp'\tm\tm'\tj\t4\t4'\tw\tN\\\t";

const string PhoneticSymbol::_SYMBOL_VOWEL_EN = "\t@\tV\te\te\tI\ti:\t{\tO:\tQ\tU\tu:\t@r\teI\taI\tOI\t@U\taU\tI@\te@\tU@\tO@\tQ@\t";

const string PhoneticSymbol::_SYMBOL_CONSONANT_EN = "\tw\tj\tb\td\tg\tbh\tdh\tgh\tdZ\tv\tD\tz\tZ\tm\tn\tN\tr\tl\tl0\tp\tt\tk\tph\tth\tkh\ttS\tf\tT\ts\tS\th\tSil\tAsp\t";

bool PhoneticSymbol::isConsonant( const string &symbol )
{
    string search = "\t" + symbol + "\t";
    int startIndex = _SYMBOL_CONSONANT_JP.find( search );
    if( startIndex != string::npos ){
        return true;
    }else{
        startIndex = _SYMBOL_CONSONANT_EN.find( search );
        if( startIndex != string::npos ){
            return true;
        }
    }
    return false;
}

bool PhoneticSymbol::isVowel( const string &symbol )
{
    string search = "\t" + symbol + "\t";
    int startIndex = _SYMBOL_VOWEL_JP.find( search );
    if( startIndex != string::npos ){
        return true;
    }else{
        startIndex = _SYMBOL_VOWEL_EN.find( search );
        if( startIndex != string::npos ){
            return true;
        }
    }
    return false;
}

bool PhoneticSymbol::isValidSymbol( const string &symbol )
{
    bool vowel = isVowel( symbol );
    bool consonant = isConsonant( symbol );
    if( vowel || consonant ){
        return true;
    }

    // ブレスの判定
    int symbolCharacterCount = symbol.size();
    if( symbol.find( "br" ) == 1 && symbolCharacterCount > 2 ){
        // br001とかをfalseにするためのチェック
        string s = symbol.substr( 3 );
        try{
            boost::lexical_cast<int>( s );
            return true;
        }catch( boost::bad_lexical_cast & ){
            return false;
        }
    }
    return false;
}
