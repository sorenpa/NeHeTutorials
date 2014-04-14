#include "mathlib.h"

Vector2f::Vector2f()
{
    x = 0.0f;
    y = 0.0f;
}

Vector2f::Vector2f(GLfloat xCoord, GLfloat yCoord)
{
    x = xCoord;
    y = yCoord;
}

Vector3f::Vector3f()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

Vector3f::Vector3f(GLfloat xCoord, GLfloat yCoord, GLfloat zCoord)
{
    x = xCoord;
    y = yCoord;
    z = zCoord;
}

Vector4f::Vector4f()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
    h = 0.0f;
}

Vector4f::Vector4f(GLfloat xCoord, GLfloat yCoord, GLfloat zCoord, GLfloat hCoord)
{
    x = xCoord;
    y = yCoord;
    z = zCoord;
    h = hCoord;
}

Vector4f::Vector4f(Vector3f v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    h = 0.0f; //
}

Matrix4x4f::Matrix4x4f()
{ 
    m_row1 = Vector4f();
    m_row2 = Vector4f();
    m_row3 = Vector4f();
    m_row4 = Vector4f();
}

Matrix4x4f::Matrix4x4f(GLfloat *M)
{
    m_row1 = Vector4f(M[0], M[1],  M[2],  M[3]);
    m_row2 = Vector4f(M[4], M[5],  M[6],  M[7]);
    m_row3 = Vector4f(M[8], M[9],  M[10], M[11]);
    m_row4 = Vector4f(M[12],M[13], M[14], M[15]);
}

Matrix4x4f::Matrix4x4f(Vector4f row1, Vector4f row2, Vector4f row3, Vector4f row4)
{ 
    m_row1 = row1;
    m_row2 = row2;
    m_row3 = row3;
    m_row4 = row4;
}


Matrix4x4f::~Matrix4x4f()
{ 
//Empty    
};

// This calculates a vector between 2 points and returns the result
Vector3f Vector(Vector3f vPoint1, Vector3f vPoint2)
{
    Vector3f v;							// The variable to hold the resultant vector

    v.x = vPoint1.x - vPoint2.x;			// Subtract point1 and point2 x's
    v.y = vPoint1.y - vPoint2.y;			// Subtract point1 and point2 y's
    v.z = vPoint1.z - vPoint2.z;			// Subtract point1 and point2 z's

    return v;								// Return the resultant vector
}

// This adds 2 vectors together and returns the result
Vector3f AddVector(Vector3f v1, Vector3f v2)
{
    Vector3f vResult;							// The variable to hold the resultant vector
    
    vResult.x = v2.x + v1.x;		// Add Vector1 and Vector2 x's
    vResult.y = v2.y + v1.y;		// Add Vector1 and Vector2 y's
    vResult.z = v2.z + v1.z;		// Add Vector1 and Vector2 z's

    return vResult;								// Return the resultant vector
}

// This divides a vector by a single number (scalar) and returns the result
Vector3f DivideVectorByScalar(Vector3f v1, GLfloat Scalar)
{
    Vector3f vResult;							// The variable to hold the resultant vector
    
    vResult.x = v1.x / Scalar;			// Divide Vector1's x value by the scalar
    vResult.y = v1.y / Scalar;			// Divide Vector1's y value by the scalar
    vResult.z = v1.z / Scalar;			// Divide Vector1's z value by the scalar

    return vResult;								// Return the resultant vector
}

// This divides a vector by a single number (scalar) and returns the result
Vector3f MultiplyVectorByScalar(Vector3f v1, GLfloat Scalar)
{
    Vector3f vResult;							// The variable to hold the resultant vector
    
    vResult.x = v1.x * Scalar;			// multiply Vector1's x value by the scalar
    vResult.y = v1.y * Scalar;			// multiply Vector1's y value by the scalar
    vResult.z = v1.z * Scalar;			// multiply Vector1's z value by the scalar

    return vResult;								// Return the resultant vector
}

// This returns the cross product between 2 vectors
Vector3f CrossProduct(Vector3f v1, Vector3f v2)
{
    Vector3f vCross;								// The vector to hold the cross product
                                                // Get the X value
    vCross.x = ((v1.y * v2.z) - (v1.z * v2.y));
                                                // Get the Y value
    vCross.y = ((v1.z * v2.x) - (v1.x * v2.z));
                                                // Get the Z value
    vCross.z = ((v1.x * v2.y) - (v1.y * v2.x));

    return vCross;								// Return the cross product
}

// This returns the cross product between 2 vectors
GLfloat DotProduct(Vector3f v1, Vector3f v2)
{
    GLfloat vDotProd;								// The vector to hold the cross product
    
    vDotProd = v1.x*v2.x + v1.y*v2.y +v1.z+v2.z; //Multiply components and add them together

    return vDotProd;								// Return the cross product
}

// This returns the normal of a vector
Vector3f Normalize(Vector3f vNormal)
{
    double magnitude;							// This holds the magitude			

    magnitude = Mag(vNormal);					// Get the magnitude

    vNormal.x /= magnitude;				// Divide the vector's X by the magnitude
    vNormal.y /= magnitude;				// Divide the vector's Y by the magnitude
    vNormal.z /= magnitude;				// Divide the vector's Z by the magnitude

    return vNormal;								// Return the normal
}

Vector3f PackNormalToRGBImage(Vector3f vPack)
{
    vPack.x /= (vPack.x +1)/2.0; //Pack the x-component of vPack to be within [0;1] range
    vPack.y /= (vPack.y +1)/2.0; //Pack the y-component of vPack to be within [0;1] range
    vPack.z /= (vPack.z +1)/2.0; //Pack the z-component of vPack to be within [0;1] range

    return vPack;								// Return the Packed vector
}

//Column vector multiplication M*v
Vector4f MuliplyMatrixByVector(Matrix4x4f M, Vector4f v)
{
    Vector4f vResult;
    
    vResult.x = M.m_row1.x * v.x + M.m_row1.y * v.y + M.m_row1.z * v.z + M.m_row1.h * v.h;
    vResult.y = M.m_row2.x * v.x + M.m_row2.y * v.y + M.m_row2.z * v.z + M.m_row2.h * v.h;
    vResult.z = M.m_row3.x * v.x + M.m_row3.y * v.y + M.m_row3.z * v.z + M.m_row3.h * v.h;
    vResult.h = M.m_row4.h * v.h; //Homogeneous coordinates

    return vResult;
}
