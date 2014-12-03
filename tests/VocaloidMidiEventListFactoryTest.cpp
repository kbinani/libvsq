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

class VocaloidMidiEventListFactoryStub : public VocaloidMidiEventListFactory
{
public:
	static vector<NrpnEvent> generateNRPN(
		Track const& target, TempoList const& tempoList, tick_t totalTicks, tick_t preMeasureTicks, int msPreSend)
	{
		return VocaloidMidiEventListFactory::generateNRPN(target, tempoList, totalTicks, preMeasureTicks, msPreSend);
	}

	static vector<NrpnEvent> generateExpressionNRPN(Track& track, TempoList& tempoList, int preSendMilliseconds)
	{
		return VocaloidMidiEventListFactory::generateExpressionNRPN(track, tempoList, preSendMilliseconds);
	}

	static NrpnEvent generateHeaderNRPN()
	{
		return VocaloidMidiEventListFactory::generateHeaderNRPN();
	}

	static vector<NrpnEvent> generateSingerNRPN(TempoList& tempoList, Event const& singerEvent, int preSendMilliseconds)
	{
		return VocaloidMidiEventListFactory::generateSingerNRPN(tempoList, singerEvent, preSendMilliseconds);
	}

	static NrpnEvent generateNoteNRPN(Track& track, TempoList& tempoList, Event const& noteEvent, int msPreSend, int noteLocation, int* lastDelay, int* delay)
	{
		return VocaloidMidiEventListFactory::generateNoteNRPN(track, tempoList, noteEvent, msPreSend, noteLocation, lastDelay, delay);
	}

	static vector<NrpnEvent> generatePitchBendNRPN(Track& track, TempoList& tempoList, int msPreSend)
	{
		return VocaloidMidiEventListFactory::generatePitchBendNRPN(track, tempoList, msPreSend);
	}

	static vector<NrpnEvent> generatePitchBendSensitivityNRPN(Track& track, TempoList& tempoList, int msPreSend)
	{
		return VocaloidMidiEventListFactory::generatePitchBendSensitivityNRPN(track, tempoList, msPreSend);
	}

	static vector<NrpnEvent> generateVibratoNRPN(TempoList& tempoList, Event& noteEvent, int msPreSend)
	{
		return VocaloidMidiEventListFactory::generateVibratoNRPN(tempoList, noteEvent, msPreSend);
	}

	static vector<NrpnEvent> generateVoiceChangeParameterNRPN(Track& track, TempoList& tempoList, int msPreSend, tick_t preMeasureTicks)
	{
		return VocaloidMidiEventListFactory::generateVoiceChangeParameterNRPN(track, tempoList, msPreSend, preMeasureTicks);
	}

	static vector<NrpnEvent> generateFx2DepthNRPN(Track& track, TempoList& tempoList, int preSendMilliseconds)
	{
		return VocaloidMidiEventListFactory::generateFx2DepthNRPN(track, tempoList, preSendMilliseconds);
	}

	static int addVoiceChangeParameters(vector<NrpnEvent>& dest, BPList& list, TempoList& tempoList, int msPreSend, int lastDelay)
	{
		return VocaloidMidiEventListFactory::addVoiceChangeParameters(dest, list, tempoList, msPreSend, lastDelay);
	}

	static void getActualTickAndDelay(TempoList& tempoList, tick_t tick, int msPreSend, tick_t* actualTick, int* delay)
	{
		_getActualTickAndDelay(tempoList, tick, msPreSend, actualTick, delay);
	}

	static void getMsbAndLsb(int value, int* msb, int* lsb)
	{
		_getMsbAndLsb(value, msb, lsb);
	}
};

