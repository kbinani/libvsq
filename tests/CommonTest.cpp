#include "Util.hpp"
#include "../Common.hpp"

using namespace std;
using namespace VSQ_NS;

class CommonTest : public CppUnit::TestCase{
public:
    /**
     * @todo
     */
    void constructFromStream(){
        TextStream stream;
        stream.writeLine( "Version=DSB303" );
        stream.writeLine( "Name=Foo" );
        stream.writeLine( "Color=1,2,3" );
        stream.writeLine( "DynamicsMode=1" );
        stream.writeLine( "PlayMode=1" );
        stream.setPointer( -1 );
        string lastLine = "";
        Common common( stream, lastLine );
        CPPUNIT_ASSERT_EQUAL( string( "DSB303" ), common.version );
        CPPUNIT_ASSERT_EQUAL( string( "Foo" ), common.name );
        CPPUNIT_ASSERT_EQUAL( string( "1,2,3" ), common.color );
        CPPUNIT_ASSERT_EQUAL( DynamicsMode::EXPERT, common.dynamicsMode );
        CPPUNIT_ASSERT_EQUAL( PlayMode::PLAY_WITH_SYNTH, common.playMode );
    }

    void constructFromArguments(){
        Common common( "__foo__", 3, 4, 5, DynamicsMode::STANDARD, PlayMode::PLAY_AFTER_SYNTH );
        CPPUNIT_ASSERT_EQUAL( string( "__foo__" ), common.name );
        CPPUNIT_ASSERT_EQUAL( string( "3,4,5" ), common.color );
        CPPUNIT_ASSERT_EQUAL( DynamicsMode::STANDARD, common.dynamicsMode );
        CPPUNIT_ASSERT_EQUAL( PlayMode::PLAY_AFTER_SYNTH, common.playMode );
    }

    /**
     * @todo
     */
    void clone(){
        Common common( "__foo__", 3, 4, 5, DynamicsMode::STANDARD, PlayMode::PLAY_AFTER_SYNTH );
        Common copy = common.clone();
        CPPUNIT_ASSERT_EQUAL( string( "__foo__" ), copy.name );
        CPPUNIT_ASSERT_EQUAL( string( "3,4,5" ), copy.color );
        CPPUNIT_ASSERT_EQUAL( DynamicsMode::STANDARD, copy.dynamicsMode );
        CPPUNIT_ASSERT_EQUAL( PlayMode::PLAY_AFTER_SYNTH, copy.playMode );
    }

    /**
     * @todo
     */
    void write(){
        Common common( "__foo__", 3, 4, 5, DynamicsMode::STANDARD, PlayMode::PLAY_AFTER_SYNTH );
        TextStream stream;
        common.write( stream );
        std::string expected =
            "[Common]\n"
            "Version=DSB301\n"
            "Name=__foo__\n"
            "Color=3,4,5\n"
            "DynamicsMode=0\n"
            "PlayMode=0\n";
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    }

    CPPUNIT_TEST_SUITE( CommonTest );
    CPPUNIT_TEST( constructFromStream );
    CPPUNIT_TEST( constructFromArguments );
    CPPUNIT_TEST( clone );
    CPPUNIT_TEST( write );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( CommonTest );
