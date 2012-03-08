#ifndef __MidiEvent_h__
#define __MidiEvent_h__

#include <cmath>
#include <vector>
#include "vsqglobal.hpp"
#include "OutputStream.hpp"

using namespace std;

VSQ_BEGIN_NAMESPACE

/**
 * MIDI イベントを表現するクラス。
 * メタイベントは、メタイベントのデータ長をData[1]に格納せず、生のデータをDataに格納するので、注意が必要
 * @class table
 * @name MidiEvent
 * @access private
 */
class MidiEvent
{
public:
    /**
     * @brief Tick 単位の時刻
     */
    VSQ_NS::tick_t clock;

    /**
     * @brief MIDI イベントの先頭バイト
     */
    int firstByte;

    /**
     * @brief MIDI イベントのデータ。
     * メタイベントについては長さ値を保持せず、出力時に <code>data</code> フィールドの長さに応じた値を自動的に出力する
     */
    std::vector<int> data;

    MidiEvent()
    {
        clock = 0;
        firstByte = 0;
    }

    /**
     * MIDI データをストリームに出力する
     * @param stream (? extends OutputStream) 出力先のストリーム
     */
    void writeData( VSQ_NS::OutputStream *stream ) const
    {
        stream->write( firstByte );
        int size = (int)data.size();
        if( 0 < size ){
            char *buffer = new char[size]();
            for( int i = 0; i < size; i++ ){
                buffer[i] = (char)data[i];
            }
            if( firstByte == 0xff ){
                stream->write( buffer[0] );
                writeDeltaClock( stream, size - 1 );
                stream->write( buffer, 1, size - 1 );
            }else{
                stream->write( buffer, 0, size );
            }
            delete [] buffer;
        }
    }

    /**
     * @brief 順序を比較する
     * @param item (MidiEvent) 比較対象のアイテム
     * @return (int) このインスタンスが比較対象よりも小さい場合は負の整数、等しい場合は 0、大きい場合は正の整数を返す
     */
    int compareTo( const MidiEvent &item ) const
    {
        if( clock != item.clock ){
            return clock - item.clock;
        }else{
            int first_this = firstByte & 0xf0;
            int first_item = item.firstByte & 0xf0;

            if( (first_this == 0x80 || first_this == 0x90) && (first_item == 0x80 || first_item == 0x90) ){
                if( !data.empty() && data.size() >= 2 && !item.data.empty() && item.data.size() >= 2 ){
                    if( first_item == 0x90 && item.data[1] == 0 ){
                        first_item = 0x80;
                    }
                    if( first_this == 0x90 && data[1] == 0 ){
                        first_this = 0x80;
                    }
                    if( data[0] == item.data[0] ){
                        if( first_this == 0x90 ){
                            if( first_item == 0x80 ){
                                // ON -> OFF
                                return 1;
                            }else{
                                // ON -> ON
                                return 0;
                            }
                        }else{
                            if( first_item == 0x80 ){
                                // OFF -> OFF
                                return 0;
                            }else{
                                // OFF -> ON
                                return -1;
                            }
                        }
                    }
                }
            }
            return clock - item.clock;
        }
    }

    /**
     * 拍子イベントを作成する
     * @param clock (int) Tick 単位の時刻
     * @param numerator (int) 拍子の分子の値
     * @param denominator (int) 表紙の分母の値
     * @return (MidiEvent) 拍子イベント
     * @access static
     */
    static MidiEvent generateTimeSigEvent( VSQ_NS::tick_t clock, int numerator, int denominator )
    {
        MidiEvent ret;
        ret.clock = clock;
        ret.firstByte = 0xff;
        int b_numer = (int)::floor( ::log2( denominator ) + 0.1 );
        ret.data.push_back( 0x58 );
        ret.data.push_back( numerator );
        ret.data.push_back( b_numer );
        ret.data.push_back( 0x18 );
        ret.data.push_back( 0x08 );
        return ret;
    }

