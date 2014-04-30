#include <stdio.h>
#include <math.h>
#include <GL\glut.h>
#include "Object3D.h"

Object3D::Object3D()
{
    m_isMorphing = false;
}

Object3D::~Object3D()
{
    if(m_vertices)
        delete [] m_vertices;
    m_vertices=NULL;
}

void Object3D::StartMorphing()
{
    m_isMorphing = true;
}
void Object3D::StopMorphing()
{
    m_isMorphing = false;
}

void Object3D::SetMorphTarget(Object3D *target)
{
    m_morphSource->ClonePoints(this); //Set the points of the Morphsource to this object
    m_morphTarget = target; //Set the new target as morph target
    m_currentMorphStep = 0;
    
    m_isMorphing = true;
}

void Object3D::SetMorphSteps(int numberOfSteps)
{
    if(numberOfSteps == 0)
    {
        m_currentMorphStep = 1000; //Default value
    }
    else if(m_morphSteps == 0)
    {
        m_currentMorphStep = numberOfSteps;
    }
    else
    {
        m_currentMorphStep = (m_currentMorphStep*numberOfSteps)/m_morphSteps;
    }

    m_morphSteps = numberOfSteps;

}

void Object3D::SetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat alpha)
{
    m_color[0] = r;
    m_color[1] = g;
    m_color[2] = b;
    m_color[3] = alpha;
}

int Object3D::GetNumberOfVertices()
{
    return m_numVertices;
}

ObjectVertex *Object3D::GetVertices()
{
    return &m_vertices[0];
}

bool Object3D::InitObject(int numVertices)
{
    m_numVertices = numVertices;
    m_vertices = new ObjectVertex[m_numVertices];

    return true;
}

bool Object3D::InitMorphSource()
{
    m_morphSource = new Object3D();
    return m_morphSource->InitObject(m_numVertices);
}


void Object3D::ClonePoints(Object3D *clone)
{
    //Copy the points
    for(int i=0;i<m_numVertices;i++)
    {
        m_vertices[i].m_position[0] = clone->m_vertices[i].m_position[0];
        m_vertices[i].m_position[1] = clone->m_vertices[i].m_position[1];
        m_vertices[i].m_position[2] = clone->m_vertices[i].m_position[2];
    }

}

void Object3D::RandomizeVertices()
{
    for(int i=0;i<m_numVertices;i++)                               // Loop Through All 468 Vertices
    {
        m_vertices[i].m_position[0]=((float)(rand()%14000)/1000)-7;          // morph4 x Point Becomes A Random Float Value From -7 to 7
        m_vertices[i].m_position[1]=((float)(rand()%14000)/1000)-7;          // morph4 y Point Becomes A Random Float Value From -7 to 7
        m_vertices[i].m_position[2]=((float)(rand()%14000)/1000)-7;          // morph4 z Point Becomes A Random Float Value From -7 to 7
    }

    m_morphSource->ClonePoints(this); //Set the points of the Morphsource to this object
    m_currentMorphStep = 0;

    m_isMorphing = true;
}

void Object3D::Draw()
{
    GLfloat displacement[3];
    // Begin Drawing Points
    glPointSize(3.0f);
    glBegin(GL_POINTS);  
    for(int i=0;i<m_numVertices;i++)
    {
        if(m_isMorphing)
        {
            if(m_morphTarget->m_numVertices = m_numVertices)
            {
                //x displacement = MorphSource Vertex[i] x - MorphTarget x Divided By Steps
                displacement[0] = (m_morphSource->m_vertices[i].m_position[0] - m_morphTarget->m_vertices[i].m_position[0]) / m_morphSteps;
                //y displacement = MorphSource Vertex[i] y - MorphTarget y Divided By Steps
                displacement[1] = (m_morphSource->m_vertices[i].m_position[1] - m_morphTarget->m_vertices[i].m_position[1]) / m_morphSteps;
                //z displacement = MorphSource Vertex[i] z - MorphTarget z Divided By Steps
                displacement[2] = (m_morphSource->m_vertices[i].m_position[2] - m_morphTarget->m_vertices[i].m_position[2]) / m_morphSteps;
                
                //move the object vertices
                m_vertices[i].m_position[0] -= displacement[0];
                m_vertices[i].m_position[1] -= displacement[1];
                m_vertices[i].m_position[2] -= displacement[2];
                
            }
        }
        else
        {
            displacement[0] = 0.0f;
            displacement[1] = 0.0f;
            displacement[2] = 0.0f;
        }

        // Draw A Point At The Current Temp Values (Vertex)
        glColor4f(m_color[0],m_color[1],m_color[2],m_color[3]);
        glVertex3f(m_vertices[i].m_position[0],
                   m_vertices[i].m_position[1],
                   m_vertices[i].m_position[2]);                       
                                        
    }   
    glEnd();                                    // Done Drawing Points

    
    if(m_isMorphing && //If the object is morphing
       m_currentMorphStep++ == m_morphSteps) //increment step and check if the morphing is complete.
    {
        m_isMorphing = false; //Stop morphing
        
        m_morphSource->ClonePoints(this); //Set the points of the Morphsource to this object
        m_currentMorphStep = 0;
        
    }


}

 
