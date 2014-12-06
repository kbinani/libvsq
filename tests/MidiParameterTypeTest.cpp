#include "Util.hpp"
#include "../include/libvsq/MidiParameterType.hpp"

using namespace std;
using namespace vsq;

TEST(MidiParameterTypeTest, testGetVoiceChangeParameterId)
{
	EXPECT_EQ(0x30, MidiParameterTypeUtil::getVoiceChangeParameterId("HarMoNiCS"));
	EXPECT_EQ(0x31, MidiParameterTypeUtil::getVoiceChangeParameterId("bre"));
	EXPECT_EQ(0x32, MidiParameterTypeUtil::getVoiceChangeParameterId("bri"));
	EXPECT_EQ(0x33, MidiParameterTypeUtil::getVoiceChangeParameterId("cle"));
	EXPECT_EQ(0x34, MidiParameterTypeUtil::getVoiceChangeParameterId("por"));
	EXPECT_EQ(0x35, MidiParameterTypeUtil::getVoiceChangeParameterId("ope"));
	EXPECT_EQ(0x40, MidiParameterTypeUtil::getVoiceChangeParameterId("reso1freq"));
	EXPECT_EQ(0x41, MidiParameterTypeUtil::getVoiceChangeParameterId("reso2freq"));
	EXPECT_EQ(0x42, MidiParameterTypeUtil::getVoiceChangeParameterId("reso3freq"));
	EXPECT_EQ(0x43, MidiParameterTypeUtil::getVoiceChangeParameterId("reso4freq"));
	EXPECT_EQ(0x50, MidiParameterTypeUtil::getVoiceChangeParameterId("reso1bw"));
	EXPECT_EQ(0x51, MidiParameterTypeUtil::getVoiceChangeParameterId("reso2bw"));
	EXPECT_EQ(0x52, MidiParameterTypeUtil::getVoiceChangeParameterId("reso3bw"));
	EXPECT_EQ(0x53, MidiParameterTypeUtil::getVoiceChangeParameterId("reso4bw"));
	EXPECT_EQ(0x60, MidiParameterTypeUtil::getVoiceChangeParameterId("reso1amp"));
	EXPECT_EQ(0x61, MidiParameterTypeUtil::getVoiceChangeParameterId("reso2amp"));
	EXPECT_EQ(0x62, MidiParameterTypeUtil::getVoiceChangeParameterId("reso3amp"));
	EXPECT_EQ(0x63, MidiParameterTypeUtil::getVoiceChangeParameterId("reso4amp"));
	EXPECT_EQ(0x70, MidiParameterTypeUtil::getVoiceChangeParameterId("gen"));

	EXPECT_EQ(0x31, MidiParameterTypeUtil::getVoiceChangeParameterId("UNKNOWN_CURVE_NAME"));
}

TEST(MidiParameterTypeTest, testIsDataLsbRequire)
{
	EXPECT_TRUE(MidiParameterTypeUtil::isDataLsbRequire(MidiParameterType::CC_PBS_VERSION_AND_DEVICE));
	EXPECT_EQ(false, MidiParameterTypeUtil::isDataLsbRequire(MidiParameterType::CVM_NM_NOTE_MESSAGE_CONTINUATION));
	EXPECT_EQ(false, MidiParameterTypeUtil::isDataLsbRequire((MidiParameterType)0));
}
