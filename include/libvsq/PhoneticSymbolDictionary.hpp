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
 * @brief \~japanese-en 歌詞を VOCALOID の発音記号列に変換する処理を行うクラス. \~english Convert phrase into corresponding VOCALOID symbols.
 */
class PhoneticSymbolDictionary
{
public:
	/**
	 * @brief \~japanese-en 歌詞と発音記号とのマッピング 1 つ分を表すクラス.
	 *        \~english Dictionary item used in PhoneticSymbolConverter.
	 */
	class Element
	{
	private:
		/**
		 * @brief \~japanese-en 歌詞.
		 *        \~english A word. In case English, phrases are separated by '-' letter.
		 */
		std::string _word;
		/**
		 * @brief \~japanese-en 歌詞 @a _word に対応する発音記号列. タブ文字で区切られる.
		 *        \~english A string of phonetic symbols, contains TAB character.
		 */
		std::string _rawSymbol;
		/**
		 * @brief \~japanese-en 歌詞 @a _word に対応する発音記号列. 空白文字 ' ' で区切られる.
		 *        \~english A string of phonetic symbols, separated by space ' ' character.
		 */
		std::string _symbol;

	public:
		/**
		 * @brief \~japanese-en 初期化する.
		 *        \~english Initialize an element.
		 * @param word \~japanese-en 歌詞.
		 *             \~english A word. In case English, phrases are separated by '-' letter.
		 * @param symbol \~japanese-en @a word に対応する発音記号列. タブ文字で区切られる.
		 *               \~english A string of phonetic symbols, contains TAB character.
		 */
		Element(std::string const& word, std::string const& symbol);

		/**
		 * @brief \~japanese-en 歌詞を取得する. 英語の場合, 音節は '-' で区切られる.
		 *        \~english Get a word. In case English, phrases are separated by '-' letter.
		 */
		std::string word() const;

		/**
		 * @brief \~japanese-en タブ文字で区切られた発音記号列を取得する.
		 *        \~english Get a string of phonetic symbols, contains TAB character.
		 */
		std::string rawSymbol() const;

		/**
		 * @brief \~japanese-en 空白文字 ' ' で区切られた発音記号列を取得する.
		 *        \~english Get a string of phonetic symbols, separated by space ' ' character.
		 */
		std::string symbol() const;
	};

private:
	std::map<std::string, Element> table;
	int _maxDivisions;

public:
	/**
	 * @brief \~japanese-en ファイルから辞書を読み込む.
	 *        \~english Read dictionary from file.
	 * @param filePath \~japanese-en 読み込むファイルのファイルパス.
	 *                 \~english File path of dictionary.
	 */
	explicit PhoneticSymbolDictionary(std::string const& filePath);

	/**
	 * @brief \~japanese-en 辞書を初期化する.
	 *        \~english Initialize dictionary.
	 */
	PhoneticSymbolDictionary();

	/**
	 * @brief \~japanese-en 歌詞 @a word に対応する発音記号情報を検索する.
	 *        \~english Get symbol information, corresponding a @a word.
	 * @param word \~japanese-en 歌詞.
	 *             \~english A word to get symbol information.
	 * @return \~japanese-en 発音記号情報. 対応する発音記号が見つからなければ null を返す.
	 *         \~english Symbol information. Returns 0 if symbol information was not found.
	 */
	Element const* attach(std::string const& word) const;

	/**
	 * @brief \~japanese-en ひとつの単語が, 最大何個の音節に分割されるかを取得する.
	 *        \~english Get a maximum division number in this dictionary.
	 * @details \~japanese-en 通常, 単語は音節に分割される. (例えば 'acorn' は 'a-', 'corn' というように.)
	 *                        この関数は, このオブジェクトが持っている単語のうち音節分割数が最大のものを返す.
	 *          \~english Usuary, a word is separated into several phrases. (ex. 'acorn' is separated into 'a-', 'corn'.)
	 *                    This method returns the maximum number of phrase-division in all word in this dictionary.
	 */
	int maxDivisions() const;

	/**
	 * @brief \~japanese-en VOCALOID 日本語ライブラリに使用される発音記号辞書を返す.
	 *        \~english Get an instance of dictionary used for VOCALOID Japanese voice bank.
	 * @return \~japanese-en 辞書オブジェクト.
	 *         \~english An instance of dictionary.
	 */
	static PhoneticSymbolDictionary const* vocaloidJpDictionary();

	/**
	 * @brief \~japanese-en VOCALOID 英語ライブラリに使用される発音記号辞書を返す.
	 *        \~english Get an instance of dictionary used for VOCALOID English voice bank.
	 * @return \~japanese-en 辞書オブジェクト.
	 *         \~english An instance of dictionary.
	 */
	static PhoneticSymbolDictionary const* vocaloidEnDictionary();

private:
	/**
	 * @brief \~japanese-en ファイルから辞書を読み込む.
	 *        \~english Read dictionary from file.
	 * @param filePath \~japanese-en 辞書ファイルのファイルパス.
	 *                 \~english File path of dictionary.
	 */
	void read(std::string const& filePath);

	/**
	 * @brief \~japanese-en 辞書の要素を追加する.
	 *        \~english Add a dictionary entry.
	 * @param word \~japanese-en 音節を TAB 文字で区切った単語. (例えば 'acid' なら 'a-\tcid'.)
	 *             \~english A word, phrase separated by TAB letter. (ex. 'a-\tcid' for 'acid'.)
	 * @param symbol \~japanese-en 発音記号列. (例えば 'acid' なら '{\ts I d'.)
	 *               \~english A symbol. (ex. '{\ts I d' for 'acid'.)
	 */
	void add(std::string const& word, std::string const& symbol);

	void init(std::string const& source);
};

LIBVSQ_END_NAMESPACE
