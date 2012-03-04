#ifndef __BP_h__
#define __BP_h__

#include "vsqglobal.h"

VSQ_BEGIN_NAMESPACE

/**
 * @brief コントロールカーブのデータ点を表現するクラス
 */
class BP
{
public:
    /**
     * @brief データ点の値
     */
    int value;

    /**
     * @brief データ点のユニーク ID
     */
    int id;

    /**
     * @brief コンストラクタ
     * @param value (int) データ点の値
     * @param id (int) データ点のユニーク ID
     */
    explicit BP( int value, int id )
    {
        this->value = value;
        this->id = id;
    }

private:
    BP()
    {
    }
};

VSQ_END_NAMESPACE

#endif
