#include "Util.hpp"
#include "../include/libvsq/MeasureLineIterator.hpp"
#include "../include/libvsq/TimesigList.hpp"

using namespace vsq;

TEST(MeasureLineIteratorTest, test)
{
	TimesigList list;
	list.push(Timesig(4, 4, 0));
	list.push(Timesig(3, 4, 1));
	MeasureLineIterator i(&list);
	i.reset(3360);

	EXPECT_TRUE(i.hasNext());
	MeasureLine actual = i.next();
	EXPECT_EQ((tick_t)0, actual.tick);
	EXPECT_EQ(true, actual.isBorder);
	EXPECT_EQ(0, actual.barCount);
	EXPECT_EQ(4, actual.numerator);
	EXPECT_EQ(4, actual.denominator);
	EXPECT_EQ(false, actual.isAssistLine);

	EXPECT_TRUE(i.hasNext());
	actual = i.next();
	EXPECT_EQ((tick_t)480, actual.tick);
	EXPECT_EQ(false, actual.isBorder);
	EXPECT_EQ(0, actual.barCount);
	EXPECT_EQ(4, actual.numerator);
	EXPECT_EQ(4, actual.denominator);
	EXPECT_EQ(false, actual.isAssistLine);

	EXPECT_TRUE(i.hasNext());
	actual = i.next();
	EXPECT_EQ((tick_t)960, actual.tick);
	EXPECT_EQ(false, actual.isBorder);
	EXPECT_EQ(0, actual.barCount);
	EXPECT_EQ(4, actual.numerator);
	EXPECT_EQ(4, actual.denominator);
	EXPECT_EQ(false, actual.isAssistLine);

	EXPECT_TRUE(i.hasNext());
	actual = i.next();
	EXPECT_EQ((tick_t)1440, actual.tick);
	EXPECT_EQ(false, actual.isBorder);
	EXPECT_EQ(0, actual.barCount);
	EXPECT_EQ(4, actual.numerator);
	EXPECT_EQ(4, actual.denominator);
	EXPECT_EQ(false, actual.isAssistLine);

	EXPECT_TRUE(i.hasNext());
	actual = i.next();
	EXPECT_EQ((tick_t)1920, actual.tick);
	EXPECT_EQ(true, actual.isBorder);
	EXPECT_EQ(1, actual.barCount);
	EXPECT_EQ(3, actual.numerator);
	EXPECT_EQ(4, actual.denominator);
	EXPECT_EQ(false, actual.isAssistLine);

	EXPECT_TRUE(i.hasNext());
	actual = i.next();
	EXPECT_EQ((tick_t)2400, actual.tick);
	EXPECT_EQ(false, actual.isBorder);
	EXPECT_EQ(1, actual.barCount);
	EXPECT_EQ(3, actual.numerator);
	EXPECT_EQ(4, actual.denominator);
	EXPECT_EQ(false, actual.isAssistLine);

	EXPECT_TRUE(i.hasNext());
	actual = i.next();
	EXPECT_EQ((tick_t)2880, actual.tick);
	EXPECT_EQ(false, actual.isBorder);
	EXPECT_EQ(1, actual.barCount);
	EXPECT_EQ(3, actual.numerator);
	EXPECT_EQ(4, actual.denominator);
	EXPECT_EQ(false, actual.isAssistLine);

	EXPECT_TRUE(i.hasNext());
	actual = i.next();
	EXPECT_EQ((tick_t)3360, actual.tick);
	EXPECT_EQ(true, actual.isBorder);
	EXPECT_EQ(2, actual.barCount);
	EXPECT_EQ(3, actual.numerator);
	EXPECT_EQ(4, actual.denominator);
	EXPECT_EQ(false, actual.isAssistLine);

	EXPECT_EQ(false, i.hasNext());
}

void testWithoutAnyBar()
{
	TimesigList list;
	list.push(Timesig(4, 4, 0));
	list.push(Timesig(3, 4, 1));
	MeasureLineIterator i(&list);
	i.reset(479);

	EXPECT_EQ(true, i.hasNext());
	MeasureLine actual = i.next();
	EXPECT_EQ((tick_t)0, actual.tick);
	EXPECT_EQ(true, actual.isBorder);
	EXPECT_EQ(false, actual.isAssistLine);

	EXPECT_EQ(false, i.hasNext());
}

TEST(MeasureLineIteratorTest, testWithAssistLineStep)
{
	TimesigList list;
	list.push(Timesig(1, 4, 0));
	MeasureLineIterator i(&list, 120);
	i.reset(480);

	EXPECT_EQ(true, i.hasNext());
	{
		MeasureLine actual = i.next();
		EXPECT_EQ((tick_t)0, actual.tick);
		EXPECT_EQ(0, actual.barCount);
		EXPECT_EQ(true, actual.isBorder);
		EXPECT_EQ(1, actual.numerator);
		EXPECT_EQ(4, actual.denominator);
		EXPECT_EQ(false, actual.isAssistLine);
	}
	EXPECT_EQ(true, i.hasNext());
	{
		MeasureLine actual = i.next();
		EXPECT_EQ((tick_t)120, actual.tick);
		EXPECT_EQ(0, actual.barCount);
		EXPECT_EQ(false, actual.isBorder);
		EXPECT_EQ(1, actual.numerator);
		EXPECT_EQ(4, actual.denominator);
		EXPECT_EQ(true, actual.isAssistLine);
	}
	EXPECT_EQ(true, i.hasNext());
	{
		MeasureLine actual = i.next();
		EXPECT_EQ((tick_t)240, actual.tick);
		EXPECT_EQ(0, actual.barCount);
		EXPECT_EQ(false, actual.isBorder);
		EXPECT_EQ(1, actual.numerator);
		EXPECT_EQ(4, actual.denominator);
		EXPECT_EQ(true, actual.isAssistLine);
	}
	EXPECT_EQ(true, i.hasNext());
	{
		MeasureLine actual = i.next();
		EXPECT_EQ((tick_t)360, actual.tick);
		EXPECT_EQ(0, actual.barCount);
		EXPECT_EQ(false, actual.isBorder);
		EXPECT_EQ(1, actual.numerator);
		EXPECT_EQ(4, actual.denominator);
		EXPECT_EQ(true, actual.isAssistLine);
	}
	EXPECT_EQ(true, i.hasNext());
	{
		MeasureLine actual = i.next();
		EXPECT_EQ((tick_t)480, actual.tick);
		EXPECT_EQ(1, actual.barCount);
		EXPECT_EQ(true, actual.isBorder);
		EXPECT_EQ(1, actual.numerator);
		EXPECT_EQ(4, actual.denominator);
		EXPECT_EQ(false, actual.isAssistLine);
	}
	EXPECT_EQ(false, i.hasNext());
}

TEST(MeasureLineIteratorTest, testWithInvalidAssistLineStep)
{
	TimesigList list;
	EXPECT_THROW(
		MeasureLineIterator(&list, 10),
		MeasureLineIterator::InvalidAssistLineStep
	);
}
