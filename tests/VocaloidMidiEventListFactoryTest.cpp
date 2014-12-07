#include "Util.hpp"
#include "../include/libvsq/VocaloidMidiEventListFactory.hpp"
#include "../include/libvsq/Sequence.hpp"

#include <iostream> // debug

using namespace std;
using namespace vsq;

struct NrpnEventSpec {
	tick_t tick;// = 0;
	MidiParameterType nrpn;// = 0;
	int dataMSB;// = 0;
	bool hasLSB;// = false;
	int dataLSB;// = 0;
	bool isMSBOmittingRequired;// = false;
};

TEST(VocaloidMidiEventListFactoryTest, test_generateExpressionNRPN)
{
	Sequence sequence("Miku", 1, 4, 4, 500000);
	Track& track = sequence.track(0);
	BPList* dynamics = track.curve("DYN");
	dynamics->add(480, 127);
	dynamics->add(1920, 0);

	vector<NrpnEvent> actual = VocaloidMidiEventListFactory::generateExpressionNRPN(track, sequence.tempoList, 500);
	EXPECT_EQ((size_t)2, actual.size());

	vector<NrpnEvent> expandedActual = actual[0].expand();
	EXPECT_EQ((tick_t)0, expandedActual[0].tick);
	EXPECT_EQ(MidiParameterType::CC_E_DELAY, expandedActual[0].nrpn);
	EXPECT_EQ(0x03, expandedActual[0].dataMSB);
	EXPECT_EQ(0x74, expandedActual[0].dataLSB);
	EXPECT_TRUE(expandedActual[0].hasLSB);
	EXPECT_EQ(false, expandedActual[0].isMSBOmittingRequired);

	EXPECT_EQ((tick_t)0, expandedActual[1].tick);
	EXPECT_EQ(MidiParameterType::CC_E_EXPRESSION, expandedActual[1].nrpn);
	EXPECT_EQ(127, expandedActual[1].dataMSB);
	EXPECT_EQ(false, expandedActual[1].hasLSB);
	EXPECT_EQ(false, expandedActual[1].isMSBOmittingRequired);

	EXPECT_EQ((tick_t)1440, actual[1].tick);
	EXPECT_EQ(MidiParameterType::CC_E_EXPRESSION, actual[1].nrpn);
	EXPECT_EQ(0, actual[1].dataMSB);
	EXPECT_EQ(false, actual[1].hasLSB);
	EXPECT_EQ(false, actual[1].isMSBOmittingRequired);
}

TEST(VocaloidMidiEventListFactoryTest, test_generateHeaderNRPN)
{
	vector<NrpnEvent> actual = VocaloidMidiEventListFactory::generateHeaderNRPN().expand();
	EXPECT_EQ((size_t)3, actual.size());

	EXPECT_EQ((tick_t)0, actual[0].tick);
	EXPECT_EQ(MidiParameterType::CC_BS_VERSION_AND_DEVICE, actual[0].nrpn);
	EXPECT_EQ(0x00, actual[0].dataMSB);
	EXPECT_EQ(0x00, actual[0].dataLSB);
	EXPECT_TRUE(actual[0].hasLSB);
	EXPECT_EQ(false, actual[0].isMSBOmittingRequired);

	EXPECT_EQ((tick_t)0, actual[1].tick);
	EXPECT_EQ(MidiParameterType::CC_BS_DELAY, actual[1].nrpn);
	EXPECT_EQ(0x00, actual[1].dataMSB);
	EXPECT_EQ(0x00, actual[1].dataLSB);
	EXPECT_TRUE(actual[1].hasLSB);
	EXPECT_EQ(false, actual[1].isMSBOmittingRequired);

	EXPECT_EQ((tick_t)0, actual[2].tick);
	EXPECT_EQ(MidiParameterType::CC_BS_LANGUAGE_TYPE, actual[2].nrpn);
	EXPECT_EQ(0x00, actual[2].dataMSB);
	EXPECT_EQ(false, actual[2].hasLSB);
	EXPECT_EQ(false, actual[2].isMSBOmittingRequired);
}

TEST(VocaloidMidiEventListFactoryTest, testGenerateSingerNRPN)
{
	Sequence sequence("Miku", 1, 4, 4, 500000);
	Event singerEvent(1920, EventType::SINGER);
	singerEvent.singerHandle = Handle(HandleType::SINGER);
	sequence.track(0).events().add(singerEvent);
	vector<NrpnEvent> actual = VocaloidMidiEventListFactory::generateSingerNRPN(sequence.tempoList, singerEvent, 500);

	EXPECT_EQ((size_t)1, actual.size());
	vector<NrpnEvent> actualExpanded = actual[0].expand();

	EXPECT_EQ((size_t)4, actualExpanded.size());
	NrpnEvent item = actualExpanded[0];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CC_BS_VERSION_AND_DEVICE, item.nrpn);
	EXPECT_EQ(0x00, item.dataMSB);
	EXPECT_EQ(0x00, item.dataLSB);
	EXPECT_TRUE(item.hasLSB);
	EXPECT_EQ(false, item.isMSBOmittingRequired);

	item = actualExpanded[1];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CC_BS_DELAY, item.nrpn);
	EXPECT_EQ(0x03, item.dataMSB);
	EXPECT_EQ(0x74, item.dataLSB);
	EXPECT_TRUE(item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);

	item = actualExpanded[2];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CC_BS_LANGUAGE_TYPE, item.nrpn);
	EXPECT_EQ(singerEvent.singerHandle.language, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);

	item = actualExpanded[3];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::PC_VOICE_TYPE, item.nrpn);
	EXPECT_EQ(singerEvent.singerHandle.program, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_EQ(false, item.isMSBOmittingRequired);
}

