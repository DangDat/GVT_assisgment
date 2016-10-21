#ifndef _spaceStation_
#define _spaceStation_

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

class SpaceStation1
{
public:
	bool active; // set to false to make it dissapear
	vec3 pos;
	bool ss1_type;
	GLfloat boundingRadius;
	vec3 centerOffset; // offset to the center of the boudiung radius
	GLfloat rotationInY;
	GLfloat mySpeed;

	

	GLUquadricObj *quadric1;    
	GLUquadricObj *quadric2;  
	GLUquadricObj *quadric3;    
	GLUquadricObj *quadric4;
	CubeClass *cube;
	GLfloat angleVar;

	SpaceStation1(); // constructor
	~SpaceStation1(); // destructor

	//void setpos(Vec3 pos); // just update pos
    //void setpos(GLfloat x, GLfloat y, GLfloat z);

	void timer();
	void render(); // draw it
	void renderSS2();
	void renderBoundingSphere();
	void drawDisk(GLfloat innerRadius); // inner raduis 0 = full disk 0.5 = no disk or circle
	void drawCone(GLfloat smallEndRadius); //  raduis 0 = Cone  0.5 = Cylinder
    void drawSphere();
	void drawCube();
	void drawCylinder();

	void drawPanelPin();
	void drawWholePanel();
	void drawSolarPanel();
	void drawSolarPanelConector();
	void drawSolarPanelAndConector(GLfloat angle);
	void drawCrewModule();
	void drawConector();
	void drawCrewModulePlus(int drawconector, GLfloat solarAngle, GLfloat solarAngleIncrement, int numSolars);
	void drawCrewModuleAndOneWing();
	void drawCrewModuleAndTwoWings();
	void drawCrewModuleAndConnector();
	void drawRobotArm1();
	void drawRobotFinger(GLfloat DatVar);
	void drawRobotHand(GLfloat DatVar);
	void drawRobotArm2(GLfloat Datvar);
	void drawConnectionWithArm();
	void drawSpaceStation1(GLfloat angle, GLfloat spinAngle);

	void drawSStation1(int start,GLfloat DatVar,GLfloat angle, GLfloat spinAngle);
	void drawSpaceStation2();
	void drawSStation21(GLfloat DatVar, GLfloat spinAngle);
	void drawSStation2(GLfloat DatVar, GLfloat spinAngle);
	void dockingSpaceShip(GLfloat DatVar, GLfloat spinAngle,GLfloat ControlAngle);

	bool collisionWithSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat radius);
	GLfloat distance6f(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2);
};


#endif