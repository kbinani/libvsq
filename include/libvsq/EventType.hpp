/**
 * @file EventType.hpp
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

LIBVSQ_BEGIN_NAMESPACE

/**
 * {@link Event} の種類を表現する列挙子.
 */
enum class EventType {
	/**
	 * @brief 不明.
	 */
	UNKNOWN = 0,

	/**
	 * @brief 歌手.
	 */
	SINGER = 1,

	/**
	 * @brief 歌詞.
	 */
	NOTE = 2,

	/**
	 * @brief アイコン.
	 */
	ICON = 3,
};

namespace EventTypeUtil
{
/**
 * @brief EventType を文字列に変換する.
 * @param v 変換する EventType の値.
 * @return EventType の文字列表現.
 */
extern std::string toString(EventType v);
}

LIBVSQ_END_NAMESPACE
