#include "Util.hpp"
#include "../NrpnEvent.hpp"

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

class NrpnEventTest : public CppUnit::TestCase
{
public:
	void testConstructWithoutLSB()
	{
		NrpnEvent event(480, MidiParameterType::CVM_NM_VELOCITY, 64);
		CPPUNIT_ASSERT_EQUAL((tick_t)480, event.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_VELOCITY, event.nrpn);
		CPPUNIT_ASSERT_EQUAL(64, event.dataMSB);
		CPPUNIT_ASSERT_EQUAL(0, event.dataLSB);
		CPPUNIT_ASSERT_EQUAL(false, event.hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, event.isMSBOmittingRequired);
	}

	void testConstructWithLSB()
	{
		NrpnEvent event(480, MidiParameterType::CVM_NM_DELAY, 0x01, 0x23);
		CPPUNIT_ASSERT_EQUAL((tick_t)480, event.tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_DELAY, event.nrpn);
		CPPUNIT_ASSERT_EQUAL(0x01, event.dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x23, event.dataLSB);
		CPPUNIT_ASSERT(event.hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, event.isMSBOmittingRequired);
	}

	void testCompareTo()
	{
		// tick が異なる場合
		NrpnEvent a(480, MidiParameterType::CVM_NM_VELOCITY, 64);
		NrpnEvent b(1920, MidiParameterType::CC_CV_VERSION_AND_DEVICE, 0x01, 0x23);
		CPPUNIT_ASSERT(b.compareTo(a) > 0);
		CPPUNIT_ASSERT(a.compareTo(b) < 0);
		CPPUNIT_ASSERT_EQUAL(0, a.compareTo(a));

		// tick が同じ場合
		// CVM_NM_VELOCITY:          0x5003 <- a
		// CC_CV_VERSION_AND_DEVICE: 0x6100 <- b
		// b - a の並びになるはず
		b.tick = 480;
		CPPUNIT_ASSERT(0 > b.compareTo(a));
		CPPUNIT_ASSERT(0 < a.compareTo(b));
		CPPUNIT_ASSERT_EQUAL(0, a.compareTo(a));
	}

	void testAppend()
	{
		// NRPNとDATA MSBを指定したappend
		NrpnEventStub target = NrpnEventStub(1920, MidiParameterType::CVM_NM_DELAY, 0x01, 0x23);
		target.append(MidiParameterType::CVM_NM_VELOCITY, 0x02);
		vector<NrpnEvent> list = target.getList();
		CPPUNIT_ASSERT_EQUAL((size_t)1, list.size());
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, list[0].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_VELOCITY, list[0].nrpn);
		CPPUNIT_ASSERT_EQUAL(0x02, list[0].dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x00, list[0].dataLSB);
		CPPUNIT_ASSERT_EQUAL(false, list[0].hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, list[0].isMSBOmittingRequired);

