#include "Util.hpp"
#include "../include/libvsq/CP932Converter.hpp"

using namespace std;
using namespace vsq;

class CP932ConverterTest : public CppUnit::TestCase
{
public:
	void testConvertFromUTF8()
	{
		ostringstream fixture;
		// "aあ\t\n"
		fixture << (char)0x61 << (char)0xE3 << (char)0x81 << (char)0x82 << (char)0x09 << (char)0x0A;
		string actual = CP932Converter::convertFromUTF8(fixture.str());
		ostringstream expected;
		expected << (char)0x61 << (char)0x82 << (char)0xA0 << "\t\n";
		CPPUNIT_ASSERT_EQUAL(expected.str(), actual);
	}

	void testConvertToUTF8()
	{
		{
			ostringstream fixture;
			fixture << (char)0x61 << (char)0x82 << (char)0xA0 << "\t\n";
			string actual = CP932Converter::convertToUTF8(fixture.str());
			ostringstream expected;
			expected << (char)0x61 << (char)0xE3 << (char)0x81 << (char)0x82 << (char)0x09 << (char)0x0A;
			CPPUNIT_ASSERT_EQUAL(expected.str(), actual);
		}

		{
			ostringstream fixture;
			fixture << (char)0x5b << (char)0x4d << (char)0x69 << (char)0x78 << (char)0x65 << (char)0x72 << (char)0x5d;
			string actual = CP932Converter::convertToUTF8(fixture.str());
			CPPUNIT_ASSERT_EQUAL(fixture.str(), actual);
		}
	}

