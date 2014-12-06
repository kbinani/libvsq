#include "Util.hpp"
#include "../include/libvsq/BPListSearchResult.hpp"

using namespace std;
using namespace vsq;

TEST(BPListSearchResultTest, construct)
{
	BPListSearchResult result;
	EXPECT_EQ((tick_t)0, result.tick);
	EXPECT_EQ(0, result.index);
}
