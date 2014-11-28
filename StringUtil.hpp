/**
 * StringUtil.hpp
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
#include <exception>
#include <vector>
#include <string>

VSQ_BEGIN_NAMESPACE

/**
 * @brief 文字列関連のユーティリティ.
 */
class StringUtil
{
public:
	/**
	 * @brief parseInt メソッドが投げる例外.
	 */
	class IntegerParseException : public std::exception
	{};

	/**
	 * @brief parseFloat メソッドが投げる例外.
	 */
	class FloatParseException : public std::exception
	{};

public:
	/**
	 * @brief 文字列を区切り文字で分割する.
	 * @param delimiter 区切り文字.
	 * @param text 文字列.
	 * @param limit 区切る回数の最大値.
	 * @return 区切られた文字列のリスト.
	 */
	static std::vector<std::string> explode(std::string const& delimiter, std::string const& text, std::string::size_type limit = std::string::npos, std::string const& escape = "");

	/**
	 * @brief 含まれる文字列を全て置換する.
	 * @param text 処理対象の文字列.
	 * @param search 検索する文字列.
	 * @param replace 置換する文字列.
	 * @return 置換後の文字列.
	 */
	static std::string replace(std::string const& text, std::string const& search, std::string const& replace);

	/**
	 * @brief 文字列を整数に変換する.
	 * @param text 変換する文字列.
	 * @return 変換後の数値.
	 */
	template<typename T>
	static T parseInt(std::string const& text, int baseNumber = 10)
	{
		try {
			return static_cast<T>(std::stoll(text, 0, baseNumber));
		} catch (...) {
			throw IntegerParseException();
		}
	}

	/**
	 * @brief 文字列を浮動小数点数に変換する.
	 * @param text 変換する文字列.
	 * @return 変換後の数値.
	 */
	template<typename T>
	static T parseFloat(std::string const& text)
	{
		try {
			return std::stold(text);
		} catch (...) {
			throw FloatParseException();
		}
	}

	/**
	 * @brief 整数を文字列に変換する.
	 * @param value 変換する数値.
	 * @return 変換後の文字列.
	 */
	static std::string toString(int value, int baseNumber = 10);

	/**
	 * @brief 数値を文字列に変換する.
	 * @param value 変換する数値.
	 * @param format 変換時のフォーマット.
	 * @return 変換後の文字列.
	 */
	template<typename T>
	static std::string toString(T value, std::string const& format)
	{
		std::vector<char> buffer(32);
		int amount = snprintf(buffer.data(), buffer.size(), format.c_str(), value);
		if (amount > buffer.size()) {
			buffer.resize(amount);
			amount = snprintf(buffer.data(), buffer.size(), format.c_str(), value);
		}
		return std::string(buffer.data(), amount);
	}

	/**
	 * @brief アルファベットの大文字を小文字に変換する.
	 * @param value 変換する文字列.
	 * @return 結果文字列.
	 */
	static std::string toLower(std::string const& value);

	/**
	 * @brief 文字列を指定回数繰り返した文字列を取得する.
	 * @param value 繰り返す文字列.
	 * @param count 繰り返す回数.
	 * @return 結果文字列.
	 */
	static std::string repeat(std::string const& value, int count);

	/**
	 * @brief Trim leading and trailing blank characters.
	 * @param value Source string.
	 */
	static std::string trim(std::string const& s);

private:
	StringUtil();

	/**
	 * @brief 文字列から, エスケープ文字を考慮してデリミタ文字列の位置を探す.
	 * @param text 文字列.
	 * @param delimiter デリミタ(区切り)文字.
	 * @param escape エスケープ文字.
	 * @param searchFrom 検索開始インデックス.
	 * @return デリミタ文字列の位置. 見つからなければ string::npos を返す.
	 */
	static std::string::size_type getDelimiterIndex(std::string const& text, std::string const& delimiter, std::string const& escape, std::string::size_type searchFrom);

	static char _toLower(char c);
};

VSQ_END_NAMESPACE
