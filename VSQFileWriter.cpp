/**
 * VSQFileWriter.cpp
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
#include "./VSQFileWriter.hpp"
#include "./Sequence.hpp"
#include "./OutputStream.hpp"
#include "./TextStream.hpp"
#include "./StringUtil.hpp"
#include "./CP932Converter.hpp"
#include "./VocaloidMidiEventListFactory.hpp"
#include "./BitConverter.hpp"
#include "./VoiceLanguage.hpp"
#include <sstream>

VSQ_BEGIN_NAMESPACE

void VSQFileWriter::write(const Sequence* sequence, OutputStream* stream, int msPreSend, const std::string& encoding, bool printPitch)
{
	Sequence copyOfSequence = *sequence;
	Sequence* targetSequence = &copyOfSequence;
	targetSequence->updateTotalClocks();
	int64_t first_position; //チャンクの先頭のファイル位置

	// ヘッダ
	// チャンクタイプ
	char mthd[] = { 0x4d, 0x54, 0x68, 0x64 };
	stream->write(mthd, 0, 4);
	// データ長
	stream->write(0x00);
	stream->write(0x00);
	stream->write(0x00);
	stream->write(0x06);
	// フォーマット
	stream->write(0x00);
	stream->write(0x01);
	// トラック数
	writeUnsignedShort(stream, targetSequence->tracks()->size() + 1);
	// 時間単位
	stream->write(0x01);
	stream->write(0xe0);

	// Master Track
	// チャンクタイプ
	std::string mtrk = getTrackHeader();
	stream->write(mtrk.c_str(), 0, 4);
	// データ長. とりあえず0を入れておく
	char empty[] = { 0x00, 0x00, 0x00, 0x00 };
	stream->write(empty, 0, 4);
	first_position = stream->getPointer();
	// トラック名
	const int masterTrackNameLength = 12;
	char masterTrackName[masterTrackNameLength] = { 0x4D, 0x61, 0x73, 0x74, 0x65, 0x72, 0x20, 0x54, 0x72, 0x61, 0x63, 0x6B };
	MidiEvent::writeDeltaClock(stream, 0);  // デルタタイム
	stream->write(0xff);  // ステータスタイプ
	stream->write(0x03);  // イベントタイプSequence/Track Name
	stream->write(masterTrackNameLength);  // トラック名の文字数. これは固定
	stream->write(masterTrackName, 0, masterTrackNameLength);

	std::vector<MidiEvent> events;
	for (int i = 0; i < targetSequence->timesigList.size(); i++) {
		Timesig entry = targetSequence->timesigList.get(i);
		events.push_back(MidiEvent::generateTimeSigEvent(entry.getClock(), entry.numerator, entry.denominator));
	}
	TempoList::Iterator itr = targetSequence->tempoList.iterator();
	while (itr.hasNext()) {
		Tempo entry = itr.next();
		events.push_back(MidiEvent::generateTempoChangeEvent(entry.clock, entry.tempo));
	}
	std::stable_sort(events.begin(), events.end(), MidiEvent::compare);
	tick_t last = 0;
	for (int i = 0; i < events.size(); i++) {
		MidiEvent midiEvent = events[i];
		MidiEvent::writeDeltaClock(stream, midiEvent.clock - last);
		midiEvent.writeData(stream);
		last = midiEvent.clock;
	}

	MidiEvent::writeDeltaClock(stream, 0);
	stream->write(0xff);
	stream->write(0x2f);  // イベントタイプEnd of Track
	stream->write(0x00);
	int64_t pos = stream->getPointer();
	stream->seek(first_position - 4);
	writeUnsignedInt(stream, pos - first_position);
	stream->seek(pos);

	// トラック
	Sequence temp = targetSequence->clone();
	_printTrack(&temp, 0, stream, msPreSend, encoding, printPitch, &targetSequence->master, &targetSequence->mixer);
	int count = targetSequence->tracks()->size();
	for (int track = 1; track < count; track++) {
		_printTrack(targetSequence, track, stream, msPreSend, encoding, printPitch, 0, 0);
	}
}

void VSQFileWriter::writeHandle(const Handle& item, TextStream& stream)
{
	stream.writeLine(std::string("[h#") + StringUtil::toString(item.index, "%04d") + std::string("]"));
	if (item.getHandleType() == HandleType::LYRIC) {
		for (int i = 0; i < item.getLyricCount(); i++) {
			stream.writeLine(std::string("L") + StringUtil::toString(i) + "=" + item.getLyricAt(i).toString(item.addQuotationMark));
		}
	} else if (item.getHandleType() == HandleType::VIBRATO) {
		stream.writeLine(std::string("IconID=") + item.iconId);
		stream.writeLine(std::string("IDS=") + item.ids);
		stream.writeLine(std::string("Original=") + StringUtil::toString(item.original));
		stream.writeLine(std::string("Caption=") + item.caption);
		stream.writeLine(std::string("Length=") + StringUtil::toString(item.getLength()));
		stream.writeLine(std::string("StartDepth=") + StringUtil::toString(item.startDepth));
		stream.writeLine(std::string("DepthBPNum=") + StringUtil::toString(item.depthBP.size()));
		if (item.depthBP.size() > 0) {
			stream.write(std::string("DepthBPX=") + StringUtil::toString(item.depthBP.get(0).x, "%.6f"));
			for (int i = 1; i < item.depthBP.size(); i++) {
				stream.write(std::string(",") + StringUtil::toString(item.depthBP.get(i).x, "%.6f"));
			}
			stream.writeLine("");
			stream.write(std::string("DepthBPY=") + StringUtil::toString(item.depthBP.get(0).y));
			for (int i = 1; i < item.depthBP.size(); i++) {
				stream.write(std::string(",") + StringUtil::toString(item.depthBP.get(i).y));
			}
			stream.writeLine("");
		}
		stream.writeLine(std::string("StartRate=") + StringUtil::toString(item.startRate));
		stream.writeLine(std::string("RateBPNum=") + StringUtil::toString(item.rateBP.size()));
		if (item.rateBP.size() > 0) {
			stream.write(std::string("RateBPX=") + StringUtil::toString(item.rateBP.get(0).x, "%.6f"));
			for (int i = 1; i < item.rateBP.size(); i++) {
				stream.write(std::string(",") + StringUtil::toString(item.rateBP.get(i).x, "%.6f"));
			}
			stream.writeLine("");
			stream.write(std::string("RateBPY=") + StringUtil::toString(item.rateBP.get(0).y));
			for (int i = 1; i < item.rateBP.size(); i++) {
				stream.write(std::string(",") + StringUtil::toString(item.rateBP.get(i).y));
			}
			stream.writeLine("");
		}
	} else if (item.getHandleType() == HandleType::SINGER) {
		stream.writeLine(std::string("IconID=") + item.iconId);
		stream.writeLine(std::string("IDS=") + item.ids);
		stream.writeLine(std::string("Original=") + StringUtil::toString(item.original));
		stream.writeLine(std::string("Caption=") + item.caption);
		stream.writeLine(std::string("Length=") + StringUtil::toString(item.getLength()));
		stream.writeLine(std::string("Language=") + StringUtil::toString(item.language));
		stream.writeLine(std::string("Program=") + StringUtil::toString(item.program));
	} else if (item.getHandleType() == HandleType::NOTE_HEAD) {
		stream.writeLine(std::string("IconID=") + item.iconId);
		stream.writeLine(std::string("IDS=") + item.ids);
		stream.writeLine(std::string("Original=") + StringUtil::toString(item.original));
		stream.writeLine(std::string("Caption=") + item.caption);
		stream.writeLine(std::string("Length=") + StringUtil::toString(item.getLength()));
		stream.writeLine(std::string("Duration=") + StringUtil::toString(item.duration));
		stream.writeLine(std::string("Depth=") + StringUtil::toString(item.depth));
	} else if (item.getHandleType() == HandleType::DYNAMICS) {
		stream.writeLine(std::string("IconID=") + item.iconId);
		stream.writeLine(std::string("IDS=") + item.ids);
		stream.writeLine(std::string("Original=") + StringUtil::toString(item.original));
		stream.writeLine(std::string("Caption=") + item.caption);
		stream.writeLine(std::string("StartDyn=") + StringUtil::toString(item.startDyn));
		stream.writeLine(std::string("EndDyn=") + StringUtil::toString(item.endDyn));
		stream.writeLine(std::string("Length=") + StringUtil::toString(item.getLength()));
		if (item.dynBP.size() <= 0) {
			stream.writeLine("DynBPNum=0");
		} else {
			int c = item.dynBP.size();
			stream.writeLine(std::string("DynBPNum=") + StringUtil::toString(c));
			stream.write(std::string("DynBPX=") + StringUtil::toString(item.dynBP.get(0).x, "%.6f"));
			for (int i = 1; i < c; i++) {
				stream.write(std::string(",") + StringUtil::toString(item.dynBP.get(i).x, "%.6f"));
			}
			stream.writeLine("");
			stream.write(std::string("DynBPY=") + StringUtil::toString(item.dynBP.get(0).y));
			for (int i = 1; i < c; i++) {
				stream.write(std::string(",") + StringUtil::toString(item.dynBP.get(i).y));
			}
			stream.writeLine("");
		}
	}
}

void VSQFileWriter::writeEvent(const TempEvent& item, TextStream& stream, EventWriteOption printTargets) const
{
	stream.write("[ID#");
	stream.write(StringUtil::toString(item.index, "%04d"));
	stream.writeLine("]");
	stream.write("Type=");
	stream.writeLine(EventType::toString(item.type));
	if (item.type == EventType::NOTE) {
		if (printTargets.length) {
			stream.write("Length=");
			stream.writeLine(StringUtil::toString(item.getLength(), "%ld"));
		}
		if (printTargets.note) {
			stream.write("Note#=");
			stream.writeLine(StringUtil::toString(item.note, "%d"));
		}
		if (printTargets.dynamics) {
			stream.write("Dynamics=");
			stream.writeLine(StringUtil::toString(item.dynamics, "%d"));
		}
		if (printTargets.pmBendDepth) {
			stream.write("PMBendDepth=");
			stream.writeLine(StringUtil::toString(item.pmBendDepth, "%d"));
		}
		if (printTargets.pmBendLength) {
			stream.write("PMBendLength=");
			stream.writeLine(StringUtil::toString(item.pmBendLength, "%d"));
		}
		if (printTargets.pmbPortamentoUse) {
			stream.write("PMbPortamentoUse=");
			stream.writeLine(StringUtil::toString(item.pmbPortamentoUse, "%d"));
		}
		if (printTargets.demDecGainRate) {
			stream.write("DEMdecGainRate=");
			stream.writeLine(StringUtil::toString(item.demDecGainRate, "%d"));
		}
		if (printTargets.demAccent) {
			stream.write("DEMaccent=");
			stream.writeLine(StringUtil::toString(item.demAccent, "%d"));
		}
		if (printTargets.preUtterance) {
			//TODO:
			//            stream.writeLine( "PreUtterance=" + ustEvent.preUtterance );
		}
		if (printTargets.voiceOverlap) {
			//TODO:
			//            stream.writeLine( "VoiceOverlap=" + ustEvent.voiceOverlap );
		}
		if (item.lyricHandle.getHandleType() == HandleType::LYRIC) {
			stream.write("LyricHandle=h#");
			stream.writeLine(StringUtil::toString(item.lyricHandle.index, "%04d"));
		}
		if (item.vibratoHandle.getHandleType() == HandleType::VIBRATO) {
			stream.write("VibratoHandle=h#");
			stream.writeLine(StringUtil::toString(item.vibratoHandle.index, "%04d"));
			stream.write("VibratoDelay=");
			stream.writeLine(StringUtil::toString(item.vibratoDelay, "%d"));
		}
		if (item.noteHeadHandle.getHandleType() == HandleType::NOTE_HEAD) {
			stream.write("NoteHeadHandle=h#");
			stream.writeLine(StringUtil::toString(item.noteHeadHandle.index, "%04d"));
		}
	} else if (item.type == EventType::SINGER) {
		stream.write("IconHandle=h#");
		stream.writeLine(StringUtil::toString(item.singerHandle.index, "%04d"));
	} else if (item.type == EventType::ICON) {
		stream.write("IconHandle=h#");
		stream.writeLine(StringUtil::toString(item.iconDynamicsHandle.index, "%04d"));
		stream.write("Note#=");
		stream.writeLine(StringUtil::toString(item.note, "%d"));
	}
}

void VSQFileWriter::printMetaText(const Track* track, TextStream& stream, int eos, tick_t start, bool printPitch, Master* master, Mixer* mixer)
{
	//if( common ~= nil ){
	track->common()->write(stream);
	//}
	if (master) {
		master->write(stream);
	}
	if (mixer) {
		mixer->write(stream);
	}

	std::vector<Handle> handle;
	{
		std::vector<TempEvent*> eventList;
		Event::ListConstIterator itr = track->events()->iterator();
		while (itr.hasNext()) {
			Event* item = itr.next();
			eventList.push_back(new TempEvent(*item));
		}

		handle = writeEventList(eventList, stream, eos);
		for (std::vector<TempEvent*>::iterator itr = eventList.begin(); itr != eventList.end(); ++itr) {
			TempEvent* item = *itr;
			writeEvent(*item, stream);
			delete item;
		}
	}
	for (int i = 0; i < handle.size(); ++i) {
		writeHandle(handle[i], stream);
	}

	TempTrack tempTrack;
	std::map<std::string, std::string> sectionNameMap
		= tempTrack.getSectionNameMap();

	// prepare list of curve name to be printed
	const std::vector<std::string>* curveNameList = track->curveNameList();

	std::vector<std::string>::const_iterator i = curveNameList->begin();
	for (; i != curveNameList->end(); ++i) {
		std::string curveName = StringUtil::toLower(*i);
		std::string sectionName;
		std::map<std::string, std::string>::iterator index = sectionNameMap.begin();
		for (; index != sectionNameMap.end(); ++index) {
			if (StringUtil::toLower(index->second) == curveName) {
				sectionName = index->first;
				break;
			}
		}
		if (track->curve(curveName)->size() > 0) {
			track->curve(curveName)->print(stream, start, sectionName);
		}
	}
}

void VSQFileWriter::_printTrack(Sequence* sequence, int track, OutputStream* stream, int msPreSend, const std::string& encoding, bool printPitch, Master* master, Mixer* mixer)
{
	// ヘッダ
	std::string mtrk = getTrackHeader();
	stream->write(mtrk.c_str(), 0, 4);
	// データ長. とりあえず0
	char empty[] = { 0x00, 0x00, 0x00, 0x00 };
	stream->write(empty, 0, 4);
	int64_t first_position = stream->getPointer();
	// トラック名
	MidiEvent::writeDeltaClock(stream, 0x00);  // デルタタイム
	stream->write(0xff);  // ステータスタイプ
	stream->write(0x03);  // イベントタイプSequence/Track Name
	std::string seq_name = CP932Converter::convertFromUTF8(sequence->track(track)->getName());
	MidiEvent::writeDeltaClock(stream, seq_name.size());   // seq_nameの文字数
	//TODO: 第3引数の型キャスト要らなくなるかも
	stream->write(seq_name.c_str(), 0, (int)seq_name.size());

	// Meta Textを準備
	TextStream textStream;
	printMetaText(sequence->track(track), textStream, sequence->getTotalClocks() + 120,
				  0, printPitch, master, mixer);
	tick_t lastClock = 0;
	std::vector<MidiEvent> meta = getMidiEventsFromMetaText(&textStream, encoding);
	for (int i = 0; i < meta.size(); i++) {
		MidiEvent::writeDeltaClock(stream, meta[i].clock - lastClock);
		meta[i].writeData(stream);
		lastClock = meta[i].clock;
	}
	tick_t maxClock = lastClock;

	lastClock = 0;
	std::vector<MidiEvent> nrpns =
		VocaloidMidiEventListFactory::generateMidiEventList(
			sequence->track(track), &sequence->tempoList,
			sequence->getTotalClocks(), sequence->getPreMeasureClocks(), msPreSend
		);
	for (int i = 0; i < nrpns.size(); i++) {
		MidiEvent item = nrpns[i];
		MidiEvent::writeDeltaClock(stream, item.clock - lastClock);
		item.writeData(stream);
		lastClock = item.clock;
	}
	maxClock = std::max(maxClock, lastClock);

	// トラックエンド
	lastClock = maxClock;
	const Event* last_event = sequence->track(track)->events()->get(sequence->track(track)->events()->size() - 1);
	maxClock = std::max(maxClock, last_event->clock + last_event->getLength());
	tick_t lastDeltaClock = maxClock - lastClock;
	if (lastDeltaClock < 0) {
		lastDeltaClock = 0;
	}
	MidiEvent::writeDeltaClock(stream, lastDeltaClock);
	stream->write(0xff);
	stream->write(0x2f);
	stream->write(0x00);
	int64_t pos = stream->getPointer();
	stream->seek(first_position - 4);
	writeUnsignedInt(stream, pos - first_position);
	stream->seek(pos);
}

std::vector<MidiEvent> VSQFileWriter::getMidiEventsFromMetaText(TextStream* sr, const std::string& encoding)
{
	std::string _NL = "\n";// + (char)0x0a;
	std::vector<MidiEvent> ret;
	sr->setPointer(-1);
	std::string tmp = "";
	if (sr->ready()) {
		int line_count = -1;
		std::vector<int> buffer;
		bool first = true;
		while (sr->ready()) {
			if (first) {
				tmp = sr->readLine();
				first = false;
			} else {
				tmp = _NL + sr->readLine();
			}
			std::string line = CP932Converter::convertFromUTF8(tmp);
			for (int i = 0; i < line.size(); i++) {
				buffer.push_back(0xff & line[i]);
			}
			std::vector<int> prefix = getLinePrefixBytes(line_count + 1);
			while (prefix.size() + buffer.size() >= 127) {
				line_count++;
				prefix = getLinePrefixBytes(line_count);
				MidiEvent add;
				add.clock = 0;
				add.firstByte = 0xff;
				add.data.push_back(0x01);
				for (int i = 0; i < prefix.size(); i++) {
					add.data.push_back(prefix[i]);
				}
				int remain = 127;
				for (int i = 0; !buffer.empty() && prefix.size() + i < remain; i++) {
					add.data.push_back(buffer[0]);
					buffer.erase(buffer.begin());
				}
				ret.push_back(add);
				prefix = getLinePrefixBytes(line_count + 1);
			}
		}
		if (false == buffer.empty()) {
			std::vector<int> prefix = getLinePrefixBytes(line_count + 1);
			while (prefix.size() + buffer.size() >= 127) {
				line_count = line_count + 1;
				prefix = getLinePrefixBytes(line_count);
				MidiEvent add;
				add.clock = 0;
				add.firstByte = 0xff;
				add.data.push_back(0x01);
				int remain = 127;
				for (int i = 0; i < prefix.size(); i++) {
					add.data.push_back(prefix[i]);
				}
				for (int i = 0; !buffer.empty() && prefix.size() + i < remain; i++) {
					add.data.push_back(buffer[0]);
					buffer.erase(buffer.begin());
				}
				ret.push_back(add);
				prefix = getLinePrefixBytes(line_count + 1);
			}
			if (false == buffer.empty()) {
				line_count = line_count + 1;
				prefix = getLinePrefixBytes(line_count);
				MidiEvent add;
				add.clock = 0;
				add.firstByte = 0xff;
				int remain = prefix.size() + buffer.size();
				add.data.push_back(0x01);
				for (int i = 0; i < prefix.size(); i++) {
					add.data.push_back(prefix[i]);
				}
				for (int i = 0; !buffer.empty() && prefix.size() + i < remain; i++) {
					add.data.push_back(buffer[0]);
					buffer.erase(buffer.begin());
				}
				ret.push_back(add);
			}
		}
	}

	return ret;
}

std::vector<int> VSQFileWriter::getLinePrefixBytes(int count)
{
	int digits = getHowManyDigits(count);
	int c = (int)::floor((digits - 1) / 4) + 1;
	std::ostringstream format;
	format << "DM:%0" << (c * 4) << "d:";
	char* str = new char[1024];
	::memset(str, 0, 1024);
	sprintf(str, format.str().c_str(), count);

	std::string resultString = str;
	delete [] str;

	std::vector<int> result;
	for (int i = 0; i < resultString.size(); i++) {
		result.push_back(0xff & resultString[i]);
	}
	return result;
}

int VSQFileWriter::getHowManyDigits(int number)
{
	number = ::abs(number);
	if (number == 0) {
		return 1;
	} else {
		return (int)::floor(::log10(number)) + 1;
	}
}

void VSQFileWriter::writeUnsignedShort(OutputStream* stream, int data)
{
	std::vector<char> dat = BitConverter::getBytesUInt16BE(data);
	stream->write(dat.data(), 0, dat.size());
}

void VSQFileWriter::writeUnsignedInt(OutputStream* stream, int data)
{
	std::vector<char> dat = BitConverter::getBytesUInt32BE(data);
	stream->write(dat.data(), 0, dat.size());
}

std::vector<Handle> VSQFileWriter::writeEventList(std::vector<TempEvent*>& eventList, TextStream& stream, tick_t eos)
{
	std::vector<Handle> handles = getHandleList(eventList);
	stream.writeLine("[EventList]");
	std::vector<TempEvent> temp;
	for (std::vector<TempEvent*>::iterator itr = eventList.begin(); itr != eventList.end(); ++itr) {
		temp.push_back(**itr);
	}
	std::stable_sort(temp.begin(), temp.end(), Event::compare);
	int i = 0;
	while (i < temp.size()) {
		TempEvent item = temp[i];
		if (! item.isEOS()) {
			std::ostringstream ids;
			ids << "ID#" << StringUtil::toString(item.index, "%04d");
			tick_t clock = temp[i].clock;
			while (i + 1 < temp.size() && clock == temp[i + 1].clock) {
				i++;
				ids << ",ID#" << StringUtil::toString(temp[i].index, "%04d");
			}
			std::ostringstream oss;
			oss << clock << "=" << ids.str();
			stream.writeLine(oss.str());
		}
		i++;
	}
	stream.write(StringUtil::toString(eos, "%d"));
	stream.writeLine("=EOS");
	return handles;
}

const std::vector<Handle> VSQFileWriter::getHandleList(std::vector<TempEvent*>& eventList)
{
	std::vector<Handle> handle;
	int current_id = -1;
	int current_handle = -1;
	bool add_quotation_mark = true;
	for (std::vector<TempEvent*>::iterator itr = eventList.begin(); itr != eventList.end(); ++itr) {
		TempEvent* item = *itr;
		current_id = current_id + 1;
		item->index = current_id;
		// SingerHandle
		if (item->singerHandle.getHandleType() == HandleType::SINGER) {
			current_handle = current_handle + 1;
			item->singerHandle.index = current_handle;
			handle.push_back(item->singerHandle);
			item->singerHandleIndex = current_handle;
			VoiceLanguage::VoiceLanguageEnum lang = VoiceLanguage::valueFromSingerName(item->singerHandle.ids);
			add_quotation_mark = lang == VoiceLanguage::JAPANESE;
		}
		// LyricHandle
		if (item->lyricHandle.getHandleType() == HandleType::LYRIC) {
			current_handle = current_handle + 1;
			item->lyricHandle.index = current_handle;
			item->lyricHandle.addQuotationMark = add_quotation_mark;
			handle.push_back(item->lyricHandle);
			item->lyricHandleIndex = current_handle;
		}
		// VibratoHandle
		if (item->vibratoHandle.getHandleType() == HandleType::VIBRATO) {
			current_handle = current_handle + 1;
			item->vibratoHandle.index = current_handle;
			handle.push_back(item->vibratoHandle);
			item->vibratoHandleIndex = current_handle;
		}
		// NoteHeadHandle
		if (item->noteHeadHandle.getHandleType() == HandleType::NOTE_HEAD) {
			current_handle = current_handle + 1;
			item->noteHeadHandle.index = current_handle;
			handle.push_back(item->noteHeadHandle);
			item->noteHeadHandleIndex = current_handle;
		}
		// IconDynamicsHandle
		if (item->iconDynamicsHandle.getHandleType() == HandleType::DYNAMICS) {
			current_handle = current_handle + 1;
			item->iconDynamicsHandle.index = current_handle;
			item->iconDynamicsHandle.setLength(item->getLength());
			handle.push_back(item->iconDynamicsHandle);
			// IconDynamicsHandleは, 歌手ハンドルと同じ扱いなので
			// _singerHandleIndexでよい
			item->singerHandleIndex = current_handle;
		}
	}
	return handle;
}

std::string VSQFileWriter::getTrackHeader()
{
	return std::string("MTrk");
}

VSQ_END_NAMESPACE
