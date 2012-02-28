#include "VibratoBP.h"

using namespace VSQ_NS;

VibratoBP::VibratoBP()
{
    x = 0.0;
    y = 0;
}

VibratoBP::VibratoBP( double x, int y )
{
    this->x = x;
    this->y = y;
}

int VibratoBP::compareTo( const VibratoBP &item )
{
    return compare( *this, item );
}

int VibratoBP::compare( const VibratoBP &a, const VibratoBP &b )
{
    double v = a.x - b.x;
    if( v > 0.0 ){
        return 1;
    }else if( v < 0.0 ){
        return -1;
    }
    return 0;
}