class VocaloidMidiEventListFactoryTest : public CppUnit::TestCase
{
public:
	void test_generateExpressionNRPN()
	{
		Sequence sequence("Miku", 1, 4, 4, 500000);
		Track& track = sequence.track(0);
		BPList* dynamics = track.curve("DYN");
		dynamics->add(480, 127);
		dynamics->add(1920, 0);

		vector<NrpnEvent> actual = VocaloidMidiEventListFactoryStub::generateExpressionNRPN(track, sequence.tempoList, 500);
		CPPUNIT_ASSERT_EQUAL((size_t)2, actual.size());

		vector<NrpnEvent> expandedActual = actual[0].expand();
		CPPUNIT_ASSERT_EQUAL((tick_t)0, expandedActual[0].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_E_DELAY, expandedActual[0].nrpn);
		CPPUNIT_ASSERT_EQUAL(0x03, expandedActual[0].dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x74, expandedActual[0].dataLSB);
		CPPUNIT_ASSERT(expandedActual[0].hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, expandedActual[0].isMSBOmittingRequired);

		CPPUNIT_ASSERT_EQUAL((tick_t)0, expandedActual[1].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_E_EXPRESSION, expandedActual[1].nrpn);
		CPPUNIT_ASSERT_EQUAL(127, expandedActual[1].dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, expandedActual[1].hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, expandedActual[1].isMSBOmittingRequired);

		CPPUNIT_ASSERT_EQUAL((tick_t)1440, actual[1].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_E_EXPRESSION, actual[1].nrpn);
		CPPUNIT_ASSERT_EQUAL(0, actual[1].dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, actual[1].hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, actual[1].isMSBOmittingRequired);
	}

	void test_generateHeaderNRPN()
	{
		vector<NrpnEvent> actual = VocaloidMidiEventListFactoryStub::generateHeaderNRPN().expand();
		CPPUNIT_ASSERT_EQUAL((size_t)3, actual.size());

		CPPUNIT_ASSERT_EQUAL((tick_t)0, actual[0].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_BS_VERSION_AND_DEVICE, actual[0].nrpn);
		CPPUNIT_ASSERT_EQUAL(0x00, actual[0].dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x00, actual[0].dataLSB);
		CPPUNIT_ASSERT(actual[0].hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, actual[0].isMSBOmittingRequired);

		CPPUNIT_ASSERT_EQUAL((tick_t)0, actual[1].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_BS_DELAY, actual[1].nrpn);
		CPPUNIT_ASSERT_EQUAL(0x00, actual[1].dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x00, actual[1].dataLSB);
		CPPUNIT_ASSERT(actual[1].hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, actual[1].isMSBOmittingRequired);

		CPPUNIT_ASSERT_EQUAL((tick_t)0, actual[2].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_BS_LANGUAGE_TYPE, actual[2].nrpn);
		CPPUNIT_ASSERT_EQUAL(0x00, actual[2].dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, actual[2].hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, actual[2].isMSBOmittingRequired);
	}

	void testGenerateSingerNRPN()
	{
		Sequence sequence("Miku", 1, 4, 4, 500000);
		Event singerEvent(1920, EventType::SINGER);
		singerEvent.singerHandle = Handle(HandleType::SINGER);
		sequence.track(0).events().add(singerEvent);
		vector<NrpnEvent> actual = VocaloidMidiEventListFactoryStub::generateSingerNRPN(sequence.tempoList, singerEvent, 500);

		CPPUNIT_ASSERT_EQUAL((size_t)1, actual.size());
		vector<NrpnEvent> actualExpanded = actual[0].expand();

		CPPUNIT_ASSERT_EQUAL((size_t)4, actualExpanded.size());
		NrpnEvent item = actualExpanded[0];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_BS_VERSION_AND_DEVICE, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(0x00, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x00, item.dataLSB);
		CPPUNIT_ASSERT(item.hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, item.isMSBOmittingRequired);

		item = actualExpanded[1];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_BS_DELAY, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(0x03, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x74, item.dataLSB);
		CPPUNIT_ASSERT(item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);

		item = actualExpanded[2];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_BS_LANGUAGE_TYPE, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(singerEvent.singerHandle.language, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);

		item = actualExpanded[3];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::PC_VOICE_TYPE, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(singerEvent.singerHandle.program, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, item.isMSBOmittingRequired);
	}

