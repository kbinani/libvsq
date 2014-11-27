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
		MidiParameterType::MidiParameterTypeEnum delayNrpn;
		MidiParameterType::MidiParameterTypeEnum nrpn;

	public:
		NrpnEventProvider(
			MidiParameterType::MidiParameterTypeEnum delayNrpnType,
			MidiParameterType::MidiParameterTypeEnum nrpnType)
		{
			this->delayNrpn = delayNrpnType;
			this->nrpn = nrpnType;
		}

		virtual ~NrpnEventProvider()
		{}

		virtual NrpnEvent getDelayNrpnEvent(tick_t actualClock, int delay)
		{
			int delayMsb, delayLsb;
			VocaloidMidiEventListFactory::_getMsbAndLsb(delay, &delayMsb, &delayLsb);
			return NrpnEvent(actualClock, delayNrpn, delayMsb, delayLsb);
		}

		virtual NrpnEvent getNrpnEvent(tick_t actualClock, int value)
		{
			return NrpnEvent(actualClock, nrpn, value);
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
		{
		}

		NrpnEvent getNrpnEvent(tick_t actualClock, int value) override
		{
			int actualValue = value + 0x2000;
			int msb, lsb;
			VocaloidMidiEventListFactory::_getMsbAndLsb(actualValue, &msb, &lsb);
			return NrpnEvent(actualClock, nrpn, msb, lsb);
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
		{
		}

		NrpnEvent getNrpnEvent(tick_t actualClock, int value) override
		{
			return NrpnEvent(actualClock, nrpn, value, 0x00);
		}
	};

public:
	/**
	 * @brief Generate a list of VOCALOID MIDI event from a specified track.
	 * @param target An instance of Track.
	 * @param tempoList Tempo information.
	 * @param totalClocks Length of the sequence (in tick unit).
	 * @param preMeasureClock Length of pre-measure (in tick unit).
	 * @param msPreSend Length of pre-send time in milli seconds.
	 * @return A list of VOCALOID MIDI event.
	 */
	static std::vector<MidiEvent> generateMidiEventList(
		Track* target, TempoList* tempoList, tick_t totalClocks, tick_t preMeasureClock, int msPreSend);

protected:
	/**
	 * @brief Generate a list of NrpnEvent from a specified track.
	 * @param track An instance of Track.
	 * @param tempoList Tempo information.
	 * @param totalClocks Length of the sequence (in tick unit).
	 * @param preMeasureClock Length of pre-measure (in tick unit).
	 * @param msPreSend Length of pre-send time in milli seconds.
	 * @return A list of NrpnEvent.
	 */
	static std::vector<NrpnEvent> generateNRPN(
		Track* target, TempoList* tempoList, tick_t totalClocks, tick_t preMeasureClock, int msPreSend);

	/**
	 * @brief Generate a list of Expression(DYN) NrpnEvent from a specified track.
	 * @param track An instance of Track.
	 * @param tempoList Tempo information.
	 * @param msPreSend Length of pre-send time in milli seconds.
	 * @return A list of NrpnEvent.
	 */
	static std::vector<NrpnEvent> generateExpressionNRPN(Track* track, TempoList* tempoList, int preSendMilliseconds);

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
	static std::vector<NrpnEvent> generateSingerNRPN(TempoList* tempoList, const Event* singerEvent, int preSendMilliseconds);

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
	static NrpnEvent generateNoteNRPN(Track* track, TempoList* tempoList, const Event* noteEvent, int msPreSend, int noteLocation, int* lastDelay, int* delay);

	/**
	 * @brief 指定したシーケンスの指定したトラックから、PitchBend の NRPN リストを作成する
	 * @param sequence (Sequence) 出力元のシーケンス
	 * @param track (int) 出力するトラックの番号
	 * @param msPreSend (int) ミリ秒単位のプリセンド時間
	 * @return (table<NrpnEvent>) NrpnEvent の配列
	 */
	static std::vector<NrpnEvent> generatePitchBendNRPN(Track* track, TempoList* tempoList, int msPreSend);

	/**
	 * @brief 指定したシーケンスの指定したトラックから、PitchBendSensitivity の NRPN リストを作成する
	 * @param sequence (Sequence) 出力元のシーケンス
	 * @param track (int) 出力するトラックの番号
	 * @param msPreSend (int) ミリ秒単位のプリセンド時間
	 * @return (table<NrpnEvent>) NrpnEvent の配列
	 */
	static std::vector<NrpnEvent> generatePitchBendSensitivityNRPN(Track* track, TempoList* tempoList, int msPreSend);

	/**
	 * @brief トラックの音符イベントから、ビブラート出力用の NRPN のリストを作成する
	 * @param sequence (Sequence) 出力元のシーケンス
	 * @param noteEvent (Event) 出力する音符イベント
	 * @param msPreSend (int) ミリ秒単位のプリセンド時間
	 * @return (table<NrpnEvent>) NrpnEvent の配列
	 */
	static std::vector<NrpnEvent> generateVibratoNRPN(TempoList* tempoList, const Event* noteEvent, int msPreSend);

	/**
	 * @brief 指定したシーケンスの指定したトラックから、VoiceChangeParameter の NRPN リストを作成する
	 * @param sequence (Sequence) 出力元のシーケンス
	 * @param track (int) 出力するトラックの番号
	 * @param msPreSend (int) ミリ秒単位のプリセンド時間
	 * @return (table<NrpnEvent>) NrpnEvent の配列
	 */
	static std::vector<NrpnEvent> generateVoiceChangeParameterNRPN(Track* track, TempoList* tempoList, int msPreSend, tick_t premeasure_clock);

	/**
	 * @param vsq [VsqFile]
	 * @param track [int]
	 * @param msPreSend [int]
	 * @return [VsqNrpn[] ]
	 */
	static std::vector<NrpnEvent> generateFx2DepthNRPN(Track* track, TempoList* tempoList, int preSendMilliseconds);

	/**
	 * @brief Voice Change Parameter の NRPN を追加する
	 * @param dest (table) 追加先のテーブル
	 * @param list (BPList) Voice Change Parameter のデータ点が格納された BPList
	 * @param sequence (Sequence) シーケンス
	 * @param msPreSend (int) ミリ秒単位のプリセンド時間
	 * @param lastDelay (int) 直前の delay 値(ミリ秒単位)
	 * @return (int) delay 値(ミリ秒単位)
	 */
	static int addVoiceChangeParameters(std::vector<NrpnEvent>& dest, BPList* list, TempoList* tempoList, int msPreSend, int lastDelay);

	/**
	 * @brief 指定した時刻における、プリセンド込の時刻と、ディレイを取得する
	 * @param tempoList テンポ情報
	 * @param clock (int) Tick 単位の時刻
	 * @param msPreSend (int) ミリ秒単位のプリセンド時間
	 * @return (int) プリセンド分のクロックを引いた Tick 単位の時刻
	 * @return (int) ミリ秒単位のプリセンド時間
	 */
	static void _getActualClockAndDelay(TempoList* tempoList, tick_t clock, int msPreSend, tick_t* actualClock, int* delay);

	/**
	 * @brief DATA の値を MSB と LSB に分解する
	 * @param value (int) 分解する値
	 * @param (int) MSB の値
	 * @param (int) LSB の値
	 */
	static void _getMsbAndLsb(int value, int* msb, int* lsb);

private:
	/**
	 * @brief データ点のリストから、NRPN のリストを作成する
	 * @param result 作成した NRPN のリストの格納先
	 * @param tempoList テンポ情報
	 * @param preSendMilliseconds ミリ秒単位のプリセンド秒
	 * @param list NRPN リストの元になるデータのリスト
	 * @param delayNrpnType delay を指定する際の NRPN のタイプ
	 * @param nrpnType データ点の値を指定する際の NRPN のタイプ
	 */
	static void generateNRPNByBPList(
		std::vector<NrpnEvent>& result,
		TempoList* tempoList, int preSendMilliseconds,
		BPList* list, NrpnEventProvider* provider
	);
};

VSQ_END_NAMESPACE
