#include "Util.hpp"
#include "../include/libvsq/VSQFileWriter.hpp"
#include "../include/libvsq/ByteArrayOutputStream.hpp"
#include "../include/libvsq/Sequence.hpp"
#include "../include/libvsq/TextStream.hpp"
#include "../include/libvsq/StringUtil.hpp"
#include <iostream>

using namespace std;
using namespace vsq;

static string getLF()
{
	static string result = "\x0A";
	return result;
}

static Handle getLyricHandle()
{
	Handle handle(HandleType::LYRIC);
	Lyric lyric0("あ", "a");
	lyric0.consonantAdjustment("0");
	lyric0.lengthRatio = 0.4;
	lyric0.isProtected = true;
	Lyric lyric1("は", "h a");
	lyric1.consonantAdjustment("64,0");
	lyric1.lengthRatio = 0.6;
	lyric1.isProtected = false;
	handle.set(0, lyric0);
	handle.add(lyric1);
	handle.index = 1;
	return handle;
}

static Handle getCrescendoHandle()
{
	Handle result(HandleType::DYNAMICS);
	result.iconId = "$05020001";
	result.ids = "Crescendo";
	result.original = 4;
	result.index = 4;
	result.caption = "Zero Crescendo Curve";
	result.length(960);
	result.startDyn = 2;
	result.endDyn = 38;
	vector<double> dynBPX;
	dynBPX.push_back(0.5);
	vector<int> dynBPY;
	dynBPY.push_back(11);
	result.dynBP = VibratoBPList(dynBPX, dynBPY);;
	return result;
}

static Handle getVibratoHandle()
{
	Handle result(HandleType::VIBRATO);
	result.iconId = "$04040004";
	result.ids = "normal-da-yo";
	result.caption = "キャプションです=あ";
	result.original = 5;
	result.length(120);
	result.index = 1;

	result.startDepth = 64;
	vector<double> depthBPX;
	depthBPX.push_back(0.5);
	depthBPX.push_back(0.75);
	depthBPX.push_back(1.0);
	vector<int> depthBPY;
	depthBPY.push_back(64);
	depthBPY.push_back(32);
	depthBPY.push_back(0);
	result.depthBP = VibratoBPList(depthBPX, depthBPY);

	result.startRate = 64;
	vector<double> rateBPX;
	rateBPX.push_back(0.5);
	rateBPX.push_back(0.75);
	rateBPX.push_back(1.0);
	vector<int> rateBPY;
	rateBPY.push_back(64);
	rateBPY.push_back(32);
	rateBPY.push_back(0);
	result.rateBP = VibratoBPList(rateBPX, rateBPY);

	return result;
}

static Handle getSingerHandle()
{
	Handle result(HandleType::SINGER);
	result.iconId = "$07010002";
	result.ids = "Miku3=God";
	result.original = 2;
	result.caption = "";
	result.length(1);
	result.language = 1;
	result.program = 2;
	result.index = 2;
	return result;
}

static Handle getAttackHandle()
{
	Handle result(HandleType::NOTE_HEAD);
	result.iconId = "$01010002";
	result.ids = "accent";
	result.original = 2;
	result.index = 3;
	result.caption = "Accent";
	result.length(120);
	result.duration = 64;
	result.depth = 63;
	return result;
}