	void test_getUnicodeBytesFromUTF8Bytes()
	{
		vector<int> a;

		// 1 byte
		vector<int> fixture;
		fixture.push_back(0x00);
		a = CP932Converter::_getUnicodeBytesFromUTF8Bytes(fixture);
		CPPUNIT_ASSERT_EQUAL((size_t)1, a.size());
		CPPUNIT_ASSERT_EQUAL(0x00, a[0]);
		fixture.clear();
		fixture.push_back(0x7F);
		a = CP932Converter::_getUnicodeBytesFromUTF8Bytes(fixture);
		CPPUNIT_ASSERT_EQUAL((size_t)1, a.size());
		CPPUNIT_ASSERT_EQUAL(0x7F, a[0]);

		// 2 bytes
		fixture.clear();
		fixture.push_back(0xC2);
		fixture.push_back(0x80);
		a = CP932Converter::_getUnicodeBytesFromUTF8Bytes(fixture);
		CPPUNIT_ASSERT_EQUAL((size_t)1, a.size());
		CPPUNIT_ASSERT_EQUAL(0x80, a[0]);

		fixture.clear();
		fixture.push_back(0xDF);
		fixture.push_back(0xBF);
		a = CP932Converter::_getUnicodeBytesFromUTF8Bytes(fixture);
		CPPUNIT_ASSERT_EQUAL((size_t)2, a.size());
		CPPUNIT_ASSERT_EQUAL(0x07, a[0]);
		CPPUNIT_ASSERT_EQUAL(0xFF, a[1]);

		// 3 bytes
		fixture.clear();
		fixture.push_back(0xE0);
		fixture.push_back(0xA0);
		fixture.push_back(0x80);
		a = CP932Converter::_getUnicodeBytesFromUTF8Bytes(fixture);
		CPPUNIT_ASSERT_EQUAL((size_t)2, a.size());
		CPPUNIT_ASSERT_EQUAL(0x08, a[0]);
		CPPUNIT_ASSERT_EQUAL(0x00, a[1]);

		fixture.clear();
		fixture.push_back(0xEF);
		fixture.push_back(0xBF);
		fixture.push_back(0xBF);
		a = CP932Converter::_getUnicodeBytesFromUTF8Bytes(fixture);
		CPPUNIT_ASSERT_EQUAL((size_t)2, a.size());
		CPPUNIT_ASSERT_EQUAL(0xFF, a[0]);
		CPPUNIT_ASSERT_EQUAL(0xFF, a[1]);

		// 4 bytes
		fixture.clear();
		fixture.push_back(0xF0);
		fixture.push_back(0x90);
		fixture.push_back(0x80);
		fixture.push_back(0x80);
		a = CP932Converter::_getUnicodeBytesFromUTF8Bytes(fixture);
		CPPUNIT_ASSERT_EQUAL((size_t)3, a.size());
		CPPUNIT_ASSERT_EQUAL(0x01, a[0]);
		CPPUNIT_ASSERT_EQUAL(0x00, a[1]);
		CPPUNIT_ASSERT_EQUAL(0x00, a[2]);

		fixture.clear();
		fixture.push_back(0xF7);
		fixture.push_back(0xBF);
		fixture.push_back(0xBF);
		fixture.push_back(0xBF);
		a = CP932Converter::_getUnicodeBytesFromUTF8Bytes(fixture);
		CPPUNIT_ASSERT_EQUAL((size_t)3, a.size());
		CPPUNIT_ASSERT_EQUAL(0x1F, a[0]);
		CPPUNIT_ASSERT_EQUAL(0xFF, a[1]);
		CPPUNIT_ASSERT_EQUAL(0xFF, a[2]);

		// 5 bytes
		fixture.clear();
		fixture.push_back(0xF8);
		fixture.push_back(0x88);
		fixture.push_back(0x80);
		fixture.push_back(0x80);
		fixture.push_back(0x80);
		a = CP932Converter::_getUnicodeBytesFromUTF8Bytes(fixture);
		CPPUNIT_ASSERT_EQUAL((size_t)3, a.size());
		CPPUNIT_ASSERT_EQUAL(0x20, a[0]);
		CPPUNIT_ASSERT_EQUAL(0x00, a[1]);
		CPPUNIT_ASSERT_EQUAL(0x00, a[2]);

		fixture.clear();
		fixture.push_back(0xFB);
		fixture.push_back(0xBF);
		fixture.push_back(0xBF);
		fixture.push_back(0xBF);
		fixture.push_back(0xBF);
		a = CP932Converter::_getUnicodeBytesFromUTF8Bytes(fixture);
		CPPUNIT_ASSERT_EQUAL((size_t)4, a.size());
		CPPUNIT_ASSERT_EQUAL(0x03, a[0]);
		CPPUNIT_ASSERT_EQUAL(0xFF, a[1]);
		CPPUNIT_ASSERT_EQUAL(0xFF, a[2]);
		CPPUNIT_ASSERT_EQUAL(0xFF, a[3]);

		// 6 bytes
		fixture.clear();
		fixture.push_back(0xFC);
		fixture.push_back(0x84);
		fixture.push_back(0x80);
		fixture.push_back(0x80);
		fixture.push_back(0x80);
		fixture.push_back(0x80);
		a = CP932Converter::_getUnicodeBytesFromUTF8Bytes(fixture);
		CPPUNIT_ASSERT_EQUAL((size_t)4, a.size());
		CPPUNIT_ASSERT_EQUAL(0x04, a[0]);
		CPPUNIT_ASSERT_EQUAL(0x00, a[1]);
		CPPUNIT_ASSERT_EQUAL(0x00, a[2]);
		CPPUNIT_ASSERT_EQUAL(0x00, a[3]);
		fixture.clear();
		fixture.push_back(0xFD);
		fixture.push_back(0xBF);
		fixture.push_back(0xBF);
		fixture.push_back(0xBF);
		fixture.push_back(0xBF);
		fixture.push_back(0xBF);
		a = CP932Converter::_getUnicodeBytesFromUTF8Bytes(fixture);
		CPPUNIT_ASSERT_EQUAL((size_t)4, a.size());
		CPPUNIT_ASSERT_EQUAL(0x7F, a[0]);
		CPPUNIT_ASSERT_EQUAL(0xFF, a[1]);
		CPPUNIT_ASSERT_EQUAL(0xFF, a[2]);
		CPPUNIT_ASSERT_EQUAL(0xFF, a[3]);
	}

	CPPUNIT_TEST_SUITE(CP932ConverterTest);
	CPPUNIT_TEST(testConvertFromUTF8);
	CPPUNIT_TEST(testConvertToUTF8);
	CPPUNIT_TEST(test_getUnicodeBytesFromUTF8Bytes);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(CP932ConverterTest);
