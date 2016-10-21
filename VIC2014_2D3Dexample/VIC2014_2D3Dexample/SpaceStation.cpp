// space station class

#include "spacestation.h"

SpaceStation1::SpaceStation1() // constructor
	{
	active=true;
	quadric1=gluNewQuadric();
	quadric2=gluNewQuadric();
	quadric3=gluNewQuadric();
	quadric4=gluNewQuadric();
	cube = new CubeClass();
	angleVar=20; // default 20 degrees
	/*pos.x=-10;
	pos.y=90;
	pos.z=0;*/
	pos.x=75;
	pos.y=70;
	pos.z=167;
	//pos.x=90;
	//pos.y=64;
	//pos.z=180;
	boundingRadius=10;
	centerOffset.x=0; // offset to the center of the boundiung radius
	centerOffset.y=0; // offset to the center of the boundiung radius
	centerOffset.z=0; // offset to the center of the boundiung radius
	mySpeed=0.1;
	} 

SpaceStation1::	~SpaceStation1() // destructor
{
	gluDeleteQuadric(quadric1);
	gluDeleteQuadric(quadric2);
	gluDeleteQuadric(quadric3);
    gluDeleteQuadric(quadric4);
	delete cube;
}

void SpaceStation1::timer()
{
  pos.x=pos.x+mySpeed;
}

GLfloat SpaceStation1::distance6f(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2)
{
return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2) + (z1-z2)*(z1-z2));
}

bool SpaceStation1::collisionWithSphere(GLfloat x, GLfloat y, GLfloat z, GLfloat radius)
{
GLfloat xx,yy,zz,dist;

xx=centerOffset.x+pos.x;
yy=centerOffset.y+pos.y;
zz=centerOffset.z+pos.z;
dist = distance6f(xx,yy,zz,x,y,z);
if (dist <radius+boundingRadius) return true;
else return false;
}

void SpaceStation1::drawCube()
{
	glPushMatrix();
	cube->render();
	glPopMatrix();
}

void SpaceStation1::drawCylinder()
{	
	glPushMatrix();
	//glRotatef(90,1,0,0);
	glTranslatef(0,0,-0.5);
	//gluQuadricOrientation(quadric1,GLU_OUTSIDE);
	gluCylinder(quadric1,0.5,0.5,1,17,4);
	glPopMatrix();
}

void SpaceStation1::drawDisk(GLfloat innerRadius) // inner raduis 0 = full disk 0.5 = no disk or circle
{	
	glPushMatrix();
	//glTranslatef(0,0,-0.5);
	//gluQuadricOrientation(quadric3,GLU_OUTSIDE);
	gluDisk(quadric3,innerRadius,0.5,17,4);
	glPopMatrix();
}

void SpaceStation1::drawCone(GLfloat smallEndRadius) //  raduis 0 = Cone  0.5 = Cylinder
{	
	glPushMatrix();
	glTranslatef(0,0,-0.5);
	//gluQuadricOrientation(quadric4,GLU_OUTSIDE);
	gluCylinder(quadric4,0.5,smallEndRadius,1,17,4);
	glPopMatrix();
}

void SpaceStation1::drawSphere()
{	
	glPushMatrix();
	//gluQuadricOrientation(quadric2,GLU_OUTSIDE);
	gluSphere(quadric2,0.5,13,13);
	glPopMatrix();
}

void SpaceStation1::renderBoundingSphere()
{
	glPushMatrix();
	glTranslatef(centerOffset.x,centerOffset.y,centerOffset.z);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); /* Wireframe mode */
	gluSphere(quadric2,boundingRadius,9,9);
	glPopMatrix();
}

void SpaceStation1::drawPanelPin()
{
	glPushMatrix();
	glScalef(1,2.0,2.0);
	//glColor3f(0.5,0.5,0.5);
	glColor3f(0.6f,0.65f,0.75f); 
	drawCube();
	glPopMatrix();
}

void SpaceStation1::drawWholePanel()
{
	glPushMatrix();
	/*glColor3f(0.6f,0.65f,0.75f);
	drawPanelPin();*/
	glTranslatef(0.0,-0.7,0.7);
	glScalef(1.0,0.5,0.5);
	//glColor3f(0.5,0.5,0.5);
	drawPanelPin();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0,0.7,-0.7);
	glScalef(1.0,0.5,0.5);
	//glColor3f(0.5,0.5,0.5);
	drawPanelPin();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0,0.7,0.7);
	glScalef(1.0,0.5,0.5);
	//glColor3f(0.5,0.5,0.5);
	drawPanelPin();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0,-0.7,-0.7);
	glScalef(1.0,0.5,0.5);
	//glColor3f(0.5,0.5,0.5);
	drawPanelPin();
	glPopMatrix();
}


