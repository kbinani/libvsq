#include <sstream>
#include "Lyric.h"
#include "StringUtil.h"
#include "PhoneticSymbol.h"
#include <iostream>

using namespace std;
using namespace VSQ_NS;

Lyric::Lyric( const std::string &line )
{
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

Lyric::Lyric( const string &phrase, const string &phoneticSymbol )
{
    this->phrase = phrase;
    _consonantAdjustment.clear();
    this->setPhoneticSymbol( phoneticSymbol );
    lengthRatio = 1.0;
    isProtected = false;
}

bool Lyric::equalsForSynth( Lyric &item )
{
    string thisSymbol = getPhoneticSymbol();
    string itemSymbol = item.getPhoneticSymbol();
    if( getPhoneticSymbol() != item.getPhoneticSymbol() ){
        return false;
    }
    if( getConsonantAdjustment() != item.getConsonantAdjustment() ){
        return false;
    }
    return true;
}

/**
 * このオブジェクトのインスタンスと、指定されたオブジェクトが同じかどうかを調べる
 * @param item (Lyric) 比較対象のオブジェクト
 * @return (boolean) 比較対象と同じであれば <code>true</code> を、そうでなければ <code>false</code> を返す
 */
bool Lyric::equals( Lyric &item )
{
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

const string Lyric::getConsonantAdjustment()
{
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

void Lyric::setConsonantAdjustment( const string &value )
{
    vector<string> spl = StringUtil::explode( ",", value );
    vector<int> arr;
    vector<string>::iterator i;
    for( i = spl.begin(); i != spl.end(); ++i ){
        arr.push_back( (int)atoi( (*i).c_str() ) );
    }
    setConsonantAdjustmentList( arr );
}

/**
 * Consonant Adjustment を、整数配列で取得する
 * @return (table<int>) Consonant Adjustment を格納した整数の配列
 */
const vector<int> Lyric::getConsonantAdjustmentList()
{
    if( _consonantAdjustment.empty() ){
        if( _phoneticSymbol.empty() ){
            _consonantAdjustment.clear();
        }else{
            _consonantAdjustment.clear();
            vector<string>::iterator i;
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

void Lyric::setConsonantAdjustmentList( const vector<int> &value )
{
    _consonantAdjustment.clear();
    vector<int>::const_iterator i;
    for( i = value.begin(); i != value.end(); ++i ){
        _consonantAdjustment.push_back( (*i) );
    }
}

const string Lyric::getPhoneticSymbol()
{
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

void Lyric::setPhoneticSymbol( const string &value )
{
    string s = StringUtil::replace( value, "  ", " " );
    _phoneticSymbol = StringUtil::explode( " ", s );
    for( int i = 0; i < _phoneticSymbol.size(); i++ ){
        _phoneticSymbol[i] = StringUtil::replace( _phoneticSymbol[i], "\\"  "\\", "\\" );
    }
}

const vector<string> Lyric::getPhoneticSymbolList()
{
    vector<string> ret;
    vector<string>::iterator i;
    for( i = _phoneticSymbol.begin(); i != _phoneticSymbol.end(); ++i ){
        ret.push_back( (*i) );
    }
    return ret;
}

/**
 * このインスタンスを文字列に変換する
 * @param addQuateMark (boolean) 歌詞、発音記号の前後に引用符(")を追加するかどうか
 * @return (string) 変換後の文字列
 */
const string Lyric::toString( bool addQuateMark )
{
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
    if( _consonantAdjustment.empty() ){
        vector<string>::iterator i;
        for( i = symbol.begin(); i != symbol.end(); ++i ){
            int consonantAdjustment;
            if( PhoneticSymbol::isConsonant( (*i) ) ){
                consonantAdjustment = 64;
            }else{
                consonantAdjustment = 0;
            }
            _consonantAdjustment.push_back( consonantAdjustment );
        }
    }
    vector<int>::iterator i;
    for( i = _consonantAdjustment.begin(); i != _consonantAdjustment.end(); ++i ){
        result << "," << (*i);
    }
    if( isProtected ){
        result << ",1";
    }else{
        result <<  ",0";
    }
    return result.str();
}