		// NRPN, DATA MSB,およびDATA LSBを指定したappend
		target = NrpnEventStub(1920, MidiParameterType::CVM_NM_DELAY, 0x01, 0x23);
		target.append(MidiParameterType::CVM_NM_VELOCITY, 0x02, 0x03);
		list = target.getList();
		CPPUNIT_ASSERT_EQUAL((size_t)1, list.size());
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, list[0].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_VELOCITY, list[0].nrpn);
		CPPUNIT_ASSERT_EQUAL(0x02, list[0].dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x03, list[0].dataLSB);
		CPPUNIT_ASSERT(list[0].hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, list[0].isMSBOmittingRequired);

		// NRPN, DATA MSBと, MSB省略フラグを指定したappend
		target = NrpnEventStub(1920, MidiParameterType::CVM_NM_DELAY, 0x01, 0x23);
		target.append(MidiParameterType::CVM_NM_VELOCITY, 0x01, true);
		list = target.getList();
		CPPUNIT_ASSERT_EQUAL((size_t)1, list.size());
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, list[0].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_VELOCITY, list[0].nrpn);
		CPPUNIT_ASSERT_EQUAL(0x01, list[0].dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x00, list[0].dataLSB);
		CPPUNIT_ASSERT_EQUAL(false, list[0].hasLSB);
		CPPUNIT_ASSERT(list[0].isMSBOmittingRequired);

		// NRPN, DATA MSB, DATA LSBと, MSB省略フラグを指定したappend
		target = NrpnEventStub(1920, MidiParameterType::CVM_NM_DELAY, 0x01, 0x23);
		target.append(MidiParameterType::CVM_NM_VELOCITY, 0x01, 0x02, true);
		list = target.getList();
		CPPUNIT_ASSERT_EQUAL((size_t)1, list.size());
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, list[0].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_VELOCITY, list[0].nrpn);
		CPPUNIT_ASSERT_EQUAL(0x01, list[0].dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x02, list[0].dataLSB);
		CPPUNIT_ASSERT(list[0].hasLSB);
		CPPUNIT_ASSERT(list[0].isMSBOmittingRequired);
	}

	void testExpand()
	{

		// DATA LSB を保持している単一 NrpnEvent アイテムの expand
		NrpnEventStub target = NrpnEventStub(1920, MidiParameterType::CVM_NM_DELAY, 0x01, 0x23);
		vector<NrpnEvent> list = target.expand();
		CPPUNIT_ASSERT_EQUAL((size_t)1, list.size());
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, list[0].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_DELAY, list[0].nrpn);
		CPPUNIT_ASSERT_EQUAL(0x01, list[0].dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x23, list[0].dataLSB);
		CPPUNIT_ASSERT(list[0].hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, list[0].isMSBOmittingRequired);

		// DATA LSB を保持していない単一 NrpnEvent アイテムの expand
		target = NrpnEventStub(1920, MidiParameterType::CVM_NM_DELAY, 0x01);
		list = target.expand();
		CPPUNIT_ASSERT_EQUAL((size_t)1, list.size());
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, list[0].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_DELAY, list[0].nrpn);
		CPPUNIT_ASSERT_EQUAL(0x01, list[0].dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x00, list[0].dataLSB);
		CPPUNIT_ASSERT_EQUAL(false, list[0].hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, list[0].isMSBOmittingRequired);

		// 子・孫アイテムを持つ NrpnEvent の expand
		target = NrpnEventStub(1920, MidiParameterType::CVM_NM_DELAY, 0x01);
		target.append(MidiParameterType::CVM_NM_VELOCITY, 64);   // 子アイテムを追加
		vector<NrpnEvent> childEventList = target.getList();
		childEventList[0].append(MidiParameterType::CVM_NM_NOTE_NUMBER, 60);   // 孫アイテムを追加
		target.setList(childEventList);
		list = target.expand();
		CPPUNIT_ASSERT_EQUAL((size_t)3, list.size());

		CPPUNIT_ASSERT_EQUAL((tick_t)1920, list[0].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_DELAY, list[0].nrpn);
		CPPUNIT_ASSERT_EQUAL(0x01, list[0].dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x00, list[0].dataLSB);
		CPPUNIT_ASSERT_EQUAL(false, list[0].hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, list[0].isMSBOmittingRequired);

		CPPUNIT_ASSERT_EQUAL((tick_t)1920, list[1].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_VELOCITY, list[1].nrpn);
		CPPUNIT_ASSERT_EQUAL(64, list[1].dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x00, list[1].dataLSB);
		CPPUNIT_ASSERT_EQUAL(false, list[1].hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, list[1].isMSBOmittingRequired);

		CPPUNIT_ASSERT_EQUAL((tick_t)1920, list[2].tick);
		CPPUNIT_ASSERT_EQUAL(MidiParameterType::CVM_NM_NOTE_NUMBER, list[2].nrpn);
		CPPUNIT_ASSERT_EQUAL(60, list[2].dataMSB);
		CPPUNIT_ASSERT_EQUAL(0x00, list[2].dataLSB);
		CPPUNIT_ASSERT_EQUAL(false, list[2].hasLSB);
		CPPUNIT_ASSERT_EQUAL(false, list[2].isMSBOmittingRequired);
	}

	void testCompare()
	{
		NrpnEvent a(480, MidiParameterType::CVM_NM_VELOCITY, 64);
		NrpnEvent b(1920, MidiParameterType::CVM_NM_DELAY, 0x01, 0x23);
		CPPUNIT_ASSERT(NrpnEvent::compare(a, b));
		CPPUNIT_ASSERT_EQUAL(false, NrpnEvent::compare(b, a));
		CPPUNIT_ASSERT_EQUAL(false, NrpnEvent::compare(a, a));
	}

	void testConvert()
	{
		NrpnEvent target = NrpnEvent(1920, MidiParameterType::CVM_NM_DELAY, 0x01, 0x23);
		target.append(MidiParameterType::CVM_NM_VELOCITY, 0x01, 0x02, true);
		target.append(MidiParameterType::CVM_NM_NOTE_NUMBER, 60);

		vector<MidiEvent> events = NrpnEvent::convert(target.expand());
		CPPUNIT_ASSERT_EQUAL((size_t)10, events.size());
		MidiEvent e;

		// CVM_NM_DELAY
		e = events[0];
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, e.tick);
		CPPUNIT_ASSERT_EQUAL(0xb0, e.firstByte);
		CPPUNIT_ASSERT_EQUAL((size_t)2, e.data.size());
		CPPUNIT_ASSERT_EQUAL(0x63, e.data[0]);
		CPPUNIT_ASSERT_EQUAL(((static_cast<int>(MidiParameterType::CVM_NM_DELAY) & 0xff00) >> 8), e.data[1]);
		e = events[1];
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, e.tick);
		CPPUNIT_ASSERT_EQUAL(0xb0, e.firstByte);
		CPPUNIT_ASSERT_EQUAL((size_t)2, e.data.size());
		CPPUNIT_ASSERT_EQUAL(0x62, e.data[0]);
		CPPUNIT_ASSERT_EQUAL((static_cast<int>(MidiParameterType::CVM_NM_DELAY) & 0xff), e.data[1]);
		e = events[2];
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, e.tick);
		CPPUNIT_ASSERT_EQUAL(0xb0, e.firstByte);
		CPPUNIT_ASSERT_EQUAL((size_t)2, e.data.size());
		CPPUNIT_ASSERT_EQUAL(0x06, e.data[0]);
		CPPUNIT_ASSERT_EQUAL(0x01, e.data[1]);
		e = events[3];
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, e.tick);
		CPPUNIT_ASSERT_EQUAL(0xb0, e.firstByte);
		CPPUNIT_ASSERT_EQUAL((size_t)2, e.data.size());
		CPPUNIT_ASSERT_EQUAL(0x26, e.data[0]);
		CPPUNIT_ASSERT_EQUAL(0x23, e.data[1]);

		// CVM_NM_VELOCITY
		e = events[4];
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, e.tick);
		CPPUNIT_ASSERT_EQUAL(0xb0, e.firstByte);
		CPPUNIT_ASSERT_EQUAL((size_t)2, e.data.size());
		CPPUNIT_ASSERT_EQUAL(0x62, e.data[0]);
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(MidiParameterType::CVM_NM_VELOCITY) & 0xff, e.data[1]);
		e = events[5];
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, e.tick);
		CPPUNIT_ASSERT_EQUAL(0xb0, e.firstByte);
		CPPUNIT_ASSERT_EQUAL((size_t)2, e.data.size());
		CPPUNIT_ASSERT_EQUAL(0x06, e.data[0]);
		CPPUNIT_ASSERT_EQUAL(0x01, e.data[1]);
		e = events[6];
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, e.tick);
		CPPUNIT_ASSERT_EQUAL(0xb0, e.firstByte);
		CPPUNIT_ASSERT_EQUAL((size_t)2, e.data.size());
		CPPUNIT_ASSERT_EQUAL(0x26, e.data[0]);
		CPPUNIT_ASSERT_EQUAL(0x02, e.data[1]);

		// CVM_NM_NOTE_NUMBER
		e = events[7];
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, e.tick);
		CPPUNIT_ASSERT_EQUAL(0xb0, e.firstByte);
		CPPUNIT_ASSERT_EQUAL((size_t)2, e.data.size());
		CPPUNIT_ASSERT_EQUAL(0x63, e.data[0]);
		CPPUNIT_ASSERT_EQUAL(((static_cast<int>(MidiParameterType::CVM_NM_NOTE_NUMBER) & 0xff00) >> 8), e.data[1]);
		e = events[8];
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, e.tick);
		CPPUNIT_ASSERT_EQUAL(0xb0, e.firstByte);
		CPPUNIT_ASSERT_EQUAL((size_t)2, e.data.size());
		CPPUNIT_ASSERT_EQUAL(0x62, e.data[0]);
		CPPUNIT_ASSERT_EQUAL(static_cast<int>(MidiParameterType::CVM_NM_NOTE_NUMBER) & 0xff, e.data[1]);
		e = events[9];
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, e.tick);
		CPPUNIT_ASSERT_EQUAL(0xb0, e.firstByte);
		CPPUNIT_ASSERT_EQUAL((size_t)2, e.data.size());
		CPPUNIT_ASSERT_EQUAL(0x06, e.data[0]);
		CPPUNIT_ASSERT_EQUAL(60, e.data[1]);
	}

	CPPUNIT_TEST_SUITE(NrpnEventTest);
	CPPUNIT_TEST(testConstructWithoutLSB);
	CPPUNIT_TEST(testConstructWithLSB);
	CPPUNIT_TEST(testCompareTo);
	CPPUNIT_TEST(testAppend);
	CPPUNIT_TEST(testExpand);
	CPPUNIT_TEST(testCompare);
	CPPUNIT_TEST(testConvert);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(NrpnEventTest);
