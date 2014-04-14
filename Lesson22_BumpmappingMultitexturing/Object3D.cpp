#include <stdio.h>
#include <GL\glew.h>
#include "Object3D.h"

Object3D::Object3D()
{
    m_posistion = Vector3f(0.0f,0.0f,0.0f);
    m_rotation = Vector3f(0.0f,0.0f,0.0f);
    m_rotationSpeed = Vector3f(0.0f,0.0f,0.0f);
    
}

Object3D::~Object3D()
{
    if(m_vertices)
        delete [] m_vertices;
    m_vertices=NULL;
}

bool Object3D::InitObject(int numVertices)
{
    m_numVertices = numVertices;
    
    m_vertices=new ObjectVertex[m_numVertices];

    //Create normalisation cube map
    glGenTextures(1, &m_normalizationCubeMap);
    glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, m_normalizationCubeMap);
    GenerateNormalisationCubeMap();
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP_ARB, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return true;
}

//Calculates the light position in Object-Space
//Precondition: m_worldSpaceLightPosition is set.
//glMatrixMode is set to ModelView.
void Object3D::CalculateObjectSpaceLightPos()
{
    glPushMatrix(); //Push the current Matrix on the stack
    glLoadIdentity(); //Reset the matrix 

    GLfloat inverseModelViewMatrix[16];

    //Reverse order translate/rotate operations (We want the inverse matrix)   
    glRotatef(-m_rotation.z,0.0f,0.0f,1.0f); //Reverse Z-Axis rotation
    glRotatef(-m_rotation.y,0.0f,1.0f,0.0f); //Reverse Y-Axis rotation
    glRotatef(-m_rotation.x,1.0f,0.0f,0.0f); //Reverse X-Axis rotation
    glTranslatef(-m_posistion.x, -m_posistion.y, -m_posistion.z); //Move into position
    glGetFloatv(GL_MODELVIEW_MATRIX,inverseModelViewMatrix); //get the inverse model view matrix

    m_objectSpaceLightPosition = MuliplyMatrixByVector(Matrix4x4f(inverseModelViewMatrix), m_worldSpaceLightPosition);
    glPopMatrix();
}

void Object3D::GenerateNormalisationCubeMap()
{
    unsigned char * data=new unsigned char[32*32*3];
    if(!data)
    {
        printf("Unable to allocate memory for texture data for cube map\n");
    }

    //some useful variables
    int size=32;
    float offset=0.5f;
    float halfSize=16.0f;
    Vector3f tempVector;
    unsigned char * bytePtr;

    //positive x
    bytePtr=data;

    for(int j=0; j<size; j++)
    {
        for(int i=0; i<size; i++)
        {
            tempVector.x = halfSize;
            tempVector.y = -(j+offset-halfSize);
            tempVector.z =-(i+offset-halfSize);

            tempVector = Normalize(tempVector);
            tempVector = PackNormalToRGBImage(tempVector);

            bytePtr[0]=(unsigned char)(tempVector.x*255);
            bytePtr[1]=(unsigned char)(tempVector.y*255);
            bytePtr[2]=(unsigned char)(tempVector.z*255);

            bytePtr+=3;
        }
    }
    glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_X_ARB,
                    0, GL_RGBA8, 32, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    //negative x
    bytePtr=data;

    for(int j=0; j<size; j++)
    {
        for(int i=0; i<size; i++)
        {
            tempVector.x = halfSize;
            tempVector.y = -(j+offset-halfSize);
            tempVector.z =-(i+offset-halfSize);

            tempVector = Normalize(tempVector);
            tempVector = PackNormalToRGBImage(tempVector);

            bytePtr[0]=(unsigned char)(tempVector.x*255);
            bytePtr[1]=(unsigned char)(tempVector.y*255);
            bytePtr[2]=(unsigned char)(tempVector.z*255);

            bytePtr+=3;
        }
    }
    glTexImage2D(	GL_TEXTURE_CUBE_MAP_NEGATIVE_X_ARB,
                    0, GL_RGBA8, 32, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    //positive y
    bytePtr=data;

    for(int j=0; j<size; j++)
    {
        for(int i=0; i<size; i++)
        {
            tempVector.x = halfSize;
            tempVector.y = -(j+offset-halfSize);
            tempVector.z =-(i+offset-halfSize);

            tempVector = Normalize(tempVector);
            tempVector = PackNormalToRGBImage(tempVector);

            bytePtr[0]=(unsigned char)(tempVector.x*255);
            bytePtr[1]=(unsigned char)(tempVector.y*255);
            bytePtr[2]=(unsigned char)(tempVector.z*255);

            bytePtr+=3;
        }
    }
    glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_Y_ARB,
                    0, GL_RGBA8, 32, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    //negative y
    bytePtr=data;

    for(int j=0; j<size; j++)
    {
        for(int i=0; i<size; i++)
        {
            tempVector.x = halfSize;
            tempVector.y = -(j+offset-halfSize);
            tempVector.z =-(i+offset-halfSize);

            tempVector = Normalize(tempVector);
            tempVector = PackNormalToRGBImage(tempVector);

            bytePtr[0]=(unsigned char)(tempVector.x*255);
            bytePtr[1]=(unsigned char)(tempVector.y*255);
            bytePtr[2]=(unsigned char)(tempVector.z*255);

            bytePtr+=3;
        }
    }
    glTexImage2D(	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_ARB,
                    0, GL_RGBA8, 32, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    //positive z
    bytePtr=data;

    for(int j=0; j<size; j++)
    {
        for(int i=0; i<size; i++)
        {
            tempVector.x = halfSize;
            tempVector.y = -(j+offset-halfSize);
            tempVector.z =-(i+offset-halfSize);

            tempVector = Normalize(tempVector);
            tempVector = PackNormalToRGBImage(tempVector);

            bytePtr[0]=(unsigned char)(tempVector.x*255);
            bytePtr[1]=(unsigned char)(tempVector.y*255);
            bytePtr[2]=(unsigned char)(tempVector.z*255);

            bytePtr+=3;
        }
    }
    glTexImage2D(	GL_TEXTURE_CUBE_MAP_POSITIVE_Z_ARB,
                    0, GL_RGBA8, 32, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    //negative z
    bytePtr=data;

    for(int j=0; j<size; j++)
    {
        for(int i=0; i<size; i++)
        {
            tempVector.x = halfSize;
            tempVector.y = -(j+offset-halfSize);
            tempVector.z =-(i+offset-halfSize);

            tempVector = Normalize(tempVector);
            tempVector = PackNormalToRGBImage(tempVector);

            bytePtr[0]=(unsigned char)(tempVector.x*255);
            bytePtr[1]=(unsigned char)(tempVector.y*255);
            bytePtr[2]=(unsigned char)(tempVector.z*255);

            bytePtr+=3;
        }
    }
    glTexImage2D(	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_ARB,
                    0, GL_RGBA8, 32, 32, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    delete [] data;

}