    /**
     * テンポイベントを作成する
     * @param clock (int) Tick 単位の時刻
     * @param tempo (int) 四分音符のマイクロ秒単位の長さ
     * @return (MidiEvent) テンポイベント
     * @name generateTempoChangeEvent
     * @access static
     */
    static MidiEvent generateTempoChangeEvent( VSQ_NS::tick_t clock, int tempo )
    {
        MidiEvent ret;
        ret.clock = clock;
        ret.firstByte = 0xff;
        int b1 = tempo & 0xff;
        tempo = tempo >> 8;
        int b2 = tempo & 0xff;
        tempo = tempo >> 8;
        int b3 = tempo & 0xff;
        ret.data.push_back( 0x51 );
        ret.data.push_back( b3 );
        ret.data.push_back( b2 );
        ret.data.push_back( b1 );
        return ret;
    }

    /**
     * @brief 可変長のデルタタイムをストリームに出力する
     * @param stream (? extends OutputStream) 出力先のストリーム
     * @param number (int) デルタタイム
     * @name writeDeltaClock
     * @access static
     */
    static void writeDeltaClock( VSQ_NS::OutputStream *stream, int number )
    {
        std::vector<bool> bits;
        long val = 0x1;
        bits.push_back( (number & val) == val );
        for ( int i = 1; i < 64; i++ ) {
            val = val << 1;
            bits.push_back( (number & val) == val );
        }
        int first = 0;
        for( int i = 63; i >= 0; i-- ){
            if( bits[i] ){
                first = i;
                break;
            }
        }
        // 何バイト必要か？
        int bytes = first / 7 + 1;
        for( int i = 1; i <= bytes; i++ ){
            int num = 0;
            long count = 0x80;
            for( int j = (bytes - i + 1) * 7 - 1; j >= (bytes - i + 1) * 7 - 6 - 1; j-- ){
                count = count >> 1;
                if( bits[j] ){
                    num += count;
                }
            }
            if( i != bytes ){
                num += 0x80;
            }
            stream->write( num );
        }
    }

    /**
     * @param stream [ByteArrayInputStream]
     * @return [long]
    MidiEvent.readDeltaClock = function( stream ){
        local ret = 0; // [long]
        while ( true ) {
            local i = stream.read();
            if( i < 0 ){
                break;
            }
            local d = i; // [byte]
            ret = (ret << 7) | (d & 0x7f);
            if( (d & 0x80) == 0x00 ){
                break;
            }
        }
        return ret;
    }*/