TEST(VSQFileWriterTest, testWriteWithoutPitch)
{
	Sequence sequence("Foo", 1, 4, 4, 500000);
	const int CURVE_COUNT = 19;
	string curveNames[CURVE_COUNT] = { "BRE", "BRI", "CLE", "POR", "GEN", "harmonics", "OPE",
										"reso1amp", "reso1bw", "reso1freq",
										"reso2amp", "reso2bw", "reso2freq",
										"reso3amp", "reso3bw", "reso3freq",
										"reso4amp", "reso4bw", "reso4freq"
										};
	for (int i = 0; i < CURVE_COUNT; i++) {
		BPList* list = sequence.track(0).curve(curveNames[i]);
		list->add(1920, 1 + i);
	}
	Event noteEvent(1920, EventType::NOTE);
	noteEvent.note = 60;
	noteEvent.length(480);
	sequence.track(0).events().add(noteEvent);

	ByteArrayOutputStream stream;
	VSQFileWriter writer;
	writer.write(sequence, stream, 500, "Shift_JIS", false);
	string actualString = stream.toString();
	stream.close();

	// 期待値と比較する
	vector<char> expected;
	string fixturePath = "VSQFileWriterTest/expected/expected.vsq";
	FILE* fileHandle = fopen(fixturePath.c_str(), "rb");
	if (! fileHandle) {
		GTEST_FAIL(); // fixture を読み込めなかった.
	}
	while (1) {
		int c = fgetc(fileHandle);
		if (c == EOF) { break; }
		expected.push_back((char)c);
	}
	fclose(fileHandle);

	vector<char> actual;
	actual.insert(actual.begin(), actualString.begin(), actualString.end());
	EXPECT_TRUE(expected == actual);
}

/**
 * @todo
 */
TEST(VSQFileWriterTest, testWriteWithPitch)
{
	//    fail();
}

TEST(VSQFileWriterTest, test_getMidiEventsFromMetaText)
{
	TextStream stream;
	// 「あ」が Shift_JIS になった時分割される「あ」を Shift_JIS にすると「0x82 0xA0」
	stream.write(StringUtil::repeat("a", 118) + "あ");
	stream.write(StringUtil::repeat("b", 63));
	VSQFileWriter writer;
	vector<MidiEvent> events = writer._getMidiEventsFromMetaText(&stream, "Shift_JIS");
	EXPECT_EQ((size_t)2, events.size());

	EXPECT_EQ((tick_t)0, events[0].tick);
	EXPECT_EQ(0xFF, events[0].firstByte);
	EXPECT_EQ((size_t)128, events[0].data.size());
	EXPECT_EQ(0x01, events[0].data[0]);
	ostringstream actual;
	for (int i = 1; i < events[0].data.size(); i++) {
		actual << (char)events[0].data[i];
	}
	EXPECT_EQ(string("DM:0000:") + StringUtil::repeat("a", 118) + (char)0x82, actual.str());

	EXPECT_EQ((tick_t)0, events[1].tick);
	EXPECT_EQ(0xFF, events[1].firstByte);
	EXPECT_EQ((size_t)73, events[1].data.size());
	EXPECT_EQ(0x01, events[1].data[0]);
	actual.str("");
	for (int i = 1; i < events[1].data.size(); i++) {
		actual << (char)events[1].data[i];
	}
	EXPECT_EQ(string("DM:0001:") + (char)0xA0 + StringUtil::repeat("b", 63), actual.str());
}

