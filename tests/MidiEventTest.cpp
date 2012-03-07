#include "Util.h"
#include "../MidiEvent.h"
#include "../ByteArrayOutputStream.h"

using namespace std;
using namespace VSQ_NS;

class MidiEventTest : public CppUnit::TestCase
{
public:
    void testWriteData()
    {
        ByteArrayOutputStream stream;
        MidiEvent event;
        event.firstByte = 0x91;
        event.data.push_back( 64 );
        event.data.push_back( 127 );
        event.writeData( (OutputStream *)&stream );
        ostringstream expected;
        expected << (char)0x91 << (char)64 << (char)127;
        CPPUNIT_ASSERT_EQUAL( expected.str(), stream.toString() );
        stream.close();
    }

    void testWriteDataWithMetaEvent()
    {
        ByteArrayOutputStream stream;
        MidiEvent event;
        event.firstByte = 0xff;
        event.data.push_back( 0x51 );
        event.data.push_back( 0x82 );
        event.data.push_back( 0x81 );
        event.data.push_back( 0x80 );
        event.writeData( (OutputStream *)&stream );
        ostringstream expected;
        expected << (char)0xff << (char)0x51 << (char)3 << (char)0x82 << (char)0x81 << (char)0x80;
        CPPUNIT_ASSERT_EQUAL( expected.str(), stream.toString() );
    }
    
    void testCompareTo()
    {
        MidiEvent a;
        MidiEvent b;
        a.clock = 0;
        b.clock = 480;
        CPPUNIT_ASSERT( 0 < b.compareTo( a ) );
        CPPUNIT_ASSERT( 0 > a.compareTo( b ) );
    
        a.firstByte = 1;
        b.firstByte = 2;
        a.clock = 0;
        b.clock = 0;
        CPPUNIT_ASSERT_EQUAL( 0, a.compareTo( b ) );
    
        //同じ音程の、Note OnとNote Offが続いていた場合、Note Offが先、Note Onが後ろになる
        a.firstByte = 0x92;
        b.firstByte = 0x82;
        a.clock = 0;
        b.clock = 0;
        a.data.clear();
        a.data.push_back( 64 );
        a.data.push_back( 127 ); // note#=64, vel=127の Note On
        b.data.clear();
        b.data.push_back( 64 );
        b.data.push_back( 127 ); // note#=64, vel=127の Note Off
        // b => a
        CPPUNIT_ASSERT( 0 < a.compareTo( b ) );
        CPPUNIT_ASSERT( 0 > b.compareTo( a ) );
    
        //同じ音程の、Note OnとNote Offが続いていた場合、Note Offが先、Note Onが後ろになる
        //ただし、Note Offが、ベロシティー0のNote Onとして表現されている場合
        a.firstByte = 0x91;
        b.firstByte = 0x91;
        a.clock = 0;
        b.clock = 0;
        a.data.clear();
        a.data.push_back( 64 );
        a.data.push_back( 127 ); // note#=64, vel=127の Note On
        b.data.clear();
        b.data.push_back( 64 );
        b.data.push_back( 0 );   // note#=64, vel=0の Note On、vel=0なので Note Off とみなされる
        // b => a
        CPPUNIT_ASSERT( 0 < a.compareTo( b ) );
        CPPUNIT_ASSERT( 0 > b.compareTo( a ) );
    
        a.firstByte = 90;
        b.firstByte = 80;
        a.clock = 0;
        b.clock = 0;
        a.data.clear();
        a.data.push_back( 63 );
        a.data.push_back( 127 );
        b.data.clear();
        b.data.push_back( 64 );
        b.data.push_back( 127 );
        CPPUNIT_ASSERT_EQUAL( 0, a.compareTo( b ) );
        CPPUNIT_ASSERT_EQUAL( 0, b.compareTo( a ) );
    }
    
    void testGenerateTimeSigEvent()
    {
        MidiEvent event = MidiEvent::generateTimeSigEvent( 10, 3, 2 );
        CPPUNIT_ASSERT_EQUAL( (tick_t)10, event.clock );
        CPPUNIT_ASSERT_EQUAL( 0xff, event.firstByte );
        CPPUNIT_ASSERT_EQUAL( 5, (int)event.data.size() );
        CPPUNIT_ASSERT_EQUAL( 0x58, event.data[0] );
        CPPUNIT_ASSERT_EQUAL( 3, event.data[1] );
        CPPUNIT_ASSERT_EQUAL( 1, event.data[2] );
        CPPUNIT_ASSERT_EQUAL( 0x18, event.data[3] );
        CPPUNIT_ASSERT_EQUAL( 0x08, event.data[4] );
    }
    
    void testGenerateTempoChangeEvent()
    {
        MidiEvent event = MidiEvent::generateTempoChangeEvent( 12, 0x828180 );
        CPPUNIT_ASSERT_EQUAL( (tick_t)12, event.clock );
        CPPUNIT_ASSERT_EQUAL( 0xff, event.firstByte );
        CPPUNIT_ASSERT_EQUAL( 4, (int)event.data.size() );
        CPPUNIT_ASSERT_EQUAL( 0x51, event.data[0] );
        CPPUNIT_ASSERT_EQUAL( 0x82, event.data[1] );
        CPPUNIT_ASSERT_EQUAL( 0x81, event.data[2] );
        CPPUNIT_ASSERT_EQUAL( 0x80, event.data[3] );
    }
    
    void testWriteDeltaClock()
    {
        ByteArrayOutputStream stream;
        MidiEvent::writeDeltaClock( (OutputStream *)&stream, 0 );
        string expected = " ";
        expected[0] = 0x0;
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    
        stream.seek( 0 );
        MidiEvent::writeDeltaClock( (OutputStream *)&stream, 127 );
        expected[0] = 0x7f;
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    
        stream.seek( 0 );
        MidiEvent::writeDeltaClock( (OutputStream *)&stream, 128 );
        expected = "  ";
        expected[0] = 0x81;
        expected[1] = 0x00;
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    
        stream.seek( 0 );
        MidiEvent::writeDeltaClock( (OutputStream *)&stream, 12345678 );
        expected = "    ";
        expected[0] = 0x85;
        expected[1] = 0xf1;
        expected[2] = 0xc2;
        expected[3] = 0x4e;
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    }
    
    void testReadDeltaClock()
    {
        //TODO:
    }
    
    void testRead()
    {
        //TODO:
    }

    CPPUNIT_TEST_SUITE( MidiEventTest );
    CPPUNIT_TEST( testWriteData );
    CPPUNIT_TEST( testWriteDataWithMetaEvent );
    CPPUNIT_TEST( testCompareTo );
    CPPUNIT_TEST( testGenerateTimeSigEvent );
    CPPUNIT_TEST( testGenerateTempoChangeEvent );
    CPPUNIT_TEST( testWriteDeltaClock );
    CPPUNIT_TEST( testReadDeltaClock );
    CPPUNIT_TEST( testRead );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( MidiEventTest );
