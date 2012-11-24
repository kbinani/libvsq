/**
 * MusicXmlWriter.hpp
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
#ifndef __MusicXmlWriter_hpp__
#define __MusicXmlWriter_hpp__

#include "vsqglobal.hpp"
#include "Sequence.hpp"
#include "OutputStream.hpp"
#include "NoteNumberUtil.hpp"

VSQ_BEGIN_NAMESPACE

class MusicXmlWriter{
public:
    /**
     * @brief シーケンスを MusicXML として、指定されたストリームに出力する
     * @param sequence 出力するシーケンス
     * @param stream 出力先のストリーム
     * @todo change_tempo を changeTempo に名前を変える
     * @todo sw を stream に名前変える
     * @todo vsq を sequence に名前変える
     * @todo tempo と change_tempo を渡せるようにする
     */
//    private void printAsMusicXmlCore( String file, String encoding, String software, int tempo, boolean change_tempo )
    void write( const Sequence *sequence, TextOutputStream *sw, const std::string &software ){
        bool change_tempo = false;
        int tempo = 120;

        Sequence vsq = *sequence;
        vsq.updateTotalClocks();
        int intTempo = tempo;
        if( !change_tempo ){
            intTempo = (int)(60e6 / vsq.tempoList.getTempoAt( 0 ));
        }

        if( change_tempo ){
//TODO:            vsq.adjustClockToMatchWith( tempo );
        }

        // ヘッダ
        sw->writeLine( "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" );
        sw->writeLine( "<!DOCTYPE score-partwise PUBLIC \"-//Recordare//DTD MusicXML 2.0 Partwise//EN\"" );
        sw->writeLine( "                                \"http://www.musicxml.org/dtds/partwise.dtd\">" );
        sw->writeLine( "<score-partwise version=\"2.0\">" );
        sw->writeLine( "  <identification>" );
        sw->writeLine( "    <encoding>" );
        if( software.size() > 0 ){
            sw->writeLine( "      <software>" + software + "</software>" );
        }
        sw->writeLine( "      <software>libvsq</software>" );
        sw->writeLine( "    </encoding>" );
        sw->writeLine( "  </identification>" );
        sw->writeLine( "  <part-list>" );

        int track_count = vsq.tracks()->size();
        Timesig timesigStart = vsq.timesigList.getTimesigAt( 0 );
        int clockPerMeasure = timesigStart.numerator * 480 * 4 / timesigStart.denominator;
        for( int i = 0; i < track_count; i++ ){
            const Track *vsq_track = vsq.track(i);
            sw->writeLine( "    <score-part id=\"P" + StringUtil::toString( i + 1 ) + "\">" );
            sw->writeLine( "      <part-name>" + vsq_track->getName() + "</part-name>" );
            sw->writeLine( "    </score-part>" );
        }
        sw->writeLine( "  </part-list>" );
        int measureStart = 0; // 出力開始する小節

        for( int i = 0; i < track_count; i++ ){
            const Track *vsq_track = vsq.track(i);
            int numEvents = vsq_track->events()->size();
            sw->writeLine( "  <part id=\"P" + StringUtil::toString( i + 1 ) + "\">" );

            // 拍子変更毎に出力していく
            int countTimesig = vsq.timesigList.size();
            int totalMeasure = measureStart; // 出力してきた小節の数
            int clockLastBase = measureStart * clockPerMeasure; // 前回の拍子ブロックで出力し終わったクロック
            int startIndex = 0;
            for( int n = 0; n < countTimesig; n++ ){
                Timesig timesigEntryThis = vsq.timesigList.get( n );
                clockPerMeasure = timesigEntryThis.numerator * 480 * 4 / timesigEntryThis.denominator;

                // この拍子が曲の終まで続くとしたら，あと何小節出力する必要があるのか？
                int remainingMeasures = 0;
                if( n + 1 < countTimesig ){
                    Timesig timesigEntryNext = vsq.timesigList.get( n + 1 );
                    remainingMeasures = timesigEntryNext.barCount - timesigEntryThis.barCount;
                }else{
                    int remainingClocks = vsq.getTotalClocks() - clockLastBase;
                    remainingMeasures = remainingClocks / clockPerMeasure;
                    if( remainingClocks % clockPerMeasure != 0 ){
                        remainingMeasures++;
                    }
                }

                // remainingMeasures小節を順次出力
                for( int j = totalMeasure; j < totalMeasure + remainingMeasures; j++ ){
                    int clockStart = clockLastBase + (j - totalMeasure) * clockPerMeasure;
                    int clockEnd = clockStart + clockPerMeasure;

                    // 今出力している小節内に、テンポ変更が挿入されているかどうか
                    int numTempo = vsq.tempoList.size();
                    std::vector<Tempo> tempoInsert; // 挿入するテンポ情報のリスト
                    for( int k = 0; k < numTempo; k++ ){
                        Tempo itemk = vsq.tempoList.get( k );
                        if( clockStart <= itemk.clock && itemk.clock < clockEnd ){
                            tempoInsert.push_back( itemk );
                        }
                    }

                    sw->writeLine( "    <measure number=\"" + StringUtil::toString( j + 1 - measureStart ) + "\">" );
                    if( j == totalMeasure ){
                        sw->writeLine( "      <attributes>" );
                        sw->writeLine( "        <divisions>480</divisions>" );
                        sw->writeLine( "        <time symbol=\"common\">" );
                        sw->writeLine( "          <beats>" + StringUtil::toString( timesigEntryThis.numerator ) + "</beats>" );
                        sw->writeLine( "          <beat-type>" + StringUtil::toString( timesigEntryThis.denominator ) + "</beat-type>" );
                        sw->writeLine( "        </time>" );
                        sw->writeLine( "      </attributes>" );
                        if( j == 0 ){
                            sw->writeLine( "      <direction placement=\"above\">" );
                            sw->writeLine( "        <direction-type>" );
                            sw->writeLine( "          <metronome>" );
                            sw->writeLine( "            <beat-unit>quarter</beat-unit>" );
                            sw->writeLine( "            <per-minute>" + StringUtil::toString( intTempo ) + "</per-minute>" );
                            sw->writeLine( "          </metronome>" );
                            sw->writeLine( "          <words>Tempo " + StringUtil::toString( intTempo ) + "</words>" );
                            sw->writeLine( "        </direction-type>" );
                            sw->writeLine( "        <sound tempo=\"" + StringUtil::toString( intTempo ) + "\"/>" );
                            sw->writeLine( "      </direction>" );
                        }
                    }

                    // 臨時記号のON/OFFを制御するために
                    std::map<std::string, bool> altered;
                    altered.insert( make_pair( "C", false ) );
                    altered.insert( make_pair( "D", false ) );
                    altered.insert( make_pair( "E", false ) );
                    altered.insert( make_pair( "F", false ) );
                    altered.insert( make_pair( "G", false ) );
                    altered.insert( make_pair( "A", false ) );
                    altered.insert( make_pair( "B", false ) );

                    int clockLast = clockStart; // 出力済みのクロック
                    for( int k = startIndex; k < numEvents; k++ ){
                        const Event *itemk = vsq_track->events()->get( k );
                        if( itemk->type != EventType::NOTE ){
                            if( clockEnd <= itemk->clock ){
                                startIndex = k;
                                break;
                            }
                            continue;
                        }

                        // 第 k 番目の音符が、第 j 小節の範囲に入っているかどうか。
                        // 入っていれば、出力する。
                        if( (clockStart <= itemk->clock && itemk->clock < clockEnd) ||
                            (clockStart <= itemk->clock + itemk->getLength() && itemk->clock + itemk->getLength() < clockEnd) ||
                            (itemk->clock <= clockStart && clockEnd <= itemk->clock + itemk->getLength()) )
                        {
                            if( clockLast < itemk->clock ){
                                // 第 j 小節の開始位置から、第 k 番目の音符の間に、休符があるので、出力する
                                // [clockLast, itemk.Clock) の間でテンポ変更イベントがあれば、これも出力する
                                std::vector<Tempo> insert;
                                if( !change_tempo ){
                                    for( int m = 0; m < tempoInsert.size(); m++ ){
                                        Tempo itemm = tempoInsert[m];
                                        if( clockLast <= itemm.clock && itemm.clock < itemk->clock ){
                                            insert.push_back( itemm );
                                        }
                                    }
                                }
                                printStyledNote( sw, clockLast, itemk->clock - clockLast, -1, insert, "", altered, false, false );
                                clockLast = itemk->clock;
                            }

                            bool tieStopRequired = false;
                            int start = itemk->clock;
                            if( start < clockStart ){
                                // 前の小節からタイで接続されている場合
                                start = clockStart;
                                tieStopRequired = true;
                            }
                            int end = itemk->clock + itemk->getLength();
                            bool tieStartRequired = false;
                            if( clockEnd < end ){
                                // 次の小節にタイで接続しなければならない場合
                                end = clockEnd;
                                tieStartRequired = true;
                            }
                            int actualLength = end - start;
                            std::vector<Tempo> insert2;
                            if( !change_tempo ){
                                for( int m = 0; m < tempoInsert.size(); m++ ){
                                    Tempo itemm = tempoInsert[m];
                                    if( start <= itemm.clock && itemm.clock < end ){
                                        insert2.push_back( itemm );
                                    }
                                }
                            }
                            printStyledNote( sw, start, actualLength, itemk->note, insert2, itemk->lyricHandle.getLyricAt( 0 ).phrase, altered, tieStartRequired, tieStopRequired );
                            clockLast = end;
                            if( tieStartRequired ){
                                startIndex = k;
                            }else{
                                startIndex = k + 1;
                            }
                        }
                    }
                    if( clockLast < clockEnd ){
                        // 小節の最後に休符を入れる必要がある
                        std::vector<Tempo> insert3;
                        if( !change_tempo ){
                            for( int m = 0; m < tempoInsert.size(); m++ ){
                                Tempo itemm = tempoInsert[m];
                                if( clockEnd <= itemm.clock && itemm.clock < clockLast ){
                                    insert3.push_back( itemm );
                                }
                            }
                        }
                        printStyledNote( sw, clockLast, (clockEnd - clockLast), -1, insert3, "", altered, false, false );
                        clockLast = clockEnd;
                    }
                    sw->writeLine( "    </measure>" );
                }
                clockLastBase += remainingMeasures * clockPerMeasure;
                totalMeasure += remainingMeasures;
            }
            sw->writeLine( "  </part>" );
        }
        sw->writeLine( "</score-partwise>" );
    }

