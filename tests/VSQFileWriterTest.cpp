#include "Util.hpp"
#include "../VSQFileWriter.hpp"
#include "../ByteArrayOutputStream.hpp"
#include <iostream>

using namespace std;
using namespace VSQ_NS;

class VSQFileWriterStub : public VSQFileWriter{
public:
    class TempEventStub : public TempEvent{
    public:
        explicit TempEventStub( const Event &item ) :
            TempEvent( item )
        {
        }
    };

    void writeUnsignedShort( OutputStream *stream, int data ){
        VSQFileWriter::writeUnsignedShort( stream, data );
    }

    void writeUnsignedInt( OutputStream *stream, int data ){
        VSQFileWriter::writeUnsignedInt( stream, data );
    }

    vector<MidiEvent> getMidiEventsFromMetaText( TextStream *stream, const string &encoding ){
        return VSQFileWriter::getMidiEventsFromMetaText( stream, encoding );
    }

    vector<int> getLinePrefixBytes( int count ){
        return VSQFileWriter::getLinePrefixBytes( count );
    }

    int getHowManyDigits( int number ){
        return VSQFileWriter::getHowManyDigits( number );
    }

    void printMetaText(const Track &t, TextStream &stream, int eos, tick_t start, bool printPitch, Master *master, Mixer *mixer){
        VSQFileWriter::printMetaText( t, stream, eos, start, printPitch, master, mixer );
    }

    void writeEvent( const TempEventStub &item, TextStream &stream, EventWriteOption printTargets = EventWriteOption() )const{
        VSQFileWriter::writeEvent( item, stream, printTargets );
    }

    void writeHandle( const Handle &item, TextStream &stream ){
        VSQFileWriter::writeHandle( item, stream );
    }
};

class VSQFileWriterTest : public CppUnit::TestCase{
    static string getLF(){
        static string result = "\x0A";
        return result;
    }

    Handle getLyricHandle(){
        Handle handle( HandleType::LYRIC );
        Lyric lyric0( "あ", "a" );
        lyric0.setConsonantAdjustment( "0" );
        lyric0.lengthRatio = 0.4;
        lyric0.isProtected = true;
        Lyric lyric1( "は", "h a" );
        lyric1.setConsonantAdjustment( "64,0" );
        lyric1.lengthRatio = 0.6;
        lyric1.isProtected = false;
        handle.setLyricAt( 0, lyric0 );
        handle.addLyric( lyric1 );
        handle.index = 1;
        return handle;
    }

    Handle getCrescendoHandle(){
        Handle result( HandleType::DYNAMICS );
        result.iconId = "$05020001";
        result.ids = "Crescendo";
        result.original = 4;
        result.index = 4;
        result.caption = "Zero Crescendo Curve";
        result.setLength( 960 );
        result.startDyn = 2;
        result.endDyn = 38;
        vector<double> dynBPX;
        dynBPX.push_back( 0.5 );
        vector<int> dynBPY;
        dynBPY.push_back( 11 );
        result.dynBP = VibratoBPList( dynBPX, dynBPY );;
        return result;
    }

    Handle getVibratoHandle(){
        Handle result( HandleType::VIBRATO );
        result.iconId = "$04040004";
        result.ids = "normal-da-yo";
        result.caption = "キャプションです=あ";
        result.original = 5;
        result.setLength( 120 );
        result.index = 1;

        result.startDepth = 64;
        vector<double> depthBPX;
        depthBPX.push_back( 0.5 );
        depthBPX.push_back( 0.75 );
        depthBPX.push_back( 1.0 );
        vector<int> depthBPY;
        depthBPY.push_back( 64 );
        depthBPY.push_back( 32 );
        depthBPY.push_back( 0 );
        result.depthBP = VibratoBPList( depthBPX, depthBPY );

        result.startRate = 64;
        vector<double> rateBPX;
        rateBPX.push_back( 0.5 );
        rateBPX.push_back( 0.75 );
        rateBPX.push_back( 1.0 );
        vector<int> rateBPY;
        rateBPY.push_back( 64 );
        rateBPY.push_back( 32 );
        rateBPY.push_back( 0 );
        result.rateBP = VibratoBPList( rateBPX, rateBPY );

        return result;
    }

    Handle getSingerHandle(){
        Handle result( HandleType::SINGER );
        result.iconId = "$07010002";
        result.ids = "Miku3=God";
        result.original = 2;
        result.caption = "";
        result.setLength( 1 );
        result.language = 1;
        result.program = 2;
        result.index = 2;
        return result;
    }

    Handle getAttackHandle(){
        Handle result( HandleType::NOTE_HEAD );
        result.iconId = "$01010002";
        result.ids = "accent";
        result.original = 2;
        result.index = 3;
        result.caption = "Accent";
        result.setLength( 120 );
        result.duration = 64;
        result.depth = 63;
        return result;
    }

