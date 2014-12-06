#include "../include/libvsq/PhoneticSymbol.hpp"
#include "Util.hpp"

using namespace std;
using namespace vsq;

class PhoneticSymbolTest : public CppUnit::TestCase
{
public:
	void testIsConsonant()
	{
		CPPUNIT_ASSERT(PhoneticSymbol::isConsonant("k'"));
		CPPUNIT_ASSERT(false == PhoneticSymbol::isConsonant("a"));
	}

	void testIsVowel()
	{
		CPPUNIT_ASSERT(PhoneticSymbol::isVowel("@"));
		CPPUNIT_ASSERT(false == PhoneticSymbol::isVowel("b"));
	}

	void testIsValidSymbol()
	{
		CPPUNIT_ASSERT(PhoneticSymbol::isValidSymbol("a"));
		CPPUNIT_ASSERT(PhoneticSymbol::isValidSymbol("br1"));
		CPPUNIT_ASSERT(false == PhoneticSymbol::isValidSymbol("__INVALID_SYMBOL__"));
		CPPUNIT_ASSERT(false == PhoneticSymbol::isValidSymbol("br_"));
	}

	CPPUNIT_TEST_SUITE(PhoneticSymbolTest);
	CPPUNIT_TEST(testIsConsonant);
	CPPUNIT_TEST(testIsVowel);
	CPPUNIT_TEST(testIsValidSymbol);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(PhoneticSymbolTest);
