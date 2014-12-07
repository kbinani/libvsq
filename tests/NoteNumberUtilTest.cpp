#include "Util.hpp"
#include "../include/libvsq/NoteNumberUtil.hpp"

using namespace std;
using namespace vsq;

TEST(NoteNumberUtilTest, getNoteStringBase)
{
	EXPECT_EQ(string("C"), NoteNumberUtil::getNoteStringBase(60));
	EXPECT_EQ(string("C"), NoteNumberUtil::getNoteStringBase(61));
	EXPECT_EQ(string("D"), NoteNumberUtil::getNoteStringBase(62));
	EXPECT_EQ(string("E"), NoteNumberUtil::getNoteStringBase(63));
	EXPECT_EQ(string("E"), NoteNumberUtil::getNoteStringBase(64));
	EXPECT_EQ(string("F"), NoteNumberUtil::getNoteStringBase(65));
	EXPECT_EQ(string("F"), NoteNumberUtil::getNoteStringBase(66));
	EXPECT_EQ(string("G"), NoteNumberUtil::getNoteStringBase(67));
	EXPECT_EQ(string("G"), NoteNumberUtil::getNoteStringBase(68));
	EXPECT_EQ(string("A"), NoteNumberUtil::getNoteStringBase(69));
	EXPECT_EQ(string("B"), NoteNumberUtil::getNoteStringBase(70));
	EXPECT_EQ(string("B"), NoteNumberUtil::getNoteStringBase(71));
}

TEST(NoteNumberUtilTest, getNoteOctave)
{
	EXPECT_EQ(3, NoteNumberUtil::getNoteOctave(61));
}

TEST(NoteNumberUtilTest, getNoteAlter)
{
	EXPECT_EQ(0, NoteNumberUtil::getNoteAlter(60));
	EXPECT_EQ(1, NoteNumberUtil::getNoteAlter(61));
	EXPECT_EQ(0, NoteNumberUtil::getNoteAlter(62));
	EXPECT_EQ(-1, NoteNumberUtil::getNoteAlter(63));
	EXPECT_EQ(0, NoteNumberUtil::getNoteAlter(64));
	EXPECT_EQ(0, NoteNumberUtil::getNoteAlter(65));
	EXPECT_EQ(1, NoteNumberUtil::getNoteAlter(66));
	EXPECT_EQ(0, NoteNumberUtil::getNoteAlter(67));
	EXPECT_EQ(1, NoteNumberUtil::getNoteAlter(68));
	EXPECT_EQ(0, NoteNumberUtil::getNoteAlter(69));
	EXPECT_EQ(-1, NoteNumberUtil::getNoteAlter(70));
	EXPECT_EQ(0, NoteNumberUtil::getNoteAlter(71));

	EXPECT_EQ(1, NoteNumberUtil::getNoteAlter(-11));
}