TEST(VSQFileWriterTest, test_getLinePrefixBytes)
{
	VSQFileWriter writer;

	// 4 桁
	vector<int> expected;
	expected.push_back(0xff & 'D');
	expected.push_back(0xff & 'M');
	expected.push_back(0xff & ':');
	expected.push_back(0xff & '0');
	expected.push_back(0xff & '1');
	expected.push_back(0xff & '2');
	expected.push_back(0xff & '3');
	expected.push_back(0xff & ':');
	vector<int> actual = writer._getLinePrefixBytes(123);
	EXPECT_EQ(expected.size(), actual.size());
	for (int i = 0; i < expected.size(); i++) {
		EXPECT_EQ(expected[i], actual[i]);
	}

	// 4 桁
	expected.clear();
	expected.push_back(0xff & 'D');
	expected.push_back(0xff & 'M');
	expected.push_back(0xff & ':');
	expected.push_back(0xff & '9');
	expected.push_back(0xff & '9');
	expected.push_back(0xff & '9');
	expected.push_back(0xff & '9');
	expected.push_back(0xff & ':');
	actual = writer._getLinePrefixBytes(9999);
	EXPECT_EQ(expected.size(), actual.size());
	for (int i = 0; i < expected.size(); i++) {
		EXPECT_EQ(expected[i], actual[i]);
	}

	// 8 桁
	expected.clear();
	expected.push_back(0xff & 'D');
	expected.push_back(0xff & 'M');
	expected.push_back(0xff & ':');
	expected.push_back(0xff & '0');
	expected.push_back(0xff & '0');
	expected.push_back(0xff & '0');
	expected.push_back(0xff & '1');
	expected.push_back(0xff & '2');
	expected.push_back(0xff & '3');
	expected.push_back(0xff & '4');
	expected.push_back(0xff & '5');
	expected.push_back(0xff & ':');
	actual = writer._getLinePrefixBytes(12345);
	EXPECT_EQ(expected.size(), actual.size());
	for (int i = 0; i < expected.size(); i++) {
		EXPECT_EQ(expected[i], actual[i]);
	}

	// 8 桁
	expected.clear();
	expected.push_back(0xff & 'D');
	expected.push_back(0xff & 'M');
	expected.push_back(0xff & ':');
	expected.push_back(0xff & '0');
	expected.push_back(0xff & '1');
	expected.push_back(0xff & '2');
	expected.push_back(0xff & '3');
	expected.push_back(0xff & '4');
	expected.push_back(0xff & '5');
	expected.push_back(0xff & '6');
	expected.push_back(0xff & '7');
	expected.push_back(0xff & ':');
	actual = writer._getLinePrefixBytes(1234567);
	EXPECT_EQ(expected.size(), actual.size());
	for (int i = 0; i < expected.size(); i++) {
		EXPECT_EQ(expected[i], actual[i]);
	}
}

TEST(VSQFileWriterTest, test_getHowManyDigits)
{
	VSQFileWriter writer;
	EXPECT_EQ(1, writer._getHowManyDigits(0));
	EXPECT_EQ(1, writer._getHowManyDigits(9));
	EXPECT_EQ(2, writer._getHowManyDigits(99));
	EXPECT_EQ(10, writer._getHowManyDigits(1000000000));
	EXPECT_EQ(2, writer._getHowManyDigits(-10));
}

TEST(VSQFileWriterTest, test_writeUnsignedShort)
{
	ByteArrayOutputStream stream;
	VSQFileWriter writer;
	writer._writeUnsignedShort(stream, 0x8421);
	string actual = stream.toString();
	ostringstream expected;
	expected << (char)0x84 << (char)0x21;
	EXPECT_EQ(expected.str(), actual);
}

TEST(VSQFileWriterTest, test_writeUnsignedInt)
{
	ByteArrayOutputStream stream;
	VSQFileWriter writer;
	writer._writeUnsignedInt(stream, 0x84212184);
	string actual = stream.toString();
	ostringstream expected;
	expected << (char)0x84 << (char)0x21 << (char)0x21 << (char)0x84;
	EXPECT_EQ(expected.str(), actual);
}

