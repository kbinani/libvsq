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
#include "PhoneticSymbol.hpp"

using namespace std;
using namespace VSQ_NS;

bool PhoneticSymbol::isConsonant( const string &symbol )
{
    string search = "\t" + symbol + "\t";
    return isConsonantEN( search ) || isConsonantJP( search );
}

bool PhoneticSymbol::isVowel( const string &symbol )
{
    string search = "\t" + symbol + "\t";
    return isVowelEN( search ) || isVowelJP( search );
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
    if( symbol.find( "br" ) == 0 && symbolCharacterCount > 2 ){
        // br001とかをfalseにするためのチェック
        string s = symbol.substr( 2 );
        try{
            boost::lexical_cast<int>( s );
            return true;
        }catch( boost::bad_lexical_cast & ){
            return false;
        }
    }
    return false;
}
