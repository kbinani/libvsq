/**
 * @file PhoneticSymbolDictionary.hpp
 * Copyright © 2012,2014 kbinani
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
#pragma once

#include "./Namespace.hpp"
#include <string>
#include <map>

LIBVSQ_BEGIN_NAMESPACE

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
		Element(std::string const& word, std::string const& symbol);

		/**
		 * @brief Get a word. In case English, phrases are separated by '-' letter.
		 */
		std::string word() const;

		/**
		 * @brief Get a string of phonetic symbols, contains '\t'.
		 */
		std::string rawSymbol() const;

		/**
		 * @brief Get a string of phonetic symbols, doesn't contain '\t'.
		 */
		std::string symbol() const;
	};

private:
	std::map<std::string, Element> table;
	int _maxDivisions;

public:
	/**
	 * @brief Read dictionary from file.
	 * @param filePath File path of dictionary.
	 */
	explicit PhoneticSymbolDictionary(std::string const& filePath);

	/**
	 * @brief Default constructor.
	 */
	PhoneticSymbolDictionary();

	/**
	 * @brief Get symbol information, corresponding a 'word'.
	 * @param word A word to get symbol information.
	 * @return Symbol information. Returns 0 if symbol information was not found.
	 */
	Element const* attach(std::string const& word) const;

	/**
	 * @brief Get a maximum division number in this dictionary.
	 *     Usuary, a word is separated into several phrases. (ex. 'acorn' is separated into 'a-', 'corn'.)
	 *     This method returns the maximum number of phrase-division in all word in this dictionary.
	 */
	int maxDivisions() const;

	/**
	 * @brief Get an instance of dictionary used for VOCALOID Japanese voice bank.
	 * @return An instance of dictionary.
	 */
	static PhoneticSymbolDictionary const* vocaloidJpDictionary();

	/**
	 * @brief Get an instance of dictionary used for VOCALOID English voice bank.
	 * @return An instance of dictionary.
	 */
	static PhoneticSymbolDictionary const* vocaloidEnDictionary();

private:
	/**
	 * @brief Read dictionary from file.
	 * @param filePath File path of dictionary.
	 */
	void read(std::string const& filePath);

	/**
	 * @brief Add a dictionary entry.
	 * @param word A word, phrase separated by TAB letter. (ex. 'a-\tcid' for 'acid'.)
	 * @param symbol A symbol. (ex. '{\ts I d' for 'acid')
	 */
	void add(std::string const& word, std::string const& symbol);

	void init(std::string const& source);
};

LIBVSQ_END_NAMESPACE
