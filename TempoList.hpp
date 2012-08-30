/**
 * TempoList.hpp
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
#ifndef __TempoList_h__
#define __TempoList_h__

#include "vsqglobal.hpp"
#include "Tempo.hpp"
#include <vector>
#include <algorithm>

VSQ_BEGIN_NAMESPACE

/**
 * @brief テンポ情報を格納したテーブルを表すクラス
 */
class TempoList{
public:
    static const int gatetimePerQuater = 480;
    static const int baseTempo = 500000;

    class Iterator{
    private:
        std::vector<Tempo> *list;
        std::vector<Tempo>::iterator iterator;

    public:
        /**
         * @todo このコンストラクタは隠蔽すべき
         */
        Iterator( std::vector<Tempo> *list ){
            this->list = list;
            this->iterator = list->begin();
        }

        bool hasNext(){
            return iterator != this->list->end();
        }

        Tempo next(){
            Tempo result = *iterator;
            ++iterator;
            return result;
        }
    };

private:
    /**
     * @brief テンポ変更情報のリスト
     */
    std::vector<VSQ_NS::Tempo> _array;

public:
    /**
     * @brief リスト内のテンポ変更イベントを順に返す反復子を取得する
     * @return (List.Iterator<Tempo>) 反復子
     */
    TempoList::Iterator iterator(){
        return Iterator( &_array );
    }

    /**
     * @brief データ点を時刻順に並べ替える
     */
    void sort(){
        std::sort( _array.begin(), _array.end(), Tempo::compare );
    }

    /**
     * @brief データ点を追加する
     * @param value (Tempo) 追加するテンポ変更情報
     */
    void push( const Tempo &value ){
        _array.push_back( value );
    }

    /**
     * @brief リスト内のテンポ変更情報の個数を取得する
     * @return (int) テンポ変更情報の個数
     */
    int size() const{
        return _array.size();
    }

    /**
     * @brief 指定したインデックスのテンポ変更情報を取得する
     * @param index (int) インデックス(最初のインデックスは0)
     * @return (Tempo) テンポ変更情報
     */
    Tempo get( int index ) const{
        return _array[index];
    }

    /**
     * @brief 指定したインデックスのテンポ変更情報を設定する
     * @param index (int) インデックス(最初のインデックスは0)
     * @param value (Tempo) 設定するイベント
     */
    void set( int index, const Tempo &value ){
        _array[index] = value;
    }

    /**
     * @brief 時刻の単位を、秒単位から Tick 単位に変換する
     * @param time (double) 秒単位の時刻
     * @return (double) Tick 単位の時刻
     */
    double getClockFromSec( double time ) const{
        // timeにおけるテンポを取得
        int tempo = TempoList::baseTempo;
        tick_t base_clock = 0;
        double base_time = 0.0;
        int c = _array.size();
        if( c == 0 ){
            tempo = TempoList::baseTempo;
            base_clock = 0;
            base_time = 0.0;
        }else if( c == 1 ){
            tempo = _array[0].tempo;
            base_clock = _array[0].clock;
            base_time = _array[0]._time;
        }else{
            for( int i = c - 1; i >= 0; i-- ){
                Tempo item = _array[i];
                if( item._time < time ){
                    return item.clock + (time - item._time) * TempoList::gatetimePerQuater * 1000000.0 / item.tempo;
                }
            }
        }
        double dt = time - base_time;
        return base_clock + dt * TempoList::gatetimePerQuater * 1000000.0 / tempo;
    }

    /**
     * @brief リスト内のテンポ変更情報の秒単位の時刻部分を更新する
     */
    void updateTempoInfo(){
        int c = _array.size();
        if( c == 0 ){
            _array.push_back( Tempo( 0, TempoList::baseTempo ) );
        }
        std::sort( _array.begin(), _array.end(), Tempo::compare );
        Tempo item0 = _array[0];
        if( item0.clock != 0 ){
            item0._time = TempoList::baseTempo * item0.clock / (TempoList::gatetimePerQuater * 1000000.0);
        }else{
            item0._time = 0.0;
        }
        double prev_time = item0._time;
        tick_t prev_clock = item0.clock;
        int prev_tempo = item0.tempo;
        double inv_tpq_sec = 1.0 / (TempoList::gatetimePerQuater * 1000000.0);
        for( int i = 1; i < c; i++ ){
            _array[i]._time = prev_time + prev_tempo * (_array[i].clock - prev_clock) * inv_tpq_sec;

            Tempo itemi = _array[i];
            prev_time = itemi._time;
            prev_tempo = itemi.tempo;
            prev_clock = itemi.clock;
        }
    }

    /**
     * @brief 時刻の単位を、Tick 単位から秒単位に変換する
     * @param clock (double) Tick 単位の時刻
     * @return (double) 秒単位の時刻
     */
    double getSecFromClock( double clock ) const{
        int c = _array.size();
        for( int i = c - 1; i >= 0; i-- ){
            Tempo item = _array[i];
            if( item.clock < clock ){
                double init = item.getTime();
                tick_t dclock = clock - item.clock;
                double sec_per_clock1 = item.tempo * 1e-6 / 480.0;
                return init + dclock * sec_per_clock1;
            }
        }

        double sec_per_clock = TempoList::baseTempo * 1e-6 / 480.0;
        return clock * sec_per_clock;
    }

    /**
     * @brief 指定した時刻におけるテンポを取得する
     * @param clock (int) Tick 単位の時刻
     * @return (int) テンポ値。四分音符の長さをマイクロ秒単位で表した値
     * @todo clock の型をtick_tに修正
     */
    int getTempoAt( int clock ) const{
        int index = 0;
        int c = size();
        for( int i = c - 1; i >= 0; i-- ){
            index = i;
            if( _array[i].clock <= clock ){
                break;
            }
        }
        return _array[index].tempo;
    }

    /**
     * @brief リストをクリアする
     */
    void clear(){
        _array.clear();
    }
};

VSQ_END_NAMESPACE

#endif
