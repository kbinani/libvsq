#ifndef __Handle_h__
#define __Handle_h__

#include <string>
#include "vsqglobal.h"
#include "HandleType.h"
#include "ArticulationType.h"
#include "VibratoBPList.h"
#include "Lyric.h"
#include "TextStream.h"

VSQ_BEGIN_NAMESPACE

/**
 * @brief ハンドルを取り扱います。ハンドルにはLyricHandle、VibratoHandle、SingerHandleおよびNoteHeadHandleがある
 */
class Handle
{
public:
    /**
     * @brief 強弱記号の場合の、IconId の最初の5文字。
     */
    static const std::string ICONID_HEAD_DYNAFF;

    /**
     * @brief クレッシェンドの場合の、IconId の最初の5文字。
     */
    static const std::string ICONID_HEAD_CRESCEND;

    /**
     * @brief デクレッシェンドの場合の、IconId の最初の5文字。
     */
    static const std::string ICONID_HEAD_DECRESCEND;

    /**
     * @brief メタテキストに出力されるこのオブジェクトの ID
     */
    int index;

    /**
     * @brief ハンドルを特定するための文字列
     */
    std::string iconId;

    /**
     * @brief ハンドルの名前
     */
    std::string ids;

    /**
     * @brief ハンドルのオリジナル
     */
    int original;

    /**
     * @brief 歌手の歌唱言語を表す番号(バンクセレクト)。歌手ハンドルでのみ使われる
     */
    int language;

    /**
     * @brief 歌手の種類を表す番号(プログラムチェンジ)。歌手ハンドルでのみ使われる
     */
    int program ;

    /**
     * @brief 歌詞・発音記号列の前後にクォーテーションマークを付けるかどうか
     */
    bool addQuotationMark;

private:
    VSQ_NS::HandleType::HandleTypeEnum _type;

    VSQ_NS::ArticulationType::ArticulationTypeEnum _articulation;

    /**
     * @brief 歌詞一覧
     */
    std::vector<VSQ_NS::Lyric> _lyrics;

    /**
     * @brief キャプション
     */
    std::string _caption;

    /**
     * @brief 長さ
     */
    int _length;

    /**
     * @brief Depth の開始値
     */
    int _startDepth;

    /**
     * @brief Depth のカーブ
     */
    VSQ_NS::VibratoBPList _depthBP;

    /**
     * @brief Rate の開始値
     */
    int _startRate;

    /**
     * @brief Rate のカーブ
     */
    VSQ_NS::VibratoBPList _rateBP;

    /**
     * @brief Duration
     */
    int _duration;

    /**
     * @brief Depth
     */
    int _depth;

    /**
     * @brief Dyn の開始値
     */
    int _startDyn;

    /**
     * @brief Dyn の終了値
     */
    int _endDyn;

    /**
     * @brief Dyn のカーブ
     */
    VSQ_NS::VibratoBPList _dynBP;

public:
    /**
     * @param type ハンドルの種類
     */
    explicit Handle( VSQ_NS::HandleType::HandleTypeEnum type );

    /**
     * @brief テキストストリームからハンドルの内容を読み込み初期化する
     * @param stream 読み込み元のテキストストリーム
     * @param index <code>index</code> フィールドの値
     * @param lastLine 読み込んだ最後の行。テーブルの ["value"] に文字列が格納される
     */
    explicit Handle( VSQ_NS::TextStream &stream, int index, std::string &lastLine );

    /**
     * @brief articulation の種類を取得する
     * @return articulation の種類
     */
    VSQ_NS::ArticulationType::ArticulationTypeEnum getArticulation() const;

    /**
     * @brief このハンドルが強弱記号を表すものかどうかを表すブール値を取得する
     * @return このオブジェクトが強弱記号を表すものであれば <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    bool isDynaffType() const;

    /**
     * @brief このハンドルがクレッシェンドを表すものかどうかを表すブール値を取得する
     * @return このオブジェクトがクレッシェンドを表すものであれば <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    bool isCrescendType() const;

    /**
     * @brief このハンドルがデクレッシェンドを表すものかどうかを表すブール値を取得する
     * @return このオブジェクトがデクレッシェンドを表すものであれば <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    bool isDecrescendType() const;

    /**
     * @brief Tick 単位の長さを取得する
     * @return
     */
    VSQ_NS::tick_t getLength() const;

    /**
     * @brief 長さを設定する
     * @param value Tick単位の長さ
     */
    void setLength( VSQ_NS::tick_t value );

