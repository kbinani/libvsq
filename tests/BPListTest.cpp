#include "Util.hpp"
#include "../BPList.hpp"

using namespace std;
using namespace VSQ_NS;

class BPListTest : public CppUnit::TestCase
{
public:
    void testConstruct()
    {
        BPList list( "foo", 63, -10, 1000 );
        CPPUNIT_ASSERT_EQUAL( string( "foo" ), list.getName() );
        CPPUNIT_ASSERT_EQUAL( 63, list.getDefault() );
        CPPUNIT_ASSERT_EQUAL( -10, list.getMinimum() );
        CPPUNIT_ASSERT_EQUAL( 1000, list.getMaximum() );
        CPPUNIT_ASSERT_EQUAL( 0, list.getMaxId() );
    }
    
    void testGetterAndSetterName()
    {
        BPList list( "foo", 63, -10, 1000 );
        CPPUNIT_ASSERT_EQUAL( string( "foo" ), list.getName() );
        list.setName( "bar" );
        CPPUNIT_ASSERT_EQUAL( string( "bar" ), list.getName() );
    }
    
    void testGetMaxId()
    {
        BPList list( "foo", 63, -10, 1000 );
        list.add( 0, 1 );
        CPPUNIT_ASSERT_EQUAL( 1, list.getMaxId() );
    }
    
    void testGetterAndSetterDefaultValue()
    {
        BPList list( "foo", 63, -10, 1000 );
        CPPUNIT_ASSERT_EQUAL( 63, list.getDefault() );
        list.setDefault( 62 );
        CPPUNIT_ASSERT_EQUAL( 62, list.getDefault() );
    }
    
    void testRenumberIds()
    {
        BPList list( "foo", 63, -10, 1000 );
        list.addWithId( 0, 1, 10000 );
        list.addWithId( 1, 2, 10001 );
        list.renumberIds();

        CPPUNIT_ASSERT( 10000 != list.get( 0 ).id );
        CPPUNIT_ASSERT_EQUAL( 1, list.get( 0 ).value );
        CPPUNIT_ASSERT_EQUAL( (tick_t)0, list.getKeyClock( 0 ) );

        CPPUNIT_ASSERT( 10001 != list.get( 1 ).id );
        CPPUNIT_ASSERT_EQUAL( 2, list.get( 1 ).value );
        CPPUNIT_ASSERT_EQUAL( (tick_t)1, list.getKeyClock( 1 ) );
    }
    
    void testGetData()
    {
        BPList list( "foo", 63, -10, 1000 );
        list.addWithId( 0, 101, 10000 );
        list.addWithId( 480, 103, 10001 );
        string expected = "0=101,480=103";
        string actual = list.getData();
        CPPUNIT_ASSERT_EQUAL( expected, actual );
    }
    
    void testSetData()
    {
        BPList list( "foo", 63, -10, 1000 );
        string value = "0=-11,240=50,480=1001";
        list.setData( value );
        string expected = "0=-10,240=50,480=1000";
        string actual = list.getData();
        CPPUNIT_ASSERT_EQUAL( expected, actual );
    }
    
    void testGetterAndSetterMaximum()
    {
        BPList list( "foo", 63, -10, 1000 );
        CPPUNIT_ASSERT_EQUAL( 1000, list.getMaximum() );
        list.setMaximum( 1001 );
        CPPUNIT_ASSERT_EQUAL( 1001, list.getMaximum() );
    }
    
    void testGetterAndSetterMinimum()
    {
        BPList list( "foo", 63, -10, 1000 );
        CPPUNIT_ASSERT_EQUAL( -10, list.getMinimum() );
        list.setMinimum( 1 );
        CPPUNIT_ASSERT_EQUAL( 1, list.getMinimum() );
    }
    