void SpaceStation1::drawSolarPanel()
{
	glPushMatrix();
	glTranslatef(0,2.5,0);
	glScalef(0.6,5,3);
	drawCube();
	glPopMatrix();
    glPushMatrix();
	glTranslatef(0.0,1.5,0.0);
	drawWholePanel();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0,3,0.0);
	drawWholePanel();
	glPopMatrix();
}

void SpaceStation1::drawSolarPanelConector()
{
	glPushMatrix();
	glColor3f(0.69,0.4,0.9);
	glTranslatef(0.0,0.0,0.5);
	drawDisk(0);
	glPopMatrix();
	glPushMatrix();
	drawCylinder();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0,0.0,-0.5);
	drawDisk(0);
	glPopMatrix();
}

void SpaceStation1::drawSolarPanelAndConector(GLfloat angle)
{
	glPushMatrix();
	glRotatef(angle, 1.0,0.0,0.0);
	glScalef(0.5,0.5,0.6);
	glColor3f(0.6f,0.65f,0.75f); 
	drawSolarPanelConector();
	glPopMatrix();
	glPushMatrix();
	//glTranslatef(0.0,-0.25,0.0);
	glTranslatef(0.0,0.2,0.0);
	//glScalef(0.6,1,1);
	drawSolarPanel();
	glPopMatrix();
}

void SpaceStation1::drawCrewModule()
{
    glPushMatrix();
	//glColor3f(0.6f,0.65f,0.75f); 
	glTranslatef(0.0,0.0,0.5);
	drawDisk(0);
	glPopMatrix();
	glPushMatrix();
	//glColor3f(0.6f,0.65f,0.75f); 
	drawCylinder();
	glTranslatef(0.0,0.0,-0.5);
	drawCylinder();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0,0.0,-1);
	drawDisk(0);
	glPopMatrix();	
}

void SpaceStation1::drawConector()
{
	glPushMatrix();
	glColor3f(0.69,0.4,0.9);
	glScalef(0.25,0.25,0.2);
	drawCrewModule();
	glPopMatrix();
}
	
void SpaceStation1::drawCrewModulePlus(int drawconector, GLfloat solarAngle, GLfloat solarAngleIncrement, int numSolars)
{
}
void SpaceStation1::drawCrewModuleAndOneWing()
{
	glPushMatrix();
	glTranslatef(0.0,0.0,0.25);
	glColor3f(0.6f,0.65f,0.75f);
	drawCrewModule();
	glPopMatrix();
	glPushMatrix();
	glScalef(0.45,0.65,0.45);
	glTranslatef(0.0,1.0,0);
	glRotatef(70,0.0,1.0,0.0);
	drawSolarPanelAndConector(90);
	glPopMatrix();
}


void SpaceStation1::drawCrewModuleAndTwoWings()
{
	glPushMatrix();
	drawCrewModuleAndOneWing();
	glRotatef(180,0.0,0.0,1.0);
	glScalef(0.45,0.65,0.45);
	glTranslatef(0.0,1.0,0);
	glRotatef(50,0.0,1.0,0.0);
	drawSolarPanelAndConector(90);
	glPopMatrix();
}

void SpaceStation1::drawCrewModuleAndConnector()
{
	glPushMatrix();
	drawCrewModuleAndTwoWings();
	glTranslatef(0.0,0.0,-0.85);
	drawConector();
	glPopMatrix();
}

void SpaceStation1::drawRobotArm1()
{
	glPushMatrix();
	drawSphere();
	glPopMatrix();
	glPushMatrix();
	//glRotatef(angleVar ,1.0,0.0,0.0);
	glTranslatef(0.0,0.0,-2.0);
	glScalef(1.0,1.0,4);
	drawCylinder();
	glPopMatrix();
}

void SpaceStation1::drawRobotFinger(GLfloat DatVar)
{
	glPushMatrix();
	glTranslatef(0.0,0.0,-0.7);
	glRotatef(- 90 - DatVar/3 ,1.0,0.0,0.0);
	drawRobotArm1();
	/*glPopMatrix();
	glPushMatrix();*/
	glTranslatef(0.0,0.0,-4.0);
	glRotatef(60 - DatVar/3 ,1.0,0.0,0.0);
	drawRobotArm1();
	/*glTranslatef(0.0,0.0,-4.0);
	glRotatef(-30 + DatVar ,-1.0,0.0,0.0);
	drawRobotArm1();*/
	glTranslatef(0.0,0.0,-4.5);
	glRotatef(180,0.0,1.0,0.0);
	drawCone(0);
	glPopMatrix();
}

void SpaceStation1::drawRobotHand(GLfloat DatVar)
{
	glPushMatrix();
	glScalef(0.5,0.5,0.5);
	drawRobotFinger(DatVar);
	glRotatef(90,0.0,0.0,1.0);
	drawRobotFinger(DatVar);
	glRotatef(90,0.0,0.0,1.0);
	drawRobotFinger(DatVar);
	glRotatef(90,0.0,0.0,1.0);
	drawRobotFinger(DatVar);
	glPopMatrix();
}

