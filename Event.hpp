/**
 * Event.h
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
#ifndef __Event_h__
#define __Event_h__

#include <string>
#include <boost/format.hpp>
#include "vsqglobal.hpp"
#include "EventType.hpp"
#include "Handle.hpp"
#include "EventWriteOption.hpp"
#include "VoiceLanguageEnum.hpp"

VSQ_BEGIN_NAMESPACE

/**
 * VSQ ファイルのメタテキスト内に記述されるイベントを表すクラス
 * @class table
 * @name Event
 */
class Event
{
    friend class VSQFileReader; //TODO:Track::Track(TextStream &, ...)の機能がVSQFIleREaderに移動したら、ここは不要になるので消す
public:
    class ListIterator;

    /**
     * @brief 固有 ID 付きの {@link Event} のリストを取り扱うクラス
     */
    class List
    {
        friend class ListIterator;

    protected:
        /**
         * @brief イベントのリスト
         */
        std::vector<VSQ_NS::Event> _events;

        /**
         * @brief イベントの ID のリスト
         */
        std::vector<int> _ids;

    public:
        /**
         * @brief イベント ID を基にイベントを検索し、そのインデックスを返す
         * @param internalId 検索するイベント ID
         * @return 検索結果のインデックス(最初のインデックスは0)。イベントが見つからなければ負の値を返す
         */
        int findIndexFromId( int internalId ) const{
            int c = _events.size();
            for( int i = 0; i < c; i++ ){
                Event item = _events[i];
                if( item.id == internalId ){
                    return i;
                }
            }
            return -1;
        }

        /**
         * @brief イベント ID を基にイベントを検索し、そのオブジェクトを返す
         * @param internalId 検索するイベント ID
         * @return 検索結果のイベント。イベントが見つからなければ <code>nil</code> を返す
         */
        const VSQ_NS::Event *findFromId( int internalId ){
            int index = findIndexFromId( internalId );
            if( 0 <= index && index < _events.size() ){
                return &_events[index];
            }else{
                return NULL;
            }
        }

        /**
         * @brief 指定されたイベント ID をもつイベントのオブジェクトを置き換える。イベントが見つからなければ何もしない
         * @param internalId 検索するイベント ID
         * @param value 置換するオブジェクト
         */
        void setForId( int internalId, const VSQ_NS::Event &value ){
            int c = _events.size();
            for( int i = 0; i < c; i++ ){
                if( _events[i].id == internalId ){
                    _events[i] = value;
                    _events[i].id = internalId;
                    break;
                }
            }
        }

        /**
         * @brief イベントを並べ替える
         */
        void sort(){
            std::sort( _events.begin(), _events.end(), Event::compare );
            updateIdList();
        }

        /**
         * @brief 全てのイベントを削除する
         */
        void clear(){
            _events.clear();
            _ids.clear();
        }

        /**
         * @brief リスト内のイベントを順に返す反復子を取得する
         * @return 反復子
         */
        ListIterator iterator(){
            updateIdList();
            return ListIterator( this );
        }

        /**
         * @brief イベントを追加する
         * @param item 追加するオブジェクト
         * @return 追加したオブジェクトに割り振られたイベント ID
         */
        int add( const VSQ_NS::Event &item ){
            int id = _getNextId( 0 );
            _addCor( item, id );
            std::sort( _events.begin(), _events.end(), Event::compare );
            int count = _events.size();
            for( int i = 0; i < count; i++ ){
                _ids[i] = _events[i].id;
            }
            return id;
        }

        /**
         * @brief イベントを追加する
         * @param item 追加するオブジェクト
         * @param internalId 追加するオブジェクトに割り振るイベント ID
         * @return オブジェクトに割り振られたイベント ID
         */
        int add( const VSQ_NS::Event &item, int internalId ){
            _addCor( item, internalId );
            std::sort( _events.begin(), _events.end(), Event::compare );
            return internalId;
        }

        /**
         * @brief イベントを削除する
         * @param index 削除するイベントのインデックス(最初のインデックスは0)
         */
        void removeAt( int index ){
            updateIdList();
            _events.erase( _events.begin() + index );
            _ids.erase( _ids.begin() + index );
        }

        /**
         * @brief イベントの個数を返す
         * @return データ点の個数
         */
        int size() const{
            return _events.size();
        }

        /**
         * @brief 指定したインデックスのイベントを取得する
         * @param index インデックス(最初のインデックスは0)
         * @return イベント
         */
        const VSQ_NS::Event get( int index ) const{
            return _events[index];
        }

        /**
         * @brief 指定したインデックスのイベントを設定する
         * @param index インデックス(最初のインデックスは0)
         * @param value 設定するイベント
         */
        void set( int index, const VSQ_NS::Event &value ){
            int id = _events[index].id;
            _events[index] = value;
            _events[index].id = id;
        }

