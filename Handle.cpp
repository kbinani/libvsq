#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include "Handle.h"
#include "StringUtil.h"

using namespace std;
using namespace VSQ_NS;

const string Handle::ICONID_HEAD_DYNAFF = "$0501";

const string Handle::ICONID_HEAD_CRESCEND = "$0502";

const string Handle::ICONID_HEAD_DECRESCEND = "$0503";

void Handle::init()
{
    _type = HandleType::LYRIC;
    _articulation = ArticulationType::NONE;
    index = 0;
    iconId = "";
    ids = "";
    original = 0;
    _caption = "";
    _length = 0;
    _startDepth = 0;
    _startRate = 0;
    language = 0;
    program = 0;
    _duration = 0;
    _depth = 0;
    _startDyn = 0;
    _endDyn = 0;
    addQuotationMark = true;
}

Handle::Handle( HandleType::HandleTypeEnum type )
{
    init();
    _type = type;
    if( _type == HandleType::DYNAMICS ){
        _init_icon_dynamics();
    }else if( type == HandleType::NOTE_HEAD ){
        _articulation = ArticulationType::NOTE_ATTACK;
    }else if( type == HandleType::VIBRATO ){
        _init_vibrato();
    }else if( type == HandleType::LYRIC ){
        _init_lyric();
    }
}

Handle::Handle( TextStream &stream, int index, string &lastLine )
{
    init();
    this->index = index;

    // default値で埋める
    _type = HandleType::VIBRATO;
    iconId = "";
    ids = "normal";
    _lyrics.push_back( Lyric( "" ) );
    original = 0;
    _caption = "";
    _length = 0;
    _startDepth = 0;
    _startRate = 0;
    language = 0;
    program = 0;
    _duration = 0;
    _depth = 64;

    string tmpDepthBPX = "";
    string tmpDepthBPY = "";
    string tmpDepthBPNum = "";

    string tmpRateBPX = "";
    string tmpRateBPY = "";
    string tmpRateBPNum = "";

    string tmpDynBPX = "";
    string tmpDynBPY = "";
    string tmpDynBPNum = "";

    // "["にぶち当たるまで読込む
    lastLine = stream.readLine();
    while( lastLine.find( "[" ) != 0 ){
        {//TODO:
            cout << "Handle::Hanlde; lastLine=" << lastLine << endl;
        }
        vector<string> spl = StringUtil::explode( "=", lastLine );
        string search = spl[0];
        if( search == "Language" ){
            _type = HandleType::SINGER;
            language = boost::lexical_cast<int>( spl[1] );
        }else if( search == "Program" ){
            program = boost::lexical_cast<int>( spl[1] );
        }else if( search == "IconID" ){
            iconId = spl[1];
        }else if( search == "IDS" ){
            ids = spl[1];
            for( int i = 2; i < spl.size(); i++ ){
                ids = ids + "=" + spl[i];
            }
        }else if( search == "Original" ){
            original = boost::lexical_cast<int>( spl[1] );
        }else if( search == "Caption" ){
            _caption = spl[1];
            for( int i = 2; i < spl.size(); i++ ){
                _caption = _caption + "=" + spl[i];
            }
        }else if( search == "Length" ){
            _length = boost::lexical_cast<tick_t>( spl[1] );
        }else if( search == "StartDepth" ){
            _startDepth = boost::lexical_cast<int>( spl[1] );
        }else if( search == "DepthBPNum" ){
            tmpDepthBPNum = spl[1];
        }else if( search == "DepthBPX" ){
            tmpDepthBPX = spl[1];
        }else if( search == "DepthBPY" ){
            tmpDepthBPY = spl[1];
        }else if( search == "StartRate" ){
            _type = HandleType::VIBRATO;
            _startRate = boost::lexical_cast<int>( spl[1] );
        }else if( search == "RateBPNum" ){
            tmpRateBPNum = spl[1];
        }else if( search == "RateBPX" ){
            tmpRateBPX = spl[1];
        }else if( search == "RateBPY" ){
            tmpRateBPY = spl[1];
        }else if( search == "Duration" ){
            _type = HandleType::NOTE_HEAD;
            _duration = boost::lexical_cast<int>( spl[1] );
        }else if( search == "Depth" ){
            _depth = boost::lexical_cast<int>( spl[1] );
        }else if( search == "StartDyn" ){
            _type = HandleType::DYNAMICS;
            _startDyn = boost::lexical_cast<int>( spl[1] );
        }else if( search == "EndDyn" ){
            _type = HandleType::DYNAMICS;
            _endDyn = boost::lexical_cast<int>( spl[1] );
        }else if( search == "DynBPNum" ){
            tmpDynBPNum = spl[1];
        }else if( search == "DynBPX" ){
            tmpDynBPX = spl[1];
        }else if( search == "DynBPY" ){
            tmpDynBPY = spl[1];
        }else if( search.find( "L" ) == 0 && search.size() >= 2 ){
            int index = boost::lexical_cast<int>( search.substr( 1, 1 ) );
            Lyric lyric( spl[1] );
            _type = HandleType::LYRIC;
            if( _lyrics.size() <= index + 1 ){
                int amount = index + 1 - _lyrics.size();
                for( int i = 0; i < amount; i++ ){
                    _lyrics.push_back( Lyric( "", "" ) );
                }
            }
            _lyrics[index] = lyric;
        }
        if( !stream.ready() ){
            break;
        }
        lastLine = stream.readLine();
    }

    // RateBPX, RateBPYの設定
    if( _type == HandleType::VIBRATO ){
        if( tmpRateBPNum != "" ){
            _rateBP = VibratoBPList( tmpRateBPNum, tmpRateBPX, tmpRateBPY );
        }else{
            _rateBP = VibratoBPList();
        }

        // DepthBPX, DepthBPYの設定
        if( tmpDepthBPNum != "" ){
            _depthBP = VibratoBPList( tmpDepthBPNum, tmpDepthBPX, tmpDepthBPY );
        }else{
            _depthBP = VibratoBPList();
        }
    }else{
        _depthBP = VibratoBPList();
        _rateBP = VibratoBPList();
    }

    if( tmpDynBPNum != "" ){
        _dynBP = VibratoBPList( tmpDynBPNum, tmpDynBPX, tmpDynBPY );
    }else{
        _dynBP = VibratoBPList();
    }
}

