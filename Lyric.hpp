/**
 * Lyric.hpp
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
#include <vector>

VSQ_BEGIN_NAMESPACE

/**
 * 歌詞ハンドルに格納する歌詞情報を保持するクラス
 * @class table
 * @name Lyric
 */
class Lyric
{
public:
	/**
	 * 歌詞
	 * @var string
	 */
	std::string phrase;// = "a";

	/**
	 * 歌詞ハンドル内に複数の歌詞が入る場合の、この歌詞の長さ分率。デフォルトは 1.0
	 * @var double
	 */
	double lengthRatio;// = 1.0;

	/**
	 * 発音記号がプロテクトされた状態かどうか
	 * @var boolean
	 */
	bool isProtected;// = false;

	/**
	 * 発音記号のリスト
	 * @var table
	 * @access private
	 * @todo privateにする
	 */
	std::vector<std::string> _phoneticSymbol;// = { "a" };

	/**
	 * Consonant Adjustment のリスト
	 * @var table
	 * @access private
	 * @todo privateにする
	 */
	std::vector<int> _consonantAdjustment;// = { 0 };

	/**
	 * @brief 文字列を元に初期化を行う
	 * @param line 「"あ","a",0.0000,0.0」などのような文字列
	 */
	explicit Lyric(std::string const& line);

	/**
	 * @brief 歌詞、発音記号を指定して初期化を行う
	 * @param phrase 歌詞
	 * @param phoneticSymbol 発音記号
	 */
	Lyric(std::string const& phrase, std::string const& phoneticSymbol);

	/**
	 * @brief このオブジェクトと、指定されたオブジェクトが同じかどうかを調べる。
	 * 音声合成したときに影響のある範囲のフィールドしか比較されない。
	 * たとえば、<code>isProtected</code> が <code>this</code> と <code>item</code> で違っていても、他が同一であれば <code>true</code> が返る。
	 * @param item 比較対象のオブジェクト
	 * @return 比較対象と同じであれば <code>true</code> を、そうでなければ <code>false</code> を返す
	 */
	bool equalsForSynth(Lyric& item) const;

	/**
	 * @brief このオブジェクトのインスタンスと、指定されたオブジェクトが同じかどうかを調べる
	 * @param item (Lyric) 比較対象のオブジェクト
	 * @return (boolean) 比較対象と同じであれば <code>true</code> を、そうでなければ <code>false</code> を返す
	 */
	bool equals(Lyric& item) const;

	/**
	 * @brief Get a string value of consonant adjustment, separated by ",".
	 * @return A string value of consonant adjustment.
	 */
	const std::string getConsonantAdjustment() const;

	/**
	 * @brief Set consonant adjustment with a string, adjustment value concatenated with ",".
	 * @param value [in] A string of consonant adjustment. (ex. "64,0")
	 */
	void setConsonantAdjustment(std::string const& value);

	/**
	 * @brief Consonant Adjustment を、整数配列で取得する
	 * @return Consonant Adjustment を格納した整数の配列
	 */
	const std::vector<int> getConsonantAdjustmentList() const;

	/**
	 * @brief Consonant Adjustment を、整数配列形式で設定する
	 * @param value Consonant Adjustment を格納した整数の配列
	 */
	void setConsonantAdjustmentList(const std::vector<int>& value);

	/**
	 * @brief コピーを作成する
	 * @return (Lyric) このインスタンスのコピー
	 */
	Lyric clone() const;

	/**
	 * @brief この歌詞の発音記号を取得する
	 * @return 発音記号
	 */
	const std::string getPhoneticSymbol() const;

	/**
	 * @brief この歌詞の発音記号を設定する
	 * @param value 発音記号
	 */
	void setPhoneticSymbol(std::string const& value);

	/**
	 * @brief この歌詞の発音記号の配列を取得する
	 * @return 発音記号の配列
	 */
	const std::vector<std::string> getPhoneticSymbolList() const;

	/**
	 * @brief このインスタンスを文字列に変換する
	 * @param addQuateMark (boolean) 歌詞、発音記号の前後に引用符(")を追加するかどうか
	 * @return (string) 変換後の文字列
	 */
	const std::string toString(bool addQuateMark = false) const;
};

VSQ_END_NAMESPACE
