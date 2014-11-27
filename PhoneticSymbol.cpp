/**
 * PhoneticSymbol.cpp
 * Copyright © 2014 kbinani
 *
 * This file is part of libvsq.
 *
 * libvsq is free software; you can redistribute it and/or
 * modify it under the terms of the BSD License.
 *
 * libvsq is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 */
#include "./PhoneticSymbol.hpp"
#include "./StringUtil.hpp"

VSQ_BEGIN_NAMESPACE

bool PhoneticSymbol::isConsonant(std::string const& symbol)
{
	std::string search = "\t" + symbol + "\t";
	return isConsonantEN(search) || isConsonantJP(search);
}

bool PhoneticSymbol::isVowel(std::string const& symbol)
{
	std::string search = "\t" + symbol + "\t";
	return isVowelEN(search) || isVowelJP(search);
}

bool PhoneticSymbol::isValidSymbol(std::string const& symbol)
{
	bool vowel = isVowel(symbol);
	bool consonant = isConsonant(symbol);
	if (vowel || consonant) {
		return true;
	}

	// ブレスの判定
	int symbolCharacterCount = symbol.size();
	if (symbol.find("br") == 0 && symbolCharacterCount > 2) {
		// br001とかをfalseにするためのチェック
		std::string s = symbol.substr(2);
		try {
			StringUtil::parseInt<int>(s);
			return true;
		} catch (StringUtil::IntegerParseException&) {
			return false;
		}
	}
	return false;
}

bool PhoneticSymbol::isVowelJP(std::string const& search)
{
	const std::string symbolVowelJP = "\ta\ti\tM\te\to\t";
	return symbolVowelJP.find(search) != std::string::npos;
}

bool PhoneticSymbol::isConsonantJP(std::string const& search)
{
	const std::string symbolConsonantJP = "\tk\tk'\tg\tg'\tN\tN'\ts\tS\tz\tZ\tdz\tdZ\tt\tt'\tts\ttS\td\td'\tn\tJ\th\th\\\tC\tp\\\tp\\'\tb\tb'\tp\tp'\tm\tm'\tj\t4\t4'\tw\tN\\\t";
	return symbolConsonantJP.find(search) != std::string::npos;
}

bool PhoneticSymbol::isVowelEN(std::string const& search)
{
	const std::string symbolVowelEN = "\t@\tV\te\te\tI\ti:\t{\tO:\tQ\tU\tu:\t@r\teI\taI\tOI\t@U\taU\tI@\te@\tU@\tO@\tQ@\t";
	return symbolVowelEN.find(search) != std::string::npos;
}

bool PhoneticSymbol::isConsonantEN(std::string const& search)
{
	const std::string symbolConsonantEN = "\tw\tj\tb\td\tg\tbh\tdh\tgh\tdZ\tv\tD\tz\tZ\tm\tn\tN\tr\tl\tl0\tp\tt\tk\tph\tth\tkh\ttS\tf\tT\ts\tS\th\tSil\tAsp\t";
	return symbolConsonantEN.find(search) != std::string::npos;
}

VSQ_END_NAMESPACE
