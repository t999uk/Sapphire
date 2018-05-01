#include <cmath>
#include "UtilMath.h"

float Core::Math::Util::distanceSq( float x, float y, float z, float x1, float y1, float z1 )
{
   float deltaX = x - x1;
   float deltaY = y - y1;
   float deltaZ = z - z1;

   return ( deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ );
}

float Core::Math::Util::distance( float x, float y, float z, float x1, float y1, float z1 )
{
   return sqrtf( distanceSq( x, y, z, x1, y1, z1 ) );
}

float Core::Math::Util::distance2DSq( float x, float y, float x1, float y1 )
{
   float deltaX = x - x1;
   float deltaY = y - y1;
   return ( deltaX * deltaX + deltaY * deltaY );
}

float Core::Math::Util::distance2D( float x, float y, float x1, float y1 )
{
   return sqrtf( distance2DSq( x, y, x1, y1 ) );
}

float Core::Math::Util::calcAngTo( float x, float y, float x1, float y1 )
{
   float dx = x - x1;
   float dy = y - y1;
   if( dy != 0.0f )
   {
      return atan2( dy, dx );
   }
   else
   {
      return 0.0f;
   }
}

float Core::Math::Util::calcAngFrom( float x, float y, float x1, float y1 )
{
   float dx = x - x1;
   float dy = y - y1;
   if( dy != 0.0f )
   {
      return atan2( dy, dx );
   }
   else
   {
      return 0.0f;
   }
}

uint16_t Core::Math::Util::floatToUInt16( float val )
{
   return static_cast< uint16_t >( 0x8000 + val * 32.767f );
}

uint16_t Core::Math::Util::floatToUInt16Rot( float val )
{
   return static_cast< uint16_t >( 0x8000 * ( ( val + PI ) ) / PI );
}

uint8_t Core::Math::Util::floatToUInt8Rot( float val )
{
   return static_cast< uint8_t >( 0x80 * ( ( val + PI ) ) / PI );
}