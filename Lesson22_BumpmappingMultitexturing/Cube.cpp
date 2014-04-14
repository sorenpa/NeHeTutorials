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
    m_vertices[0].m_texCoords = Vector2f(1.0f,1.0f); 
    m_vertices[0].m_position = Vector3f(width/2.0f,height/2.0f,length/2.0f); 
    m_vertices[0].m_tTangent = Vector3f(1.0f,0.0f,0.0f); //t points +x direction
    m_vertices[0].m_bTangent = Vector3f(0.0f,1.0f,0.0f); //b points +y direction
    m_vertices[0].m_normal = Vector3f(0.0f,0.0f,1.0f); //n points +z direction
    //Top right vertex
    m_vertices[1].m_texCoords = Vector2f(0.0f,1.0f); 
    m_vertices[1].m_position = Vector3f(-width/2.0f,height/2.0f,length/2.0f); 
    m_vertices[1].m_tTangent = Vector3f(1.0f,0.0f,0.0f); //t points +x direction
    m_vertices[1].m_bTangent = Vector3f(0.0f,1.0f,0.0f); //b points +y direction
    m_vertices[1].m_normal = Vector3f(0.0f,0.0f,1.0f); //n points +z direction
    //Bottom right vertex
    m_vertices[2].m_texCoords = Vector2f(0.0f,0.0f);
    m_vertices[2].m_position = Vector3f(-width/2.0f,-height/2.0f,length/2.0f); 
    m_vertices[2].m_tTangent = Vector3f(1.0f,0.0f,0.0f); //t points +x direction
    m_vertices[2].m_bTangent = Vector3f(0.0f,1.0f,0.0f); //b points +y direction
    m_vertices[2].m_normal = Vector3f(0.0f,0.0f,1.0f); //n points +z direction
    //Botteom right vertex
    m_vertices[3].m_texCoords = Vector2f(1.0f,0.0f);
    m_vertices[3].m_position = Vector3f(width/2.0f,-height/2.0f,length/2.0f); 
    m_vertices[3].m_tTangent = Vector3f(1.0f,0.0f,0.0f); //t points +x direction
    m_vertices[3].m_bTangent = Vector3f(0.0f,1.0f,0.0f); //b points +y direction
    m_vertices[3].m_normal = Vector3f(0.0f,0.0f,1.0f); //n points +z direction

    //Specify face 2 (right)
    //Top left vertex
    m_vertices[4].m_texCoords = Vector2f(1.0f,1.0f); 
    m_vertices[4].m_position = Vector3f(width/2.0f,height/2.0f,-length/2.0f); 
    m_vertices[4].m_tTangent = Vector3f(0.0f,0.0f,-1.0f); //t points -z direction
    m_vertices[4].m_bTangent = Vector3f(0.0f,1.0f,0.0f); //b points +y direction
    m_vertices[4].m_normal = Vector3f(1.0f,0.0f,0.0f); //n points +x direction
    //Top right vertex
    m_vertices[5].m_texCoords = Vector2f(0.0f,1.0f); 
    m_vertices[5].m_position = Vector3f(width/2.0f,height/2.0f,length/2.0f); 
    m_vertices[5].m_tTangent = Vector3f(0.0f,0.0f,-1.0f); //t points -z direction
    m_vertices[5].m_bTangent = Vector3f(0.0f,1.0f,0.0f); //b points +y direction
    m_vertices[5].m_normal = Vector3f(1.0f,0.0f,0.0f); //n points +x direction
    //Bottom right vertex
    m_vertices[6].m_texCoords = Vector2f(0.0f,0.0f);
    m_vertices[6].m_position = Vector3f(width/2.0f,-height/2.0f,length/2.0f); 
    m_vertices[6].m_tTangent = Vector3f(0.0f,0.0f,-1.0f); //t points -z direction
    m_vertices[6].m_bTangent = Vector3f(0.0f,1.0f,0.0f); //b points +y direction
    m_vertices[6].m_normal = Vector3f(1.0f,0.0f,0.0f); //n points +x direction
    //Botteom right vertex
    m_vertices[7].m_texCoords = Vector2f(1.0f,0.0f);
    m_vertices[7].m_position = Vector3f(width/2.0f,-height/2.0f,-length/2.0f); 
    m_vertices[7].m_tTangent = Vector3f(0.0f,0.0f,-1.0f); //t points -z direction
    m_vertices[7].m_bTangent = Vector3f(0.0f,1.0f,0.0f); //b points +y direction
    m_vertices[7].m_normal = Vector3f(1.0f,0.0f,0.0f); //n points +x direction

    
    //Specify face 3 (back)
    //Top left vertex
    m_vertices[8].m_texCoords = Vector2f(1.0f,1.0f); 
    m_vertices[8].m_position = Vector3f(-width/2.0f,height/2.0f,-length/2.0f); 
    m_vertices[8].m_tTangent = Vector3f(-1.0f,0.0f,0.0f); //t points -x direction
    m_vertices[8].m_bTangent = Vector3f(0.0f,1.0f,0.0f); //b points +y direction
    m_vertices[8].m_normal = Vector3f(0.0f,0.0f,-1.0f); //n points -z direction
    //Top right vertex
    m_vertices[9].m_texCoords = Vector2f(0.0f,1.0f); 
    m_vertices[9].m_position = Vector3f(width/2.0f,height/2.0f,-length/2.0f); 
    m_vertices[9].m_tTangent = Vector3f(-1.0f,0.0f,0.0f); //t points -x direction
    m_vertices[9].m_bTangent = Vector3f(0.0f,1.0f,0.0f); //b points +y direction
    m_vertices[9].m_normal = Vector3f(0.0f,0.0f,-1.0f); //n points -z direction
    //Bottom right vertex
    m_vertices[10].m_texCoords = Vector2f(0.0f,0.0f);
    m_vertices[10].m_position = Vector3f(width/2.0f,-height/2.0f,-length/2.0f); 
    m_vertices[10].m_tTangent = Vector3f(-1.0f,0.0f,0.0f); //t points -x direction
    m_vertices[10].m_bTangent = Vector3f(0.0f,1.0f,0.0f); //b points +y direction
    m_vertices[10].m_normal = Vector3f(0.0f,0.0f,-1.0f); //n points -z direction
    //Botteom right vertex
    m_vertices[11].m_texCoords = Vector2f(1.0f,0.0f);
    m_vertices[11].m_position = Vector3f(-width/2.0f,-height/2.0f,-length/2.0f); 
    m_vertices[11].m_tTangent = Vector3f(-1.0f,0.0f,0.0f); //t points -x direction
    m_vertices[11].m_bTangent = Vector3f(0.0f,1.0f,0.0f); //b points +y direction
    m_vertices[11].m_normal = Vector3f(0.0f,0.0f,-1.0f); //n points -z direction

    //Specify face 4 (left)
    //Top left vertex
    m_vertices[12].m_texCoords = Vector2f(1.0f,1.0f); 
    m_vertices[12].m_position = Vector3f(-width/2.0f,height/2.0f,length/2.0f); 
    m_vertices[12].m_tTangent = Vector3f(0.0f,0.0f,1.0f); //t points +x direction
    m_vertices[12].m_bTangent = Vector3f(0.0f,1.0f,0.0f); //b points +y direction
    m_vertices[12].m_normal = Vector3f(-1.0f,0.0f,0.0f); //n points -z direction
    //Top right vertex
    m_vertices[13].m_texCoords = Vector2f(0.0f,1.0f); 
    m_vertices[13].m_position = Vector3f(-width/2.0f,height/2.0f,-length/2.0f); 
    m_vertices[13].m_tTangent = Vector3f(0.0f,0.0f,1.0f); //t points +x direction
    m_vertices[13].m_bTangent = Vector3f(0.0f,1.0f,0.0f); //b points +y direction
    m_vertices[13].m_normal = Vector3f(-1.0f,0.0f,0.0f); //n points -z direction
    //Bottom right vertex
    m_vertices[14].m_texCoords = Vector2f(0.0f,0.0f);
    m_vertices[14].m_position = Vector3f(-width/2.0f,-height/2.0f,-length/2.0f); 
    m_vertices[14].m_tTangent = Vector3f(0.0f,0.0f,1.0f); //t points +x direction
    m_vertices[14].m_bTangent = Vector3f(0.0f,1.0f,0.0f); //b points +y direction
    m_vertices[14].m_normal = Vector3f(-1.0f,0.0f,0.0f); //n points -z direction
    //Botteom right vertex
    m_vertices[15].m_texCoords = Vector2f(1.0f,0.0f);
    m_vertices[15].m_position = Vector3f(-width/2.0f,-height/2.0f,length/2.0f); 
    m_vertices[15].m_tTangent = Vector3f(0.0f,0.0f,1.0f); //t points +x direction
    m_vertices[15].m_bTangent = Vector3f(0.0f,1.0f,0.0f); //b points +y direction
    m_vertices[15].m_normal = Vector3f(-1.0f,0.0f,0.0f); //n points -z direction
    
    //Specify face 5 (top)
    //Top left vertex
    m_vertices[16].m_texCoords = Vector2f(1.0f,1.0f); 
    m_vertices[16].m_position = Vector3f(width/2.0f,height/2.0f,-length/2.0f); 
    m_vertices[16].m_tTangent = Vector3f(1.0f,0.0f,0.0f); //t points +x direction
    m_vertices[16].m_bTangent = Vector3f(0.0f,0.0f,-1.0f); //b points -z direction
    m_vertices[16].m_normal = Vector3f(0.0f,1.0f,0.0f); //n points +y direction
    //Top right vertex
    m_vertices[17].m_texCoords = Vector2f(0.0f,1.0f); 
    m_vertices[17].m_position = Vector3f(-width/2.0f,height/2.0f,-length/2.0f); 
    m_vertices[17].m_tTangent = Vector3f(1.0f,0.0f,0.0f); //t points +x direction
    m_vertices[17].m_bTangent = Vector3f(0.0f,0.0f,-1.0f); //b points -z direction
    m_vertices[17].m_normal = Vector3f(0.0f,1.0f,0.0f); //n points +y direction
    //Bottom right vertex
    m_vertices[18].m_texCoords = Vector2f(0.0f,0.0f);
    m_vertices[18].m_position = Vector3f(-width/2.0f,height/2.0f,length/2.0f); 
    m_vertices[18].m_tTangent = Vector3f(1.0f,0.0f,0.0f); //t points +x direction
    m_vertices[18].m_bTangent = Vector3f(0.0f,0.0f,-1.0f); //b points -z direction
    m_vertices[18].m_normal = Vector3f(0.0f,1.0f,0.0f); //n points +y direction
    //Botteom right vertex
    m_vertices[19].m_texCoords = Vector2f(1.0f,0.0f);
    m_vertices[19].m_position = Vector3f(width/2.0f,height/2.0f,length/2.0f); 
    m_vertices[19].m_tTangent = Vector3f(1.0f,0.0f,0.0f); //t points +x direction
    m_vertices[19].m_bTangent = Vector3f(0.0f,0.0f,-1.0f); //b points -z direction
    m_vertices[19].m_normal = Vector3f(0.0f,1.0f,0.0f); //n points +y direction

    //Specify face 6 (bottom)
    //Top left vertex
    m_vertices[20].m_texCoords = Vector2f(1.0f,1.0f); 
    m_vertices[20].m_position = Vector3f(width/2.0f,-height/2.0f,length/2.0f); 
    m_vertices[20].m_tTangent = Vector3f(1.0f,0.0f,0.0f); //t points +x direction
    m_vertices[20].m_bTangent = Vector3f(0.0f,0.0f,1.0f); //b points +z direction
    m_vertices[20].m_normal = Vector3f(0.0f,-1.0f,0.0f); //n points -y direction
    //Top right vertex
    m_vertices[21].m_texCoords = Vector2f(0.0f,1.0f); 
    m_vertices[21].m_position = Vector3f(-width/2.0f,-height/2.0f,length/2.0f); 
    m_vertices[21].m_tTangent = Vector3f(1.0f,0.0f,0.0f); //t points +x direction
    m_vertices[21].m_bTangent = Vector3f(0.0f,0.0f,1.0f); //b points +z direction
    m_vertices[21].m_normal = Vector3f(0.0f,-1.0f,0.0f); //n points -y direction
    //Bottom right vertex
    m_vertices[22].m_texCoords = Vector2f(0.0f,0.0f);
    m_vertices[22].m_position = Vector3f(-width/2.0f,-height/2.0f,-length/2.0f); 
    m_vertices[22].m_tTangent = Vector3f(1.0f,0.0f,0.0f); //t points +x direction
    m_vertices[22].m_bTangent = Vector3f(0.0f,0.0f,1.0f); //b points +z direction
    m_vertices[22].m_normal = Vector3f(0.0f,-1.0f,0.0f); //n points -y direction
    //Botteom right vertex
    m_vertices[23].m_texCoords = Vector2f(1.0f,0.0f);
    m_vertices[23].m_position = Vector3f(width/2.0f,-height/2.0f,-length/2.0f); 
    m_vertices[23].m_tTangent = Vector3f(1.0f,0.0f,0.0f); //t points +x direction
    m_vertices[23].m_bTangent = Vector3f(0.0f,0.0f,1.0f); //b points +z direction
    m_vertices[23].m_normal = Vector3f(0.0f,-1.0f,0.0f); //n points -y direction
}



