/**
 * Mixer.hpp
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
#include "./MixerItem.hpp"
#include <vector>

VSQ_BEGIN_NAMESPACE

class TextStream;

/**
 * @brief VSQ ファイルのメタテキストの [Mixer] セクションに記録される内容を取り扱うクラス
 */
class Mixer
{
public:
	/**
	 * @brief MasterFader 値
	 */
	int masterFeder;// = 0;

	/**
	 * @brief MasterMute 値
	 */
	int masterMute;// = 0;

	/**
	 * @brief MasterPanpot 値
	 */
	int masterPanpot;// = 0;

	/**
	 * @brief OutputMode 値
	 */
	int outputMode;// = 0;

	/**
	 * @brief 各トラックのミキサー情報
	 */
	std::vector<MixerItem> slave;

	/**
	 * @brief 初期化を行う
	 */
	Mixer();

	/**
	 * @brief 各パラメータを指定し、初期化を行う
	 * @param masterFeder MasterFader 値
	 * @param masterPanpot MasterPanpot 値
	 * @param masterMute MasterMute 値
	 * @param outputMode OutputMode 値
	 */
	Mixer(int masterFeder, int masterPanpot, int masterMute, int outputMode);

	/**
	 * @brief テキストストリームから読み込みを行い、初期化を行う
	 * @param stream (TextStream) 読み込むテキストストリーム
	 * @param lastLine (table) 読み込んだ最後の行。テーブルの ["value"] に文字列が格納される
	 */
	Mixer(TextStream& stream, std::string& lastLine);

	/**
	 * @brief コピーを作成する
	 * @return このオブジェクトのコピー
	 */
	Mixer clone() const;

	/**
	 * @brief テキストストリームに出力する
	 * @param stream (TextStream) 出力先のストリーム
	 */
	void write(TextStream& stream) const;
};

VSQ_END_NAMESPACE
