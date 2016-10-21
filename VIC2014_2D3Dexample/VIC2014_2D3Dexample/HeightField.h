#ifndef _heightfield_
#define _heightfield_

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "glut.h"

#include "utility.h"
#include "glUtils.h"

#include "Axes.h"
#include "CubeClass.h"
#include "2DText.h"
#include "grid.h"
#include "fileOperations.h"
#include "Lights.h"

#pragma warning(disable : 4996) // Reduce unsafe warning messages

class HeightField
{
public:


int heights[MAXWIDTH][MAXDEPTH];
int hfWidth;
int hfDepth;



GLfloat h[MAXWIDTH][MAXDEPTH]; // scaled 
GLfloat hnorm[MAXWIDTH][MAXDEPTH][3]; // normals for triangles
GLfloat htri[MAXWIDTH][MAXDEPTH][2][3][3]; // converted to triangles

int maxHeight;

HeightField();
~HeightField();

int setup(GLfloat xsizeZ, GLfloat zsizeZ, int xstepsZ, int zstepsZ);
int setupNormals();

vec3 calculateNormal(vec3 p1,vec3 p2, vec3 p3);


int renderPoints(); // testing
int renderTriangles(); // smooth no lighting colour based on height
int renderCrystal();  // with lighting and normals

void setMaterialHeight(GLfloat height);
void setColour(int height);
bool hrenderNormal;

protected:
	GLfloat xsize;
    GLfloat zsize;
    int xsteps;
    int zsteps;

	materialClass* mat;
    materialClass* matLines;

	materialClass* matHigh;
    materialClass* matBeach;
    materialClass* matLand;

	GLfloat highHeight;
    GLfloat grassHeight;

};

#endif
// end 