        /**
         * @brief リスト内部のイベント ID のデータを更新する
         */
        void updateIdList(){
            if( _ids.size() != _events.size() ){
                _ids.clear();
            }
            int count = _events.size();
            for( int i = 0; i < count; i++ ){
                _ids.push_back( _events[i].id );
            }
        }

    private:
        /**
         * @brief イベントを追加する
         * @param item 追加するオブジェクト
         * @param internal_id 追加するオブジェクトに割り振るイベント ID
         */
        void _addCor( const VSQ_NS::Event &item, int internalId ){
            updateIdList();
            Event add = item;
            add.id = internalId;
            _events.push_back( add );
            _ids.push_back( internalId );
        }

        /**
         * @brief イベントに割り振る ID を取得する
         * @param next
         * @return
         */
        int _getNextId( int next ){
            updateIdList();
            int max = -1;
            for( int i = 0; i < _ids.size(); i++ ){
                max = ::max( max, _ids[i] );
            }
            return max + 1 + next;
        }
    };

    /**
     * @brief イベントリストのアイテムを順に返す反復子
     */
    class ListIterator
    {
    private:
        /**
         * @brief 反復子の元になるリスト
         */
        List *_list;

        /**
         * @brief 反復子の現在の位置
         */
        int _pos;

    public:
        /**
         * @brief 初期化を行う
         * @param list 反復子の元になるリスト
         */
        ListIterator( List *list ) :
            _list( list ),
            _pos( -1 )
        {
        }

        /**
         * @brief 反復子が次の要素を持つ場合に <code>true</code> を返す
         * @return 反復子がさらに要素を持つ場合は <code>true</code> を、そうでなければ <code>false</code> を返す
         */
        bool hasNext()
        {
            if( 0 <= _pos + 1 && _pos + 1 < _list->size() ){
                return true;
            }
            return false;
        }

        /**
         * @brief 反復子の次の要素を返す
         * @return 次の要素
         */
        VSQ_NS::Event *next()
        {
            _pos++;
            return &_list->_events[_pos];
        }

        /**
         * @brief 反復子によって最後に返された要素を削除する
         */
        void remove()
        {
            if( 0 <= _pos && _pos < _list->size() ){
                _list->removeAt( _pos );
                _pos--;
            }
        }
    };

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
     * イベントの種類
     * @var EventTypeEnum
     */
    VSQ_NS::EventType::EventTypeEnum type;

    /**
     * @brief 歌手ハンドル
     * @todo 型をHandle*に変える
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
     * @brief 歌詞ハンドル
     * @todo 型をHandle*に変える
     */
    VSQ_NS::Handle lyricHandle;

    /**
     * @brief ビブラートハンドル
     * @todo 型をHandle*に変える
     */
    VSQ_NS::Handle vibratoHandle;

    /**
     * イベント先頭から測った、ビブラートの開始位置(Tick 単位)
     * @var int
     */
    int vibratoDelay ;

    /**
     * @brief アタックハンドル
     * @todo 型をHandle*に変える
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
     * @brief 強弱記号ハンドル
     * @todo 型をHandle*に変える
     */
    VSQ_NS::Handle iconDynamicsHandle;

    /**
     * @brief EOSイベントかどうか
     */
    bool isEos;

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

public:
    /**
     * @brief 初期化を行う
     * @param line (string) VSQ メタテキスト中の [EventList] セクション内のイベント宣言文字列(ex."480=ID#0001")
     */
    explicit Event( const std::string &line ) :
        vibratoHandle()
    {
        init();
        vector<string> spl = StringUtil::explode( "=", line );
        clock = boost::lexical_cast<tick_t>( spl[0] );
        isEos = (spl[1] == "EOS");
    }

    /**
     * @brief 初期化を行う。この初期化メソッドは末尾のイベントリストを表すインスタンスを初期化する
     */
    explicit Event(){
        init();
        clock = 0;
        isEos = false;
        id = 0;
    }

    /**
     * @brief 初期化を行う
     * @param clock (int) Tick 単位の時刻
     * @param eventType (EventTypeEnum) イベントの種類
     */
    explicit Event( VSQ_NS::tick_t clock, VSQ_NS::EventType::EventTypeEnum eventType ){
        init();
        this->clock = clock;
        this->type = eventType;
        if( eventType == EventType::SINGER ){
            singerHandle = Handle( HandleType::SINGER );
        }else if( eventType == EventType::NOTE ){
            lyricHandle = Handle( HandleType::LYRIC );
            lyricHandle.setLyricAt( 0, Lyric( "a", "a" ) );
        }
        id = 0;
    }

    /**
     * @brief 長さを取得する
     * @return (int) 長さ
     */
    VSQ_NS::tick_t getLength() const{
        return _length;
    }

