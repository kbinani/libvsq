#include "Util.hpp"
#include "../include/libvsq/VSQFileReader.hpp"
#include "../include/libvsq/FileInputStream.hpp"
#include "../include/libvsq/TextStream.hpp"
#include "../include/libvsq/Sequence.hpp"

using namespace std;
using namespace vsq;

static void getLyricStream(TextStream& stream)
{
	stream.writeLine("L0=あ,a,0.4,0,1");
	stream.writeLine("L1=は,h a,0.6,64,0,0");
	stream.setPointer(-1);
}

static void getVibratoStream(TextStream& stream)
{
	stream.writeLine("IconID=$04040004");
	stream.writeLine("IDS=normal-da-yo");
	stream.writeLine("Caption=キャプションです=あ");
	stream.writeLine("Original=5");
	stream.writeLine("Length=120");
	stream.writeLine("StartDepth=64");
	stream.writeLine("DepthBPNum=3");
	stream.writeLine("DepthBPX=0.500000,0.750000,1.000000");
	stream.writeLine("DepthBPY=64,32,0");
	stream.writeLine("StartRate=64");
	stream.writeLine("RateBPNum=3");
	stream.writeLine("RateBPX=0.500000,0.750000,1.000000");
	stream.writeLine("RateBPY=64,32,0");
	stream.writeLine("[h#0002]");
	stream.setPointer(-1);
}

static void getSingerStream(TextStream& stream)
{
	stream.writeLine("IconID=$07010002");
	stream.writeLine("IDS=Miku3=God");
	stream.writeLine("Original=2");
	stream.writeLine("Caption=");
	stream.writeLine("Length=1");
	stream.writeLine("Language=1");
	stream.writeLine("Program=2");
	stream.setPointer(-1);
}

static void getAttackStream(TextStream& stream)
{
	stream.writeLine("IconID=$01010002");
	stream.writeLine("IDS=accent");
	stream.writeLine("Original=2");
	stream.writeLine("Caption=Accent");
	stream.writeLine("Length=120");
	stream.writeLine("Duration=64");
	stream.writeLine("Depth=63");
	stream.setPointer(-1);
}

static void getCrescendoStream(TextStream& stream)
{
	stream.writeLine("IconID=$05020001");
	stream.writeLine("IDS=Crescendo");
	stream.writeLine("Original=4");
	stream.writeLine("Caption=Zero Crescendo Curve");
	stream.writeLine("Length=960");
	stream.writeLine("StartDyn=2");
	stream.writeLine("EndDyn=38");
	stream.writeLine("DynBPNum=1");
	stream.writeLine("DynBPX=0.5");
	stream.writeLine("DynBPY=11");
	stream.setPointer(-1);
}

