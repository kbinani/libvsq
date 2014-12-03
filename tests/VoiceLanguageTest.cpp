#include "Util.hpp"
#include "../include/libvsq/VoiceLanguage.hpp"

using namespace std;
using namespace vsq;

class VoiceLanguageEnumTest : public CppUnit::TestCase
{
public:
	void test()
	{
		CPPUNIT_ASSERT_EQUAL(0, (int)VoiceLanguage::JAPANESE);
		CPPUNIT_ASSERT_EQUAL(1, (int)VoiceLanguage::ENGLISH);

		CPPUNIT_ASSERT_EQUAL(VoiceLanguage::ENGLISH, VoiceLanguageUtil::valueFromSingerName("sweet_ann"));
		CPPUNIT_ASSERT_EQUAL(VoiceLanguage::ENGLISH, VoiceLanguageUtil::valueFromSingerName("prima"));
		CPPUNIT_ASSERT_EQUAL(VoiceLanguage::ENGLISH, VoiceLanguageUtil::valueFromSingerName("luka_eng"));
		CPPUNIT_ASSERT_EQUAL(VoiceLanguage::ENGLISH, VoiceLanguageUtil::valueFromSingerName("sonika"));
		CPPUNIT_ASSERT_EQUAL(VoiceLanguage::ENGLISH, VoiceLanguageUtil::valueFromSingerName("lola"));
		CPPUNIT_ASSERT_EQUAL(VoiceLanguage::ENGLISH, VoiceLanguageUtil::valueFromSingerName("leon"));
		CPPUNIT_ASSERT_EQUAL(VoiceLanguage::ENGLISH, VoiceLanguageUtil::valueFromSingerName("miriam"));
		CPPUNIT_ASSERT_EQUAL(VoiceLanguage::ENGLISH, VoiceLanguageUtil::valueFromSingerName("big_al"));
		CPPUNIT_ASSERT_EQUAL(VoiceLanguage::JAPANESE, VoiceLanguageUtil::valueFromSingerName("___FOO___"));
	}

	CPPUNIT_TEST_SUITE(VoiceLanguageEnumTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(VoiceLanguageEnumTest);
