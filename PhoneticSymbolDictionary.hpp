/**
 * PhoneticSymbolDictionary.hpp
 * Copyright © 2012 kbinani
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
#ifndef LIBVSQ_PHONETICSYMBOLCONVERTER_HPP_
#define LIBVSQ_PHONETICSYMBOLCONVERTER_HPP_

#include "vsqglobal.hpp"
#include "StringUtil.hpp"
#include <map>
#include <string>
#include <fstream>

VSQ_BEGIN_NAMESPACE

/**
 * @brief Convert phrase into corresponding VOCALOID symbols.
 */
class PhoneticSymbolDictionary
{
public:
	/**
	 * @brief Dictionary item used in PhoneticSymbolConverter.
	 */
	class Element
	{
	private:
		/**
		 * @brief A word. In case English, phrases are separated by '-' letter.
		 */
		std::string _word;
		/**
		 * @brief A string of phonetic symbols, contains '\t'.
		 */
		std::string _rawSymbol;
		/**
		 * @brief A string of phonetic symbols, doesn't contain '\t'.
		 */
		std::string _symbol;

	public:
		/**
		 * @brief Initialize an element.
		 * @param word A word. In case English, phrases are separated by '-' letter.
		 * @param symbol A string of phonetic symbols, contains '\t'.
		 */
		Element(const std::string& word, const std::string& symbol)
		{
			_word = word;
			_rawSymbol = symbol;
			_symbol = StringUtil::replace(symbol, "\t", " ");
		}

		/**
		 * @brief Get a word. In case English, phrases are separated by '-' letter.
		 */
		std::string word()const
		{
			return _word;
		}

		/**
		 * @brief Get a string of phonetic symbols, contains '\t'.
		 */
		std::string rawSymbol()const
		{
			return _rawSymbol;
		}

		/**
		 * @brief Get a string of phonetic symbols, doesn't contain '\t'.
		 */
		std::string symbol()const
		{
			return _symbol;
		}
	};

private:
	std::map<std::string, Element> table;
	int _maxDivisions;

public:
	/**
	 * @brief Read dictionary from file.
	 * @param filePath File path of dictionary.
	 */
	explicit PhoneticSymbolDictionary(const std::string& filePath)
	{
		_maxDivisions = 1;
		read(filePath);
	}

	/**
	 * @brief Default constructor.
	 */
	explicit PhoneticSymbolDictionary()
	{
		_maxDivisions = 1;
	}

	/**
	 * @brief Get symbol information, corresponding a 'word'.
	 * @param word A word to get symbol information.
	 * @return Symbol information. Returns 0 if symbol information was not found.
	 */
	const Element* attach(const std::string& word)const
	{
		std::string key = StringUtil::toLower(word);
		std::map<std::string, Element>::const_iterator index = table.find(key);
		if (index == table.end()) {
			return 0;
		} else {
			return &index->second;
		}
	}

	/**
	 * @brief Get a maximum division number in this dictionary.
	 *     Usuary, a word is separated into several phrases. (ex. 'acorn' is separated into 'a-', 'corn'.)
	 *     This method returns the maximum number of phrase-division in all word in this dictionary.
	 */
	int maxDivisions()const
	{
		return _maxDivisions;
	}

	/**
	 * @brief Get an instance of dictionary used for VOCALOID Japanese voice bank.
	 * @return An instance of dictionary.
	 */
	static const PhoneticSymbolDictionary* vocaloidJpDictionary()
	{
		static PhoneticSymbolDictionary result;
		if (result.table.empty()) {
#include "PhoneticSymbolDictionaryVocaloidJp.include"
		}
		return &result;
	}

	/**
	 * @brief Get an instance of dictionary used for VOCALOID English voice bank.
	 * @return An instance of dictionary.
	 */
	static const PhoneticSymbolDictionary* vocaloidEnDictionary()
	{
		static PhoneticSymbolDictionary result;
		if (result.table.empty()) {
#include "PhoneticSymbolDictionaryVocaloidEn.include"
		}
		return &result;
	}

private:
	/**
	 * @brief Read dictionary from file.
	 * @param filePath File path of dictionary.
	 */
	void read(const std::string& filePath)
	{
		std::ifstream stream(filePath.c_str());
		std::string line;
		while (stream && std::getline(stream, line)) {
			line = StringUtil::trim(line);
			std::vector<std::string> parameters = StringUtil::explode("\t\t", line);
			if (parameters.size() == 2) {
				add(parameters[0], parameters[1]);
			}
		}
	}

	/**
	 * @brief Add a dictionary entry.
	 * @param word A word, phrase separated by TAB letter. (ex. 'a-\tcid' for 'acid'.)
	 * @param symbol A symbol. (ex. '{\ts I d' for 'acid')
	 */
	void add(const std::string& word, const std::string& symbol)
	{
		std::string key = StringUtil::toLower(StringUtil::replace(word, "-\t", ""));
		if (table.find(key) == table.end()) {
			table.insert(std::make_pair(key, Element(word, symbol)));

			std::vector<std::string> splittedWord = StringUtil::explode("\t", symbol);
			_maxDivisions = std::max(_maxDivisions, (int)splittedWord.size());
		}
	}
};

VSQ_END_NAMESPACE

#endif
