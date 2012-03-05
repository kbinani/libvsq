#include "Util.h"
#include "../MidiParameterType.h"

using namespace std;
using namespace VSQ_NS;

class MidiParameterTypeTest : public CppUnit::TestCase
{
public:
    void testGetVoiceChangeParameterId()
    {
        CPPUNIT_ASSERT_EQUAL( 0x30, MidiParameterType::getVoiceChangeParameterId( "HarMoNiCS" ) );
        //CPPUNIT_ASSERT_EQUAL( nil, MidiParameterType::getVoiceChangeParameterId( nil ) );
        CPPUNIT_ASSERT_EQUAL( 0x31, MidiParameterType::getVoiceChangeParameterId( "UNKNOWN_CURVE_NAME" ) );
    }
    
    void testIsDataLsbRequire()
    {
        CPPUNIT_ASSERT( MidiParameterType::isDataLsbRequire( MidiParameterType::CC_PBS_VERSION_AND_DEVICE ) );
    }

    CPPUNIT_TEST_SUITE( MidiParameterTypeTest );
    CPPUNIT_TEST( testGetVoiceChangeParameterId );
    CPPUNIT_TEST( testIsDataLsbRequire );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( MidiParameterTypeTest );
