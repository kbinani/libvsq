/**
 * Common.hpp
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

VSQ_BEGIN_NAMESPACE

class TextStream;

/**
 * @brief VSQ ファイルのメタテキストの [Common] セクションに記録される内容を格納するクラス
 */
class Common
{
public:
	/**
	 * @brief トラックの歌声合成エンジンのバージョンを表す文字列
	 */
	std::string version;// = "DSB301";

	/**
	 * @brief トラックの名前
	 */
	std::string name;// = "Miku";

	/**
	 * @brief ダイナミクスモード
	 */
	DynamicsMode::DynamicsModeEnum dynamicsMode;// = DynamicsMode::EXPERT;

	/**
	 * @brief (Unknown)
	 */
	std::string color;

protected:
	/**
	 * @brief Play mode.
	 */
	PlayMode::PlayModeEnum _playMode;

private:
	/**
	 * @brief Previous play mode before play mode is set to OFF.
	 */
	PlayMode::PlayModeEnum _lastPlayMode;

public:
	/**
	 * @brief 初期化を行う
	 */
	Common();

	/**
	 * @brief 初期化を行う
	 * @param stream 読み込み元のテキストストリーム
	 * @param lastLine 読み込んだ最後の行。テーブルの ["value"] に文字列が格納される
	 * @todo VSQFileReaderに移動する
	 */
	Common(TextStream& stream, std::string& lastLine);

	/**
	 * @brief 初期化を行う
	 * @param name トラック名
	 * @param r 赤(意味は不明)
	 * @param g 緑(意味は不明)
	 * @param b 青(意味は不明)
	 * @param dynamicsMode シーケンスの Dynamics モード
	 * @param playMode シーケンスの Play モード
	 */
	Common(std::string name, int r, int g, int b, DynamicsMode::DynamicsModeEnum dynamicsMode, PlayMode::PlayModeEnum playMode);

	/**
	 * @brief コピーを作成する
	 * @return このインスタンスのコピー
	 */
	Common clone() const;

	/**
	 * @brief Get play mode.
	 */
	PlayMode::PlayModeEnum playMode() const;

	/**
	 * @brief Set play mode.
	 */
	void setPlayMode(PlayMode::PlayModeEnum mode);

	/**
	 * @brief Get previous play mode before play mode is set to OFF.
	 */
	PlayMode::PlayModeEnum lastPlayMode() const;

	/**
	 * @brief テキストストリームに出力する
	 * @param stream (TextStream) 出力先のストリーム
	 * @todo VSQFileWriterに移動する
	 */
	void write(TextStream& stream) const;

private:
	void init();
};

VSQ_END_NAMESPACE
