/**
 * Lyric.h
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
#ifndef __Lyric_h__
#define __Lyric_h__

#include "vsqglobal.hpp"
#include "StringUtil.hpp"
#include "PhoneticSymbol.hpp"
#include <string>
#include <vector>

VSQ_BEGIN_NAMESPACE

using namespace std;
using namespace VSQ_NS;

/**
 * 歌詞ハンドルに格納する歌詞情報を保持するクラス
 * @class table
 * @name Lyric
 */
class Lyric
{
public:
    /**
     * 歌詞
     * @var string
     */
    std::string phrase;// = "a";

    /**
     * 歌詞ハンドル内に複数の歌詞が入る場合の、この歌詞の長さ分率。デフォルトは 1.0
     * @var double
     */
    double lengthRatio;// = 1.0;

    /**
     * 発音記号がプロテクトされた状態かどうか
     * @var boolean
     */
    bool isProtected;// = false;

    /**
     * 発音記号のリスト
     * @var table
     * @access private
     */
    std::vector<std::string> _phoneticSymbol;// = { "a" };

    /**
     * Consonant Adjustment のリスト
     * @var table
     * @access private
     */
    std::vector<int> _consonantAdjustment;// = { 0 };

    /**
     * @brief 文字列を元に初期化を行う
     * @param line 「"あ","a",0.0000,0.0」などのような文字列
     */
    explicit Lyric( const std::string &line ){
        if( line.size() == 0 ){
            phrase = "a";
            setPhoneticSymbol( "a" );
            lengthRatio = 1.0;
            isProtected = false;
            setConsonantAdjustment( "0" );
            return;
        }
        int len = line.size();
        int indx = 0;
        int dquote_count = 0;
        string work = "";
        string consonantAdjustment = "";
        for( int i = 0; i < len; i++ ){
            char c = line[i];
            if( c == ',' || i + 1 == len ){
                if( i + 1 == len ){
                    work = work + c;
                }
                if( dquote_count % 2 == 0 ){
                    // ,の左側に偶数個の"がある場合→,は区切り文字
                    indx = indx + 1;
                    string search = "\"";
                    if( indx == 1 ){
                        // phrase
                        work = StringUtil::replace( work, "\"\"", "\"" ); // "は""として保存される
                        if( work.find( search ) == 0 && work.find_last_of( search ) == (work.size() - search.size()) ){
                            int l = work.size();
                            if( l > 2 ){
                                phrase = work.substr( 0, l - 3 );
                            }else{
                                phrase = "a";
                            }
                        }else{
                            phrase = work;
                        }
                        work = "";
                    }else if( indx == 2 ){
                        // symbols
                        string symbols = "";
                        if( (work.find( search ) == 0) && (work.find_last_of( search ) == (work.size() - search.size())) ){
                            int l = work.size();
                            if( l > 2 ){
                                symbols = work.substr( 0, l - 3 );
                            }else{
                                symbols = "a";
                            }
                        }else{
                            symbols = work;
                        }
                        setPhoneticSymbol( symbols );
                        work = "";
                    }else if( indx == 3 ){
                        // lengthRatio
                        lengthRatio = atof( work.c_str() );
                        work = "";
                    }else{
                        if( indx - 3 <= _phoneticSymbol.size() ){
                            // consonant adjustment
                            if( indx - 3 == 1 ){
                                consonantAdjustment = consonantAdjustment + work;
                            }else{
                                consonantAdjustment = consonantAdjustment + "," + work;
                            }
                        }else{
                            // protected
                            isProtected = (work == "1");
                        }
                        work = "";
                    }
                }else{
                    // ,の左側に奇数個の"がある場合→,は歌詞等の一部
                    work = work + "" + c;
                }
            }else{
                work = work + "" + c;
                if( c == '"' ){
                    dquote_count = dquote_count + 1;
                }
            }
        }
        setConsonantAdjustment( consonantAdjustment );
    }

    /**
     * @brief 歌詞、発音記号を指定して初期化を行う
     * @param phrase 歌詞
     * @param phoneticSymbol 発音記号
     */
    explicit Lyric( const std::string &phrase, const std::string &phoneticSymbol ){
        this->phrase = phrase;
        _consonantAdjustment.clear();
        this->setPhoneticSymbol( phoneticSymbol );
        lengthRatio = 1.0;
        isProtected = false;
    }

    /**
     * @brief このオブジェクトと、指定されたオブジェクトが同じかどうかを調べる。
     * 音声合成したときに影響のある範囲のフィールドしか比較されない。
     * たとえば、<code>isProtected</code> が <code>this</code> と <code>item</code> で違っていても、他が同一であれば <code>true</code> が返る。
     * @param item 比較対象のオブジェクト
     * @return 比較対象と同じであれば <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    bool equalsForSynth( Lyric &item ) const{
        if( getPhoneticSymbol() != item.getPhoneticSymbol() ){
            return false;
        }
        if( getConsonantAdjustment() != item.getConsonantAdjustment() ){
            return false;
        }
        return true;
    }

    /**
     * @brief このオブジェクトのインスタンスと、指定されたオブジェクトが同じかどうかを調べる
     * @param item (Lyric) 比較対象のオブジェクト
     * @return (boolean) 比較対象と同じであれば <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    bool equals( Lyric &item ) const{
        if( false == equalsForSynth( item ) ){
            return false;
        }
        if( isProtected != item.isProtected ){
            return false;
        }
        if( phrase != item.phrase ){
            return false;
        }
        if( lengthRatio != item.lengthRatio ){
            return false;
        }
        return true;
    }

    /**
     * @brief Consonant Adjustmentの文字列形式を取得する
     * @return Consonant Adjustment を空白区切りで連結した文字列
     */
    const std::string getConsonantAdjustment() const{
        const vector<int> arr = getConsonantAdjustmentList();
        if( arr.empty() ){
            return "";
        }
        vector<int>::const_iterator i;
        ostringstream ret;
        for( i = arr.begin(); i != arr.end(); ++i ){
            ret << (i == arr.begin() ? "" : " ") << (*i);
        }
        return ret.str();
    }

