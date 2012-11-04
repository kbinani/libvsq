#include "Util.hpp"
#include "../PhoneticSymbolElement.hpp"

using namespace std;
using namespace VSQ_NS;

class PhoneticSymbolElementTest : public CppUnit::TestCase{
public:
    void ctor(){
        {
            PhoneticSymbolElement actual("a-" "\t" "corn", "eI" "\t" "k O@ n");
            CPPUNIT_ASSERT_EQUAL(string("a-" "\t" "corn"), actual.word());
            CPPUNIT_ASSERT_EQUAL(string("eI" "\t" "k O@ n"), actual.rawSymbol());
            CPPUNIT_ASSERT_EQUAL(string("eI k O@ n"), actual.symbol());
        }

        {
            PhoneticSymbolElement actual("ふぇ", "p\\ e");
            CPPUNIT_ASSERT_EQUAL(string("ふぇ"), actual.word());
            CPPUNIT_ASSERT_EQUAL(string("p\\ e"), actual.rawSymbol());
            CPPUNIT_ASSERT_EQUAL(string("p\\ e"), actual.symbol());
        }
    }

    CPPUNIT_TEST_SUITE(PhoneticSymbolElementTest);
    CPPUNIT_TEST(ctor);
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(PhoneticSymbolElementTest);