    /**
     * @brief 長さを設定する
     * @param value (int) 長さ
     */
    void setLength( VSQ_NS::tick_t value ){
        _length = value;
    }

    /**
     * @brief コピーを作成する
     * @return (Event) このインスタンスのコピー
     */
    Event clone() const{
        Event result( clock, type );

        result.type = type;
        result.singerHandle = singerHandle.clone();
        result.setLength( getLength() );
        result.note = note;
        result.dynamics = dynamics;
        result.pmBendDepth = pmBendDepth;
        result.pmBendLength = pmBendLength;
        result.pmbPortamentoUse = pmbPortamentoUse;
        result.demDecGainRate = demDecGainRate;
        result.demAccent = demAccent;
        result.d4mean = d4mean;
        result.pMeanOnsetFirstNote = pMeanOnsetFirstNote;
        result.vMeanNoteTransition = vMeanNoteTransition;
        result.pMeanEndingNote = pMeanEndingNote;
        result.lyricHandle = lyricHandle.clone();
        result.vibratoHandle = vibratoHandle.clone();
        result.vibratoDelay = vibratoDelay;
        result.noteHeadHandle = noteHeadHandle.clone();
        result.iconDynamicsHandle = iconDynamicsHandle.clone();
        result.isEos = isEos;

        result.id = id;
        result.tag = tag;
        return result;
    }

    /**
     * このオブジェクトがイベントリストの末尾の要素( EOS )かどうかを取得する
     * @return (boolean) このオブジェクトが EOS 要素であれば <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    bool isEOS() const{
        return isEos;
    }

    /**
     * @brief 順序を比較する
     * @param item (Event) 比較対象のアイテム
     * @return (int) このインスタンスが比較対象よりも小さい場合は負の整数、等しい場合は 0、大きい場合は正の整数を返す
     */
    int compareTo( const VSQ_NS::Event &item ) const{
        tick_t ret = clock - item.clock;
        if( ret == 0 ){
            return type - item.type;
        }else{
            return (int)ret;
        }
    }

    /**
     * @brief 2 つの {@link Event} を比較する
     * @param a (Event) 比較対象のオブジェクト
     * @param b (Event) 比較対象のオブジェクト
     * @return (boolean) <code>a</code> が <code>b</code> よりも小さい場合は <code>true</code>、そうでない場合は <code>false</code> を返す
     */
    static bool compare( const VSQ_NS::Event &a, const VSQ_NS::Event &b ){
        return (a.compareTo( b ) < 0);
    }

    /**
     * @brief イベントリストの末尾の要素を表すオブジェクトを取得する
     * @return (Event) オブジェクト
     */
    static const VSQ_NS::Event getEOS(){
        return Event();
    }

