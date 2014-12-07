#include "Util.hpp"
#include "../include/libvsq/NrpnEvent.hpp"

using namespace std;
using namespace vsq;

class NrpnEventStub : public NrpnEvent
{
public:
	explicit NrpnEventStub()
	{
	}

	explicit NrpnEventStub(tick_t tick, MidiParameterType nrpn, int dataMsb)
		: NrpnEvent(tick, nrpn, dataMsb)
	{
	}

	explicit NrpnEventStub(tick_t tick, MidiParameterType nrpn, int dataMsb, int dataLsb)
		: NrpnEvent(tick, nrpn, dataMsb, dataLsb)
	{
	}

	vector<NrpnEvent> getList()
	{
		return this->_list;
	}

	void setList(vector<NrpnEvent> list)
	{
		this->_list = list;
	}
};

TEST(NrpnEventTest, testtestConstructWithoutLSB)
{
	NrpnEvent event(480, MidiParameterType::CVM_NM_VELOCITY, 64);
	EXPECT_EQ((tick_t)480, event.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_VELOCITY, event.nrpn);
	EXPECT_EQ(64, event.dataMSB);
	EXPECT_EQ(0, event.dataLSB);
	EXPECT_EQ(false, event.hasLSB);
	EXPECT_EQ(false, event.isMSBOmittingRequired);
}

TEST(NrpnEventTest, testtestConstructWithLSB)
{
	NrpnEvent event(480, MidiParameterType::CVM_NM_DELAY, 0x01, 0x23);
	EXPECT_EQ((tick_t)480, event.tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_DELAY, event.nrpn);
	EXPECT_EQ(0x01, event.dataMSB);
	EXPECT_EQ(0x23, event.dataLSB);
	EXPECT_TRUE(event.hasLSB);
	EXPECT_EQ(false, event.isMSBOmittingRequired);
}

TEST(NrpnEventTest, testtestCompareTo)
{
	// tick が異なる場合
	NrpnEvent a(480, MidiParameterType::CVM_NM_VELOCITY, 64);
	NrpnEvent b(1920, MidiParameterType::CC_CV_VERSION_AND_DEVICE, 0x01, 0x23);
	EXPECT_TRUE(b.compareTo(a) > 0);
	EXPECT_TRUE(a.compareTo(b) < 0);
	EXPECT_EQ(0, a.compareTo(a));

	// tick が同じ場合
	// CVM_NM_VELOCITY:          0x5003 <- a
	// CC_CV_VERSION_AND_DEVICE: 0x6100 <- b
	// b - a の並びになるはず
	b.tick = 480;
	EXPECT_TRUE(0 > b.compareTo(a));
	EXPECT_TRUE(0 < a.compareTo(b));
	EXPECT_EQ(0, a.compareTo(a));
}

TEST(NrpnEventTest, testtestAppend)
{
	// NRPNとDATA MSBを指定したappend
	NrpnEventStub target = NrpnEventStub(1920, MidiParameterType::CVM_NM_DELAY, 0x01, 0x23);
	target.append(MidiParameterType::CVM_NM_VELOCITY, 0x02);
	vector<NrpnEvent> list = target.getList();
	EXPECT_EQ((size_t)1, list.size());
	EXPECT_EQ((tick_t)1920, list[0].tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_VELOCITY, list[0].nrpn);
	EXPECT_EQ(0x02, list[0].dataMSB);
	EXPECT_EQ(0x00, list[0].dataLSB);
	EXPECT_EQ(false, list[0].hasLSB);
	EXPECT_EQ(false, list[0].isMSBOmittingRequired);

	// NRPN, DATA MSB,およびDATA LSBを指定したappend
	target = NrpnEventStub(1920, MidiParameterType::CVM_NM_DELAY, 0x01, 0x23);
	target.append(MidiParameterType::CVM_NM_VELOCITY, 0x02, 0x03);
	list = target.getList();
	EXPECT_EQ((size_t)1, list.size());
	EXPECT_EQ((tick_t)1920, list[0].tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_VELOCITY, list[0].nrpn);
	EXPECT_EQ(0x02, list[0].dataMSB);
	EXPECT_EQ(0x03, list[0].dataLSB);
	EXPECT_TRUE(list[0].hasLSB);
	EXPECT_EQ(false, list[0].isMSBOmittingRequired);

	// NRPN, DATA MSBと, MSB省略フラグを指定したappend
	target = NrpnEventStub(1920, MidiParameterType::CVM_NM_DELAY, 0x01, 0x23);
	target.append(MidiParameterType::CVM_NM_VELOCITY, 0x01, true);
	list = target.getList();
	EXPECT_EQ((size_t)1, list.size());
	EXPECT_EQ((tick_t)1920, list[0].tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_VELOCITY, list[0].nrpn);
	EXPECT_EQ(0x01, list[0].dataMSB);
	EXPECT_EQ(0x00, list[0].dataLSB);
	EXPECT_EQ(false, list[0].hasLSB);
	EXPECT_TRUE(list[0].isMSBOmittingRequired);

	// NRPN, DATA MSB, DATA LSBと, MSB省略フラグを指定したappend
	target = NrpnEventStub(1920, MidiParameterType::CVM_NM_DELAY, 0x01, 0x23);
	target.append(MidiParameterType::CVM_NM_VELOCITY, 0x01, 0x02, true);
	list = target.getList();
	EXPECT_EQ((size_t)1, list.size());
	EXPECT_EQ((tick_t)1920, list[0].tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_VELOCITY, list[0].nrpn);
	EXPECT_EQ(0x01, list[0].dataMSB);
	EXPECT_EQ(0x02, list[0].dataLSB);
	EXPECT_TRUE(list[0].hasLSB);
	EXPECT_TRUE(list[0].isMSBOmittingRequired);
}

