/**
 * VSQFileReader.cpp
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
#include "../include/libvsq/VSQFileReader.hpp"
#include "../include/libvsq/SMFReader.hpp"
#include "../include/libvsq/Sequence.hpp"
#include "../include/libvsq/TextStream.hpp"
#include "../include/libvsq/StringUtil.hpp"
#include "../include/libvsq/CP932Converter.hpp"
#include <sstream>

VSQ_BEGIN_NAMESPACE

void VSQFileReader::read(Sequence& sequence, InputStream& stream, std::string const& encoding)
{
	std::vector<std::vector<MidiEvent> > events;
	SMFReader reader;
	int format, timeFormat;
	reader.read(stream, events, format, timeFormat);

	int num_track = events.size();
	sequence.tracks().clear();
	for (int i = 1; i < num_track; i++) {
		TextStream textStream;
		std::string trackName;
		getMetatextByMidiEventList(events[i], encoding, textStream, trackName);
		Track track;
		if (i == 1) {
			Master master;
			Mixer mixer;
			track = getTrackByTextStream(textStream, &master, &mixer);
			sequence.master = master;
			sequence.mixer = mixer;
		} else {
			track = getTrackByTextStream(textStream);
		}
		track.name(trackName);
		sequence.tracks().push_back(track);
	}

	parseTempoList(events[0], sequence.tempoList);
	parseTimesigList(events[0], sequence.timesigList);

	// 曲の長さを計算
	sequence.tempoList.updateTempoInfo();
	sequence.updateTotalTicks();
}

VSQFileReader::TentativeEvent
VSQFileReader::parseEvent(TextStream& stream, std::string& lastLine)
{
	TentativeEvent result;
	result.setType(EventType::UNKNOWN);
	result.singerHandleIndex = -2;
	result.lyricHandleIndex = -1;
	result.vibratoHandleIndex = -1;
	result.noteHeadHandleIndex = -1;
	result.length(0);
	result.note = 0;
	result.dynamics = 64;
	result.pmBendDepth = 8;
	result.pmBendLength = 0;
	result.pmbPortamentoUse = 0;
	result.demDecGainRate = 50;
	result.demAccent = 50;
	result.vibratoDelay = 0;
	result.setEOS(false);
	lastLine = stream.readLine();
	while (lastLine.find("[") != 0) {
		std::vector<std::string> parameters = StringUtil::explode("=", lastLine);
		std::string search = parameters[0];
		if (search == "Type") {
			if (parameters[1] == "Anote") {
				result.setType(EventType::NOTE);
			} else if (parameters[1] == "Singer") {
				result.setType(EventType::SINGER);
			} else if (parameters[1] == "Aicon") {
				result.setType(EventType::ICON);
			} else {
				result.setType(EventType::UNKNOWN);
			}
		} else if (search == "Length") {
			result.length(StringUtil::parseInt<tick_t>(parameters[1]));
		} else if (search == "Note#") {
			result.note = StringUtil::parseInt<int>(parameters[1]);
		} else if (search == "Dynamics") {
			result.dynamics = StringUtil::parseInt<int>(parameters[1]);
		} else if (search == "PMBendDepth") {
			result.pmBendDepth = StringUtil::parseInt<int>(parameters[1]);
		} else if (search == "PMBendLength") {
			result.pmBendLength = StringUtil::parseInt<int>(parameters[1]);
		} else if (search == "DEMdecGainRate") {
			result.demDecGainRate = StringUtil::parseInt<int>(parameters[1]);
		} else if (search == "DEMaccent") {
			result.demAccent = StringUtil::parseInt<int>(parameters[1]);
		} else if (search == "LyricHandle") {
			result.lyricHandleIndex = Handle::getHandleIndexFromString(parameters[1]);
		} else if (search == "IconHandle") {
			result.singerHandleIndex = Handle::getHandleIndexFromString(parameters[1]);
		} else if (search == "VibratoHandle") {
			result.vibratoHandleIndex = Handle::getHandleIndexFromString(parameters[1]);
		} else if (search == "VibratoDelay") {
			result.vibratoDelay = StringUtil::parseInt<int>(parameters[1]);
		} else if (search == "PMbPortamentoUse") {
			result.pmbPortamentoUse = StringUtil::parseInt<int>(parameters[1]);
		} else if (search == "NoteHeadHandle") {
			result.noteHeadHandleIndex = Handle::getHandleIndexFromString(parameters[1]);
		}
		if (! stream.ready()) {
			break;
		}
		lastLine = stream.readLine();
	}
	return result;
}

Handle VSQFileReader::parseHandle(TextStream& stream, int index, std::string& lastLine)
{
	TentativeHandle result(HandleType::UNKNOWN);
	result.index = index;

	// default値で埋める
	result.setHandleType(HandleType::VIBRATO);
	result.iconId = "";
	result.ids = "normal";
	result.setLyrics(std::vector<Lyric>());
	result.add(Lyric(""));
	result.original = 0;
	result.caption = "";
	result.length(0);
	result.startDepth = 0;
	result.startRate = 0;
	result.language = 0;
	result.program = 0;
	result.duration = 0;
	result.depth = 64;

	std::string tmpDepthBPX = "";
	std::string tmpDepthBPY = "";
	std::string tmpDepthBPNum = "";

	std::string tmpRateBPX = "";
	std::string tmpRateBPY = "";
	std::string tmpRateBPNum = "";

	std::string tmpDynBPX = "";
	std::string tmpDynBPY = "";
	std::string tmpDynBPNum = "";

	// "["にぶち当たるまで読込む
	lastLine = stream.readLine();
	while (lastLine.find("[") != 0) {
		std::vector<std::string> parameters = StringUtil::explode("=", lastLine);
		std::string search = parameters[0];
		if (search == "Language") {
			result.setHandleType(HandleType::SINGER);
			result.language = StringUtil::parseInt<int>(parameters[1]);
		} else if (search == "Program") {
			result.program = StringUtil::parseInt<int>(parameters[1]);
		} else if (search == "IconID") {
			result.iconId = parameters[1];
		} else if (search == "IDS") {
			result.ids = parameters[1];
			for (int i = 2; i < parameters.size(); i++) {
				result.ids = result.ids + "=" + parameters[i];
			}
		} else if (search == "Original") {
			result.original = StringUtil::parseInt<int>(parameters[1]);
		} else if (search == "Caption") {
			result.caption = parameters[1];
			for (int i = 2; i < parameters.size(); i++) {
				result.caption = result.caption + "=" + parameters[i];
			}
		} else if (search == "Length") {
			result.length(StringUtil::parseInt<tick_t>(parameters[1]));
		} else if (search == "StartDepth") {
			result.startDepth = StringUtil::parseInt<int>(parameters[1]);
		} else if (search == "DepthBPNum") {
			tmpDepthBPNum = parameters[1];
		} else if (search == "DepthBPX") {
			tmpDepthBPX = parameters[1];
		} else if (search == "DepthBPY") {
			tmpDepthBPY = parameters[1];
		} else if (search == "StartRate") {
			result.setHandleType(HandleType::VIBRATO);
			result.startRate = StringUtil::parseInt<int>(parameters[1]);
		} else if (search == "RateBPNum") {
			tmpRateBPNum = parameters[1];
		} else if (search == "RateBPX") {
			tmpRateBPX = parameters[1];
		} else if (search == "RateBPY") {
			tmpRateBPY = parameters[1];
		} else if (search == "Duration") {
			result.setHandleType(HandleType::NOTE_HEAD);
			result.duration = StringUtil::parseInt<int>(parameters[1]);
		} else if (search == "Depth") {
			result.depth = StringUtil::parseInt<int>(parameters[1]);
		} else if (search == "StartDyn") {
			result.setHandleType(HandleType::DYNAMICS);
			result.startDyn = StringUtil::parseInt<int>(parameters[1]);
		} else if (search == "EndDyn") {
			result.setHandleType(HandleType::DYNAMICS);
			result.endDyn = StringUtil::parseInt<int>(parameters[1]);
		} else if (search == "DynBPNum") {
			tmpDynBPNum = parameters[1];
		} else if (search == "DynBPX") {
			tmpDynBPX = parameters[1];
		} else if (search == "DynBPY") {
			tmpDynBPY = parameters[1];
		} else if (search.find("L") == 0 && search.size() >= 2) {
			int index = StringUtil::parseInt<int>(search.substr(1, 1));
			Lyric lyric(parameters[1]);
			result.setHandleType(HandleType::LYRIC);
			if (result.size() <= index + 1) {
				int amount = index + 1 - result.size();
				for (int i = 0; i < amount; i++) {
					result.add(Lyric("", ""));
				}
			}
			result.set(index, lyric);
		}
		if (! stream.ready()) {
			break;
		}
		lastLine = stream.readLine();
	}

	// parse RateBPX and RateBPY
	if (result.type() == HandleType::VIBRATO) {
		if (tmpRateBPNum != "") {
			result.rateBP = VibratoBPList(tmpRateBPNum, tmpRateBPX, tmpRateBPY);
		} else {
			result.rateBP = VibratoBPList();
		}

		// parse DepthBPX, DepthBPY
		if (tmpDepthBPNum != "") {
			result.depthBP = VibratoBPList(tmpDepthBPNum, tmpDepthBPX, tmpDepthBPY);
		} else {
			result.depthBP = VibratoBPList();
		}
	} else {
		result.depthBP = VibratoBPList();
		result.rateBP = VibratoBPList();
	}

	if (tmpDynBPNum != "") {
		result.dynBP = VibratoBPList(tmpDynBPNum, tmpDynBPX, tmpDynBPY);
	} else {
		result.dynBP = VibratoBPList();
	}
	return result;
}

void VSQFileReader::parseTimesigList(std::vector<MidiEvent> const& midiEventList, TimesigList& timesigList)
{
	timesigList.clear();
	int dnominator = 4;
	int numerator = 4;
	int count = -1;
	for (int j = 0; j < midiEventList.size(); j++) {
		if (midiEventList[j].firstByte == 0xff && midiEventList[j].data.size() >= 5 && midiEventList[j].data[0] == 0x58) {
			count++;
			numerator = midiEventList[j].data[1];
			dnominator = 1;
			for (int i = 0; i < midiEventList[j].data[2]; i++) {
				dnominator = dnominator * 2;
			}
			if (count == 0) {
				if (midiEventList[j].tick == 0) {
					timesigList.push(Timesig(numerator, dnominator, 0));
				} else {
					timesigList.push(Timesig(4, 4, 0));
					timesigList.push(Timesig(numerator, dnominator, (int)midiEventList[j].tick / (480 * 4)));
					count++;
				}
			} else {
				int lastNumerator = timesigList.get(count - 1).numerator;
				int lastDenominator = timesigList.get(count - 1).denominator;
				tick_t tick = timesigList.get(count - 1).tick();
				int barCount = timesigList.get(count - 1).barCount;
				int dif = 480 * 4 / lastDenominator * lastNumerator;//1小節が何クロックか？
				barCount += ((int)midiEventList[j].tick - tick) / dif;
				timesigList.push(Timesig(numerator, dnominator, barCount));
			}
		}
	}
}

void VSQFileReader::parseTempoList(std::vector<MidiEvent> const& midiEventList, TempoList& tempoList)
{
	tempoList.clear();
	int lastTempo = 500000;
	int count = -1;
	int listSize = midiEventList.size();
	for (int j = 0; j < listSize; j++) {
		MidiEvent item = midiEventList[j];
		if (item.firstByte == 0xff && item.data.size() >= 4 && item.data[0] == 0x51) {
			count++;
			if (count == 0 && item.tick != 0) {
				tempoList.push(Tempo(0, lastTempo));
			}
			int currentTempo = item.data[1] << 16 | item.data[2] << 8 | item.data[3];
			tick_t currentTick = midiEventList[j].tick;
			tempoList.push(Tempo(currentTick, currentTempo));
			lastTempo = currentTempo;
		}
	}
	tempoList.updateTempoInfo();
}

void VSQFileReader::getMetatextByMidiEventList(std::vector<MidiEvent> const& midiEventList, std::string const& encoding, TextStream& stream, std::string& trackName)
{
	int count = midiEventList.size();
	std::vector<int> buffer;
	for (int i = 0; i < count; i++) {
		MidiEvent item = midiEventList[i];
		if (item.firstByte == 0xff && item.data.size() > 0) {
			// meta textを抽出
			int type = item.data[0];
			if (type == 0x01 || type == 0x03) {
				if (type == 0x01) {
					int colonCount = 0;
					for (int j = 1; j < item.data.size(); j++) {
						int d = item.data[j];
						if (d == 0x3a) {
							colonCount++;
							if (colonCount <= 2) {
								continue;
							}
						}
						if (colonCount < 2) {
							continue;
						}
						buffer.push_back(d);
					}

					std::vector<int>::iterator lineFeedPosition = std::find(buffer.begin(), buffer.end(), 0x0a);
					while (lineFeedPosition != buffer.end()) {
						std::ostringstream copy;
						int lineFeedIndex = lineFeedPosition - buffer.begin();
						for (int j = 0; j < lineFeedIndex; j++) {
							copy << (char)(0xff & buffer[0]);
							buffer.erase(buffer.begin());
						}

						std::string line = CP932Converter::convertToUTF8(copy.str());
						stream.writeLine(line);
						buffer.erase(buffer.begin());
						lineFeedPosition = std::find(buffer.begin(), buffer.end(), 0x0a);
					}
				} else {
					for (int j = 1; j < item.data.size(); j++) {
						buffer.push_back(item.data[j]);
					}
					int c = buffer.size();
					std::ostringstream copy;
					for (int j = 0; j < c; j++) {
						copy << (char)(0xff & buffer[j]);
					}
					trackName = CP932Converter::convertToUTF8(copy.str());
					buffer.clear();
				}
			}
		} else {
			continue;
		}
	}

	int remain = buffer.size();
	if (0 < remain) {
		std::ostringstream copy;
		for (int j = 0; j < remain; j++) {
			copy << (char)(0xff & buffer[j]);
		}
		std::string line = CP932Converter::convertToUTF8(copy.str());
		stream.writeLine(line);
	}

	stream.setPointer(-1);
}

Track VSQFileReader::getTrackByTextStream(TextStream& stream, Master* master, Mixer* mixer)
{
	std::map<int, tick_t> eventTickMap;
	std::map<int, TentativeEvent*> eventIdMap;
	std::map<int, Handle> handleIdMap;
	std::vector<Event*> temporaryEventList;

	TentativeTrack result;
	std::map<std::string, std::string> sectionNameMap = result.getSectionNameMap();

	std::string lastLine = stream.readLine();
	while (1) {
		// TextMemoryStreamから順次読込み
		if (lastLine.length() == 0) {
			break;
		}

		std::map<std::string, std::string>::const_iterator index =
			sectionNameMap.find(lastLine);
		if (index != sectionNameMap.end()) {
			std::string name = index->second;
			lastLine = result.curve(name)->appendFromText(stream);
		} else if (lastLine == "[Common]") {
			result.setCommon(Common(stream, lastLine));
		} else if (lastLine == "[Master]" && master != 0) {
			*master = Master(stream, lastLine);
		} else if (lastLine == "[Mixer]" && mixer != 0) {
			*mixer = Mixer(stream, lastLine);
		} else if (lastLine == "[EventList]") {
			lastLine = stream.readLine();
			while (lastLine.find("[") != 0) {
				std::vector<std::string> parameters = StringUtil::explode("=", lastLine);
				tick_t tick = StringUtil::parseInt<tick_t>(parameters[0]);
				if (parameters[1] != "EOS") {
					std::vector<std::string> idList = StringUtil::explode(",", parameters[1]);
					for (int i = 0; i < idList.size(); i++) {
						std::vector<std::string> idParameters = StringUtil::explode("#", idList[i]);
						int id = StringUtil::parseInt<int>(idParameters[1]);
						eventTickMap.insert(std::make_pair(id, tick));
					}
				} else {
					eventTickMap.insert(std::make_pair(-1, tick));
				}
				if (! stream.ready()) {
					break;
				} else {
					lastLine = stream.readLine();
				}
			}
		} else {
			std::string buffer = lastLine;
			buffer = StringUtil::replace(buffer, "[", "");
			buffer = StringUtil::replace(buffer, "]", "");
			std::vector<std::string> parameters = StringUtil::explode("#", buffer);
			int index = StringUtil::parseInt<int>(parameters[1]);
			if (lastLine.find("[ID#") == 0) {
				TentativeEvent* item = new TentativeEvent();
				*item = parseEvent(stream, lastLine);
				temporaryEventList.push_back(item);
				eventIdMap.insert(std::make_pair(index, item));
			} else if (lastLine.find("[h#") == 0) {
				handleIdMap.insert(std::make_pair(index, parseHandle(stream, index, lastLine)));
			}
		}

		if (! stream.ready()) {
			break;
		}
	}

	// まずhandleをidに埋め込み
	for (std::map<int, TentativeEvent*>::iterator i = eventIdMap.begin(); i != eventIdMap.end(); ++i) {
		TentativeEvent* id = i->second;
		if (handleIdMap.find(id->singerHandleIndex) != handleIdMap.end()) {
			if (id->type() == EventType::SINGER) {
				id->singerHandle = handleIdMap[id->singerHandleIndex];
			} else if (id->type() == EventType::ICON) {
				id->iconDynamicsHandle = handleIdMap[id->singerHandleIndex];
			}
		}
		if (handleIdMap.find(id->lyricHandleIndex) != handleIdMap.end()) {
			id->lyricHandle = handleIdMap[id->lyricHandleIndex];
		}
		if (handleIdMap.find(id->vibratoHandleIndex) != handleIdMap.end()) {
			id->vibratoHandle = handleIdMap[id->vibratoHandleIndex];
		}
		if (handleIdMap.find(id->noteHeadHandleIndex) != handleIdMap.end()) {
			id->noteHeadHandle = handleIdMap[id->noteHeadHandleIndex];
		}
	}

	// idをeventListに埋め込み
	Event::List& events = result.events();
	events.clear();
	int count = 0;
	for (std::map<int, tick_t>::iterator i = eventTickMap.begin(); i != eventTickMap.end(); ++i) {
		int id = i->first;
		tick_t tick = i->second;
		if (eventIdMap.find(id) != eventIdMap.end()) {
			count++;
			Event item = *eventIdMap[id];
			item.tick = tick;
			events.add(item, count);
		}
	}
	events.sort();

	for (std::vector<Event*>::iterator i = temporaryEventList.begin(); i != temporaryEventList.end(); ++i) {
		delete(*i);
	}

	return (Track)result;
}

VSQ_END_NAMESPACE
