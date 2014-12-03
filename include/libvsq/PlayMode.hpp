/**
 * PlayMode.hpp
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

VSQ_BEGIN_NAMESPACE

/**
 * @brief 歌声合成の再生モードを表す列挙子.
 */
enum class PlayMode {
	/**
	 * @brief トラックはミュートされる(-1).
	 */
	OFF = -1,

	/**
	 * @brief トラックは合成された後再生される(0).
	 */
	PLAY_AFTER_SYNTH = 0,

	/**
	 * @brief トラックは合成しながら再生される(1).
	 */
	PLAY_WITH_SYNTH = 1,
};

LIBVSQ_DECLARE_ENUM_TO_OSTREAM_OPERATOR(PlayMode)

VSQ_END_NAMESPACE
