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
#ifndef __VibratoBP_h__
#define __VibratoBP_h__

#include "vsqglobal.hpp"

VSQ_BEGIN_NAMESPACE

/**
 * ビブラートのデータポイント
 * @class table
 * @name VibratoBP
 */
class VibratoBP
{
public:
    /**
     * データ点の X 値
     * @var double
     */
    double x;

    /**
     * データ点の Y 値
     * @var int
     */
    int y;

public:
    VibratoBP();

    /**
     * 初期化を行う
     * @param x (double) x 軸の値
     * @param y (int) y 軸の値
     * @return (VibratoBP)
     */
    VibratoBP( double x, int y );

    /**
     * 順序を比較する
     * @param item (VibratoBP) 比較対象のアイテム
     * @return (int) このインスタンスが比較対象よりも小さい場合は負の整数、等しい場合は 0、大きい場合は正の整数を返す
     */
    int compareTo( const VSQ_NS::VibratoBP &item );

    /**
     * 2 つの {@link VibratoBP} を比較する
     * @param a (VibratoBP) 比較対象のオブジェクト
     * @param b (VibratoBP) 比較対象のオブジェクト
     * @return (boolean) <code>a</code> が <code>b</code> よりも小さい場合は <code>true</code>、そうでない場合は <code>false</code> を返す
     * @access static
     */
    static bool compare( const VSQ_NS::VibratoBP &a, const VSQ_NS::VibratoBP &b );

private:
    static int compareCore( const VSQ_NS::VibratoBP &a, const VSQ_NS::VibratoBP &b );

};

VSQ_END_NAMESPACE

#endif
