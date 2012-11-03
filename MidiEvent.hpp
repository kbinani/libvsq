#ifndef __MidiEvent_h__
#define __MidiEvent_h__

#include <cmath>
#include <vector>
#include "vsqglobal.hpp"
#include "OutputStream.hpp"
#include "InputStream.hpp"
#include "StringUtil.hpp"

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
    class ParseException : public std::exception{
        std::string message;
    public:
        explicit ParseException( const std::string &message )
            : message( message )
        {
        }

        ~ParseException() throw() {
        }

        std::string getMessage() const{
            return message;
        }
    };

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
        int b_numer = (int)::floor( ::log( (double)denominator ) / ::log( 2.0 ) + 0.1 );
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
        uint64_t val = 0x1;
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
     * @brief ストリームから、delta clock を読み込む
     * @param stream 読み込み元のストリーム
     * @return delta clock
     */
    static tick_t readDeltaClock( InputStream *stream ){
        tick_t ret = 0;
        const tick_t mask = ~(tick_t)0 - (tick_t)0xFF + (tick_t)0x80;
        while( 1 ){
            int i = stream->read();
            if( i < 0 ){
                break;
            }
            int d = i;
            ret = (mask & (ret << 7)) | (d & 0x7f);
            if( (d & 0x80) == 0x00 ){
                break;
            }
        }
        return ret;
    }

    /**
     * @brief ストリームから MIDI イベントを一つ読み込む
     * @param stream 読み込み元のストリーム
     * @param last_clock [ByRef<Long>]
     * @param last_status_byte [ByRef<Integer>]
     */
    static MidiEvent read( InputStream *stream, tick_t &last_clock, uint8_t &last_status_byte ){
        tick_t delta_clock = readDeltaClock( stream );
        last_clock += delta_clock;
        int first_byte = stream->read();
        if( first_byte < 0x80 ){
            // ランニングステータスが適用される
            int64_t pos = stream->getPointer();
            stream->seek( pos - 1 );
            first_byte = last_status_byte;
        }else{
            last_status_byte = first_byte;
        }
        int ctrl = first_byte & 0xf0;
        if( ctrl == 0x80 || ctrl == 0x90 || ctrl == 0xA0 || ctrl == 0xB0 || ctrl == 0xE0 || first_byte == 0xF2 ){
            // 3byte使用するチャンネルメッセージ：
            //     0x8*: ノートオフ
            //     0x9*: ノートオン
            //     0xA*: ポリフォニック・キープレッシャ
            //     0xB*: コントロールチェンジ
            //     0xE*: ピッチベンドチェンジ
            // 3byte使用するシステムメッセージ
            //     0xF2: ソングポジション・ポインタ
            MidiEvent me;
            me.clock = last_clock;
            me.firstByte = first_byte;
            me.data.clear();
            me.data.push_back( 0xff & stream->read() );
            me.data.push_back( 0xff & stream->read() );
            return me;
        }else if( ctrl == 0xC0 || ctrl == 0xD0 || first_byte == 0xF1 || first_byte == 0xF3 ){
            // 2byte使用するチャンネルメッセージ
            //     0xC*: プログラムチェンジ
            //     0xD*: チャンネルプレッシャ
            // 2byte使用するシステムメッセージ
            //     0xF1: クォータフレーム
            //     0xF3: ソングセレクト
            MidiEvent me;
            me.clock = last_clock;
            me.firstByte = first_byte;
            me.data.clear();
            me.data.push_back( 0xff & stream->read() );
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
            MidiEvent me;
            me.clock = last_clock;
            me.firstByte = first_byte;
            me.data.clear();
            return me;
        }else if( first_byte == 0xff ){
            // メタイベント
            int meta_event_type = stream->read();
            tick_t meta_event_length = readDeltaClock( stream );
            MidiEvent me;
            me.clock = last_clock;
            me.firstByte = first_byte;
            me.data.clear();
            me.data.push_back( meta_event_type );
            for( int i = 0; i < meta_event_length; i++ ){
                me.data.push_back( stream->read() );
            }
            return me;
        }else if( first_byte == 0xf0 ){
            // f0ステータスのSysEx
            MidiEvent me;
            me.clock = last_clock;
            me.firstByte = first_byte;
            int sysex_length = (int)readDeltaClock( stream );
            me.data.clear();
            for( int i = 0; i < sysex_length + 1; i++ ){
                me.data.push_back( stream->read() );
            }
            return me;
        }else if( first_byte == 0xf7 ){
            // f7ステータスのSysEx
            MidiEvent me;
            me.clock = last_clock;
            me.firstByte = first_byte;
            int sysex_length = (int)readDeltaClock( stream );
            me.data.clear();
            for( int i = 0; i < sysex_length; i++ ){
                me.data.push_back( stream->read() );
            }
            return me;
        }else{
            throw ParseException( "don't know how to process first_byte: 0x" + StringUtil::toString( first_byte, 16 ) );
        }
    }

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
