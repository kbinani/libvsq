/**
 * HandleType.hpp
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

namespace HandleType
{

/**
 * @brief {@link Handle} の種類を表現する列挙子.
 */
enum HandleTypeEnum {
	UNKNOWN = 0,

	/**
	 * @brief 歌詞ハンドル.
	 */
	LYRIC = 1,

	/**
	 * @brief ビブラートハンドル.
	 */
	VIBRATO = 2,

	/**
	 * @brief 歌手ハンドル.
	 */
	SINGER = 3,

	/**
	 * @brief アタックハンドル.
	 */
	NOTE_HEAD = 4,

	/**
	 * @brief Dynamics ハンドル.
	 */
	DYNAMICS = 5
};

}

VSQ_END_NAMESPACE
