#ifndef __Tempo_h__
#define __Tempo_h__

#include "vsqglobal.h"
#include <string>

VSQ_BEGIN_NAMESPACE

/**
 * @brief テンポ情報テーブル内の要素を表現するクラス
 */
class Tempo
{
public:
    /**
     * Tick 単位の時刻
     * @var int
     */
    int clock;

    /**
     * テンポ値。四分音符の長さをマイクロ秒単位で表した値
     * @var int
     */
    int tempo;

protected:
    /**
     * テンポ変更の秒単位の時刻
     * @var double
     * @access private
     */
    double _time;

public:
    /**
     * 文字列に変換する
     * @return (string) 変換後の文字列
     */
    const std::string toString();

    /**
     * 初期化を行う
     * @param clock (int) Tick 単位の時刻
     * @param tempo (int) テンポ値。四分音符の長さをマイクロ秒単位で表した値
     * @return (Tempo)
     * @name new
     * @access static ctor
     */
    explicit Tempo( VSQ_NS::tick_t clock, int tempo );

    explicit Tempo();

    /**
     * 順序を比較する
     * @param entry (Tempo) 比較対象のアイテム
     * @return (int) このインスタンスが比較対象よりも小さい場合は負の整数、等しい場合は 0、大きい場合は正の整数を返す
     */
    int compareTo( const Tempo &entry );

    /**
     * このオブジェクトのインスタンスと、指定されたオブジェクトが同じかどうかを調べる
     * @param entry (Tempo) 比較対象のオブジェクト
     * @return (boolean) 比較対象と同じであれば <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    bool equals( const Tempo &entry );

    /**
     * 秒単位の時刻を取得する
     * @return (double) 秒単位の時刻
     */
    double getTime();

    /**
     * 2 つの {@link Tempo} を比較する
     * @param a (Tempo) 比較対象のオブジェクト
     * @param b (Tempo) 比較対象のオブジェクト
     * @return (boolean) <code>a</code> が <code>b</code> よりも小さい場合は <code>true</code>、そうでない場合は <code>false</code> を返す
     * @access static
     */
    static bool compare( const Tempo &a, const Tempo &b );

private:
    static int compareCore( const Tempo &a, const Tempo &b );

};

VSQ_END_NAMESPACE

#endif