    /**
     * @brief Consonant Adjustmentを文字列形式で設定する
     * @param value Consonant Adjustment を空白区切りで連結した文字列
     */
    void setConsonantAdjustment( const std::string &value ){
        vector<string> spl = StringUtil::explode( ",", value );
        vector<int> arr;
        vector<string>::iterator i;
        for( i = spl.begin(); i != spl.end(); ++i ){
            arr.push_back( (int)atoi( (*i).c_str() ) );
        }
        setConsonantAdjustmentList( arr );
    }

    /**
     * @brief Consonant Adjustment を、整数配列で取得する
     * @return Consonant Adjustment を格納した整数の配列
     */
    const std::vector<int> getConsonantAdjustmentList() const{
        vector<int> _consonantAdjustment = this->_consonantAdjustment;
        if( _consonantAdjustment.empty() ){
            if( _phoneticSymbol.empty() ){
                _consonantAdjustment.clear();
            }else{
                _consonantAdjustment.clear();
                vector<string>::const_iterator i;
                for( i = _phoneticSymbol.begin(); i != _phoneticSymbol.end(); ++i ){
                    int consonantAdjustment;
                    if( PhoneticSymbol::isConsonant( (*i) ) ){
                        consonantAdjustment = 64;
                    }else{
                        consonantAdjustment = 0;
                    }
                    _consonantAdjustment.push_back( consonantAdjustment );
                }
            }
        }
        return _consonantAdjustment;
    }

    /**
     * @brief Consonant Adjustment を、整数配列形式で設定する
     * @param value Consonant Adjustment を格納した整数の配列
     */
    void setConsonantAdjustmentList( const std::vector<int> &value ){
        _consonantAdjustment.clear();
        vector<int>::const_iterator i;
        for( i = value.begin(); i != value.end(); ++i ){
            _consonantAdjustment.push_back( (*i) );
        }
    }

    /**
     * @brief この歌詞の発音記号を取得する
     * @return 発音記号
     */
    const std::string getPhoneticSymbol() const{
        const vector<string> symbol = getPhoneticSymbolList();
        if( symbol.empty() ){
            return string( "" );
        }
        ostringstream result;
        vector<string>::const_iterator i;
        for( i = symbol.begin(); i != symbol.end(); ++i ){
            result << (i == symbol.begin() ? "" : " ") << (*i);
        }
        return result.str();
    }

    /**
     * @brief この歌詞の発音記号を設定する
     * @param value 発音記号
     */
    void setPhoneticSymbol( const std::string &value ){
        string s = StringUtil::replace( value, "  ", " " );
        _phoneticSymbol = StringUtil::explode( " ", s );
        for( int i = 0; i < _phoneticSymbol.size(); i++ ){
            _phoneticSymbol[i] = StringUtil::replace( _phoneticSymbol[i], "\\"  "\\", "\\" );
        }
    }

    /**
     * @brief この歌詞の発音記号の配列を取得する
     * @return 発音記号の配列
     */
    const std::vector<std::string> getPhoneticSymbolList() const{
        vector<string> ret;
        vector<string>::const_iterator i;
        for( i = _phoneticSymbol.begin(); i != _phoneticSymbol.end(); ++i ){
            ret.push_back( (*i) );
        }
        return ret;
    }

    /**
     * @brief このインスタンスを文字列に変換する
     * @param addQuateMark (boolean) 歌詞、発音記号の前後に引用符(")を追加するかどうか
     * @return (string) 変換後の文字列
     */
    const std::string toString( bool addQuateMark = false ) const{
        string quot;
        if( addQuateMark ){
            quot = "\"";
        }else{
            quot = "";
        }
        ostringstream result;
        result << quot << phrase << quot << ",";
        vector<string> symbol = getPhoneticSymbolList();
        string strSymbol = getPhoneticSymbol();
        if( false == addQuateMark ){
            if( strSymbol.size() == 0 ){
                strSymbol = "u:";
            }
        }
        result << quot << strSymbol << quot << "," << lengthRatio;

        string escaped = StringUtil::replace( result.str(), "\\"  "\\", "\\" );
        result.str( "" );
        result.clear( ostringstream::goodbit );
        result << escaped;
        vector<int> consonantAdjustment = _consonantAdjustment;
        if( consonantAdjustment.empty() ){
            vector<string>::iterator i;
            for( i = symbol.begin(); i != symbol.end(); ++i ){
                int adjustment;
                if( PhoneticSymbol::isConsonant( (*i) ) ){
                    adjustment = 64;
                }else{
                    adjustment = 0;
                }
                consonantAdjustment.push_back( adjustment );
            }
        }
        vector<int>::iterator i;
        for( i = consonantAdjustment.begin(); i != consonantAdjustment.end(); ++i ){
            result << "," << (*i);
        }
        if( isProtected ){
            result << ",1";
        }else{
            result <<  ",0";
        }
        return result.str();
    }
};

VSQ_END_NAMESPACE

#endif
