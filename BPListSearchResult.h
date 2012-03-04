#ifndef __BPListSearchResult_h__
#define __BPListSearchResult_h__

#include "vsqglobal.h"
#include "BP.h"

VSQ_BEGIN_NAMESPACE

/**
 * コントロールパラメータのデータ点検索結果を格納するクラス
 * @class table
 * @name BPListSearchResult
 */
class BPListSearchResult
{
public:
    /**
     * データ点の Tick 単位の時刻
     * @var int
     */
    VSQ_NS::tick_t clock;

    /**
     * データ点のインデックス。最初のインデックスは 0
     * @var int
     */
    int index;

    /**
     * データ点のオブジェクト
     * @var BP
     */
    VSQ_NS::BP point;

    /**
     * 初期化を行う
     * @return (BPListSearchResult)
     * @access static ctor
     */
    BPListSearchResult() :
        clock( 0 ),
        index( 0 ),
        point( 0, 0 )
    {
    }
};

VSQ_END_NAMESPACE

#endif
