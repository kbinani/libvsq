/**
 * VocaloidMidiEventListFactory.cpp
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
#include "../include/libvsq/VocaloidMidiEventListFactory.hpp"
#include "../include/libvsq/Track.hpp"
#include "../include/libvsq/TempoList.hpp"
#include "../include/libvsq/StringUtil.hpp"
#include <cmath>
#include <algorithm>
#include <sstream>

VSQ_BEGIN_NAMESPACE

std::vector<MidiEvent> VocaloidMidiEventListFactory::generateMidiEventList(
	Track const& target, TempoList const& tempoList, tick_t totalTicks, tick_t preMeasureTick, int msPreSend)
{
	std::vector<NrpnEvent> nrpnEventList = generateNRPN(target, tempoList, totalTicks, preMeasureTick, msPreSend);
	return NrpnEvent::convert(nrpnEventList);
}

std::vector<NrpnEvent> VocaloidMidiEventListFactory::generateNRPN(
	Track const& target, TempoList const& tempoList, tick_t totalTicks, tick_t preMeasureTick, int msPreSend)
{
	std::vector<NrpnEvent> list;

	std::string version = target.common().version;
	Event::List const& events = target.events();

	int count = events.size();
	int note_start = 0;
	int note_end = count - 1;
	for (int i = 0; i < count; i++) {
		if (0 <= events.get(i)->tick) {
			note_start = i;
			break;
		}
		note_start = i;
	}
	for (int i = count - 1; i >= 0; i--) {
		if (events.get(i)->tick <= totalTicks) {
			note_end = i;
			break;
		}
	}

	// determine first singer
	int singer_event = -1;
	for (int i = note_start; i >= 0; i--) {
		if (events.get(i)->type() == EventType::SINGER) {
			singer_event = i;
			break;
		}
	}
	if (singer_event >= 0) {
		// first singer was found
		std::vector<NrpnEvent> singerNrpnList = generateSingerNRPN(tempoList, *events.get(singer_event), 0);
		list.insert(list.end(), singerNrpnList.begin(), singerNrpnList.end());
	} else {
		// first singer was not found. may be rate-case
		list.push_back(NrpnEvent(0, MidiParameterType::CC_BS_LANGUAGE_TYPE, 0x0));
		list.push_back(NrpnEvent(0, MidiParameterType::PC_VOICE_TYPE, 0x0));
	}

	std::vector<NrpnEvent> voiceChangeParameterNrpn = generateVoiceChangeParameterNRPN(target, tempoList, msPreSend, preMeasureTick);
	list.insert(list.end(), voiceChangeParameterNrpn.begin(), voiceChangeParameterNrpn.end());
	if (version.substr(0, 4) == "DSB2") {
		std::vector<NrpnEvent> fx2DepthNrpn = generateFx2DepthNRPN(target, tempoList, msPreSend);
		list.insert(list.end(), fx2DepthNrpn.begin(), fx2DepthNrpn.end());
	}

	int ms_presend = msPreSend;
	if (target.curve("dyn")->size() > 0) {
		std::vector<NrpnEvent> listdyn = generateExpressionNRPN(target, tempoList, ms_presend);
		if (! listdyn.empty()) {
			list.insert(list.end(), listdyn.begin(), listdyn.end());
		}
	}
	if (target.curve("pbs")->size() > 0) {
		std::vector<NrpnEvent> listpbs = generatePitchBendSensitivityNRPN(target, tempoList, ms_presend);
		if (! listpbs.empty()) {
			list.insert(list.end(), listpbs.begin(), listpbs.end());
		}
	}
	if (target.curve("pit")->size() > 0) {
		std::vector<NrpnEvent> listpit = generatePitchBendNRPN(target, tempoList, ms_presend);
		if (! listpit.empty()) {
			list.insert(list.end(), listpit.begin(), listpit.end());
		}
	}

	int lastDelay = 0;
	int last_note_end = 0;
	for (int i = note_start; i <= note_end; i++) {
		const Event* item = events.get(i);
		if (item->type() == EventType::NOTE) {
			int note_loc = 0x03;
			if (item->tick == last_note_end) {
				note_loc = note_loc - 0x02;
			}

			// find next note event
			tick_t nexttick = item->tick + item->length() + 1;
			int event_count = events.size();
			for (int j = i + 1; j < event_count; j++) {
				const Event* itemj = events.get(j);
				if (itemj->type() == EventType::NOTE) {
					nexttick = itemj->tick;
					break;
				}
			}
			if (item->tick + item->length() == nexttick) {
				note_loc = note_loc - 0x01;
			}

			int delay;
			NrpnEvent noteNrpn =
				generateNoteNRPN(target, tempoList, *item, msPreSend, note_loc, &lastDelay, &delay);
			lastDelay = delay;

			list.push_back(noteNrpn);
			std::vector<NrpnEvent> vibratoNrpn = generateVibratoNRPN(tempoList, *item, msPreSend);
			list.insert(list.end(), vibratoNrpn.begin(), vibratoNrpn.end());
			last_note_end = item->tick + item->length();
		} else if (item->type() == EventType::SINGER) {
			if (i > note_start && i != singer_event) {
				std::vector<NrpnEvent> singerNrpn = generateSingerNRPN(tempoList, *item, msPreSend);
				list.insert(list.end(), singerNrpn.begin(), singerNrpn.end());
			}
		}
	}

	std::stable_sort(list.begin(), list.end(), NrpnEvent::compare);
	std::vector<NrpnEvent> merged;
	for (int i = 0; i < list.size(); i++) {
		std::vector<NrpnEvent> expanded = list[i].expand();
		merged.insert(merged.end(), expanded.begin(), expanded.end());
	}
	return merged;
}

std::vector<NrpnEvent> VocaloidMidiEventListFactory::generateExpressionNRPN(Track const& track, TempoList const& tempoList, int preSendMilliseconds)
{
	std::vector<NrpnEvent> ret;
	BPList const* dyn = track.curve("DYN");
	NrpnEventProvider* provider = new NrpnEventProvider(MidiParameterType::CC_E_DELAY, MidiParameterType::CC_E_EXPRESSION);
	generateNRPNByBPList(ret, tempoList, preSendMilliseconds, *dyn, *provider);
	delete provider;
	return ret;
}

NrpnEvent VocaloidMidiEventListFactory::generateHeaderNRPN()
{
	NrpnEvent ret(0, MidiParameterType::CC_BS_VERSION_AND_DEVICE, 0x00, 0x00);
	ret.append(MidiParameterType::CC_BS_DELAY, 0x00, 0x00);
	ret.append(MidiParameterType::CC_BS_LANGUAGE_TYPE, 0x00);
	return ret;
}

std::vector<NrpnEvent> VocaloidMidiEventListFactory::generateSingerNRPN(TempoList const& tempoList, Event const& singerEvent, int preSendMilliseconds)
{
	tick_t tick = singerEvent.tick;
	Handle singer_handle;

	double tick_msec = tempoList.timeFromTick(tick) * 1000.0;

	double msEnd = tempoList.timeFromTick(singerEvent.tick + singerEvent.length()) * 1000.0;
	int duration = (int)::floor(::ceil(msEnd - tick_msec));

	int duration0, duration1;
	_getMsbAndLsb(duration, &duration0, &duration1);

	tick_t actualTick;
	int delay;
	_getActualTickAndDelay(tempoList, tick, preSendMilliseconds, &actualTick, &delay);
	int delayMsb, delayLsb;
	_getMsbAndLsb(delay, &delayMsb, &delayLsb);

	NrpnEvent add(actualTick, MidiParameterType::CC_BS_VERSION_AND_DEVICE, 0x00, 0x00);
	add.append(MidiParameterType::CC_BS_DELAY, delayMsb, delayLsb, true);
	add.append(MidiParameterType::CC_BS_LANGUAGE_TYPE, singer_handle.language, true);
	add.append(MidiParameterType::PC_VOICE_TYPE, singer_handle.program);

	std::vector<NrpnEvent> ret;
	ret.push_back(add);
	return ret;
}

NrpnEvent VocaloidMidiEventListFactory::generateNoteNRPN(Track const& track, TempoList const& tempoList, Event const& noteEvent, int msPreSend, int noteLocation, int* lastDelay, int* delay)
{
	tick_t tick = noteEvent.tick;
	NrpnEvent add(0, MidiParameterType::CC_BS_DELAY, 0, 0);

	tick_t actualTick;
	_getActualTickAndDelay(tempoList, tick, msPreSend, &actualTick, delay);

	// take care, lastDelay and delay are both pointer
	bool addInitialized = false;
	int lastDelayValue;
	if (0 == lastDelay) {
		add = NrpnEvent(
				  actualTick,
				  MidiParameterType::CVM_NM_VERSION_AND_DEVICE,
				  0x00, 0x00
			  );
		lastDelayValue = 0;
		addInitialized = true;
	} else {
		lastDelayValue = *lastDelay;
	}

	if (lastDelayValue != *delay) {
		int delayMsb, delayLsb;
		_getMsbAndLsb(*delay, &delayMsb, &delayLsb);
		if (false == addInitialized) {
			//TODO: In this case, CVM_NM_VERSION_AND_DEVICE is omitted. Farther verification is required.
			add = NrpnEvent(actualTick, MidiParameterType::CVM_NM_DELAY, delayMsb, delayLsb);
			addInitialized = true;
		} else {
			add.append(MidiParameterType::CVM_NM_DELAY, delayMsb, delayLsb, true);
		}
	}

	if (false == addInitialized) {
		add = NrpnEvent(actualTick, MidiParameterType::CVM_NM_NOTE_NUMBER, noteEvent.note);
	} else {
		add.append(MidiParameterType::CVM_NM_NOTE_NUMBER, noteEvent.note, true);
	}

	// Velocity
	add.append(MidiParameterType::CVM_NM_VELOCITY, noteEvent.dynamics, true);

	// Note Duration
	double msEnd = tempoList.timeFromTick(tick + noteEvent.length()) * 1000.0;
	double tick_msec = tempoList.timeFromTick(tick) * 1000.0;
	int duration = (int)::floor(msEnd - tick_msec);
	int duration0, duration1;
	_getMsbAndLsb(duration, &duration0, &duration1);
	add.append(MidiParameterType::CVM_NM_NOTE_DURATION, duration0, duration1, true);

	// Note Location
	add.append(MidiParameterType::CVM_NM_NOTE_LOCATION, noteLocation, true);

	if (noteEvent.vibratoHandle.type() != HandleType::UNKNOWN) {
		add.append(MidiParameterType::CVM_NM_INDEX_OF_VIBRATO_DB, 0x00, 0x00, true);
		std::string icon_id = noteEvent.vibratoHandle.iconId;
		std::string num = icon_id.substr(icon_id.length() - 3);
		int vibrato_type = StringUtil::parseInt<int>(num, 16);
		int note_length = noteEvent.length();
		int vibrato_delay = noteEvent.vibratoDelay;
		int bVibratoDuration = (int)::floor((note_length - vibrato_delay) / (double)note_length * 127.0);
		int bVibratoDelay = 0x7f - bVibratoDuration;
		add.append(MidiParameterType::CVM_NM_VIBRATO_CONFIG, vibrato_type, bVibratoDuration, true);
		add.append(MidiParameterType::CVM_NM_VIBRATO_DELAY, bVibratoDelay, true);
	}

	std::vector<std::string> spl = noteEvent.lyricHandle.get(0).phoneticSymbolList();
	std::ostringstream os;
	for (int j = 0; j < spl.size(); j++) {
		os << spl[j];
	}
	std::string s = os.str();
	std::vector<std::string> symbols;
	for (int i = 0; i < s.length(); i++) {
		symbols.push_back(s.substr(i, 1));
	}

	std::string renderer = track.common().version;
	if (renderer.substr(0, 4) == std::string("DSB2")) {
		add.append((MidiParameterType)0x5011, 0x01, true);  //TODO: Meaning of (byte)0x5011 is unknown.
	}

	add.append(MidiParameterType::CVM_NM_PHONETIC_SYMBOL_BYTES, symbols.size(), true);  // (byte)0x12(Number of phonetic symbols in bytes)
	int count = -1;
	std::vector<int> consonantAdjustment = noteEvent.lyricHandle.get(0).consonantAdjustmentList();
	for (int j = 0; j < spl.size(); j++) {
		std::string chars = spl[j];
		for (int k = 0; k < chars.length(); k++) {
			count = count + 1;
			if (k == 0) {
				add.append((MidiParameterType)((0x50 << 8) | (0x13 + count)), chars[k], consonantAdjustment[j], true);   // Phonetic symbol j
			} else {
				add.append((MidiParameterType)((0x50 << 8) | (0x13 + count)), chars[k], true);   // Phonetic symbol j
			}
		}
	}
	if (renderer.substr(0, 4) != std::string("DSB2")) {
		add.append(MidiParameterType::CVM_NM_PHONETIC_SYMBOL_CONTINUATION, 0x7f, true);   // End of phonetic symbols
	}
	if (renderer.substr(0, 4) == std::string("DSB3")) {
		int v1mean = (int)::floor(noteEvent.pmBendDepth * 60 / 100);
		if (v1mean < 0) {
			v1mean = 0;
		}
		if (60 < v1mean) {
			v1mean = 60;
		}
		int d1mean = (int)::floor(0.3196 * noteEvent.pmBendLength + 8.0);
		int d2mean = (int)::floor(0.92 * noteEvent.pmBendLength + 28.0);
		add.append(MidiParameterType::CVM_NM_V1MEAN, v1mean, true);  // (byte)0x50(v1mean)
		add.append(MidiParameterType::CVM_NM_D1MEAN, d1mean, true);  // (byte)0x51(d1mean)
		add.append(MidiParameterType::CVM_NM_D1MEAN_FIRST_NOTE, 0x14, true);  // (byte)0x52(d1meanFirstNote)
		add.append(MidiParameterType::CVM_NM_D2MEAN, d2mean, true);  // (byte)0x53(d2mean)
		add.append(MidiParameterType::CVM_NM_D4MEAN, noteEvent.d4mean, true);  // (byte)0x54(d4mean)
		add.append(MidiParameterType::CVM_NM_PMEAN_ONSET_FIRST_NOTE, noteEvent.pMeanOnsetFirstNote, true);   // 055(pMeanOnsetFirstNote)
		add.append(MidiParameterType::CVM_NM_VMEAN_NOTE_TRNSITION, noteEvent.vMeanNoteTransition, true);   // (byte)0x56(vMeanNoteTransition)
		add.append(MidiParameterType::CVM_NM_PMEAN_ENDING_NOTE, noteEvent.pMeanEndingNote, true);  // (byte)0x57(pMeanEndingNote)
		add.append(MidiParameterType::CVM_NM_ADD_PORTAMENTO, noteEvent.pmbPortamentoUse, true);  // (byte)0x58(AddScoopToUpInternals&AddPortamentoToDownIntervals)
		int decay = (int)::floor(noteEvent.demDecGainRate / 100.0 * 0x64);
		add.append(MidiParameterType::CVM_NM_CHANGE_AFTER_PEAK, decay, true);  // (byte)0x59(changeAfterPeak)
		int accent = (int)::floor(0x64 * noteEvent.demAccent / 100.0);
		add.append(MidiParameterType::CVM_NM_ACCENT, accent, true);  // (byte)0x5a(Accent)
	}
	add.append(MidiParameterType::CVM_NM_NOTE_MESSAGE_CONTINUATION, 0x7f, true);  // (byte)0x7f(Note message continuation)
	return add;
}

std::vector<NrpnEvent> VocaloidMidiEventListFactory::generatePitchBendNRPN(Track const& track, TempoList const& tempoList, int msPreSend)
{
	std::vector<NrpnEvent> ret;
	BPList const* pit = track.curve("PIT");
	PitchBendNrpnEventProvider* provider = new PitchBendNrpnEventProvider();
	generateNRPNByBPList(ret, tempoList, msPreSend, *pit, *provider);
	delete provider;
	return ret;
}

std::vector<NrpnEvent> VocaloidMidiEventListFactory::generatePitchBendSensitivityNRPN(Track const& track, TempoList const& tempoList, int msPreSend)
{
	std::vector<NrpnEvent> ret;
	BPList const* pbs = track.curve("PBS");
	PitchBendSensitivityNrpnEventProvider* provider = new PitchBendSensitivityNrpnEventProvider();
	generateNRPNByBPList(ret, tempoList, msPreSend, *pbs, *provider);
	delete provider;
	return ret;
}

std::vector<NrpnEvent> VocaloidMidiEventListFactory::generateVibratoNRPN(TempoList const& tempoList, Event const& noteEvent, int msPreSend)
{
	std::vector<NrpnEvent> ret;
	if (noteEvent.vibratoHandle.type() != HandleType::UNKNOWN) {
		tick_t vtick = noteEvent.tick + noteEvent.vibratoDelay;
		tick_t actualTick;
		int delay;
		_getActualTickAndDelay(tempoList, vtick, msPreSend, &actualTick, &delay);
		int delayMsb, delayLsb;
		_getMsbAndLsb(delay, &delayMsb, &delayLsb);
		NrpnEvent add2(actualTick, MidiParameterType::CC_VD_VERSION_AND_DEVICE, 0x00, 0x00);
		add2.append(MidiParameterType::CC_VR_VERSION_AND_DEVICE, 0x00, 0x00);
		add2.append(MidiParameterType::CC_VD_DELAY, delayMsb, delayLsb);
		add2.append(MidiParameterType::CC_VR_DELAY, delayMsb, delayLsb);
		// CC_VD_VIBRATO_DEPTH, CC_VR_VIBRATO_RATE では, NRPN の MSB を省略してはいけない
		add2.append(MidiParameterType::CC_VD_VIBRATO_DEPTH, noteEvent.vibratoHandle.startDepth);
		add2.append(MidiParameterType::CC_VR_VIBRATO_RATE, noteEvent.vibratoHandle.startRate);
		ret.push_back(add2);
		tick_t vlength = noteEvent.length() - noteEvent.vibratoDelay;

		VibratoBPList depthBP = noteEvent.vibratoHandle.depthBP;
		int count = depthBP.size();
		if (count > 0) {
			int lastDelay = 0;
			for (int i = 0; i < count; i++) {
				VibratoBP itemi = depthBP.get(i);
				double percent = itemi.x;
				tick_t cl = vtick + (tick_t) ::floor(percent * vlength);
				_getActualTickAndDelay(tempoList, cl, msPreSend, &actualTick, &delay);
				NrpnEvent nrpnEvent(0, MidiParameterType::CC_BS_DELAY, 0);
				if (lastDelay != delay) {
					_getMsbAndLsb(delay, &delayMsb, &delayLsb);
					nrpnEvent = NrpnEvent(actualTick, MidiParameterType::CC_VD_DELAY, delayMsb, delayLsb);
					nrpnEvent.append(MidiParameterType::CC_VD_VIBRATO_DEPTH, itemi.y);
				} else {
					nrpnEvent = NrpnEvent(actualTick, MidiParameterType::CC_VD_VIBRATO_DEPTH, itemi.y);
				}
				lastDelay = delay;
				ret.push_back(nrpnEvent);
			}
		}

		VibratoBPList rateBP = noteEvent.vibratoHandle.rateBP;
		count = rateBP.size();
		if (count > 0) {
			int lastDelay = 0;
			for (int i = 0; i < count; i++) {
				VibratoBP itemi = rateBP.get(i);
				double percent = itemi.x;
				tick_t cl = vtick + (tick_t)::floor(percent * vlength);
				_getActualTickAndDelay(tempoList, cl, msPreSend, &actualTick, &delay);
				NrpnEvent nrpnEvent(0, MidiParameterType::CC_BS_DELAY, 0);
				if (lastDelay != delay) {
					_getMsbAndLsb(delay, &delayMsb, &delayLsb);
					nrpnEvent = NrpnEvent(actualTick, MidiParameterType::CC_VR_DELAY, delayMsb, delayLsb);
					nrpnEvent.append(MidiParameterType::CC_VR_VIBRATO_RATE, itemi.y);
				} else {
					nrpnEvent = NrpnEvent(actualTick, MidiParameterType::CC_VR_VIBRATO_RATE, itemi.y);
				}
				lastDelay = delay;
				ret.push_back(nrpnEvent);
			}
		}
	}
	std::stable_sort(ret.begin(), ret.end(), NrpnEvent::compare);
	return ret;
}

std::vector<NrpnEvent> VocaloidMidiEventListFactory::generateVoiceChangeParameterNRPN(Track const& track, TempoList const& tempoList, int msPreSend, tick_t premeasure_tick)
{
	std::string renderer = track.common().version;
	std::vector<NrpnEvent> res;

	std::vector<std::string> curves;
	if (renderer.substr(0, 4) == "DSB3") {
		curves.push_back("BRE");
		curves.push_back("BRI");
		curves.push_back("CLE");
		curves.push_back("POR");
		curves.push_back("OPE");
		curves.push_back("GEN");
	} else if (renderer.substr(0, 4) == "DSB2") {
		curves.push_back("BRE");
		curves.push_back("BRI");
		curves.push_back("CLE");
		curves.push_back("POR");
		curves.push_back("GEN");
		curves.push_back("harmonics");
		curves.push_back("reso1amp");
		curves.push_back("reso1bw");
		curves.push_back("reso1freq");
		curves.push_back("reso2amp");
		curves.push_back("reso2bw");
		curves.push_back("reso2freq");
		curves.push_back("reso3amp");
		curves.push_back("reso3bw");
		curves.push_back("reso3freq");
		curves.push_back("reso4amp");
		curves.push_back("reso4bw");
		curves.push_back("reso4freq");
	} else {
		curves.push_back("BRE");
		curves.push_back("BRI");
		curves.push_back("CLE");
		curves.push_back("POR");
		curves.push_back("GEN");
	}

	int lastDelay = 0;
	for (int i = 0; i < curves.size(); i++) {
		BPList const* list = track.curve(curves[i]);
		if (list->size() > 0) {
			lastDelay = addVoiceChangeParameters(res, *list, tempoList, msPreSend, lastDelay);
		}
	}
	std::stable_sort(res.begin(), res.end(), NrpnEvent::compare);
	return res;
}

std::vector<NrpnEvent> VocaloidMidiEventListFactory::generateFx2DepthNRPN(Track const& track, TempoList const& tempoList, int preSendMilliseconds)
{
	std::vector<NrpnEvent> ret;
	BPList const* fx2depth = track.curve("fx2depth");
	NrpnEventProvider* provider = new NrpnEventProvider(MidiParameterType::CC_FX2_DELAY, MidiParameterType::CC_FX2_EFFECT2_DEPTH);
	generateNRPNByBPList(ret, tempoList, preSendMilliseconds, *fx2depth, *provider);
	delete provider;
	return ret;
}

int VocaloidMidiEventListFactory::addVoiceChangeParameters(std::vector<NrpnEvent>& dest, BPList const& list, TempoList const& tempoList, int msPreSend, int lastDelay)
{
	int id = MidiParameterTypeUtil::getVoiceChangeParameterId(list.name());
	for (int j = 0; j < list.size(); j++) {
		tick_t tick = list.keyTickAt(j);
		int value = list.get(j).value;
		tick_t actualTick;
		int delay;
		_getActualTickAndDelay(tempoList, tick, msPreSend, &actualTick, &delay);

		if (actualTick >= 0) {
			if (lastDelay != delay) {
				int delayMsb, delayLsb;
				_getMsbAndLsb(delay, &delayMsb, &delayLsb);
				dest.emplace_back(actualTick, MidiParameterType::VCP_DELAY, delayMsb, delayLsb);
				lastDelay = delay;
			}

			NrpnEvent add(actualTick, MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID, id);
			add.append(MidiParameterType::VCP_VOICE_CHANGE_PARAMETER, value, true);
			dest.push_back(add);
		}
	}
	return lastDelay;
}

void VocaloidMidiEventListFactory::_getActualTickAndDelay(TempoList const& tempoList, tick_t tick, int msPreSend, tick_t* actualTick, int* delay)
{
	double tick_msec = tempoList.timeFromTick(tick) * 1000.0;

	if (tick_msec - msPreSend <= 0) {
		*actualTick = 0;
	} else {
		double draft_tick_sec = (tick_msec - msPreSend) / 1000.0;
		*actualTick = (tick_t)::floor(tempoList.tickFromTime(draft_tick_sec));
	}
	*delay = (int)::floor(tick_msec - tempoList.timeFromTick((double) * actualTick) * 1000.0);
}

void VocaloidMidiEventListFactory::_getMsbAndLsb(int value, int* msb, int* lsb)
{
	if (0x3fff < value) {
		*msb = 0x7f;
		*lsb = 0x7f;
	} else {
		*msb = 0xff & (value >> 7);
		*lsb = value - (*msb << 7);
	}
}

void VocaloidMidiEventListFactory::generateNRPNByBPList(
	std::vector<NrpnEvent>& result,
	TempoList const& tempoList, int preSendMilliseconds,
	BPList const& list, NrpnEventProvider const& provider
)
{
	size_t count = list.size();
	int lastDelay = 0;
	for (int i = 0; i < count; i++) {
		tick_t tick = list.keyTickAt(i);
		tick_t actualTick;
		int delay;
		_getActualTickAndDelay(tempoList, tick, preSendMilliseconds, &actualTick, &delay);
		if (actualTick >= 0) {
			NrpnEvent add = provider.getNrpnEvent(actualTick, list.get(i).value);
			if (lastDelay != delay) {
				NrpnEvent delayNrpn = provider.getDelayNrpnEvent(actualTick, delay);
				if (add.hasLSB) {
					delayNrpn.append(add.nrpn, add.dataLSB, add.dataLSB, add.isMSBOmittingRequired);
				} else {
					delayNrpn.append(add.nrpn, add.dataMSB, add.isMSBOmittingRequired);
				}
				result.push_back(delayNrpn);
			} else {
				result.push_back(add);
			}
			lastDelay = delay;
		}
	}
}

VSQ_END_NAMESPACE