TEST(VSQFileReaderTest, testRead)
{
	Sequence sequence("singer", 1, 4, 4, 500000);
	VSQFileReader reader;
	FileInputStream stream("VSQFileReaderTest/fixture/fixture.vsq");
	reader.read(sequence, stream, "Shift_JIS");
	stream.close();

	// シーケンス全体
	EXPECT_EQ((size_t)1, sequence.tracks().size());
	// [Master]
	EXPECT_EQ(1, sequence.master.preMeasure);
	// [Mixer]
	EXPECT_EQ(2, sequence.mixer.masterFeder);
	EXPECT_EQ(3, sequence.mixer.masterPanpot);
	EXPECT_EQ(1, sequence.mixer.masterMute);
	EXPECT_EQ(4, sequence.mixer.outputMode);
	EXPECT_EQ((size_t)1, sequence.mixer.slave.size());
	EXPECT_EQ(0, sequence.mixer.slave[0].feder);
	EXPECT_EQ(0, sequence.mixer.slave[0].panpot);
	EXPECT_EQ(0, sequence.mixer.slave[0].mute);
	EXPECT_EQ(0, sequence.mixer.slave[0].solo);

	// トラック1
	Track const& track1 = sequence.track(0);
	// [Common]
	EXPECT_EQ(string("DSB301"), track1.common().version);
	EXPECT_EQ(string("Voice1"), track1.common().name);
	EXPECT_EQ(string("179,181,123"), track1.common().color);
	EXPECT_EQ(DynamicsMode::EXPERT, track1.common().dynamicsMode);
	EXPECT_EQ(PlayMode::PLAY_WITH_SYNTH, track1.common().playMode());
	{
		// イベントリスト
		EXPECT_EQ(2, track1.events().size());
		{
			// イベント/歌手変更
			const Event* item = track1.events().get(0);
			EXPECT_EQ((tick_t)0, item->tick);
			EXPECT_EQ(EventType::SINGER, item->type());
			EXPECT_TRUE(false == item->isEOS());
			{
				//歌手ハンドルの内容物をテスト
				EXPECT_EQ(HandleType::SINGER, item->singerHandle.type());
				EXPECT_EQ(string("$07010000"), item->singerHandle.iconId);
				EXPECT_EQ(string("Foo"), item->singerHandle.ids);
				EXPECT_EQ(0, item->singerHandle.original);
				EXPECT_EQ(string(""), item->singerHandle.caption);
				EXPECT_EQ((tick_t)1, item->singerHandle.length());
				EXPECT_EQ(0, item->singerHandle.language);
				EXPECT_EQ(0, item->singerHandle.program);
			}
			{
				//歌手ハンドル以外のハンドルがemptyとなっていること
				EXPECT_EQ(HandleType::UNKNOWN, item->iconDynamicsHandle.type());
				EXPECT_EQ(HandleType::UNKNOWN, item->lyricHandle.type());
				EXPECT_EQ(HandleType::UNKNOWN, item->noteHeadHandle.type());
				EXPECT_EQ(HandleType::UNKNOWN, item->vibratoHandle.type());
			}
		}
		{
			// イベント/音符
			const Event* item = track1.events().get(1);
			EXPECT_EQ((tick_t)1920, item->tick);
			EXPECT_EQ(EventType::NOTE, item->type());
			EXPECT_EQ((tick_t)480, item->length());
			EXPECT_EQ(60, item->note);
			EXPECT_EQ(0, item->dynamics);
			EXPECT_EQ(0, item->pmBendDepth);
			EXPECT_EQ(0, item->pmBendLength);
			EXPECT_EQ(0, item->pmbPortamentoUse);
			EXPECT_EQ(0, item->demDecGainRate);
			EXPECT_EQ(0, item->demAccent);
			EXPECT_TRUE(false == item->isEOS());
			{
				// 歌詞ハンドルの内容物が正しいこと
				EXPECT_EQ(HandleType::LYRIC, item->lyricHandle.type());
				EXPECT_EQ(1, item->lyricHandle.size());
				EXPECT_EQ(string("a"), item->lyricHandle.get(0).phrase);
				EXPECT_EQ(string("a"), item->lyricHandle.get(0).phoneticSymbol());
				EXPECT_EQ(1.0, item->lyricHandle.get(0).lengthRatio);
				EXPECT_EQ(string("0"), item->lyricHandle.get(0).consonantAdjustment());
				EXPECT_TRUE(false == item->lyricHandle.get(0).isProtected);
			}
			{
				// 歌詞ハンドル以外のハンドルがemptyとなっていること
				EXPECT_EQ(HandleType::UNKNOWN, item->iconDynamicsHandle.type());
				EXPECT_EQ(HandleType::UNKNOWN, item->singerHandle.type());
				EXPECT_EQ(HandleType::UNKNOWN, item->noteHeadHandle.type());
				EXPECT_EQ(HandleType::UNKNOWN, item->vibratoHandle.type());
			}
		}
	}
	{
		// 各種コントロールカーブ
		const BPList* list = NULL;
		// BRE
		list = track1.curve("BRE");
		EXPECT_EQ(1, list->size());
		EXPECT_EQ((tick_t)1920, list->keyTickAt(0));
		EXPECT_EQ(1, list->get(0).value);
		// BRI
		list = track1.curve("BRI");
		EXPECT_EQ(1, list->size());
		EXPECT_EQ((tick_t)1920, list->keyTickAt(0));
		EXPECT_EQ(2, list->get(0).value);
		// CLE
		list = track1.curve("CLE");
		EXPECT_EQ(1, list->size());
		EXPECT_EQ((tick_t)1920, list->keyTickAt(0));
		EXPECT_EQ(3, list->get(0).value);
		// POR
		list = track1.curve("POR");
		EXPECT_EQ(1, list->size());
		EXPECT_EQ((tick_t)1920, list->keyTickAt(0));
		EXPECT_EQ(4, list->get(0).value);
		// GEN
		list = track1.curve("GEN");
		EXPECT_EQ(1, list->size());
		EXPECT_EQ((tick_t)1920, list->keyTickAt(0));
		EXPECT_EQ(5, list->get(0).value);
		// harmonics
		EXPECT_EQ(0, track1.curve("harmonics")->size());
		// OPE
		list = track1.curve("OPE");
		EXPECT_EQ(1, list->size());
		EXPECT_EQ((tick_t)1920, list->keyTickAt(0));
		EXPECT_EQ(7, list->get(0).value);
		// reso1amp
		EXPECT_EQ(0, track1.curve("reso1amp")->size());
		// reso1bw
		EXPECT_EQ(0, track1.curve("reso1bw")->size());
		// reso1freq
		EXPECT_EQ(0, track1.curve("reso1freq")->size());
		// reso2amp
		EXPECT_EQ(0, track1.curve("reso2amp")->size());
		// reso2bw
		EXPECT_EQ(0, track1.curve("reso2bw")->size());
		// reso2freq
		EXPECT_EQ(0, track1.curve("reso2freq")->size());
		// reso3amp
		EXPECT_EQ(0, track1.curve("reso3amp")->size());
		// reso3bw
		EXPECT_EQ(0, track1.curve("reso3bw")->size());
		// reso3freq
		EXPECT_EQ(0, track1.curve("reso3freq")->size());
		// reso4amp
		EXPECT_EQ(0, track1.curve("reso4amp")->size());
		// reso4bw
		EXPECT_EQ(0, track1.curve("reso4bw")->size());
		// reso4freq
		EXPECT_EQ(0, track1.curve("reso4freq")->size());
	}

	{
		// テンポ変更
		EXPECT_EQ(2, sequence.tempoList.size());
		{
			Tempo tempo = sequence.tempoList.get(0);
			EXPECT_EQ(500000, tempo.tempo);
			EXPECT_EQ((tick_t)0, tempo.tick);
			EXPECT_EQ(0.0, tempo.time());
		}
		{
			Tempo tempo = sequence.tempoList.get(1);
			EXPECT_EQ(250000, tempo.tempo);
			EXPECT_EQ((tick_t)1920, tempo.tick);
			EXPECT_EQ(2.0, tempo.time());
		}
	}

	{
		// 拍子変更
		EXPECT_EQ(2, sequence.timesigList.size());
		{
			Timesig timesig = sequence.timesigList.get(0);
			EXPECT_EQ(0, timesig.barCount);
			EXPECT_EQ(4, timesig.numerator);
			EXPECT_EQ(4, timesig.denominator);
			EXPECT_EQ((tick_t)0, timesig.tick());
		}
		{
			Timesig timesig = sequence.timesigList.get(1);
			EXPECT_EQ(1, timesig.barCount);
			EXPECT_EQ(3, timesig.numerator);
			EXPECT_EQ(4, timesig.denominator);
			EXPECT_EQ((tick_t)1920, timesig.tick());
		}
	}
}

