#include "stdafx.h"
#include "X3DEasings.h"

#include "X3D.h"

namespace X3D
{

float easeInOutCircf( float t, float b, float c, float d )
{
	if( ( t /= d / 2 ) < 1 ) return -c / 2 * ( sqrt( 1 - t*t ) - 1 ) + b;
	return c / 2 * ( sqrt( 1 - t*( t -= 2 ) ) + 1 ) + b;
}

}