//Implement the interface from Object3D. This method calculates the light position in tangent space.
void Object3D::CalculateTangenSpaceLightPos(ObjectVertex *v)
{
    //Construct the inverse tangent space matrix from the m_tTangent, m_bTangent and normal vectors for the vertex v.
    //the inverse tangent space matrix inverseTbnMatrix
    Matrix4x4f inverseTbnMatrix= Matrix4x4f(Vector4f(v->m_tTangent),Vector4f(v->m_bTangent),Vector4f(v->m_normal),Vector4f(0.0f,0.0f,0.0f,1.0f));

    //Multiply the objectspace light position into tangent space
    Vector4f temp = MuliplyMatrixByVector(inverseTbnMatrix, m_objectSpaceLightPosition);

    v->m_tangentSpaceLight = Normalize(Vector3f(temp.x,temp.y,temp.z));
}

void Object3D::DrawNormalMapPass()
{
    //Bind normal map to texture unit 0
        glBindTexture(GL_TEXTURE_2D, m_normalMap);
        glEnable(GL_TEXTURE_2D);

        //Bind normalisation cube map to texture unit 1
        glActiveTextureARB(GL_TEXTURE1_ARB);
        glBindTexture(GL_TEXTURE_CUBE_MAP_ARB, m_normalizationCubeMap);
        glEnable(GL_TEXTURE_CUBE_MAP_ARB);
        glActiveTextureARB(GL_TEXTURE0_ARB);

        //Set vertex arrays for cube
        glVertexPointer(3, GL_FLOAT, sizeof(ObjectVertex), &m_vertices[0].m_position);
        glEnableClientState(GL_VERTEX_ARRAY);

        //Send texture coords for normal map to unit 0
        glTexCoordPointer(2, GL_FLOAT, sizeof(ObjectVertex), &m_vertices[0].m_tTangent);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        //Send tangent space light vectors for normalisation to unit 1
        glClientActiveTextureARB(GL_TEXTURE1_ARB);
        glTexCoordPointer(3, GL_FLOAT, sizeof(ObjectVertex), &m_vertices[0].m_tangentSpaceLight);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glClientActiveTextureARB(GL_TEXTURE0_ARB);

        //Draw the cube
        glBegin(GL_QUADS);									
        //loop through vertices
        for(int v=0;v<m_numVertices;v++) //loop through the 4 vertexes for a face
        {
            glTexCoord2f(m_vertices[v].m_texCoords.x, m_vertices[v].m_texCoords.y); //Vertex texture coordinate
            glVertex3f(m_vertices[v].m_position.x, m_vertices[v].m_position.y, m_vertices[v].m_position.z); //Vertex position
        }
        glEnd();

        //Set up texture environment to do (tex0 dot tex1)*color
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_REPLACE);

        glActiveTextureARB(GL_TEXTURE1_ARB);

        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
        glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_DOT3_RGB_ARB);
        glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_PREVIOUS_ARB);

        glActiveTextureARB(GL_TEXTURE0_ARB);

        //Disable textures
        glDisable(GL_TEXTURE_2D);

        glActiveTextureARB(GL_TEXTURE1_ARB);
        glDisable(GL_TEXTURE_CUBE_MAP_ARB);
        glActiveTextureARB(GL_TEXTURE0_ARB);

        //disable vertex arrays
        glDisableClientState(GL_VERTEX_ARRAY);

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);

        glClientActiveTextureARB(GL_TEXTURE1_ARB);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glClientActiveTextureARB(GL_TEXTURE0_ARB);

        //Return to standard modulate texenv
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