    void testWriteWithoutPitch(){
        Sequence sequence( "Foo", 1, 4, 4, 500000 );
        const int CURVE_COUNT = 19;
        string curveNames[CURVE_COUNT] = { "BRE", "BRI", "CLE", "POR", "GEN", "harmonics", "OPE",
                       "reso1amp", "reso1bw", "reso1freq",
                       "reso2amp", "reso2bw", "reso2freq",
                       "reso3amp", "reso3bw", "reso3freq",
                       "reso4amp", "reso4bw", "reso4freq" };
        for( int i = 0; i < CURVE_COUNT; i++ ){
            BPList *list = sequence.track[1].getCurve( curveNames[i] );
            list->add( 1920, 1 + i );
        }
        Event noteEvent( 1920, EventType::NOTE );
        noteEvent.note = 60;
        noteEvent.setLength( 480 );
        sequence.track[1].getEvents()->add( noteEvent );

        ByteArrayOutputStream stream;
        VSQFileWriter writer;
        writer.write( &sequence, &stream, 500, "Shift_JIS", false );
        string actualString = stream.toString();
        stream.close();

        // 期待値と比較する
        vector<char> expected;
        string fixturePath = TestUtil::getFixtureRootPath() + "/VSQFileWriterTest/expected/expected.vsq";
        FILE *fileHandle = fopen( fixturePath.c_str(), "rb" );
        if( ! fileHandle ){
            CPPUNIT_FAIL( "fixture を読み込めなかった" );
        }
        while( 1 ){
            int c = fgetc( fileHandle );
            if( c == EOF ) break;
            expected.push_back( (char)c );
        }
        fclose( fileHandle );

        vector<char> actual;
        actual.insert( actual.begin(), actualString.begin(), actualString.end() );
        CPPUNIT_ASSERT( expected == actual );
    }

    /**
     * @todo
     */
    void testWriteWithPitch(){
    //    fail();
    }

    void test_getMidiEventsFromMetaText(){
        TextStream stream;
        // 「あ」が Shift_JIS になった時分割される「あ」を Shift_JIS にすると「0x82 0xA0」
        stream.write( StringUtil::repeat( "a", 118 ) + "あ" );
        stream.write( StringUtil::repeat( "b", 63 ) );
        VSQFileWriterStub writer;
        vector<MidiEvent> events = writer.getMidiEventsFromMetaText( &stream, "Shift_JIS" );
        CPPUNIT_ASSERT_EQUAL( (size_t)2, events.size() );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, events[0].clock );
        CPPUNIT_ASSERT_EQUAL( 0xFF, events[0].firstByte );
        CPPUNIT_ASSERT_EQUAL( (size_t)128, events[0].data.size() );
        CPPUNIT_ASSERT_EQUAL( 0x01, events[0].data[0] );
        ostringstream actual;
        for( int i = 1; i < events[0].data.size(); i++ ){
            actual << (char)events[0].data[i];
        }
        CPPUNIT_ASSERT_EQUAL( string( "DM:0000:" ) + StringUtil::repeat( "a", 118 ) + (char)0x82, actual.str() );

