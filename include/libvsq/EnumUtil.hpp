/**
 * @file EnumUtil.hpp
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
#pragma once

#include "./Namespace.hpp"
#include <string>
#include <ostream>
#include <type_traits>

/**
 * @def LIBVSQ_DECLARE_ENUM_TO_STRING_FREE_FUNCTION
 * @brief enum class を文字列に変換する関数を宣言し, std::basic_ostream へ書き込む << 演算子を定義する.
 * @param ENUM_NAME 対象の enum class の名前.
 */
#define LIBVSQ_DECLARE_ENUM_TO_STRING_FREE_FUNCTION(ENUM_NAME) \
	namespace ENUM_NAME ## Util																					\
	{																											\
	/**																											\
	 * @brief ENUM_NAME を文字列に変換する.																		\
	 * @param v 書き込む ENUM_NAME の値.																			\
	 * @return ENUM_NAME の文字列表現.																			\
	 */																											\
	extern std::string toString(ENUM_NAME v);																	\
	}																											\
	/**																											\
	 * @fn std::basic_ostream<char, Traits>& operator << (std::basic_ostream<char, Traits>& os, ENUM_NAME v)	\
	 * @brief ENUM_NAME をストリームに書き込む.																	\
	 * @param os 書き込み先のストリーム.																			\
	 * @param v 書き込む ENUM_NAME の値.																			\
	 * @return 書き込んだ後のストリーム @a os.																		\
	 */																											\
	template<class Traits>																						\
	inline std::basic_ostream<char, Traits>& operator << (std::basic_ostream<char, Traits>& os, ENUM_NAME v)	\
	{																											\
		os << ENUM_NAME ## Util::toString(v);																	\
		return os;																								\
	}

/**
 * @def LIBVSQ_DECLARE_ENUM_TO_OSTREAM_OPERATOR
 * @brief enum class の値を std::basic_ostream へ書き込む << 演算子を定義する.
 * @param ENUM_NAME 対象の enum class の名前.
 */
#define LIBVSQ_DECLARE_ENUM_TO_OSTREAM_OPERATOR(ENUM_NAME) \
	/**																											\
	 * @fn std::basic_ostream<char, Traits>& operator << (std::basic_ostream<char, Traits>& os, ENUM_NAME v)	\
	 * @brief ENUM_NAME をストリームに書き込む.																	\
	 * @param os 書き込み先のストリーム.																			\
	 * @param v 書き込む ENUM_NAME の値.																			\
	 * @return 書き込んだ後のストリーム @a os.																		\
	 */																											\
	template<class Traits>																						\
	inline std::basic_ostream<char, Traits>& operator << (std::basic_ostream<char, Traits>& os, ENUM_NAME v)	\
	{																											\
		os << static_cast<std::underlying_type<ENUM_NAME>::type>(v);											\
		return os;																								\
	}
