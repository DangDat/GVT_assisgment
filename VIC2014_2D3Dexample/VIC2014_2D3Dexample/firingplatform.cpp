// firingplatform.cpp


#include "FiringPlatform.h"

FiringPlatform::FiringPlatform()
{

setPos(90,64,184);
	//setPos(0,0,0);
color=vec3(0.1,0.1,0.7);
//rotationX=180+20;
rotationX =180 + 20;
rotationY=0;

projectileSpeed=vec3(0,0,0);
projectileFired=false;

quadric1=gluNewQuadric();
quadric2=gluNewQuadric();
quadric3=gluNewQuadric();
}

void FiringPlatform::setPos(GLfloat x,GLfloat y,GLfloat z)
{
	pos.x = x;
	pos.y = y;
	pos.z = z;

	projectilePos.x=pos.x;
	projectilePos.y=pos.y;
	projectilePos.z=pos.z;
}

FiringPlatform::~FiringPlatform()
{
	gluDeleteQuadric(quadric1);
	gluDeleteQuadric(quadric2);
	gluDeleteQuadric(quadric3);
}

void FiringPlatform::drawCylinder()
{	
	glPushMatrix();
	//glRotatef(90,1,0,0);
	glTranslatef(0,0,-0.5);
	
	gluCylinder(quadric1,0.5,0.5,1,17,4);
	glPopMatrix();
}

void FiringPlatform::drawCylinder2()
{	
	glPushMatrix();
	//glRotatef(90,1,0,0);
	glTranslatef(0,0,0);
	gluCylinder(quadric3,0.5,0.5,1,17,4);
	glPopMatrix();
}

void FiringPlatform::drawDisk(GLfloat innerRadius) // inner raduis 0 = full disk 0.5 = no disk or circle
{	
	glPushMatrix();
	//glTranslatef(0,0,-0.5);
	
	gluDisk(quadric2,innerRadius,0.5,17,4);
	glPopMatrix();
}

void FiringPlatform::aimUp()
{
	rotationX=rotationX+1;
}

void FiringPlatform::aimDown()
{
	rotationX=rotationX-1;
}

void FiringPlatform::aimLeft()
{
	rotationY=rotationY-1;
}

void FiringPlatform::aimRight()
{
	rotationY=rotationY+1;
}

void FiringPlatform::movePlane(GLfloat oldx, GLfloat oldy,  GLfloat oldz,
          GLfloat roll, GLfloat pitch, GLfloat yaw, GLfloat speed,
          GLfloat *resultx, GLfloat *resulty,  GLfloat *resultz)
{
GLfloat tmpx,tmpy,tmpz;
GLfloat newx,newy,newz;

tmpy=0; /* set to origin */
tmpx=0;
tmpz=speed; /* move us forward by speed */

newx=(tmpx * cos(roll))-(tmpy * sin(roll)); // rotate around z
newy=(tmpx * sin(roll))+(tmpy * cos(roll));
tmpx=newx;
tmpy=newy;

newy=(tmpy * cos(pitch)) - (tmpz * sin(pitch)); // rotate around x
newz=(tmpy * sin(pitch)) + (tmpz * cos(pitch));
tmpy=newy;
tmpz=newz;

newx=(tmpz * sin(yaw)) + (tmpx * cos(yaw));
newz=(tmpz * cos(yaw)) - (tmpx * sin(yaw));
tmpx=newx;
tmpz=newz;

newx=tmpx+oldx;
newy=tmpy+oldy;
newz=tmpz+oldz;

*resultx=newx;
*resulty=newy;
*resultz=newz;
 //printf("%6.3f",newx);
 //printf("%6.3f",newy);
 //printf("%6.3f",newz);
}

void FiringPlatform::fire()
{
	GLfloat xDelta;
	GLfloat yDelta;
	GLfloat zDelta;

	if (projectileFired) return;
	projectilePos.x=pos.x;
	projectilePos.y=pos.y;
	projectilePos.z=pos.z;

	movePlane(0, 0, 0,
		0*3.1416/180, (rotationX-180)*3.1416/180, -rotationY*3.1416/180, 1, // damm radians to degrees 3.1416/180;
          &xDelta, &yDelta, &zDelta);

	projectileSpeed.x=-xDelta;
	projectileSpeed.y=-yDelta;
	projectileSpeed.z=-zDelta;

	projectileFired=true;
}

void FiringPlatform::reload()
{
		projectileFired=false;
		projectilePos.x=pos.x;
		projectilePos.y=pos.y;
		projectilePos.z=pos.z;
}

void FiringPlatform::timer()
{
	float distancefromgun;
// move projectile
	if (!projectileFired) return;
	projectilePos.x=projectilePos.x+projectileSpeed.x;
	projectilePos.y=projectilePos.y+projectileSpeed.y;
	projectilePos.z=projectilePos.z+projectileSpeed.z;
 	//projectileSpeed.y=projectileSpeed.y-0.01; // gravity
	 /*printf("%6.3f",projectilePos.x);
	 printf(" ");
     printf("%6.3f",projectilePos.y);
	 printf(" ");
	 printf("%6.3f \n",projectilePos.z);*/
	distancefromgun = dist(pos,projectilePos);
	if (distancefromgun > 50) 
	{
		reload();
		//projectileFired=false;
		//projectilePos.x=pos.x;
		//projectilePos.y=pos.y;
		//projectilePos.z=pos.z;
	}

}

void FiringPlatform::render()
{
	glDisable(GL_COLOR_MATERIAL);
	glPushMatrix();
	glTranslatef(pos.x,pos.y,pos.z);
	glColor3f(0.4f,0.4f,0.3f);
	glRotatef(90,1,0,0);
	glScalef(2,2,2);
	drawDisk(0.1);
	glPopMatrix();
	
	glPushMatrix();
	glColor3f(0.3f,0.3f,0.3f);
	glTranslatef(pos.x,pos.y-0.1,pos.z);
	glRotatef(90,1,0,0);
	glScalef(2,2,0.2);
	drawCylinder();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(pos.x,pos.y+0.2,pos.z);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); /* fill mode */
	glLineWidth(3);
	glColor3f(color.x,color.y,color.z);
	glRotatef(rotationX,1,0,0);
	glRotatef(rotationY,0,1,0);
	glScalef(0.5,0.5,1.5);
	drawCylinder2();
	glPopMatrix();

}

void FiringPlatform::renderProjectile()
{
	if (!projectileFired) 
	{
	glPushMatrix();
	glTranslatef(pos.x,pos.y+0.2,pos.z);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); /* fill mode */
	glLineWidth(4);
	glColor3f(1.0,0.1,0.1);
	glScalef(0.6,0.6,0.6);
	gluSphere(quadric2,0.5,13,13);
	glPopMatrix();
	}
	else
	{
	glPushMatrix();
	glTranslatef(projectilePos.x,projectilePos.y+0.2,projectilePos.z);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); /* fill mode */
	glLineWidth(4);
	glColor3f(1.0,0.2,0.2);
	glScalef(0.6,0.6,0.6);
	gluSphere(quadric2,0.5,13,13);
	glPopMatrix();
	}
}

//end