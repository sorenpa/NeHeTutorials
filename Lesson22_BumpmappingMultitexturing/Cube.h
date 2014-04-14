/*******************************************************************
Written by: Soren Peter Andersen
email: soren.p.andersen@gmail.com

Description: Header for a class that represent a 3D cube
*******************************************************************/
#include "mathlib.h"
#include "Object3D.h"


class Cube: public Object3D
{
public:
    Cube(GLfloat height, GLfloat length, GLfloat width);
    ~Cube();

private:
    void ConstructCube(GLfloat height, GLfloat length, GLfloat width);

};