TEST(VSQFileWriterTest, testPrintTrackMetaText)
{
	Track track("DummyTrackName", "DummySingerName");

	Event singerEvent(0, EventType::SINGER);
	singerEvent.singerHandle = Handle(HandleType::SINGER);   //h#0000
	singerEvent.singerHandle.iconId = "$07010002";
	singerEvent.singerHandle.ids = "Miku";
	singerEvent.singerHandle.original = 1;
	singerEvent.singerHandle.caption = "caption for miku";
	singerEvent.singerHandle.language = 1;
	singerEvent.singerHandle.program = 2;
	track.events().set(0, singerEvent);

	Event crescendoEvent(240, EventType::ICON);
	crescendoEvent.note = 64;
	crescendoEvent.iconDynamicsHandle = Handle(HandleType::DYNAMICS);   //h#0001
	crescendoEvent.iconDynamicsHandle.iconId = "$05020001";
	crescendoEvent.iconDynamicsHandle.ids = "crescendo";
	crescendoEvent.iconDynamicsHandle.original = 1;
	crescendoEvent.iconDynamicsHandle.caption = "caption for crescendo";
	crescendoEvent.iconDynamicsHandle.startDyn = 4;
	crescendoEvent.iconDynamicsHandle.endDyn = 7;
	crescendoEvent.length(10);
	track.events().add(crescendoEvent, 2);

	Event dynaffEvent(480, EventType::ICON);
	dynaffEvent.note = 65;
	dynaffEvent.iconDynamicsHandle = Handle(HandleType::DYNAMICS);  //h#0002
	dynaffEvent.iconDynamicsHandle.iconId = "$05010001";
	dynaffEvent.iconDynamicsHandle.ids = "dynaff";
	dynaffEvent.iconDynamicsHandle.original = 2;
	dynaffEvent.iconDynamicsHandle.caption = "caption for dynaff";
	dynaffEvent.iconDynamicsHandle.startDyn = 5;
	dynaffEvent.iconDynamicsHandle.endDyn = 8;
	dynaffEvent.length(11);
	track.events().add(dynaffEvent, 3);

	Event decrescendoEvent(720, EventType::ICON);
	decrescendoEvent.note = 66;
	decrescendoEvent.iconDynamicsHandle = Handle(HandleType::DYNAMICS);  //h#0003
	decrescendoEvent.iconDynamicsHandle.iconId = "$05030001";
	decrescendoEvent.iconDynamicsHandle.ids = "decrescendo";
	decrescendoEvent.iconDynamicsHandle.original = 3;
	decrescendoEvent.iconDynamicsHandle.caption = "caption for decrescendo";
	decrescendoEvent.iconDynamicsHandle.startDyn = 6;
	decrescendoEvent.iconDynamicsHandle.endDyn = 9;
	decrescendoEvent.length(12);
	track.events().add(decrescendoEvent, 4);

	Event singerEvent2(1920, EventType::SINGER);
	singerEvent2.singerHandle = Handle(HandleType::SINGER);  //h#0004
	singerEvent2.singerHandle.iconId = "$07020003";
	singerEvent2.singerHandle.ids = "Luka_EN";
	singerEvent2.singerHandle.original = 0x82;
	singerEvent2.singerHandle.caption = "caption for luka";
	singerEvent2.singerHandle.language = 2;
	singerEvent2.singerHandle.program = 3;
	track.events().add(singerEvent2, 5);

	Event noteEvent(1920, EventType::NOTE);
	noteEvent.note = 67;
	noteEvent.dynamics = 68;
	noteEvent.pmBendDepth = 69;
	noteEvent.pmBendLength = 70;
	noteEvent.pmbPortamentoUse = 3;
	noteEvent.demDecGainRate = 71;
	noteEvent.demAccent = 72;
	noteEvent.length(480);
	noteEvent.lyricHandle = Handle(HandleType::LYRIC);
	noteEvent.lyricHandle.set(0, Lyric("ら", "4 a"));    //h#0005
	noteEvent.vibratoHandle = Handle(HandleType::VIBRATO);  //h#0006
	noteEvent.vibratoDelay = 73;
	noteEvent.vibratoHandle.iconId = "$04040004";
	noteEvent.vibratoHandle.ids = "vibrato";
	noteEvent.vibratoHandle.original = 1;
	noteEvent.vibratoHandle.caption = "caption for vibrato";
	noteEvent.vibratoHandle.length(407);
	noteEvent.vibratoHandle.startDepth = 13;
	noteEvent.vibratoHandle.startRate = 14;
	noteEvent.noteHeadHandle = Handle(HandleType::NOTE_HEAD);  //h#0007
	noteEvent.noteHeadHandle.iconId = "$05030000";
	noteEvent.noteHeadHandle.ids = "attack";
	noteEvent.noteHeadHandle.original = 15;
	noteEvent.noteHeadHandle.caption = "caption for attack";
	noteEvent.noteHeadHandle.length(120);
	noteEvent.noteHeadHandle.duration = 62;
	noteEvent.noteHeadHandle.depth = 65;
	track.events().add(noteEvent, 6);

	Master master;
	master.preMeasure = 1;

	Mixer mixer;
	mixer.masterFeder = 1;
	mixer.masterPanpot = 2;
	mixer.masterMute = 3;
	mixer.outputMode = 4;
	mixer.slave.push_back(MixerItem(5, 6, 7, 8));

	track.common().version = "DSB301";
	track.common().name = "foo";
	track.common().color = "1,2,3";
	track.common().dynamicsMode = DynamicsMode::STANDARD;
	track.common().playMode(PlayMode::PLAY_WITH_SYNTH);

	vector<string> curves;
	curves.push_back("pit");
	curves.push_back("pbs");
	curves.push_back("dyn");
	curves.push_back("bre");
	curves.push_back("bri");
	curves.push_back("cle");
	curves.push_back("RESO1FREQ");
	curves.push_back("RESO2FREQ");
	curves.push_back("RESO3FREQ");
	curves.push_back("RESO4FREQ");
	curves.push_back("RESO1BW");
	curves.push_back("RESO2BW");
	curves.push_back("RESO3BW");
	curves.push_back("RESO4BW");
	curves.push_back("RESO1amp");
	curves.push_back("RESO2amp");
	curves.push_back("RESO3amp");
	curves.push_back("RESO4amp");
	curves.push_back("HARMONICS");
	curves.push_back("fx2depth");
	curves.push_back("GEN");
	curves.push_back("pOr");
	curves.push_back("OPE");
	for (int i = 0; i < curves.size(); i++) {
		string curveName = curves[i];
		track.curve(curveName)->add(480 + i, i);
	}

	TextStream stream;
	VSQFileWriter writer;
	writer._printMetaText(track, stream, 2400, 0, false, &master, &mixer);
	string expected =
		"[Common]\n"
		"Version=DSB301\n"
		"Name=foo\n"
		"Color=1,2,3\n"
		"DynamicsMode=0\n"
		"PlayMode=1\n"

		"[Master]\n"
		"PreMeasure=1\n"

		"[Mixer]\n"
		"MasterFeder=1\n"
		"MasterPanpot=2\n"
		"MasterMute=3\n"
		"OutputMode=4\n"
		"Tracks=1\n"
		"Feder0=5\n"
		"Panpot0=6\n"
		"Mute0=7\n"
		"Solo0=8\n"

		"[EventList]\n"
		"0=ID#0000\n"
		"240=ID#0001\n"
		"480=ID#0002\n"
		"720=ID#0003\n"
		"1920=ID#0004,ID#0005\n"
		"2400=EOS\n"

		"[ID#0000]\n"
		"Type=Singer\n"
		"IconHandle=h#0000\n"

		"[ID#0001]\n"
		"Type=Aicon\n"
		"IconHandle=h#0001\n"
		"Note#=64\n"

		"[ID#0002]\n"
		"Type=Aicon\n"
		"IconHandle=h#0002\n"
		"Note#=65\n"

		"[ID#0003]\n"
		"Type=Aicon\n"
		"IconHandle=h#0003\n"
		"Note#=66\n"

		"[ID#0004]\n"
		"Type=Singer\n"
		"IconHandle=h#0004\n"

		"[ID#0005]\n"
		"Type=Anote\n"
		"Length=480\n"
		"Note#=67\n"
		"Dynamics=68\n"
		"PMBendDepth=69\n"
		"PMBendLength=70\n"
		"PMbPortamentoUse=3\n"
		"DEMdecGainRate=71\n"
		"DEMaccent=72\n"
		"LyricHandle=h#0005\n"
		"VibratoHandle=h#0006\n"
		"VibratoDelay=73\n"
		"NoteHeadHandle=h#0007\n"

		"[h#0000]\n"
		"IconID=$07010002\n"
		"IDS=Miku\n"
		"Original=1\n"
		"Caption=caption for miku\n"
		"Length=0\n"
		"Language=1\n"
		"Program=2\n"

		"[h#0001]\n"
		"IconID=$05020001\n"
		"IDS=crescendo\n"
		"Original=1\n"
		"Caption=caption for crescendo\n"
		"StartDyn=4\n"
		"EndDyn=7\n"
		"Length=10\n"
		"DynBPNum=0\n"

		"[h#0002]\n"
		"IconID=$05010001\n"
		"IDS=dynaff\n"
		"Original=2\n"
		"Caption=caption for dynaff\n"
		"StartDyn=5\n"
		"EndDyn=8\n"
		"Length=11\n"
		"DynBPNum=0\n"

		"[h#0003]\n"
		"IconID=$05030001\n"
		"IDS=decrescendo\n"
		"Original=3\n"
		"Caption=caption for decrescendo\n"
		"StartDyn=6\n"
		"EndDyn=9\n"
		"Length=12\n"
		"DynBPNum=0\n"

		"[h#0004]\n"
		"IconID=$07020003\n"
		"IDS=Luka_EN\n"
		"Original=130\n"
		"Caption=caption for luka\n"
		"Length=0\n"
		"Language=2\n"
		"Program=3\n"

		"[h#0005]\n"
		"L0=\"ら\",\"4 a\",1,64,0,0\n"

		"[h#0006]\n"
		"IconID=$04040004\n"
		"IDS=vibrato\n"
		"Original=1\n"
		"Caption=caption for vibrato\n"
		"Length=407\n"
		"StartDepth=13\n"
		"DepthBPNum=0\n"
		"StartRate=14\n"
		"RateBPNum=0\n"

		"[h#0007]\n"
		"IconID=$05030000\n"
		"IDS=attack\n"
		"Original=15\n"
		"Caption=caption for attack\n"
		"Length=120\n"
		"Duration=62\n"
		"Depth=65\n"

		"[PitchBendBPList]\n"
		"480=0\n"

		"[PitchBendSensBPList]\n"
		"481=1\n"

		"[DynamicsBPList]\n"
		"482=2\n"

		"[EpRResidualBPList]\n"
		"483=3\n"

		"[EpRESlopeBPList]\n"
		"484=4\n"

		"[EpRESlopeDepthBPList]\n"
		"485=5\n"

		"[GenderFactorBPList]\n"
		"500=20\n"

		"[PortamentoTimingBPList]\n"
		"501=21\n"

		"[OpeningBPList]\n"
		"502=22\n";

	EXPECT_EQ(expected, stream.toString());
}

