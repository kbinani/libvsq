#ifndef __Event_h__
#define __Event_h__

#include <string>
#include "vsqglobal.h"
#include "EventType.h"
#include "Handle.h"
#include "EventWriteOption.h"

VSQ_BEGIN_NAMESPACE

/**
 * VSQ ファイルのメタテキスト内に記述されるイベントを表すクラス
 * @class table
 * @name Event
 */
class Event
{
    friend class EventList;

public:
    /**
     * ミリ秒で表した、音符の最大長さ
     * @access static
     */
    static const int MAX_NOTE_MILLISEC_LENGTH = 16383;

    /**
     * イベントに付けるタグ文字列
     * @var string
     */
    std::string tag;

    /**
     * 内部で使用するオブジェクト固有の ID
     * @var int
     */
    int id;

    /**
     * Tick 単位の時刻
     * @var int
     */
    VSQ_NS::tick_t clock;

    /**
     * VSQ メタテキストに出力されるこのオブジェクトの ID
     * @var int
     */
    int index;

    /**
     * イベントの種類
     * @var EventTypeEnum
     */
    VSQ_NS::EventType::EventTypeEnum type;

    /**
     * 歌手ハンドル
     * @var Handle
     */
    VSQ_NS::Handle singerHandle;

    /**
     * ノート番号
     * @var int
     */
    int note;

    /**
     * ベロシティ
     * @var int
     */
    int dynamics;

    /**
     * ベンド深さ
     * @var int
     */
    int pmBendDepth;

    /**
     * ベンド長さ
     * @var int
     */
    int pmBendLength;

    /**
     * ポルタメント
     * @var int
     */
    int pmbPortamentoUse;

    /**
     * ディケイ
     * @var int
     */
    int demDecGainRate;

    /**
     * アクセント
     * @var int
     */
    int demAccent ;

    /**
     * 歌詞ハンドル
     * @var Handle
     */
    VSQ_NS::Handle lyricHandle;

    /**
     * ビブラートハンドル
     * @var Handle
     */
    VSQ_NS::Handle vibratoHandle;

    /**
     * イベント先頭から測った、ビブラートの開始位置(Tick 単位)
     * @var int
     */
    int vibratoDelay ;

    /**
     * アタックハンドル
     * @var Handle
     */
    VSQ_NS::Handle noteHeadHandle;

    /**
     * @var int
     */
    int pMeanOnsetFirstNote;

    /**
     * @var int
     */
    int vMeanNoteTransition;

    /**
     * @var int
     */
    int d4mean;

    /**
     * @var int
     */
    int pMeanEndingNote;

    /**
     * 強弱記号ハンドル
     * @var Handle
     */
    VSQ_NS::Handle iconDynamicsHandle;

private:
    /**
     * Tick 単位のイベント長さ
     * @var int
     * @access private
     */
    VSQ_NS::tick_t _length;

    /**
     * @todo 未実装
     * @var UstEvent
    VSQ_NS::UstEvent ustEvent = nil;
     */

    /**
     * @brief
     */
    int _singerHandleIndex;

    /**
     * @brief
     */
    int _lyricHandleIndex;

    /**
     * @brief
     */
    int _vibratoHandleIndex;

    /**
     * @brief
     */
    int _noteHeadHandleIndex;

public:
    /**
     * 初期化を行う
     * @param line (string) VSQ メタテキスト中の [EventList] セクション内のイベント宣言文字列(ex."480=ID#0001")
     * @return (Event)
     * @name new<! *2 *>
     * @access static ctor
     */
    explicit Event( const std::string &line );

    /**
     * 初期化を行う。この初期化メソッドは末尾のイベントリストを表すインスタンスを初期化する
     * @return (Event)
     * @name new<! *1 *>
     * @access static ctor
     */
    explicit Event();

    /**
     * 初期化を行う
     * @param clock (int) Tick 単位の時刻
     * @param eventType (EventTypeEnum) イベントの種類
     * @return (Event)
     * @name new<! *3 *>
     * @access static ctor
     */
    explicit Event( VSQ_NS::tick_t clock, VSQ_NS::EventType::EventTypeEnum eventType );

    /**
     * テキストファイルからのコンストラクタ
     * @param sr [TextStream] 読み込み対象
     * @param value [int]
     * @param last_line [ByRef<string>] 読み込んだ最後の行が返されます
     * @return (Id)
     *
    Event( TextStream &sr, int value, std::string &last_line );
     */

    /**
     * 長さを取得する
     * @return (int) 長さ
     */
    VSQ_NS::tick_t getLength() const;

    /**
     * 長さを設定する
     * @param value (int) 長さ
     */
    void setLength( VSQ_NS::tick_t value );

    /**
     * テキストストリームに書き出す
     * @param stream (TextStream) 出力先
     * @param printTargets (table) 出力するアイテムのリスト
     * @name write<! *2 *>
     */
    void write( VSQ_NS::TextStream &stream, VSQ_NS::EventWriteOption printTargets = EventWriteOption() ) const;

    /**
     * このオブジェクトがイベントリストの末尾の要素( EOS )かどうかを取得する
     * @return (boolean) このオブジェクトが EOS 要素であれば <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    bool isEOS() const;

    /**
     * 順序を比較する
     * @param item (Event) 比較対象のアイテム
     * @return (int) このインスタンスが比較対象よりも小さい場合は負の整数、等しい場合は 0、大きい場合は正の整数を返す
     */
    int compareTo( const VSQ_NS::Event &item ) const;

    /**
     * 2 つの {@link Event} を比較する
     * @param a (Event) 比較対象のオブジェクト
     * @param b (Event) 比較対象のオブジェクト
     * @return (boolean) <code>a</code> が <code>b</code> よりも小さい場合は <code>true</code>、そうでない場合は <code>false</code> を返す
     * @access static
     */
    static bool compare( const VSQ_NS::Event &a, const VSQ_NS::Event &b );

    /**
     * イベントリストの末尾の要素を表すオブジェクトを取得する
     * @return (Event) オブジェクト
     * @access static
     */
    static const VSQ_NS::Event getEOS();

private:
    void init();

};

VSQ_END_NAMESPACE

#endif
