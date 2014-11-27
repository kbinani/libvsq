/**
 * PhoneticSymbol.hpp
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

VSQ_BEGIN_NAMESPACE

/**
 * VSQ で使用される発音記号のためのユーティリティ
 * @class table
 * @name PhoneticSymbol
 */
class PhoneticSymbol
{
public:
	/**
	 * @brief 指定した文字列が子音を表す発音記号かどうかを判定する
	 * @param symbol (string) 判定対象の発音記号
	 * @return (boolean) 子音であれば <code>true</code> を、そうでなければ <code>false</code> を返す
	 */
	static bool isConsonant(std::string const& symbol);

	/**
	 * @brief 指定した文字列が母音を表す発音記号かどうかを判定する
	 * @param symbol (string) 判定対象の発音記号
	 * @return (boolean) 母音であれば <code>true</code> を、そうでなければ <code>false</code> を返す
	 */
	static bool isVowel(std::string const& symbol);

	/**
	 * @brief 指定した文字列が発音記号として有効かどうかを判定する
	 * @param symbol (string) 判定対象の発音記号
	 * @return (boolean) 有効であれば <code>true</code> を、そうでなければ <code>false</code> を返す
	 */
	static bool isValidSymbol(std::string const& symbol);

private:
	PhoneticSymbol()
	{}

	static bool isVowelJP(std::string const& search);

	static bool isConsonantJP(std::string const& search);

	static bool isVowelEN(std::string const& search);

	static bool isConsonantEN(std::string const& search);
};

VSQ_END_NAMESPACE
