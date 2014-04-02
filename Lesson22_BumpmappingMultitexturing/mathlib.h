/************************************************
*
* header file for math lib
*
***********************************************/

#ifndef _MATHLIB_H
#define _MATHLIB_H

#include <math.h>

//VC++ math.h (and others) do not define M_PI
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/// This is our 3D point class.  This will be used to store the vertices of our model.
class Vector3f
{
public:
	float x, y, z;
};

/// This is our 2D point class.  This will be used to store the UV coordinates.
class Vector2f
{
public:
	float x, y;
};

/// This is our Color class. It is used to hold r,g,b values.
class Color_RGB
{
public:
	float r, g, b;
};

//////////////////////////////////////////////////////////////
/// Function Prototypes
//////////////////////////////////////////////////////////////

// This computes the magnitude of a normal.   (magnitude = sqrt(x^2 + y^2 + z^2)
#define Mag(Normal) (sqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z))

// This calculates a vector between 2 points and returns the result
Vector3f Vector(Vector3f vPoint1, Vector3f vPoint2);

// This adds 2 vectors together and returns the result
Vector3f AddVector(Vector3f vVector1, Vector3f vVector2);

// This divides a vector by a single number (scalar) and returns the result
Vector3f DivideVectorByScaler(Vector3f vVector1, float Scaler);

// This returns the cross product between 2 vectors
Vector3f Cross(Vector3f vVector1, Vector3f vVector2);

// This returns the normal of a vector
Vector3f Normalize(Vector3f vNormal);

//Packs a vectors components into the range [0;1] for storage in an RGB image
Vector3f PackToRGBImage(Vector3f vPack);

#endif //_MATHLIB_H