    void testRemove()
    {
        BPList list( "foo", 63, -10, 1000 );
        list.add( 0, 11 );
        list.add( 240, 13 );
        list.add( 480, 17 );

        list.remove( 240 );
        CPPUNIT_ASSERT_EQUAL( 2, list.size() );
        string expected = "0=11,480=17";
        string actual = list.getData();
        CPPUNIT_ASSERT_EQUAL( expected, actual );

        list.remove( 100 );
        CPPUNIT_ASSERT_EQUAL( 2, list.size() );
        actual = list.getData();
        CPPUNIT_ASSERT_EQUAL( expected, actual );
    }
    
    void testRemoveElementAt()
    {
        BPList list( "foo", 63, -10, 1000 );
        list.add( 0, 11 );
        list.add( 240, 13 );
        list.add( 480, 17 );

        list.removeElementAt( 1 );
        string expected = "0=11,480=17";
        string actual = list.getData();
        CPPUNIT_ASSERT_EQUAL( expected, actual );
    }
    
    void testIsContainsKey()
    {
        BPList list( "foo", 63, -10, 1000 );
        CPPUNIT_ASSERT( false == list.isContainsKey( 480 ) );
        list.add( 480, 1 );
        CPPUNIT_ASSERT( list.isContainsKey( 480 ) );
    }
    
    void testMove()
    {
        // 移動先にデータ点がない場合
        BPList listA( "foo", 63, -10, 1000 );
        listA.addWithId( 0, 11, 1 );
        listA.addWithId( 240, 13, 2 );
        listA.addWithId( 480, 17, 3 );
        listA.move( 240, 481, 19 );

        string expected = "0=11,480=17,481=19";
        assertEqual( expected, listA.getData() );
        assertEqual( 3, listA.size() );

        assertEqual( 11, listA.get( 0 ).value );
        assertEqual( (tick_t)0, listA.getKeyClock( 0 ) );
        assertEqual( 1, listA.get( 0 ).id );

        assertEqual( 17, listA.get( 1 ).value );
        assertEqual( (tick_t)480, listA.getKeyClock( 1 ) );
        assertEqual( 3, listA.get( 1 ).id );

        assertEqual( 19, listA.get( 2 ).value );
        assertEqual( (tick_t)481, listA.getKeyClock( 2 ) );
        assertEqual( 2, listA.get( 2 ).id );

        // 移動先にデータがある場合
        BPList listB( "foo", 63, -10, 1000 );
        listB.addWithId( 0, 11, 1 );
        listB.addWithId( 240, 13, 2 );
        listB.addWithId( 480, 17, 3 );
        listB.move( 240, 480, 19 );

        expected = "0=11,480=19";
        assertEqual( 2, listB.size() );
        assertEqual( expected, listB.getData() );

        assertEqual( 11, listB.get( 0 ).value );
        assertEqual( (tick_t)0, listB.getKeyClock( 0 ) );
        assertEqual( 1, listB.get( 0 ).id );

        assertEqual( 19, listB.get( 1 ).value );
        assertEqual( (tick_t)480, listB.getKeyClock( 1 ) );
        assertEqual( 2, listB.get( 1 ).id );
    }
    
    void testGetValue()
    {
        BPList list( "foo", 63, -10, 1000 );
        list.add( 480, 1 );
        list.add( 1920, 2 );
        CPPUNIT_ASSERT_EQUAL( 1, list.getValue( 0 ) );
        CPPUNIT_ASSERT_EQUAL( 2, list.getValue( 1 ) );
    }
    
    void testGet()
    {
        BPList list( "foo", 63, -10, 1000 );
        list.add( 480, 11 );
        list.add( 1920, 12 );
        CPPUNIT_ASSERT_EQUAL( 1, list.get( 0 ).id );
        CPPUNIT_ASSERT_EQUAL( 11, list.get( 0 ).value );
        CPPUNIT_ASSERT_EQUAL( 2, list.get( 1 ).id );
        CPPUNIT_ASSERT_EQUAL( 12, list.get( 1 ).value );
    }
    
