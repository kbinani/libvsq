/**
 * EventListIndexItertorKind.hpp
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

VSQ_BEGIN_NAMESPACE

/**
 * @brief IndexIterator の反復子の種類を表す列挙子.
 */
class EventListIndexIteratorKind
{
public:
	enum EventListIndexIteratorKindEnum {
		/**
		 * @brief 全てのイベント.
		 */
		ALL = 0xffff,

		/**
		 * @brief 歌手イベント.
		 */
		SINGER = 1,

		/**
		 * @brief 音符イベント.
		 */
		NOTE = 2,

		/**
		 * @brief クレッシェンドイベント.
		 */
		CRESCENDO = 4,

		/**
		 * @brief デクレッシェンドイベント.
		 */
		DECRESCENDO = 8,

		/**
		 * @brief 強弱記号イベント.
		 */
		DYNAFF = 16
	};
};

VSQ_END_NAMESPACE