TEST(NrpnEventTest, testtestExpand)
{

	// DATA LSB を保持している単一 NrpnEvent アイテムの expand
	NrpnEventStub target = NrpnEventStub(1920, MidiParameterType::CVM_NM_DELAY, 0x01, 0x23);
	vector<NrpnEvent> list = target.expand();
	EXPECT_EQ((size_t)1, list.size());
	EXPECT_EQ((tick_t)1920, list[0].tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_DELAY, list[0].nrpn);
	EXPECT_EQ(0x01, list[0].dataMSB);
	EXPECT_EQ(0x23, list[0].dataLSB);
	EXPECT_TRUE(list[0].hasLSB);
	EXPECT_EQ(false, list[0].isMSBOmittingRequired);

	// DATA LSB を保持していない単一 NrpnEvent アイテムの expand
	target = NrpnEventStub(1920, MidiParameterType::CVM_NM_DELAY, 0x01);
	list = target.expand();
	EXPECT_EQ((size_t)1, list.size());
	EXPECT_EQ((tick_t)1920, list[0].tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_DELAY, list[0].nrpn);
	EXPECT_EQ(0x01, list[0].dataMSB);
	EXPECT_EQ(0x00, list[0].dataLSB);
	EXPECT_EQ(false, list[0].hasLSB);
	EXPECT_EQ(false, list[0].isMSBOmittingRequired);

	// 子・孫アイテムを持つ NrpnEvent の expand
	target = NrpnEventStub(1920, MidiParameterType::CVM_NM_DELAY, 0x01);
	target.append(MidiParameterType::CVM_NM_VELOCITY, 64);   // 子アイテムを追加
	vector<NrpnEvent> childEventList = target.getList();
	childEventList[0].append(MidiParameterType::CVM_NM_NOTE_NUMBER, 60);   // 孫アイテムを追加
	target.setList(childEventList);
	list = target.expand();
	EXPECT_EQ((size_t)3, list.size());

	EXPECT_EQ((tick_t)1920, list[0].tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_DELAY, list[0].nrpn);
	EXPECT_EQ(0x01, list[0].dataMSB);
	EXPECT_EQ(0x00, list[0].dataLSB);
	EXPECT_EQ(false, list[0].hasLSB);
	EXPECT_EQ(false, list[0].isMSBOmittingRequired);

	EXPECT_EQ((tick_t)1920, list[1].tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_VELOCITY, list[1].nrpn);
	EXPECT_EQ(64, list[1].dataMSB);
	EXPECT_EQ(0x00, list[1].dataLSB);
	EXPECT_EQ(false, list[1].hasLSB);
	EXPECT_EQ(false, list[1].isMSBOmittingRequired);

	EXPECT_EQ((tick_t)1920, list[2].tick);
	EXPECT_EQ(MidiParameterType::CVM_NM_NOTE_NUMBER, list[2].nrpn);
	EXPECT_EQ(60, list[2].dataMSB);
	EXPECT_EQ(0x00, list[2].dataLSB);
	EXPECT_EQ(false, list[2].hasLSB);
	EXPECT_EQ(false, list[2].isMSBOmittingRequired);
}

