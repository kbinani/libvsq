/**
 * VoiceLanguage.hpp
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
 * @brief 歌手の歌唱言語を表す列挙子.
 */
enum class VoiceLanguage {
	/**
	 * @brief 日本語.
	 */
	JAPANESE = 0,

	/**
	 * @brief 英語.
	 */
	ENGLISH = 1,
};

LIBVSQ_DECLARE_ENUM_TO_OSTREAM_OPERATOR(VoiceLanguage)

namespace VoiceLanguageUtil
{
/**
 * @brief 歌手の名前から, その歌手の歌唱言語を取得する.
 * @param name 歌手の名前.
 * @return 歌唱言語.
 */
extern VoiceLanguage valueFromSingerName(std::string const& name);
}

VSQ_END_NAMESPACE
