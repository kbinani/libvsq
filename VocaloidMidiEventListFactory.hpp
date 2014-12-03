/**
 * VocaloidMidiEventListFactory.hpp
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

#include "./MidiParameterType.hpp"
#include "./NrpnEvent.hpp"

VSQ_BEGIN_NAMESPACE

class Track;
class TempoList;
class Event;
class BPList;

/**
 * @brief A class to generate NRPN list from an instance of Track. The NRPN list is used for VOCALOID.
 */
class VocaloidMidiEventListFactory
{
private:
	/**
	 * @brief A base class to provide Delay NrpnEvent and Value NrpnEvent from an instance of BPList.
	 */
	class NrpnEventProvider
	{
	protected:
		MidiParameterType delayNrpn;
		MidiParameterType nrpn;

	public:
		NrpnEventProvider(
			MidiParameterType delayNrpnType,
			MidiParameterType nrpnType)
		{
			this->delayNrpn = delayNrpnType;
			this->nrpn = nrpnType;
		}

		virtual ~NrpnEventProvider()
		{}

		virtual NrpnEvent getDelayNrpnEvent(tick_t actualTick, int delay) const
		{
			int delayMsb, delayLsb;
			VocaloidMidiEventListFactory::_getMsbAndLsb(delay, &delayMsb, &delayLsb);
			return NrpnEvent(actualTick, delayNrpn, delayMsb, delayLsb);
		}

		virtual NrpnEvent getNrpnEvent(tick_t actualTick, int value) const
		{
			return NrpnEvent(actualTick, nrpn, value);
		}
	};

	/**
	 * @brief An implementation of NrpnEventProvider for "Pitch Bend" BPList.
	 */
	class PitchBendNrpnEventProvider : public NrpnEventProvider
	{
	public:
		explicit PitchBendNrpnEventProvider()
			: NrpnEventProvider(MidiParameterType::PB_DELAY, MidiParameterType::PB_PITCH_BEND)
		{}

		NrpnEvent getNrpnEvent(tick_t actualTick, int value) const override
		{
			int actualValue = value + 0x2000;
			int msb, lsb;
			VocaloidMidiEventListFactory::_getMsbAndLsb(actualValue, &msb, &lsb);
			return NrpnEvent(actualTick, nrpn, msb, lsb);
		}
	};

	/**
	 * @brief An implementation of NrpnEventProvider for "Pitch Bend Sensitivity" BPList.
	 */
	class PitchBendSensitivityNrpnEventProvider : public NrpnEventProvider
	{
	public:
		PitchBendSensitivityNrpnEventProvider()
			: NrpnEventProvider(MidiParameterType::CC_PBS_DELAY, MidiParameterType::CC_PBS_PITCH_BEND_SENSITIVITY)
		{}

		NrpnEvent getNrpnEvent(tick_t actualTick, int value) const override
		{
			return NrpnEvent(actualTick, nrpn, value, 0x00);
		}
	};

public:
	/**
	 * @brief Generate a list of VOCALOID MIDI event from a specified track.
	 * @param target An instance of Track.
	 * @param tempoList Tempo information.
	 * @param totalTicks Length of the sequence (in tick unit).
	 * @param preMeasureTicks Length of pre-measure (in tick unit).
	 * @param msPreSend Length of pre-send time in milli seconds.
	 * @return A list of VOCALOID MIDI event.
	 */
	static std::vector<MidiEvent> generateMidiEventList(
		Track const& target, TempoList const& tempoList, tick_t totalTicks, tick_t preMeasureTicks, int msPreSend);

protected:
	/**
	 * @brief Generate a list of NrpnEvent from a specified track.
	 * @param track An instance of Track.
	 * @param tempoList Tempo information.
	 * @param totalTicks Length of the sequence (in tick unit).
	 * @param preMeasureTicks Length of pre-measure (in tick unit).
	 * @param msPreSend Length of pre-send time in milli seconds.
	 * @return A list of NrpnEvent.
	 */
	static std::vector<NrpnEvent> generateNRPN(
		Track const& target, TempoList const& tempoList, tick_t totalTicks, tick_t preMeasureTicks, int msPreSend);

	/**
	 * @brief Generate a list of Expression(DYN) NrpnEvent from a specified track.
	 * @param track An instance of Track.
	 * @param tempoList Tempo information.
	 * @param msPreSend Length of pre-send time in milli seconds.
	 * @return A list of NrpnEvent.
	 */
	static std::vector<NrpnEvent> generateExpressionNRPN(Track const& track, TempoList const& tempoList, int preSendMilliseconds);

	/**
	 * @brief Generate prefix NrpnEvent for track.
	 * @return A NrpnEvent.
	 */
	static NrpnEvent generateHeaderNRPN();

	/**
	 * @brief Generate a list of singer change NrpnEvent.
	 *     This method is not intended to use for first singer event on a track.
	 *     That is generated in generateNRPN method.
	 * @param tempoList Tempo information.
	 * @param singerEvent A singer event.
	 * @param preSendMilliseconds Length of pre-send time in milli seconds.
	 * @return A list of NrpnEvent.
	 */
	static std::vector<NrpnEvent> generateSingerNRPN(TempoList const& tempoList, Event const& singerEvent, int preSendMilliseconds);