    void testGetKeyClock()
    {
        BPList list( "foo", 63, -10, 1000 );
        list.add( 480, 11 );
        list.add( 1920, 12 );
        CPPUNIT_ASSERT_EQUAL( (tick_t)480, list.getKeyClock( 0 ) );
        CPPUNIT_ASSERT_EQUAL( (tick_t)1920, list.getKeyClock( 1 ) );
    }
    
    void testFindValueFromId()
    {
        BPList list( "foo", 63, -10, 1000 );
        int idA = list.add( 480, 11 );
        int idB = list.add( 1920, 12 );
        CPPUNIT_ASSERT_EQUAL( 11, list.findValueFromId( idA ) );
        CPPUNIT_ASSERT_EQUAL( 12, list.findValueFromId( idB ) );
    }
    
    void testFindElement()
    {
        BPList list( "foo", 63, -10, 1000 );
        int idA = list.add( 480, 11 );
        int idB = list.add( 1920, 12 );
        BPListSearchResult resultA = list.findElement( idA );
        BPListSearchResult resultB = list.findElement( idB );
        CPPUNIT_ASSERT_EQUAL( (tick_t)480, resultA.clock );
        CPPUNIT_ASSERT_EQUAL( 0, resultA.index );
        CPPUNIT_ASSERT_EQUAL( idA, resultA.point.id );
        CPPUNIT_ASSERT_EQUAL( 11, resultA.point.value );
        CPPUNIT_ASSERT_EQUAL( (tick_t)1920, resultB.clock );
        CPPUNIT_ASSERT_EQUAL( 1, resultB.index );
        CPPUNIT_ASSERT_EQUAL( idB, resultB.point.id );
        CPPUNIT_ASSERT_EQUAL( 12, resultB.point.value );
    }
    
    void testSetValueForId()
    {
        BPList list( "foo", 63, -10, 1000 );
        int idA = list.add( 480, 11 );
        list.add( 1920, 12 );
        list.setValueForId( idA, 13 );
        CPPUNIT_ASSERT_EQUAL( 13, list.findValueFromId( idA ) );
    }
    
    void testPrint()
    {
        BPList list( "foo", 63, -10, 1000 );
        string header = "[BPList]";
    
        TextStream stream;
        list.print( stream, 0, header );
        string expected =
            "[BPList]\n";
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
        stream.close();

        stream = TextStream();
        list.add( 480, 11 );
        list.add( 1920, 12 );
        list.print( stream, 0, header );
        expected =
            "[BPList]\n"
            "480=11\n"
            "1920=12\n";
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
        stream.close();
    
        stream = TextStream();
        list.print( stream, 1921, header );
        expected =
            "[BPList]\n"
            "1921=12\n";
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    }
    
    void testAppendFromText()
    {
        BPList list( "foo", 63, -10, 1000 );
        TextStream stream;
        stream.writeLine( "0=11" );
        stream.writeLine( "340=13" );
        stream.writeLine( "480=17" );
        stream.writeLine( "[foooo]" );
        stream.writeLine( "481=19" );
        stream.setPointer( -1 );
        string lastLine = list.appendFromText( stream );
        string expected = "0=11,340=13,480=17";
        assertEqual( expected, list.getData() );
        assertEqual( string( "[foooo]" ), lastLine );
    }
    
    void testSize()
    {
        BPList list( "foo", 63, -10, 1000 );
        CPPUNIT_ASSERT_EQUAL( 0, list.size() );
        list.add( 480, 11 );
        CPPUNIT_ASSERT_EQUAL( 1, list.size() );
    }
    
