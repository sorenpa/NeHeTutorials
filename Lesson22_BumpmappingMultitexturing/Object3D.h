/*******************************************************************
Written by: Soren Peter Andersen
email: soren.p.andersen@gmail.com

Description: Header for a class containing generic values for a
             3D object such as vertices, position, etc.
*******************************************************************/
#include "mathlib.h"

#ifndef OBJECT3D_H
#define OBJECT3D_H

class ObjectVertex
{
public:
    Vector3f m_position;
    Vector2f m_texCoords;
    Vector3f m_tTangent; 
    Vector3f m_bTangent;
    Vector3f m_normal;
    Vector3f m_tangentSpaceLight;
};

//Generic stuff for objects
class Object3D
{
public:
    
    //Constructor/Destructor
    Object3D();
    ~Object3D();

    //Draw function
    void Draw();

    //Position/Rotation
    Vector3f m_posistion;
    Vector3f m_rotation; //[x-angle,y-angle,z-angle]
    Vector3f m_rotationSpeed; //rotation angles per frame
    //Lights
    Vector4f m_worldSpaceLightPosition;
    Vector4f m_objectSpaceLightPosition;
    //Vertices
    int m_numVertices; 
    ObjectVertex * m_vertices;
    //Textures
    GLuint m_colorMap; //Color texture map
    GLuint m_normalMap; //Normal texture map
    GLuint m_normalizationCubeMap; //storage for a cubemap to do texturing
    bool m_drawNormalMap;
    bool m_drawColorMap;

    //init function
    bool InitObject(int _numVertices);

protected:
    //Calculates the light position in the tangent space for a given vertex
    void CalculateObjectSpaceLightPos();
    void CalculateTangenSpaceLightPos(ObjectVertex *v);
    void GenerateNormalisationCubeMap();
    void DrawNormalMapPass();

    
};


#endif	