void Handle::_init_vibrato()
{
    _articulation = ArticulationType::VIBRATO;
    index = 0;
    iconId = "$04040000";
    ids = "";
    original = 0;
    _startRate = 64;
    _startDepth = 64;
    _rateBP = VibratoBPList();
    _depthBP = VibratoBPList();
}

void Handle::_init_icon_dynamics()
{
    _articulation = ArticulationType::DYNAFF;
    iconId = "";
    ids = "";
    original = 0;
}

void Handle::_init_lyric()
{
    index = 0;
    _lyrics.clear();
}

ArticulationType::ArticulationTypeEnum Handle::getArticulation() const
{
    return _articulation;
}

bool Handle::isDynaffType() const
{
    return iconId.find( Handle::ICONID_HEAD_DYNAFF ) == 0;
}

bool Handle::isCrescendType() const
{
    return iconId.find( Handle::ICONID_HEAD_CRESCEND ) == 0;
}

bool Handle::isDecrescendType() const
{
    return iconId.find( Handle::ICONID_HEAD_DECRESCEND ) == 0;
}

tick_t Handle::getLength() const
{
    return _length;
}

void Handle::setLength( tick_t value )
{
    _length = value;
}

const string Handle::getCaption() const
{
    return _caption;
}

void Handle::setCaption( const string &value )
{
    _caption = value;
}

int Handle::getStartDyn() const
{
    return _startDyn;
}

void Handle::setStartDyn( int value )
{
    _startDyn = value;
}

int Handle::getEndDyn() const
{
    return _endDyn;
}

void Handle::setEndDyn( int value )
{
    _endDyn = value;
}

const VibratoBPList Handle::getDynBP() const
{
    return _dynBP;
}

void Handle::setDynBP( const VibratoBPList &value )
{
    _dynBP = value;
}

int Handle::getDepth() const
{
    return _depth;
}

void Handle::setDepth( int value )
{
    _depth = value;
}

int Handle::getDuration() const
{
    return _duration;
}

void Handle::setDuration( int value )
{
    _duration = value;
}

const VibratoBPList Handle::getRateBP() const
{
    return _rateBP;
}

void Handle::setRateBP( const VibratoBPList &value )
{
    _rateBP = value;
}

const VibratoBPList Handle::getDepthBP() const
{
    return _depthBP;
}

void Handle::setDepthBP( const VibratoBPList &value )
{
    _depthBP = value;
}

int Handle::getStartRate() const
{
    return _startRate;
}

void Handle::setStartRate( int value )
{
    _startRate = value;
}

int Handle::getStartDepth() const
{
    return _startDepth;
}

void Handle::setStartDepth( int value )
{
    _startDepth = value;
}

const Lyric Handle::getLyricAt( int index ) const
{
    return _lyrics[index];
}