    void testKeyClockIterator()
    {
        BPList list( "foo", 63, -10, 1000 );
        list.add( 480, 11 );
        list.add( 1920, 12 );
        BPList::KeyClockIterator iterator = list.keyClockIterator();
        CPPUNIT_ASSERT( iterator.hasNext() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)480, iterator.next() );
        CPPUNIT_ASSERT( iterator.hasNext() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)1920, iterator.next() );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    }
    
    void testAdd()
    {
        BPList list( "foo", 63, -10, 1000 );
        CPPUNIT_ASSERT_EQUAL( 0, list.size() );
        int idA = list.add( 480, 11 );
        CPPUNIT_ASSERT_EQUAL( 1, list.size() );
        CPPUNIT_ASSERT_EQUAL( 1, idA );
        CPPUNIT_ASSERT_EQUAL( 11, list.getValue( 0 ) );
    
        // 同じclockに値をaddすると、データ点は増えずに値が上書きされる
        idA = list.add( 480, 12 );
        CPPUNIT_ASSERT_EQUAL( 1, list.size() );
        CPPUNIT_ASSERT_EQUAL( 1, idA );
        CPPUNIT_ASSERT_EQUAL( 12, list.getValue( 0 ) );
    
        // 既存の点より小さいclockに値をaddすると、並び替えが起こる
        int idB = list.add( 240, 99 );
        CPPUNIT_ASSERT_EQUAL( 2, list.size() );
        CPPUNIT_ASSERT( idA != idB );
        CPPUNIT_ASSERT_EQUAL( (tick_t)240, list.getKeyClock( 0 ) );
        CPPUNIT_ASSERT_EQUAL( idB, list.get( 0 ).id );
        CPPUNIT_ASSERT_EQUAL( 99, list.get( 0 ).value );
        CPPUNIT_ASSERT_EQUAL( (tick_t)480, list.getKeyClock( 1 ) );
        CPPUNIT_ASSERT_EQUAL( idA, list.get( 1 ).id );
        CPPUNIT_ASSERT_EQUAL( 12, list.get( 1 ).value );
    }
    
    void testAddWithId()
    {
        BPList list( "foo", 63, -10, 1000 );
        int idA = list.addWithId( 480, 11, 3 );
        CPPUNIT_ASSERT_EQUAL( 3, idA );
        CPPUNIT_ASSERT_EQUAL( 11, list.getValue( 0 ) );
        CPPUNIT_ASSERT_EQUAL( 3, list.getMaxId() );
    
        // 同じclockに値をaddすると、データ点は増えずに値が上書きされる
        idA = list.addWithId( 480, 12, 4 );
        CPPUNIT_ASSERT_EQUAL( 4, idA );
        CPPUNIT_ASSERT_EQUAL( 12, list.getValue( 0 ) );
        CPPUNIT_ASSERT_EQUAL( 4, list.getMaxId() );
    
        // 既存の点より小さいclockに値をaddすると、並び替えが起こる
        int idB = list.addWithId( 240, 99, 5 );
        CPPUNIT_ASSERT_EQUAL( 5, idB );
        CPPUNIT_ASSERT_EQUAL( 5, list.getMaxId() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)240, list.getKeyClock( 0 ) );
        CPPUNIT_ASSERT_EQUAL( 5, list.get( 0 ).id );
        CPPUNIT_ASSERT_EQUAL( 99, list.get( 0 ).value );
        CPPUNIT_ASSERT_EQUAL( (tick_t)480, list.getKeyClock( 1 ) );
        CPPUNIT_ASSERT_EQUAL( 4, list.get( 1 ).id );
        CPPUNIT_ASSERT_EQUAL( 12, list.get( 1 ).value );
    }
    
    void testRemoveWithId()
    {
        BPList list( "foo", 63, -10, 1000 );
        list.addWithId( 480, 11, 1 );
        list.addWithId( 1920, 12, 2 );

        list.removeWithId( 1 );
        string expected = "1920=12";
        assertEqual( expected, list.getData() );

        // 存在しないIDを削除しようとする
        list.removeWithId( 1 );
        assertEqual( expected, list.getData() );
    }
    
    void testGetValueAtWithoutLastIndex()
    {
        BPList list( "foo", 63, -10, 1000 );
        list.add( 480, 11 );
        list.add( 1920, 12 );
        CPPUNIT_ASSERT_EQUAL( 63, list.getValueAt( 479 ) );
        CPPUNIT_ASSERT_EQUAL( 11, list.getValueAt( 480 ) );
        CPPUNIT_ASSERT_EQUAL( 12, list.getValueAt( 2000 ) );
    }
    
    void testGetValueAtWithLastIndex()
    {
        BPList list( "foo", 63, -10, 1000 );
        list.add( 480, 11 );
        list.add( 1920, 12 );
        int index = 0;
        CPPUNIT_ASSERT_EQUAL( 63, list.getValueAt( 479, &index ) );
        CPPUNIT_ASSERT_EQUAL( 0, index );
        CPPUNIT_ASSERT_EQUAL( 11, list.getValueAt( 480, &index ) );
        CPPUNIT_ASSERT_EQUAL( 0, index );
        CPPUNIT_ASSERT_EQUAL( 12, list.getValueAt( 2000, &index ) );
        CPPUNIT_ASSERT_EQUAL( 1, index );
        CPPUNIT_ASSERT_EQUAL( 63, list.getValueAt( 479, &index ) );
        CPPUNIT_ASSERT_EQUAL( 0, index );
    }

    CPPUNIT_TEST_SUITE( BPListTest );
    CPPUNIT_TEST( testConstruct );
    CPPUNIT_TEST( testGetterAndSetterName );
    CPPUNIT_TEST( testGetMaxId );
    CPPUNIT_TEST( testGetterAndSetterDefaultValue );
    CPPUNIT_TEST( testRenumberIds );
    CPPUNIT_TEST( testGetData );
    CPPUNIT_TEST( testSetData );
    CPPUNIT_TEST( testGetterAndSetterMaximum );
    CPPUNIT_TEST( testGetterAndSetterMinimum );
    CPPUNIT_TEST( testRemove );
    CPPUNIT_TEST( testRemoveElementAt );
    CPPUNIT_TEST( testIsContainsKey );
    CPPUNIT_TEST( testMove );
    CPPUNIT_TEST( testGetValue );
    CPPUNIT_TEST( testGet );
    CPPUNIT_TEST( testGetKeyClock );
    CPPUNIT_TEST( testFindValueFromId );
    CPPUNIT_TEST( testFindElement );
    CPPUNIT_TEST( testSetValueForId );
    CPPUNIT_TEST( testPrint );
    CPPUNIT_TEST( testAppendFromText );
    CPPUNIT_TEST( testSize );
    CPPUNIT_TEST( testKeyClockIterator );
    CPPUNIT_TEST( testAdd );
    CPPUNIT_TEST( testAddWithId );
    CPPUNIT_TEST( testRemoveWithId );
    CPPUNIT_TEST( testGetValueAtWithoutLastIndex );
    CPPUNIT_TEST( testGetValueAtWithLastIndex );
    CPPUNIT_TEST_SUITE_END();
};

class BPListKeyClockIteratorTest : public CppUnit::TestCase
{
public:
    void test()
    {
        BPList list( "foo", 63, -10, 1000 );
        BPList::KeyClockIterator iterator = BPList::KeyClockIterator( &list );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
        list.add( 480, 1 );
        list.add( 1920, 2 );

        iterator = BPList::KeyClockIterator( &list );
        CPPUNIT_ASSERT( iterator.hasNext() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)480, iterator.next() );
        CPPUNIT_ASSERT( iterator.hasNext() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)1920, iterator.next() );
        CPPUNIT_ASSERT( false == iterator.hasNext() );

        iterator = BPList::KeyClockIterator( &list );
        CPPUNIT_ASSERT( iterator.hasNext() );
        CPPUNIT_ASSERT_EQUAL( (tick_t)480, iterator.next() );
        iterator.remove();
        CPPUNIT_ASSERT_EQUAL( (tick_t)1920, iterator.next() );
        CPPUNIT_ASSERT( false == iterator.hasNext() );
    }

    CPPUNIT_TEST_SUITE( BPListKeyClockIteratorTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( BPListTest );
REGISTER_TEST_SUITE( BPListKeyClockIteratorTest );