	void testGenerateNoteNRPN()
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
		NrpnEvent actual = VocaloidMidiEventListFactoryStub::generateNoteNRPN(sequence.track(0), sequence.tempoList, noteEvent, msPreSend, noteLocation, (int*)0, &delay);
		vector<NrpnEvent> actualExpanded = actual.expand();
		CPPUNIT_ASSERT_EQUAL((size_t)24, actualExpanded.size());
		CPPUNIT_ASSERT_EQUAL(500, delay);
		NrpnEvent item = actualExpanded[0];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_VERSION_AND_DEVICE, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(0x00, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x00, item.dataLSB);
		CPPUNIT_ASSERT(item.hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, item.isMSBOmittingRequired);
		item = actualExpanded[1];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_DELAY, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(0x03, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x74, item.dataLSB);
		CPPUNIT_ASSERT(item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[2];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_NOTE_NUMBER, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(60, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[3];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_VELOCITY, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(127, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[4];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_NOTE_DURATION, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(0x03, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x74, item.dataLSB);
		CPPUNIT_ASSERT(item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[5];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_NOTE_LOCATION, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(1, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[6];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_INDEX_OF_VIBRATO_DB, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(0x00, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x00, item.dataLSB);
		CPPUNIT_ASSERT(item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[7];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_VIBRATO_CONFIG, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(5, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(63, item.dataLSB);
		CPPUNIT_ASSERT(item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[8];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_VIBRATO_DELAY, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(64, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[9];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_PHONETIC_SYMBOL_BYTES, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(1, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[10];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL((MidiParameterType)0x5013, item.nrpn);
		CPPUNIT_ASSERT_EQUAL((int)'a', item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(0, item.dataLSB);
		CPPUNIT_ASSERT(item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[11];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_PHONETIC_SYMBOL_CONTINUATION, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(0x7f, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[12];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_V1MEAN, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(4, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[13];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_D1MEAN, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(8, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[14];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_D1MEAN_FIRST_NOTE, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(0x14, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[15];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_D2MEAN, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(28, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[16];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_D4MEAN, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(63, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[17];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_PMEAN_ONSET_FIRST_NOTE, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(65, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[18];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_VMEAN_NOTE_TRNSITION, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(66, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[19];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_PMEAN_ENDING_NOTE, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(67, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[20];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_ADD_PORTAMENTO, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(3, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[21];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_CHANGE_AFTER_PEAK, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(50, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[22];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_ACCENT, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(50, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
		item = actualExpanded[23];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_NOTE_MESSAGE_CONTINUATION, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(0x7f, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);

		// lastDelay が nil でないために, CVM_NM_VERSION_AND_DEVICE が出力されないパターン
		lastDelay = 0;
		actual = VocaloidMidiEventListFactoryStub::generateNoteNRPN(sequence.track(track), sequence.tempoList, noteEvent, msPreSend, noteLocation, &lastDelay, &delay);
		actualExpanded = actual.expand();
		CPPUNIT_ASSERT_EQUAL((size_t)23, actualExpanded.size());
		CPPUNIT_ASSERT_EQUAL(500, delay);

		// lastDelay が, 該当音符についての delay と同じであるために, CVM_NM_DELAY が出力されないパターン
		lastDelay = 500;
		actual = VocaloidMidiEventListFactoryStub::generateNoteNRPN(sequence.track(track), sequence.tempoList, noteEvent, msPreSend, noteLocation, &lastDelay, &delay);
		actualExpanded = actual.expand();
		CPPUNIT_ASSERT_EQUAL((size_t)22, actualExpanded.size());
		CPPUNIT_ASSERT_EQUAL(500, delay);

		// vibratoHandle が nil であるために, CVM_NM_INDEX_OF_VIBRATO_DB, CVM_NM_VIBRATO_CONFIG, CVM_NM_VIBRATO_DELAY
		// が出力されないパターン
		lastDelay = 500;
		noteEvent.vibratoHandle = Handle();
		actual = VocaloidMidiEventListFactoryStub::generateNoteNRPN(sequence.track(track), sequence.tempoList, noteEvent, msPreSend, noteLocation, &lastDelay, &delay);
		actualExpanded = actual.expand();
		CPPUNIT_ASSERT_EQUAL((size_t)19, actualExpanded.size());
		CPPUNIT_ASSERT_EQUAL(500, delay);

		sequence.track(0).common().version = "DSB2";
		// VOCALOID1 であるために, 0x5011が出力され, CVM_NM_PHONETIC_SYMBOL_CONTINUATIONとVOCALOID2用のNRPNが出力されない
		lastDelay = 500;
		noteEvent.vibratoHandle = Handle();
		actual = VocaloidMidiEventListFactoryStub::generateNoteNRPN(sequence.track(track), sequence.tempoList, noteEvent, msPreSend, noteLocation, &lastDelay, &delay);
		actualExpanded = actual.expand();
		CPPUNIT_ASSERT_EQUAL((size_t)8, actualExpanded.size());
		item = actualExpanded[4];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL((MidiParameterType)0x5011, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(0x01, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT(item.isMSBOmittingRequired);
	}

	void testGeneratePitchBendNRPN()
	{
		Sequence sequence("Miku", 1, 4, 4, 500000);
		BPList* pit = sequence.track(0).curve("PIT");
		pit->add(480, 8191);
		pit->add(1920, -8192);

		vector<NrpnEvent> actual = VocaloidMidiEventListFactoryStub::generatePitchBendNRPN(sequence.track(0), sequence.tempoList, 500);
		CPPUNIT_ASSERT_EQUAL((size_t)2, actual.size());

		vector<NrpnEvent> expandedActual = actual[0].expand();
		CPPUNIT_ASSERT_EQUAL((tick_t)0, expandedActual[0].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::PB_DELAY, expandedActual[0].nrpn);
		CPPUNIT_ASSERT_EQUAL(0x03, expandedActual[0].dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x74, expandedActual[0].dataLSB);
		CPPUNIT_ASSERT(expandedActual[0].hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, expandedActual[0].isMSBOmittingRequired);

		CPPUNIT_ASSERT_EQUAL((tick_t)0, expandedActual[1].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::PB_PITCH_BEND, expandedActual[1].nrpn);
		CPPUNIT_ASSERT_EQUAL(0x7F, expandedActual[1].dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x7F, expandedActual[1].dataLSB);
		CPPUNIT_ASSERT(expandedActual[1].hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, expandedActual[1].isMSBOmittingRequired);

		CPPUNIT_ASSERT_EQUAL((tick_t)1440, actual[1].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::PB_PITCH_BEND, actual[1].nrpn);
		CPPUNIT_ASSERT_EQUAL(0x00, actual[1].dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x00, actual[1].dataLSB);
		CPPUNIT_ASSERT(actual[1].hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, actual[1].isMSBOmittingRequired);
	}

	void testGeneratePitchBendSensitivityNRPN()
	{
		Sequence sequence("Miku", 1, 4, 4, 500000);
		BPList* pbs = sequence.track(0).curve("PBS");
		pbs->add(480, 0);
		pbs->add(1920, 24);

		vector<NrpnEvent> actual = VocaloidMidiEventListFactoryStub::generatePitchBendSensitivityNRPN(sequence.track(0), sequence.tempoList, 500);
		CPPUNIT_ASSERT_EQUAL((size_t)2, actual.size());

		vector<NrpnEvent> expandedActual = actual[0].expand();
		CPPUNIT_ASSERT_EQUAL((tick_t)0, expandedActual[0].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_PBS_DELAY, expandedActual[0].nrpn);
		CPPUNIT_ASSERT_EQUAL(0x03, expandedActual[0].dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x74, expandedActual[0].dataLSB);
		CPPUNIT_ASSERT(expandedActual[0].hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, expandedActual[0].isMSBOmittingRequired);

		CPPUNIT_ASSERT_EQUAL((tick_t)0, expandedActual[1].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_PBS_PITCH_BEND_SENSITIVITY, expandedActual[1].nrpn);
		CPPUNIT_ASSERT_EQUAL(0, expandedActual[1].dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x00, expandedActual[1].dataLSB);
		CPPUNIT_ASSERT(expandedActual[1].hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, expandedActual[1].isMSBOmittingRequired);

		CPPUNIT_ASSERT_EQUAL((tick_t)1440, actual[1].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_PBS_PITCH_BEND_SENSITIVITY, actual[1].nrpn);
		CPPUNIT_ASSERT_EQUAL(24, actual[1].dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x00, actual[1].dataLSB);
		CPPUNIT_ASSERT(actual[1].hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, actual[1].isMSBOmittingRequired);
	}

	void testGenerateVibratoNRPN()
	{
		Sequence sequence("Miku", 1, 4, 4, 500000);
		Event noteEvent(480, EventType::NOTE);
		noteEvent.vibratoHandle = Handle();

		// ビブラートがないため, NRPN が生成されない場合
		vector<NrpnEvent> actual = VocaloidMidiEventListFactoryStub::generateVibratoNRPN(sequence.tempoList, noteEvent, 500);
		CPPUNIT_ASSERT_EQUAL((size_t)0, actual.size());

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
		actual = VocaloidMidiEventListFactoryStub::generateVibratoNRPN(sequence.tempoList, noteEvent, 500);
		CPPUNIT_ASSERT_EQUAL((size_t)5, actual.size());

		vector<NrpnEvent> actualExpanded = actual[0].expand();
		CPPUNIT_ASSERT_EQUAL((size_t)6, actualExpanded.size());
		NrpnEvent item = actualExpanded[0];
		CPPUNIT_ASSERT_EQUAL((tick_t)240, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_VD_VERSION_AND_DEVICE, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(0x00, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x00, item.dataLSB);
		CPPUNIT_ASSERT(item.hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, item.isMSBOmittingRequired);
		item = actualExpanded[1];
		CPPUNIT_ASSERT_EQUAL((tick_t)240, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_VR_VERSION_AND_DEVICE, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(0x00, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x00, item.dataLSB);
		CPPUNIT_ASSERT(item.hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, item.isMSBOmittingRequired);
		item = actualExpanded[2];
		CPPUNIT_ASSERT_EQUAL((tick_t)240, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_VD_DELAY, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(0x03, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x74, item.dataLSB);
		CPPUNIT_ASSERT(item.hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, item.isMSBOmittingRequired);
		item = actualExpanded[3];
		CPPUNIT_ASSERT_EQUAL((tick_t)240, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_VR_DELAY, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(0x03, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x74, item.dataLSB);
		CPPUNIT_ASSERT(item.hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, item.isMSBOmittingRequired);
		item = actualExpanded[4];
		CPPUNIT_ASSERT_EQUAL((tick_t)240, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_VD_VIBRATO_DEPTH, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(71, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, item.isMSBOmittingRequired);
		item = actualExpanded[5];
		CPPUNIT_ASSERT_EQUAL((tick_t)240, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_VR_VIBRATO_RATE, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(72, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, item.isMSBOmittingRequired);

		actualExpanded = actual[1].expand();
		CPPUNIT_ASSERT_EQUAL((size_t)2, actualExpanded.size());
		item = actualExpanded[0];
		CPPUNIT_ASSERT_EQUAL((tick_t)336, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_VD_DELAY, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(0x03, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x74, item.dataLSB);
		CPPUNIT_ASSERT(item.hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, item.isMSBOmittingRequired);
		item = actualExpanded[1];
		CPPUNIT_ASSERT_EQUAL((tick_t)336, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_VD_VIBRATO_DEPTH, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(13, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, item.isMSBOmittingRequired);

		item = actual[2];
		actualExpanded = item.expand();
		CPPUNIT_ASSERT_EQUAL((size_t)2, actualExpanded.size());
		item = actualExpanded[0];
		CPPUNIT_ASSERT_EQUAL((tick_t)360, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_VR_DELAY, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(0x03, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x74, item.dataLSB);
		CPPUNIT_ASSERT(item.hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, item.isMSBOmittingRequired);
		item = actualExpanded[1];
		CPPUNIT_ASSERT_EQUAL((tick_t)360, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_VR_VIBRATO_RATE, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(11, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, item.isMSBOmittingRequired);

		item = actual[3];
		CPPUNIT_ASSERT_EQUAL((tick_t)456, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_VD_VIBRATO_DEPTH, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(14, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, item.isMSBOmittingRequired);

		item = actual[4];
		CPPUNIT_ASSERT_EQUAL((tick_t)480, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_VR_VIBRATO_RATE, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(12, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, item.isMSBOmittingRequired);
	}

	void testGenerateVoiceChangeParameterNRPN()
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
		vector<NrpnEvent> events = VocaloidMidiEventListFactoryStub::generateVoiceChangeParameterNRPN(track, sequence.tempoList, 500, sequence.preMeasureTicks());
		// 中身は見ない. 各カーブに MIDI イベントが1つずつできることだけをチェック
		// 各イベントの子にあたるイベントのテストは, test_addVoiceChangeParameters で行う
		// vocaloid1 で出力されるのは 18 種類 + DELAY イベント 1 個
		CPPUNIT_ASSERT_EQUAL((size_t)(18 + 1), events.size());

		// VOCALOID2 の場合
		// 6 種類 + DELAY イベント 1 個
		track.common().version = "DSB3";
		events = VocaloidMidiEventListFactoryStub::generateVoiceChangeParameterNRPN(track, sequence.tempoList, 500, sequence.preMeasureTicks());
		CPPUNIT_ASSERT_EQUAL((size_t)(6 + 1), events.size());

		// UNKNOWN の場合
		// 5 種類 + DELAY イベント 1 個
		track.common().version = "";
		events = VocaloidMidiEventListFactoryStub::generateVoiceChangeParameterNRPN(track, sequence.tempoList, 500, sequence.preMeasureTicks());
		CPPUNIT_ASSERT_EQUAL((size_t)(5 + 1), events.size());
	}

	void testAddVoiceChangeParameters()
	{
		Sequence sequence("Foo", 1, 4, 4, 500000);
		BPList* list = sequence.track(0).curve("BRE");
		list->clear();
		list->add(480, 0);
		list->add(1920, 127);
		vector<NrpnEvent> dest;
		int preSend = 500;
		int delay = VocaloidMidiEventListFactoryStub::addVoiceChangeParameters(dest, *list, sequence.tempoList, preSend, 0);

		CPPUNIT_ASSERT_EQUAL((size_t)3, dest.size());
		CPPUNIT_ASSERT_EQUAL(500, delay);

		{
			vector<NrpnEvent> actual = dest[0].expand();
			CPPUNIT_ASSERT_EQUAL((size_t)1, actual.size());
			CPPUNIT_ASSERT_EQUAL((tick_t)0, actual[0].tick);
			CPPUNIT_ASSERT_EQUAL(MidiParameterType::VCP_DELAY, actual[0].nrpn);
			CPPUNIT_ASSERT_EQUAL(0x03, actual[0].dataMSB);
			CPPUNIT_ASSERT_EQUAL(0x74, actual[0].dataLSB);
			CPPUNIT_ASSERT(actual[0].hasLSB);
			CPPUNIT_ASSERT_EQUAL(false, actual[0].isMSBOmittingRequired);
		}

		{
			vector<NrpnEvent> actual = dest[1].expand();
			CPPUNIT_ASSERT_EQUAL((size_t)2, actual.size());
			CPPUNIT_ASSERT_EQUAL((tick_t)0, actual[0].tick);
			CPPUNIT_ASSERT_EQUAL(MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID, actual[0].nrpn);
			CPPUNIT_ASSERT_EQUAL(0x31, actual[0].dataMSB);
			CPPUNIT_ASSERT_EQUAL(false, actual[0].hasLSB);
			CPPUNIT_ASSERT_EQUAL(false, actual[0].isMSBOmittingRequired);
			CPPUNIT_ASSERT_EQUAL((tick_t)0, actual[1].tick);
			CPPUNIT_ASSERT_EQUAL(MidiParameterType::VCP_VOICE_CHANGE_PARAMETER, actual[1].nrpn);
			CPPUNIT_ASSERT_EQUAL(0, actual[1].dataMSB);
			CPPUNIT_ASSERT_EQUAL(false, actual[1].hasLSB);
			CPPUNIT_ASSERT(actual[1].isMSBOmittingRequired);
		}

		{
			vector<NrpnEvent> actual = dest[2].expand();
			CPPUNIT_ASSERT_EQUAL((size_t)2, actual.size());
			CPPUNIT_ASSERT_EQUAL((tick_t)1440, actual[0].tick);
			CPPUNIT_ASSERT_EQUAL(MidiParameterType::VCP_VOICE_CHANGE_PARAMETER_ID, actual[0].nrpn);
			CPPUNIT_ASSERT_EQUAL(0x31, actual[0].dataMSB);
			CPPUNIT_ASSERT_EQUAL(false, actual[0].hasLSB);
			CPPUNIT_ASSERT_EQUAL(false, actual[0].isMSBOmittingRequired);
			CPPUNIT_ASSERT_EQUAL((tick_t)1440, actual[1].tick);
			CPPUNIT_ASSERT_EQUAL(MidiParameterType::VCP_VOICE_CHANGE_PARAMETER, actual[1].nrpn);
			CPPUNIT_ASSERT_EQUAL(127, actual[1].dataMSB);
			CPPUNIT_ASSERT_EQUAL(false, actual[1].hasLSB);
			CPPUNIT_ASSERT(actual[1].isMSBOmittingRequired);
		}
	}

	void testGenerateFx2DepthNRPN()
	{
		Sequence sequence("Miku", 1, 4, 4, 500000);
		Track& track = sequence.track(0);
		BPList* list = track.curve("fx2depth");
		list->clear();
		list->add(480, 64);
		list->add(1920, 63);

		vector<NrpnEvent> actual = VocaloidMidiEventListFactoryStub::generateFx2DepthNRPN(track, sequence.tempoList, 500);
		CPPUNIT_ASSERT_EQUAL((size_t)2, actual.size());

		vector<NrpnEvent> expandedActual = actual[0].expand();
		NrpnEvent item = expandedActual[0];
		CPPUNIT_ASSERT_EQUAL((tick_t)0, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_FX2_DELAY, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(0x03, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x74, item.dataLSB);
		CPPUNIT_ASSERT(item.hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, item.isMSBOmittingRequired);

		item = expandedActual[1];
		CPPUNIT_ASSERT_EQUAL((tick_t)0, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_FX2_EFFECT2_DEPTH, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(64, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, item.isMSBOmittingRequired);

		item = actual[1];
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, item.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CC_FX2_EFFECT2_DEPTH, item.nrpn);
		CPPUNIT_ASSERT_EQUAL(63, item.dataMSB);
		CPPUNIT_ASSERT_EQUAL(false, item.hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, item.isMSBOmittingRequired);
	}

	void testGetActualTickAndDelay()
	{
		Sequence sequence("Miku", 1, 4, 4, 500000);
		tick_t actualTick;
		int delay;

		VocaloidMidiEventListFactoryStub::getActualTickAndDelay(sequence.tempoList, 1920, 500, &actualTick, &delay);
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, actualTick);
		CPPUNIT_ASSERT_EQUAL(500, delay);

		VocaloidMidiEventListFactoryStub::getActualTickAndDelay(sequence.tempoList, 1920, 499, &actualTick, &delay);
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, actualTick);
		CPPUNIT_ASSERT_EQUAL(500, delay);

		VocaloidMidiEventListFactoryStub::getActualTickAndDelay(sequence.tempoList, 1920, 498, &actualTick, &delay);
		CPPUNIT_ASSERT_EQUAL((tick_t)1441, actualTick);
		CPPUNIT_ASSERT_EQUAL(498, delay);

		VocaloidMidiEventListFactoryStub::getActualTickAndDelay(sequence.tempoList, 0, 500, &actualTick, &delay);
		CPPUNIT_ASSERT_EQUAL((tick_t)0, actualTick);
		CPPUNIT_ASSERT_EQUAL(0, delay);

		VocaloidMidiEventListFactoryStub::getActualTickAndDelay(sequence.tempoList, 0, 0, &actualTick, &delay);
		CPPUNIT_ASSERT_EQUAL((tick_t)0, actualTick);
		CPPUNIT_ASSERT_EQUAL(0, delay);
	}

	void test_getMsbAndLsb()
	{
		int msb, lsb;
		VocaloidMidiEventListFactoryStub::getMsbAndLsb(264, &msb, &lsb);
		CPPUNIT_ASSERT_EQUAL(2, msb);
		CPPUNIT_ASSERT_EQUAL(8, lsb);
	}

	void testGenerateNRPN()
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
		vector<NrpnEvent> actual = VocaloidMidiEventListFactoryStub::generateNRPN(
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

		CPPUNIT_ASSERT_EQUAL(expected.size(), actual.size());

		for (int i = 0; i < expected.size(); ++i) {
			ostringstream message;
			message << "#" << i << " element";
			NrpnEventSpec const& spec = expected[i];
			CPPUNIT_ASSERT_EQUAL_MESSAGE(message.str(), spec.tick, actual[i].tick);
			CPPUNIT_ASSERT_EQUAL_MESSAGE(message.str(), spec.nrpn, actual[i].nrpn);
			CPPUNIT_ASSERT_EQUAL_MESSAGE(message.str(), spec.dataMSB, actual[i].dataMSB);
			CPPUNIT_ASSERT_EQUAL_MESSAGE(message.str(), spec.hasLSB, actual[i].hasLSB);
			if (spec.hasLSB) {
				CPPUNIT_ASSERT_EQUAL_MESSAGE(message.str(), spec.dataLSB, actual[i].dataLSB);
			}
			CPPUNIT_ASSERT_EQUAL_MESSAGE(message.str(), spec.isMSBOmittingRequired, actual[i].isMSBOmittingRequired);
		}
	}

	CPPUNIT_TEST_SUITE(VocaloidMidiEventListFactoryTest);
	CPPUNIT_TEST(test_generateExpressionNRPN);
	CPPUNIT_TEST(test_generateHeaderNRPN);
	CPPUNIT_TEST(testGenerateSingerNRPN);
	CPPUNIT_TEST(testGenerateNoteNRPN);
	CPPUNIT_TEST(testGeneratePitchBendNRPN);
	CPPUNIT_TEST(testGeneratePitchBendSensitivityNRPN);
	CPPUNIT_TEST(testGenerateVibratoNRPN);
	CPPUNIT_TEST(testGenerateVoiceChangeParameterNRPN);
	CPPUNIT_TEST(testAddVoiceChangeParameters);
	CPPUNIT_TEST(testGenerateFx2DepthNRPN);
	CPPUNIT_TEST(testGetActualTickAndDelay);
	CPPUNIT_TEST(test_getMsbAndLsb);
	CPPUNIT_TEST(testGenerateNRPN);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(VocaloidMidiEventListFactoryTest);
