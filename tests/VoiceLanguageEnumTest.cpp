#include "Util.hpp"
#include "../VoiceLanguageEnum.hpp"

using namespace std;
using namespace VSQ_NS;

class VoiceLanguageEnumTest : public CppUnit::TestCase
{
public:
    void test(){
        CPPUNIT_ASSERT_EQUAL( 0, (int)VoiceLanguage::JAPANESE );
        CPPUNIT_ASSERT_EQUAL( 1, (int)VoiceLanguage::ENGLISH );
    
        CPPUNIT_ASSERT_EQUAL( VoiceLanguage::ENGLISH, VoiceLanguage::valueFromSingerName( "sweet_ann" ) );
        CPPUNIT_ASSERT_EQUAL( VoiceLanguage::ENGLISH, VoiceLanguage::valueFromSingerName( "prima" ) );
        CPPUNIT_ASSERT_EQUAL( VoiceLanguage::ENGLISH, VoiceLanguage::valueFromSingerName( "luka_eng" ) );
        CPPUNIT_ASSERT_EQUAL( VoiceLanguage::ENGLISH, VoiceLanguage::valueFromSingerName( "sonika" ) );
        CPPUNIT_ASSERT_EQUAL( VoiceLanguage::ENGLISH, VoiceLanguage::valueFromSingerName( "lola" ) );
        CPPUNIT_ASSERT_EQUAL( VoiceLanguage::ENGLISH, VoiceLanguage::valueFromSingerName( "leon" ) );
        CPPUNIT_ASSERT_EQUAL( VoiceLanguage::ENGLISH, VoiceLanguage::valueFromSingerName( "miriam" ) );
        CPPUNIT_ASSERT_EQUAL( VoiceLanguage::ENGLISH, VoiceLanguage::valueFromSingerName( "big_al" ) );
        CPPUNIT_ASSERT_EQUAL( VoiceLanguage::JAPANESE, VoiceLanguage::valueFromSingerName( "___FOO___" ) );
    }

    CPPUNIT_TEST_SUITE( VoiceLanguageEnumTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( VoiceLanguageEnumTest );