TEST(VSQFileWriterTest, testWriteEventNote)
{
	std::unique_ptr<Event> event(new Event(0, EventType::NOTE));
	event->length(2);
	event->note = 6;
	event->dynamics = 21;
	event->pmBendDepth = 4;
	event->pmBendLength = 5;
	event->pmbPortamentoUse = 3;
	event->demDecGainRate = 7;
	event->demAccent = 8;
	event->vibratoDelay = 13;
	event->lyricHandle.index = 1;
	event->tick = 20;
	int event_index = 1;

	TextStream stream;

	// handleがどれもnilな音符イベント
	VSQFileWriter writer;
	writer._writeEvent(event.get(), event_index, stream);
	string expected =
		"[ID#0001]\n"
		"Type=Anote\n"
		"Length=2\n"
		"Note#=6\n"
		"Dynamics=21\n"
		"PMBendDepth=4\n"
		"PMBendLength=5\n"
		"PMbPortamentoUse=3\n"
		"DEMdecGainRate=7\n"
		"DEMaccent=8\n"
		"LyricHandle=h#0001\n";
	EXPECT_EQ(expected, stream.toString());

	// handleに全部値が入っている音符イベント
	// 現在, PreUtteranceとVoiceOverlapは扱わないようにしているので,
	// オプション全指定と, オプションが無い場合の動作が全くおなじになってしまっている.
	// ustEventをちゃんと処理するようになったら, TODOコメントのところを外すこと
	event->lyricHandle = Handle(HandleType::LYRIC);
	event->lyricHandle.set(0, Lyric("わ", "w a"));
	event->lyricHandle.index = 11;
	event->vibratoHandle = Handle(HandleType::VIBRATO);
	event->vibratoHandle.index = 12;
	event->noteHeadHandle = Handle(HandleType::NOTE_HEAD);
	event->noteHeadHandle.index = 14;
	stream = TextStream();
	writer._writeEvent(event.get(), event_index, stream);
	expected =
		"[ID#0001]\n"
		"Type=Anote\n"
		"Length=2\n"
		"Note#=6\n"
		"Dynamics=21\n"
		"PMBendDepth=4\n"
		"PMBendLength=5\n"
		"PMbPortamentoUse=3\n"
		"DEMdecGainRate=7\n"
		"DEMaccent=8\n"
		"LyricHandle=h#0011\n"
		"VibratoHandle=h#0012\n"
		"VibratoDelay=13\n"
		"NoteHeadHandle=h#0014\n";
	EXPECT_EQ(expected, stream.toString());
}

