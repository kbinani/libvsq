#include "Util.hpp"
#include <libvsq/VSQXFileReader.hpp>
#include <libvsq/FileInputStream.hpp>

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

using namespace vsq;

namespace
{
	void assertSequence(Sequence const& actual)
	{
		// timesigList
		ASSERT_EQ(2, actual.timesigList.size());
		auto const& ts1 = actual.timesigList.get(0);
		EXPECT_EQ(0, ts1.barCount);
		EXPECT_EQ(4, ts1.numerator);
		EXPECT_EQ(4, ts1.denominator);
		
		auto const& ts2 = actual.timesigList.get(1);
		EXPECT_EQ(4, ts2.barCount);
		EXPECT_EQ(4, ts2.numerator);
		EXPECT_EQ(4, ts2.denominator);
		
		// tempoList
		ASSERT_EQ(2, actual.tempoList.size());
		auto const& t1 = actual.tempoList.get(0);
		EXPECT_EQ(0, t1.tick);
		EXPECT_EQ(600000, t1.tempo);
		
		auto const& t2 = actual.tempoList.get(1);
		EXPECT_EQ(7680, t2.tick);
		EXPECT_EQ(600000, t2.tempo);
		
		// master
		EXPECT_EQ(4, actual.master.preMeasure);
		
		// tracks
		ASSERT_EQ(1, actual.tracks().size());
		
		// track#1
		ASSERT_EQ(62, actual.track(0).events().size());
		
		// track#1, PBS
		BPList const* pbs = actual.track(0).curve("PBS");
		ASSERT_TRUE(pbs != nullptr);
		ASSERT_EQ(1, pbs->size());
		BP p = pbs->get(0);
		EXPECT_EQ(1, p.value);
		
		// track#1. singer change
		Event const* sc = actual.track(0).events().get(0);
		EXPECT_EQ(EventType::SINGER, sc->type());
		EXPECT_EQ(0, sc->singerHandle.original);
		EXPECT_EQ(1, sc->singerHandle.program);
		
		// track#1, note#10
		Event const* note10 = actual.track(0).events().get(10);
		EXPECT_EQ(EventType::NOTE, note10->type());
		EXPECT_EQ(7680 /*offset*/ + 11520 /*tick from part head*/, note10->tick);
		EXPECT_EQ(69, note10->note);
		EXPECT_EQ(960, note10->length());
		EXPECT_EQ(64, note10->dynamics);
		EXPECT_EQ(1, note10->lyricHandle.size());
		EXPECT_EQ("け", note10->lyricHandle.get(0).phrase);
		EXPECT_EQ("k e", note10->lyricHandle.get(0).phoneticSymbol());
		EXPECT_EQ(50, note10->demAccent);
		EXPECT_EQ(8, note10->pmBendDepth);
		EXPECT_EQ(0, note10->pmBendLength);
		EXPECT_EQ(50, note10->demDecGainRate);
		EXPECT_EQ(0, note10->pmbPortamentoUse);

		EXPECT_EQ(HandleType::VIBRATO, note10->vibratoHandle.type());
		EXPECT_EQ("$04040001", note10->vibratoHandle.iconId);
		ASSERT_EQ(1, note10->vibratoHandle.rateBP.size());
		EXPECT_EQ(0.0, note10->vibratoHandle.rateBP.get(0).x);
		EXPECT_EQ(50, note10->vibratoHandle.rateBP.get(0).y);
		ASSERT_EQ(1, note10->vibratoHandle.depthBP.size());
		EXPECT_EQ(0.0, note10->vibratoHandle.depthBP.get(0).x);
		EXPECT_EQ(64, note10->vibratoHandle.depthBP.get(0).y);

		// mixer
        EXPECT_TRUE(actual.mixer.slave.size() == actual.tracks().size());
        EXPECT_EQ(0, actual.mixer.masterFeder);

        // mixer slave#1
        EXPECT_EQ(0, actual.mixer.slave[0].feder);
        EXPECT_EQ(0, actual.mixer.slave[0].mute);
        EXPECT_EQ(0, actual.mixer.slave[0].panpot);
        EXPECT_EQ(0, actual.mixer.slave[0].solo);
    }

    Sequence createTestSequence()
    {
        Sequence sequence("singer", 1, 3, 4, 500000);
        sequence.mixer.masterFeder = 7;
        return sequence;
    }
}


TEST(VSQXFileReaderTest, vsq3)
{
    Sequence sequence = createTestSequence();
	VSQXFileReader reader;
	FileInputStream stream("VSQXFileReaderTest/fixture/d3s_3.vsqx");
	reader.read(sequence, stream);
	stream.close();
	assertSequence(sequence);
}


TEST(VSQXFileReaderTest, vsq4)
{
    Sequence sequence = createTestSequence();
	VSQXFileReader reader;
	FileInputStream stream("VSQXFileReaderTest/fixture/d3s_4.vsqx");
	reader.read(sequence, stream);
	stream.close();
	assertSequence(sequence);
}
