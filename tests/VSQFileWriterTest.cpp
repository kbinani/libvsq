#include "Util.hpp"
#include "../VSQFileWriter.hpp"
#include "../ByteArrayOutputStream.hpp"
#include "iostream"

using namespace std;
using namespace VSQ_NS;

class VSQFileWriterStub : public VSQFileWriter{
public:
    void writeUnsignedShort( OutputStream *stream, int data ){
        VSQFileWriter::writeUnsignedShort( stream, data );
    }

    void writeUnsignedInt( OutputStream *stream, int data ){
        VSQFileWriter::writeUnsignedInt( stream, data );
    }

    vector<MidiEvent> getMidiEventsFromMetaText( TextStream *stream, const string &encoding ){
        return VSQFileWriter::getMidiEventsFromMetaText( stream, encoding );
    }

    vector<int> getLinePrefixBytes( int count ){
        return VSQFileWriter::getLinePrefixBytes( count );
    }

    int getHowManyDigits( int number ){
        return VSQFileWriter::getHowManyDigits( number );
    }
};

class VSQFileWriterTest : public CppUnit::TestCase{
    void testWriteWithoutPitch(){
        Sequence sequence( "Foo", 1, 4, 4, 500000 );
        const int CURVE_COUNT = 19;
        string curveNames[CURVE_COUNT] = { "BRE", "BRI", "CLE", "POR", "GEN", "harmonics", "OPE",
                       "reso1amp", "reso1bw", "reso1freq",
                       "reso2amp", "reso2bw", "reso2freq",
                       "reso3amp", "reso3bw", "reso3freq",
                       "reso4amp", "reso4bw", "reso4freq" };
        for( int i = 0; i < CURVE_COUNT; i++ ){
            BPList *list = sequence.track[1].getCurve( curveNames[i] );
            list->add( 1920, 1 + i );
        }
        Event noteEvent( 1920, EventType::NOTE );
        noteEvent.note = 60;
        noteEvent.setLength( 480 );
        sequence.track[1].getEvents()->add( noteEvent );

        ByteArrayOutputStream stream;
        VSQFileWriter writer;
        writer.write( &sequence, &stream, 500, "Shift_JIS", false );
        string actualString = stream.toString();
        stream.close();

        // 期待値と比較する
        vector<char> expected;
        string fixturePath = TestUtil::getFixtureRootPath() + "/VSQFileWriterTest/expected/expected.vsq";
        FILE *fileHandle = fopen( fixturePath.c_str(), "rb" );
        if( ! fileHandle ){
            CPPUNIT_FAIL( "fixture を読み込めなかった" );
        }
        while( 1 ){
            int c = fgetc( fileHandle );
            if( c == EOF ) break;
            expected.push_back( (char)c );
        }
        fclose( fileHandle );

        vector<char> actual;
        actual.insert( actual.begin(), actualString.begin(), actualString.end() );
        CPPUNIT_ASSERT( expected == actual );
    }

    /**
     * @todo
     */
    void testWriteWithPitch(){
    //    fail();
    }

