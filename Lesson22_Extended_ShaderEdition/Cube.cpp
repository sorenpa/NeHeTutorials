/*******************************************************************
Written by: Soren Peter Andersen
email: soren.p.andersen@gmail.com

Description: Class representing a cube in 3D space
*******************************************************************/

#include <stdio.h>
#include <GL\glew.h>
#include <GL\SOIL.h>
#include "Cube.h"




Cube::Cube(GLfloat height, GLfloat length, GLfloat width) 
{

    if(!InitObject(24))
    {
        printf("Unable to allocate memory for cube vertices\n");
    }
    

    ConstructCube(height, length, width);
}

Cube::~Cube()
{
    Object3D::~Object3D();
}

void Cube::ConstructCube(GLfloat height, GLfloat length, GLfloat width)
{

    //Specify face 1 (front)
    //Top left vertex
    m_vertices[0].m_texCoords[0] = 1.0f;
    m_vertices[0].m_texCoords[1] = 1.0f;
    m_vertices[0].m_position[0] = width/2.0f;
    m_vertices[0].m_position[1] = height/2.0f;
    m_vertices[0].m_position[2] = length/2.0f;
    //Top right vertex
    m_vertices[1].m_texCoords[0] = 0.0f;
    m_vertices[1].m_texCoords[1] = 1.0f;
    m_vertices[1].m_position[0] = -width/2.0f;
    m_vertices[1].m_position[1] = height/2.0f;
    m_vertices[1].m_position[2] = length/2.0f;

    //Bottom right vertex
    m_vertices[2].m_texCoords[0] = 0.0f;
    m_vertices[2].m_texCoords[1] = 0.0f;
    m_vertices[2].m_position[0] = -width/2.0f;
    m_vertices[2].m_position[1] = -height/2.0f;
    m_vertices[2].m_position[2] = length/2.0f;
    //Botteom right vertex
    m_vertices[3].m_texCoords[0] = 1.0f;
    m_vertices[3].m_texCoords[1] = 0.0f;
    m_vertices[3].m_position[0] = width/2.0f;
    m_vertices[3].m_position[1] = -height/2.0f;
    m_vertices[3].m_position[2] = length/2.0f;
    //Specify face 2 (right)
    //Top left vertex
    m_vertices[4].m_texCoords[0] = 1.0f;
    m_vertices[4].m_texCoords[1] = 1.0f;
    m_vertices[4].m_position[0] = width/2.0f;
    m_vertices[4].m_position[1] = height/2.0f;
    m_vertices[4].m_position[2] = -length/2.0f;
    //Top right vertex
    m_vertices[5].m_texCoords[0] = 0.0f;
    m_vertices[5].m_texCoords[1] = 1.0f;
    m_vertices[5].m_position[0] = width/2.0f;
    m_vertices[5].m_position[1] = height/2.0f;
    m_vertices[5].m_position[2] = length/2.0f;
    //Bottom right vertex
    m_vertices[6].m_texCoords[0] = 0.0f;
    m_vertices[6].m_texCoords[1] = 0.0f;
    m_vertices[6].m_position[0] = width/2.0f;
    m_vertices[6].m_position[1] = -height/2.0f;
    m_vertices[6].m_position[2] = length/2.0f;
    //Botteom right vertex
    m_vertices[7].m_texCoords[0] = 1.0f;
    m_vertices[7].m_texCoords[1] = 0.0f;
    m_vertices[7].m_position[0] = width/2.0f;
    m_vertices[7].m_position[1] = -height/2.0f;
    m_vertices[7].m_position[2] = -length/2.0f;

    //Specify face 3 (back)
    //Top left vertex
    m_vertices[8].m_texCoords[0] = 1.0f;
    m_vertices[8].m_texCoords[1] = 1.0f;
    m_vertices[8].m_position[0] = -width/2.0f;
    m_vertices[8].m_position[1] = height/2.0f;
    m_vertices[8].m_position[2] = -length/2.0f;
    //Top right vertex
    m_vertices[9].m_texCoords[0] = 0.0f;
    m_vertices[9].m_texCoords[1] = 1.0f;
    m_vertices[9].m_position[0] = width/2.0f;
    m_vertices[9].m_position[1] = height/2.0f;
    m_vertices[9].m_position[2] = -length/2.0f;
    //Bottom right vertex
    m_vertices[10].m_texCoords[0] = 0.0f;
    m_vertices[10].m_texCoords[1] = 0.0f;
    m_vertices[10].m_position[0] = width/2.0f;
    m_vertices[10].m_position[1] = -height/2.0f;
    m_vertices[10].m_position[2] = -length/2.0f;
    //Botteom right vertex
    m_vertices[11].m_texCoords[0] = 1.0f;
    m_vertices[11].m_texCoords[1] = 0.0f;
    m_vertices[11].m_position[0] = -width/2.0f;
    m_vertices[11].m_position[1] = -height/2.0f;
    m_vertices[11].m_position[2] = -length/2.0f;

    //Specify face 4 (left)
    //Top left vertex
    m_vertices[12].m_texCoords[0] = 1.0f;
    m_vertices[12].m_texCoords[1] = 1.0f;
    m_vertices[12].m_position[0] = -width/2.0f;
    m_vertices[12].m_position[1] = height/2.0f;
    m_vertices[12].m_position[2] = length/2.0f;
    //Top right vertex
    m_vertices[13].m_texCoords[0] = 0.0f;
    m_vertices[13].m_texCoords[1] = 1.0f;
    m_vertices[13].m_position[0] = -width/2.0f;
    m_vertices[13].m_position[1] = height/2.0f;
    m_vertices[13].m_position[2] = -length/2.0f;
    //Bottom right vertex
    m_vertices[14].m_texCoords[0] = 0.0f;
    m_vertices[14].m_texCoords[1] = 0.0f;
    m_vertices[14].m_position[0] = -width/2.0f;
    m_vertices[14].m_position[1] = -height/2.0f;
    m_vertices[14].m_position[2] = -length/2.0f;
    //Botteom right vertex
    m_vertices[15].m_texCoords[0] = 1.0f;
    m_vertices[15].m_texCoords[1] = 0.0f;
    m_vertices[15].m_position[0] = -width/2.0f;
    m_vertices[15].m_position[1] = -height/2.0f;
    m_vertices[15].m_position[2] = length/2.0f;
    
    //Specify face 5 (top)
    //Top left vertex
    m_vertices[16].m_texCoords[0] = 1.0f;
    m_vertices[16].m_texCoords[1] = 1.0f;
    m_vertices[16].m_position[0] = width/2.0f;
    m_vertices[16].m_position[1] = height/2.0f;
    m_vertices[16].m_position[2] = -length/2.0f;
    //Top right vertex
    m_vertices[17].m_texCoords[0] = 0.0f;
    m_vertices[17].m_texCoords[1] = 1.0f;
    m_vertices[17].m_position[0] = -width/2.0f;
    m_vertices[17].m_position[1] = height/2.0f;
    m_vertices[17].m_position[2] = -length/2.0f;
    //Bottom right vertex
    m_vertices[18].m_texCoords[0] = 0.0f;
    m_vertices[18].m_texCoords[1] = 0.0f;
    m_vertices[18].m_position[0] = -width/2.0f;
    m_vertices[18].m_position[1] = height/2.0f;
    m_vertices[18].m_position[2] = length/2.0f;

    //Botteom right vertex
    m_vertices[19].m_texCoords[0] = 1.0f;
    m_vertices[19].m_texCoords[1] = 0.0f;
    m_vertices[19].m_position[0] = width/2.0f;
    m_vertices[19].m_position[1] = height/2.0f;
    m_vertices[19].m_position[2] = length/2.0f;

    //Specify face 6 (bottom)
    //Top left vertex
    m_vertices[20].m_texCoords[0] = 1.0f;
    m_vertices[20].m_texCoords[1] = 1.0f;
    m_vertices[20].m_position[0] = width/2.0f;
    m_vertices[20].m_position[1] = -height/2.0f;
    m_vertices[20].m_position[2] = length/2.0f;
    //Top right vertex
    m_vertices[21].m_texCoords[0] = 0.0f;
    m_vertices[21].m_texCoords[1] = 1.0f;
    m_vertices[21].m_position[0] = -width/2.0f;
    m_vertices[21].m_position[1] = -height/2.0f;
    m_vertices[21].m_position[2] = length/2.0f;
    //Bottom right vertex
    m_vertices[22].m_texCoords[0] = 0.0f;
    m_vertices[22].m_texCoords[1] = 0.0f;
    m_vertices[22].m_position[0] = -width/2.0f;
    m_vertices[22].m_position[1] = -height/2.0f;
    m_vertices[22].m_position[2] = -length/2.0f;
    //Botteom right vertex
    m_vertices[23].m_texCoords[0] = 1.0f;
    m_vertices[23].m_texCoords[1] = 0.0f;
    m_vertices[23].m_position[0] = width/2.0f;
    m_vertices[23].m_position[1] = -height/2.0f;
    m_vertices[23].m_position[2] = -length/2.0f;
}