void SpaceStation1::drawRobotArm2(GLfloat DatVar)
{
	glPushMatrix();
	/*drawCone(1);*/
	glTranslatef(0.0,0.0,-0.7);
	glRotatef(-90 - DatVar ,1.0,0.0,0.0);
	drawRobotArm1();
	/*glPopMatrix();
	glPushMatrix();*/
	glTranslatef(0.0,0.0,-4.0);
	glRotatef(90 + DatVar ,1.0,0.0,0.0);

	drawRobotArm1();

	glTranslatef(0.0,0.0,-4.0);
	glRotatef(0,1.0,0.0,0.0);
	drawRobotHand(DatVar);
	glPopMatrix();
}
void SpaceStation1::drawConnectionWithArm()
{
	glPushMatrix();
	glRotatef(90,1,0,0);
	glScalef(0.7,0.7,0.7);
	glPopMatrix();
}


void SpaceStation1::drawSpaceStation1(GLfloat angle, GLfloat spinAngle)
{
	glPushMatrix();
	glRotatef(2*angle,0,0,1);
	drawCrewModuleAndConnector();
	glTranslatef(0.0,0.0,-1.8);
	glRotatef(spinAngle,0,0,1);
	drawCrewModuleAndConnector();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0,0.0,-3.6);
	glRotatef(-30,0.0,0.0,1.0);
	glRotatef(-spinAngle,0,0,1);
	drawCrewModuleAndOneWing();
	glPopMatrix();
	glPushMatrix();
	//glTranslatef(0.0,0.0,0.8);
	glTranslatef(0.0,0.0,1.1);
	//glScalef(0.1,0.1,0.1);
	//glRotatef(180,1.0,0.0,0.0);
	//drawCone(1);
	drawConnectionWithArm();
	glPopMatrix();
	/*glPushMatrix();
	glTranslatef(0.0,0.0,0.8);
	glRotatef(180,1.0,0.0,0.0);
	glScalef(0.1,0.1,0.1);
	drawRobotArm2();
	glPopMatrix();*/
}





void SpaceStation1::drawSStation1(int start,GLfloat DatVar,GLfloat angle, GLfloat spinAngle)
{
	if (!active) return;
	else
	{
	glPushMatrix();
	glScalef(2,2,2);
	glTranslatef(0.0,0.0,start - DatVar);
	drawSpaceStation1(angle, spinAngle);
	glPopMatrix();
	}
}

void SpaceStation1::drawSpaceStation2()
{
	glPushMatrix();
	drawCrewModule();
	glScalef(1,1,0.5);
	glTranslatef(0.0,0.0,-0.5);
	drawSolarPanel();
	glPopMatrix();
	glPushMatrix();
	glRotatef(180,0.0,0.0,1.0);
	glScalef(1,1,0.5);
	glTranslatef(0.0,0.0,-0.5);
	drawSolarPanel();
	glPopMatrix();
	glPushMatrix();
	glTranslatef(0.0,0.0,1.0);
	drawCone(2);
	drawSphere();
	glPopMatrix();
}

void SpaceStation1::drawSStation21(GLfloat DatVar, GLfloat spinAngle)
{
	glPushMatrix();
	//glShadeModel(GL_SMOOTH);
	glTranslatef(4.0,4.0,9.7);
	//glTranslatef(10.0,10.0,3.0);
	glRotatef(180,1.0,0.0,0.0);
	glScalef(0.5,0.5,0.5);
	/*drawCone(1);*/
	drawRobotArm2(DatVar);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(4.0,4.0,6.7);
	//glTranslatef(10.0,10.0,0.0);
	glTranslatef(0.0,0.0,24.0 + DatVar/6);
	glRotatef(90,0.0,0.0,1.0);
	glRotatef(2*spinAngle,0,0,1);
	drawSpaceStation2();
	glPopMatrix();
}


void SpaceStation1::drawSStation2(GLfloat DatVar, GLfloat spinAngle)
{
	if (!active) return;
	glPushMatrix();
	//glShadeModel(GL_SMOOTH);
	//mat0->doMaterial();
	//mat0->setMaterialJade();
	glRotatef(-angleVar,1,0,0);
    glTranslatef(0.0,0.0,angleVar/5.7);
	drawSStation21(DatVar,spinAngle);
	glPopMatrix();

}

void SpaceStation1::dockingSpaceShip(GLfloat DatVar,GLfloat spinAngle,GLfloat ControlAngle)
{
	ControlAngle = spinAngle/4;
	DatVar = -spinAngle;
	if (DatVar <= -90)
		DatVar = -90;
}

// end