    void test_getMidiEventsFromMetaText(){
        TextStream stream;
        // 「あ」が Shift_JIS になった時分割される「あ」を Shift_JIS にすると「0x82 0xA0」
        stream.write( StringUtil::repeat( "a", 118 ) + "あ" );
        stream.write( StringUtil::repeat( "b", 63 ) );
        VSQFileWriterStub writer;
        vector<MidiEvent> events = writer.getMidiEventsFromMetaText( &stream, "Shift_JIS" );
        CPPUNIT_ASSERT_EQUAL( (size_t)2, events.size() );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, events[0].clock );
        CPPUNIT_ASSERT_EQUAL( 0xFF, events[0].firstByte );
        CPPUNIT_ASSERT_EQUAL( (size_t)128, events[0].data.size() );
        CPPUNIT_ASSERT_EQUAL( 0x01, events[0].data[0] );
        ostringstream actual;
        for( int i = 1; i < events[0].data.size(); i++ ){
            actual << (char)events[0].data[i];
        }
        CPPUNIT_ASSERT_EQUAL( string( "DM:0000:" ) + StringUtil::repeat( "a", 118 ) + (char)0x82, actual.str() );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, events[1].clock );
        CPPUNIT_ASSERT_EQUAL( 0xFF, events[1].firstByte );
        CPPUNIT_ASSERT_EQUAL( (size_t)73, events[1].data.size() );
        CPPUNIT_ASSERT_EQUAL( 0x01, events[1].data[0] );
        actual.str( "" );
        for( int i = 1; i < events[1].data.size(); i++ ){
            actual << (char)events[1].data[i];
        }
        CPPUNIT_ASSERT_EQUAL( string( "DM:0001:" ) + (char)0xA0 + StringUtil::repeat( "b", 63 ), actual.str() );
    }

    void test_getLinePrefixBytes(){
        VSQFileWriterStub writer;

        // 4 桁
        vector<int> expected;
        expected.push_back( 0xff & 'D' );
        expected.push_back( 0xff & 'M' );
        expected.push_back( 0xff & ':' );
        expected.push_back( 0xff & '0' );
        expected.push_back( 0xff & '1' );
        expected.push_back( 0xff & '2' );
        expected.push_back( 0xff & '3' );
        expected.push_back( 0xff & ':' );
        vector<int> actual = writer.getLinePrefixBytes( 123 );
        CPPUNIT_ASSERT_EQUAL( expected.size(), actual.size() );
        for( int i = 0; i < expected.size(); i++ ){
            CPPUNIT_ASSERT_EQUAL( expected[i], actual[i] );
        }

        // 4 桁
        expected.clear();
        expected.push_back( 0xff & 'D' );
        expected.push_back( 0xff & 'M' );
        expected.push_back( 0xff & ':' );
        expected.push_back( 0xff & '9' );
        expected.push_back( 0xff & '9' );
        expected.push_back( 0xff & '9' );
        expected.push_back( 0xff & '9' );
        expected.push_back( 0xff & ':' );
        actual = writer.getLinePrefixBytes( 9999 );
        CPPUNIT_ASSERT_EQUAL( expected.size(), actual.size() );
        for( int i = 0; i < expected.size(); i++ ){
            CPPUNIT_ASSERT_EQUAL( expected[i], actual[i] );
        }

        // 8 桁
        expected.clear();
        expected.push_back( 0xff & 'D' );
        expected.push_back( 0xff & 'M' );
        expected.push_back( 0xff & ':' );
        expected.push_back( 0xff & '0' );
        expected.push_back( 0xff & '0' );
        expected.push_back( 0xff & '0' );
        expected.push_back( 0xff & '1' );
        expected.push_back( 0xff & '2' );
        expected.push_back( 0xff & '3' );
        expected.push_back( 0xff & '4' );
        expected.push_back( 0xff & '5' );
        expected.push_back( 0xff & ':' );
        actual = writer.getLinePrefixBytes( 12345 );
        CPPUNIT_ASSERT_EQUAL( expected.size(), actual.size() );
        for( int i = 0; i < expected.size(); i++ ){
            CPPUNIT_ASSERT_EQUAL( expected[i], actual[i] );
        }

        // 8 桁
        expected.clear();
        expected.push_back( 0xff & 'D' );
        expected.push_back( 0xff & 'M' );
        expected.push_back( 0xff & ':' );
        expected.push_back( 0xff & '0' );
        expected.push_back( 0xff & '1' );
        expected.push_back( 0xff & '2' );
        expected.push_back( 0xff & '3' );
        expected.push_back( 0xff & '4' );
        expected.push_back( 0xff & '5' );
        expected.push_back( 0xff & '6' );
        expected.push_back( 0xff & '7' );
        expected.push_back( 0xff & ':' );
        actual = writer.getLinePrefixBytes( 1234567 );
        CPPUNIT_ASSERT_EQUAL( expected.size(), actual.size() );
        for( int i = 0; i < expected.size(); i++ ){
            CPPUNIT_ASSERT_EQUAL( expected[i], actual[i] );
        }
    }

    void test_getHowManyDigits(){
        VSQFileWriterStub writer;
        CPPUNIT_ASSERT_EQUAL( 1, writer.getHowManyDigits( 0 ) );
        CPPUNIT_ASSERT_EQUAL( 1, writer.getHowManyDigits( 9 ) );
        CPPUNIT_ASSERT_EQUAL( 2, writer.getHowManyDigits( 99 ) );
        CPPUNIT_ASSERT_EQUAL( 10, writer.getHowManyDigits( 1000000000 ) );
        CPPUNIT_ASSERT_EQUAL( 2, writer.getHowManyDigits( -10 ) );
    }

    void test_writeUnsignedShort(){
        ByteArrayOutputStream stream;
        VSQFileWriterStub writer;
        writer.writeUnsignedShort( &stream, 0x8421 );
        string actual = stream.toString();
        ostringstream expected;
        expected << (char)0x84 << (char)0x21;
        CPPUNIT_ASSERT_EQUAL( expected.str(), actual );
    }

    void test_writeUnsignedInt(){
        ByteArrayOutputStream stream;
        VSQFileWriterStub writer;
        writer.writeUnsignedInt( &stream, 0x84212184 );
        string actual = stream.toString();
        ostringstream expected;
        expected << (char)0x84 << (char)0x21 << (char)0x21 << (char)0x84;
        CPPUNIT_ASSERT_EQUAL( expected.str(), actual );
    }

    CPPUNIT_TEST_SUITE( VSQFileWriterTest );
    CPPUNIT_TEST( testWriteWithoutPitch );
    CPPUNIT_TEST( testWriteWithPitch );
    CPPUNIT_TEST( test_getMidiEventsFromMetaText );
    CPPUNIT_TEST( test_getLinePrefixBytes );
    CPPUNIT_TEST( test_getHowManyDigits );
    CPPUNIT_TEST( test_writeUnsignedShort );
    CPPUNIT_TEST( test_writeUnsignedInt );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( VSQFileWriterTest );
