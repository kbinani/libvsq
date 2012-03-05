/**
 * MidiParameterType.h
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
#ifndef __MidiParameterType_h__
#define __MidiParameterType_h__

#include <string>
#include <algorithm>
#include <cctype>
#include "vsqglobal.h"

VSQ_BEGIN_NAMESPACE

class MidiParameterType
{
public:
    /**
     * @brief VOCALOID で使用される NRPN を表す列挙子
     */
    enum MidiParameterTypeEnum
    {
        /**
         * @brief (0x5000) Version number(MSB) &amp;, Device number(LSB)
         */
        CVM_NM_VERSION_AND_DEVICE = 0x5000,
        /**
         * @brief (0x5001) Delay in millisec(MSB, LSB)
         */
        CVM_NM_DELAY = 0x5001,
        /**
         * @brief (0x5002) Note number(MSB)
         */
        CVM_NM_NOTE_NUMBER = 0x5002,
        /**
         * @brief (0x5003) Velocity(MSB)
         */
        CVM_NM_VELOCITY = 0x5003,
        /**
         * @brief (0x5004) Note Duration in millisec(MSB, LSB)
         */
        CVM_NM_NOTE_DURATION = 0x5004,
        /**
         * @brief (0x5005) Note Location(MSB)
         */
        CVM_NM_NOTE_LOCATION = 0x5005,
        /**
         * @brief (0x5006) Attack Type(MSB, LSB)
         */
        CVM_NM_ATTACK_TYPE = 0x5006,
        /**
         * @brief (0x5007) Attack Duration in millisec(MSB, LSB)
         */
        CVM_NM_ATTACK_DURATION = 0x5007,
        /**
         * @brief (0x5008) Attack Depth(MSB)
         */
        CVM_NM_ATTACK_DEPTH = 0x5008,
        /**
         * @brief (0x500c) Index of Vibrato DB(MSB: ID_H00, LSB:ID_L00)
         */
        CVM_NM_INDEX_OF_VIBRATO_DB = 0x500c,
        /**
         * @brief (0x500d) Vibrato configuration(MSB: Index of Vibrato Type, LSB: Duration &amp;, Configuration parameter of vibrato)
         */
        CVM_NM_VIBRATO_CONFIG = 0x500d,
        /**
         * @brief (0x500e) Vibrato Delay(MSB)
         */
        CVM_NM_VIBRATO_DELAY = 0x500e,
        /**
         * @brief (0x5011) Unknonw(MSB), only used in VOCALOID1
         */
        CVM_NM_UNKNOWN1 = 0x5011,
        /**
         * @brief (0x5012) Number of phonetic symbols in bytes(MSB)
         */
        CVM_NM_PHONETIC_SYMBOL_BYTES = 0x5012,
        /**
         * @brief (0x5013) Phonetic symbol 1(MSB:Phonetic symbol 1, LSB: Consonant adjustment 1)
         */
        CVM_NM_PHONETIC_SYMBOL1 = 0x5013,
        /**
         * @brief (0x5014) Phonetic symbol 2(MSB:Phonetic symbol 2, LSB: Consonant adjustment 2)
         */
        CVM_NM_PHONETIC_SYMBOL2 = 0x5014,
        /**
         * @brief (0x5015) Phonetic symbol 3(MSB:Phonetic symbol 3, LSB: Consonant adjustment 3)
         */
        CVM_NM_PHONETIC_SYMBOL3 = 0x5015,
        /**
         * @brief (0x5016) Phonetic symbol 4(MSB:Phonetic symbol 4, LSB: Consonant adjustment 4)
         */
        CVM_NM_PHONETIC_SYMBOL4 = 0x5016,
        /**
         * @brief (0x5017) Phonetic symbol 5(MSB:Phonetic symbol 5, LSB: Consonant adjustment 5)
         */
        CVM_NM_PHONETIC_SYMBOL5 = 0x5017,
        /**
         * @brief (0x5018) Phonetic symbol 6(MSB:Phonetic symbol 6, LSB: Consonant adjustment 6)
         */
        CVM_NM_PHONETIC_SYMBOL6 = 0x5018,
        /**
         * @brief (0x5019) Phonetic symbol 7(MSB:Phonetic symbol 7, LSB: Consonant adjustment 7)
         */
        CVM_NM_PHONETIC_SYMBOL7 = 0x5019,
        /**
         * @brief (0x501a) Phonetic symbol 8(MSB:Phonetic symbol 8, LSB: Consonant adjustment 8)
         */
        CVM_NM_PHONETIC_SYMBOL8 = 0x501a,
        /**
         * @brief (0x501b) Phonetic symbol 9(MSB:Phonetic symbol 9, LSB: Consonant adjustment 9)
         */
        CVM_NM_PHONETIC_SYMBOL9 = 0x501b,
        /**
         * @brief (0x501c) Phonetic symbol 10(MSB:Phonetic symbol 10, LSB: Consonant adjustment 10)
         */
        CVM_NM_PHONETIC_SYMBOL10 = 0x501c,
        /**
         * @brief (0x501d) Phonetic symbol 11(MSB:Phonetic symbol 11, LSB: Consonant adjustment 11)
         */
        CVM_NM_PHONETIC_SYMBOL11 = 0x501d,
        /**
         * @brief (0x501e) Phonetic symbol 12(MSB:Phonetic symbol 12, LSB: Consonant adjustment 12)
         */
        CVM_NM_PHONETIC_SYMBOL12 = 0x501e,
        /**
         * @brief (0x501f) Phonetic symbol 13(MSB:Phonetic symbol 13, LSB: Consonant adjustment 13)
         */
        CVM_NM_PHONETIC_SYMBOL13 = 0x501f,
        /**
         * @brief (0x5020) Phonetic symbol 14(MSB:Phonetic symbol 14, LSB: Consonant adjustment 14)
         */
        CVM_NM_PHONETIC_SYMBOL14 = 0x5020,
        /**
         * @brief (0x5021) Phonetic symbol 15(MSB:Phonetic symbol 15, LSB: Consonant adjustment 15)
         */
        CVM_NM_PHONETIC_SYMBOL15 = 0x5021,
        /**
         * @brief (0x5022) Phonetic symbol 16(MSB:Phonetic symbol 16, LSB: Consonant adjustment 16)
         */
        CVM_NM_PHONETIC_SYMBOL16 = 0x5022,
        /**
         * @brief (0x5023) Phonetic symbol 17(MSB:Phonetic symbol 17, LSB: Consonant adjustment 17)
         */
        CVM_NM_PHONETIC_SYMBOL17 = 0x5023,
        /**
         * @brief (0x5024) Phonetic symbol 18(MSB:Phonetic symbol 18, LSB: Consonant adjustment 18)
         */
        CVM_NM_PHONETIC_SYMBOL18 = 0x5024,
        /**
         * @brief (0x5025) Phonetic symbol 19(MSB:Phonetic symbol 19, LSB: Consonant adjustment 19)
         */
        CVM_NM_PHONETIC_SYMBOL19 = 0x5025,
        /**
         * @brief (0x5026) Phonetic symbol 20(MSB:Phonetic symbol 20, LSB: Consonant adjustment 20)
         */
        CVM_NM_PHONETIC_SYMBOL20 = 0x5026,
        /**
         * @brief (0x5027) Phonetic symbol 21(MSB:Phonetic symbol 21, LSB: Consonant adjustment 21)
         */
        CVM_NM_PHONETIC_SYMBOL21 = 0x5027,
        /**
         * @brief (0x5028) Phonetic symbol 22(MSB:Phonetic symbol 22, LSB: Consonant adjustment 22)
         */
        CVM_NM_PHONETIC_SYMBOL22 = 0x5028,
        /**
         * @brief (0x5029) Phonetic symbol 23(MSB:Phonetic symbol 23, LSB: Consonant adjustment 23)
         */
        CVM_NM_PHONETIC_SYMBOL23 = 0x5029,
        /**
         * @brief (0x502a) Phonetic symbol 24(MSB:Phonetic symbol 24, LSB: Consonant adjustment 24)
         */
        CVM_NM_PHONETIC_SYMBOL24 = 0x502a,
        /**
         * @brief (0x502b) Phonetic symbol 25(MSB:Phonetic symbol 25, LSB: Consonant adjustment 25)
         */
        CVM_NM_PHONETIC_SYMBOL25 = 0x502b,
        /**
         * @brief (0x502c) Phonetic symbol 26(MSB:Phonetic symbol 26, LSB: Consonant adjustment 26)
         */
        CVM_NM_PHONETIC_SYMBOL26 = 0x502c,
        /**
         * @brief (0x502d) Phonetic symbol 27(MSB:Phonetic symbol 27, LSB: Consonant adjustment 27)
         */
        CVM_NM_PHONETIC_SYMBOL27 = 0x502d,
        /**
         * @brief (0x502e) Phonetic symbol 28(MSB:Phonetic symbol 28, LSB: Consonant adjustment 28)
         */
        CVM_NM_PHONETIC_SYMBOL28 = 0x502e,
        /**
         * @brief (0x502f) Phonetic symbol 29(MSB:Phonetic symbol 29, LSB: Consonant adjustment 29)
         */
        CVM_NM_PHONETIC_SYMBOL29 = 0x502f,
        /**
         * @brief (0x5030) Phonetic symbol 30(MSB:Phonetic symbol 30, LSB: Consonant adjustment 30)
         */
        CVM_NM_PHONETIC_SYMBOL30 = 0x5030,
        /**
         * @brief (0x5031) Phonetic symbol 31(MSB:Phonetic symbol 31, LSB: Consonant adjustment 31)
         */
        CVM_NM_PHONETIC_SYMBOL31 = 0x5031,
        /**
         * @brief (0x5032) Phonetic symbol 32(MSB:Phonetic symbol 32, LSB: Consonant adjustment 32)
         */
        CVM_NM_PHONETIC_SYMBOL32 = 0x5032,
        /**
         * @brief (0x5033) Phonetic symbol 33(MSB:Phonetic symbol 33, LSB: Consonant adjustment 33)
         */
        CVM_NM_PHONETIC_SYMBOL33 = 0x5033,
        /**
         * @brief (0x5034) Phonetic symbol 34(MSB:Phonetic symbol 34, LSB: Consonant adjustment 34)
         */
        CVM_NM_PHONETIC_SYMBOL34 = 0x5034,
        /**
         * @brief (0x5035) Phonetic symbol 35(MSB:Phonetic symbol 35, LSB: Consonant adjustment 35)
         */
        CVM_NM_PHONETIC_SYMBOL35 = 0x5035,
        /**
         * @brief (0x5036) Phonetic symbol 36(MSB:Phonetic symbol 36, LSB: Consonant adjustment 36)
         */
        CVM_NM_PHONETIC_SYMBOL36 = 0x5036,
        /**
         * @brief (0x5037) Phonetic symbol 37(MSB:Phonetic symbol 37, LSB: Consonant adjustment 37)
         */
        CVM_NM_PHONETIC_SYMBOL37 = 0x5037,
        /**
         * @brief (0x5038) Phonetic symbol 38(MSB:Phonetic symbol 38, LSB: Consonant adjustment 38)
         */
        CVM_NM_PHONETIC_SYMBOL38 = 0x5038,
        /**
         * @brief (0x5039) Phonetic symbol 39(MSB:Phonetic symbol 39, LSB: Consonant adjustment 39)
         */
        CVM_NM_PHONETIC_SYMBOL39 = 0x5039,
        /**
         * @brief (0x503a) Phonetic symbol 40(MSB:Phonetic symbol 40, LSB: Consonant adjustment 40)
         */
        CVM_NM_PHONETIC_SYMBOL40 = 0x503a,
        /**
         * @brief (0x503b) Phonetic symbol 41(MSB:Phonetic symbol 41, LSB: Consonant adjustment 41)
         */
        CVM_NM_PHONETIC_SYMBOL41 = 0x503b,
        /**
         * @brief (0x503c) Phonetic symbol 42(MSB:Phonetic symbol 42, LSB: Consonant adjustment 42)
         */
        CVM_NM_PHONETIC_SYMBOL42 = 0x503c,
        /**
         * @brief (0x503d) Phonetic symbol 43(MSB:Phonetic symbol 43, LSB: Consonant adjustment 43)
         */
        CVM_NM_PHONETIC_SYMBOL43 = 0x503d,
        /**
         * @brief (0x503e) Phonetic symbol 44(MSB:Phonetic symbol 44, LSB: Consonant adjustment 44)
         */
        CVM_NM_PHONETIC_SYMBOL44 = 0x503e,
        /**
         * @brief (0x503f) Phonetic symbol 45(MSB:Phonetic symbol 45, LSB: Consonant adjustment 45)
         */
        CVM_NM_PHONETIC_SYMBOL45 = 0x503f,
        /**
         * @brief (0x5040) Phonetic symbol 46(MSB:Phonetic symbol 46, LSB: Consonant adjustment 46)
         */
        CVM_NM_PHONETIC_SYMBOL46 = 0x5040,
        /**
         * @brief (0x5041) Phonetic symbol 47(MSB:Phonetic symbol 47, LSB: Consonant adjustment 47)
         */
        CVM_NM_PHONETIC_SYMBOL47 = 0x5041,
        /**
         * @brief (0x5042) Phonetic symbol 48(MSB:Phonetic symbol 48, LSB: Consonant adjustment 48)
         */
        CVM_NM_PHONETIC_SYMBOL48 = 0x5042,
        /**
         * @brief (0x5043) Phonetic symbol 49(MSB:Phonetic symbol 49, LSB: Consonant adjustment 49)
         */
        CVM_NM_PHONETIC_SYMBOL49 = 0x5043,
        /**
         * @brief (0x5044) Phonetic symbol 50(MSB:Phonetic symbol 50, LSB: Consonant adjustment 50)
         */
        CVM_NM_PHONETIC_SYMBOL50 = 0x5044,
        /**
         * @brief (0x5045) Phonetic symbol 51(MSB:Phonetic symbol 51, LSB: Consonant adjustment 51)
         */
        CVM_NM_PHONETIC_SYMBOL51 = 0x5045,
        /**
         * @brief (0x5046) Phonetic symbol 52(MSB:Phonetic symbol 52, LSB: Consonant adjustment 52)
         */
        CVM_NM_PHONETIC_SYMBOL52 = 0x5046,
        /**
         * @brief (0x5047) Phonetic symbol 53(MSB:Phonetic symbol 53, LSB: Consonant adjustment 53)
         */
        CVM_NM_PHONETIC_SYMBOL53 = 0x5047,
        /**
         * @brief (0x5048) Phonetic symbol 54(MSB:Phonetic symbol 54, LSB: Consonant adjustment 54)
         */
        CVM_NM_PHONETIC_SYMBOL54 = 0x5048,
        /**
         * @brief (0x5049) Phonetic symbol 55(MSB:Phonetic symbol 55, LSB: Consonant adjustment 55)
         */
        CVM_NM_PHONETIC_SYMBOL55 = 0x5049,
        /**
         * @brief (0x504a) Phonetic symbol 56(MSB:Phonetic symbol 56, LSB: Consonant adjustment 56)
         */
        CVM_NM_PHONETIC_SYMBOL56 = 0x504a,
        /**
         * @brief (0x504b) Phonetic symbol 57(MSB:Phonetic symbol 57, LSB: Consonant adjustment 57)
         */
        CVM_NM_PHONETIC_SYMBOL57 = 0x504b,
        /**
         * @brief (0x504c) Phonetic symbol 58(MSB:Phonetic symbol 58, LSB: Consonant adjustment 58)
         */
        CVM_NM_PHONETIC_SYMBOL58 = 0x504c,
        /**
         * @brief (0x504d) Phonetic symbol 59(MSB:Phonetic symbol 59, LSB: Consonant adjustment 59)
         */
        CVM_NM_PHONETIC_SYMBOL59 = 0x504d,
        /**
         * @brief (0x504e) Phonetic symbol 60(MSB:Phonetic symbol 60, LSB: Consonant adjustment 60)
         */
        CVM_NM_PHONETIC_SYMBOL60 = 0x504e,
        /**
         * @brief (0x504f) Phonetic symbol continuation(MSB, 0x7f=end, 0x00=continue)
         */
        CVM_NM_PHONETIC_SYMBOL_CONTINUATION = 0x504f,
        /**
         * @brief (0x5050) v1mean in Cent/5(MSB)
         */
        CVM_NM_V1MEAN = 0x5050,
        /**
         * @brief (0x5051) d1mean in millisec/5(MSB)
         */
        CVM_NM_D1MEAN = 0x5051,
        /**
         * @brief (0x5052) d1meanFirstNote in millisec/5(MSB)
         */
        CVM_NM_D1MEAN_FIRST_NOTE = 0x5052,
        /**
         * @brief (0x5053) d2mean in millisec/5(MSB)
         */
        CVM_NM_D2MEAN = 0x5053,
        /**
         * @brief (0x5054) d4mean in millisec/5(MSB)
         */
        CVM_NM_D4MEAN = 0x5054,
        /**
         * @brief (0x5055) pMeanOnsetFirstNote in Cent/5(MSB)
         */
        CVM_NM_PMEAN_ONSET_FIRST_NOTE = 0x5055,
        /**
         * @brief (0x5056) vMeanNoteTransition in Cent/5(MSB)
         */
        CVM_NM_VMEAN_NOTE_TRNSITION = 0x5056,
        /**
         * @brief (0x5057) pMeanEndingNote in Cent/5(MSB)
         */
        CVM_NM_PMEAN_ENDING_NOTE = 0x5057,
        /**
         * @brief (0x5058) AddScooptoUpIntervals &amp;, AddPortamentoToDownIntervals(MSB)
         */
        CVM_NM_ADD_PORTAMENTO = 0x5058,
        /**
         * @brief (0x5059) changAfterPeak(MSB)
         */
        CVM_NM_CHANGE_AFTER_PEAK = 0x5059,
        /**
         * @brief (0x505a) Accent(MSB)
         */
        CVM_NM_ACCENT = 0x505a,
        /**
         * @brief (0x507f) Note message continuation(MSB)
         */
        CVM_NM_NOTE_MESSAGE_CONTINUATION = 0x507f,

        /**
         * @brief (0x5075) Extended Note message; Voice Overlap(MSB, LSB)(VoiceOverlap = ((MSB &amp; 0x7f) &lt;&lt; 7) | (LSB &amp; 0x7f) - 8192)
         */
        CVM_EXNM_VOICE_OVERLAP = 0x5075,
        /**
         * @brief (0x5076) Extended Note message; Flags length in bytes(MSB, LSB)
         */
        CVM_EXNM_FLAGS_BYTES = 0x5076,
        /**
         * @brief (0x5077) Extended Note message; Flag(MSB)
         */
        CVM_EXNM_FLAGS = 0x5077,
        /**
         * @brief (0x5078) Extended Note message; Flag continuation(MSB)(MSB, 0x7f=end, 0x00=continue)
         */
        CVM_EXNM_FLAGS_CONINUATION = 0x5078,
        /**
         * @brief (0x5079) Extended Note message; Moduration(MSB, LSB)(Moduration = ((MSB &amp; 0x7f) &lt;&lt; 7) | (LSB &amp; 0x7f) - 100)
         */
        CVM_EXNM_MODURATION = 0x5079,
        /**
         * @brief (0x507a) Extended Note message; PreUtterance(MSB, LSB)(PreUtterance = ((MSB &amp; 0x7f) &lt;&lt; 7) | (LSB &amp; 0x7f) - 8192)
         */
        CVM_EXNM_PRE_UTTERANCE = 0x507a,
        /**
         * @brief (0x507e) Extended Note message; Envelope: value1(MSB, LSB) actual value = (value3.msb &amp; 0xf) &lt;&lt; 28 | (value2.msb &amp; 0x7f) &lt;&lt; 21 | (value2.lsb &amp; 0x7f) &lt;&lt; 14 | (value1.msb &amp; 0x7f) &lt;&lt; 7 | (value1.lsb &amp; 0x7f)
         */
        CVM_EXNM_ENV_DATA1 = 0x507e,
        /**
         * @brief (0x507d) Extended Note message; Envelope: value2(MSB, LSB)
         */
        CVM_EXNM_ENV_DATA2 = 0x507d,
        /**
         * @brief (0x507c) Extended Note message; Envelope: value3(MSB)
         */
        CVM_EXNM_ENV_DATA3 = 0x507c,
        /**
         * @brief (0x507b) Extended Note message; Envelope: data point continuation(MSB)(MSB, 0x7f=end, 0x00=continue)
         */
        CVM_EXNM_ENV_DATA_CONTINUATION = 0x507b,

        /**
         * @brief (0x6000) Version number &amp;, Device number(MSB, LSB)
         */
        CC_BS_VERSION_AND_DEVICE = 0x6000,
        /**
         * @brief (0x6001) Delay in millisec(MSB, LSB)
         */
        CC_BS_DELAY = 0x6001,
        /**
         * @brief (0x6002) Laugnage type(MSB, optional LSB)
         */
        CC_BS_LANGUAGE_TYPE = 0x6002,

        /**
         * @brief (0x6100) Version number &amp;, device number(MSB, LSB)
         */
        CC_CV_VERSION_AND_DEVICE = 0x6100,
        /**
         * @brief (0x6101) Delay in millisec(MSB, LSB)
         */
        CC_CV_DELAY = 0x6101,
        /**
         * @brief (0x6102) Volume value(MSB)
         */
        CC_CV_VOLUME = 0x6102,

        /**
         * @brief (0x6200) Version number &amp;, device number(MSB, LSB)
         */
        CC_P_VERSION_AND_DEVICE = 0x6200,
        /**
         * @brief (0x6201) Delay in millisec(MSB, LSB)
         */
        CC_P_DELAY = 0x6201,
        /**
         * @brief (0x6202) Pan value(MSB)
         */
        CC_PAN = 0x6202,

        /**
         * @brief (0x6300) Version number &amp;, device number(MSB, LSB)
         */
        CC_E_VESION_AND_DEVICE = 0x6300,
        /**
         * @brief (0x6301) Delay in millisec(MSB, LSB)
         */
        CC_E_DELAY = 0x6301,
        /**
         * @brief (0x6302) Expression vlaue(MSB)
         */
        CC_E_EXPRESSION = 0x6302,

        /**
         * @brief (0x6400) Version number &amp;, device number(MSB, LSB)
         */
        CC_VR_VERSION_AND_DEVICE = 0x6400,
        /**
         * @brief (0x6401) Delay in millisec(MSB, LSB)
         */
        CC_VR_DELAY = 0x6401,
        /**
         * @brief (0x6402) Vibrato Rate value(MSB)
         */
        CC_VR_VIBRATO_RATE = 0x6402,

        /**
         * @brief (0x6500) Version number &amp;, device number(MSB, LSB)
         */
        CC_VD_VERSION_AND_DEVICE = 0x6500,
        /**
         * @brief (0x6501) Delay in millisec(MSB, LSB)
         */
        CC_VD_DELAY = 0x6501,
        /**
         * @brief (0x6502) Vibrato Depth value(MSB)
         */
        CC_VD_VIBRATO_DEPTH = 0x6502,

        /**
         * @brief (0x6600) Version number &amp;, device number(MSB, LSB)
         */
        CC_FX2_VERSION_AND_DEVICE = 0x6600,
        /**
         * @brief (0x6601) Delay in millisec(MSB, LSB)
         */
        CC_FX2_DELAY = 0x6601,
        /**
         * @brief (0x6602) Effect2 Depth(MSB)
         */
        CC_FX2_EFFECT2_DEPTH = 0x6602,

        /**
         * @brief (0x6700) Version number &amp;, device number(MSB, LSB)
         */
        CC_PBS_VERSION_AND_DEVICE = 0x6700,
        /**
         * @brief (0x6701) Delay in millisec(MSB, LSB)
         */
        CC_PBS_DELAY = 0x6701,
        /**
         * @brief (0x6702) Pitch Bend Sensitivity(MSB, LSB)
         */
        CC_PBS_PITCH_BEND_SENSITIVITY = 0x6702,

        /**
         * @brief (0x5300) Version number &amp;, device number(MSB, LSB)
         */
        PC_VERSION_AND_DEVICE = 0x5300,
        /**
         * @brief (0x5301) Delay in millisec(MSB, LSB)
         */
        PC_DELAY = 0x5301,
        /**
         * @brief (0x5302) Voice Type(MSB)
         */
        PC_VOICE_TYPE = 0x5302,

        /**
         * @brief (0x5400) Version number &amp;, device number(MSB, LSB)
         */
        PB_VERSION_AND_DEVICE = 0x5400,
        /**
         * @brief (0x5401) Delay in millisec(MSB, LSB)
         */
        PB_DELAY = 0x5401,
        /**
         * @brief (0x5402) Pitch Bend value(MSB, LSB)
         */
        PB_PITCH_BEND = 0x5402,

        /**
         * @brief (0x5500) Version number &amp;, device number(MSB, LSB)
         */
        VCP_VERSION_AND_DEVICE = 0x5500,
        /**
         * @brief (0x5501) Delay in millisec(MSB, LSB)
         */
        VCP_DELAY = 0x5501,
        /**
         * @brief (0x5502) Voice Change Parameter ID(MSB)
         */
        VCP_VOICE_CHANGE_PARAMETER_ID = 0x5502,
        /**
         * @brief (0x5503) Voice Change Parameter value(MSB)
         */
        VCP_VOICE_CHANGE_PARAMETER = 0x5503
    };

    /**
     * @brief 指定したコントロールに対応する Voice Change Parameter ID の値を取得する
     * @param curveName コントロールカーブの名前
     * @return Voice Change Parameter ID の値
     */
    static int getVoiceChangeParameterId( const std::string &curve_name )
    {
        int lsb = 0x31;
        std::string curveName = curve_name;
        transform( curveName.begin(), curveName.end(), curveName.begin(), tolower );
        if( curveName == "harmonics" ){
            lsb = 0x30;
        }else if( curveName == "bre" ){
            lsb = 0x31;
        }else if( curveName == "bri" ){
            lsb = 0x32;
        }else if( curveName == "cle" ){
            lsb = 0x33;
        }else if( curveName == "por" ){
            lsb = 0x34;
        }else if( curveName == "ope" ){
            lsb = 0x35;
        }else if( curveName == "reso1freq" ){
            lsb = 0x40;
        }else if( curveName == "reso2freq" ){
            lsb = 0x41;
        }else if( curveName == "reso3freq" ){
            lsb = 0x42;
        }else if( curveName == "reso4freq" ){
            lsb = 0x43;
        }else if( curveName == "reso1bw" ){
            lsb = 0x50;
        }else if( curveName == "reso2bw" ){
            lsb = 0x51;
        }else if( curveName == "reso3bw" ){
            lsb = 0x52;
        }else if( curveName == "reso4bw" ){
            lsb = 0x53;
        }else if( curveName == "reso1amp" ){
            lsb = 0x60;
        }else if( curveName == "reso2amp" ){
            lsb = 0x61;
        }else if( curveName == "reso3amp" ){
            lsb = 0x62;
        }else if( curveName == "reso4amp" ){
            lsb = 0x63;
        }else if( curveName == "gen" ){
            lsb = 0x70;
        }
        return lsb;
    }

    /**
     * @brief 指定された NRPN が、DATA LSB が必要なものかどうかを調べる
     * @param nrpn 対象の NRPN
     * @return DATA LSB が必要であれば <code>true</code> を、そうでなければ <code>false</code> を返す
     */
    static bool isDataLsbRequire( int nrpn )
    {
        if( nrpn == MidiParameterType::CVM_NM_VERSION_AND_DEVICE ||
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
            nrpn == MidiParameterType::CVM_EXNM_ENV_DATA2 ){
            return true;
        }else if( nrpn == MidiParameterType::CVM_NM_NOTE_NUMBER ||
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
            nrpn == MidiParameterType::CVM_EXNM_ENV_DATA_CONTINUATION ){
            return false;
        }
        return false;
    }
};

VSQ_END_NAMESPACE

#endif
