#ifndef __VibratoBPList_h__
#define __VibratoBPList_h__

#include <vector>
#include "vsqglobal.h"
#include "VibratoBP.h"

VSQ_BEGIN_NAMESPACE

/**
 * ビブラートデータポイントのリストを表すクラス
 * @class table
 * @name VibratoBPList
 */
class VibratoBPList
{
private:
    /**
     * カーブのデータ点を保持するリスト
     * @var table<VibratoBP>
     */
    std::vector<VSQ_NS::VibratoBP> _list;

public:
    /**
     * 初期化を行う
     * @param textNum (string) データ点の個数の文字列表現
     * @param textBPX (string) x 軸のデータ点の値をカンマ区切りで繋げた文字列
     * @param textBPY (string) y 軸のデータ点の値をカンマ区切りで繋げた文字列
     * @return (VibratoBPList)
     */
    VibratoBPList( const std::string &textNum, const std::string &textBPX, const std::string &textBPY );

    /**
     * 初期化を行う
     * @param x (table<double>) x 軸の値のリスト
     * @param y (table<int>) y 軸の値のリスト
     * @return (VibratoBPList)
     */
    VibratoBPList( const std::vector<double> &x, const std::vector<int> &y );

    /**
     * 指定した位置のビブラートカーブの値を取得する
     * @param x (double) 取得したい x 軸の値
     * @param defaultValue (int) ビブラートカーブのデフォルト値
     * @return (int) ビブラートカーブの値
     */
    int getValueAt( double x, int defaultValue );

    /**
     * データ点の個数を返す
     * @return (int) データ点の個数
     */
    int size();

    /**
     * 指定したインデックスのデータ点を取得する
     * @param index (int) 0から始まるインデックス
     * @return (VibratoBP) データ点
     */
    const VibratoBP get( int index );

    /**
     * 指定したインデックスのデータ点を設定する
     * @param index (int) インデックス(最初のインデックスは0)
     * @param value (VibratoBP) 設定するデータ点
     */
    void set( int index, const VSQ_NS::VibratoBP value );

    /**
     * データ点のリストを、文字列に変換する。例えば "key1=value1,key2=value2" のように変換される
     * @return (string) 変換後の文字列
     */
    const std::string getData();

    /**
     * "key1=value=1,key2=value2" のような文字列から、データ点のリストを設定する
     * @param value (string) データ点の文字列形式
     */
    void setData( const std::string &value );
};

VSQ_END_NAMESPACE

#endif
