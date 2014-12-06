/**
 * @file DynamicsMode.hpp
 * Copyright © 2012 kbinani
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
 * @brief VOCALOID1における, ダイナミクスモードを表す定数を格納するための列挙子.
 */
enum class DynamicsMode {
	/**
	 * @brief DYNカーブが非表示になるモード(デフォルト).
	 */
	STANDARD = 0,

	/**
	 * @brief DYNカーブが表示されるモード(エキスパートモード).
	 */
	EXPERT = 1,
};

LIBVSQ_END_NAMESPACE