        CPPUNIT_ASSERT_EQUAL( (tick_t)0, events[1].clock );
        CPPUNIT_ASSERT_EQUAL( 0xFF, events[1].firstByte );
        CPPUNIT_ASSERT_EQUAL( (size_t)73, events[1].data.size() );
        CPPUNIT_ASSERT_EQUAL( 0x01, events[1].data[0] );
        actual.str( "" );
        for( int i = 1; i < events[1].data.size(); i++ ){
            actual << (char)events[1].data[i];
        }
        CPPUNIT_ASSERT_EQUAL( string( "DM:0001:" ) + (char)0xA0 + StringUtil::repeat( "b", 63 ), actual.str() );
    }

    void test_getLinePrefixBytes(){
        VSQFileWriterStub writer;

        // 4 桁
        vector<int> expected;
        expected.push_back( 0xff & 'D' );
        expected.push_back( 0xff & 'M' );
        expected.push_back( 0xff & ':' );
        expected.push_back( 0xff & '0' );
        expected.push_back( 0xff & '1' );
        expected.push_back( 0xff & '2' );
        expected.push_back( 0xff & '3' );
        expected.push_back( 0xff & ':' );
        vector<int> actual = writer.getLinePrefixBytes( 123 );
        CPPUNIT_ASSERT_EQUAL( expected.size(), actual.size() );
        for( int i = 0; i < expected.size(); i++ ){
            CPPUNIT_ASSERT_EQUAL( expected[i], actual[i] );
        }

        // 4 桁
        expected.clear();
        expected.push_back( 0xff & 'D' );
        expected.push_back( 0xff & 'M' );
        expected.push_back( 0xff & ':' );
        expected.push_back( 0xff & '9' );
        expected.push_back( 0xff & '9' );
        expected.push_back( 0xff & '9' );
        expected.push_back( 0xff & '9' );
        expected.push_back( 0xff & ':' );
        actual = writer.getLinePrefixBytes( 9999 );
        CPPUNIT_ASSERT_EQUAL( expected.size(), actual.size() );
        for( int i = 0; i < expected.size(); i++ ){
            CPPUNIT_ASSERT_EQUAL( expected[i], actual[i] );
        }

        // 8 桁
        expected.clear();
        expected.push_back( 0xff & 'D' );
        expected.push_back( 0xff & 'M' );
        expected.push_back( 0xff & ':' );
        expected.push_back( 0xff & '0' );
        expected.push_back( 0xff & '0' );
        expected.push_back( 0xff & '0' );
        expected.push_back( 0xff & '1' );
        expected.push_back( 0xff & '2' );
        expected.push_back( 0xff & '3' );
        expected.push_back( 0xff & '4' );
        expected.push_back( 0xff & '5' );
        expected.push_back( 0xff & ':' );
        actual = writer.getLinePrefixBytes( 12345 );
        CPPUNIT_ASSERT_EQUAL( expected.size(), actual.size() );
        for( int i = 0; i < expected.size(); i++ ){
            CPPUNIT_ASSERT_EQUAL( expected[i], actual[i] );
        }

        // 8 桁
        expected.clear();
        expected.push_back( 0xff & 'D' );
        expected.push_back( 0xff & 'M' );
        expected.push_back( 0xff & ':' );
        expected.push_back( 0xff & '0' );
        expected.push_back( 0xff & '1' );
        expected.push_back( 0xff & '2' );
        expected.push_back( 0xff & '3' );
        expected.push_back( 0xff & '4' );
        expected.push_back( 0xff & '5' );
        expected.push_back( 0xff & '6' );
        expected.push_back( 0xff & '7' );
        expected.push_back( 0xff & ':' );
        actual = writer.getLinePrefixBytes( 1234567 );
        CPPUNIT_ASSERT_EQUAL( expected.size(), actual.size() );
        for( int i = 0; i < expected.size(); i++ ){
            CPPUNIT_ASSERT_EQUAL( expected[i], actual[i] );
        }
    }

    void test_getHowManyDigits(){
        VSQFileWriterStub writer;
        CPPUNIT_ASSERT_EQUAL( 1, writer.getHowManyDigits( 0 ) );
        CPPUNIT_ASSERT_EQUAL( 1, writer.getHowManyDigits( 9 ) );
        CPPUNIT_ASSERT_EQUAL( 2, writer.getHowManyDigits( 99 ) );
        CPPUNIT_ASSERT_EQUAL( 10, writer.getHowManyDigits( 1000000000 ) );
        CPPUNIT_ASSERT_EQUAL( 2, writer.getHowManyDigits( -10 ) );
    }

    void test_writeUnsignedShort(){
        ByteArrayOutputStream stream;
        VSQFileWriterStub writer;
        writer.writeUnsignedShort( &stream, 0x8421 );
        string actual = stream.toString();
        ostringstream expected;
        expected << (char)0x84 << (char)0x21;
        CPPUNIT_ASSERT_EQUAL( expected.str(), actual );
    }

    void test_writeUnsignedInt(){
        ByteArrayOutputStream stream;
        VSQFileWriterStub writer;
        writer.writeUnsignedInt( &stream, 0x84212184 );
        string actual = stream.toString();
        ostringstream expected;
        expected << (char)0x84 << (char)0x21 << (char)0x21 << (char)0x84;
        CPPUNIT_ASSERT_EQUAL( expected.str(), actual );
    }

    void testPrintTrackMetaText(){
        Track track( "DummyTrackName", "DummySingerName" );

        Event singerEvent( 0, EventType::SINGER );
        singerEvent.singerHandle = Handle( HandleType::SINGER ); //h#0000
        singerEvent.singerHandle.iconId = "$07010002";
        singerEvent.singerHandle.ids = "Miku";
        singerEvent.singerHandle.original = 1;
        singerEvent.singerHandle.caption = "caption for miku";
        singerEvent.singerHandle.language = 1;
        singerEvent.singerHandle.program = 2;
        track.getEvents()->set( 0, singerEvent );

        Event crescendoEvent( 240, EventType::ICON );
        crescendoEvent.note = 64;
        crescendoEvent.iconDynamicsHandle = Handle( HandleType::DYNAMICS ); //h#0001
        crescendoEvent.iconDynamicsHandle.iconId = "$05020001";
        crescendoEvent.iconDynamicsHandle.ids = "crescendo";
        crescendoEvent.iconDynamicsHandle.original = 1;
        crescendoEvent.iconDynamicsHandle.caption = "caption for crescendo";
        crescendoEvent.iconDynamicsHandle.startDyn = 4;
        crescendoEvent.iconDynamicsHandle.endDyn = 7;
        crescendoEvent.setLength( 10 );
        track.getEvents()->add( crescendoEvent, 2 );

        Event dynaffEvent( 480, EventType::ICON );
        dynaffEvent.note = 65;
        dynaffEvent.iconDynamicsHandle = Handle( HandleType::DYNAMICS );//h#0002
        dynaffEvent.iconDynamicsHandle.iconId = "$05010001";
        dynaffEvent.iconDynamicsHandle.ids = "dynaff";
        dynaffEvent.iconDynamicsHandle.original = 2;
        dynaffEvent.iconDynamicsHandle.caption = "caption for dynaff";
        dynaffEvent.iconDynamicsHandle.startDyn = 5;
        dynaffEvent.iconDynamicsHandle.endDyn = 8;
        dynaffEvent.setLength( 11 );
        track.getEvents()->add( dynaffEvent, 3 );

        Event decrescendoEvent( 720, EventType::ICON );
        decrescendoEvent.note = 66;
        decrescendoEvent.iconDynamicsHandle = Handle( HandleType::DYNAMICS );//h#0003
        decrescendoEvent.iconDynamicsHandle.iconId = "$05030001";
        decrescendoEvent.iconDynamicsHandle.ids = "decrescendo";
        decrescendoEvent.iconDynamicsHandle.original = 3;
        decrescendoEvent.iconDynamicsHandle.caption = "caption for decrescendo";
        decrescendoEvent.iconDynamicsHandle.startDyn = 6;
        decrescendoEvent.iconDynamicsHandle.endDyn = 9;
        decrescendoEvent.setLength( 12 );
        track.getEvents()->add( decrescendoEvent, 4 );

        Event singerEvent2( 1920, EventType::SINGER );
        singerEvent2.singerHandle = Handle( HandleType::SINGER );//h#0004
        singerEvent2.singerHandle.iconId = "$07020003";
        singerEvent2.singerHandle.ids = "Luka_EN";
        singerEvent2.singerHandle.original = 0x82;
        singerEvent2.singerHandle.caption = "caption for luka";
        singerEvent2.singerHandle.language = 2;
        singerEvent2.singerHandle.program = 3;
        track.getEvents()->add( singerEvent2, 5 );

        Event noteEvent( 1920, EventType::NOTE );
        noteEvent.note = 67;
        noteEvent.dynamics = 68;
        noteEvent.pmBendDepth = 69;
        noteEvent.pmBendLength = 70;
        noteEvent.pmbPortamentoUse = 3;
        noteEvent.demDecGainRate = 71;
        noteEvent.demAccent = 72;
        noteEvent.setLength( 480 );
        noteEvent.lyricHandle = Handle( HandleType::LYRIC );
        noteEvent.lyricHandle.setLyricAt( 0, Lyric( "ら", "4 a" ) );//h#0005
        noteEvent.vibratoHandle = Handle( HandleType::VIBRATO );//h#0006
        noteEvent.vibratoDelay = 73;
        noteEvent.vibratoHandle.iconId ="$04040004";
        noteEvent.vibratoHandle.ids = "vibrato";
        noteEvent.vibratoHandle.original = 1;
        noteEvent.vibratoHandle.caption = "caption for vibrato";
        noteEvent.vibratoHandle.setLength( 407 );
        noteEvent.vibratoHandle.startDepth = 13;
        noteEvent.vibratoHandle.startRate = 14;
        noteEvent.noteHeadHandle = Handle( HandleType::NOTE_HEAD );//h#0007
        noteEvent.noteHeadHandle.iconId = "$05030000";
        noteEvent.noteHeadHandle.ids = "attack";
        noteEvent.noteHeadHandle.original = 15;
        noteEvent.noteHeadHandle.caption = "caption for attack";
        noteEvent.noteHeadHandle.setLength( 120 );
        noteEvent.noteHeadHandle.duration = 62;
        noteEvent.noteHeadHandle.depth = 65;
        track.getEvents()->add( noteEvent, 6 );

        Master master;
        master.preMeasure = 1;

        Mixer mixer;
        mixer.masterFeder = 1;
        mixer.masterPanpot = 2;
        mixer.masterMute = 3;
        mixer.outputMode = 4;
        mixer.slave.push_back( MixerItem( 5, 6, 7, 8 ) );

        track.getCommon()->version = "DSB301";
        track.getCommon()->name = "foo";
        track.getCommon()->color = "1,2,3";
        track.getCommon()->dynamicsMode = DynamicsMode::STANDARD;
        track.getCommon()->playMode = PlayMode::PLAY_WITH_SYNTH;

        vector<string> curves;
        curves.push_back( "pit" );
        curves.push_back( "pbs" );
        curves.push_back( "dyn" );
        curves.push_back( "bre" );
        curves.push_back( "bri" );
        curves.push_back( "cle" );
        curves.push_back( "RESO1FREQ" );
        curves.push_back( "RESO2FREQ" );
        curves.push_back( "RESO3FREQ" );
        curves.push_back( "RESO4FREQ" );
        curves.push_back( "RESO1BW" );
        curves.push_back( "RESO2BW" );
        curves.push_back( "RESO3BW" );
        curves.push_back( "RESO4BW" );
        curves.push_back( "RESO1amp" );
        curves.push_back( "RESO2amp" );
        curves.push_back( "RESO3amp" );
        curves.push_back( "RESO4amp" );
        curves.push_back( "HARMONICS" );
        curves.push_back( "fx2depth" );
        curves.push_back( "GEN" );
        curves.push_back( "pOr" );
        curves.push_back( "OPE" );
        for( int i = 0; i < curves.size(); i++ ){
            string curveName = curves[i];
            track.getCurve( curveName )->add( 480 + i, i );
        }

        TextStream stream;
        VSQFileWriterStub writer;
        writer.printMetaText( track, stream, 2400, 0, false, &master, &mixer );
        string expected =
            "[Common]\n"
            "Version=DSB301\n"
            "Name=foo\n"
            "Color=1,2,3\n"
            "DynamicsMode=0\n"
            "PlayMode=1\n"

            "[Master]\n"
            "PreMeasure=1\n"

            "[Mixer]\n"
            "MasterFeder=1\n"
            "MasterPanpot=2\n"
            "MasterMute=3\n"
            "OutputMode=4\n"
            "Tracks=1\n"
            "Feder0=5\n"
            "Panpot0=6\n"
            "Mute0=7\n"
            "Solo0=8\n"

            "[EventList]\n"
            "0=ID#0000\n"
            "240=ID#0001\n"
            "480=ID#0002\n"
            "720=ID#0003\n"
            "1920=ID#0004,ID#0005\n"
            "2400=EOS\n"

            "[ID#0000]\n"
            "Type=Singer\n"
            "IconHandle=h#0000\n"

            "[ID#0001]\n"
            "Type=Aicon\n"
            "IconHandle=h#0001\n"
            "Note#=64\n"

            "[ID#0002]\n"
            "Type=Aicon\n"
            "IconHandle=h#0002\n"
            "Note#=65\n"

            "[ID#0003]\n"
            "Type=Aicon\n"
            "IconHandle=h#0003\n"
            "Note#=66\n"

            "[ID#0004]\n"
            "Type=Singer\n"
            "IconHandle=h#0004\n"

            "[ID#0005]\n"
            "Type=Anote\n"
            "Length=480\n"
            "Note#=67\n"
            "Dynamics=68\n"
            "PMBendDepth=69\n"
            "PMBendLength=70\n"
            "PMbPortamentoUse=3\n"
            "DEMdecGainRate=71\n"
            "DEMaccent=72\n"
            "LyricHandle=h#0005\n"
            "VibratoHandle=h#0006\n"
            "VibratoDelay=73\n"
            "NoteHeadHandle=h#0007\n"

            "[h#0000]\n"
            "IconID=$07010002\n"
            "IDS=Miku\n"
            "Original=1\n"
            "Caption=caption for miku\n"
            "Length=0\n"
            "Language=1\n"
            "Program=2\n"

            "[h#0001]\n"
            "IconID=$05020001\n"
            "IDS=crescendo\n"
            "Original=1\n"
            "Caption=caption for crescendo\n"
            "StartDyn=4\n"
            "EndDyn=7\n"
            "Length=10\n"
            "DynBPNum=0\n"

            "[h#0002]\n"
            "IconID=$05010001\n"
            "IDS=dynaff\n"
            "Original=2\n"
            "Caption=caption for dynaff\n"
            "StartDyn=5\n"
            "EndDyn=8\n"
            "Length=11\n"
            "DynBPNum=0\n"

            "[h#0003]\n"
            "IconID=$05030001\n"
            "IDS=decrescendo\n"
            "Original=3\n"
            "Caption=caption for decrescendo\n"
            "StartDyn=6\n"
            "EndDyn=9\n"
            "Length=12\n"
            "DynBPNum=0\n"

            "[h#0004]\n"
            "IconID=$07020003\n"
            "IDS=Luka_EN\n"
            "Original=130\n"
            "Caption=caption for luka\n"
            "Length=0\n"
            "Language=2\n"
            "Program=3\n"

            "[h#0005]\n"
            "L0=\"ら\",\"4 a\",1,64,0,0\n"

            "[h#0006]\n"
            "IconID=$04040004\n"
            "IDS=vibrato\n"
            "Original=1\n"
            "Caption=caption for vibrato\n"
            "Length=407\n"
            "StartDepth=13\n"
            "DepthBPNum=0\n"
            "StartRate=14\n"
            "RateBPNum=0\n"

            "[h#0007]\n"
            "IconID=$05030000\n"
            "IDS=attack\n"
            "Original=15\n"
            "Caption=caption for attack\n"
            "Length=120\n"
            "Duration=62\n"
            "Depth=65\n"

            "[PitchBendBPList]\n"
            "480=0\n"

            "[PitchBendSensBPList]\n"
            "481=1\n"

            "[DynamicsBPList]\n"
            "482=2\n"

            "[EpRResidualBPList]\n"
            "483=3\n"

            "[EpRESlopeBPList]\n"
            "484=4\n"

            "[EpRESlopeDepthBPList]\n"
            "485=5\n"

            "[GenderFactorBPList]\n"
            "500=20\n"

            "[PortamentoTimingBPList]\n"
            "501=21\n"

            "[OpeningBPList]\n"
            "502=22\n";

        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    }

    void testWriteEventNoteWithOption(){
        VSQFileWriterStub::TempEventStub event( Event( 0, EventType::NOTE ) );
        event.setLength( 2 );
        event.note = 6;
        event.dynamics = 21;
        event.pmBendDepth = 4;
        event.pmBendLength = 5;
        event.pmbPortamentoUse = 3;
        event.demDecGainRate = 7;
        event.demAccent = 8;
//        noteEvent.preUtterance = 9;
//        noteEvent.voiceOverlap = 10;
        event.vibratoDelay = 13;
        event.lyricHandle.index = 1;
        event.clock = 20;
        event.index = 1;
        EventWriteOption optionAll;
        optionAll.length = true;
        optionAll.note = true;
        optionAll.dynamics = true;
        optionAll.pmBendDepth = true;
        optionAll.pmBendLength = true;
        optionAll.pmbPortamentoUse = true;
        optionAll.demDecGainRate = true;
        optionAll.demAccent = true;
        //TODO. optionAll.preUtterance = true;
        //TODO. optionAll.voiceOverlap = true;

        TextStream stream;

        // handleがどれもnilな音符イベント
        VSQFileWriterStub writer;
        writer.writeEvent( event, stream, optionAll );
        string expected =
                "[ID#0001]\n"
                "Type=Anote\n"
                "Length=2\n"
                "Note#=6\n"
                "Dynamics=21\n"
                "PMBendDepth=4\n"
                "PMBendLength=5\n"
                "PMbPortamentoUse=3\n"
                "DEMdecGainRate=7\n"
                "DEMaccent=8\n"
                "LyricHandle=h#0001\n";
            //TODO. "PreUtterance=9\n" ..
            //TODO. "VoiceOverlap=10\n" ..
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );

        // handleに全部値が入っている音符イベント
        // 現在、PreUtteranceとVoiceOverlapは扱わないようにしているので、
        // オプション全指定と、オプションが無い場合の動作が全くおなじになってしまっている。
        // ustEventをちゃんと処理するようになったら、TODOコメントのところを外すこと
        event.lyricHandle = Handle( HandleType::LYRIC );
        event.lyricHandle.setLyricAt( 0, Lyric( "わ", "w a" ) );
        event.lyricHandle.index = 11;
        event.vibratoHandle = Handle( HandleType::VIBRATO );
        event.vibratoHandle.index = 12;
        event.noteHeadHandle = Handle( HandleType::NOTE_HEAD );
        event.noteHeadHandle.index = 14;
        stream = TextStream();
        writer.writeEvent( event, stream, optionAll );
        expected =
            "[ID#0001]\n"
            "Type=Anote\n"
            "Length=2\n"
            "Note#=6\n"
            "Dynamics=21\n"
            "PMBendDepth=4\n"
            "PMBendLength=5\n"
            "PMbPortamentoUse=3\n"
            "DEMdecGainRate=7\n"
            "DEMaccent=8\n"
            //TODO. "PreUtterance=9\n" ..
            //TODO. "VoiceOverlap=10\n" ..
            "LyricHandle=h#0011\n"
            "VibratoHandle=h#0012\n"
            "VibratoDelay=13\n"
            "NoteHeadHandle=h#0014\n";
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );

        // オプションが無い場合
        stream = TextStream();
        writer.writeEvent( event, stream );
        expected =
            "[ID#0001]\n"
            "Type=Anote\n"
            "Length=2\n"
            "Note#=6\n"
            "Dynamics=21\n"
            "PMBendDepth=4\n"
            "PMBendLength=5\n"
            "PMbPortamentoUse=3\n"
            "DEMdecGainRate=7\n"
            "DEMaccent=8\n"
            "LyricHandle=h#0011\n"
            "VibratoHandle=h#0012\n"
            "VibratoDelay=13\n"
            "NoteHeadHandle=h#0014\n";
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );

        // オプションが空の場合
        stream = TextStream();
        EventWriteOption emptyOption;
        emptyOption.demAccent = false;
        emptyOption.demDecGainRate = false;
        emptyOption.dynamics = false;
        emptyOption.length = false;
        emptyOption.note = false;
        emptyOption.pmBendDepth = false;
        emptyOption.pmBendLength = false;
        emptyOption.pmbPortamentoUse = false;
        emptyOption.preUtterance = false;
        emptyOption.voiceOverlap = false;
        writer.writeEvent( event, stream, emptyOption );
        expected =
            "[ID#0001]\n"
            "Type=Anote\n"
            "LyricHandle=h#0011\n"
            "VibratoHandle=h#0012\n"
            "VibratoDelay=13\n"
            "NoteHeadHandle=h#0014\n";
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    }

    void testWriteEventSinger(){
        VSQFileWriterStub::TempEventStub event( Event( 0, EventType::SINGER ) );
        event.singerHandle = Handle( HandleType::SINGER );
        event.singerHandle.index = 16;
        event.index = 16;
        event.clock = 1;
        event.index = 15;
        TextStream stream;
        VSQFileWriterStub writer;
        writer.writeEvent( event, stream );
        string expected =
            "[ID#0015]\n"
            "Type=Singer\n"
            "IconHandle=h#0016\n";
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    }

    void testWriteEventIcon(){
        VSQFileWriterStub::TempEventStub event( Event( 0, EventType::ICON ) );
        event.note = 19;
        event.index = 17;
        event.iconDynamicsHandle = Handle( HandleType::DYNAMICS );
        event.iconDynamicsHandle.index = 18;
        event.clock = 2;
        event.index = 17;
        TextStream stream;
        VSQFileWriterStub writer;
        writer.writeEvent( event, stream );
        string expected =
            "[ID#0017]\n"
            "Type=Aicon\n"
            "IconHandle=h#0018\n"
            "Note#=19\n";
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    }

    void testWriteHandleLyric(){
        Handle handle = getLyricHandle();

        TextStream stream;
        VSQFileWriterStub writer;
        writer.writeHandle( handle, stream );
        string expected =
            "[h#0001]" + getLF() +
            "L0=\"あ\",\"a\",0.4,0,1" + getLF() +
            "L1=\"は\",\"h a\",0.6,64,0,0" + getLF();
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    }

    void testWriteHandleVibrato(){
        string lf = getLF();
        Handle handle = getVibratoHandle();
        {
            TextStream stream;
            VSQFileWriterStub writer;
            writer.writeHandle( handle, stream );
            string expected =
                "[h#0001]" + lf +
                "IconID=$04040004" + lf +
                "IDS=normal-da-yo" + lf +
                "Original=5" + lf +
                "Caption=キャプションです=あ" + lf +
                "Length=120" + lf +
                "StartDepth=64" + lf +
                "DepthBPNum=3" + lf +
                "DepthBPX=0.500000,0.750000,1.000000" + lf +
                "DepthBPY=64,32,0" + lf +
                "StartRate=64" + lf +
                "RateBPNum=3" + lf +
                "RateBPX=0.500000,0.750000,1.000000" + lf +
                "RateBPY=64,32,0" + lf;
            CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
        }

        {
            handle.rateBP = VibratoBPList( vector<double>(), vector<int>() );
            handle.depthBP = VibratoBPList( vector<double>(), vector<int>() );
            TextStream stream;
            VSQFileWriterStub writer;
            writer.writeHandle( handle, stream );
            string expected =
                "[h#0001]" + lf +
                "IconID=$04040004" + lf +
                "IDS=normal-da-yo" + lf +
                "Original=5" + lf +
                "Caption=キャプションです=あ" + lf +
                "Length=120" + lf +
                "StartDepth=64" + lf +
                "DepthBPNum=0" + lf +
                "StartRate=64" + lf +
                "RateBPNum=0" + lf;
            CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
        }
    }

    void testWriteHandleSinger(){
        Handle handle = getSingerHandle();
        TextStream stream;
        VSQFileWriterStub writer;
        writer.writeHandle( handle, stream );
        string lf = getLF();
        string expected =
            "[h#0002]" + lf +
            "IconID=$07010002" + lf +
            "IDS=Miku3=God" + lf +
            "Original=2" + lf +
            "Caption=" + lf +
            "Length=1" + lf +
            "Language=1" + lf +
            "Program=2" + lf;
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    }

    void testWriteHandleAttack(){
        Handle handle = getAttackHandle();
        TextStream stream;
        VSQFileWriterStub writer;
        writer.writeHandle( handle, stream );
        string lf = getLF();
        string expected =
            "[h#0003]" + lf +
            "IconID=$01010002" + lf +
            "IDS=accent" + lf +
            "Original=2" + lf +
            "Caption=Accent" + lf +
            "Length=120" + lf +
            "Duration=64" + lf +
            "Depth=63" + lf;
        CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
    }

    void testWriteHandleCrescendo(){
        string lf = getLF();
        Handle handle = getCrescendoHandle();
        {
            TextStream stream;
            VSQFileWriterStub writer;
            writer.writeHandle( handle, stream );
            string expected =
                "[h#0004]" + lf +
                "IconID=$05020001" + lf +
                "IDS=Crescendo" + lf +
                "Original=4" + lf +
                "Caption=Zero Crescendo Curve" + lf +
                "StartDyn=2" + lf +
                "EndDyn=38" + lf +
                "Length=960" + lf +
                "DynBPNum=1" + lf +
                "DynBPX=0.500000" + lf +
                "DynBPY=11" + lf;
            CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
        }

        {
            // dynBPのデータ点が複数
            handle.dynBP = VibratoBPList( "2", "0.4,0.8", "1,2" );
            TextStream stream;
            VSQFileWriterStub writer;
            writer.writeHandle( handle, stream );
            string expected =
                "[h#0004]" + lf +
                "IconID=$05020001" + lf +
                "IDS=Crescendo" + lf +
                "Original=4" + lf +
                "Caption=Zero Crescendo Curve" + lf +
                "StartDyn=2" + lf +
                "EndDyn=38" + lf +
                "Length=960" + lf +
                "DynBPNum=2" + lf +
                "DynBPX=0.400000,0.800000" + lf +
                "DynBPY=1,2" + lf;
            CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
        }

        {
            // dynBPのデータ点が 0 個
            handle.dynBP = VibratoBPList( vector<double>(), vector<int>() );
            TextStream stream;
            VSQFileWriterStub writer;
            writer.writeHandle( handle, stream );
            string expected =
                "[h#0004]" + lf +
                "IconID=$05020001" + lf +
                "IDS=Crescendo" + lf +
                "Original=4" + lf +
                "Caption=Zero Crescendo Curve" + lf +
                "StartDyn=2" + lf +
                "EndDyn=38" + lf +
                "Length=960" + lf +
                "DynBPNum=0" + lf;
            CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
        }

        {
            // dynBPがから
            handle.dynBP = VibratoBPList( vector<double>(), vector<int>() );
            TextStream stream;
            VSQFileWriterStub writer;
            writer.writeHandle( handle, stream );
            string expected =
                "[h#0004]" + lf +
                "IconID=$05020001" + lf +
                "IDS=Crescendo" + lf +
                "Original=4" + lf +
                "Caption=Zero Crescendo Curve" + lf +
                "StartDyn=2" + lf +
                "EndDyn=38" + lf +
                "Length=960" + lf +
                "DynBPNum=0" + lf;
            CPPUNIT_ASSERT_EQUAL( expected, stream.toString() );
        }
    }

    CPPUNIT_TEST_SUITE( VSQFileWriterTest );
    CPPUNIT_TEST( testWriteWithoutPitch );
    CPPUNIT_TEST( testWriteWithPitch );
    CPPUNIT_TEST( test_getMidiEventsFromMetaText );
    CPPUNIT_TEST( test_getLinePrefixBytes );
    CPPUNIT_TEST( test_getHowManyDigits );
    CPPUNIT_TEST( test_writeUnsignedShort );
    CPPUNIT_TEST( test_writeUnsignedInt );
    CPPUNIT_TEST( testPrintTrackMetaText );
    CPPUNIT_TEST( testWriteEventNoteWithOption );
    CPPUNIT_TEST( testWriteEventSinger );
    CPPUNIT_TEST( testWriteEventIcon );
    CPPUNIT_TEST( testWriteHandleLyric );
    CPPUNIT_TEST( testWriteHandleVibrato );
    CPPUNIT_TEST( testWriteHandleSinger );
    CPPUNIT_TEST( testWriteHandleAttack );
    CPPUNIT_TEST( testWriteHandleCrescendo );
    CPPUNIT_TEST_SUITE_END();
};

REGISTER_TEST_SUITE( VSQFileWriterTest );