void Handle::setLyricAt( int index, const Lyric &value )
{
    if( _lyrics.size() < index + 1 ){
        int remain = index + 1 - _lyrics.size();
        for( int i = 0; i < remain; i++ ){
            _lyrics.push_back( Lyric( "", "" ) );
        }
    }
    _lyrics[index] = value;
}

int Handle::getLyricCount() const
{
    return _lyrics.size();
}

const string Handle::getDisplayString() const
{
    return ids + _caption;
}

HandleType::HandleTypeEnum Handle::getHandleType() const
{
    return _type;
}

void Handle::write( TextStream &stream )
{
    stream.writeLine( toString() );
}

const string Handle::toString() const
{
    ostringstream result;
    result << "[h#" << (boost::format( "%04d" ) % index) << "]";
    if( _type == HandleType::LYRIC ){
        for( int i = 0; i < _lyrics.size(); i++ ){
            result << "\n" << "L" << i << "=" << _lyrics[i].toString( addQuotationMark );
        }
    }else if( _type == HandleType::VIBRATO ){
        result << "\n" << "IconID=" << iconId << "\n";
        result << "IDS=" << ids << "\n";
        result << "Original=" << original << "\n";
        result << "Caption=" << _caption << "\n";
        result << "Length=" << _length << "\n";
        result << "StartDepth=" << _startDepth << "\n";
        result << "DepthBPNum=" << _depthBP.size() << "\n";
        if( _depthBP.size() > 0 ){
            result << "DepthBPX=" << (boost::format( "%.6f" ) % _depthBP.get( 0 ).x);
            for( int i = 1; i < _depthBP.size(); i++ ){
                result << "," << (boost::format( "%.6f" ) % _depthBP.get( i ).x);
            }
            result << "\n" << "DepthBPY=" << _depthBP.get( 0 ).y;
            for( int i = 1; i < _depthBP.size(); i++ ){
                result << "," << _depthBP.get( i ).y;
            }
            result << "\n";
        }
        result << "StartRate=" << _startRate << "\n";
        result << "RateBPNum=" << _rateBP.size();
        if( _rateBP.size() > 0 ){
            result << "\n" << "RateBPX=" << (boost::format( "%.6f" ) % _rateBP.get( 0 ).x);
            for( int i = 1; i < _rateBP.size(); i++ ){
                result << "," << (boost::format( "%.6f" ) % _rateBP.get( i ).x);
            }
            result << "\n" << "RateBPY=" << _rateBP.get( 0 ).y;
            for( int i = 1; i < _rateBP.size(); i++ ){
                result << "," << _rateBP.get( i ).y;
            }
        }
    }else if( _type == HandleType::SINGER ){
        result << "\n" << "IconID=" << iconId << "\n";
        result << "IDS=" << ids << "\n";
        result << "Original=" << original << "\n";
        result << "Caption=" << _caption << "\n";
        result << "Length=" << _length << "\n";
        result << "Language=" << language << "\n";
        result << "Program=" << program;
    }else if( _type == HandleType::NOTE_HEAD ){
        result << "\n" << "IconID=" << iconId << "\n";
        result << "IDS=" << ids << "\n";
        result << "Original=" << original << "\n";
        result << "Caption=" << _caption << "\n";
        result << "Length=" << _length << "\n";
        result << "Duration=" << _duration << "\n";
        result << "Depth=" << _depth;
    }else if( _type == HandleType::DYNAMICS ){
        result << "\n" << "IconID=" << iconId << "\n";
        result << "IDS=" << ids << "\n";
        result << "Original=" << original << "\n";
        result << "Caption=" << _caption << "\n";
        result << "StartDyn=" << _startDyn << "\n";
        result << "EndDyn=" << _endDyn << "\n";
        result << "Length=" << getLength() << "\n";
        if( _dynBP.size() <= 0 ){
            result << "DynBPNum=0";
        }else{
            int c = _dynBP.size();
            result << "DynBPNum=" << c << "\n";
            result << "DynBPX=" << (boost::format( "%.6f" ) % _dynBP.get( 0 ).x);
            for( int i = 1; i < c; i++ ){
                result << "," << (boost::format( "%.6f" ) % _dynBP.get( i ).x);
            }
            result << "\n" << "DynBPY=" << _dynBP.get( 0 ).y;
            for( int i = 1; i < c; i++ ){
                result << "," << _dynBP.get( i ).y;
            }
        }
    }
    return result.str();
}

int Handle::getHandleIndexFromString( const string &s )
{
    vector<string> spl = StringUtil::explode( "#", s );
    return boost::lexical_cast<int>( spl[1] );
}
