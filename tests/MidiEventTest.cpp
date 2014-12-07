#include "Util.hpp"
#include "../include/libvsq/MidiEvent.hpp"
#include "../include/libvsq/ByteArrayOutputStream.hpp"
#include "../include/libvsq/InputStream.hpp"

using namespace std;
using namespace vsq;

class MemoryInputStream : public InputStream
{
private:
	char* data;
	int length;
	int pointer;

public:
	explicit MemoryInputStream(char* data, int length)
	{
		this->data = data;
		this->length = length;
		this->pointer = 0;
	}

	int read() override
	{
		if (0 <= pointer && pointer < length) {
			return 0xFF & data[this->pointer++];
		} else {
			return -1;
		}
	}

	int read(char* buffer, int64_t startIndex, int64_t length) override
	{
		int count = 0;
		for (int64_t i = 0; i < length; ++i) {
			int c = read();
			if (c < 0) {
				break;
			}
			buffer[startIndex + i] = c;
			++count;
		}
		return count;
	}

	void close() override
	{
		length = 0;
	}

	int64_t getPointer() override
	{
		return pointer;
	}

	void seek(int64_t pointer) override
	{
		this->pointer = pointer;
	}
};

TEST(MidiEventTest, testWriteData)
{
	ByteArrayOutputStream stream;
	MidiEvent event;
	event.firstByte = 0x91;
	event.data.push_back(64);
	event.data.push_back(127);
	event.writeData(stream);
	ostringstream expected;
	expected << (char)0x91 << (char)64 << (char)127;
	EXPECT_EQ(expected.str(), stream.toString());
	stream.close();
}

TEST(MidiEventTest, testWriteDataWithMetaEvent)
{
	ByteArrayOutputStream stream;
	MidiEvent event;
	event.firstByte = 0xff;
	event.data.push_back(0x51);
	event.data.push_back(0x82);
	event.data.push_back(0x81);
	event.data.push_back(0x80);
	event.writeData(stream);
	ostringstream expected;
	expected << (char)0xff << (char)0x51 << (char)3 << (char)0x82 << (char)0x81 << (char)0x80;
	EXPECT_EQ(expected.str(), stream.toString());
}

TEST(MidiEventTest, testCompareTo)
{
	MidiEvent a;
	MidiEvent b;
	a.tick = 0;
	b.tick = 480;
	EXPECT_TRUE(0 < b.compareTo(a));
	EXPECT_TRUE(0 > a.compareTo(b));

	a.firstByte = 1;
	b.firstByte = 2;
	a.tick = 0;
	b.tick = 0;
	EXPECT_EQ(0, a.compareTo(b));

	//同じ音程の, Note OnとNote Offが続いていた場合, Note Offが先, Note Onが後ろになる
	a.firstByte = 0x92;
	b.firstByte = 0x82;
	a.tick = 0;
	b.tick = 0;
	a.data.clear();
	a.data.push_back(64);
	a.data.push_back(127);   // note#=64, vel=127の Note On
	b.data.clear();
	b.data.push_back(64);
	b.data.push_back(127);   // note#=64, vel=127の Note Off
	// b => a
	EXPECT_TRUE(0 < a.compareTo(b));
	EXPECT_TRUE(0 > b.compareTo(a));

	//同じ音程の, Note OnとNote Offが続いていた場合, Note Offが先, Note Onが後ろになる
	//ただし, Note Offが, ベロシティー0のNote Onとして表現されている場合
	a.firstByte = 0x91;
	b.firstByte = 0x91;
	a.tick = 0;
	b.tick = 0;
	a.data.clear();
	a.data.push_back(64);
	a.data.push_back(127);   // note#=64, vel=127の Note On
	b.data.clear();
	b.data.push_back(64);
	b.data.push_back(0);     // note#=64, vel=0の Note On, vel=0なので Note Off とみなされる
	// b => a
	EXPECT_TRUE(0 < a.compareTo(b));
	EXPECT_TRUE(0 > b.compareTo(a));

	a.firstByte = 90;
	b.firstByte = 80;
	a.tick = 0;
	b.tick = 0;
	a.data.clear();
	a.data.push_back(63);
	a.data.push_back(127);
	b.data.clear();
	b.data.push_back(64);
	b.data.push_back(127);
	EXPECT_EQ(0, a.compareTo(b));
	EXPECT_EQ(0, b.compareTo(a));
}

