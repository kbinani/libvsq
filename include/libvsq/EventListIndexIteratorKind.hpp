/**
 * @file EventListIndexIteratorKind.hpp
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

LIBVSQ_BEGIN_NAMESPACE

/**
 * @brief IndexIterator の反復子の種類を表す列挙子.
 */
enum class EventListIndexIteratorKind {
	/**
	 * @brief 全てのイベント.
	 */
	ALL = 0xFFFF,

	/**
	 * @brief 歌手イベント.
	 */
	SINGER = 1,

	/**
	 * @brief 音符イベント.
	 */
	NOTE = 1 << 1,

	/**
	 * @brief クレッシェンドイベント.
	 */
	CRESCENDO = 1 << 2,

	/**
	 * @brief デクレッシェンドイベント.
	 */
	DECRESCENDO = 1 << 3,

	/**
	 * @brief 強弱記号イベント.
	 */
	DYNAFF = 1 << 4,
};

LIBVSQ_END_NAMESPACE
