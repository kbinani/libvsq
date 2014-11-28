/**
 * NrpnEvent.hpp
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
#include "./MidiParameterType.hpp"
#include "./MidiEvent.hpp"
#include <vector>

VSQ_BEGIN_NAMESPACE

/**
 * @brief NRPN イベントを表すクラス.
 */
class NrpnEvent
{
public:
	/**
	 * @brief Tick 単位の時刻.
	 */
	tick_t clock;

	/**
	 * @brief NRPN の値.
	 */
	MidiParameterType nrpn;

	/**
	 * @brief DATA MSB.
	 */
	int dataMSB;

	/**
	 * @brief DATA LSB.
	 */
	int dataLSB;

	/**
	 * @brief DATA LSB 値を持っているかどうか.
	 */
	bool hasLSB;

	/**
	 * @brief MSB の出力を省略するかどうか.
	 */
	bool isMSBOmittingRequired;

protected:
	/**
	 * @brief 子イベントのリスト.
	 */
	std::vector<NrpnEvent> _list;

public:
	/**
	 * @brief 時刻, NRPN, DATA MSB を指定し, 初期化を行う.
	 * @param clock Tick 単位の時刻.
	 * @param nrpn NRPN.
	 * @param dataMsb DATA MSB.
	 */
	NrpnEvent(tick_t clock, MidiParameterType nrpn, int dataMsb);

	/**
	 * @brief 時刻, NRPN, DATA MSB, DATA LSB を指定し, 初期化を行う.
	 * @param clock Tick 単位の時刻.
	 * @param nrpn NRPN.
	 * @param dataMsb DATA MSB.
	 * @param dataLsb DATA LSB.
	 */
	NrpnEvent(tick_t clock, MidiParameterType nrpn, int dataMsb, int dataLsb);

	/**
	 * @brief 親子関係によって入れ子になっている NRPN イベントを展開し, 配列に変換する.
	 * @return 展開後の NRPN イベントの配列.
	 */
	std::vector<NrpnEvent> expand();

	/**
	 * @brief 順序を比較する.
	 * @param item 比較対象のアイテム.
	 * @return このインスタンスが比較対象よりも小さい場合は負の整数, 等しい場合は 0, 大きい場合は正の整数を返す.
	 */
	int compareTo(NrpnEvent const& item) const;

	/**
	 * @brief NRPN, DATA MSB を指定し, イベントを追加する.
	 * @param nrpn NRPN.
	 * @param dataMsb DATA MSB.
	 */
	void append(MidiParameterType nrpn, int dataMsb);

	/**
	 * @brief NRPN, DATA MSB, DATA LSB を指定し, イベントを追加する.
	 * @param nrpn NRPN.
	 * @param dataMsb DATA MSB.
	 * @param dataLsb DATA LSB.
	 */
	void append(MidiParameterType nrpn, int dataMsb, int dataLsb);

	/**
	 * @brief NRPN, DATA MSB, MSB 省略フラグを指定し, イベントを追加する.
	 * @param nrpn NRPN.
	 * @param dataMsb DATA MSB.
	 * @param isMsbOmittingRequired NRPN MSB を省略する場合は <code>true</code> を, そうでない場合は <code>false</code> を指定する.
	 */
	void append(MidiParameterType nrpn, int dataMsb, bool isMsbOmittingRequired);

	/**
	 * @brief NRPN, DATA MSB, DATA LSB, MSB 省略フラグを指定し, イベントを追加する.
	 * @param nrpn NRPN.
	 * @param dataMsb DATA MSB.
	 * @param dataLsb DATA LSB.
	 * @param isMsbOmittingRequired NRPN MSB を省略する場合は <code>true</code> を, そうでない場合は <code>false</code> を指定する.
	 */
	void append(MidiParameterType nrpn, int dataMsb, int dataLsb, bool isMsbOmittingRequired);

	/**
	 * @brief 2 つの {@link NrpnEvent} を比較する.
	 * @param a 比較対象のオブジェクト.
	 * @param b 比較対象のオブジェクト.
	 * @return <code>a</code> が <code>b</code> よりも小さい場合は <code>true</code>, そうでない場合は <code>false</code> を返す.
	 */
	static bool compare(NrpnEvent const& a, NrpnEvent const& b);

	/**
	 * @brief NRPN イベントの配列を, {@link MidiEvent} の配列に変換する.
	 * @param source NRPN イベントの配列.
	 * @return {@link MidiEvent} の配列.
	 */
	static std::vector<MidiEvent> convert(std::vector<NrpnEvent> const& source);

protected:
	NrpnEvent();
};

VSQ_END_NAMESPACE
