/**
 * NrpnEvent.cpp
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
#include "./NrpnEvent.hpp"

VSQ_BEGIN_NAMESPACE

NrpnEvent::NrpnEvent(tick_t clock, MidiParameterType::MidiParameterTypeEnum nrpn, int dataMsb)
{
	this->clock = clock;
	this->nrpn = nrpn;
	this->dataMSB = dataMsb;
	this->dataLSB = 0x0;
	this->hasLSB = false;
	this->isMSBOmittingRequired = false;
}

NrpnEvent::NrpnEvent(tick_t clock, MidiParameterType::MidiParameterTypeEnum nrpn, int dataMsb, int dataLsb)
{
	this->clock = clock;
	this->nrpn = nrpn;
	this->dataMSB = dataMsb;
	this->dataLSB = dataLsb;
	this->hasLSB = true;
	this->isMSBOmittingRequired = false;
}

std::vector<NrpnEvent> NrpnEvent::expand()
{
	std::vector<NrpnEvent> ret;
	if (hasLSB) {
		NrpnEvent v(clock, nrpn, dataMSB, dataLSB);
		v.isMSBOmittingRequired = isMSBOmittingRequired;
		ret.push_back(v);
	} else {
		NrpnEvent v(clock, nrpn, dataMSB);
		v.isMSBOmittingRequired = isMSBOmittingRequired;
		ret.push_back(v);
	}
	for (int i = 0; i < _list.size(); i++) {
		std::vector<NrpnEvent> add = _list[i].expand();
		for (int j = 0; j < add.size(); j++) {
			ret.push_back(add[j]);
		}
	}
	return ret;
}

int NrpnEvent::compareTo(const NrpnEvent& item) const
{
	if (clock == item.clock) {
		int thisNrpnMsb = (nrpn - (nrpn % 0x100)) / 0x100;
		int itemNrpnMsb = (item.nrpn - (item.nrpn % 0x100)) / 0x100;
		return itemNrpnMsb - thisNrpnMsb;
	} else {
		return clock - item.clock;
	}
}

void NrpnEvent::append(MidiParameterType::MidiParameterTypeEnum nrpn, int dataMsb)
{
	_list.push_back(NrpnEvent(clock, nrpn, dataMsb));
}

void NrpnEvent::append(MidiParameterType::MidiParameterTypeEnum nrpn, int dataMsb, int dataLsb)
{
	_list.push_back(NrpnEvent(clock, nrpn, dataMsb, dataLsb));
}

void NrpnEvent::append(MidiParameterType::MidiParameterTypeEnum nrpn, int dataMsb, bool isMsbOmittingRequired)
{
	NrpnEvent v(this->clock, nrpn, dataMsb);
	v.isMSBOmittingRequired = isMsbOmittingRequired;
	_list.push_back(v);
}

void NrpnEvent::append(MidiParameterType::MidiParameterTypeEnum nrpn, int dataMsb, int dataLsb, bool isMsbOmittingRequired)
{
	NrpnEvent v(this->clock, nrpn, dataMsb, dataLsb);
	v.isMSBOmittingRequired = isMsbOmittingRequired;
	_list.push_back(v);
}

bool NrpnEvent::compare(const NrpnEvent& a, const NrpnEvent& b)
{
	if (a.compareTo(b) < 0) {
		return true;
	} else {
		return false;
	}
}

std::vector<MidiEvent> NrpnEvent::convert(const std::vector<NrpnEvent>& source)
{
	int nrpn = source[0].nrpn;
	int msb = 0xff & (nrpn >> 8);
	int lsb = nrpn - (0xff00 & (msb << 8));
	std::vector<MidiEvent> ret;
	MidiEvent e;

	e = MidiEvent();
	e.clock = source[0].clock;
	e.firstByte = 0xb0;
	e.data.push_back(0x63);
	e.data.push_back(msb);
	ret.push_back(e);

	e = MidiEvent();
	e.clock = source[0].clock;
	e.firstByte = 0xb0;
	e.data.push_back(0x62);
	e.data.push_back(lsb);
	ret.push_back(e);

	e = MidiEvent();
	e.clock = source[0].clock;
	e.firstByte = 0xb0;
	e.data.push_back(0x06);
	e.data.push_back(source[0].dataMSB);
	ret.push_back(e);

	if (source[0].hasLSB) {
		e = MidiEvent();
		e.clock = source[0].clock;
		e.firstByte = 0xb0;
		e.data.push_back(0x26);
		e.data.push_back(source[0].dataLSB);
		ret.push_back(e);
	}

	for (int i = 1; i < source.size(); i++) {
		NrpnEvent item = source[i];
		int tnrpn = item.nrpn;
		msb = 0xff & (tnrpn >> 8);
		lsb = tnrpn - (0xff00 & (msb << 8));
		if (false == item.isMSBOmittingRequired) {
			e = MidiEvent();
			e.clock = item.clock;
			e.firstByte = 0xb0;
			e.data.push_back(0x63);
			e.data.push_back(msb);
			ret.push_back(e);
		}

		e = MidiEvent();
		e.clock = item.clock;
		e.firstByte = 0xb0;
		e.data.push_back(0x62);
		e.data.push_back(lsb);
		ret.push_back(e);

		e = MidiEvent();
		e.clock = item.clock;
		e.firstByte = 0xb0;
		e.data.push_back(0x06);
		e.data.push_back(item.dataMSB);
		ret.push_back(e);
		if (item.hasLSB) {
			e = MidiEvent();
			e.clock = item.clock;
			e.firstByte = 0xb0;
			e.data.push_back(0x26);
			e.data.push_back(item.dataLSB);
			ret.push_back(e);
		}
	}
	return ret;
}

NrpnEvent::NrpnEvent()
{
	this->clock = 0;
	this->dataLSB = 0;
	this->dataMSB = 0;
	this->hasLSB = false;
	this->isMSBOmittingRequired = false;
	this->nrpn = (MidiParameterType::MidiParameterTypeEnum)0;
}

VSQ_END_NAMESPACE