/**
	* 歌詞ハンドルの読み込みテスト
	* EOFで読み込みが終了する場合
	*/
TEST(VSQFileReaderTest, testConstructLyricFromTextStreamStopWithEOF)
{
	TextStream stream;
	getLyricStream(stream);
	string lastLine = "";
	int index = 100;

	VSQFileReader reader;
	Handle handle = reader.parseHandle(stream, index, lastLine);
	EXPECT_EQ(HandleType::LYRIC, handle.type());
	EXPECT_EQ(index, handle.index);
	EXPECT_EQ(2, handle.size());

	Lyric lyric1 = handle.get(0);
	EXPECT_EQ(string("あ"), lyric1.phrase);
	EXPECT_EQ(string("a"), lyric1.phoneticSymbol());
	EXPECT_EQ(0.4, lyric1.lengthRatio);
	EXPECT_EQ(string("0"), lyric1.consonantAdjustment());
	EXPECT_TRUE(lyric1.isProtected);

	Lyric lyric2 = handle.get(1);
	EXPECT_EQ(string("は"), lyric2.phrase);
	EXPECT_EQ(string("h a"), lyric2.phoneticSymbol());
	EXPECT_EQ(0.6, lyric2.lengthRatio);
	EXPECT_EQ(string("64,0"), lyric2.consonantAdjustment());
	EXPECT_TRUE(false == lyric2.isProtected);
}

