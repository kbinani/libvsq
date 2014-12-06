#include "Util.hpp"
#include "../include/libvsq/MeasureLineIterator.hpp"
#include "../include/libvsq/TimesigList.hpp"

using namespace vsq;

class MeasureLineIteratorTest : public CppUnit::TestCase
{
public:
	void test()
	{
		TimesigList list;
		list.push(Timesig(4, 4, 0));
		list.push(Timesig(3, 4, 1));
		MeasureLineIterator i(&list);
		i.reset(3360);

		CPPUNIT_ASSERT(i.hasNext());
		MeasureLine actual = i.next();
		CPPUNIT_ASSERT_EQUAL((tick_t)0, actual.tick);
		CPPUNIT_ASSERT_EQUAL(true, actual.isBorder);
		CPPUNIT_ASSERT_EQUAL(0, actual.barCount);
		CPPUNIT_ASSERT_EQUAL(4, actual.numerator);
		CPPUNIT_ASSERT_EQUAL(4, actual.denominator);
		CPPUNIT_ASSERT_EQUAL(false, actual.isAssistLine);

		CPPUNIT_ASSERT(i.hasNext());
		actual = i.next();
		CPPUNIT_ASSERT_EQUAL((tick_t)480, actual.tick);
		CPPUNIT_ASSERT_EQUAL(false, actual.isBorder);
		CPPUNIT_ASSERT_EQUAL(0, actual.barCount);
		CPPUNIT_ASSERT_EQUAL(4, actual.numerator);
		CPPUNIT_ASSERT_EQUAL(4, actual.denominator);
		CPPUNIT_ASSERT_EQUAL(false, actual.isAssistLine);

		CPPUNIT_ASSERT(i.hasNext());
		actual = i.next();
		CPPUNIT_ASSERT_EQUAL((tick_t)960, actual.tick);
		CPPUNIT_ASSERT_EQUAL(false, actual.isBorder);
		CPPUNIT_ASSERT_EQUAL(0, actual.barCount);
		CPPUNIT_ASSERT_EQUAL(4, actual.numerator);
		CPPUNIT_ASSERT_EQUAL(4, actual.denominator);
		CPPUNIT_ASSERT_EQUAL(false, actual.isAssistLine);

		CPPUNIT_ASSERT(i.hasNext());
		actual = i.next();
		CPPUNIT_ASSERT_EQUAL((tick_t)1440, actual.tick);
		CPPUNIT_ASSERT_EQUAL(false, actual.isBorder);
		CPPUNIT_ASSERT_EQUAL(0, actual.barCount);
		CPPUNIT_ASSERT_EQUAL(4, actual.numerator);
		CPPUNIT_ASSERT_EQUAL(4, actual.denominator);
		CPPUNIT_ASSERT_EQUAL(false, actual.isAssistLine);

		CPPUNIT_ASSERT(i.hasNext());
		actual = i.next();
		CPPUNIT_ASSERT_EQUAL((tick_t)1920, actual.tick);
		CPPUNIT_ASSERT_EQUAL(true, actual.isBorder);
		CPPUNIT_ASSERT_EQUAL(1, actual.barCount);
		CPPUNIT_ASSERT_EQUAL(3, actual.numerator);
		CPPUNIT_ASSERT_EQUAL(4, actual.denominator);
		CPPUNIT_ASSERT_EQUAL(false, actual.isAssistLine);

		CPPUNIT_ASSERT(i.hasNext());
		actual = i.next();
		CPPUNIT_ASSERT_EQUAL((tick_t)2400, actual.tick);
		CPPUNIT_ASSERT_EQUAL(false, actual.isBorder);
		CPPUNIT_ASSERT_EQUAL(1, actual.barCount);
		CPPUNIT_ASSERT_EQUAL(3, actual.numerator);
		CPPUNIT_ASSERT_EQUAL(4, actual.denominator);
		CPPUNIT_ASSERT_EQUAL(false, actual.isAssistLine);

		CPPUNIT_ASSERT(i.hasNext());
		actual = i.next();
		CPPUNIT_ASSERT_EQUAL((tick_t)2880, actual.tick);
		CPPUNIT_ASSERT_EQUAL(false, actual.isBorder);
		CPPUNIT_ASSERT_EQUAL(1, actual.barCount);
		CPPUNIT_ASSERT_EQUAL(3, actual.numerator);
		CPPUNIT_ASSERT_EQUAL(4, actual.denominator);
		CPPUNIT_ASSERT_EQUAL(false, actual.isAssistLine);

		CPPUNIT_ASSERT(i.hasNext());
		actual = i.next();
		CPPUNIT_ASSERT_EQUAL((tick_t)3360, actual.tick);
		CPPUNIT_ASSERT_EQUAL(true, actual.isBorder);
		CPPUNIT_ASSERT_EQUAL(2, actual.barCount);
		CPPUNIT_ASSERT_EQUAL(3, actual.numerator);
		CPPUNIT_ASSERT_EQUAL(4, actual.denominator);
		CPPUNIT_ASSERT_EQUAL(false, actual.isAssistLine);

		CPPUNIT_ASSERT_EQUAL(false, i.hasNext());
	}

