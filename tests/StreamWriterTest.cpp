#include "Util.hpp"
#include "../StreamWriter.hpp"

using namespace std;
using namespace VSQ_NS;

class StreamWriterTest : public CppUnit::TestCase{
public:
    void test(){
        StreamWriter writer( "foo.txt" );
        writer.write( "foo" );
        writer.writeLine( "bar" );
        writer.close();

        std::ostringstream expected;
        expected << "foobar" << (char)0x0A;

        std::ostringstream actual;
        FILE *file = fopen( "foo.txt", "rb" );
        int c;
        while( (c = fgetc( file )) >= 0 ){
            actual << (char)c;
        }
        fclose( file );

        CPPUNIT_ASSERT_EQUAL( expected.str(), actual.str() );
    }

    CPPUNIT_TEST_SUITE( StreamWriterTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( StreamWriterTest );
