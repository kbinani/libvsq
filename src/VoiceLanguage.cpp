/**
 * @file VoiceLanguage.cpp
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
#include "../include/libvsq/VoiceLanguage.hpp"
#include "../include/libvsq/StringUtil.hpp"

LIBVSQ_BEGIN_NAMESPACE

VoiceLanguage
VoiceLanguageUtil::valueFromSingerName(std::string const& name)
{
	std::string search = StringUtil::toLower(name);
	if (search == "meiko" ||
		search == "kaito" ||
		search == "miku" ||
		search == "rin" ||
		search == "len" ||
		search == "rin_act2" ||
		search == "len_act2" ||
		search == "gackpoid" ||
		search == "luka_jpn" ||
		search == "megpoid" ||
		search == "sfa2_miki" ||
		search == "yuki" ||
		search == "kiyoteru" ||
		search == "miku_sweet" ||
		search == "miku_dark" ||
		search == "miku_soft" ||
		search == "miku_light" ||
		search == "miku_vivid" ||
		search == "miku_solid") {
		return VoiceLanguage::JAPANESE;
	} else if (search == "sweet_ann" ||
			   search == "prima" ||
			   search == "luka_eng" ||
			   search == "sonika" ||
			   search == "lola" ||
			   search == "leon" ||
			   search == "miriam" ||
			   search == "big_al") {
		return VoiceLanguage::ENGLISH;
	}
	return VoiceLanguage::JAPANESE;
}

LIBVSQ_END_NAMESPACE