	void testWithoutAnyBar()
	{
		TimesigList list;
		list.push(Timesig(4, 4, 0));
		list.push(Timesig(3, 4, 1));
		MeasureLineIterator i(&list);
		i.reset(479);

		CPPUNIT_ASSERT_EQUAL(true, i.hasNext());
		MeasureLine actual = i.next();
		CPPUNIT_ASSERT_EQUAL((tick_t)0, actual.tick);
		CPPUNIT_ASSERT_EQUAL(true, actual.isBorder);
		CPPUNIT_ASSERT_EQUAL(false, actual.isAssistLine);

		CPPUNIT_ASSERT_EQUAL(false, i.hasNext());
	}

	void testWithAssistLineStep()
	{
		TimesigList list;
		list.push(Timesig(1, 4, 0));
		MeasureLineIterator i(&list, 120);
		i.reset(480);

		CPPUNIT_ASSERT_EQUAL(true, i.hasNext());
		{
			MeasureLine actual = i.next();
			CPPUNIT_ASSERT_EQUAL((tick_t)0, actual.tick);
			CPPUNIT_ASSERT_EQUAL(0, actual.barCount);
			CPPUNIT_ASSERT_EQUAL(true, actual.isBorder);
			CPPUNIT_ASSERT_EQUAL(1, actual.numerator);
			CPPUNIT_ASSERT_EQUAL(4, actual.denominator);
			CPPUNIT_ASSERT_EQUAL(false, actual.isAssistLine);
		}
		CPPUNIT_ASSERT_EQUAL(true, i.hasNext());
		{
			MeasureLine actual = i.next();
			CPPUNIT_ASSERT_EQUAL((tick_t)120, actual.tick);
			CPPUNIT_ASSERT_EQUAL(0, actual.barCount);
			CPPUNIT_ASSERT_EQUAL(false, actual.isBorder);
			CPPUNIT_ASSERT_EQUAL(1, actual.numerator);
			CPPUNIT_ASSERT_EQUAL(4, actual.denominator);
			CPPUNIT_ASSERT_EQUAL(true, actual.isAssistLine);
		}
		CPPUNIT_ASSERT_EQUAL(true, i.hasNext());
		{
			MeasureLine actual = i.next();
			CPPUNIT_ASSERT_EQUAL((tick_t)240, actual.tick);
			CPPUNIT_ASSERT_EQUAL(0, actual.barCount);
			CPPUNIT_ASSERT_EQUAL(false, actual.isBorder);
			CPPUNIT_ASSERT_EQUAL(1, actual.numerator);
			CPPUNIT_ASSERT_EQUAL(4, actual.denominator);
			CPPUNIT_ASSERT_EQUAL(true, actual.isAssistLine);
		}
		CPPUNIT_ASSERT_EQUAL(true, i.hasNext());
		{
			MeasureLine actual = i.next();
			CPPUNIT_ASSERT_EQUAL((tick_t)360, actual.tick);
			CPPUNIT_ASSERT_EQUAL(0, actual.barCount);
			CPPUNIT_ASSERT_EQUAL(false, actual.isBorder);
			CPPUNIT_ASSERT_EQUAL(1, actual.numerator);
			CPPUNIT_ASSERT_EQUAL(4, actual.denominator);
			CPPUNIT_ASSERT_EQUAL(true, actual.isAssistLine);
		}
		CPPUNIT_ASSERT_EQUAL(true, i.hasNext());
		{
			MeasureLine actual = i.next();
			CPPUNIT_ASSERT_EQUAL((tick_t)480, actual.tick);
			CPPUNIT_ASSERT_EQUAL(1, actual.barCount);
			CPPUNIT_ASSERT_EQUAL(true, actual.isBorder);
			CPPUNIT_ASSERT_EQUAL(1, actual.numerator);
			CPPUNIT_ASSERT_EQUAL(4, actual.denominator);
			CPPUNIT_ASSERT_EQUAL(false, actual.isAssistLine);
		}
		CPPUNIT_ASSERT_EQUAL(false, i.hasNext());
	}

	void testWithInvalidAssistLineStep()
	{
		TimesigList list;
		CPPUNIT_ASSERT_THROW(
			MeasureLineIterator(&list, 10),
			MeasureLineIterator::InvalidAssistLineStep
		);
	}

	CPPUNIT_TEST_SUITE(MeasureLineIteratorTest);
	CPPUNIT_TEST(test);
	CPPUNIT_TEST(testWithoutAnyBar);
	CPPUNIT_TEST(testWithAssistLineStep);
	CPPUNIT_TEST(testWithInvalidAssistLineStep);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(MeasureLineIteratorTest);