    /**
     * @brief キャプションを取得する
     * @return キャプション
     */
    const std::string getCaption() const;

    /**
     * @brief キャプションを設定する
     * @param value キャプション
     */
    void setCaption( const std::string &value );

    /**
     * @brief DYN の開始値を取得する
     * @return DYN の開始値
     */
    int getStartDyn() const;

    /**
     * @brief DYN の開始値を設定する
     * @param value DYN の開始値
     */
    void setStartDyn( int value );

    /**
     * @brief DYN の終了値を取得する
     * @return DYN の終了値
     */
    int getEndDyn() const;

    /**
     * @brief DYN の終了値を設定する
     * @param value DYN の終了値
     */
    void setEndDyn( int value );

    /**
     * @brief DYN カーブを取得する
     * @return DYN カーブ
     */
    const VSQ_NS::VibratoBPList getDynBP() const;

    /**
     * @brief DYN カーブを設定する
     * @param value DYN カーブ
     */
    void setDynBP( const VSQ_NS::VibratoBPList &value );

    /**
     * @brief Depth 値を取得する
     * @return  Depth 値
     */
    int getDepth() const;

    /**
     * @brief Depth 値を設定する
     * @param value Depth 値
     */
    void setDepth( int value );

    /**
     * @brief Duration 値を取得する
     * @return Duration 値
     */
    int getDuration() const;

    /**
     * @brief Duration 値を設定する
     * @param value Duration 値
     */
    void setDuration( int value );

    /**
     * @brief Rate のビブラートカーブを取得する
     * @return Rate のビブラートカーブ
     */
    const VSQ_NS::VibratoBPList getRateBP() const;

    /**
     * @brief Rate のビブラートカーブを設定する
     * @param value 設定するビブラートカーブ
     */
    void setRateBP( const VSQ_NS::VibratoBPList &value );

    /**
     * @brief Depth のビブラートカーブを取得する
     * @return Depth のビビラートカーブ
     */
    const VSQ_NS::VibratoBPList getDepthBP() const;

    /**
     * @brief Depth のビブラートカーブを設定する
     * @param value 設定するビブラートカーブ
     */
    void setDepthBP( const VSQ_NS::VibratoBPList &value );

    /**
     * @brief Rate の開始値を取得する
     * @return Rate の開始値
     */
    int getStartRate() const;

    /**
     * @brief Rate の開始値を設定する
     * @param value Rate の開始値
     */
    void setStartRate( int value );

    /**
     * @brief Depth の開始値を取得する
     * @return Depth の開始値
     */
    int getStartDepth() const;

    /**
     * @brief Depth の開始値を設定する
     * @param value Depth の開始値
     */
    void setStartDepth( int value );

    /**
     * @brief 指定した位置にある歌詞を取得する
     * @param index 取得する要素のインデックス(最初のインデックスは0)
     * @return 歌詞
     */
    const VSQ_NS::Lyric getLyricAt( int index ) const;

    /**
     * @brief 指定した位置にある歌詞を指定した要素で置き換える
     * @param index 置き換える要素のインデックス(最初のインデックスは0)
     * @param value 置き換える要素
     */
    void setLyricAt( int index, const VSQ_NS::Lyric &value );

    /**
     * @brief 歌詞の個数を返す
     * @return 歌詞の個数
     */
    int getLyricCount() const;

    /**
     * @brief Display String 値を取得する
     * @return Display String 値
     */
    const std::string getDisplayString() const;

    /**
     * @brief ハンドルのタイプを取得する
     * @return ハンドルのタイプ
     */
    VSQ_NS::HandleType::HandleTypeEnum getHandleType() const;

    /**
     * @brief ストリームに書き込む
     * @param stream 書き込み先のストリーム
     */
    void write( VSQ_NS::TextStream &stream );

    /**
     * @brief オブジェクトを文字列に変換する
     * @return 文字列
     */
    const std::string toString() const;

protected:
    void init();

    /**
     * @brief ビブラートハンドルとして初期化を行う
     */
    void _init_vibrato();

    /**
     * @brief 強弱記号ハンドルとして初期化を行う
     */
    void _init_icon_dynamics();

    /**
     * @brief 歌詞ハンドルとして初期化を行う
     */
    void _init_lyric();

    /**
     * @brief ハンドル指定子（例えば"h#0123"という文字列）からハンドル番号を取得する
     * @param s ハンドル指定子
     * @return ハンドル番号
     */
    static int getHandleIndexFromString( const std::string &s );

};

VSQ_END_NAMESPACE

#endif
