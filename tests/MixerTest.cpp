#include "Util.hpp"
#include "../Mixer.hpp"
#include "../TextStream.hpp"

using namespace vsq;

class MixerTest : public CppUnit::TestCase
{
public:
	void testConstruct()
	{
		Mixer mixer(1, 2, 3, 4);
		CPPUNIT_ASSERT_EQUAL(1, mixer.masterFeder);
		CPPUNIT_ASSERT_EQUAL(2, mixer.masterPanpot);
		CPPUNIT_ASSERT_EQUAL(3, mixer.masterMute);
		CPPUNIT_ASSERT_EQUAL(4, mixer.outputMode);
		CPPUNIT_ASSERT_EQUAL((std::size_t)0, mixer.slave.size());
	}

	void testConstructFromStream()
	{
		TextStream stream;
		stream.writeLine("MasterFeder=1");
		stream.writeLine("MasterPanpot=2");
		stream.writeLine("MasterMute=3");
		stream.writeLine("OutputMode=4");
		stream.writeLine("Tracks=1");
		stream.writeLine("Feder0=5");
		stream.writeLine("Panpot0=6");
		stream.writeLine("Mute0=7");
		stream.writeLine("Solo0=8");
		stream.setPointer(-1);

		std::string lastLine = "";
		Mixer mixer(stream, lastLine);

		CPPUNIT_ASSERT_EQUAL((std::size_t)1, mixer.slave.size());
		CPPUNIT_ASSERT_EQUAL(1, mixer.masterFeder);
		CPPUNIT_ASSERT_EQUAL(2, mixer.masterPanpot);
		CPPUNIT_ASSERT_EQUAL(3, mixer.masterMute);
		CPPUNIT_ASSERT_EQUAL(4, mixer.outputMode);
		CPPUNIT_ASSERT_EQUAL(5, mixer.slave[0].feder);
		CPPUNIT_ASSERT_EQUAL(6, mixer.slave[0].panpot);
		CPPUNIT_ASSERT_EQUAL(7, mixer.slave[0].mute);
		CPPUNIT_ASSERT_EQUAL(8, mixer.slave[0].solo);
	}

	void testClone()
	{
		Mixer mixer(1, 2, 3, 4);
		mixer.slave.push_back(MixerItem(5, 6, 7, 8));
		mixer.slave.push_back(MixerItem(9, 10, 11, 12));

		Mixer copy = mixer.clone();
		CPPUNIT_ASSERT_EQUAL((std::size_t)2, copy.slave.size());
		CPPUNIT_ASSERT_EQUAL(1, copy.masterFeder);
		CPPUNIT_ASSERT_EQUAL(2, copy.masterPanpot);
		CPPUNIT_ASSERT_EQUAL(3, copy.masterMute);
		CPPUNIT_ASSERT_EQUAL(4, copy.outputMode);
		CPPUNIT_ASSERT_EQUAL(5, copy.slave[0].feder);
		CPPUNIT_ASSERT_EQUAL(6, copy.slave[0].panpot);
		CPPUNIT_ASSERT_EQUAL(7, copy.slave[0].mute);
		CPPUNIT_ASSERT_EQUAL(8, copy.slave[0].solo);
		CPPUNIT_ASSERT_EQUAL(9, copy.slave[1].feder);
		CPPUNIT_ASSERT_EQUAL(10, copy.slave[1].panpot);
		CPPUNIT_ASSERT_EQUAL(11, copy.slave[1].mute);
		CPPUNIT_ASSERT_EQUAL(12, copy.slave[1].solo);
	}

	void testWrite()
	{
		Mixer mixer(1, 2, 3, 4);
		mixer.slave.push_back(MixerItem(5, 6, 7, 8));
		mixer.slave.push_back(MixerItem(9, 10, 11, 12));
		TextStream stream;
		mixer.write(stream);
		std::string expected =
			"[Mixer]\n"
			"MasterFeder=1\n"
			"MasterPanpot=2\n"
			"MasterMute=3\n"
			"OutputMode=4\n"
			"Tracks=2\n"
			"Feder0=5\n"
			"Panpot0=6\n"
			"Mute0=7\n"
			"Solo0=8\n"
			"Feder1=9\n"
			"Panpot1=10\n"
			"Mute1=11\n"
			"Solo1=12\n";
		CPPUNIT_ASSERT_EQUAL(expected, stream.toString());
	}

	CPPUNIT_TEST_SUITE(MixerTest);
	CPPUNIT_TEST(testConstruct);
	CPPUNIT_TEST(testConstructFromStream);
	CPPUNIT_TEST(testClone);
	CPPUNIT_TEST(testWrite);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(MixerTest);
