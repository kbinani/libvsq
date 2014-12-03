/**
 * MidiParameterType.cpp
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
#include "../include/libvsq/MidiParameterType.hpp"
#include "../include/libvsq/StringUtil.hpp"

VSQ_BEGIN_NAMESPACE

int MidiParameterTypeUtil::getVoiceChangeParameterId(std::string const& curve_name)
{
	int lsb = 0x31;
	std::string curveName = StringUtil::toLower(curve_name);
	if (curveName == "harmonics") {
		lsb = 0x30;
	} else if (curveName == "bre") {
		lsb = 0x31;
	} else if (curveName == "bri") {
		lsb = 0x32;
	} else if (curveName == "cle") {
		lsb = 0x33;
	} else if (curveName == "por") {
		lsb = 0x34;
	} else if (curveName == "ope") {
		lsb = 0x35;
	} else if (curveName == "reso1freq") {
		lsb = 0x40;
	} else if (curveName == "reso2freq") {
		lsb = 0x41;
	} else if (curveName == "reso3freq") {
		lsb = 0x42;
	} else if (curveName == "reso4freq") {
		lsb = 0x43;
	} else if (curveName == "reso1bw") {
		lsb = 0x50;
	} else if (curveName == "reso2bw") {
		lsb = 0x51;
	} else if (curveName == "reso3bw") {
		lsb = 0x52;
	} else if (curveName == "reso4bw") {
		lsb = 0x53;
	} else if (curveName == "reso1amp") {
		lsb = 0x60;
	} else if (curveName == "reso2amp") {
		lsb = 0x61;
	} else if (curveName == "reso3amp") {
		lsb = 0x62;
	} else if (curveName == "reso4amp") {
		lsb = 0x63;
	} else if (curveName == "gen") {
		lsb = 0x70;
	}
	return lsb;
}

bool MidiParameterTypeUtil::isDataLsbRequire(MidiParameterType nrpn)
{
	if (nrpn == MidiParameterType::CVM_NM_VERSION_AND_DEVICE ||
		nrpn == MidiParameterType::CVM_NM_DELAY ||
		nrpn == MidiParameterType::CVM_NM_NOTE_DURATION ||
		nrpn == MidiParameterType::CVM_NM_INDEX_OF_VIBRATO_DB ||
		nrpn == MidiParameterType::CVM_NM_VIBRATO_CONFIG ||
		nrpn == MidiParameterType::CVM_NM_PHONETIC_SYMBOL1 ||
		nrpn == MidiParameterType::CC_BS_VERSION_AND_DEVICE ||
		nrpn == MidiParameterType::CC_BS_DELAY ||
		nrpn == MidiParameterType::CC_BS_LANGUAGE_TYPE ||
		nrpn == MidiParameterType::CC_CV_VERSION_AND_DEVICE ||
		nrpn == MidiParameterType::CC_CV_DELAY ||
		nrpn == MidiParameterType::CC_P_VERSION_AND_DEVICE ||
		nrpn == MidiParameterType::CC_P_DELAY ||
		nrpn == MidiParameterType::CC_E_VESION_AND_DEVICE ||
		nrpn == MidiParameterType::CC_E_DELAY ||
		nrpn == MidiParameterType::CC_VR_VERSION_AND_DEVICE ||
		nrpn == MidiParameterType::CC_VR_DELAY ||
		nrpn == MidiParameterType::CC_VD_VERSION_AND_DEVICE ||
		nrpn == MidiParameterType::CC_VD_DELAY ||
		nrpn == MidiParameterType::CC_FX2_VERSION_AND_DEVICE ||
		nrpn == MidiParameterType::CC_FX2_DELAY ||
		nrpn == MidiParameterType::CC_PBS_VERSION_AND_DEVICE ||
		nrpn == MidiParameterType::CC_PBS_DELAY ||
		nrpn == MidiParameterType::CC_PBS_PITCH_BEND_SENSITIVITY ||
		nrpn == MidiParameterType::PC_VERSION_AND_DEVICE ||
		nrpn == MidiParameterType::PC_DELAY ||
		nrpn == MidiParameterType::PB_VERSION_AND_DEVICE ||
		nrpn == MidiParameterType::PB_DELAY ||
		nrpn == MidiParameterType::PB_PITCH_BEND ||
		nrpn == MidiParameterType::VCP_VERSION_AND_DEVICE ||
		nrpn == MidiParameterType::VCP_DELAY ||
		nrpn == MidiParameterType::CVM_EXNM_ENV_DATA1 ||
		nrpn == MidiParameterType::CVM_EXNM_ENV_DATA2) {
		return true;
	} else if (nrpn == MidiParameterType::CVM_NM_NOTE_NUMBER ||
			   nrpn == MidiParameterType::CVM_NM_VELOCITY ||
			   nrpn == MidiParameterType::CVM_NM_NOTE_LOCATION ||
			   nrpn == MidiParameterType::CVM_NM_VIBRATO_DELAY ||
			   // case MidiParameterEnum.CVM_NM_UNKNOWN1 ||
			   nrpn == MidiParameterType::CVM_NM_PHONETIC_SYMBOL_BYTES ||
			   nrpn == MidiParameterType::CVM_NM_PHONETIC_SYMBOL_CONTINUATION ||
			   nrpn == MidiParameterType::CVM_NM_V1MEAN ||
			   nrpn == MidiParameterType::CVM_NM_D1MEAN ||
			   nrpn == MidiParameterType::CVM_NM_D1MEAN_FIRST_NOTE ||
			   nrpn == MidiParameterType::CVM_NM_D2MEAN ||
			   nrpn == MidiParameterType::CVM_NM_D4MEAN ||
			   nrpn == MidiParameterType::CVM_NM_PMEAN_ONSET_FIRST_NOTE ||
			   nrpn == MidiParameterType::CVM_NM_VMEAN_NOTE_TRNSITION ||
			   nrpn == MidiParameterType::CVM_NM_PMEAN_ENDING_NOTE ||
			   nrpn == MidiParameterType::CVM_NM_ADD_PORTAMENTO ||
			   nrpn == MidiParameterType::CVM_NM_CHANGE_AFTER_PEAK ||
			   nrpn == MidiParameterType::CVM_NM_ACCENT ||
			   nrpn == MidiParameterType::CVM_NM_NOTE_MESSAGE_CONTINUATION ||
			   nrpn == MidiParameterType::CC_CV_VOLUME ||
			   nrpn == MidiParameterType::CC_PAN ||
			   nrpn == MidiParameterType::CC_E_EXPRESSION ||
			   nrpn == MidiParameterType::CC_VR_VIBRATO_RATE ||
			   nrpn == MidiParameterType::CC_VD_VIBRATO_DEPTH ||
			   nrpn == MidiParameterType::CC_FX2_EFFECT2_DEPTH ||
			   nrpn == MidiParameterType::PC_VOICE_TYPE ||
			   nrpn == MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID ||
			   nrpn == MidiParameterType::VCP_VOICE_CHANGE_PARAMETER ||
			   nrpn == MidiParameterType::CVM_EXNM_ENV_DATA3 ||
			   nrpn == MidiParameterType::CVM_EXNM_ENV_DATA_CONTINUATION) {
		return false;
	}
	return false;
}

VSQ_END_NAMESPACE
