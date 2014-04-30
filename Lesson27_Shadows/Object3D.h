/*******************************************************************
Written by: Soren Peter Andersen
email: soren.p.andersen@gmail.com

Description: Header for a class containing generic values for a
             3D object such as vertices, position, etc.
*******************************************************************/

#ifndef OBJECT3D_H
#define OBJECT3D_H

// Definition Of "INFINITY" For Calculating The Extension Vector For The Shadow Volume
#define INFINITY    100

class ObjectVertex
{
public:
    GLfloat m_position[3];
};

// Structure Describing A Plane, In The Format: ax + by + cz + d = 0
class PlaneEquation
{
public:
    GLfloat a, b, c, d;
};

class ObjectFace
{
public:
    ObjectFace();
    int vertexIndices[3];           // Index Of Each Vertex Within An Object That Makes Up The Triangle Of This Face
    ObjectVertex normals[3];         // Normals To Each Vertex
    PlaneEquation planeEquation;            // Equation Of A Plane That Contains This Triangle
    int neighbourIndices[3];        // Index Of Each Face That Neighbours This One Within The Object
    bool visible;               // Is The Face Visible By The Light?
};



//Generic stuff for objects
class Object3D
{
public:
    


    //Constructor/Destructor
    Object3D();
    ~Object3D();

    //Accessors
    void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat alpha);
    int GetNumberOfVertices();
    ObjectVertex *GetVertices();
    int GetNumberOfFaces();
    ObjectFace *GetFaces();
    void SetPosition(GLfloat x, GLfloat y, GLfloat z);
    GLfloat *GetPosition();
    void SetLightPosition(GLfloat *lightPos);

    //Initializers
    bool InitObjectVertices(int _numVertices);
    bool InitObjectFaces(int _numFaces);

    //Util
    void SetConnectivity(void);
    void CalculatePlane( int faceIndex );
    void CalculateAllPlanes();
    void CastShadow();
    void RunShadowPass();
    //Drawing
    void Draw();

    
private:
    //Vertices
    int m_numVertices; 
    GLfloat m_lightPosition[4]; //object space light pos.
    ObjectVertex *m_vertices;

    GLfloat m_position[3];

    int m_numFaces;
    ObjectFace *m_faces;               // Will Be Dynamically Allocated
    
    GLfloat m_color[4];


    
 
    
};



#endif	