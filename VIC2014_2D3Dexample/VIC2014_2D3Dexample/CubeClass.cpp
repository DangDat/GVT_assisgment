/****		Cube in points-polygons (polyhedron) form	****/

#include "CubeClass.h"
#include "vec3.h"

static GLfloat Verts[8][3] = {
  { -0.5,  0.5, -0.5 }, /* 0 left top rear */
  {  0.5,  0.5, -0.5 },	/* 1 right top rear */
  {  0.5, -0.5, -0.5 },	/* 2 right bottom rear */
  { -0.5, -0.5, -0.5 },	/* 3 left bottom rear */
  { -0.5,  0.5,  0.5 },	/* 4 left top front */
  {  0.5,  0.5,  0.5 },	/* 5 right top front */
  {  0.5, -0.5,  0.5 },	/* 6 right bottom front */
  { -0.5, -0.5,  0.5 }	/* 7 left bottom front */
};

static GLuint Faces[6][4] = {
  4, 5, 6, 7,	/* front */
  5, 1, 2, 6,	/* right */
  0, 4, 7, 3,	/* left */
  4, 0, 1, 5,	/* top */
  7, 6, 2, 3,	/* bottom */
  1, 0, 3, 2	/* rear */
};

//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//  glColor3f(0.0, 1.0, 1.0);
//  glLineWidth(3);

vec3 pnormal; // temp for polygon normal
vec3 pnormalEnd1; // temp for polygon normal render
vec3 pnormalEnd2; // temp for polygon normal render

vec3 p0;
vec3 p1;
vec3 p2;
vec3 p3;

vec3 temp0;
vec3 temp1;

bool renderNormal;

CubeClass::CubeClass()
{
	renderNormal=false;
}

void CubeClass::renderCW()
{
  int v;
  /* Draw cube in traditional OpenGL style */
 
	for (int face = 0; face < 6; face++) 
		{
		v= Faces[face][0]; // this line is supposed to make things simpler
	    p0.x = Verts[v][0]; //x
		p0.y = Verts[v][1]; //y
		p0.z = Verts[v][2]; //z

		v= Faces[face][1]; // this line is supposed to make things simpler
		p1.x =Verts[v][0]; //x
		p1.y =Verts[v][1]; //y
		p1.z =Verts[v][2]; //z

		v= Faces[face][2]; // this line is supposed to make things simpler
		p2.x =Verts[v][0]; //x
		p2.y =Verts[v][1]; //y
		p2.z =Verts[v][2]; //z

		v= Faces[face][3]; // this line is supposed to make things simpler
		p3.x =Verts[v][0]; //x
		p3.y =Verts[v][1]; //y
		p3.z =Verts[v][2]; //z

		temp0 = p0-p1;
        temp1 = p0-p2;

		polygonNormal(pnormal,temp0,temp1);

		pnormalEnd1.x = (p0.x +p1.x +p2.x +p3.x)/4.0f;
		pnormalEnd1.y = (p0.y +p1.y +p2.y +p3.y)/4.0f;
		pnormalEnd1.z = (p0.z +p1.z +p2.z +p3.z)/4.0f;

		pnormalEnd2 = pnormalEnd1+pnormal;

    glBegin(GL_QUADS);
			glVertex3fv(p0.fv());
			glVertex3fv(p1.fv());
			glVertex3fv(p2.fv());
			glVertex3fv(p3.fv());
    glEnd();

	if (renderNormal)
		{
		glBegin(GL_LINE);
		    glColor3f(1,0,0);
			glVertex3fv(pnormalEnd1.fv());
			glColor3f(0,1,0);
			glVertex3fv(pnormalEnd2.fv());
		glEnd();
		}
	}
}

void CubeClass::renderCCW()
{
  int v;
  /* Draw cube in traditional OpenGL style */
 
	for (int face = 0; face < 6; face++) 
		{
		v= Faces[face][3]; // this line is supposed to make things simpler
	    p0.x = Verts[v][0]; //x
		p0.y = Verts[v][1]; //y
		p0.z = Verts[v][2]; //z

		v= Faces[face][2]; // this line is supposed to make things simpler
		p1.x =Verts[v][0]; //x
		p1.y =Verts[v][1]; //y
		p1.z =Verts[v][2]; //z

		v= Faces[face][1]; // this line is supposed to make things simpler
		p2.x =Verts[v][0]; //x
		p2.y =Verts[v][1]; //y
		p2.z =Verts[v][2]; //z

		v= Faces[face][0]; // this line is supposed to make things simpler
		p3.x =Verts[v][0]; //x
		p3.y =Verts[v][1]; //y
		p3.z =Verts[v][2]; //z

		//temp0 = p0-p1;
        //temp1 = p0-p2;

	    pnormal	= polygonNormal(p0,p1,p2);

		pnormalEnd1.x = (p0.x +p1.x +p2.x +p3.x)/4.0f;
		pnormalEnd1.y = (p0.y +p1.y +p2.y +p3.y)/4.0f;
		pnormalEnd1.z = (p0.z +p1.z +p2.z +p3.z)/4.0f;

		pnormalEnd2 = pnormalEnd1+pnormal;

    glBegin(GL_QUADS);
			glVertex3fv(p0.fv());
			glVertex3fv(p1.fv());
			glVertex3fv(p2.fv());
			glVertex3fv(p3.fv());
    glEnd();

	if (renderNormal)
		{
		glBegin(GL_LINES);
			glVertex3fv(pnormalEnd1.fv());
			glVertex3fv(pnormalEnd2.fv());
		glEnd();
		}
	}
}

void CubeClass::render()
{
	renderCCW();
	
}
