#include "Util.hpp"
#include "../include/libvsq/VoiceLanguage.hpp"

using namespace std;
using namespace vsq;

TEST(VoiceLanguageEnumTest, test)
{
	EXPECT_EQ(0, (int)VoiceLanguage::JAPANESE);
	EXPECT_EQ(1, (int)VoiceLanguage::ENGLISH);

	EXPECT_EQ(VoiceLanguage::ENGLISH, VoiceLanguageUtil::valueFromSingerName("sweet_ann"));
	EXPECT_EQ(VoiceLanguage::ENGLISH, VoiceLanguageUtil::valueFromSingerName("prima"));
	EXPECT_EQ(VoiceLanguage::ENGLISH, VoiceLanguageUtil::valueFromSingerName("luka_eng"));
	EXPECT_EQ(VoiceLanguage::ENGLISH, VoiceLanguageUtil::valueFromSingerName("sonika"));
	EXPECT_EQ(VoiceLanguage::ENGLISH, VoiceLanguageUtil::valueFromSingerName("lola"));
	EXPECT_EQ(VoiceLanguage::ENGLISH, VoiceLanguageUtil::valueFromSingerName("leon"));
	EXPECT_EQ(VoiceLanguage::ENGLISH, VoiceLanguageUtil::valueFromSingerName("miriam"));
	EXPECT_EQ(VoiceLanguage::ENGLISH, VoiceLanguageUtil::valueFromSingerName("big_al"));
	EXPECT_EQ(VoiceLanguage::JAPANESE, VoiceLanguageUtil::valueFromSingerName("___FOO___"));
}
