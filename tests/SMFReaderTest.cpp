#include "Util.hpp"
#include "../include/libvsq/SMFReader.hpp"
#include "../include/libvsq/FileInputStream.hpp"

using namespace std;
using namespace vsq;

TEST(SMFReaderTest, testRead)
{
	FileInputStream stream("VSQFileWriterTest/expected/expected.vsq");
	SMFReader reader;
	vector<vector<MidiEvent> > actual;
	int format, timeFormat;
	reader.read(stream, actual, format, timeFormat);
	stream.close();

	EXPECT_EQ((size_t)2, actual.size());

	// 1トラック(Masterトラック)
	EXPECT_EQ((size_t)4, actual[0].size());

	// 2トラック(メタイベント8個+NRPNイベント90個+メタイベント[Track End]1個)
	EXPECT_EQ((size_t)99, actual[1].size());

	EXPECT_EQ(1, format);
	EXPECT_EQ(480, timeFormat);
}
