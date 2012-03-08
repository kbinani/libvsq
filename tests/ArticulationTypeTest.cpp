#include "Util.hpp"
#include "../ArticulationType.hpp"

using namespace VSQ_NS;

class ArticulationTypeTest : public CppUnit::TestCase
{
public:
    void test()
    {
        ArticulationType::ArticulationTypeEnum e
            = ArticulationType::CRESCENDO;
        CPPUNIT_ASSERT( e != ArticulationType::DYNAFF );
        CPPUNIT_ASSERT( e == ArticulationType::CRESCENDO );
    }

    CPPUNIT_TEST_SUITE( ArticulationTypeTest );
    CPPUNIT_TEST( test );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( ArticulationTypeTest );