/**
	* 歌詞ハンドルの読み込みテスト
	* 次の歌詞ハンドルの先頭に到達して読み込みが終了する場合
	*/
TEST(VSQFileReaderTest, testConstructLyricFromTextStreamStopWithNextHandle)
{
	TextStream stream;
	stream.writeLine("L0=あ,a,0.4,0,1");
	stream.writeLine("[h#0002]");
	stream.setPointer(-1);
	string lastLine = "";
	int index = 100;

	VSQFileReader reader;
	Handle handle = reader.parseHandle(stream, index, lastLine);
	EXPECT_EQ(HandleType::LYRIC, handle.type());
	EXPECT_EQ(index, handle.index);
	EXPECT_EQ(1, handle.size());

	EXPECT_EQ(0, handle.rateBP.size());
	EXPECT_EQ(0, handle.depthBP.size());
	EXPECT_EQ(0, handle.dynBP.size());

	Lyric lyric = handle.get(0);
	EXPECT_EQ(string("あ"), lyric.phrase);
	EXPECT_EQ(string("a"), lyric.phoneticSymbol());
	EXPECT_EQ(0.4, lyric.lengthRatio);
	EXPECT_EQ(string("0"), lyric.consonantAdjustment());
	EXPECT_TRUE(lyric.isProtected);

	EXPECT_EQ(string("[h#0002]"), lastLine);
}

TEST(VSQFileReaderTest, testConstructVibratoFromTextStream)
{
	TextStream stream;
	getVibratoStream(stream);
	string lastLine = "";
	int index = 101;
	VSQFileReader reader;
	Handle handle = reader.parseHandle(stream, index, lastLine);

	EXPECT_EQ(HandleType::VIBRATO, handle.type());
	EXPECT_EQ(string("$04040004"), handle.iconId);
	EXPECT_EQ(string("normal-da-yo"), handle.ids);
	EXPECT_EQ(string("キャプションです=あ"), handle.caption);
	EXPECT_EQ(5, handle.original);
	EXPECT_EQ((tick_t)120, handle.length());
	EXPECT_EQ(64, handle.startDepth);
	EXPECT_EQ(string("0.5=64,0.75=32,1=0"), handle.depthBP.data());
	EXPECT_EQ(64, handle.startRate);
	EXPECT_EQ(string("0.5=64,0.75=32,1=0"), handle.rateBP.data());

	EXPECT_EQ(string("[h#0002]"), lastLine);
}

TEST(VSQFileReaderTest, testConstructVibratoFromTextStreamWithoutBP)
{
	TextStream stream;
	stream.writeLine("IconID=$04040004");
	stream.writeLine("IDS=normal-da-yo");
	stream.writeLine("Caption=キャプションです=あ");
	stream.writeLine("Original=5");
	stream.writeLine("Length=120");
	stream.writeLine("StartDepth=64");
	stream.writeLine("StartRate=64");
	stream.writeLine("[h#0002]");
	stream.setPointer(-1);

	string lastLine = "";
	int index = 101;
	VSQFileReader reader;
	Handle handle = reader.parseHandle(stream, index, lastLine);

	EXPECT_EQ(0, handle.rateBP.size());
	EXPECT_EQ(0, handle.depthBP.size());
}

