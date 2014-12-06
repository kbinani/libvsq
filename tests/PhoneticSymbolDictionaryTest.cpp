#include "Util.hpp"
#include "../include/libvsq/PhoneticSymbolDictionary.hpp"

using namespace std;
using namespace vsq;

TEST(PhoneticSymbolDictionaryTest, ctorElement)
{
	{
		PhoneticSymbolDictionary::Element actual("a-" "\t" "corn", "eI" "\t" "k O@ n");
		EXPECT_EQ(string("a-" "\t" "corn"), actual.word());
		EXPECT_EQ(string("eI" "\t" "k O@ n"), actual.rawSymbol());
		EXPECT_EQ(string("eI k O@ n"), actual.symbol());
	}

	{
		PhoneticSymbolDictionary::Element actual("ふぇ", "p\\ e");
		EXPECT_EQ(string("ふぇ"), actual.word());
		EXPECT_EQ(string("p\\ e"), actual.rawSymbol());
		EXPECT_EQ(string("p\\ e"), actual.symbol());
	}
}

TEST(PhoneticSymbolDictionaryTest, read)
{
	string fixture = "PhoneticSymbolDictionaryTest/fixture/dict_en.txt";
	PhoneticSymbolDictionary dictionary(fixture);
	EXPECT_EQ(3, dictionary.maxDivisions());

	{
		// ABS
		const PhoneticSymbolDictionary::Element* actual = dictionary.attach("ABS");
		EXPECT_TRUE(0 != actual);
		EXPECT_EQ(string("a-" "\t" "b-" "\t" "s"), actual->word());
		EXPECT_EQ(string("eI" "\t" "b i:" "\t" "e s"), actual->rawSymbol());
		EXPECT_EQ(string("eI b i: e s"), actual->symbol());
	}

	{
		// lain
		const PhoneticSymbolDictionary::Element* actual = dictionary.attach("lain");
		EXPECT_TRUE(0 != actual);
		EXPECT_EQ(string("lain"), actual->word());
		EXPECT_EQ(string("l0 eI n"), actual->rawSymbol());
		EXPECT_EQ(string("l0 eI n"), actual->symbol());
	}

	{
		// unknown word
		const PhoneticSymbolDictionary::Element* actual = dictionary.attach("hogehoge");
		EXPECT_TRUE(0 == actual);
	}
}
