/**
 * @file ArticulationType.hpp
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

#include "./EnumUtil.hpp"

LIBVSQ_BEGIN_NAMESPACE

/**
 * @brief アイコン設定の種類を表す.
 */
enum class ArticulationType {
	/**
	 * @brief アイコン設定がない.
	 */
	NONE = 0,

	/**
	 * @brief ビブラート.
	 */
	VIBRATO = 1,

	/**
	 * @brief クレッシェンド, またはデクレッシェンド.
	 */
	CRESCENDO = 2,

	/**
	 * @brief ピアノ, フォルテ等の強弱記号.
	 */
	DYNAFF = 3,

	/**
	 * @brief アタック.
	 */
	NOTE_ATTACK = 4,

	/**
	 * @brief NoteTransition(詳細不明).
	 */
	NOTE_TRANSITION = 5,
};

LIBVSQ_DECLARE_ENUM_TO_OSTREAM_OPERATOR(ArticulationType)

LIBVSQ_END_NAMESPACE
