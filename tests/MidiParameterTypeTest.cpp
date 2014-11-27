#include "Util.hpp"
#include "../MidiParameterType.hpp"

using namespace std;
using namespace vsq;

class MidiParameterTypeTest : public CppUnit::TestCase
{
public:
	void testGetVoiceChangeParameterId()
	{
		CPPUNIT_ASSERT_EQUAL(0x30, MidiParameterType::getVoiceChangeParameterId("HarMoNiCS"));
		CPPUNIT_ASSERT_EQUAL(0x31, MidiParameterType::getVoiceChangeParameterId("bre"));
		CPPUNIT_ASSERT_EQUAL(0x32, MidiParameterType::getVoiceChangeParameterId("bri"));
		CPPUNIT_ASSERT_EQUAL(0x33, MidiParameterType::getVoiceChangeParameterId("cle"));
		CPPUNIT_ASSERT_EQUAL(0x34, MidiParameterType::getVoiceChangeParameterId("por"));
		CPPUNIT_ASSERT_EQUAL(0x35, MidiParameterType::getVoiceChangeParameterId("ope"));
		CPPUNIT_ASSERT_EQUAL(0x40, MidiParameterType::getVoiceChangeParameterId("reso1freq"));
		CPPUNIT_ASSERT_EQUAL(0x41, MidiParameterType::getVoiceChangeParameterId("reso2freq"));
		CPPUNIT_ASSERT_EQUAL(0x42, MidiParameterType::getVoiceChangeParameterId("reso3freq"));
		CPPUNIT_ASSERT_EQUAL(0x43, MidiParameterType::getVoiceChangeParameterId("reso4freq"));
		CPPUNIT_ASSERT_EQUAL(0x50, MidiParameterType::getVoiceChangeParameterId("reso1bw"));
		CPPUNIT_ASSERT_EQUAL(0x51, MidiParameterType::getVoiceChangeParameterId("reso2bw"));
		CPPUNIT_ASSERT_EQUAL(0x52, MidiParameterType::getVoiceChangeParameterId("reso3bw"));
		CPPUNIT_ASSERT_EQUAL(0x53, MidiParameterType::getVoiceChangeParameterId("reso4bw"));
		CPPUNIT_ASSERT_EQUAL(0x60, MidiParameterType::getVoiceChangeParameterId("reso1amp"));
		CPPUNIT_ASSERT_EQUAL(0x61, MidiParameterType::getVoiceChangeParameterId("reso2amp"));
		CPPUNIT_ASSERT_EQUAL(0x62, MidiParameterType::getVoiceChangeParameterId("reso3amp"));
		CPPUNIT_ASSERT_EQUAL(0x63, MidiParameterType::getVoiceChangeParameterId("reso4amp"));
		CPPUNIT_ASSERT_EQUAL(0x70, MidiParameterType::getVoiceChangeParameterId("gen"));

		CPPUNIT_ASSERT_EQUAL(0x31, MidiParameterType::getVoiceChangeParameterId("UNKNOWN_CURVE_NAME"));
	}

	void testIsDataLsbRequire()
	{
		CPPUNIT_ASSERT(MidiParameterType::isDataLsbRequire(MidiParameterType::CC_PBS_VERSION_AND_DEVICE));
		CPPUNIT_ASSERT_EQUAL(false, MidiParameterType::isDataLsbRequire(MidiParameterType::CVM_NM_NOTE_MESSAGE_CONTINUATION));
		CPPUNIT_ASSERT_EQUAL(false, MidiParameterType::isDataLsbRequire((MidiParameterType::MidiParameterTypeEnum)0));
	}

	CPPUNIT_TEST_SUITE(MidiParameterTypeTest);
	CPPUNIT_TEST(testGetVoiceChangeParameterId);
	CPPUNIT_TEST(testIsDataLsbRequire);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(MidiParameterTypeTest);