void Object3D::Draw()
{
    glMatrixMode( GL_MODELVIEW );  // Select The Model View Matrix
    glPushMatrix(); //Store current matrix in the stack
    glLoadIdentity(); //Restore the model view matrix
    
    
    //calculate and set ObjectSpace lightpos
    CalculateObjectSpaceLightPos();
    

    glTranslatef(m_posistion.x, m_posistion.y, m_posistion.z); //Move into position
    glRotatef(m_rotation.x,1.0f,0.0f,0.0f); //X-Axis rotation
    glRotatef(m_rotation.y,0.0f,1.0f,0.0f); //Y-Axis rotation
    glRotatef(m_rotation.z,0.0f,0.0f,1.0f); //Z-Axis rotation
    
    
    //Setup tangenspace light vectors
    for(int i=0;i<m_numVertices;i++)
    {
        CalculateTangenSpaceLightPos(&m_vertices[i]); //sets the v.m_tangentSpaceMatrix
    }

    //Draw bump pass
    if(m_drawNormalMap)
    {
        DrawNormalMapPass();
    }
    

    GLfloat white[] = {1.0f,1.0f,1.0f};
    //Enable Lighting
    glEnable(GL_LIGHT1);
    glEnable(GL_LIGHTING);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, white);

    
    //If we are drawing both passes, enable blending to multiply them together
    if(m_drawNormalMap && m_drawColorMap)
    {
        //Enable multiplicative blending
        glBlendFunc(GL_DST_COLOR, GL_ZERO);
        glEnable(GL_BLEND);
    }

    //Perform a second pass to color the cube
    if(m_drawColorMap)
    {
    
        //Bind decal texture
        glBindTexture(GL_TEXTURE_2D, m_colorMap);
        glEnable(GL_TEXTURE_2D);

        //Set vertex arrays for cube
        glVertexPointer(3, GL_FLOAT, sizeof(ObjectVertex), &m_vertices[0].m_position);
        glEnableClientState(GL_VERTEX_ARRAY);

        glNormalPointer(GL_FLOAT, sizeof(ObjectVertex), &m_vertices[0].m_normal);
        glEnableClientState(GL_NORMAL_ARRAY);

        glTexCoordPointer(2, GL_FLOAT, sizeof(ObjectVertex), &m_vertices[0].m_tTangent);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

       //Draw the cube
        glBegin(GL_QUADS);									
        //loop through vertices
        for(int v=0;v<m_numVertices;v++) //loop through the 4 vertexes for a face
        {
            glTexCoord2f(m_vertices[v].m_texCoords.x, m_vertices[v].m_texCoords.y); //Vertex texture coordinate
            glVertex3f(m_vertices[v].m_position.x, m_vertices[v].m_position.y, m_vertices[v].m_position.z); //Vertex position
        }
        glEnd();

    
        //Disable texture
        glDisable(GL_TEXTURE_2D);

        //disable vertex arrays
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }

    //Disable blending if it is enabled
    if(m_drawNormalMap && m_drawColorMap)
    {
        glDisable(GL_BLEND);
    }
    
    glPopMatrix(); //Reload the matrix from the stack

    m_rotation.x += m_rotationSpeed.x;
    m_rotation.y += m_rotationSpeed.y;
    m_rotation.z += m_rotationSpeed.z;
}