TEST(VSQFileWriterTest, testWriteEventSinger)
{
	std::unique_ptr<Event> event(new Event(0, EventType::SINGER));
	event->singerHandle = Handle(HandleType::SINGER);
	event->singerHandle.index = 16;
	event->tick = 1;
	int event_index = 15;
	TextStream stream;
	VSQFileWriter writer;
	writer._writeEvent(event.get(), event_index, stream);
	string expected =
		"[ID#0015]\n"
		"Type=Singer\n"
		"IconHandle=h#0016\n";
	EXPECT_EQ(expected, stream.toString());
}

TEST(VSQFileWriterTest, testWriteEventIcon)
{
	std::unique_ptr<Event> event(new Event(0, EventType::ICON));
	event->note = 19;
	event->iconDynamicsHandle = Handle(HandleType::DYNAMICS);
	event->iconDynamicsHandle.index = 18;
	event->tick = 2;
	int event_index = 17;
	TextStream stream;
	VSQFileWriter writer;
	writer._writeEvent(event.get(), event_index, stream);
	string expected =
		"[ID#0017]\n"
		"Type=Aicon\n"
		"IconHandle=h#0018\n"
		"Note#=19\n";
	EXPECT_EQ(expected, stream.toString());
}

TEST(VSQFileWriterTest, testWriteHandleLyric)
{
	Handle handle = getLyricHandle();

	TextStream stream;
	VSQFileWriter writer;
	writer._writeHandle(handle, stream);
	string expected =
		"[h#0001]" + getLF() +
		"L0=\"あ\",\"a\",0.4,0,1" + getLF() +
		"L1=\"は\",\"h a\",0.6,64,0,0" + getLF();
	EXPECT_EQ(expected, stream.toString());
}

