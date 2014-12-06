#include "Util.hpp"
#include "../include/libvsq/MidiParameterType.hpp"

using namespace std;
using namespace vsq;

class MidiParameterTypeTest : public CppUnit::TestCase
{
public:
	void testGetVoiceChangeParameterId()
	{
		CPPUNIT_ASSERT_EQUAL(0x30, MidiParameterTypeUtil::getVoiceChangeParameterId("HarMoNiCS"));
		CPPUNIT_ASSERT_EQUAL(0x31, MidiParameterTypeUtil::getVoiceChangeParameterId("bre"));
		CPPUNIT_ASSERT_EQUAL(0x32, MidiParameterTypeUtil::getVoiceChangeParameterId("bri"));
		CPPUNIT_ASSERT_EQUAL(0x33, MidiParameterTypeUtil::getVoiceChangeParameterId("cle"));
		CPPUNIT_ASSERT_EQUAL(0x34, MidiParameterTypeUtil::getVoiceChangeParameterId("por"));
		CPPUNIT_ASSERT_EQUAL(0x35, MidiParameterTypeUtil::getVoiceChangeParameterId("ope"));
		CPPUNIT_ASSERT_EQUAL(0x40, MidiParameterTypeUtil::getVoiceChangeParameterId("reso1freq"));
		CPPUNIT_ASSERT_EQUAL(0x41, MidiParameterTypeUtil::getVoiceChangeParameterId("reso2freq"));
		CPPUNIT_ASSERT_EQUAL(0x42, MidiParameterTypeUtil::getVoiceChangeParameterId("reso3freq"));
		CPPUNIT_ASSERT_EQUAL(0x43, MidiParameterTypeUtil::getVoiceChangeParameterId("reso4freq"));
		CPPUNIT_ASSERT_EQUAL(0x50, MidiParameterTypeUtil::getVoiceChangeParameterId("reso1bw"));
		CPPUNIT_ASSERT_EQUAL(0x51, MidiParameterTypeUtil::getVoiceChangeParameterId("reso2bw"));
		CPPUNIT_ASSERT_EQUAL(0x52, MidiParameterTypeUtil::getVoiceChangeParameterId("reso3bw"));
		CPPUNIT_ASSERT_EQUAL(0x53, MidiParameterTypeUtil::getVoiceChangeParameterId("reso4bw"));
		CPPUNIT_ASSERT_EQUAL(0x60, MidiParameterTypeUtil::getVoiceChangeParameterId("reso1amp"));
		CPPUNIT_ASSERT_EQUAL(0x61, MidiParameterTypeUtil::getVoiceChangeParameterId("reso2amp"));
		CPPUNIT_ASSERT_EQUAL(0x62, MidiParameterTypeUtil::getVoiceChangeParameterId("reso3amp"));
		CPPUNIT_ASSERT_EQUAL(0x63, MidiParameterTypeUtil::getVoiceChangeParameterId("reso4amp"));
		CPPUNIT_ASSERT_EQUAL(0x70, MidiParameterTypeUtil::getVoiceChangeParameterId("gen"));

		CPPUNIT_ASSERT_EQUAL(0x31, MidiParameterTypeUtil::getVoiceChangeParameterId("UNKNOWN_CURVE_NAME"));
	}

	void testIsDataLsbRequire()
	{
		CPPUNIT_ASSERT(MidiParameterTypeUtil::isDataLsbRequire(MidiParameterType::CC_PBS_VERSION_AND_DEVICE));
		CPPUNIT_ASSERT_EQUAL(false, MidiParameterTypeUtil::isDataLsbRequire(MidiParameterType::CVM_NM_NOTE_MESSAGE_CONTINUATION));
		CPPUNIT_ASSERT_EQUAL(false, MidiParameterTypeUtil::isDataLsbRequire((MidiParameterType)0));
	}

	CPPUNIT_TEST_SUITE(MidiParameterTypeTest);
	CPPUNIT_TEST(testGetVoiceChangeParameterId);
	CPPUNIT_TEST(testIsDataLsbRequire);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(MidiParameterTypeTest);