//Implement the interface from Object3D. This method calculates the light position in tangent space.


void Object3D::Draw()
{
    glMatrixMode( GL_MODELVIEW );  // Select The Model View Matrix
    
    glPushMatrix(); //Store current matrix in the stack
    glLoadIdentity(); //Restore the model view matrix


    
    //Use the shader program
    glUseProgram(m_shaderProgram);

    
    glTranslatef(m_objectPosistion[0], m_objectPosistion[1], m_objectPosistion[2]); //Move into position
    glRotatef(m_rotation[0],1.0f,0.0f,0.0f); //X-Axis rotation
    glRotatef(m_rotation[1],0.0f,1.0f,0.0f); //Y-Axis rotation
    glRotatef(m_rotation[2],0.0f,0.0f,1.0f); //Z-Axis rotation

    
    //Draw the cube
    glBegin(GL_QUADS);									
    //loop through vertices
    for(int v=0;v<m_numVertices;v++) //loop through the 4 vertexes for a face
        {
            glTexCoord2f(m_vertices[v].m_texCoords[0], m_vertices[v].m_texCoords[1]); //Vertex texture coordinate
            glVertex3f(m_vertices[v].m_position[0], m_vertices[v].m_position[1], m_vertices[v].m_position[2]); //Vertex position
        }
    glEnd();

    glPopMatrix(); //Reload the matrix from the stack
    
    m_rotation[0] += m_rotationSpeed[0]; //x
    m_rotation[1] += m_rotationSpeed[1]; //y
    m_rotation[2] += m_rotationSpeed[3]; //z
}