TEST(VSQFileWriterTest, testWriteHandleVibrato)
{
	string lf = getLF();
	Handle handle = getVibratoHandle();
	{
		TextStream stream;
		VSQFileWriter writer;
		writer._writeHandle(handle, stream);
		string expected =
			"[h#0001]" + lf +
			"IconID=$04040004" + lf +
			"IDS=normal-da-yo" + lf +
			"Original=5" + lf +
			"Caption=キャプションです=あ" + lf +
			"Length=120" + lf +
			"StartDepth=64" + lf +
			"DepthBPNum=3" + lf +
			"DepthBPX=0.500000,0.750000,1.000000" + lf +
			"DepthBPY=64,32,0" + lf +
			"StartRate=64" + lf +
			"RateBPNum=3" + lf +
			"RateBPX=0.500000,0.750000,1.000000" + lf +
			"RateBPY=64,32,0" + lf;
		EXPECT_EQ(expected, stream.toString());
	}

	{
		handle.rateBP = VibratoBPList(vector<double>(), vector<int>());
		handle.depthBP = VibratoBPList(vector<double>(), vector<int>());
		TextStream stream;
		VSQFileWriter writer;
		writer._writeHandle(handle, stream);
		string expected =
			"[h#0001]" + lf +
			"IconID=$04040004" + lf +
			"IDS=normal-da-yo" + lf +
			"Original=5" + lf +
			"Caption=キャプションです=あ" + lf +
			"Length=120" + lf +
			"StartDepth=64" + lf +
			"DepthBPNum=0" + lf +
			"StartRate=64" + lf +
			"RateBPNum=0" + lf;
		EXPECT_EQ(expected, stream.toString());
	}
}

