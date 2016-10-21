#ifndef _firingplatform_
#define _firingplatform_

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
#include "Vec3.h"

#pragma warning(disable : 4996) // Reduce unsafe warning messages

class FiringPlatform
{
public:
    vec3 pos;
	vec3 color;
	GLfloat rotationX;
    GLfloat rotationY;

	vec3 projectilePos;
	vec3 projectileSpeed;
	bool projectileFired;

	GLUquadricObj *quadric1;    
	GLUquadricObj *quadric2;  
	GLUquadricObj *quadric3;   

	FiringPlatform();
	~FiringPlatform();

	void setPos(GLfloat x,GLfloat y,GLfloat z);

	void drawDisk(GLfloat innerRadius); // inner raduis 0 = full
	void drawCylinder(); 	
	void drawCylinder2(); 

	void aimUp();
	void aimDown();
	void aimLeft();
	void aimRight();

	void fire();
	void timer();
	void render();
	void renderProjectile();
	void reload();
	void movePlane(GLfloat oldx, GLfloat oldy,  GLfloat oldz,
          GLfloat roll, GLfloat pitch, GLfloat yaw, GLfloat speed,
          GLfloat *resultx, GLfloat *resulty,  GLfloat *resultz);

};

#endif
//end