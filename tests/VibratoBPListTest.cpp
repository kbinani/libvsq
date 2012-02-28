#include "Util.h"
#include "../VibratoBPList.h"

using namespace std;
using namespace VSQ_NS;

class VibratoBPListTest : public CppUnit::TestCase
{
public:
    void testConstructWithString()
    {
        VibratoBPList list = VibratoBPList( "2", "1.0,0.0", "128,1" );
        CPPUNIT_ASSERT_EQUAL( 2, list.size() );
        CPPUNIT_ASSERT_EQUAL( string( "0=1,1=128" ), list.getData() );
    }

    void testConstructWithArray()
    {
        vector<double> x;
        x.push_back( 1.0 );
        x.push_back( 0.0 );
        vector<int> y;
        y.push_back( 128 );
        y.push_back( 1 );
        VibratoBPList list = VibratoBPList( x, y );
        CPPUNIT_ASSERT_EQUAL( 2, list.size() );
        CPPUNIT_ASSERT_EQUAL( string( "0=1,1=128" ), list.getData() );
    }

    void testGetValueAt()
    {
        VibratoBPList list = VibratoBPList( "2", "0.0,1.0", "1,128" );
        CPPUNIT_ASSERT_EQUAL( 1, list.getValueAt( 0.0, 64 ) );
        CPPUNIT_ASSERT_EQUAL( 1, list.getValueAt( 0.99999, 64 ) );
        CPPUNIT_ASSERT_EQUAL( 128, list.getValueAt( 1.0, 64 ) );
        CPPUNIT_ASSERT_EQUAL( 64, list.getValueAt( -0.0000001, 64 ) );
    }

    void testGetCount()
    {
        VibratoBPList list = VibratoBPList( "3", "0.0,0.4,1.0", "1,64,128" );
        CPPUNIT_ASSERT_EQUAL( 3, list.size() );
    }

    void testGet()
    {
        VibratoBPList list = VibratoBPList( "3", "0.4,0.0,1.0", "64,1,128" );
        CPPUNIT_ASSERT_EQUAL( 0.0, list.get( 0 ).x );
        CPPUNIT_ASSERT_EQUAL( 1, list.get( 0 ).y );
        CPPUNIT_ASSERT_EQUAL( 0.4, list.get( 1 ).x );
        CPPUNIT_ASSERT_EQUAL( 64, list.get( 1 ).y );
        CPPUNIT_ASSERT_EQUAL( 1.0, list.get( 2 ).x );
        CPPUNIT_ASSERT_EQUAL( 128, list.get( 2 ).y );
    }

    void testSet()
    {
        VibratoBPList list = VibratoBPList( "3", "0.0,0.4,1.0", "1,64,128" );
        list.set( 1, VibratoBP( 0.2, 32 ) );
        CPPUNIT_ASSERT_EQUAL( 0.2, list.get( 1 ).x );
        CPPUNIT_ASSERT_EQUAL( 32, list.get( 1 ).y );
    }

    void testGetData()
    {
        VibratoBPList list = VibratoBPList( "3", "0.0,0.4,1.0", "1,64,128" );
        CPPUNIT_ASSERT_EQUAL( string( "0=1,0.4=64,1=128" ), list.getData() );
        list = VibratoBPList( vector<double>(), vector<int>() );
        CPPUNIT_ASSERT_EQUAL( string( "" ), list.getData() );
    }

    void testSetData()
    {
        VibratoBPList list = VibratoBPList( vector<double>(), vector<int>() );
        list.setData( "0.4=64,0=1,1=128" );
        CPPUNIT_ASSERT_EQUAL( 3, list.size() );
        CPPUNIT_ASSERT_EQUAL( 0.0, list.get( 0 ).x );
        CPPUNIT_ASSERT_EQUAL( 1, list.get( 0 ).y );
        CPPUNIT_ASSERT_EQUAL( 0.4, list.get( 1 ).x );
        CPPUNIT_ASSERT_EQUAL( 64, list.get( 1 ).y );
        CPPUNIT_ASSERT_EQUAL( 1.0, list.get( 2 ).x );
        CPPUNIT_ASSERT_EQUAL( 128, list.get( 2 ).y );
    }

    CPPUNIT_TEST_SUITE( VibratoBPListTest );
    CPPUNIT_TEST( testConstructWithString );
    CPPUNIT_TEST( testConstructWithArray );
    CPPUNIT_TEST( testGetValueAt );
    CPPUNIT_TEST( testGetCount );
    CPPUNIT_TEST( testGet );
    CPPUNIT_TEST( testSet );
    CPPUNIT_TEST( testGetData );
    CPPUNIT_TEST( testSetData );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( VibratoBPListTest );
