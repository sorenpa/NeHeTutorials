/*******************************************************************
Written by: Soren Peter Andersen
email: soren.p.andersen@gmail.com

Description: Header for a class containing generic values for a
             3D object such as vertices, position, etc.
*******************************************************************/

#ifndef OBJECT3D_H
#define OBJECT3D_H

class ObjectVertex
{
public:
    GLfloat m_position[3];
};

//Generic stuff for objects
class Object3D
{
public:
    


    //Constructor/Destructor
    Object3D();
    ~Object3D();

    //Accessors
    
    void StartMorphing();
    void StopMorphing();
    void SetMorphTarget(Object3D *target);
    void SetMorphSteps(int numberOfSteps);
    void SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat alpha);
    int GetNumberOfVertices();
    ObjectVertex *GetVertices();

    //Initializers
    bool InitObject(int _numVertices);
    bool InitMorphSource();

    //Util
    void ClonePoints(Object3D *clone);
    void RandomizeVertices();

    //Drawing
    void Draw();

    
private:
    //Vertices
    int m_numVertices; 
    ObjectVertex * m_vertices;
    
    GLfloat m_color[4];

    Object3D *m_morphSource; //Object morph into
    Object3D *m_morphTarget; //Object to morph into

    //Morphing
    bool m_isMorphing;
    int m_morphSteps;
    int m_currentMorphStep;
    
 
    
};


#endif	