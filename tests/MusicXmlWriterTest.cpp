#include "Util.hpp"
#include "../include/libvsq/MusicXmlWriter.hpp"
#include "../include/libvsq/ByteArrayOutputStream.hpp"
#include "../include/libvsq/TextStream.hpp"
#include "../include/libvsq/Sequence.hpp"

using namespace vsq;

class MusicXmlWriterTest : public CppUnit::TestCase
{
public:
	void writeWithoutTempoChange()
	{
		MusicXmlWriter writer;
		TextStream stream;

		Sequence sequence("foo", 1, 3, 4, 500000);
		sequence.tempoList.clear();
		sequence.tempoList.push(Tempo(0, 500000));
		sequence.tempoList.push(Tempo(1920, 250000));
		sequence.timesigList.clear();
		sequence.timesigList.push(Timesig(4, 4, 0));
		sequence.timesigList.push(Timesig(3, 4, 2));
		Track& track = sequence.track(0);
		Event note(1920, EventType::NOTE);
		note.note = 60;
		note.length(2865);
		Lyric lyric("あ", "a");
		note.lyricHandle.set(0, lyric);
		track.events().add(note);

		writer.write(sequence, stream, "foo");

		std::string actualString = stream.toString();
		stream.close();

		// 期待値と比較する
		std::vector<char> expected;
		std::string fixturePath = TestUtil::getFixtureRootPath() + "/MusicXmlWriterTest/expected/expected.xml";
		FILE* fileHandle = fopen(fixturePath.c_str(), "rb");
		if (! fileHandle) {
			CPPUNIT_FAIL("fixture を読み込めなかった");
		}
		while (1) {
			int c = fgetc(fileHandle);
			if (c == EOF) { break; }
			expected.push_back((char)c);
		}
		fclose(fileHandle);

		std::vector<char> actual;
		actual.insert(actual.begin(), actualString.begin(), actualString.end());
		CPPUNIT_ASSERT(expected == actual);
	}

	/**
	 * @todo
	 */
	void writeWithTempoChange()
	{
	}

	CPPUNIT_TEST_SUITE(MusicXmlWriterTest);
	CPPUNIT_TEST(writeWithoutTempoChange);
	CPPUNIT_TEST(writeWithTempoChange);
	CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE(MusicXmlWriterTest);
