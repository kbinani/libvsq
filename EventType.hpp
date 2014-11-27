/**
 * EventType.hpp
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

VSQ_BEGIN_NAMESPACE

class EventType
{
public:
	/**
	 * {@link Event} の種類を表現する列挙子
	 */
	enum EventTypeEnum {
		/**
		 * @brief 不明
		 */
		UNKNOWN = 0,

		/**
		 * @brief 歌手
		 */
		SINGER = 1,

		/**
		 * @brief 歌詞
		 */
		NOTE = 2,

		/**
		 * @brief アイコン
		 */
		ICON = 3
	};

	/**
	 * @brief 文字列に変換する
	 * @param value 指定された列挙子の文字列表現
	 * @return 変換後の文字列
	 */
	static const std::string toString(EventTypeEnum value);

private:
	EventType();
};

VSQ_END_NAMESPACE
