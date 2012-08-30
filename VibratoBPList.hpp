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
#ifndef __VibratoBPList_h__
#define __VibratoBPList_h__

#include "vsqglobal.hpp"
#include "VibratoBP.hpp"
#include "StringUtil.hpp"
#include <vector>
#include <boost/lexical_cast.hpp>

VSQ_BEGIN_NAMESPACE

/**
 * @brief ビブラートデータポイントのリストを表すクラス
 * @todo ビブラート用とは限らないので、クラス名を良いものに変える。例えば、NoteHeadHandleのDyn
 */
class VibratoBPList
{
private:
    /**
     * @brief カーブのデータ点を保持するリスト
     */
    std::vector<VSQ_NS::VibratoBP> _list;

public:
    /**
     * @brief 初期化を行う
     * @param textNum (string) データ点の個数の文字列表現
     * @param textBPX (string) x 軸のデータ点の値をカンマ区切りで繋げた文字列
     * @param textBPY (string) y 軸のデータ点の値をカンマ区切りで繋げた文字列
     * @return (VibratoBPList)
     */
    explicit VibratoBPList( const std::string &textNum, const std::string &textBPX, const std::string &textBPY ){
        int num = 0;
        try{
            num = boost::lexical_cast<int>( textNum );
        }catch( boost::bad_lexical_cast & ){
            num = 0;
        }
        vector<string> bpx = StringUtil::explode( ",", textBPX );
        vector<string> bpy = StringUtil::explode( ",", textBPY );
        int actNum = ::min( num, ::min( (int)bpx.size(), (int)bpy.size() ) );
        if( actNum > 0 ){
            double *x = new double[actNum]();
            int *y = new int[actNum]();
            try{
                for( int i = 0; i < actNum; i++ ){
                    x[i] = boost::lexical_cast<double>( bpx[i] );
                    y[i] = boost::lexical_cast<int>( bpy[i] );
                }
            }catch( boost::bad_lexical_cast & ){
                delete [] x;
                delete [] y;
                x = NULL;
                y = NULL;
            }

            if( x && y ){
                for( int i = 0; i < actNum; i++ ){
                    _list.push_back( VibratoBP( x[i], y[i] ) );
                }
                std::sort( _list.begin(), _list.end(), VibratoBP::compare );
                delete [] x;
                delete [] y;
            }
        }
    }

    /**
     * @brief 初期化を行う
     * @param x (table<double>) x 軸の値のリスト
     * @param y (table<int>) y 軸の値のリスト
     * @return (VibratoBPList)
     */
    explicit VibratoBPList( const std::vector<double> &x = std::vector<double>(), const std::vector<int> &y = std::vector<int>() ){
        int len = ::min( x.size(), y.size() );
        for( int i = 0; i < len; i++ ){
            _list.push_back( VibratoBP( x[i], y[i] ) );
        }
        std::sort( _list.begin(), _list.end(), VibratoBP::compare );
    }

    /**
     * @brief 指定した位置のビブラートカーブの値を取得する
     * @param x (double) 取得したい x 軸の値
     * @param defaultValue (int) ビブラートカーブのデフォルト値
     * @return (int) ビブラートカーブの値
     */
    int getValueAt( double x, int defaultValue ) const{
        if( _list.size() <= 0 ){
            return defaultValue;
        }
        int index = -1;
        for( int i = 0; i < _list.size(); i++ ){
            if( x < _list[i].x ){
                break;
            }
            index = i;
        }
        if( index == -1 ){
            return defaultValue;
        }else{
            return _list[index].y;
        }
    }

    /**
     * @brief コピーを作成する
     * @return (VibratoBPList) このオブジェクトのコピー
     */
    VibratoBPList clone() const{
        VibratoBPList ret;
        for( int i = 0; i < _list.size(); i++ ){
            ret._list.push_back( VibratoBP( _list[i].x, _list[i].y ) );
        }
        return ret;
    }

    /**
     * @brief データ点の個数を返す
     * @return (int) データ点の個数
     */
    int size() const{
        return (int)_list.size();
    }

    /**
     * @brief 指定したインデックスのデータ点を取得する
     * @param index (int) 0から始まるインデックス
     * @return (VibratoBP) データ点
     */
    const VibratoBP get( int index ) const{
        return _list[index];
    }

    /**
     * @brief 指定したインデックスのデータ点を設定する
     * @param index (int) インデックス(最初のインデックスは0)
     * @param value (VibratoBP) 設定するデータ点
     */
    void set( int index, const VSQ_NS::VibratoBP value ){
        _list[index] = value;
    }

    /**
     * @brief データ点のリストを、文字列に変換する。例えば "key1=value1,key2=value2" のように変換される
     * @return (string) 変換後の文字列
     */
    const std::string getData() const{
        ostringstream ret;
        for( int i = 0; i < _list.size(); i++ ){
            if( 0 < i ){
                ret << ",";
            }
            ret << _list[i].x << "=" << _list[i].y;
        }
        return ret.str();
    }

    /**
     * @brief "key1=value=1,key2=value2" のような文字列から、データ点のリストを設定する
     * @param value (string) データ点の文字列形式
     */
    void setData( const std::string &value ){
        _list.clear();
        vector<string> spl = StringUtil::explode( ",", value );
        vector<string>::iterator i;
        for( i = spl.begin(); i != spl.end(); ++i ){
            vector<string> spl2 = StringUtil::explode( "=", (*i) );
            if( spl2.size() >= 2 ){
                double x;
                int y;
                try{
                    x = boost::lexical_cast<double>( spl2[0] );
                    y = boost::lexical_cast<int>( spl2[1] );
                }catch( boost::bad_lexical_cast & ){
                    continue;
                }
                _list.push_back( VibratoBP( x, y ) );
            }
        }
        std::sort( _list.begin(), _list.end(), VibratoBP::compare );
    }

private:
    VibratoBPList( const std::vector<double> & ){
    }

};

VSQ_END_NAMESPACE

#endif