protected:
    void printStyledNote(
        TextOutputStream *writer,
        int clock_start,
        int clock_length,
        int note,
        std::vector<Tempo> &tempoInsert,
        std::string lyric,
        std::map<std::string, bool> &altered_context,
        bool tie_start_required,
        bool tie_stop_required )
    {
        int numInsert = tempoInsert.size();
        for( int i = 0; i < numInsert; i++ ){
            Tempo itemi = tempoInsert[i];
            int tempo = (int)(60e6 / (double)itemi.tempo);
            writer->writeLine( "      <direction placement=\"above\">" );
            writer->writeLine( "        <direction-type>" );
            writer->writeLine( "          <metronome>" );
            writer->writeLine( "            <beat-unit>quarter</beat-unit>" );
            writer->writeLine( "            <per-minute>" + StringUtil::toString( tempo ) + "</per-minute>" );
            writer->writeLine( "          </metronome>" );
            writer->writeLine( "          <words>Tempo " + StringUtil::toString( tempo ) + "</words>" );
            writer->writeLine( "        </direction-type>" );
            writer->writeLine( "        <sound tempo=\"" + StringUtil::toString( tempo ) + "\"/>" );
            writer->writeLine( "        <offset>" + StringUtil::toString( itemi.clock - clock_start ) + "</offset>" );
            writer->writeLine( "      </direction>" );
        }
        int ret[9];
        int len[8] = { 1920, 960, 480, 240, 120, 60, 30, 15 };
        std::vector<std::string> name;
        name.push_back( "whole" );
        name.push_back( "half" );
        name.push_back( "quarter" );
        name.push_back( "eighth" );
        name.push_back( "16th" );
        name.push_back( "32nd" );
        name.push_back( "64th" );
        name.push_back( "128th" );
        name.push_back( "" );
        int remain = clock_length;
        for( int i = 0; i < 8; i++ ){
            ret[i] = remain / len[i];
            if ( ret[i] > 0 ) {
                remain -= len[i] * ret[i];
            }
        }
        ret[8] = remain;

        int firstContainedIndex = -1;
        int lastContainedIndex = -1;
        int numSeparated = 0;
        for( int i = 0; i < 8; i++ ){
            if( ret[i] > 0 ){
                if( firstContainedIndex < 0 ){
                    firstContainedIndex = i;
                }
                lastContainedIndex = i;
                numSeparated += ret[i];
            }
        }
        for( int i = 0; i < 8; i++ ){
            int count = ret[i];
            if( count <= 0 ){
                continue;
            }
            for( int j = 0; j < count; j++ ){
                bool local_tie_start_required = numSeparated > 0 ? true : false;
                bool local_tie_stop_required = numSeparated > 0 ? true : false;
                if( i == firstContainedIndex && j == 0 ){
                    local_tie_stop_required = tie_stop_required;
                }
                if( i == lastContainedIndex && j == count - 1 ){
                    local_tie_start_required = tie_start_required;
                }
                printStyledNoteCor( writer, len[i], note, lyric, altered_context, local_tie_start_required, local_tie_stop_required, name[i] );
            }
        }
    }

    void printStyledNoteCor(
        TextOutputStream *writer,
        int clock_length,
        int note,
        std::string lyric,
        std::map<std::string, bool> &altered_context,
        bool tie_start_required,
        bool tie_stop_required,
        std::string type )
    {
        writer->writeLine( "      <note>" );
        if( note < 0 ){
            writer->writeLine( "        <rest/>" );
        }else{
            std::string noteStringBase = NoteNumberUtil::getNoteStringBase( note ); // "C"など
            int octave = NoteNumberUtil::getNoteOctave( note );
            writer->writeLine( "        <pitch>" );
            writer->writeLine( "          <step>" + noteStringBase + "</step>" );
            int alter = NoteNumberUtil::getNoteAlter( note );
            if( alter != 0 ){
                writer->writeLine( "          <alter>" + StringUtil::toString( alter ) + "</alter>" );
            }
            writer->writeLine( "          <octave>" + StringUtil::toString( octave + 1 ) + "</octave>" );
            writer->writeLine( "        </pitch>" );
            std::string stem = note >= 70 ? "down" : "up";
            writer->writeLine( "        <stem>" + stem + "</stem>" );
            std::string accidental = "";
            std::string checkAltered = noteStringBase;
            if( !tie_stop_required && altered_context.find( checkAltered ) != altered_context.end() ){
                if( alter == 0 ){
                    if( altered_context[checkAltered] ){
                        accidental = "natural";
                        altered_context[checkAltered] = false;
                    }
                }else{
                    if( !altered_context[checkAltered] ){
                        accidental = alter == 1 ? "sharp" : "flat";
                        altered_context[checkAltered] = true;
                    }
                }
            }

            if( accidental.size() > 0 ) {
                writer->writeLine( "        <accidental>" + accidental + "</accidental>" );
            }
            if( tie_start_required ){
                writer->writeLine( "        <tie type=\"start\"/>" );
                writer->writeLine( "        <notations>" );
                writer->writeLine( "          <tied type=\"start\"/>" );
                writer->writeLine( "        </notations>" );
            }
            if( tie_stop_required ){
                writer->writeLine( "        <tie type=\"stop\"/>" );
                writer->writeLine( "        <notations>" );
                writer->writeLine( "          <tied type=\"stop\"/>" );
                writer->writeLine( "        </notations>" );
            }
            if( !tie_stop_required ){
                writer->writeLine( "        <lyric>" );
                writer->writeLine( "          <text>" + lyric + "</text>" );
                writer->writeLine( "        </lyric>" );
            }
        }

        writer->writeLine( "        <voice>1</voice>" );
        writer->writeLine( "        <duration>" + StringUtil::toString( clock_length ) + "</duration>" );
        if( type.size() > 0 ){
            writer->writeLine( "        <type>" + type + "</type>" );
        }
        writer->writeLine( "      </note>" );
    }
};

VSQ_END_NAMESPACE

#endif
