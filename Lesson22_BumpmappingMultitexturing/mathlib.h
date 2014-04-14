/************************************************
*
* header file for math lib
*
***********************************************/
#include <Windows.h>
#include <math.h>

#ifndef __gl_h_
#ifndef __GL_H__
#include <GL\GL.h>
#endif
#endif

#ifndef _MATHLIB_H
#define _MATHLIB_H






//VC++ math.h (and others) do not define M_PI
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif



/// Class representing a 2D vector
class Vector2f
{
public:
    Vector2f();
    Vector2f(GLfloat x, GLfloat y);
    GLfloat x, y;
};

/// Class representing a 3D vector
class Vector3f
{
public:
    Vector3f();
    Vector3f(GLfloat x, GLfloat y, GLfloat z);
    GLfloat x, y, z;
};

/// Class representing a 4D vector
class Vector4f
{
public:
    Vector4f();
    Vector4f(GLfloat x, GLfloat y, GLfloat z, GLfloat h);
    Vector4f(Vector3f v);
    GLfloat x, y, z, h;
};

class Matrix4x4f
{
public:
    Matrix4x4f();
    Matrix4x4f(GLfloat *M);
    Matrix4x4f(Vector4f row1, Vector4f row2, Vector4f row3, Vector4f row4);
    ~Matrix4x4f();

    Vector4f m_row1;
    Vector4f m_row2;
    Vector4f m_row3;
    Vector4f m_row4;

private:

};


//////////////////////////////////////////////////////////////
/// Function Prototypes
//////////////////////////////////////////////////////////////

// This computes the magnitude of a normal.   (magnitude = sqrt(x^2 + y^2 + z^2)
#define Mag(Normal) (sqrt(Normal.x*Normal.x + Normal.y*Normal.y + Normal.z*Normal.z))

// This calculates a vector between 2 points and returns the result
Vector3f Vector(Vector3f vPoint1, Vector3f vPoint2);

// This adds 2 vectors together and returns the result
Vector3f AddVector(Vector3f v1, Vector3f v2);

// This divides a vector by a single number (scalar) and returns the result
Vector3f DivideVectorByScalar(Vector3f v1, GLfloat Scalar);

//Multiplies a 3D vector by a scalar
Vector3f MultiplyVectorByScalar(Vector3f v1, GLfloat Scalar);

// This returns the cross product between 2 vectors
Vector3f CrossProduct(Vector3f v1, Vector3f v2);

// This returns the dot product between 2 vectors
GLfloat DotProduct(Vector3f v1, Vector3f v2);

// This returns the normal of a vector
Vector3f Normalize(Vector3f vNormal);

//Packs a vectors components into the range [0;1] for storage in an RGB image
Vector3f PackNormalToRGBImage(Vector3f vPack);

Vector4f MuliplyMatrixByVector(Matrix4x4f,Vector4f);
#endif //_MATHLIB_H