TEST(NrpnEventTest, testtestCompare)
{
	NrpnEvent a(480, MidiParameterType::CVM_NM_VELOCITY, 64);
	NrpnEvent b(1920, MidiParameterType::CVM_NM_DELAY, 0x01, 0x23);
	EXPECT_TRUE(NrpnEvent::compare(a, b));
	EXPECT_EQ(false, NrpnEvent::compare(b, a));
	EXPECT_EQ(false, NrpnEvent::compare(a, a));
}

TEST(NrpnEventTest, testtestConvert)
{
	NrpnEvent target = NrpnEvent(1920, MidiParameterType::CVM_NM_DELAY, 0x01, 0x23);
	target.append(MidiParameterType::CVM_NM_VELOCITY, 0x01, 0x02, true);
	target.append(MidiParameterType::CVM_NM_NOTE_NUMBER, 60);

	vector<MidiEvent> events = NrpnEvent::convert(target.expand());
	EXPECT_EQ((size_t)10, events.size());
	MidiEvent e;

	// CVM_NM_DELAY
	e = events[0];
	EXPECT_EQ((tick_t)1920, e.tick);
	EXPECT_EQ(0xb0, e.firstByte);
	EXPECT_EQ((size_t)2, e.data.size());
	EXPECT_EQ(0x63, e.data[0]);
	EXPECT_EQ(((static_cast<int>(MidiParameterType::CVM_NM_DELAY) & 0xff00) >> 8), e.data[1]);
	e = events[1];
	EXPECT_EQ((tick_t)1920, e.tick);
	EXPECT_EQ(0xb0, e.firstByte);
	EXPECT_EQ((size_t)2, e.data.size());
	EXPECT_EQ(0x62, e.data[0]);
	EXPECT_EQ((static_cast<int>(MidiParameterType::CVM_NM_DELAY) & 0xff), e.data[1]);
	e = events[2];
	EXPECT_EQ((tick_t)1920, e.tick);
	EXPECT_EQ(0xb0, e.firstByte);
	EXPECT_EQ((size_t)2, e.data.size());
	EXPECT_EQ(0x06, e.data[0]);
	EXPECT_EQ(0x01, e.data[1]);
	e = events[3];
	EXPECT_EQ((tick_t)1920, e.tick);
	EXPECT_EQ(0xb0, e.firstByte);
	EXPECT_EQ((size_t)2, e.data.size());
	EXPECT_EQ(0x26, e.data[0]);
	EXPECT_EQ(0x23, e.data[1]);

	// CVM_NM_VELOCITY
	e = events[4];
	EXPECT_EQ((tick_t)1920, e.tick);
	EXPECT_EQ(0xb0, e.firstByte);
	EXPECT_EQ((size_t)2, e.data.size());
	EXPECT_EQ(0x62, e.data[0]);
	EXPECT_EQ(static_cast<int>(MidiParameterType::CVM_NM_VELOCITY) & 0xff, e.data[1]);
	e = events[5];
	EXPECT_EQ((tick_t)1920, e.tick);
	EXPECT_EQ(0xb0, e.firstByte);
	EXPECT_EQ((size_t)2, e.data.size());
	EXPECT_EQ(0x06, e.data[0]);
	EXPECT_EQ(0x01, e.data[1]);
	e = events[6];
	EXPECT_EQ((tick_t)1920, e.tick);
	EXPECT_EQ(0xb0, e.firstByte);
	EXPECT_EQ((size_t)2, e.data.size());
	EXPECT_EQ(0x26, e.data[0]);
	EXPECT_EQ(0x02, e.data[1]);

	// CVM_NM_NOTE_NUMBER
	e = events[7];
	EXPECT_EQ((tick_t)1920, e.tick);
	EXPECT_EQ(0xb0, e.firstByte);
	EXPECT_EQ((size_t)2, e.data.size());
	EXPECT_EQ(0x63, e.data[0]);
	EXPECT_EQ(((static_cast<int>(MidiParameterType::CVM_NM_NOTE_NUMBER) & 0xff00) >> 8), e.data[1]);
	e = events[8];
	EXPECT_EQ((tick_t)1920, e.tick);
	EXPECT_EQ(0xb0, e.firstByte);
	EXPECT_EQ((size_t)2, e.data.size());
	EXPECT_EQ(0x62, e.data[0]);
	EXPECT_EQ(static_cast<int>(MidiParameterType::CVM_NM_NOTE_NUMBER) & 0xff, e.data[1]);
	e = events[9];
	EXPECT_EQ((tick_t)1920, e.tick);
	EXPECT_EQ(0xb0, e.firstByte);
	EXPECT_EQ((size_t)2, e.data.size());
	EXPECT_EQ(0x06, e.data[0]);
	EXPECT_EQ(60, e.data[1]);
}
