#include <sstream>
#include "Tempo.h"

using namespace std;
using namespace VSQ_NS;

Tempo::Tempo()
{
    clock = 0;
    tempo = 0;
    _time = 0.0;
}

const string Tempo::toString()
{
    ostringstream oss;
    oss << "{Clock=" << clock << ", Tempo=" << tempo << ", Time=" << _time << "}";
    return oss.str();
}

Tempo::Tempo( tick_t clock, int tempo )
{
    this->clock = clock;
    this->tempo = tempo;
    _time = 0.0;
}

int Tempo::compareTo(const Tempo &entry)
{
    return compareCore( *this, entry );
}

bool Tempo::equals( const Tempo &entry )
{
    if( clock == entry.clock ){
        return true;
    }else{
        return false;
    }
}

double Tempo::getTime()
{
    return _time;
}

bool Tempo::compare( const Tempo &a, const Tempo &b )
{
    return compareCore( a, b ) < 0;
}

int Tempo::compareCore(const Tempo &a, const Tempo &b)
{
    return (int)(a.clock - b.clock);
}
