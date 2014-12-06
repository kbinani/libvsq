#include "Util.hpp"
#include "../include/libvsq/VSQFileReader.hpp"
#include "../include/libvsq/FileInputStream.hpp"
#include "../include/libvsq/TextStream.hpp"
#include "../include/libvsq/Sequence.hpp"

using namespace std;
using namespace vsq;

class VSQFileReaderTest : public CppUnit::TestCase
{
public:
	void getLyricStream(TextStream& stream)
	{
		stream.writeLine("L0=あ,a,0.4,0,1");
		stream.writeLine("L1=は,h a,0.6,64,0,0");
		stream.setPointer(-1);
	}

	void getVibratoStream(TextStream& stream)
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

	void getSingerStream(TextStream& stream)
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

	void getAttackStream(TextStream& stream)
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

	void getCrescendoStream(TextStream& stream)
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

	void testRead()
	{
		Sequence sequence("singer", 1, 4, 4, 500000);
		VSQFileReader reader;
		FileInputStream stream(TestUtil::getFixtureRootPath() + "/VSQFileReaderTest/fixture/fixture.vsq");
		reader.read(sequence, stream, "Shift_JIS");
		stream.close();

		// シーケンス全体
		CPPUNIT_ASSERT_EQUAL((size_t)1, sequence.tracks().size());
		// [Master]
		CPPUNIT_ASSERT_EQUAL(1, sequence.master.preMeasure);
		// [Mixer]
		CPPUNIT_ASSERT_EQUAL(2, sequence.mixer.masterFeder);
		CPPUNIT_ASSERT_EQUAL(3, sequence.mixer.masterPanpot);
		CPPUNIT_ASSERT_EQUAL(1, sequence.mixer.masterMute);
		CPPUNIT_ASSERT_EQUAL(4, sequence.mixer.outputMode);
		CPPUNIT_ASSERT_EQUAL((size_t)1, sequence.mixer.slave.size());
		CPPUNIT_ASSERT_EQUAL(0, sequence.mixer.slave[0].feder);
		CPPUNIT_ASSERT_EQUAL(0, sequence.mixer.slave[0].panpot);
		CPPUNIT_ASSERT_EQUAL(0, sequence.mixer.slave[0].mute);
		CPPUNIT_ASSERT_EQUAL(0, sequence.mixer.slave[0].solo);

		// トラック1
		Track const& track1 = sequence.track(0);
		// [Common]
		CPPUNIT_ASSERT_EQUAL(string("DSB301"), track1.common().version);
		CPPUNIT_ASSERT_EQUAL(string("Voice1"), track1.common().name);
		CPPUNIT_ASSERT_EQUAL(string("179,181,123"), track1.common().color);
		CPPUNIT_ASSERT_EQUAL(DynamicsMode::EXPERT, track1.common().dynamicsMode);
		CPPUNIT_ASSERT_EQUAL(PlayMode::PLAY_WITH_SYNTH, track1.common().playMode());
		{
			// イベントリスト
			CPPUNIT_ASSERT_EQUAL(2, track1.events().size());
			{
				// イベント/歌手変更
				const Event* item = track1.events().get(0);
				CPPUNIT_ASSERT_EQUAL((tick_t)0, item->tick);
				CPPUNIT_ASSERT_EQUAL(EventType::SINGER, item->type());
				CPPUNIT_ASSERT(false == item->isEOS());
				{
					//歌手ハンドルの内容物をテスト
					CPPUNIT_ASSERT_EQUAL(HandleType::SINGER, item->singerHandle.type());
					CPPUNIT_ASSERT_EQUAL(string("$07010000"), item->singerHandle.iconId);
					CPPUNIT_ASSERT_EQUAL(string("Foo"), item->singerHandle.ids);
					CPPUNIT_ASSERT_EQUAL(0, item->singerHandle.original);
					CPPUNIT_ASSERT_EQUAL(string(""), item->singerHandle.caption);
					CPPUNIT_ASSERT_EQUAL((tick_t)1, item->singerHandle.length());
					CPPUNIT_ASSERT_EQUAL(0, item->singerHandle.language);
					CPPUNIT_ASSERT_EQUAL(0, item->singerHandle.program);
				}
				{
					//歌手ハンドル以外のハンドルがemptyとなっていること
					CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, item->iconDynamicsHandle.type());
					CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, item->lyricHandle.type());
					CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, item->noteHeadHandle.type());
					CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, item->vibratoHandle.type());
				}
			}
			{
				// イベント/音符
				const Event* item = track1.events().get(1);
				CPPUNIT_ASSERT_EQUAL((tick_t)1920, item->tick);
				CPPUNIT_ASSERT_EQUAL(EventType::NOTE, item->type());
				CPPUNIT_ASSERT_EQUAL((tick_t)480, item->length());
				CPPUNIT_ASSERT_EQUAL(60, item->note);
				CPPUNIT_ASSERT_EQUAL(0, item->dynamics);
				CPPUNIT_ASSERT_EQUAL(0, item->pmBendDepth);
				CPPUNIT_ASSERT_EQUAL(0, item->pmBendLength);
				CPPUNIT_ASSERT_EQUAL(0, item->pmbPortamentoUse);
				CPPUNIT_ASSERT_EQUAL(0, item->demDecGainRate);
				CPPUNIT_ASSERT_EQUAL(0, item->demAccent);
				CPPUNIT_ASSERT(false == item->isEOS());
				{
					// 歌詞ハンドルの内容物が正しいこと
					CPPUNIT_ASSERT_EQUAL(HandleType::LYRIC, item->lyricHandle.type());
					CPPUNIT_ASSERT_EQUAL(1, item->lyricHandle.size());
					CPPUNIT_ASSERT_EQUAL(string("a"), item->lyricHandle.get(0).phrase);
					CPPUNIT_ASSERT_EQUAL(string("a"), item->lyricHandle.get(0).phoneticSymbol());
					CPPUNIT_ASSERT_EQUAL(1.0, item->lyricHandle.get(0).lengthRatio);
					CPPUNIT_ASSERT_EQUAL(string("0"), item->lyricHandle.get(0).consonantAdjustment());
					CPPUNIT_ASSERT(false == item->lyricHandle.get(0).isProtected);
				}
				{
					// 歌詞ハンドル以外のハンドルがemptyとなっていること
					CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, item->iconDynamicsHandle.type());
					CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, item->singerHandle.type());
					CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, item->noteHeadHandle.type());
					CPPUNIT_ASSERT_EQUAL(HandleType::UNKNOWN, item->vibratoHandle.type());
				}
			}
		}
		{
			// 各種コントロールカーブ
			const BPList* list = NULL;
			// BRE
			list = track1.curve("BRE");
			CPPUNIT_ASSERT_EQUAL(1, list->size());
			CPPUNIT_ASSERT_EQUAL((tick_t)1920, list->keyTickAt(0));
			CPPUNIT_ASSERT_EQUAL(1, list->get(0).value);
			// BRI
			list = track1.curve("BRI");
			CPPUNIT_ASSERT_EQUAL(1, list->size());
			CPPUNIT_ASSERT_EQUAL((tick_t)1920, list->keyTickAt(0));
			CPPUNIT_ASSERT_EQUAL(2, list->get(0).value);
			// CLE
			list = track1.curve("CLE");
			CPPUNIT_ASSERT_EQUAL(1, list->size());
			CPPUNIT_ASSERT_EQUAL((tick_t)1920, list->keyTickAt(0));
			CPPUNIT_ASSERT_EQUAL(3, list->get(0).value);
			// POR
			list = track1.curve("POR");
			CPPUNIT_ASSERT_EQUAL(1, list->size());
			CPPUNIT_ASSERT_EQUAL((tick_t)1920, list->keyTickAt(0));
			CPPUNIT_ASSERT_EQUAL(4, list->get(0).value);
			// GEN
			list = track1.curve("GEN");
			CPPUNIT_ASSERT_EQUAL(1, list->size());
			CPPUNIT_ASSERT_EQUAL((tick_t)1920, list->keyTickAt(0));
			CPPUNIT_ASSERT_EQUAL(5, list->get(0).value);
			// harmonics
			CPPUNIT_ASSERT_EQUAL(0, track1.curve("harmonics")->size());
			// OPE
			list = track1.curve("OPE");
			CPPUNIT_ASSERT_EQUAL(1, list->size());
			CPPUNIT_ASSERT_EQUAL((tick_t)1920, list->keyTickAt(0));
			CPPUNIT_ASSERT_EQUAL(7, list->get(0).value);
			// reso1amp
			CPPUNIT_ASSERT_EQUAL(0, track1.curve("reso1amp")->size());
			// reso1bw
			CPPUNIT_ASSERT_EQUAL(0, track1.curve("reso1bw")->size());
			// reso1freq
			CPPUNIT_ASSERT_EQUAL(0, track1.curve("reso1freq")->size());
			// reso2amp
			CPPUNIT_ASSERT_EQUAL(0, track1.curve("reso2amp")->size());
			// reso2bw
			CPPUNIT_ASSERT_EQUAL(0, track1.curve("reso2bw")->size());
			// reso2freq
			CPPUNIT_ASSERT_EQUAL(0, track1.curve("reso2freq")->size());
			// reso3amp
			CPPUNIT_ASSERT_EQUAL(0, track1.curve("reso3amp")->size());
			// reso3bw
			CPPUNIT_ASSERT_EQUAL(0, track1.curve("reso3bw")->size());
			// reso3freq
			CPPUNIT_ASSERT_EQUAL(0, track1.curve("reso3freq")->size());
			// reso4amp
			CPPUNIT_ASSERT_EQUAL(0, track1.curve("reso4amp")->size());
			// reso4bw
			CPPUNIT_ASSERT_EQUAL(0, track1.curve("reso4bw")->size());
			// reso4freq
			CPPUNIT_ASSERT_EQUAL(0, track1.curve("reso4freq")->size());
		}

		{
			// テンポ変更
			CPPUNIT_ASSERT_EQUAL(2, sequence.tempoList.size());
			{
				Tempo tempo = sequence.tempoList.get(0);
				CPPUNIT_ASSERT_EQUAL(500000, tempo.tempo);
				CPPUNIT_ASSERT_EQUAL((tick_t)0, tempo.tick);
				CPPUNIT_ASSERT_EQUAL(0.0, tempo.time());
			}
			{
				Tempo tempo = sequence.tempoList.get(1);
				CPPUNIT_ASSERT_EQUAL(250000, tempo.tempo);
				CPPUNIT_ASSERT_EQUAL((tick_t)1920, tempo.tick);
				CPPUNIT_ASSERT_EQUAL(2.0, tempo.time());
			}
		}

		{
			// 拍子変更
			CPPUNIT_ASSERT_EQUAL(2, sequence.timesigList.size());
			{
				Timesig timesig = sequence.timesigList.get(0);
				CPPUNIT_ASSERT_EQUAL(0, timesig.barCount);
				CPPUNIT_ASSERT_EQUAL(4, timesig.numerator);
				CPPUNIT_ASSERT_EQUAL(4, timesig.denominator);
				CPPUNIT_ASSERT_EQUAL((tick_t)0, timesig.tick());
			}
			{
				Timesig timesig = sequence.timesigList.get(1);
				CPPUNIT_ASSERT_EQUAL(1, timesig.barCount);
				CPPUNIT_ASSERT_EQUAL(3, timesig.numerator);
				CPPUNIT_ASSERT_EQUAL(4, timesig.denominator);
				CPPUNIT_ASSERT_EQUAL((tick_t)1920, timesig.tick());
			}
		}
	}

	/**
	 * 歌詞ハンドルの読み込みテスト
	 * EOFで読み込みが終了する場合
	 */
	void testConstructLyricFromTextStreamStopWithEOF()
	{
		TextStream stream;
		getLyricStream(stream);
		string lastLine = "";
		int index = 100;

		VSQFileReader reader;
		Handle handle = reader.parseHandle(stream, index, lastLine);
		CPPUNIT_ASSERT_EQUAL(HandleType::LYRIC, handle.type());
		CPPUNIT_ASSERT_EQUAL(index, handle.index);
		CPPUNIT_ASSERT_EQUAL(2, handle.size());

		Lyric lyric1 = handle.get(0);
		CPPUNIT_ASSERT_EQUAL(string("あ"), lyric1.phrase);
		CPPUNIT_ASSERT_EQUAL(string("a"), lyric1.phoneticSymbol());
		CPPUNIT_ASSERT_EQUAL(0.4, lyric1.lengthRatio);
		CPPUNIT_ASSERT_EQUAL(string("0"), lyric1.consonantAdjustment());
		CPPUNIT_ASSERT(lyric1.isProtected);

		Lyric lyric2 = handle.get(1);
		CPPUNIT_ASSERT_EQUAL(string("は"), lyric2.phrase);
		CPPUNIT_ASSERT_EQUAL(string("h a"), lyric2.phoneticSymbol());
		CPPUNIT_ASSERT_EQUAL(0.6, lyric2.lengthRatio);
		CPPUNIT_ASSERT_EQUAL(string("64,0"), lyric2.consonantAdjustment());
		CPPUNIT_ASSERT(false == lyric2.isProtected);
	}

	/**
	 * 歌詞ハンドルの読み込みテスト
	 * 次の歌詞ハンドルの先頭に到達して読み込みが終了する場合
	 */
	void testConstructLyricFromTextStreamStopWithNextHandle()
	{
		TextStream stream;
		stream.writeLine("L0=あ,a,0.4,0,1");
		stream.writeLine("[h#0002]");
		stream.setPointer(-1);
		string lastLine = "";
		int index = 100;

		VSQFileReader reader;
		Handle handle = reader.parseHandle(stream, index, lastLine);
		CPPUNIT_ASSERT_EQUAL(HandleType::LYRIC, handle.type());
		CPPUNIT_ASSERT_EQUAL(index, handle.index);
		CPPUNIT_ASSERT_EQUAL(1, handle.size());

		CPPUNIT_ASSERT_EQUAL(0, handle.rateBP.size());
		CPPUNIT_ASSERT_EQUAL(0, handle.depthBP.size());
		CPPUNIT_ASSERT_EQUAL(0, handle.dynBP.size());

		Lyric lyric = handle.get(0);
		CPPUNIT_ASSERT_EQUAL(string("あ"), lyric.phrase);
		CPPUNIT_ASSERT_EQUAL(string("a"), lyric.phoneticSymbol());
		CPPUNIT_ASSERT_EQUAL(0.4, lyric.lengthRatio);
		CPPUNIT_ASSERT_EQUAL(string("0"), lyric.consonantAdjustment());
		CPPUNIT_ASSERT(lyric.isProtected);

		CPPUNIT_ASSERT_EQUAL(string("[h#0002]"), lastLine);
	}

	void testConstructVibratoFromTextStream()
	{
		TextStream stream;
		getVibratoStream(stream);
		string lastLine = "";
		int index = 101;
		VSQFileReader reader;
		Handle handle = reader.parseHandle(stream, index, lastLine);

		CPPUNIT_ASSERT_EQUAL(HandleType::VIBRATO, handle.type());
		CPPUNIT_ASSERT_EQUAL(string("$04040004"), handle.iconId);
		CPPUNIT_ASSERT_EQUAL(string("normal-da-yo"), handle.ids);
		CPPUNIT_ASSERT_EQUAL(string("キャプションです=あ"), handle.caption);
		CPPUNIT_ASSERT_EQUAL(5, handle.original);
		CPPUNIT_ASSERT_EQUAL((tick_t)120, handle.length());
		CPPUNIT_ASSERT_EQUAL(64, handle.startDepth);
		CPPUNIT_ASSERT_EQUAL(string("0.5=64,0.75=32,1=0"), handle.depthBP.data());
		CPPUNIT_ASSERT_EQUAL(64, handle.startRate);
		CPPUNIT_ASSERT_EQUAL(string("0.5=64,0.75=32,1=0"), handle.rateBP.data());

		CPPUNIT_ASSERT_EQUAL(string("[h#0002]"), lastLine);
	}

	void testConstructVibratoFromTextStreamWithoutBP()
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

		CPPUNIT_ASSERT_EQUAL(0, handle.rateBP.size());
		CPPUNIT_ASSERT_EQUAL(0, handle.depthBP.size());
	}

	void testConstructSingerFromTextStream()
	{
		TextStream stream;
		getSingerStream(stream);
		int index = 101;
		string lastLine = "";
		VSQFileReader reader;
		Handle handle = reader.parseHandle(stream, index, lastLine);
		CPPUNIT_ASSERT_EQUAL(index, handle.index);
		CPPUNIT_ASSERT_EQUAL(HandleType::SINGER, handle.type());
		CPPUNIT_ASSERT_EQUAL(string("$07010002"), handle.iconId);
		CPPUNIT_ASSERT_EQUAL(string("Miku3=God"), handle.ids);
		CPPUNIT_ASSERT_EQUAL(2, handle.original);
		CPPUNIT_ASSERT_EQUAL(string(""), handle.caption);
		CPPUNIT_ASSERT_EQUAL((tick_t)1, handle.length());
		CPPUNIT_ASSERT_EQUAL(1, handle.language);
		CPPUNIT_ASSERT_EQUAL(2, handle.program);
	}

	void testConstructAttackFromTextStream()
	{
		TextStream stream;
		getAttackStream(stream);
		string lastLine = "";
		int index = 204;
		VSQFileReader reader;
		Handle handle = reader.parseHandle(stream, index, lastLine);
		CPPUNIT_ASSERT_EQUAL(HandleType::NOTE_HEAD, handle.type());
		CPPUNIT_ASSERT_EQUAL(index, handle.index);
		CPPUNIT_ASSERT_EQUAL(string("$01010002"), handle.iconId);
		CPPUNIT_ASSERT_EQUAL(string("accent"), handle.ids);
		CPPUNIT_ASSERT_EQUAL(2, handle.original);
		CPPUNIT_ASSERT_EQUAL(string("Accent"), handle.caption);
		CPPUNIT_ASSERT_EQUAL((tick_t)120, handle.length());
		CPPUNIT_ASSERT_EQUAL(64, handle.duration);
		CPPUNIT_ASSERT_EQUAL(63, handle.depth);
	}

	void testConstructCrescendFromTextStream()
	{
		TextStream stream;
		getCrescendoStream(stream);
		string lastLine;
		int index = 204;
		VSQFileReader reader;
		Handle handle = reader.parseHandle(stream, index, lastLine);
		CPPUNIT_ASSERT_EQUAL(index, handle.index);
		CPPUNIT_ASSERT_EQUAL(HandleType::DYNAMICS, handle.type());
		CPPUNIT_ASSERT_EQUAL(string("$05020001"), handle.iconId);
		CPPUNIT_ASSERT_EQUAL(string("Crescendo"), handle.ids);
		CPPUNIT_ASSERT_EQUAL(4, handle.original);
		CPPUNIT_ASSERT_EQUAL(string("Zero Crescendo Curve"), handle.caption);
		CPPUNIT_ASSERT_EQUAL((tick_t)960, handle.length());
		CPPUNIT_ASSERT_EQUAL(2, handle.startDyn);
		CPPUNIT_ASSERT_EQUAL(38, handle.endDyn);
		CPPUNIT_ASSERT_EQUAL(string("0.5=11"), handle.dynBP.data());
	}

	void testParseEvent()
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

		CPPUNIT_ASSERT_EQUAL(EventType::NOTE, event.type());
		CPPUNIT_ASSERT_EQUAL((tick_t)1, event.length());
		CPPUNIT_ASSERT_EQUAL(2, event.note);
		CPPUNIT_ASSERT_EQUAL(3, event.dynamics);
		CPPUNIT_ASSERT_EQUAL(4, event.pmBendDepth);
		CPPUNIT_ASSERT_EQUAL(5, event.pmBendLength);
		CPPUNIT_ASSERT_EQUAL(6, event.demDecGainRate);
		CPPUNIT_ASSERT_EQUAL(7, event.demAccent);
		CPPUNIT_ASSERT_EQUAL(1, lyricHandleIndex);
		CPPUNIT_ASSERT_EQUAL(2, singerHandleIndex);
		CPPUNIT_ASSERT_EQUAL(3, vibratoHandleIndex);
		CPPUNIT_ASSERT_EQUAL(4, noteHeadHandleIndex);
	}

	CPPUNIT_TEST_SUITE(VSQFileReaderTest);
	CPPUNIT_TEST(testRead);
	CPPUNIT_TEST(testConstructLyricFromTextStreamStopWithEOF);
	CPPUNIT_TEST(testConstructLyricFromTextStreamStopWithNextHandle);
	CPPUNIT_TEST(testConstructVibratoFromTextStream);
	CPPUNIT_TEST(testConstructVibratoFromTextStreamWithoutBP);
	CPPUNIT_TEST(testConstructSingerFromTextStream);
	CPPUNIT_TEST(testConstructAttackFromTextStream);
	CPPUNIT_TEST(testConstructCrescendFromTextStream);
	CPPUNIT_TEST(testParseEvent);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(VSQFileReaderTest);
