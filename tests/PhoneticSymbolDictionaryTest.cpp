#include "Util.hpp"
#include "../include/libvsq/PhoneticSymbolDictionary.hpp"

using namespace std;
using namespace vsq;

class PhoneticSymbolDictionaryTest : public CppUnit::TestCase
{
public:
	void ctorElement()
	{
		{
			PhoneticSymbolDictionary::Element actual("a-" "\t" "corn", "eI" "\t" "k O@ n");
			CPPUNIT_ASSERT_EQUAL(string("a-" "\t" "corn"), actual.word());
			CPPUNIT_ASSERT_EQUAL(string("eI" "\t" "k O@ n"), actual.rawSymbol());
			CPPUNIT_ASSERT_EQUAL(string("eI k O@ n"), actual.symbol());
		}

		{
			PhoneticSymbolDictionary::Element actual("ふぇ", "p\\ e");
			CPPUNIT_ASSERT_EQUAL(string("ふぇ"), actual.word());
			CPPUNIT_ASSERT_EQUAL(string("p\\ e"), actual.rawSymbol());
			CPPUNIT_ASSERT_EQUAL(string("p\\ e"), actual.symbol());
		}
	}

	void read()
	{
		string fixture = TestUtil::getFixtureRootPath() + "/PhoneticSymbolDictionaryTest/fixture/dict_en.txt";
		PhoneticSymbolDictionary dictionary(fixture);
		CPPUNIT_ASSERT_EQUAL(3, dictionary.maxDivisions());

		{
			// ABS
			const PhoneticSymbolDictionary::Element* actual = dictionary.attach("ABS");
			CPPUNIT_ASSERT(0 != actual);
			CPPUNIT_ASSERT_EQUAL(string("a-" "\t" "b-" "\t" "s"), actual->word());
			CPPUNIT_ASSERT_EQUAL(string("eI" "\t" "b i:" "\t" "e s"), actual->rawSymbol());
			CPPUNIT_ASSERT_EQUAL(string("eI b i: e s"), actual->symbol());
		}

		{
			// lain
			const PhoneticSymbolDictionary::Element* actual = dictionary.attach("lain");
			CPPUNIT_ASSERT(0 != actual);
			CPPUNIT_ASSERT_EQUAL(string("lain"), actual->word());
			CPPUNIT_ASSERT_EQUAL(string("l0 eI n"), actual->rawSymbol());
			CPPUNIT_ASSERT_EQUAL(string("l0 eI n"), actual->symbol());
		}

		{
			// unknown word
			const PhoneticSymbolDictionary::Element* actual = dictionary.attach("hogehoge");
			CPPUNIT_ASSERT(0 == actual);
		}
	}

	CPPUNIT_TEST_SUITE(PhoneticSymbolDictionaryTest);
	CPPUNIT_TEST(ctorElement);
	CPPUNIT_TEST(read);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(PhoneticSymbolDictionaryTest);
