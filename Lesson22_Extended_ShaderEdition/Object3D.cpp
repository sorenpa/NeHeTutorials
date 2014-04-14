#include <stdio.h>
#include <GL\glew.h>
#include "Object3D.h"

Object3D::Object3D()
{
    m_objectPosistion[0] = 0.0f;
    m_objectPosistion[1] = 0.0f;
    m_objectPosistion[2] = 0.0f;
    
    m_rotation[0] = 0.0f;
    m_rotation[1] = 0.0f;
    m_rotation[2] = 0.0f;
    
    m_rotationSpeed[0] = 0.0f;
    m_rotationSpeed[1] = 0.0f;
    m_rotationSpeed[2] = 0.0f;
    
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

    return true;
}

 void Object3D::SetShader(GLuint shaderID)
 {
     m_shaderProgram = shaderID;
     
     
     m_shd_mem_colorMap = glGetUniformLocationARB(m_shaderProgram,"colorMap");

     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_2D,m_colorMap);

     glUniform1iARB(m_shd_mem_colorMap, 0); //Link to texture 0

     m_shd_mem_normalMap = glGetUniformLocationARB(m_shaderProgram,"normalMap");

     glActiveTexture(GL_TEXTURE1);
     glBindTexture(GL_TEXTURE_2D,m_normalMap);

     glUniform1iARB(m_shd_mem_normalMap, 1);
     
 }

 void Object3D::SetColorMap(GLuint TextureID)
 {
     m_colorMap = TextureID;
     
 }

 void Object3D::SetNormalMap(GLuint TextureID)
 {
    m_normalMap = TextureID;
    
 }

 
