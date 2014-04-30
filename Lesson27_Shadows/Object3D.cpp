#include <stdio.h>
#include <math.h>
#include <GL\glut.h>
#include "Object3D.h"

ObjectFace::ObjectFace()
{
    neighbourIndices[0] = -1;//unset
    neighbourIndices[1] = -1;//unset
    neighbourIndices[2] = -1;//unset
}

Object3D::Object3D()
{ 
    m_position[0]=0.0f;
    m_position[1]=0.0f;
    m_position[2]=0.0f;
}

Object3D::~Object3D()
{
    if(m_vertices)
        delete [] m_vertices;
    m_vertices=NULL;
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

int Object3D::GetNumberOfFaces()
{
    return m_numFaces;
}

ObjectFace *Object3D::GetFaces()
{
    return &m_faces[0];
}

void Object3D::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
    m_position[0]=x;
    m_position[1]=y;
    m_position[2]=z;
}

GLfloat *Object3D::GetPosition()
{
    return &m_position[0];
}

void Object3D::SetLightPosition(GLfloat *lightPos)
{
    m_lightPosition[0] = lightPos[0];
    m_lightPosition[1] = lightPos[1];
    m_lightPosition[2] = lightPos[2];
    m_lightPosition[3] = lightPos[3];
}

bool Object3D::InitObjectVertices(int numVertices)
{
    m_numVertices = numVertices;
    m_vertices = new ObjectVertex[m_numVertices];

    return true;
}

bool Object3D::InitObjectFaces(int numFaces)
{
    m_numFaces = numFaces;
    m_faces = new ObjectFace[m_numFaces];

    return true;
}

void Object3D::SetConnectivity()
{
    for(int fa=0;fa<m_numFaces;fa++) //for each face
    {
        for(int ea=0;ea<3;ea++)
        {
            //Check if the face has a neighbour (are they initialized to -1)
            if(m_faces[fa].neighbourIndices[ea] == -1)
            {
                for(int fb=0;fb<m_numFaces;fb++)
                {
                    if(fa!=fb)
                    {
                        for(int eb=0;eb<3;eb++)
                        {
                            int vertA1 = m_faces[fa].vertexIndices[ea];
                            int vertA2 = m_faces[fa].vertexIndices[(ea+1 )%3];
 
                            int vertB1 = m_faces[fb].vertexIndices[eb];
                            int vertB2 = m_faces[fb].vertexIndices[(eb+1 )%3];
 
                            // Check If They Are Neighbours - IE, The Edges Are The Same
                            if (( vertA1 == vertB1 && vertA2 == vertB2 ) || ( vertA1 == vertB2 && vertA2 == vertB1 ))
                            {
                                m_faces[fa].neighbourIndices[ea] = fb;
                                m_faces[fb].neighbourIndices[eb] = fa;
                                break;
                            }
                        }
                    }
                }
            }
        }
    }

                
                    
}

void Object3D::CalculatePlane( int faceIndex )
{
    // Get Shortened Names For The Vertices Of The Face
    const ObjectVertex& v1 = m_vertices[m_faces[faceIndex].vertexIndices[0]];
    const ObjectVertex& v2 = m_vertices[m_faces[faceIndex].vertexIndices[1]];
    const ObjectVertex& v3 = m_vertices[m_faces[faceIndex].vertexIndices[2]];
 
    m_faces[faceIndex].planeEquation.a = v1.m_position[1]*(v2.m_position[2]-v3.m_position[2]) + v2.m_position[1]*(v3.m_position[2]-v1.m_position[2]) + v3.m_position[1]*(v1.m_position[2]-v2.m_position[2]);
    m_faces[faceIndex].planeEquation.b = v1.m_position[2]*(v2.m_position[0]-v3.m_position[0]) + v2.m_position[2]*(v3.m_position[0]-v1.m_position[0]) + v3.m_position[2]*(v1.m_position[0]-v2.m_position[0]);
    m_faces[faceIndex].planeEquation.c = v1.m_position[0]*(v2.m_position[1]-v3.m_position[1]) + v2.m_position[0]*(v3.m_position[1]-v1.m_position[1]) + v3.m_position[0]*(v1.m_position[1]-v2.m_position[1]);
    m_faces[faceIndex].planeEquation.d = -( v1.m_position[0]*( v2.m_position[1]*v3.m_position[2] - v3.m_position[1]*v2.m_position[2] ) +
                v2.m_position[0]*(v3.m_position[1]*v1.m_position[2] - v1.m_position[1]*v3.m_position[2]) +
                v3.m_position[0]*(v1.m_position[1]*v2.m_position[2] - v2.m_position[1]*v1.m_position[2]) );
}

void Object3D::CalculateAllPlanes()
{
    for(int i=0;i<m_numFaces;i++)
    {
        CalculatePlane(i);
    }
}