    /**
        -- @param item [VsqEvent]
        -- @return [bool]
        -- @todo 実装できたら、TrackTest::testGetIndexIteratorNote, testGetIndexIteratorDynamics
                 の中のequals使うassertionを復活させること
        function this:equals( item )
            if( self.clock ~= item.clock )then
                return false;
            end
            if( self.type ~= item.type )then
                return false;
            end
            if( self.type == idType.Anote )then
                if( self.note ~= item.note )then
                    return false;
                end
                if( self:getLength() ~= item:getLength() )then
                    return false;
                end
                if( self.d4mean ~= item.d4mean )then
                    return false;
                end
                if( self.demAccent ~= item.demAccent )then
                    return false;
                end
                if( self.demDecGainRate ~= item.demDecGainRate )then
                    return false;
                end
                if( self.dynamics ~= item.dynamics )then
                    return false;
                end
                if( self.lyricHandle ~= nil and item.lyricHandle ~= nil )then
                    return false;
                end
                if( self.lyricHandle ~= nil and item.lyricHandle == nil )then
                    return false;
                end
                if( self.lyricHandle ~= nil and item.lyricHandle ~= nil )then
                    if( self.lyricHandle:size() ~= item.lyricHandle:size() )then
                        return false;
                    end
                    local count = self.lyricHandle:size();
                    local k;
                    for k = 0, count - 1, 1 do
                        if( not self.lyricHandle:getLyricAt( k ):equalsForSynth( item.lyricHandle:getLyricAt( k ) ) )then
                            return false;
                        end
                    end
                end
                if( self.noteHeadHandle == nil and item.noteHeadHandle ~= nil )then
                    return false;
                end
                if( self.noteHeadHandle ~= nil and item.noteHeadHandle == nil )then
                    return false;
                end
                if( self.noteHeadHandle ~= nil and item.noteHeadHandle ~= nil )then
                    if( self.NoteHeadHandle.iconId ~= item.noteHeadHandle.iconId )then
                        return false;
                    end
                    if( self.noteHeadHandle:getDepth() ~= item.noteHeadHandle:getDepth() )then
                        return false;
                    end
                    if( self.noteHeadHandle:getDuration() ~= item.noteHeadHandle:getDuration() )then
                        return false;
                    end
                    if( self.noteHeadHandle:getLength() ~= item.noteHeadHandle:getLength() )then
                        return false;
                    end
                end
                if( self.pmBendDepth ~= item.pmBendDepth )then
                    return false;
                end
                if( self.pmBendLength ~= item.pmBendLength )then
                    return false;
                end
                if( self.pmbPortamentoUse ~= item.pmbPortamentoUse )then
                    return false;
                end
                if( self.pMeanEndingNote ~= item.pMeanEndingNote )then
                    return false;
                end
                if( self.pMeanOnsetFirstNote ~= item.pMeanOnsetFirstNote )then
                    return false;
                end
                local hVibratoThis = self.vibratoHandle;
                local hVibratoItem = item.vibratoHandle;
                if( hVibratoThis == nil and hVibratoItem ~= nil )then
                    return false;
                end
                if( hVibratoThis ~= nil and hVibratoItem == nil )then
                    return false;
                end
                if( hVibratoThis ~= nil and hVibratoItem ~= nil )then
                    if( self.vibratoDelay ~= item.vibratoDelay )then
                        return false;
                    end
                    if( hVibratoThis.iconId ~= hVibratoItem.iconId )then
                        return false;
                    end
                    if( hVibratoThis:getStartDepth() ~= hVibratoItem:getStartDepth() )then
                        return false;
                    end
                    if( hVibratoThis:getStartRate() ~= hVibratoItem:getStartRate() )then
                        return false;
                    end
                    local vibRateThis = hVibratoThis:getRateBP();
                    local vibRateItem = hVibratoItem:getRateBP();
                    if( vibRateThis == nil and vibRateItem ~= nil )then
                        return false;
                    end
                    if( vibRateThis ~= nil and vibRateItem == nil )then
                        return false;
                    end
                    if( vibRateThis ~= nil and vibRateItem ~= nil )then
                        local numRateCount = vibRateThis:size();
                        if( numRateCount ~= vibRateItem:size() )then
                            return false;
                        end
                        local k;
                        for k = 0, numRateCount - 1, 1 do
                            local pThis = vibRateThis:getElement( k );
                            local pItem = vibRateItem:getElement( k );
                            if( pThis.x ~= pItem.x )then
                                return false;
                            end
                            if( pThis.y ~= pItem.y )then
                                return false;
                            end
                        end
                    end
                    local vibDepthThis = hVibratoThis:getDepthBP();
                    local vibDepthItem = hVibratoItem:getDepthBP();
                    if( vibDepthThis == nil and vibDepthItem ~= nil )then
                        return false;
                    end
                    if( vibDepthThis ~= nil and vibDepthItem == nil )then
                        return false;
                    end
                    if( vibDepthThis ~= nil and vibDepthItem ~= nil )then
                        local numDepthCount = vibDepthThis:size();
                        if( numDepthCount ~= vibDepthItem:size() )then
                            return false;
                        end
                        local k;
                        for k = 0, numDepthCount - 1, 1 do
                            local pThis = vibDepthThis:getElement( k );
                            local pItem = vibDepthItem:getElement( k );
                            if( pThis.x ~= pItem.x )then
                                return false;
                            end
                            if( pThis.y ~= pItem.y )then
                                return false;
                            end
                        end
                    end
                end
                if( self.vMeanNoteTransition ~= item.vMeanNoteTransition )then
                    return false;
                end
            elseif( self.type == EventTypeEnum.SINGER )then
                -- シンガーイベントの比較
                if( self.singerHandle.program ~= item.singerHandle.program )then
                    return false;
                end
            elseif( self.type == EventTypeEnum.ICON )then
                if( self.iconDynamicsHandle.iconId ~= item.iconDynamicsHandle.iconId )then
                    return false;
                end
                if( self.iconDynamicsHandle:isDynaffType() )then
                    -- 強弱記号
                else
                    -- クレッシェンド・デクレッシェンド
                    if( self:getLength() ~= item:getLength() )then
                        return false;
                    end
                end
            end

            return true;
        end*/

private:
    void init(){
        tag = "";
        id = -1;
        clock = 0;
        isEos = false;
        type = EventType::NOTE;
        _length = 0;
        note = 0;
        dynamics = 0;
        pmBendDepth = 0;
        pmBendLength = 0;
        pmbPortamentoUse = 0;
        demDecGainRate = 0;
        demAccent = 0;
        vibratoDelay = 0;
        pMeanOnsetFirstNote = 10;
        vMeanNoteTransition = 12;
        d4mean = 24;
        pMeanEndingNote = 12;
    //    ustEvent = nil;
    }

};

VSQ_END_NAMESPACE

#endif