TEST(MidiEventTest, testGenerateTimeSigEvent)
{
	MidiEvent event = MidiEvent::generateTimeSigEvent(10, 3, 2);
	EXPECT_EQ((tick_t)10, event.tick);
	EXPECT_EQ(0xff, event.firstByte);
	EXPECT_EQ(5, (int)event.data.size());
	EXPECT_EQ(0x58, event.data[0]);
	EXPECT_EQ(3, event.data[1]);
	EXPECT_EQ(1, event.data[2]);
	EXPECT_EQ(0x18, event.data[3]);
	EXPECT_EQ(0x08, event.data[4]);
}

TEST(MidiEventTest, testGenerateTempoChangeEvent)
{
	MidiEvent event = MidiEvent::generateTempoChangeEvent(12, 0x828180);
	EXPECT_EQ((tick_t)12, event.tick);
	EXPECT_EQ(0xff, event.firstByte);
	EXPECT_EQ(4, (int)event.data.size());
	EXPECT_EQ(0x51, event.data[0]);
	EXPECT_EQ(0x82, event.data[1]);
	EXPECT_EQ(0x81, event.data[2]);
	EXPECT_EQ(0x80, event.data[3]);
}

TEST(MidiEventTest, testWriteDeltaTick)
{
	ByteArrayOutputStream stream;
	MidiEvent::writeDeltaTick(stream, 0);
	string expected = " ";
	expected[0] = 0x0;
	EXPECT_EQ(expected, stream.toString());

	stream.seek(0);
	MidiEvent::writeDeltaTick(stream, 127);
	expected[0] = 0x7f;
	EXPECT_EQ(expected, stream.toString());

	stream.seek(0);
	MidiEvent::writeDeltaTick(stream, 128);
	expected = "  ";
	expected[0] = 0x81;
	expected[1] = 0x00;
	EXPECT_EQ(expected, stream.toString());

	stream.seek(0);
	MidiEvent::writeDeltaTick(stream, 12345678);
	expected = "    ";
	expected[0] = 0x85;
	expected[1] = 0xf1;
	expected[2] = 0xc2;
	expected[3] = 0x4e;
	EXPECT_EQ(expected, stream.toString());
}

TEST(MidiEventTest, testReadDeltaTick)
{
	// 空のストリームが渡された場合
	char* emptyData = nullptr;
	MemoryInputStream emptyStream(emptyData, 0);
	EXPECT_EQ((tick_t)0, MidiEvent::readDeltaTick(emptyStream));

	// 2バイト読み込む場合
	char data[] = { (char)0x81, 0x00 };
	MemoryInputStream stream(data, 2);
	EXPECT_EQ((tick_t)128, MidiEvent::readDeltaTick(stream));

	// 読み込みの途中でEOFとなる場合
	char data2[] = { (char)0x81 };
	MemoryInputStream stream2(data2, 1);
	EXPECT_EQ((tick_t)0x1, MidiEvent::readDeltaTick(stream2));
}

