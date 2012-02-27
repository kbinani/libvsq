/**
 * StringUtil.h
 * Copyright © 2012 kbinani
 *
 * This file is part of `MIDIInput UG Job Plugin'.
 *
 * `MIDIInput UG Job Plugin' is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * `MIDIInput UG Job Plugin' is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#ifndef __StringUtil_h__
#define __StringUtil_h__

#include <vector>
#include <string>

using namespace std;

/**
 * 文字列関連のユーティリティ
 */
class StringUtil
{
public:
    /**
     * 文字列を区切り文字で分割する
     * @param delimiter 区切り文字
     * @param text 文字列
     * @param limit 区切る回数の最大値
     * @return 区切られた文字列のリスト
     */
    static vector<string> explode( string delimiter, string text, string::size_type limit = string::npos, string escape = "" )
    {
        vector<string> result;
        string::size_type searchFrom = 0;
        string::size_type delimiterIndex = getDelimiterIndex( text, delimiter, escape, searchFrom );
        while( delimiterIndex != string::npos ){
            string token = text.substr( searchFrom, delimiterIndex - searchFrom );
            result.push_back( token );
            searchFrom = delimiterIndex + delimiter.length();
            if( result.size() + 1 == limit ){
                break;
            }
            delimiterIndex = getDelimiterIndex( text, delimiter, escape, searchFrom );
        }
        result.push_back( text.substr( searchFrom ) );
        return result;
    }

    /**
     * 含まれる文字列を全て置換する
     * @param text 処理対象の文字列
     * @param search 検索する文字列
     * @param replace 置換する文字列
     * @return 置換後の文字列
     */
    static string replace( string text, string search, string replace )
    {
        if( search == replace ){
            return text;
        }
        string result = text;
        string::size_type index = result.find( search, 0 );
        int searchLength = search.length();
        int replaceLength = replace.length();

        while( string::npos != index ){
            result.replace( index, searchLength, replace );
            index = result.find( search, index - searchLength + replaceLength + 1 );
        }
        return result;
    }

private:
    StringUtil()
    {
    }

    /**
     * @brief 文字列から、エスケープ文字を考慮してデリミタ文字列の位置を探す
     * @param text 文字列
     * @param delimiter デリミタ(区切り)文字
     * @param escape エスケープ文字
     * @param searchFrom 検索開始インデックス
     * @return デリミタ文字列の位置。見つからなければ string::npos を返す
     */
    static string::size_type getDelimiterIndex( string &text, string &delimiter, string &escape, string::size_type searchFrom )
    {
        if( escape.length() == 0 ){
            return (int)text.find( delimiter, searchFrom );
        }else{
            while( searchFrom < text.length() ){
                int draft = (int)text.find( delimiter, searchFrom );
                if( draft == string::npos ){
                    return string::npos;
                }
                int expectedEscapeIndex = draft - escape.length();
                if( 0 <= expectedEscapeIndex ){
                    // エスケープ文字が存在する可能性がある
                    if( text.substr( expectedEscapeIndex, escape.length() ) == escape ){
                        // エスケープ文字だった場合
                        searchFrom = draft + 1;
                    }else{
                        return draft;
                    }
                }else{
                    // エスケープ文字が存在し得ない
                    return draft;
                }
            }
            return string::npos;
        }
    }
};

#endif
