#include "Util.hpp"
#include "../include/libvsq/Sequence.hpp"
#include "../include/libvsq/ByteArrayOutputStream.hpp"

using namespace std;
using namespace vsq;

/**
 * 指定されたシーケンスがデフォルトのシーケンスと等しいかどうかを検査する
 */
void isEqualToDefaultSequence(const Sequence& sequence)
{
	EXPECT_EQ((tick_t)(1 * 480 * 4 / 4 * 4), sequence.totalTicks());

	// track
	EXPECT_EQ((size_t)1, sequence.tracks().size());
	// 第1トラック
	Track const& track1 = sequence.track(0);
	/*assert_not_nil( track1.common );
	assert_not_nil( track1:getCurve( "pit" ) );
	assert_not_nil( track1:getCurve( "pbs" ) );
	assert_not_nil( track1:getCurve( "dyn" ) );
	assert_not_nil( track1:getCurve( "bre" ) );
	assert_not_nil( track1:getCurve( "bri" ) );
	assert_not_nil( track1:getCurve( "cle" ) );
	assert_not_nil( track1:getCurve( "reso1Freq" ) );
	assert_not_nil( track1:getCurve( "reso2Freq" ) );
	assert_not_nil( track1:getCurve( "reso3Freq" ) );
	assert_not_nil( track1:getCurve( "reso4Freq" ) );
	assert_not_nil( track1:getCurve( "reso1BW" ) );
	assert_not_nil( track1:getCurve( "reso2BW" ) );
	assert_not_nil( track1:getCurve( "reso3BW" ) );
	assert_not_nil( track1:getCurve( "reso4BW" ) );
	assert_not_nil( track1:getCurve( "reso1Amp" ) );
	assert_not_nil( track1:getCurve( "reso2Amp" ) );
	assert_not_nil( track1:getCurve( "reso3Amp" ) );
	assert_not_nil( track1:getCurve( "reso4Amp" ) );
	assert_not_nil( track1:getCurve( "harmonics" ) );
	assert_not_nil( track1:getCurve( "fx2depth" ) );
	assert_not_nil( track1:getCurve( "gen" ) );
	assert_not_nil( track1:getCurve( "por" ) );
	assert_not_nil( track1:getCurve( "ope" ) );
	assert_not_nil( track1:getCurve( "pitch" ) );
	assert_not_nil( track1.events );*/
	EXPECT_EQ(1, track1.events().size());
	EXPECT_EQ((tick_t)0, track1.events().get(0)->tick);
	EXPECT_EQ(EventType::SINGER, track1.events().get(0)->type());

	// master
	EXPECT_EQ(1, sequence.master.preMeasure);

	// mixer
	EXPECT_EQ(0, sequence.mixer.masterFeder);
	EXPECT_EQ(0, sequence.mixer.masterMute);
	EXPECT_EQ(0, sequence.mixer.masterPanpot);
	EXPECT_EQ(0, sequence.mixer.outputMode);
	// mixer.slave
	EXPECT_EQ((size_t)1, sequence.mixer.slave.size());
	EXPECT_EQ(0, sequence.mixer.slave[0].feder);
	EXPECT_EQ(0, sequence.mixer.slave[0].panpot);
	EXPECT_EQ(0, sequence.mixer.slave[0].mute);
	EXPECT_EQ(0, sequence.mixer.slave[0].solo);

	// timesigTable
	EXPECT_EQ(1, sequence.timesigList.size());
	EXPECT_EQ((tick_t)0, sequence.timesigList.get(0).tick());
	EXPECT_EQ(4, sequence.timesigList.get(0).denominator);
	EXPECT_EQ(4, sequence.timesigList.get(0).numerator);
	EXPECT_EQ(0, sequence.timesigList.get(0).barCount);

	// tempoTable
	EXPECT_EQ(1, sequence.tempoList.size());
	EXPECT_EQ((tick_t)0, sequence.tempoList.get(0).tick);
	EXPECT_EQ(500000, sequence.tempoList.get(0).tempo);
	EXPECT_EQ(0.0, sequence.tempoList.get(0).time());
}

TEST(SequenceTest, testConstruct)
{
	Sequence sequence("Miku", 1, 4, 4, 500000);
	isEqualToDefaultSequence(sequence);
}

TEST(SequenceTest, testClone)
{
	Sequence sequence("Miku", 1, 4, 4, 500000);
	Sequence copy = sequence.clone();
	isEqualToDefaultSequence(copy);
}

TEST(SequenceTest, testGetBaseTempo)
{
	Sequence sequence("Miku", 1, 4, 4, 500000);
	EXPECT_EQ(500000, sequence.baseTempo());
}

TEST(SequenceTest, testGetPreMeasure)
{
	int preMeasure = 1;
	Sequence sequence("Miku", preMeasure, 4, 4, 500000);
	EXPECT_EQ(preMeasure, sequence.preMeasure());
}

TEST(SequenceTest, testGetPreMeasureTicks)
{
	Sequence sequence("Miku", 1, 4, 4, 500000);
	EXPECT_EQ((tick_t)1920, sequence.preMeasureTicks());
}

TEST(SequenceTest, testGetTickPerQuarter)
{
	Sequence sequence("Miku", 1, 4, 4, 500000);
	EXPECT_EQ((tick_t)480, sequence.tickPerQuarter());
}

TEST(SequenceTest, testUpdateTotalTicks)
{
	Sequence sequence("Miku", 1, 4, 4, 500000);
	EXPECT_EQ((tick_t)(1 * 480 * 4 / 4 * 4), sequence.totalTicks());
	Event note(1920, EventType::NOTE);
	note.length(480);
	note.note = 60;
	sequence.track(0).events().add(note);
	sequence.updateTotalTicks();
	EXPECT_EQ((tick_t)2400, sequence.totalTicks());
}

TEST(SequenceTest, testGetMaximumNoteLengthAt)
{
	//    fail();
}

/**
	* @todo
	*/
TEST(SequenceTest, testGenerateNRPNAll)
{
	//    fail();
}

/**
	* @todo
	*/
TEST(SequenceTest, testGenerateNRPNPartial)
{
	//    fail();
}