TEST(MidiEventTest, testRead)
{
	{
		// データ部が3byteの場合(note off)
		char data[] = { 0x00, (char)0x81, 0x01, 0x02 };
		MemoryInputStream stream(data, 4);
		tick_t lastTick = 10;
		uint8_t lastStatus = 0;
		MidiEvent event = MidiEvent::read(stream, lastTick, lastStatus);
		EXPECT_EQ((tick_t)10, lastTick);
		EXPECT_EQ((uint8_t)0x81, lastStatus);
		EXPECT_EQ((tick_t)10, event.tick);
		EXPECT_EQ(0x81, event.firstByte);
		EXPECT_EQ((size_t)2, event.data.size());
		EXPECT_EQ(0x01, event.data[0]);
		EXPECT_EQ(0x02, event.data[1]);
	}

	{
		// データ部が3byteの場合(note off, ランニングステータスが適用されるパターン)
		char data[] = { 0x00, 0x03, 0x04 };
		MemoryInputStream stream(data, 3);
		tick_t lastTick = 11;
		uint8_t lastStatus = 0x81;
		MidiEvent event = MidiEvent::read(stream, lastTick, lastStatus);
		EXPECT_EQ((tick_t)11, lastTick);
		EXPECT_EQ((uint8_t)0x81, lastStatus);
		EXPECT_EQ((tick_t)11, event.tick);
		EXPECT_EQ(0x81, event.firstByte);
		EXPECT_EQ((size_t)2, event.data.size());
		EXPECT_EQ(0x03, event.data[0]);
		EXPECT_EQ(0x04, event.data[1]);
	}

	{
		// データ部が2byteの場合
		char data[] = { 0x01, (char)0xF3, 0x05 };
		MemoryInputStream stream(data, 3);
		tick_t lastTick = 12;
		uint8_t lastStatus = 0;
		MidiEvent event = MidiEvent::read(stream, lastTick, lastStatus);
		EXPECT_EQ((tick_t)13, lastTick);
		EXPECT_EQ((uint8_t)0xF3, lastStatus);
		EXPECT_EQ((tick_t)13, event.tick);
		EXPECT_EQ(0xF3, event.firstByte);
		EXPECT_EQ((size_t)1, event.data.size());
		EXPECT_EQ(0x05, event.data[0]);
	}

	{
		// データ部が1byteの場合
		char data[] = { 0x02, (char)0xF6 };
		MemoryInputStream stream(data, 2);
		tick_t lastTick = 13;
		uint8_t lastStatus = 0;
		MidiEvent event = MidiEvent::read(stream, lastTick, lastStatus);
		EXPECT_EQ((tick_t)15, lastTick);
		EXPECT_EQ((uint8_t)0xF6, lastStatus);
		EXPECT_EQ((tick_t)15, event.tick);
		EXPECT_EQ(0xF6, event.firstByte);
		EXPECT_EQ((size_t)0, event.data.size());
	}

	{
		// メタイベントの場合
		char data[] = { 0x03, (char)0xFF, 0x06, 0x05, 0x01, 0x02, 0x03, 0x04, 0x05 };
		MemoryInputStream stream(data, 9);
		tick_t lastTick = 14;
		uint8_t lastStatus = 0;
		MidiEvent event = MidiEvent::read(stream, lastTick, lastStatus);
		EXPECT_EQ((tick_t)17, lastTick);
		EXPECT_EQ((uint8_t)0xFF, lastStatus);
		EXPECT_EQ((tick_t)17, event.tick);
		EXPECT_EQ(0xFF, event.firstByte);
		EXPECT_EQ((size_t)6, event.data.size());
		EXPECT_EQ(0x06, event.data[0]);
		EXPECT_EQ(0x01, event.data[1]);
		EXPECT_EQ(0x02, event.data[2]);
		EXPECT_EQ(0x03, event.data[3]);
		EXPECT_EQ(0x04, event.data[4]);
		EXPECT_EQ(0x05, event.data[5]);
	}

	{
		// f0ステータスのSysEx
		char data[] = { 0x04, (char)0xF0, 0x03, (char)0xF0, 0x06, 0x07, (char)0xF7 };
		MemoryInputStream stream(data, 7);
		tick_t lastTick = 0;
		uint8_t lastStatus = 0;
		MidiEvent event = MidiEvent::read(stream, lastTick, lastStatus);
		EXPECT_EQ((tick_t)0x04, lastTick);
		EXPECT_EQ((uint8_t)0xF0, lastStatus);
		EXPECT_EQ((tick_t)0x04, event.tick);
		EXPECT_EQ(0xF0, event.firstByte);
		EXPECT_EQ((size_t)4, event.data.size());
		EXPECT_EQ(0xF0, event.data[0]);
		EXPECT_EQ(0x06, event.data[1]);
		EXPECT_EQ(0x07, event.data[2]);
		EXPECT_EQ(0xF7, event.data[3]);
	}

	{
		// f7ステータスのSysEx
		char data[] = { 0x05, (char)0xF7, 0x03, 0x08, 0x09, 0x0A };
		MemoryInputStream stream(data, 6);
		tick_t lastTick = 1440;
		uint8_t lastStatus = 0x81;
		MidiEvent event = MidiEvent::read(stream, lastTick, lastStatus);
		EXPECT_EQ((tick_t)1445, lastTick);
		EXPECT_EQ((uint8_t)0xF7, lastStatus);
		EXPECT_EQ((tick_t)1445, event.tick);
		EXPECT_EQ(0xF7, event.firstByte);
		EXPECT_EQ((size_t)3, event.data.size());
		EXPECT_EQ(0x08, event.data[0]);
		EXPECT_EQ(0x09, event.data[1]);
		EXPECT_EQ(0x0A, event.data[2]);
	}

	{
		// 処理できないMIDIイベント
		char data[] = { 0x01, (char)0xF4 };
		MemoryInputStream stream(data, 2);
		tick_t lastTick = 0;
		uint8_t lastStatus = 0;
		try {
			MidiEvent::read(stream, lastTick, lastStatus);
			GTEST_FAIL(); // 期待した例外がスローされない.
		} catch (MidiEvent::ParseException& e) {
			EXPECT_EQ(string("don't know how to process first_byte: 0xF4"), e.message());
		}
	}
}
