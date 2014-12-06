/**
 * @file NoteNumberUtil.cpp
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
#include "../include/libvsq/NoteNumberUtil.hpp"

LIBVSQ_BEGIN_NAMESPACE

std::string NoteNumberUtil::getNoteStringBase(int note)
{
	int odd = note % 12;
	switch (odd) {
		case 0:
		case 1: {
			return "C";
		}
		case 2: {
			return "D";
		}
		case 3:
		case 4: {
			return "E";
		}
		case 5:
		case 6: {
			return "F";
		}
		case 7:
		case 8: {
			return "G";
		}
		case 9: {
			return "A";
		}
		case 10:
		case 11: {
			return "B";
		}
		default: {
			return "";
		}
	}
}

int NoteNumberUtil::getNoteOctave(int note)
{
	int odd = note % 12;
	return (note - odd) / 12 - 2;
}

int NoteNumberUtil::getNoteAlter(int note)
{
	static int const ALTER[13] = { 0, 1, 0, -1, 0, 0, 1, 0, 1, 0, -1, 0, 0 };
	note = note % 12 + 12;
	return ALTER[note % 12];
}

LIBVSQ_END_NAMESPACE