    /**
     * @param stream [ByteArrayInputStream]
     * @param last_clock [ByRef<Long>]
     * @param last_status_byte [ByRef<Integer>]
    MidiEvent.read = function( stream, last_clock, last_status_byte ){
        local delta_clock = this.readDeltaClock( stream ); // [long]
        last_clock.value += delta_clock;
        local first_byte = stream.read(); // [int]
        if( first_byte < 0x80 ){
            // ランニングステータスが適用される
            local pos = stream.getFilePointer();
            stream.seek( pos - 1 );
            first_byte = last_status_byte.value;
        }else{
            last_status_byte.value = first_byte;
        }
        local ctrl = first_byte & 0xf0;
        if( ctrl == 0x80 || ctrl == 0x90 || ctrl == 0xA0 || ctrl == 0xB0 || ctrl == 0xE0 || first_byte == 0xF2 ){
            // 3byte使用するチャンネルメッセージ：
            //     0x8*: ノートオフ
            //     0x9*: ノートオン
            //     0xA*: ポリフォニック・キープレッシャ
            //     0xB*: コントロールチェンジ
            //     0xE*: ピッチベンドチェンジ
            // 3byte使用するシステムメッセージ
            //     0xF2: ソングポジション・ポインタ
            local me = new MidiEvent(); // [MidiEvent]
            me.clock = last_clock.value;
            me.firstByte = first_byte;
            me.data = new Array( 2 ); //int[2];
            local d = new Array( 2 ); // byte[2];
            stream.readArray( d, 0, 2 );
            for ( local i = 0; i < 2; i++ ) {
                me.data[i] = 0xff & d[i];
            }
            return me;
        }else if( ctrl == 0xC0 || ctrl == 0xD0 || first_byte == 0xF1 || first_byte == 0xF2 ){
            // 2byte使用するチャンネルメッセージ
            //     0xC*: プログラムチェンジ
            //     0xD*: チャンネルプレッシャ
            // 2byte使用するシステムメッセージ
            //     0xF1: クォータフレーム
            //     0xF3: ソングセレクト
            local me = new MidiEvent(); // [MidiEvent]
            me.clock = last_clock.value;
            me.firstByte = first_byte;
            me.data = new Array( 1 );// int[1];
            local d = new Array( 1 );// byte[1];
            stream.readArray( d, 0, 1 );
            me.data[0] = 0xff & d[0];
            return me;
        }else if( first_byte == 0xF6 ){
            // 1byte使用するシステムメッセージ
            //     0xF6: チューンリクエスト
            //     0xF7: エンドオブエクスクルーシブ（このクラスではF0ステータスのSysExの一部として取り扱う）
            //     0xF8: タイミングクロック
            //     0xFA: スタート
            //     0xFB: コンティニュー
            //     0xFC: ストップ
            //     0xFE: アクティブセンシング
            //     0xFF: システムリセット
            local me = new MidiEvent(); // [MidiEvent]
            me.clock = last_clock.value;
            me.firstByte = first_byte;
            me.data = new Array(); //int[0];
            return me;
        }else if( first_byte == 0xff ){
            // メタイベント
            local meta_event_type = stream.read(); //[int]
            local meta_event_length = this.readDeltaClock( stream ); // [long]
            local me = new MidiEvent(); //[MidiEvent]
            me.clock = last_clock.value;
            me.firstByte = first_byte;
            me.data = new Array( meta_event_length + 1 ); // int[]
            me.data[0] = meta_event_type;
            local d = new Array( meta_event_length + 1 ); // byte[]
            stream.readArray( d, 1, meta_event_length );
            for ( local i = 1; i < meta_event_length + 1; i++ ) {
                me.data[i] = 0xff & d[i];
            }
            return me;
        }else if( first_byte == 0xf0 ){
            // f0ステータスのSysEx
            local me = new MidiEvent();// [MidiEvent]
            me.clock = last_clock.value;
            me.firstByte = first_byte;
            local sysex_length = this.readDeltaClock( stream ); // [long]
            me.data = new Array( sysex_length + 1 ); // int[]
            local d = new Array( sysex_length + 1 ); // byte[]
            stream.readArray( d, 0, sysex_length + 1 );
            for ( local i = 0; i < sysex_length + 1; i++ ) {
                me.data[i] = 0xff & d[i];
            }
            return me;
        }else if( first_byte == 0xf7 ){
            // f7ステータスのSysEx
            local me = new MidiEvent();
            me.clock = last_clock.value;
            me.firstByte = first_byte;
            local sysex_length = this.readDeltaClock( stream );
            me.data = new Array( sysex_length );
            local d = new Array( sysex_length );//byte[]
            stream.readArray( d, 0, sysex_length );
            for ( local i = 0; i < sysex_length; i++ ) {
                me.data[i] = 0xff & d[i];
            }
            return me;
        }else{
            throw new Exception( "don't know how to process first_byte: 0x" + PortUtil.toHexString( first_byte ) );
        }
    }*/

    /**
     * 2 つの {@link MidiEvent} を比較する
     * @param a (MidiEvent) 比較対象のオブジェクト
     * @param b (MidiEvent) 比較対象のオブジェクト
     * @return (boolean) <code>a</code> が <code>b</code> よりも小さい場合は <code>true</code>、そうでない場合は <code>false</code> を返す
     * @name compare
     */
    static bool compare( const VSQ_NS::MidiEvent &a, const VSQ_NS::MidiEvent &b )
    {
        return (a.compareTo( b ) < 0);
    }
};

VSQ_END_NAMESPACE

#endif
