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

#define LIBVSQ_DECLARE_ENUM_TO_STRING_FREE_FUNCTION(__enum__) \
	namespace __enum__ ## Util																				\
	{																										\
	extern std::string toString(__enum__ v);																\
	}																										\
	template<class Traits>																					\
	inline std::basic_ostream<char, Traits>& operator << (std::basic_ostream<char, Traits>& os, __enum__ v) \
	{																										\
		os << __enum__ ## Util::toString(v);																\
		return os;																							\
	}

#define LIBVSQ_DECLARE_ENUM_TO_OSTREAM_OPERATOR(__enum__) \
	template<class Traits>																					\
	inline std::basic_ostream<char, Traits>& operator << (std::basic_ostream<char, Traits>& os, __enum__ v)	\
	{																										\
		os << static_cast<std::underlying_type<__enum__>::type>(v);											\
		return os;																							\
	}
