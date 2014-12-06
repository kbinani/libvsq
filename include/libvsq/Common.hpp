/**
 * @file Common.hpp
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

#include "./BasicTypes.hpp"
#include "./DynamicsMode.hpp"
#include "./PlayMode.hpp"
#include <string>

LIBVSQ_BEGIN_NAMESPACE

class TextStream;

/**
 * @brief VSQ ファイルのメタテキストの [Common] セクションに記録される内容を格納するクラス.
 */
class Common
{
public:
	/**
	 * @brief トラックの歌声合成エンジンのバージョンを表す文字列.
	 */
	std::string version;

	/**
	 * @brief トラックの名前.
	 */
	std::string name;

	/**
	 * @brief ダイナミクスモード.
	 */
	DynamicsMode dynamicsMode;

	/**
	 * @brief (Unknown)
	 */
	std::string color;

private:
	/**
	 * @brief Play mode.
	 */
	PlayMode _playMode;

	/**
	 * @brief Previous play mode before play mode is set to OFF.
	 */
	PlayMode _lastPlayMode;

public:
	/**
	 * @brief 初期化を行う.
	 */
	Common();

	/**
	 * @brief 初期化を行う.
	 * @param name トラック名.
	 * @param r 赤(意味は不明).
	 * @param g 緑(意味は不明).
	 * @param b 青(意味は不明).
	 * @param dynamicsMode シーケンスの Dynamics モード.
	 * @param playMode シーケンスの Play モード.
	 */
	Common(std::string const& name, int r, int g, int b, DynamicsMode dynamicsMode, PlayMode playMode);

	/**
	 * @brief コピーを作成する.
	 * @return このインスタンスのコピー.
	 */
	Common clone() const;

	/**
	 * @brief Get play mode.
	 */
	PlayMode playMode() const;

	/**
	 * @brief Set play mode.
	 */
	void playMode(PlayMode mode);

	/**
	 * @brief Get previous play mode before play mode is set to OFF.
	 */
	PlayMode lastPlayMode() const;

private:
	void init();
};

LIBVSQ_END_NAMESPACE