	/**
	 * @brief Generate NRPN from note events in the track.
	 * @param track A track, source of note events.
	 * @param noteEvent A note event, to be output.
	 * @param msPreSend Pre-send time in milli-seconds
	 * @param noteLocation Specifies note position relationship around the note event.<ul>
	 *     <li>00: the note item is bonded with both forward and backward note items.
	 *     <li>01: the note item is bonded with only backward note item.
	 *     <li>02: the note item is bonded with only forward note item.
	 *     <li>03: the note item is not boned with any note item.
	 *     </ul>
	 * @param lastDelay The delay time set to forward note item. Set NULL if the note item is the first note item in the track.
	 * @param [out] delay Delay time of the note item.
	 * @return Generated NRPN.
	 */
	static NrpnEvent generateNoteNRPN(Track const& track, TempoList const& tempoList, Event const& noteEvent, int msPreSend, int noteLocation, int* lastDelay, int* delay);

	/**
	 * @brief 指定したシーケンスの指定したトラックから, PitchBend の NRPN リストを作成する.
	 * @param sequence 出力元のシーケンス.
	 * @param track 出力するトラックの番号.
	 * @param msPreSend ミリ秒単位のプリセンド時間.
	 * @return NrpnEvent の配列.
	 */
	static std::vector<NrpnEvent> generatePitchBendNRPN(Track const& track, TempoList const& tempoList, int msPreSend);

	/**
	 * @brief 指定したシーケンスの指定したトラックから, PitchBendSensitivity の NRPN リストを作成する.
	 * @param sequence 出力元のシーケンス.
	 * @param track 出力するトラックの番号.
	 * @param msPreSend ミリ秒単位のプリセンド時間.
	 * @return NrpnEvent の配列.
	 */
	static std::vector<NrpnEvent> generatePitchBendSensitivityNRPN(Track const& track, TempoList const& tempoList, int msPreSend);

	/**
	 * @brief トラックの音符イベントから, ビブラート出力用の NRPN のリストを作成する.
	 * @param sequence 出力元のシーケンス.
	 * @param noteEvent 出力する音符イベント.
	 * @param msPreSend ミリ秒単位のプリセンド時間.
	 * @return NrpnEvent の配列.
	 */
	static std::vector<NrpnEvent> generateVibratoNRPN(TempoList const& tempoList, Event const& noteEvent, int msPreSend);

	/**
	 * @brief 指定したシーケンスの指定したトラックから, VoiceChangeParameter の NRPN リストを作成する.
	 * @param sequence 出力元のシーケンス.
	 * @param track 出力するトラックの番号.
	 * @param msPreSend ミリ秒単位のプリセンド時間.
	 * @return NrpnEvent の配列.
	 */
	static std::vector<NrpnEvent> generateVoiceChangeParameterNRPN(Track const& track, TempoList const& tempoList, int msPreSend, tick_t preMeasureTicks);

	static std::vector<NrpnEvent> generateFx2DepthNRPN(Track const& track, TempoList const& tempoList, int preSendMilliseconds);

	/**
	 * @brief Voice Change Parameter の NRPN を追加する.
	 * @param dest 追加先のテーブル.
	 * @param list Voice Change Parameter のデータ点が格納された BPList.
	 * @param sequence シーケンス.
	 * @param msPreSend ミリ秒単位のプリセンド時間.
	 * @param lastDelay 直前の delay 値(ミリ秒単位).
	 * @return delay 値(ミリ秒単位).
	 */
	static int addVoiceChangeParameters(std::vector<NrpnEvent>& dest, BPList const& list, TempoList const& tempoList, int msPreSend, int lastDelay);

	/**
	 * @brief 指定した時刻における, プリセンド込の時刻と, ディレイを取得する.
	 * @param tempoList テンポ情報.
	 * @param tick Tick 単位の時刻.
	 * @param msPreSend ミリ秒単位のプリセンド時間.
	 * @param[out] actualTick プリセンド分のクロックを引いた Tick 単位の時刻.
	 * @param[out] delay ミリ秒単位のプリセンド時間.
	 */
	static void _getActualTickAndDelay(TempoList const& tempoList, tick_t tick, int msPreSend, tick_t* actualTick, int* delay);

	/**
	 * @brief DATA の値を MSB と LSB に分解する.
	 * @param value 分解する値.
	 * @param[out] MSB の値.
	 * @param[out] LSB の値.
	 */
	static void _getMsbAndLsb(int value, int* msb, int* lsb);

private:
	/**
	 * @brief データ点のリストから, NRPN のリストを作成する.
	 * @param[out] result 作成した NRPN のリストの格納先.
	 * @param tempoList テンポ情報.
	 * @param preSendMilliseconds ミリ秒単位のプリセンド秒.
	 * @param list NRPN リストの元になるデータのリスト.
	 * @param delayNrpnType delay を指定する際の NRPN のタイプ.
	 * @param nrpnType データ点の値を指定する際の NRPN のタイプ.
	 */
	static void generateNRPNByBPList(
		std::vector<NrpnEvent>& result,
		TempoList const& tempoList, int preSendMilliseconds,
		BPList const& list, NrpnEventProvider const& provider
	);
};

VSQ_END_NAMESPACE
