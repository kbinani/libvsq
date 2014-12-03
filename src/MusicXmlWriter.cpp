/**
 * MusicXmlWriter.cpp
 * Copyright © 2014 kbinani
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
#include "../include/libvsq/MusicXmlWriter.hpp"
#include "../include/libvsq/Sequence.hpp"
#include "../include/libvsq/TextOutputStream.hpp"
#include "../include/libvsq/StringUtil.hpp"
#include "../include/libvsq/NoteNumberUtil.hpp"

LIBVSQ_BEGIN_NAMESPACE

void MusicXmlWriter::write(Sequence const& sequence, TextOutputStream& stream, std::string const& software)
{
	int tempo = 120;

	Sequence vsq = sequence;
	vsq.updateTotalTicks();
	int intTempo = tempo;
	intTempo = (int)(60e6 / vsq.tempoList.tempoAt(0));

	// ヘッダ
	stream.writeLine("<?xml version=\"1.0\" encoding=\"UTF-8\"?>");
	stream.writeLine("<!DOCTYPE score-partwise PUBLIC \"-//Recordare//DTD MusicXML 2.0 Partwise//EN\"");
	stream.writeLine("                                \"http://www.musicxml.org/dtds/partwise.dtd\">");
	stream.writeLine("<score-partwise version=\"2.0\">");
	stream.writeLine("  <identification>");
	stream.writeLine("    <encoding>");
	if (software.size() > 0) {
		stream.writeLine("      <software>" + software + "</software>");
	}
	stream.writeLine("      <software>libvsq</software>");
	stream.writeLine("    </encoding>");
	stream.writeLine("  </identification>");
	stream.writeLine("  <part-list>");

	int track_count = vsq.tracks().size();
	Timesig timesigStart = vsq.timesigList.timesigAt(0);
	int tickPerMeasure = timesigStart.numerator * 480 * 4 / timesigStart.denominator;
	for (int i = 0; i < track_count; i++) {
		Track const& vsq_track = vsq.track(i);
		stream.writeLine("    <score-part id=\"P" + StringUtil::toString(i + 1) + "\">");
		stream.writeLine("      <part-name>" + vsq_track.name() + "</part-name>");
		stream.writeLine("    </score-part>");
	}
	stream.writeLine("  </part-list>");
	int measureStart = 0; // 出力開始する小節

	for (int i = 0; i < track_count; i++) {
		Track const& vsq_track = vsq.track(i);
		int numEvents = vsq_track.events().size();
		stream.writeLine("  <part id=\"P" + StringUtil::toString(i + 1) + "\">");

		// 拍子変更毎に出力していく
		int countTimesig = vsq.timesigList.size();
		int totalMeasure = measureStart; // 出力してきた小節の数
		int tickLastBase = measureStart * tickPerMeasure; // 前回の拍子ブロックで出力し終わったクロック
		int startIndex = 0;
		for (int n = 0; n < countTimesig; n++) {
			Timesig timesigEntryThis = vsq.timesigList.get(n);
			tickPerMeasure = timesigEntryThis.numerator * 480 * 4 / timesigEntryThis.denominator;

			// この拍子が曲の終まで続くとしたら, あと何小節出力する必要があるのか？
			int remainingMeasures = 0;
			if (n + 1 < countTimesig) {
				Timesig timesigEntryNext = vsq.timesigList.get(n + 1);
				remainingMeasures = timesigEntryNext.barCount - timesigEntryThis.barCount;
			} else {
				int remainingTicks = vsq.totalTicks() - tickLastBase;
				remainingMeasures = remainingTicks / tickPerMeasure;
				if (remainingTicks % tickPerMeasure != 0) {
					remainingMeasures++;
				}
			}

			// remainingMeasures小節を順次出力
			for (int j = totalMeasure; j < totalMeasure + remainingMeasures; j++) {
				int tickStart = tickLastBase + (j - totalMeasure) * tickPerMeasure;
				int tickEnd = tickStart + tickPerMeasure;

				// 今出力している小節内に, テンポ変更が挿入されているかどうか
				int numTempo = vsq.tempoList.size();
				std::vector<Tempo> tempoInsert; // 挿入するテンポ情報のリスト
				for (int k = 0; k < numTempo; k++) {
					Tempo itemk = vsq.tempoList.get(k);
					if (tickStart <= itemk.tick && itemk.tick < tickEnd) {
						tempoInsert.push_back(itemk);
					}
				}

				stream.writeLine("    <measure number=\"" + StringUtil::toString(j + 1 - measureStart) + "\">");
				if (j == totalMeasure) {
					stream.writeLine("      <attributes>");
					stream.writeLine("        <divisions>480</divisions>");
					stream.writeLine("        <time symbol=\"common\">");
					stream.writeLine("          <beats>" + StringUtil::toString(timesigEntryThis.numerator) + "</beats>");
					stream.writeLine("          <beat-type>" + StringUtil::toString(timesigEntryThis.denominator) + "</beat-type>");
					stream.writeLine("        </time>");
					stream.writeLine("      </attributes>");
					if (j == 0) {
						stream.writeLine("      <direction placement=\"above\">");
						stream.writeLine("        <direction-type>");
						stream.writeLine("          <metronome>");
						stream.writeLine("            <beat-unit>quarter</beat-unit>");
						stream.writeLine("            <per-minute>" + StringUtil::toString(intTempo) + "</per-minute>");
						stream.writeLine("          </metronome>");
						stream.writeLine("          <words>Tempo " + StringUtil::toString(intTempo) + "</words>");
						stream.writeLine("        </direction-type>");
						stream.writeLine("        <sound tempo=\"" + StringUtil::toString(intTempo) + "\"/>");
						stream.writeLine("      </direction>");
					}
				}

				// 臨時記号のON/OFFを制御するために
				std::map<std::string, bool> altered;
				altered.insert(std::make_pair("C", false));
				altered.insert(std::make_pair("D", false));
				altered.insert(std::make_pair("E", false));
				altered.insert(std::make_pair("F", false));
				altered.insert(std::make_pair("G", false));
				altered.insert(std::make_pair("A", false));
				altered.insert(std::make_pair("B", false));

				int tickLast = tickStart; // 出力済みのクロック
				for (int k = startIndex; k < numEvents; k++) {
					const Event* itemk = vsq_track.events().get(k);
					if (itemk->type() != EventType::NOTE) {
						if (tickEnd <= itemk->tick) {
							startIndex = k;
							break;
						}
						continue;
					}

					// 第 k 番目の音符が, 第 j 小節の範囲に入っているかどうか.
					// 入っていれば, 出力する.
					if ((tickStart <= itemk->tick && itemk->tick < tickEnd) ||
						(tickStart <= itemk->tick + itemk->length() && itemk->tick + itemk->length() < tickEnd) ||
						(itemk->tick <= tickStart && tickEnd <= itemk->tick + itemk->length())) {
						if (tickLast < itemk->tick) {
							// 第 j 小節の開始位置から, 第 k 番目の音符の間に, 休符があるので, 出力する
							// [tickLast, itemk.tick) の間でテンポ変更イベントがあれば, これも出力する
							std::vector<Tempo> insert;
							for (int m = 0; m < tempoInsert.size(); m++) {
								Tempo itemm = tempoInsert[m];
								if (tickLast <= itemm.tick && itemm.tick < itemk->tick) {
									insert.push_back(itemm);
								}
							}
							printStyledNote(stream, tickLast, itemk->tick - tickLast, -1, insert, "", altered, false, false);
							tickLast = itemk->tick;
						}

						bool tieStopRequired = false;
						int start = itemk->tick;
						if (start < tickStart) {
							// 前の小節からタイで接続されている場合
							start = tickStart;
							tieStopRequired = true;
						}
						int end = itemk->tick + itemk->length();
						bool tieStartRequired = false;
						if (tickEnd < end) {
							// 次の小節にタイで接続しなければならない場合
							end = tickEnd;
							tieStartRequired = true;
						}
						int actualLength = end - start;
						std::vector<Tempo> insert2;
						for (int m = 0; m < tempoInsert.size(); m++) {
							Tempo itemm = tempoInsert[m];
							if (start <= itemm.tick && itemm.tick < end) {
								insert2.push_back(itemm);
							}
						}
						printStyledNote(stream, start, actualLength, itemk->note, insert2, itemk->lyricHandle.get(0).phrase, altered, tieStartRequired, tieStopRequired);
						tickLast = end;
						if (tieStartRequired) {
							startIndex = k;
						} else {
							startIndex = k + 1;
						}
					}
				}
				if (tickLast < tickEnd) {
					// 小節の最後に休符を入れる必要がある
					std::vector<Tempo> insert3;
					for (int m = 0; m < tempoInsert.size(); m++) {
						Tempo itemm = tempoInsert[m];
						if (tickEnd <= itemm.tick && itemm.tick < tickLast) {
							insert3.push_back(itemm);
						}
					}
					printStyledNote(stream, tickLast, (tickEnd - tickLast), -1, insert3, "", altered, false, false);
					tickLast = tickEnd;
				}
				stream.writeLine("    </measure>");
			}
			tickLastBase += remainingMeasures * tickPerMeasure;
			totalMeasure += remainingMeasures;
		}
		stream.writeLine("  </part>");
	}
	stream.writeLine("</score-partwise>");
}

void MusicXmlWriter::printStyledNote(
	TextOutputStream& stream,
	int tickStart,
	int tickLength,
	int note,
	std::vector<Tempo>& tempoInsert,
	std::string lyric,
	std::map<std::string, bool>& altered_context,
	bool tie_start_required,
	bool tie_stop_required)
{
	int numInsert = tempoInsert.size();
	for (int i = 0; i < numInsert; i++) {
		Tempo itemi = tempoInsert[i];
		int tempo = (int)(60e6 / (double)itemi.tempo);
		stream.writeLine("      <direction placement=\"above\">");
		stream.writeLine("        <direction-type>");
		stream.writeLine("          <metronome>");
		stream.writeLine("            <beat-unit>quarter</beat-unit>");
		stream.writeLine("            <per-minute>" + StringUtil::toString(tempo) + "</per-minute>");
		stream.writeLine("          </metronome>");
		stream.writeLine("          <words>Tempo " + StringUtil::toString(tempo) + "</words>");
		stream.writeLine("        </direction-type>");
		stream.writeLine("        <sound tempo=\"" + StringUtil::toString(tempo) + "\"/>");
		stream.writeLine("        <offset>" + StringUtil::toString(itemi.tick - tickStart) + "</offset>");
		stream.writeLine("      </direction>");
	}
	int ret[9];
	int len[8] = { 1920, 960, 480, 240, 120, 60, 30, 15 };
	std::vector<std::string> name;
	name.push_back("whole");
	name.push_back("half");
	name.push_back("quarter");
	name.push_back("eighth");
	name.push_back("16th");
	name.push_back("32nd");
	name.push_back("64th");
	name.push_back("128th");
	name.push_back("");
	int remain = tickLength;
	for (int i = 0; i < 8; i++) {
		ret[i] = remain / len[i];
		if (ret[i] > 0) {
			remain -= len[i] * ret[i];
		}
	}
	ret[8] = remain;

	int firstContainedIndex = -1;
	int lastContainedIndex = -1;
	int numSeparated = 0;
	for (int i = 0; i < 8; i++) {
		if (ret[i] > 0) {
			if (firstContainedIndex < 0) {
				firstContainedIndex = i;
			}
			lastContainedIndex = i;
			numSeparated += ret[i];
		}
	}
	for (int i = 0; i < 8; i++) {
		int count = ret[i];
		if (count <= 0) {
			continue;
		}
		for (int j = 0; j < count; j++) {
			bool local_tie_start_required = numSeparated > 0 ? true : false;
			bool local_tie_stop_required = numSeparated > 0 ? true : false;
			if (i == firstContainedIndex && j == 0) {
				local_tie_stop_required = tie_stop_required;
			}
			if (i == lastContainedIndex && j == count - 1) {
				local_tie_start_required = tie_start_required;
			}
			printStyledNoteCor(stream, len[i], note, lyric, altered_context, local_tie_start_required, local_tie_stop_required, name[i]);
		}
	}
}

void MusicXmlWriter::printStyledNoteCor(
	TextOutputStream& stream,
	int tickLength,
	int note,
	std::string lyric,
	std::map<std::string, bool>& alteredContext,
	bool tieStartRequired,
	bool tieStopRequired,
	std::string type)
{
	stream.writeLine("      <note>");
	if (note < 0) {
		stream.writeLine("        <rest/>");
	} else {
		std::string noteStringBase = NoteNumberUtil::getNoteStringBase(note);   // "C"など
		int octave = NoteNumberUtil::getNoteOctave(note);
		stream.writeLine("        <pitch>");
		stream.writeLine("          <step>" + noteStringBase + "</step>");
		int alter = NoteNumberUtil::getNoteAlter(note);
		if (alter != 0) {
			stream.writeLine("          <alter>" + StringUtil::toString(alter) + "</alter>");
		}
		stream.writeLine("          <octave>" + StringUtil::toString(octave + 1) + "</octave>");
		stream.writeLine("        </pitch>");
		std::string stem = note >= 70 ? "down" : "up";
		stream.writeLine("        <stem>" + stem + "</stem>");
		std::string accidental = "";
		std::string checkAltered = noteStringBase;
		if (!tieStopRequired && alteredContext.find(checkAltered) != alteredContext.end()) {
			if (alter == 0) {
				if (alteredContext[checkAltered]) {
					accidental = "natural";
					alteredContext[checkAltered] = false;
				}
			} else {
				if (!alteredContext[checkAltered]) {
					accidental = alter == 1 ? "sharp" : "flat";
					alteredContext[checkAltered] = true;
				}
			}
		}

		if (accidental.size() > 0) {
			stream.writeLine("        <accidental>" + accidental + "</accidental>");
		}
		if (tieStartRequired) {
			stream.writeLine("        <tie type=\"start\"/>");
			stream.writeLine("        <notations>");
			stream.writeLine("          <tied type=\"start\"/>");
			stream.writeLine("        </notations>");
		}
		if (tieStopRequired) {
			stream.writeLine("        <tie type=\"stop\"/>");
			stream.writeLine("        <notations>");
			stream.writeLine("          <tied type=\"stop\"/>");
			stream.writeLine("        </notations>");
		}
		if (!tieStopRequired) {
			stream.writeLine("        <lyric>");
			stream.writeLine("          <text>" + lyric + "</text>");
			stream.writeLine("        </lyric>");
		}
	}

	stream.writeLine("        <voice>1</voice>");
	stream.writeLine("        <duration>" + StringUtil::toString(tickLength) + "</duration>");
	if (type.size() > 0) {
		stream.writeLine("        <type>" + type + "</type>");
	}
	stream.writeLine("      </note>");
}

LIBVSQ_END_NAMESPACE
