#include "Util.hpp"
#include "../NoteNumberUtil.hpp"

using namespace std;
using namespace VSQ_NS;

class NoteNumberUtilTest : public CppUnit::TestCase{
public:
    void getNoteStringBase(){
        CPPUNIT_ASSERT_EQUAL( string( "C" ), NoteNumberUtil::getNoteStringBase( 60 ) );
        CPPUNIT_ASSERT_EQUAL( string( "C" ), NoteNumberUtil::getNoteStringBase( 61 ) );
        CPPUNIT_ASSERT_EQUAL( string( "D" ), NoteNumberUtil::getNoteStringBase( 62 ) );
        CPPUNIT_ASSERT_EQUAL( string( "E" ), NoteNumberUtil::getNoteStringBase( 63 ) );
        CPPUNIT_ASSERT_EQUAL( string( "E" ), NoteNumberUtil::getNoteStringBase( 64 ) );
        CPPUNIT_ASSERT_EQUAL( string( "F" ), NoteNumberUtil::getNoteStringBase( 65 ) );
        CPPUNIT_ASSERT_EQUAL( string( "F" ), NoteNumberUtil::getNoteStringBase( 66 ) );
        CPPUNIT_ASSERT_EQUAL( string( "G" ), NoteNumberUtil::getNoteStringBase( 67 ) );
        CPPUNIT_ASSERT_EQUAL( string( "G" ), NoteNumberUtil::getNoteStringBase( 68 ) );
        CPPUNIT_ASSERT_EQUAL( string( "A" ), NoteNumberUtil::getNoteStringBase( 69 ) );
        CPPUNIT_ASSERT_EQUAL( string( "B" ), NoteNumberUtil::getNoteStringBase( 70 ) );
        CPPUNIT_ASSERT_EQUAL( string( "B" ), NoteNumberUtil::getNoteStringBase( 71 ) );
    }

    void getNoteOctave(){
        CPPUNIT_ASSERT_EQUAL( 3, NoteNumberUtil::getNoteOctave( 61 ) );
    }

    void getNoteAlter(){
        CPPUNIT_ASSERT_EQUAL( 0, NoteNumberUtil::getNoteAlter( 60 ) );
        CPPUNIT_ASSERT_EQUAL( 1, NoteNumberUtil::getNoteAlter( 61 ) );
        CPPUNIT_ASSERT_EQUAL( 0, NoteNumberUtil::getNoteAlter( 62 ) );
        CPPUNIT_ASSERT_EQUAL( -1, NoteNumberUtil::getNoteAlter( 63 ) );
        CPPUNIT_ASSERT_EQUAL( 0, NoteNumberUtil::getNoteAlter( 64 ) );
        CPPUNIT_ASSERT_EQUAL( 0, NoteNumberUtil::getNoteAlter( 65 ) );
        CPPUNIT_ASSERT_EQUAL( 1, NoteNumberUtil::getNoteAlter( 66 ) );
        CPPUNIT_ASSERT_EQUAL( 0, NoteNumberUtil::getNoteAlter( 67 ) );
        CPPUNIT_ASSERT_EQUAL( 1, NoteNumberUtil::getNoteAlter( 68 ) );
        CPPUNIT_ASSERT_EQUAL( 0, NoteNumberUtil::getNoteAlter( 69 ) );
        CPPUNIT_ASSERT_EQUAL( -1, NoteNumberUtil::getNoteAlter( 70 ) );
        CPPUNIT_ASSERT_EQUAL( 0, NoteNumberUtil::getNoteAlter( 71 ) );

        CPPUNIT_ASSERT_EQUAL( 1, NoteNumberUtil::getNoteAlter( -11 ) );
    }

    CPPUNIT_TEST_SUITE( NoteNumberUtilTest );
    CPPUNIT_TEST( getNoteStringBase );
    CPPUNIT_TEST( getNoteOctave );
    CPPUNIT_TEST( getNoteAlter );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( NoteNumberUtilTest );