TEST(VSQFileWriterTest, testWriteHandleSinger)
{
	Handle handle = getSingerHandle();
	TextStream stream;
	VSQFileWriter writer;
	writer._writeHandle(handle, stream);
	string lf = getLF();
	string expected =
		"[h#0002]" + lf +
		"IconID=$07010002" + lf +
		"IDS=Miku3=God" + lf +
		"Original=2" + lf +
		"Caption=" + lf +
		"Length=1" + lf +
		"Language=1" + lf +
		"Program=2" + lf;
	EXPECT_EQ(expected, stream.toString());
}

TEST(VSQFileWriterTest, testWriteHandleAttack)
{
	Handle handle = getAttackHandle();
	TextStream stream;
	VSQFileWriter writer;
	writer._writeHandle(handle, stream);
	string lf = getLF();
	string expected =
		"[h#0003]" + lf +
		"IconID=$01010002" + lf +
		"IDS=accent" + lf +
		"Original=2" + lf +
		"Caption=Accent" + lf +
		"Length=120" + lf +
		"Duration=64" + lf +
		"Depth=63" + lf;
	EXPECT_EQ(expected, stream.toString());
}

TEST(VSQFileWriterTest, testWriteHandleCrescendo)
{
	string lf = getLF();
	Handle handle = getCrescendoHandle();
	{
		TextStream stream;
		VSQFileWriter writer;
		writer._writeHandle(handle, stream);
		string expected =
			"[h#0004]" + lf +
			"IconID=$05020001" + lf +
			"IDS=Crescendo" + lf +
			"Original=4" + lf +
			"Caption=Zero Crescendo Curve" + lf +
			"StartDyn=2" + lf +
			"EndDyn=38" + lf +
			"Length=960" + lf +
			"DynBPNum=1" + lf +
			"DynBPX=0.500000" + lf +
			"DynBPY=11" + lf;
		EXPECT_EQ(expected, stream.toString());
	}

	{
		// dynBPのデータ点が複数
		handle.dynBP = VibratoBPList("2", "0.4,0.8", "1,2");
		TextStream stream;
		VSQFileWriter writer;
		writer._writeHandle(handle, stream);
		string expected =
			"[h#0004]" + lf +
			"IconID=$05020001" + lf +
			"IDS=Crescendo" + lf +
			"Original=4" + lf +
			"Caption=Zero Crescendo Curve" + lf +
			"StartDyn=2" + lf +
			"EndDyn=38" + lf +
			"Length=960" + lf +
			"DynBPNum=2" + lf +
			"DynBPX=0.400000,0.800000" + lf +
			"DynBPY=1,2" + lf;
		EXPECT_EQ(expected, stream.toString());
	}

	{
		// dynBPのデータ点が 0 個
		handle.dynBP = VibratoBPList(vector<double>(), vector<int>());
		TextStream stream;
		VSQFileWriter writer;
		writer._writeHandle(handle, stream);
		string expected =
			"[h#0004]" + lf +
			"IconID=$05020001" + lf +
			"IDS=Crescendo" + lf +
			"Original=4" + lf +
			"Caption=Zero Crescendo Curve" + lf +
			"StartDyn=2" + lf +
			"EndDyn=38" + lf +
			"Length=960" + lf +
			"DynBPNum=0" + lf;
		EXPECT_EQ(expected, stream.toString());
	}

	{
		// dynBPがから
		handle.dynBP = VibratoBPList(vector<double>(), vector<int>());
		TextStream stream;
		VSQFileWriter writer;
		writer._writeHandle(handle, stream);
		string expected =
			"[h#0004]" + lf +
			"IconID=$05020001" + lf +
			"IDS=Crescendo" + lf +
			"Original=4" + lf +
			"Caption=Zero Crescendo Curve" + lf +
			"StartDyn=2" + lf +
			"EndDyn=38" + lf +
			"Length=960" + lf +
			"DynBPNum=0" + lf;
		EXPECT_EQ(expected, stream.toString());
	}
}
