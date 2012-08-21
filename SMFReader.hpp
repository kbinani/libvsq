/**
 * SMFReader.hpp
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
#ifndef __SMFReader_hpp__
#define __SMFReader_hpp__

#include "vsqglobal.hpp"
#include "InputStream.hpp"
#include "MidiEvent.hpp"
#include "BitConverter.hpp"
#include <vector>

VSQ_BEGIN_NAMESPACE

using namespace std;
using namespace VSQ_NS;

/**
 * @brief SMF(Standard MIDI File)の読み込みを行うクラス
 */
class SMFReader{
public:
    class ParseException : public std::exception{
    };

    /**
     * @brief ストリームから、SMF を読み込む
     * @param [in] stream 読み込むストリーム
     * @param [out] dest 読み込んだ MIDI イベントのリスト。MIDI イベントは、トラックごとに格納される
     * @param [out] format SMF のフォーマット
     * @param [out] timeFormat 時間分解能
     */
    void read( InputStream *stream, vector<vector<MidiEvent> > &dest, int &format, int &timeFormat ){
        // ヘッダ
        char byte4[4] = { 0 };
        stream->read( byte4, 0, 4 );
        if( BitConverter::makeUInt32BE( byte4 ) != 0x4d546864 ){
            throw ParseException();
        }

        // データ長
        stream->read( byte4, 0, 4 );

        // フォーマット
        stream->read( byte4, 0, 2 );
        format = BitConverter::makeUInt16BE( byte4 );

        // トラック数
        stream->read( byte4, 0, 2 );
        int tracks = (int)BitConverter::makeUInt16BE( byte4 );

        // 時間分解能
        stream->read( byte4, 0, 2 );
        timeFormat = BitConverter::makeUInt16BE( byte4 );

        // 各トラックを読込み
        dest.clear();
        for( int track = 0; track < tracks; track++ ){
            dest.push_back( vector<MidiEvent>() );

            // ヘッダー
            stream->read( byte4, 0, 4 );
            if( BitConverter::makeUInt32BE( byte4 ) != 0x4d54726b ){
                throw ParseException();// "header error; MTrk" );
            }

            // チャンクサイズ
            stream->read( byte4, 0, 4 );
            long size = (long)BitConverter::makeUInt32BE( byte4 );
            int64_t startpos = stream->getPointer();

            // チャンクの終わりまで読込み
            tick_t clock = 0;
            uint8_t last_status_byte = 0x00;
            while( stream->getPointer() < startpos + size ){
                MidiEvent mi = MidiEvent::read( stream, clock, last_status_byte );
                dest[track].push_back( mi );
            }
        }
    }
};

VSQ_END_NAMESPACE

#endif
