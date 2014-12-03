/**
 * MixerItem.hpp
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
 * @brief @link Mixer の <code>slave</code> 要素に格納されるアイテムを表すクラス.
 */
class MixerItem
{
public:
	/**
	 * @brief Feder値.
	 */
	int feder;

	/**
	 * @brief Panpot値.
	 */
	int panpot;

	/**
	 * @brief Mute値.
	 */
	int mute;

	/**
	 * @brief Solo値.
	 */
	int solo;

	/**
	 * @brief 各パラメータを指定し, 初期化を行う.
	 * @param feder Feder値.
	 * @param panpot Panpot値.
	 * @param mute Mute値.
	 * @param solo Solo値.
	 */
	MixerItem(int feder, int panpot, int mute, int solo);

	/**
	 * @brief コピーを作成する.
	 * @return このオブジェクトのコピー.
	 */
	MixerItem clone() const;
};

LIBVSQ_END_NAMESPACE
