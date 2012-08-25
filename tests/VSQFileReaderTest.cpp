#include "Util.hpp"
#include "../VSQFileReader.hpp"
#include "../FileInputStream.hpp"

using namespace std;
using namespace VSQ_NS;

class VSQFileReaderTest : public CppUnit::TestCase{
public:
    void testRead(){
        Sequence sequence( "singer", 1, 4, 4, 500000 );
        VSQFileReader reader;
        FileInputStream stream( TestUtil::getFixtureRootPath() + "/VSQFileReaderTest/fixture/fixture.vsq" );
        reader.read( sequence, &stream, "Shift_JIS" );
        stream.close();

        // シーケンス全体
        CPPUNIT_ASSERT_EQUAL( (size_t)2, sequence.track.size() );
        // [Master]
        CPPUNIT_ASSERT_EQUAL( 1, sequence.master.preMeasure );
        // [Mixer]
        CPPUNIT_ASSERT_EQUAL( 2, sequence.mixer.masterFeder );
        CPPUNIT_ASSERT_EQUAL( 3, sequence.mixer.masterPanpot );
        CPPUNIT_ASSERT_EQUAL( 1, sequence.mixer.masterMute );
        CPPUNIT_ASSERT_EQUAL( 4, sequence.mixer.outputMode );
        CPPUNIT_ASSERT_EQUAL( (size_t)1, sequence.mixer.slave.size() );
        CPPUNIT_ASSERT_EQUAL( 0, sequence.mixer.slave[0].feder );
        CPPUNIT_ASSERT_EQUAL( 0, sequence.mixer.slave[0].panpot );
        CPPUNIT_ASSERT_EQUAL( 0, sequence.mixer.slave[0].mute );
        CPPUNIT_ASSERT_EQUAL( 0, sequence.mixer.slave[0].solo );

        // トラック1
        Track *track1 = &sequence.track[1];
        // [Common]
        CPPUNIT_ASSERT_EQUAL( string( "DSB301" ), track1->getCommon()->version );
        CPPUNIT_ASSERT_EQUAL( string( "Voice1" ), track1->getCommon()->name );
        CPPUNIT_ASSERT_EQUAL( string( "179,181,123" ), track1->getCommon()->color );
        CPPUNIT_ASSERT_EQUAL( DynamicsMode::EXPERT, track1->getCommon()->dynamicsMode );
        CPPUNIT_ASSERT_EQUAL( PlayMode::PLAY_WITH_SYNTH, track1->getCommon()->playMode );
        {
            // イベントリスト
            CPPUNIT_ASSERT_EQUAL( 2, track1->getEvents()->size() );
            {
                // イベント/歌手変更
                Event item = track1->getEvents()->get( 0 );
                CPPUNIT_ASSERT_EQUAL( (tick_t)0, item.clock );
                CPPUNIT_ASSERT_EQUAL( EventType::SINGER, item.type );
                {
                    //歌手ハンドルの内容物をテスト
                    CPPUNIT_ASSERT_EQUAL( HandleType::SINGER, item.singerHandle.getHandleType() );
                    CPPUNIT_ASSERT_EQUAL( string( "$07010000" ), item.singerHandle.iconId );
                    CPPUNIT_ASSERT_EQUAL( string( "Foo" ), item.singerHandle.ids );
                    CPPUNIT_ASSERT_EQUAL( 0, item.singerHandle.original );
                    CPPUNIT_ASSERT_EQUAL( string( "" ), item.singerHandle.getCaption() );
                    CPPUNIT_ASSERT_EQUAL( (tick_t)1, item.singerHandle.getLength() );
                    CPPUNIT_ASSERT_EQUAL( 0, item.singerHandle.language );
                    CPPUNIT_ASSERT_EQUAL( 0, item.singerHandle.program );
                }
                {
                    //歌手ハンドル以外のハンドルがemptyとなっていること
                    CPPUNIT_ASSERT_EQUAL( HandleType::UNKNOWN, item.iconDynamicsHandle.getHandleType() );
                    CPPUNIT_ASSERT_EQUAL( HandleType::UNKNOWN, item.lyricHandle.getHandleType() );
                    CPPUNIT_ASSERT_EQUAL( HandleType::UNKNOWN, item.noteHeadHandle.getHandleType() );
                    CPPUNIT_ASSERT_EQUAL( HandleType::UNKNOWN, item.vibratoHandle.getHandleType() );
                }
            }
            {
                // イベント/音符
                Event item = track1->getEvents()->get( 1 );
                CPPUNIT_ASSERT_EQUAL( (tick_t)1920, item.clock );
                CPPUNIT_ASSERT_EQUAL( EventType::NOTE, item.type );
                CPPUNIT_ASSERT_EQUAL( (tick_t)480, item.getLength() );
                CPPUNIT_ASSERT_EQUAL( 60, item.note );
                CPPUNIT_ASSERT_EQUAL( 0, item.dynamics );
                CPPUNIT_ASSERT_EQUAL( 0, item.pmBendDepth );
                CPPUNIT_ASSERT_EQUAL( 0, item.pmBendLength );
                CPPUNIT_ASSERT_EQUAL( 0, item.pmbPortamentoUse );
                CPPUNIT_ASSERT_EQUAL( 0, item.demDecGainRate );
                CPPUNIT_ASSERT_EQUAL( 0, item.demAccent );
                {
                    // 歌詞ハンドルの内容物が正しいこと
                    CPPUNIT_ASSERT_EQUAL( HandleType::LYRIC, item.lyricHandle.getHandleType() );
                    CPPUNIT_ASSERT_EQUAL( 1, item.lyricHandle.getLyricCount() );
                    CPPUNIT_ASSERT_EQUAL( string( "a" ), item.lyricHandle.getLyricAt( 0 ).phrase );
                    CPPUNIT_ASSERT_EQUAL( string( "a" ), item.lyricHandle.getLyricAt( 0 ).getPhoneticSymbol() );
                    CPPUNIT_ASSERT_EQUAL( 1.0, item.lyricHandle.getLyricAt( 0 ).lengthRatio );
                    CPPUNIT_ASSERT_EQUAL( string( "0" ), item.lyricHandle.getLyricAt( 0 ).getConsonantAdjustment() );
                    CPPUNIT_ASSERT( false == item.lyricHandle.getLyricAt( 0 ).isProtected );
                }
                {
                    // 歌詞ハンドル以外のハンドルがemptyとなっていること
                    CPPUNIT_ASSERT_EQUAL( HandleType::UNKNOWN, item.iconDynamicsHandle.getHandleType() );
                    CPPUNIT_ASSERT_EQUAL( HandleType::UNKNOWN, item.singerHandle.getHandleType() );
                    CPPUNIT_ASSERT_EQUAL( HandleType::UNKNOWN, item.noteHeadHandle.getHandleType() );
                    CPPUNIT_ASSERT_EQUAL( HandleType::UNKNOWN, item.vibratoHandle.getHandleType() );
                }
            }
        }
        {
            // 各種コントロールカーブ
            BPList *list = NULL;
            // BRE
            list = track1->getCurve( "BRE" );
            CPPUNIT_ASSERT_EQUAL( 1, list->size() );
            CPPUNIT_ASSERT_EQUAL( (tick_t)1920, list->getKeyClock( 0 ) );
            CPPUNIT_ASSERT_EQUAL( 1, list->get( 0 ).value );
            // BRI
            list = track1->getCurve( "BRI" );
            CPPUNIT_ASSERT_EQUAL( 1, list->size() );
            CPPUNIT_ASSERT_EQUAL( (tick_t)1920, list->getKeyClock( 0 ) );
            CPPUNIT_ASSERT_EQUAL( 2, list->get( 0 ).value );
            // CLE
            list = track1->getCurve( "CLE" );
            CPPUNIT_ASSERT_EQUAL( 1, list->size() );
            CPPUNIT_ASSERT_EQUAL( (tick_t)1920, list->getKeyClock( 0 ) );
            CPPUNIT_ASSERT_EQUAL( 3, list->get( 0 ).value );
            // POR
            list = track1->getCurve( "POR" );
            CPPUNIT_ASSERT_EQUAL( 1, list->size() );
            CPPUNIT_ASSERT_EQUAL( (tick_t)1920, list->getKeyClock( 0 ) );
            CPPUNIT_ASSERT_EQUAL( 4, list->get( 0 ).value );
            // GEN
            list = track1->getCurve( "GEN" );
            CPPUNIT_ASSERT_EQUAL( 1, list->size() );
            CPPUNIT_ASSERT_EQUAL( (tick_t)1920, list->getKeyClock( 0 ) );
            CPPUNIT_ASSERT_EQUAL( 5, list->get( 0 ).value );
            // harmonics
            CPPUNIT_ASSERT_EQUAL( 0, track1->getCurve( "harmonics" )->size() );
            // OPE
            list = track1->getCurve( "OPE" );
            CPPUNIT_ASSERT_EQUAL( 1, list->size() );
            CPPUNIT_ASSERT_EQUAL( (tick_t)1920, list->getKeyClock( 0 ) );
            CPPUNIT_ASSERT_EQUAL( 7, list->get( 0 ).value );
            // reso1amp
            CPPUNIT_ASSERT_EQUAL( 0, track1->getCurve( "reso1amp" )->size() );
            // reso1bw
            CPPUNIT_ASSERT_EQUAL( 0, track1->getCurve( "reso1bw" )->size() );
            // reso1freq
            CPPUNIT_ASSERT_EQUAL( 0, track1->getCurve( "reso1freq" )->size() );
            // reso2amp
            CPPUNIT_ASSERT_EQUAL( 0, track1->getCurve( "reso2amp" )->size() );
            // reso2bw
            CPPUNIT_ASSERT_EQUAL( 0, track1->getCurve( "reso2bw" )->size() );
            // reso2freq
            CPPUNIT_ASSERT_EQUAL( 0, track1->getCurve( "reso2freq" )->size() );
            // reso3amp
            CPPUNIT_ASSERT_EQUAL( 0, track1->getCurve( "reso3amp" )->size() );
            // reso3bw
            CPPUNIT_ASSERT_EQUAL( 0, track1->getCurve( "reso3bw" )->size() );
            // reso3freq
            CPPUNIT_ASSERT_EQUAL( 0, track1->getCurve( "reso3freq" )->size() );
            // reso4amp
            CPPUNIT_ASSERT_EQUAL( 0, track1->getCurve( "reso4amp" )->size() );
            // reso4bw
            CPPUNIT_ASSERT_EQUAL( 0, track1->getCurve( "reso4bw" )->size() );
            // reso4freq
            CPPUNIT_ASSERT_EQUAL( 0, track1->getCurve( "reso4freq" )->size() );
        }

        {
            // テンポ変更
            CPPUNIT_ASSERT_EQUAL( 2, sequence.tempoList.size() );
            {
                Tempo tempo = sequence.tempoList.get( 0 );
                CPPUNIT_ASSERT_EQUAL( 500000, tempo.tempo );
                CPPUNIT_ASSERT_EQUAL( (tick_t)0, tempo.clock );
                CPPUNIT_ASSERT_EQUAL( 0.0, tempo.getTime() );
            }
            {
                Tempo tempo = sequence.tempoList.get( 1 );
                CPPUNIT_ASSERT_EQUAL( 250000, tempo.tempo );
                CPPUNIT_ASSERT_EQUAL( (tick_t)1920, tempo.clock );
                CPPUNIT_ASSERT_EQUAL( 2.0, tempo.getTime() );
            }
        }

        {
            // 拍子変更
            CPPUNIT_ASSERT_EQUAL( 2, sequence.timesigList.size() );
            {
                Timesig timesig = sequence.timesigList.get( 0 );
                CPPUNIT_ASSERT_EQUAL( 0, timesig.barCount );
                CPPUNIT_ASSERT_EQUAL( 4, timesig.numerator );
                CPPUNIT_ASSERT_EQUAL( 4, timesig.denominator );
                CPPUNIT_ASSERT_EQUAL( (tick_t)0, timesig.clock );
            }
            {
                Timesig timesig = sequence.timesigList.get( 1 );
                CPPUNIT_ASSERT_EQUAL( 1, timesig.barCount );
                CPPUNIT_ASSERT_EQUAL( 3, timesig.numerator );
                CPPUNIT_ASSERT_EQUAL( 4, timesig.denominator );
                CPPUNIT_ASSERT_EQUAL( (tick_t)1920, timesig.clock );
            }
        }
    }

    CPPUNIT_TEST_SUITE( VSQFileReaderTest );
    CPPUNIT_TEST( testRead );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( VSQFileReaderTest );
