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
    GLfloat m_texCoords[2];
    GLfloat m_tTangent[3];
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

    void SetShader(GLuint shaderID);
    void SetColorMap(GLuint TextureID);
    void SetNormalMap(GLuint TextureID);

    //Position/Rotation
    GLfloat m_objectPosistion[3];
    GLfloat m_rotation[3]; //[x-angle,y-angle,z-angle]
    GLfloat m_rotationSpeed[3]; //rotation angles per frame
 
protected:
    //Vertices
    int m_numVertices; 
    ObjectVertex * m_vertices;
    
    //Textures
    GLuint m_colorMap; //Color texture map
    GLuint m_normalMap; //Normal texture map

    //Shaders
    GLuint m_shaderProgram; //Shader program ID
    GLfloat m_vTangent[3];
    GLint m_shd_mem_vTangent; //memory location for the texture coordinate variable used by the shader.
    GLint m_shd_mem_colorMap; //memory location for the texture map ID
    GLint m_shd_mem_normalMap; //memory location for the norml map ID

    //init function
    bool InitObject(int _numVertices);
    void initShader(void);
};


#endif	