void Object3D::RunShadowPass()
{
    for ( int i = 0; i < m_numFaces; i++ )
    {
        const ObjectFace& face = m_faces[i];
 
        if ( face.visible )
        {
            // Go Through Each Edge
            for ( int j = 0; j < 3; j++ )
            {
                int neighbourIndex = face.neighbourIndices[j];
                // If There Is No Neighbour, Or Its Neighbouring Face Is Not Visible, Then This Edge Casts A Shadow
                if ( neighbourIndex == -1 || m_faces[neighbourIndex].visible == false )
                {
                    // Get The Points On The Edge
                    const ObjectVertex& v1 = m_vertices[face.vertexIndices[j]];
                    const ObjectVertex& v2 = m_vertices[face.vertexIndices[( j+1 )%3]];
 
                    // Calculate The Two Vertices In Distance
                    ObjectVertex v3, v4;
 
                    v3.m_position[0] = ( v1.m_position[0]-m_lightPosition[0] )*INFINITY;
                    v3.m_position[1] = ( v1.m_position[1]-m_lightPosition[1] )*INFINITY;
                    v3.m_position[2] = ( v1.m_position[2]-m_lightPosition[2] )*INFINITY;
 
                    v4.m_position[0] = ( v2.m_position[0]-m_lightPosition[0] )*INFINITY;
                    v4.m_position[1] = ( v2.m_position[1]-m_lightPosition[1] )*INFINITY;
                    v4.m_position[2] = ( v2.m_position[2]-m_lightPosition[2] )*INFINITY;
                   // Draw The Quadrilateral (As A Triangle Strip)
                    glBegin( GL_TRIANGLE_STRIP );
                        glVertex3f( v1.m_position[0], v1.m_position[1], v1.m_position[2] );
                        glVertex3f( v1.m_position[0]+v3.m_position[0], v1.m_position[1]+v3.m_position[1], v1.m_position[2]+v3.m_position[2] );
                        glVertex3f( v2.m_position[0], v2.m_position[1], v2.m_position[2] );
                        glVertex3f( v2.m_position[0]+v4.m_position[0], v2.m_position[1]+v4.m_position[1], v2.m_position[2]+v4.m_position[2] );
                    glEnd();
                }
            }
        }
    }
}

void Object3D::CastShadow()
{
    // Determine Which Faces Are Visible By The Light.
    for ( int i = 0; i < m_numFaces; i++ )
    {
        const PlaneEquation& plane = m_faces[i].planeEquation;
 
        GLfloat side = plane.a*m_lightPosition[0]+
            plane.b*m_lightPosition[1]+
            plane.c*m_lightPosition[2]+
            plane.d;
 
        if ( side > 0 )
            m_faces[i].visible = true;
        else
            m_faces[i].visible = false;
    }

    glPushAttrib( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ENABLE_BIT | GL_POLYGON_BIT | GL_STENCIL_BUFFER_BIT );
    glDisable( GL_LIGHTING );                   // Turn Off Lighting
    glDepthMask( GL_FALSE );                    // Turn Off Writing To The Depth-Buffer
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_STENCIL_TEST );                    // Turn On Stencil Buffer Testing
    glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );      // Don't Draw Into The Colour Buffer
    glStencilFunc( GL_ALWAYS, 1, 0xFFFFFFFFL );

    // First Pass. Increase Stencil Value In The Shadow
    glFrontFace( GL_CCW );
    glStencilOp( GL_KEEP, GL_KEEP, GL_INCR );
    RunShadowPass();
    
    // Second Pass. Decrease Stencil Value In The Shadow - This is what we call carmacks reverse
    glFrontFace( GL_CW );
    glStencilOp( GL_KEEP, GL_KEEP, GL_DECR );
    RunShadowPass();
    
    glFrontFace( GL_CCW );
    glColorMask( GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE );  // Enable Rendering To Colour Buffer For All Components
 
    //glDisable(GL_STENCIL_TEST);

    // Draw A Shadowing Rectangle Covering The Entire Screen
    glColor4f( 0.0f, 0.0f, 0.0f, 0.4f );
    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glStencilFunc( GL_NOTEQUAL, 0, 0xFFFFFFFFL );
    glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
    glPushMatrix();
    glLoadIdentity();
    glBegin( GL_TRIANGLE_STRIP );
        glVertex3f(-5.0f, 5.0f,-1.00f);
        glVertex3f(-5.0f,-5.0f,-1.00f);
        glVertex3f( 5.0f, 5.0f,-1.00f);
        glVertex3f( 5.0f,-5.0f,-1.00f);
    glEnd();
    glPopMatrix();
    glPopAttrib();
}

void Object3D::Draw()
{
    
    glBegin( GL_TRIANGLES );
    for ( int i = 0; i < m_numFaces; i++ )
    {
        const ObjectFace& face = m_faces[i];
 
        for ( int j = 0; j < 3; j++ )
        {
            const ObjectVertex& vertex = m_vertices[face.vertexIndices[j]];
 
            glNormal3f( face.normals[j].m_position[0], face.normals[j].m_position[1], face.normals[j].m_position[2] );
            glVertex3f( vertex.m_position[0], vertex.m_position[1], vertex.m_position[2] );
        }
    }
    glEnd();

}

 