TEST(VSQFileReaderTest, testConstructSingerFromTextStream)
{
	TextStream stream;
	getSingerStream(stream);
	int index = 101;
	string lastLine = "";
	VSQFileReader reader;
	Handle handle = reader.parseHandle(stream, index, lastLine);
	EXPECT_EQ(index, handle.index);
	EXPECT_EQ(HandleType::SINGER, handle.type());
	EXPECT_EQ(string("$07010002"), handle.iconId);
	EXPECT_EQ(string("Miku3=God"), handle.ids);
	EXPECT_EQ(2, handle.original);
	EXPECT_EQ(string(""), handle.caption);
	EXPECT_EQ((tick_t)1, handle.length());
	EXPECT_EQ(1, handle.language);
	EXPECT_EQ(2, handle.program);
}

TEST(VSQFileReaderTest, testConstructAttackFromTextStream)
{
	TextStream stream;
	getAttackStream(stream);
	string lastLine = "";
	int index = 204;
	VSQFileReader reader;
	Handle handle = reader.parseHandle(stream, index, lastLine);
	EXPECT_EQ(HandleType::NOTE_HEAD, handle.type());
	EXPECT_EQ(index, handle.index);
	EXPECT_EQ(string("$01010002"), handle.iconId);
	EXPECT_EQ(string("accent"), handle.ids);
	EXPECT_EQ(2, handle.original);
	EXPECT_EQ(string("Accent"), handle.caption);
	EXPECT_EQ((tick_t)120, handle.length());
	EXPECT_EQ(64, handle.duration);
	EXPECT_EQ(63, handle.depth);
}

TEST(VSQFileReaderTest, testConstructCrescendFromTextStream)
{
	TextStream stream;
	getCrescendoStream(stream);
	string lastLine;
	int index = 204;
	VSQFileReader reader;
	Handle handle = reader.parseHandle(stream, index, lastLine);
	EXPECT_EQ(index, handle.index);
	EXPECT_EQ(HandleType::DYNAMICS, handle.type());
	EXPECT_EQ(string("$05020001"), handle.iconId);
	EXPECT_EQ(string("Crescendo"), handle.ids);
	EXPECT_EQ(4, handle.original);
	EXPECT_EQ(string("Zero Crescendo Curve"), handle.caption);
	EXPECT_EQ((tick_t)960, handle.length());
	EXPECT_EQ(2, handle.startDyn);
	EXPECT_EQ(38, handle.endDyn);
	EXPECT_EQ(string("0.5=11"), handle.dynBP.data());
}

TEST(VSQFileReaderTest, testParseEvent)
{
	TextStream stream;
	stream.writeLine("Type=Anote");
	stream.writeLine("Length=1");
	stream.writeLine("Note#=2");
	stream.writeLine("Dynamics=3");
	stream.writeLine("PMBendDepth=4");
	stream.writeLine("PMBendLength=5");
	stream.writeLine("DEMdecGainRate=6");
	stream.writeLine("DEMaccent=7");
	stream.writeLine("LyricHandle=h#0001");
	stream.writeLine("IconHandle=h#0002");
	stream.writeLine("VibratoHandle=h#0003");
	stream.writeLine("VibratoDelay=8");
	stream.writeLine("PMbPortamentoUse=3");
	stream.writeLine("NoteHeadHandle=h#0004");
	stream.writeLine("[ID#9999]");
	stream.setPointer(-1);
	string lastLine = "";
	VSQFileReader reader;
	int lyricHandleIndex, singerHandleIndex, vibratoHandleIndex, noteHeadHandleIndex;
	EventType type;
	Event event = reader.parseEvent(stream, lastLine, type, lyricHandleIndex, singerHandleIndex, vibratoHandleIndex, noteHeadHandleIndex);

	EXPECT_EQ(EventType::NOTE, type);
	EXPECT_EQ((tick_t)1, event.length());
	EXPECT_EQ(2, event.note);
	EXPECT_EQ(3, event.dynamics);
	EXPECT_EQ(4, event.pmBendDepth);
	EXPECT_EQ(5, event.pmBendLength);
	EXPECT_EQ(6, event.demDecGainRate);
	EXPECT_EQ(7, event.demAccent);
	EXPECT_EQ(1, lyricHandleIndex);
	EXPECT_EQ(2, singerHandleIndex);
	EXPECT_EQ(3, vibratoHandleIndex);
	EXPECT_EQ(4, noteHeadHandleIndex);
}
