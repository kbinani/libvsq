/**
 * StringUtil.h
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
#ifndef __StringUtil_h__
#define __StringUtil_h__

#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <algorithm>
#include <iomanip>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <functional>
#include <cctype>

using namespace std;

/**
 * @brief 文字列関連のユーティリティ
 */
class StringUtil
{
public:
    /**
     * @brief parseInt メソッドが投げる例外
     */
    class IntegerParseException : public std::exception{
    };

    /**
     * @brief parseFloat メソッドが投げる例外
     */
    class FloatParseException : public std::exception{
    };

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

    /**
     * @brief 文字列を整数に変換する
     * @param text 変換する文字列
     * @return 変換後の数値
     */
    template<typename T>
    static T parseInt( const std::string &text, int baseNumber = 10 )throw( IntegerParseException ){
        if( baseNumber == 10 ){
            try{
                return boost::lexical_cast<T>( text );
            }catch( boost::bad_lexical_cast &e ){
                throw IntegerParseException();
            }
        }else{
            char *endptr;
            T result = (T)strtol( text.c_str(), &endptr, baseNumber );
            if( *endptr != '\0' ){
                throw IntegerParseException();
            }else{
                return result;
            }
        }
    }

    /**
     * @brief 文字列を浮動小数点数に変換する
     * @param text 変換する文字列
     * @return 変換後の数値
     */
    template<typename T>
    static T parseFloat( const std::string &text ){
        try{
            return boost::lexical_cast<T>( text );
        }catch( boost::bad_lexical_cast & ){
            throw FloatParseException();
        }
    }

    /**
     * @brief 整数を文字列に変換する
     * @param value 変換する数値
     * @return 変換後の文字列
     */
    static std::string toString( int value, int baseNumber = 10 ){
        ostringstream oss;
        oss << uppercase << setbase( baseNumber ) << value;
        return oss.str();
    }

    /**
     * @brief 数値を文字列に変換する
     * @param value 変換する数値
     * @param format 変換時のフォーマット
     * @return 変換後の文字列
     */
    template<typename T>
    static string toString( T value, const string &format ){
        return (boost::format( format ) % value).str();
    }

    /**
     * @brief アルファベットの大文字を小文字に変換する
     * @param value 変換する文字列
     * @return 結果文字列
     */
    static std::string toLower( std::string value ){
        std::string result = value;
        transform( result.begin(), result.end(), result.begin(), _toLower );
        return result;
    }

    /**
     * @brief 文字列を指定回数繰り返した文字列を取得する
     * @param value 繰り返す文字列
     * @param count 繰り返す回数
     * @return 結果文字列
     */
    static std::string repeat( const std::string &value, int count ){
        ostringstream result;
        for( int i = 0; i < count; i++ ){
            result << value;
        }
        return result.str();
    }

    /**
     * @brief Trim leading and trailing blank characters.
     * @param value Source string.
     */
    static std::string trim(const std::string &s){
        std::string::const_iterator left
                = std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun(::isspace)));
        std::string::const_reverse_iterator right
                = std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun(::isspace)));
        return (left < right.base()) ? std::string(left, right.base()) : std::string();
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

    static char _toLower( char c ){
        return tolower( c );
    }
};

#endif
