/**
 * Sequence.cpp
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
#include "../include/libvsq/Sequence.hpp"

LIBVSQ_BEGIN_NAMESPACE

Sequence::Sequence()
{
	init("", 1, 4, 4, _baseTempo);
}

Sequence::Sequence(std::string const& singer, int preMeasure, int numerator, int denominator, int tempo)
{
	init(singer, preMeasure, numerator, denominator, tempo);
}

Sequence Sequence::clone() const
{
	Sequence ret("Miku", 1, 4, 4, _baseTempo);
	ret._track.clear();
	for (int i = 0; i < _track.size(); i++) {
		ret._track.push_back(_track[i].clone());
	}

	ret.tempoList = TempoList();
	for (int i = 0; i < tempoList.size(); i++) {
		ret.tempoList.push(tempoList.get(i).clone());
	}

	TimesigList copy = timesigList;
	for (int i = 0; i < copy.size(); i++) {
		ret.timesigList.push(copy.get(i).clone());
	}

	ret._totalTicks = _totalTicks;
	ret.master = master.clone();
	ret.mixer = mixer.clone();
	return ret;
}

Track const& Sequence::track(int trackIndex) const
{
	return _track[trackIndex];
}

Track& Sequence::track(int trackIndex)
{
	return _track[trackIndex];
}

std::vector<Track> const& Sequence::tracks() const
{
	return _track;
}

std::vector<Track>& Sequence::tracks()
{
	return _track;
}

int Sequence::baseTempo() const
{
	return _baseTempo;
}

tick_t Sequence::totalTicks() const
{
	return _totalTicks;
}

int Sequence::preMeasure() const
{
	return master.preMeasure;
}

tick_t Sequence::preMeasureTicks() const
{
	return _calculatePreMeasureInTick();
}

tick_t Sequence::tickPerQuarter() const
{
	return _tickPerQuarter;
}

std::vector<std::string> Sequence::curveNameList()
{
	static std::vector<std::string> const result = {
		"VEL",
		"DYN",
		"BRE",
		"BRI",
		"CLE",
		"OPE",
		"GEN",
		"POR",
		"PIT",
		"PBS",
	};
	return result;
}

void Sequence::updateTotalTicks()
{
	tick_t max = preMeasureTicks();
	std::vector<std::string> curveNameList = this->curveNameList();
	for (int i = 0; i < _track.size(); i++) {
		Track& track = _track[i];
		int numEvents = track.events().size();
		if (0 < numEvents) {
			Event const* lastItem = track.events().get(numEvents - 1);
			max = std::max(max, lastItem->tick + lastItem->length());
		}
		for (int j = 0; j < curveNameList.size(); j++) {
			std::string vct = curveNameList[j];
			BPList const* list = track.curve(vct);
			if (list) {
				int size = list->size();
				if (size > 0) {
					tick_t last_key = list->keyTickAt(size - 1);
					max = std::max(max, last_key);
				}
			}
		}
	}
	_totalTicks = max;
}

tick_t Sequence::_calculatePreMeasureInTick() const
{
	int pre_measure = master.preMeasure;
	int last_bar_count = timesigList.get(0).barCount;
	tick_t last_tick = timesigList.get(0).tick();
	int last_denominator = timesigList.get(0).denominator;
	int last_numerator = timesigList.get(0).numerator;
	for (int i = 1; i < timesigList.size(); i++) {
		if (timesigList.get(i).barCount >= pre_measure) {
			break;
		} else {
			last_bar_count = timesigList.get(i).barCount;
			last_tick = timesigList.get(i).tick();
			last_denominator = timesigList.get(i).denominator;
			last_numerator = timesigList.get(i).numerator;
		}
	}

	int remained = pre_measure - last_bar_count;// プリメジャーの終わりまでの残り小節数
	return last_tick + (int)::floor(remained * last_numerator * 480 * 4 / (double)last_denominator);
}

void Sequence::init(std::string const& singer, int preMeasure, int numerator, int denominator, int tempo)
{
	_totalTicks = preMeasure * 480 * 4 / denominator * numerator;
	_track.push_back(Track("Voice1", singer));
	master = Master(preMeasure);
	mixer = Mixer(0, 0, 0, 0);
	mixer.slave.push_back(MixerItem(0, 0, 0, 0));
	timesigList.push(Timesig(numerator, denominator, 0));
	tempoList.push(Tempo(0, tempo));
}

LIBVSQ_END_NAMESPACE
