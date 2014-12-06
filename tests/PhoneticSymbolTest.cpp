#include "../include/libvsq/PhoneticSymbol.hpp"
#include "Util.hpp"

using namespace std;
using namespace vsq;

TEST(PhoneticSymbolTest, testIsConsonant)
{
	EXPECT_TRUE(PhoneticSymbol::isConsonant("k'"));
	EXPECT_TRUE(false == PhoneticSymbol::isConsonant("a"));
}

TEST(PhoneticSymbolTest, testIsVowel)
{
	EXPECT_TRUE(PhoneticSymbol::isVowel("@"));
	EXPECT_TRUE(false == PhoneticSymbol::isVowel("b"));
}

TEST(PhoneticSymbolTest, testIsValidSymbol)
{
	EXPECT_TRUE(PhoneticSymbol::isValidSymbol("a"));
	EXPECT_TRUE(PhoneticSymbol::isValidSymbol("br1"));
	EXPECT_TRUE(false == PhoneticSymbol::isValidSymbol("__INVALID_SYMBOL__"));
	EXPECT_TRUE(false == PhoneticSymbol::isValidSymbol("br_"));
}