TEST(VocaloidMidiEventListFactoryTest, testGenerateNoteNRPN)
{
	Sequence sequence("Miku", 1, 4, 4, 500000);
	Event noteEvent(1920, EventType::NOTE);
	noteEvent.length(480);
	noteEvent.note = 60;
	noteEvent.dynamics = 127;
	noteEvent.pmBendDepth = 8;
	noteEvent.pmBendLength = 0;
	noteEvent.d4mean = 63;
	noteEvent.pMeanOnsetFirstNote = 65;
	noteEvent.vMeanNoteTransition = 66;
	noteEvent.pMeanEndingNote = 67;
	noteEvent.pmbPortamentoUse = 3;
	noteEvent.demDecGainRate = 50;
	noteEvent.demAccent = 50;
	noteEvent.vibratoDelay = 240;
	noteEvent.vibratoHandle = Handle(HandleType::VIBRATO);
	noteEvent.vibratoHandle.length(240);
	noteEvent.vibratoHandle.iconId = "$04040005";
	noteEvent.lyricHandle = Handle(HandleType::LYRIC);
	noteEvent.lyricHandle.set(0, Lyric("あ", "a"));
	sequence.track(0).common().version = "DSB3";

	// lastDelay が nil であるために, CVM_NM_VERSION_AND_DEVICE が出力されるケース
	int lastDelay;// = nil;
	int noteLocation = 1;
	int msPreSend = 500;
	int track = 0;
	int delay;
	NrpnEvent actual = VocaloidMidiEventListFactory::generateNoteNRPN(sequence.track(0), sequence.tempoList, noteEvent, msPreSend, noteLocation, (int*)0, &delay);
	vector<NrpnEvent> actualExpanded = actual.expand();
	EXPECT_EQ((size_t)24, actualExpanded.size());
	EXPECT_EQ(500, delay);
	NrpnEvent item = actualExpanded[0];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_VERSION_AND_DEVICE, item.nrpn);
	EXPECT_EQ(0x00, item.dataMSB);
	EXPECT_EQ(0x00, item.dataLSB);
	EXPECT_TRUE(item.hasLSB);
	EXPECT_EQ(false, item.isMSBOmittingRequired);
	item = actualExpanded[1];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_DELAY, item.nrpn);
	EXPECT_EQ(0x03, item.dataMSB);
	EXPECT_EQ(0x74, item.dataLSB);
	EXPECT_TRUE(item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[2];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_NOTE_NUMBER, item.nrpn);
	EXPECT_EQ(60, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[3];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_VELOCITY, item.nrpn);
	EXPECT_EQ(127, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[4];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_NOTE_DURATION, item.nrpn);
	EXPECT_EQ(0x03, item.dataMSB);
	EXPECT_EQ(0x74, item.dataLSB);
	EXPECT_TRUE(item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[5];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_NOTE_LOCATION, item.nrpn);
	EXPECT_EQ(1, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[6];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_INDEX_OF_VIBRATO_DB, item.nrpn);
	EXPECT_EQ(0x00, item.dataMSB);
	EXPECT_EQ(0x00, item.dataLSB);
	EXPECT_TRUE(item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[7];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_VIBRATO_CONFIG, item.nrpn);
	EXPECT_EQ(5, item.dataMSB);
	EXPECT_EQ(63, item.dataLSB);
	EXPECT_TRUE(item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[8];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_VIBRATO_DELAY, item.nrpn);
	EXPECT_EQ(64, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[9];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_PHONETIC_SYMBOL_BYTES, item.nrpn);
	EXPECT_EQ(1, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[10];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ((MidiParameterType)0x5013, item.nrpn);
	EXPECT_EQ((int)'a', item.dataMSB);
	EXPECT_EQ(0, item.dataLSB);
	EXPECT_TRUE(item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[11];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_PHONETIC_SYMBOL_CONTINUATION, item.nrpn);
	EXPECT_EQ(0x7f, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[12];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_V1MEAN, item.nrpn);
	EXPECT_EQ(4, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[13];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_D1MEAN, item.nrpn);
	EXPECT_EQ(8, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[14];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_D1MEAN_FIRST_NOTE, item.nrpn);
	EXPECT_EQ(0x14, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[15];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_D2MEAN, item.nrpn);
	EXPECT_EQ(28, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[16];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_D4MEAN, item.nrpn);
	EXPECT_EQ(63, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[17];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_PMEAN_ONSET_FIRST_NOTE, item.nrpn);
	EXPECT_EQ(65, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[18];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_VMEAN_NOTE_TRNSITION, item.nrpn);
	EXPECT_EQ(66, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[19];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_PMEAN_ENDING_NOTE, item.nrpn);
	EXPECT_EQ(67, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[20];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_ADD_PORTAMENTO, item.nrpn);
	EXPECT_EQ(3, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[21];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_CHANGE_AFTER_PEAK, item.nrpn);
	EXPECT_EQ(50, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[22];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_ACCENT, item.nrpn);
	EXPECT_EQ(50, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
	item = actualExpanded[23];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_NOTE_MESSAGE_CONTINUATION, item.nrpn);
	EXPECT_EQ(0x7f, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);

	// lastDelay が nil でないために, CVM_NM_VERSION_AND_DEVICE が出力されないパターン
	lastDelay = 0;
	actual = VocaloidMidiEventListFactory::generateNoteNRPN(sequence.track(track), sequence.tempoList, noteEvent, msPreSend, noteLocation, &lastDelay, &delay);
	actualExpanded = actual.expand();
	EXPECT_EQ((size_t)23, actualExpanded.size());
	EXPECT_EQ(500, delay);

	// lastDelay が, 該当音符についての delay と同じであるために, CVM_NM_DELAY が出力されないパターン
	lastDelay = 500;
	actual = VocaloidMidiEventListFactory::generateNoteNRPN(sequence.track(track), sequence.tempoList, noteEvent, msPreSend, noteLocation, &lastDelay, &delay);
	actualExpanded = actual.expand();
	EXPECT_EQ((size_t)22, actualExpanded.size());
	EXPECT_EQ(500, delay);

	// vibratoHandle が nil であるために, CVM_NM_INDEX_OF_VIBRATO_DB, CVM_NM_VIBRATO_CONFIG, CVM_NM_VIBRATO_DELAY
	// が出力されないパターン
	lastDelay = 500;
	noteEvent.vibratoHandle = Handle();
	actual = VocaloidMidiEventListFactory::generateNoteNRPN(sequence.track(track), sequence.tempoList, noteEvent, msPreSend, noteLocation, &lastDelay, &delay);
	actualExpanded = actual.expand();
	EXPECT_EQ((size_t)19, actualExpanded.size());
	EXPECT_EQ(500, delay);

	sequence.track(0).common().version = "DSB2";
	// VOCALOID1 であるために, 0x5011が出力され, CVM_NM_PHONETIC_SYMBOL_CONTINUATIONとVOCALOID2用のNRPNが出力されない
	lastDelay = 500;
	noteEvent.vibratoHandle = Handle();
	actual = VocaloidMidiEventListFactory::generateNoteNRPN(sequence.track(track), sequence.tempoList, noteEvent, msPreSend, noteLocation, &lastDelay, &delay);
	actualExpanded = actual.expand();
	EXPECT_EQ((size_t)8, actualExpanded.size());
	item = actualExpanded[4];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ((MidiParameterType)0x5011, item.nrpn);
	EXPECT_EQ(0x01, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_TRUE(item.isMSBOmittingRequired);
}

TEST(VocaloidMidiEventListFactoryTest, testGeneratePitchBendNRPN)
{
	Sequence sequence("Miku", 1, 4, 4, 500000);
	BPList* pit = sequence.track(0).curve("PIT");
	pit->add(480, 8191);
	pit->add(1920, -8192);

	vector<NrpnEvent> actual = VocaloidMidiEventListFactory::generatePitchBendNRPN(sequence.track(0), sequence.tempoList, 500);
	EXPECT_EQ((size_t)2, actual.size());

	vector<NrpnEvent> expandedActual = actual[0].expand();
	EXPECT_EQ((tick_t)0, expandedActual[0].tick);
	EXPECT_EQ(MidiParameterType::PB_DELAY, expandedActual[0].nrpn);
	EXPECT_EQ(0x03, expandedActual[0].dataMSB);
	EXPECT_EQ(0x74, expandedActual[0].dataLSB);
	EXPECT_TRUE(expandedActual[0].hasLSB);
	EXPECT_EQ(false, expandedActual[0].isMSBOmittingRequired);

	EXPECT_EQ((tick_t)0, expandedActual[1].tick);
	EXPECT_EQ(MidiParameterType::PB_PITCH_BEND, expandedActual[1].nrpn);
	EXPECT_EQ(0x7F, expandedActual[1].dataMSB);
	EXPECT_EQ(0x7F, expandedActual[1].dataLSB);
	EXPECT_TRUE(expandedActual[1].hasLSB);
	EXPECT_EQ(false, expandedActual[1].isMSBOmittingRequired);

	EXPECT_EQ((tick_t)1440, actual[1].tick);
	EXPECT_EQ(MidiParameterType::PB_PITCH_BEND, actual[1].nrpn);
	EXPECT_EQ(0x00, actual[1].dataMSB);
	EXPECT_EQ(0x00, actual[1].dataLSB);
	EXPECT_TRUE(actual[1].hasLSB);
	EXPECT_EQ(false, actual[1].isMSBOmittingRequired);
}

TEST(VocaloidMidiEventListFactoryTest, testGeneratePitchBendSensitivityNRPN)
{
	Sequence sequence("Miku", 1, 4, 4, 500000);
	BPList* pbs = sequence.track(0).curve("PBS");
	pbs->add(480, 0);
	pbs->add(1920, 24);

	vector<NrpnEvent> actual = VocaloidMidiEventListFactory::generatePitchBendSensitivityNRPN(sequence.track(0), sequence.tempoList, 500);
	EXPECT_EQ((size_t)2, actual.size());

	vector<NrpnEvent> expandedActual = actual[0].expand();
	EXPECT_EQ((tick_t)0, expandedActual[0].tick);
	EXPECT_EQ(MidiParameterType::CC_PBS_DELAY, expandedActual[0].nrpn);
	EXPECT_EQ(0x03, expandedActual[0].dataMSB);
	EXPECT_EQ(0x74, expandedActual[0].dataLSB);
	EXPECT_TRUE(expandedActual[0].hasLSB);
	EXPECT_EQ(false, expandedActual[0].isMSBOmittingRequired);

	EXPECT_EQ((tick_t)0, expandedActual[1].tick);
	EXPECT_EQ(MidiParameterType::CC_PBS_PITCH_BEND_SENSITIVITY, expandedActual[1].nrpn);
	EXPECT_EQ(0, expandedActual[1].dataMSB);
	EXPECT_EQ(0x00, expandedActual[1].dataLSB);
	EXPECT_TRUE(expandedActual[1].hasLSB);
	EXPECT_EQ(false, expandedActual[1].isMSBOmittingRequired);

	EXPECT_EQ((tick_t)1440, actual[1].tick);
	EXPECT_EQ(MidiParameterType::CC_PBS_PITCH_BEND_SENSITIVITY, actual[1].nrpn);
	EXPECT_EQ(24, actual[1].dataMSB);
	EXPECT_EQ(0x00, actual[1].dataLSB);
	EXPECT_TRUE(actual[1].hasLSB);
	EXPECT_EQ(false, actual[1].isMSBOmittingRequired);
}

TEST(VocaloidMidiEventListFactoryTest, testGenerateVibratoNRPN)
{
	Sequence sequence("Miku", 1, 4, 4, 500000);
	Event noteEvent(480, EventType::NOTE);
	noteEvent.vibratoHandle = Handle();

	// ビブラートがないため, NRPN が生成されない場合
	vector<NrpnEvent> actual = VocaloidMidiEventListFactory::generateVibratoNRPN(sequence.tempoList, noteEvent, 500);
	EXPECT_EQ((size_t)0, actual.size());

	// ビブラートがある場合
	noteEvent.vibratoHandle = Handle(HandleType::VIBRATO);
	noteEvent.length(480);
	noteEvent.vibratoDelay = 240;
	noteEvent.vibratoHandle.length(240);
	noteEvent.vibratoHandle.startDepth = 71;
	noteEvent.vibratoHandle.startRate = 72;
	VibratoBPList rateCurve("2", "0.5,1.0", "11,12");
	VibratoBPList depthCurve("2", "0.4,0.9", "13,14");
	noteEvent.vibratoHandle.rateBP = rateCurve;
	noteEvent.vibratoHandle.depthBP = depthCurve;
	actual = VocaloidMidiEventListFactory::generateVibratoNRPN(sequence.tempoList, noteEvent, 500);
	EXPECT_EQ((size_t)5, actual.size());

	vector<NrpnEvent> actualExpanded = actual[0].expand();
	EXPECT_EQ((size_t)6, actualExpanded.size());
	NrpnEvent item = actualExpanded[0];
	EXPECT_EQ((tick_t)240, item.tick);
	EXPECT_EQ(MidiParameterType::CC_VD_VERSION_AND_DEVICE, item.nrpn);
	EXPECT_EQ(0x00, item.dataMSB);
	EXPECT_EQ(0x00, item.dataLSB);
	EXPECT_TRUE(item.hasLSB);
	EXPECT_EQ(false, item.isMSBOmittingRequired);
	item = actualExpanded[1];
	EXPECT_EQ((tick_t)240, item.tick);
	EXPECT_EQ(MidiParameterType::CC_VR_VERSION_AND_DEVICE, item.nrpn);
	EXPECT_EQ(0x00, item.dataMSB);
	EXPECT_EQ(0x00, item.dataLSB);
	EXPECT_TRUE(item.hasLSB);
	EXPECT_EQ(false, item.isMSBOmittingRequired);
	item = actualExpanded[2];
	EXPECT_EQ((tick_t)240, item.tick);
	EXPECT_EQ(MidiParameterType::CC_VD_DELAY, item.nrpn);
	EXPECT_EQ(0x03, item.dataMSB);
	EXPECT_EQ(0x74, item.dataLSB);
	EXPECT_TRUE(item.hasLSB);
	EXPECT_EQ(false, item.isMSBOmittingRequired);
	item = actualExpanded[3];
	EXPECT_EQ((tick_t)240, item.tick);
	EXPECT_EQ(MidiParameterType::CC_VR_DELAY, item.nrpn);
	EXPECT_EQ(0x03, item.dataMSB);
	EXPECT_EQ(0x74, item.dataLSB);
	EXPECT_TRUE(item.hasLSB);
	EXPECT_EQ(false, item.isMSBOmittingRequired);
	item = actualExpanded[4];
	EXPECT_EQ((tick_t)240, item.tick);
	EXPECT_EQ(MidiParameterType::CC_VD_VIBRATO_DEPTH, item.nrpn);
	EXPECT_EQ(71, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_EQ(false, item.isMSBOmittingRequired);
	item = actualExpanded[5];
	EXPECT_EQ((tick_t)240, item.tick);
	EXPECT_EQ(MidiParameterType::CC_VR_VIBRATO_RATE, item.nrpn);
	EXPECT_EQ(72, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_EQ(false, item.isMSBOmittingRequired);

	actualExpanded = actual[1].expand();
	EXPECT_EQ((size_t)2, actualExpanded.size());
	item = actualExpanded[0];
	EXPECT_EQ((tick_t)336, item.tick);
	EXPECT_EQ(MidiParameterType::CC_VD_DELAY, item.nrpn);
	EXPECT_EQ(0x03, item.dataMSB);
	EXPECT_EQ(0x74, item.dataLSB);
	EXPECT_TRUE(item.hasLSB);
	EXPECT_EQ(false, item.isMSBOmittingRequired);
	item = actualExpanded[1];
	EXPECT_EQ((tick_t)336, item.tick);
	EXPECT_EQ(MidiParameterType::CC_VD_VIBRATO_DEPTH, item.nrpn);
	EXPECT_EQ(13, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_EQ(false, item.isMSBOmittingRequired);

	item = actual[2];
	actualExpanded = item.expand();
	EXPECT_EQ((size_t)2, actualExpanded.size());
	item = actualExpanded[0];
	EXPECT_EQ((tick_t)360, item.tick);
	EXPECT_EQ(MidiParameterType::CC_VR_DELAY, item.nrpn);
	EXPECT_EQ(0x03, item.dataMSB);
	EXPECT_EQ(0x74, item.dataLSB);
	EXPECT_TRUE(item.hasLSB);
	EXPECT_EQ(false, item.isMSBOmittingRequired);
	item = actualExpanded[1];
	EXPECT_EQ((tick_t)360, item.tick);
	EXPECT_EQ(MidiParameterType::CC_VR_VIBRATO_RATE, item.nrpn);
	EXPECT_EQ(11, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_EQ(false, item.isMSBOmittingRequired);

	item = actual[3];
	EXPECT_EQ((tick_t)456, item.tick);
	EXPECT_EQ(MidiParameterType::CC_VD_VIBRATO_DEPTH, item.nrpn);
	EXPECT_EQ(14, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_EQ(false, item.isMSBOmittingRequired);

	item = actual[4];
	EXPECT_EQ((tick_t)480, item.tick);
	EXPECT_EQ(MidiParameterType::CC_VR_VIBRATO_RATE, item.nrpn);
	EXPECT_EQ(12, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_EQ(false, item.isMSBOmittingRequired);
}

TEST(VocaloidMidiEventListFactoryTest, testGenerateVoiceChangeParameterNRPN)
{
	Sequence sequence("Foo", 1, 4, 4, 500000);
	Track& track = sequence.track(0);

	// 全種類のカーブに, データ点を1個ずつ入れる
	vector<string> curveNames;
	curveNames.push_back("BRE");
	curveNames.push_back("BRI");
	curveNames.push_back("CLE");
	curveNames.push_back("POR");
	curveNames.push_back("GEN");
	curveNames.push_back("harmonics");
	curveNames.push_back("OPE");
	curveNames.push_back("reso1amp");
	curveNames.push_back("reso1bw");
	curveNames.push_back("reso1freq");
	curveNames.push_back("reso2amp");
	curveNames.push_back("reso2bw");
	curveNames.push_back("reso2freq");
	curveNames.push_back("reso3amp");
	curveNames.push_back("reso3bw");
	curveNames.push_back("reso3freq");
	curveNames.push_back("reso4amp");
	curveNames.push_back("reso4bw");
	curveNames.push_back("reso4freq");
	for (int i = 0; i < curveNames.size(); i++) {
		string curveName = curveNames[i];
		BPList* list = track.curve(curveName);
		list->clear();
		list->name(curveName);
		list->defaultValue(0);
		list->minimum(0);
		list->maximum(127);
		list->add(480, 64);
	}

	// VOCALOID1 の場合
	track.common().version = "DSB2";
	vector<NrpnEvent> events = VocaloidMidiEventListFactory::generateVoiceChangeParameterNRPN(track, sequence.tempoList, 500, sequence.preMeasureTicks());
	// 中身は見ない. 各カーブに MIDI イベントが1つずつできることだけをチェック
	// 各イベントの子にあたるイベントのテストは, test_addVoiceChangeParameters で行う
	// vocaloid1 で出力されるのは 18 種類 + DELAY イベント 1 個
	EXPECT_EQ((size_t)(18 + 1), events.size());

	// VOCALOID2 の場合
	// 6 種類 + DELAY イベント 1 個
	track.common().version = "DSB3";
	events = VocaloidMidiEventListFactory::generateVoiceChangeParameterNRPN(track, sequence.tempoList, 500, sequence.preMeasureTicks());
	EXPECT_EQ((size_t)(6 + 1), events.size());

	// UNKNOWN の場合
	// 5 種類 + DELAY イベント 1 個
	track.common().version = "";
	events = VocaloidMidiEventListFactory::generateVoiceChangeParameterNRPN(track, sequence.tempoList, 500, sequence.preMeasureTicks());
	EXPECT_EQ((size_t)(5 + 1), events.size());
}

TEST(VocaloidMidiEventListFactoryTest, testAddVoiceChangeParameters)
{
	Sequence sequence("Foo", 1, 4, 4, 500000);
	BPList* list = sequence.track(0).curve("BRE");
	list->clear();
	list->add(480, 0);
	list->add(1920, 127);
	vector<NrpnEvent> dest;
	int preSend = 500;
	int delay = VocaloidMidiEventListFactory::addVoiceChangeParameters(dest, *list, sequence.tempoList, preSend, 0);

	EXPECT_EQ((size_t)3, dest.size());
	EXPECT_EQ(500, delay);

	{
		vector<NrpnEvent> actual = dest[0].expand();
		EXPECT_EQ((size_t)1, actual.size());
		EXPECT_EQ((tick_t)0, actual[0].tick);
		EXPECT_EQ(MidiParameterType::VCP_DELAY, actual[0].nrpn);
		EXPECT_EQ(0x03, actual[0].dataMSB);
		EXPECT_EQ(0x74, actual[0].dataLSB);
		EXPECT_TRUE(actual[0].hasLSB);
		EXPECT_EQ(false, actual[0].isMSBOmittingRequired);
	}

	{
		vector<NrpnEvent> actual = dest[1].expand();
		EXPECT_EQ((size_t)2, actual.size());
		EXPECT_EQ((tick_t)0, actual[0].tick);
		EXPECT_EQ(MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID, actual[0].nrpn);
		EXPECT_EQ(0x31, actual[0].dataMSB);
		EXPECT_EQ(false, actual[0].hasLSB);
		EXPECT_EQ(false, actual[0].isMSBOmittingRequired);
		EXPECT_EQ((tick_t)0, actual[1].tick);
		EXPECT_EQ(MidiParameterType::VCP_VOICE_CHANGE_PARAMETER, actual[1].nrpn);
		EXPECT_EQ(0, actual[1].dataMSB);
		EXPECT_EQ(false, actual[1].hasLSB);
		EXPECT_TRUE(actual[1].isMSBOmittingRequired);
	}

	{
		vector<NrpnEvent> actual = dest[2].expand();
		EXPECT_EQ((size_t)2, actual.size());
		EXPECT_EQ((tick_t)1440, actual[0].tick);
		EXPECT_EQ(MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID, actual[0].nrpn);
		EXPECT_EQ(0x31, actual[0].dataMSB);
		EXPECT_EQ(false, actual[0].hasLSB);
		EXPECT_EQ(false, actual[0].isMSBOmittingRequired);
		EXPECT_EQ((tick_t)1440, actual[1].tick);
		EXPECT_EQ(MidiParameterType::VCP_VOICE_CHANGE_PARAMETER, actual[1].nrpn);
		EXPECT_EQ(127, actual[1].dataMSB);
		EXPECT_EQ(false, actual[1].hasLSB);
		EXPECT_TRUE(actual[1].isMSBOmittingRequired);
	}
}

TEST(VocaloidMidiEventListFactoryTest, testGenerateFx2DepthNRPN)
{
	Sequence sequence("Miku", 1, 4, 4, 500000);
	Track& track = sequence.track(0);
	BPList* list = track.curve("fx2depth");
	list->clear();
	list->add(480, 64);
	list->add(1920, 63);

	vector<NrpnEvent> actual = VocaloidMidiEventListFactory::generateFx2DepthNRPN(track, sequence.tempoList, 500);
	EXPECT_EQ((size_t)2, actual.size());

	vector<NrpnEvent> expandedActual = actual[0].expand();
	NrpnEvent item = expandedActual[0];
	EXPECT_EQ((tick_t)0, item.tick);
	EXPECT_EQ(MidiParameterType::CC_FX2_DELAY, item.nrpn);
	EXPECT_EQ(0x03, item.dataMSB);
	EXPECT_EQ(0x74, item.dataLSB);
	EXPECT_TRUE(item.hasLSB);
	EXPECT_EQ(false, item.isMSBOmittingRequired);

	item = expandedActual[1];
	EXPECT_EQ((tick_t)0, item.tick);
	EXPECT_EQ(MidiParameterType::CC_FX2_EFFECT2_DEPTH, item.nrpn);
	EXPECT_EQ(64, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_EQ(false, item.isMSBOmittingRequired);

	item = actual[1];
	EXPECT_EQ((tick_t)1440, item.tick);
	EXPECT_EQ(MidiParameterType::CC_FX2_EFFECT2_DEPTH, item.nrpn);
	EXPECT_EQ(63, item.dataMSB);
	EXPECT_EQ(false, item.hasLSB);
	EXPECT_EQ(false, item.isMSBOmittingRequired);
}

TEST(VocaloidMidiEventListFactoryTest, testGetActualTickAndDelay)
{
	Sequence sequence("Miku", 1, 4, 4, 500000);
	tick_t actualTick;
	int delay;

	VocaloidMidiEventListFactory::_getActualTickAndDelay(sequence.tempoList, 1920, 500, &actualTick, &delay);
	EXPECT_EQ((tick_t)1440, actualTick);
	EXPECT_EQ(500, delay);

	VocaloidMidiEventListFactory::_getActualTickAndDelay(sequence.tempoList, 1920, 499, &actualTick, &delay);
	EXPECT_EQ((tick_t)1440, actualTick);
	EXPECT_EQ(500, delay);

	VocaloidMidiEventListFactory::_getActualTickAndDelay(sequence.tempoList, 1920, 498, &actualTick, &delay);
	EXPECT_EQ((tick_t)1441, actualTick);
	EXPECT_EQ(498, delay);

	VocaloidMidiEventListFactory::_getActualTickAndDelay(sequence.tempoList, 0, 500, &actualTick, &delay);
	EXPECT_EQ((tick_t)0, actualTick);
	EXPECT_EQ(0, delay);

	VocaloidMidiEventListFactory::_getActualTickAndDelay(sequence.tempoList, 0, 0, &actualTick, &delay);
	EXPECT_EQ((tick_t)0, actualTick);
	EXPECT_EQ(0, delay);
}

TEST(VocaloidMidiEventListFactoryTest, test_getMsbAndLsb)
{
	int msb, lsb;
	VocaloidMidiEventListFactory::_getMsbAndLsb(264, &msb, &lsb);
	EXPECT_EQ(2, msb);
	EXPECT_EQ(8, lsb);
}

TEST(VocaloidMidiEventListFactoryTest, testGenerateNRPN)
{
	Sequence sequence("Miku", 1, 4, 4, 500000);
	Track& track = sequence.track(0);

	// singerEvent
	Event singerEvent(1920, EventType::SINGER);
	singerEvent.singerHandle = Handle(HandleType::SINGER);
	track.events().add(singerEvent);

	// voiceChangeParameter
	vector<string> curveNames;
	curveNames.push_back("BRE");
	curveNames.push_back("BRI");
	curveNames.push_back("CLE");
	curveNames.push_back("POR");
	curveNames.push_back("GEN");
	curveNames.push_back("OPE");
	for (int i = 0; i < curveNames.size(); i++) {
		string curveName = curveNames[i];
		BPList* list = track.curve(curveName);
		list->clear();
		list->defaultValue(0);
		list->minimum(0);
		list->maximum(127);
		list->add(480, 64);
	}

	// expression
	track.curve("dyn")->clear();
	track.curve("dyn")->add(480, 127);
	track.curve("dyn")->add(1920, 0);

	// pitchBendSensitivity
	track.curve("pbs")->add(480, 0);
	track.curve("pbs")->add(1920, 24);

	// pitchBend
	track.curve("pit")->add(480, 8191);
	track.curve("pit")->add(1920, -8192);

	// note
	Event noteEvent(1920, EventType::NOTE);
	noteEvent.length(480);
	noteEvent.note = 60;
	noteEvent.dynamics = 127;
	noteEvent.pmBendDepth = 8;
	noteEvent.pmBendLength = 0;
	noteEvent.d4mean = 63;
	noteEvent.pMeanOnsetFirstNote = 65;
	noteEvent.vMeanNoteTransition = 66;
	noteEvent.pMeanEndingNote = 67;
	noteEvent.pmbPortamentoUse = 3;
	noteEvent.demDecGainRate = 50;
	noteEvent.demAccent = 50;
	noteEvent.vibratoHandle = Handle(HandleType::UNKNOWN);
	noteEvent.lyricHandle = Handle(HandleType::LYRIC);
	noteEvent.lyricHandle.set(0, Lyric("あ", "a"));
	track.events().add(noteEvent);

	int preSendMilliseconds = 500;
	vector<NrpnEvent> actual = VocaloidMidiEventListFactory::generateNRPN(
								   track, sequence.tempoList, sequence.totalTicks(), sequence.preMeasureTicks(), preSendMilliseconds);

	std::vector<NrpnEventSpec> expected = {
		// 0
		{ (tick_t)0,	MidiParameterType::CC_PBS_DELAY,						0x03,		true,	0x74,	false},
		{ (tick_t)0,	MidiParameterType::CC_PBS_PITCH_BEND_SENSITIVITY,		0,			true,	0x00,	false },
		{ (tick_t)0,	MidiParameterType::CC_E_DELAY,							0x03,		true,	0x74,	false },
		{ (tick_t)0,	MidiParameterType::CC_E_EXPRESSION,						127,		false,	0xFF,	false },
		{ (tick_t)0,	MidiParameterType::CC_BS_VERSION_AND_DEVICE,			0x00,		true,	0x00,	false },
		// 5
		{ (tick_t)0,	MidiParameterType::CC_BS_DELAY,							0x00,		true,	0x00,	true },
		{ (tick_t)0,	MidiParameterType::CC_BS_LANGUAGE_TYPE,					0,			false,	0xFF,	true },
		{ (tick_t)0,	MidiParameterType::PC_VOICE_TYPE,						0,			false,	0xFF,	false },
		{ (tick_t)0,	MidiParameterType::VCP_DELAY,							0x03,		true,	0x74,	false },
		{ (tick_t)0,	MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID,		0x31,		false,	0xFF,	false },
		// 10
		{ (tick_t)0,	MidiParameterType::VCP_VOICE_CHANGE_PARAMETER,			64,			false,	0xFF,	true },
		{ (tick_t)0,	MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID,		0x32,		false,	0xFF,	false },
		{ (tick_t)0,	MidiParameterType::VCP_VOICE_CHANGE_PARAMETER,			64,			false,	0xFF,	true },
		{ (tick_t)0,	MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID,		0x33,		false,	0xFF,	false },
		{ (tick_t)0,	MidiParameterType::VCP_VOICE_CHANGE_PARAMETER,			64,			false,	0xFF,	true },
		// 15
		{ (tick_t)0,	MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID,		0x34,		false,	0xFF,	false },
		{ (tick_t)0,	MidiParameterType::VCP_VOICE_CHANGE_PARAMETER,			64,			false,	0xFF,	true },
		{ (tick_t)0,	MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID,		0x35,		false,	0xFF,	false },
		{ (tick_t)0,	MidiParameterType::VCP_VOICE_CHANGE_PARAMETER,			64,			false,	0xFF,	true },
		{ (tick_t)0,	MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID,		0x70,		false,	0xFF,	false },
		// 20
		{ (tick_t)0,	MidiParameterType::VCP_VOICE_CHANGE_PARAMETER,			64,			false,	0xFF,	true },
		{ (tick_t)0,	MidiParameterType::PB_DELAY,							0x03,		true,	0x74,	false },
		{ (tick_t)0,	MidiParameterType::PB_PITCH_BEND,						0x7F,		true,	0x7F,	false },
		{ (tick_t)1440,	MidiParameterType::CC_PBS_PITCH_BEND_SENSITIVITY,		24,			true,	0x00,	false },
		{ (tick_t)1440, MidiParameterType::CC_E_EXPRESSION,						0,			false,	0xFF,	false },
		// 25
		{ (tick_t)1440, MidiParameterType::CC_BS_VERSION_AND_DEVICE,			0x00,		true,	0x00,	false },
		{ (tick_t)1440, MidiParameterType::CC_BS_DELAY,							0x03,		true,	0x74,	true },
		{ (tick_t)1440, MidiParameterType::CC_BS_LANGUAGE_TYPE,					0,			false,	0xFF,	true },
		{ (tick_t)1440, MidiParameterType::PC_VOICE_TYPE,						0,			false,	0xFF,	false },
		{ (tick_t)1440, MidiParameterType::PB_PITCH_BEND,						0x00,		true,	0x00,	false },
		// 30
		{ (tick_t)1440, MidiParameterType::CVM_NM_DELAY,						0x03,		true,	0x74,	false },
		{ (tick_t)1440, MidiParameterType::CVM_NM_NOTE_NUMBER,					60,			false,	0xFF,	true },
		{ (tick_t)1440, MidiParameterType::CVM_NM_VELOCITY,						127,		false,	0xFF,	true },
		{ (tick_t)1440, MidiParameterType::CVM_NM_NOTE_DURATION,				0x03,		true,	0x74,	true },
		{ (tick_t)1440, MidiParameterType::CVM_NM_NOTE_LOCATION,				3,			false,	0xFF,	true },
		// 35
		{ (tick_t)1440, MidiParameterType::CVM_NM_PHONETIC_SYMBOL_BYTES,		1,			false,	0xFF,	true },
		{ (tick_t)1440, (MidiParameterType)0x5013,	(int)'a',	true,	0,		true },
		{ (tick_t)1440, MidiParameterType::CVM_NM_PHONETIC_SYMBOL_CONTINUATION,	0x7f,		false,	0xFF,	true },
		{ (tick_t)1440, MidiParameterType::CVM_NM_V1MEAN,						4,			false,	0xFF,	true },
		{ (tick_t)1440, MidiParameterType::CVM_NM_D1MEAN,						8,			false,	0xFF,	true },
		// 40
		{ (tick_t)1440, MidiParameterType::CVM_NM_D1MEAN_FIRST_NOTE,			0x14,		false,	0xFF,	true },
		{ (tick_t)1440, MidiParameterType::CVM_NM_D2MEAN,						28,			false,	0xFF,	true },
		{ (tick_t)1440, MidiParameterType::CVM_NM_D4MEAN,						63,			false,	0xFF,	true },
		{ (tick_t)1440, MidiParameterType::CVM_NM_PMEAN_ONSET_FIRST_NOTE,		65,			false,	0xFF,	true },
		{ (tick_t)1440, MidiParameterType::CVM_NM_VMEAN_NOTE_TRNSITION,			66,			false,	0xFF,	true },
		// 45
		{ (tick_t)1440, MidiParameterType::CVM_NM_PMEAN_ENDING_NOTE,			67,			false,	0xFF,	true },
		{ (tick_t)1440, MidiParameterType::CVM_NM_ADD_PORTAMENTO,				3,			false,	0xFF,	true },
		{ (tick_t)1440, MidiParameterType::CVM_NM_CHANGE_AFTER_PEAK,			50,			false,	0xFF,	true },
		{ (tick_t)1440, MidiParameterType::CVM_NM_ACCENT,						50,			false,	0xFF,	true },
		{ (tick_t)1440, MidiParameterType::CVM_NM_NOTE_MESSAGE_CONTINUATION,	0x7f,		false,	0xFF,	true },
	};

	EXPECT_EQ(expected.size(), actual.size());

	for (int i = 0; i < expected.size(); ++i) {
		ostringstream message;
		message << "#" << i << " element";
		NrpnEventSpec const& spec = expected[i];
		EXPECT_EQ(spec.tick, actual[i].tick);
		EXPECT_EQ(spec.nrpn, actual[i].nrpn);
		EXPECT_EQ(spec.dataMSB, actual[i].dataMSB);
		EXPECT_EQ(spec.hasLSB, actual[i].hasLSB);
		if (spec.hasLSB) {
			EXPECT_EQ(spec.dataLSB, actual[i].dataLSB);
		}
		EXPECT_EQ(spec.isMSBOmittingRequired, actual[i].isMSBOmittingRequired);
	}
}
