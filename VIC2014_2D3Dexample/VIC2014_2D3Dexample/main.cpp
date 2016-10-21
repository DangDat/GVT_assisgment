// Assign answer 2014

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include <math.h>

#include "glut.h"

#include "utility.h"
#include "glUtils.h"

#include "Axes.h"
#include "CubeClass.h"
#include "2DText.h"
#include "grid.h"
#include "spacestation.h"
#include "HeightField.h"
#include "FiringPlatform.h"

#include "vec4.h"
#include "Bitmap.h"
#include "fileOperations2.h"

#include <ctime>
#include "System.h"


#pragma warning(disable : 4996) // Reduce unsafe warning messages

#define framesPerSecond 25
#define timerMSecs ((int)1000/framesPerSecond)  // compute time in miliseconds for a frame
bool timeForNextFrame=false;

#define ESC	  27

#define cmdRed		 1
#define cmdGreen	 2
#define cmdExit		99

#define MAX_PSYSTEM 400


float zoom;
System particleSystem;
GLfloat texture[10];
GLuint LoadTextureRAW( const char * filename, int width, int height);
void FreeTexture( GLuint texturez );

void DrawParticles (void)
{
   int i;
   for (i = 1; i < particleSystem.getNumOfParticles(); i++)
   {
      glPushMatrix();
      // set color and fade value (alpha) of current particle
      glColor4f(particleSystem.getR(i), particleSystem.getG(i), particleSystem.getB(i), particleSystem.getAlpha(i));
      // move the current particle to its new position
      glTranslatef(particleSystem.getXPos(i), particleSystem.getYPos(i), particleSystem.getZPos(i) + zoom);
      // rotate the particle (this is proof of concept for when proper smoke texture is added)
      glRotatef (particleSystem.getDirection(i) - 90, 0, 0, 1);
      // scale the wurrent particle (only used for smoke)
      glScalef(particleSystem.getScale(i), particleSystem.getScale(i), particleSystem.getScale(i));

      glDisable (GL_DEPTH_TEST);
      glEnable (GL_BLEND);

      glBlendFunc (GL_DST_COLOR, GL_ZERO);
      glBindTexture (GL_TEXTURE_2D, texture[0]);

      glBegin (GL_QUADS);
      glTexCoord2d (0, 0);
      glVertex3f (-1, -1, 0);
      glTexCoord2d (1, 0);
      glVertex3f (1, -1, 0);
      glTexCoord2d (1, 1);
      glVertex3f (1, 1, 0);


      glBlendFunc (GL_ONE, GL_ONE);
      glBindTexture (GL_TEXTURE_2D, texture[1]);

      glBegin (GL_QUADS);
      glTexCoord2d (0, 0);
      glVertex3f (-1, -1, 0);
      glTexCoord2d (1, 0);
      glVertex3f (1, -1, 0);
      glTexCoord2d (1, 1);
      glVertex3f (1, 1, 0);
      glTexCoord2d (0, 1);
      glVertex3f (-1, 1, 0);
      glEnd();

      glEnable(GL_DEPTH_TEST);

      glPopMatrix();
   }
}


static int	WinWidth, WinHeight;
static int	AppMenu;

static GLfloat	spinAngle;
GLfloat spinIncrement=0.4f; // slow

static GLfloat	eyePoint[3];
static GLfloat	lookAtPoint[3];
static GLfloat	upVec[3];//
static GLfloat  fov; // field of view
static GLfloat	nearPlane;
static GLfloat	farPlane;


static GLfloat	color3[3];

static GLfloat	red[]		= {1.0, 0.0, 0.0};
static GLfloat	blue[]		= {0.0, 1.0, 0.0};
static GLfloat	green[]		= {0.0, 0.0, 1.0};
static GLfloat	cyan[]		= {0.0, 1.0, 1.0};
static GLfloat	magenta[]	= {1.0, 0.0, 1.0};
static GLfloat	yellow[]	= {1.0, 1.0, 0.0};
static GLfloat	black[]		= {0.0, 0.0, 0.0};
static GLfloat	white[]		= {1.0, 1.0, 1.0};

CubeClass *cube; // pointer to a cube object
/*GLUquadricObj *quadric1;    
GLUquadricObj *quadric2;  
GLUquadricObj *quadric3;    
GLUquadricObj *quadric4;  */  
Axes *axes; //
Grid *grid; // To help with push and scal etc
HeightField *hf;
FiringPlatform *fp;
SpaceStation1 *spaceShip;


int showGrid=FALSE;
int showAxes=TRUE;
int showSStation1=TRUE;
int showSStation2=FALSE;
int showSStation3=FALSE;
int showWireFrame=2; // 2=points 1=wireframe 0=filled
int showSpin=FALSE;
int smoothShade=FALSE; // change center of spin 
int showNormals=FALSE; // change center of spin 
int showBoundingSphere=false;
int aimMode=false;
int movingSStation = FALSE;
int showCollide = FALSE;

int light0=TRUE; // light 0 
int light1=FALSE; // light 1 on (7 key)
int light2=FALSE; // light 2 on (8 key)
int light3=FALSE; // light 3 on (9 key)
int showLights=0; // 0=lighting off ,1=lighting on, 2=lighting and show pos of lights as sphere 

int hiddenDepth=FALSE;
int hiddenCull=FALSE;
int clockWise=FALSE; // start CCW
GLfloat angle = 0;
GLfloat angleVar = 0;
GLfloat DatVar = 0;
GLfloat ControlAngle = 0;

int mode=2; // mode 2 = 2D, 3=3D , 5=Both
Text2D *drawText2d;
char dispString[255] = "2=2D (also F1-help)  3=3D  5=2D+3D";

SpaceStation1 *ss1;
SpaceStation1 *ss2;
SpaceStation1 *ss3;

lightClass * lightP0;
lightClass * lightP1;
lightClass * lightP2;
lightClass * lightP3;

GLuint  ttex[10];
CBitmap *texTemp;
vec3    eyePos;
//vec3 collPos;
vec3 collPos = (87,66,157);
GLubyte tex1[128][128][4];    
GLfloat billboardAngle=0;
int err;




struct particleStruct
{
       bool active;   // active Y or n
	   int time;      // ticks of activity
       vec3 pos1;      // position
       vec3 prevPos;  // prevPosition
       vec3 velosity; // how fast do i move
	   vec4 colour;
	   float pSize;   // size 
};

 // 0=Points of initial size only
 // 1=last turn lines
 // 2=128x128 texture

struct PsystemStruct
       {
	  bool active;            // true if active
	   int activeticks;       // num of ticks for the posys to be active -1 lasts forever
	   bool particlesStillActive; // 
       int typeOfPsystem;      // 0,1,2 - see above
	   GLuint texHandle;       // texture handle for type 2 systems
	   bool billboard;         // true if billboard
       vec3 pos1;               // location of start     
       GLfloat emissionRate;   // number of particles per tick (can be fractional)
	   GLfloat emmisionRateRandom; // a max random number which we div by 100 and add to emision rate
	   GLfloat residualEmmision; // how many to emit this turn - to allow for fractional emission rates
       int maxParticles;       // no more than this please
       bool noMoreThanMax;     // after maxParticles set emission rate to 0
	   int numParticles;       // the actual number 
       GLfloat particleSize;   // size in points 
       GLfloat particleSizeDelta;   // size in points change per tick 
	   int max_ticks;          // after this many ticks die
	   vec3 dieIfLess;      // kill the particl if its less than any of these values
       vec4 colour;     // particle color
	   vec4 colourChange; // colour change + added each tick
       vec4 colourMax;  // 1,1,1 = white
	   vec3 gravity;    // velosity change due to gravity
       vec3 wind;       // movement due to wind
       vec3 initialVelosity;
	   vec3 initialVelosityRandom; // max random number which we dive by 100 and add to initial velosity 
       vec3 airResistance;    // how it slows always positive
       vec3 absTerminalVelosity; // max speed always positive

       // particle data
	   particleStruct p[MAX_PSYSTEM];

       };

int psCurr;
PsystemStruct ps[10]; // 0 through 9 

//particleSystemClass * psc;
// random integer value 0 to max (macro for speed)
#define rndToMax(max) (rand()%(int)max)

// random float value 0 to max (macro for speed)
#define rndToMaxf(max) ((float)(rand()%(int)max))

// random value between -1 and 1 (macro for speed)
//#define rnd() ((rand()%10000)/5000.0-1)

double dist(double x1, double y1, double x2, double y2)
{
/* calc the distance between 2 points */
double retv;
double d1,d2;
double temp;

d1=x1-x2;
d2=y1-y2;
temp=d1*d1+d2*d2;
retv=sqrt(temp);
return(retv);
}

void direct(GLfloat xx1,GLfloat yy1,GLfloat xx2,GLfloat yy2,GLfloat *angle,GLfloat *distz)
/* calculate angle in degrees and distance to location */
{
GLfloat temp;
GLfloat ttt;
*distz=dist(xx1,yy1,xx2,yy2);
//printf("dist=%f\n",(*distz));
if (*distz==0)
	{
	*angle=0;
	return;
	}
ttt=(xx2-xx1)/(*distz);
//printf("ttt=%f\n",ttt); 
//if (ttt>=1) ttt=1;
if (ttt<=-1) ttt=-1;
temp=asin(ttt);
*angle=temp*180/3.14159;
if (yy2<yy1)
	{
		*angle=180-*angle;
	}
else
	{
	if( *angle < 0 )
		*angle = *angle + 360;
	}
}

void drawGround()
{
int i,j;
glPushMatrix();
glColor4f(0.0, 0.5, 0.0, 1);
glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

glLineWidth(2);
for (i=-4;i<5;i++)
    for (j=-4;j<5;j++)
    {
    glBegin(GL_QUADS);
    glVertex3f(i+1, 0, j+1);
    glVertex3f(i+0, 0, j+1);
    glVertex3f(i+0, 0, j-1);
    glVertex3f(i+1, 0, j-1);
    glEnd();
    }

glPopMatrix();
}


void drawPsystemType0(PsystemStruct *ps)
{
int i;

if (!ps->active && !ps->particlesStillActive) return;
for (i=0; i< ps->maxParticles; i++)
    {
	  glColor4fv(ps->p[i].colour);
      glPointSize(ps->particleSize);
      glBegin(GL_POINTS);
      if (ps->p[i].active)
       {
	   //glPointSize(ps->p[i].pSize);
       glVertex3fv(ps->p[i].pos1.fv());
       printf("%d %f %f %f\n",i,ps->p[i].pos1[X],ps->p[i].pos1[Y],ps->p[i].pos1[Z]);
       }
    glEnd();
    }
}

//void drawPsystemType1(PsystemStruct *ps)
//{
//int i;
//
//if (!ps->active && !ps->particlesStillActive) return;
//glBegin(GL_LINES);
//for (i=0; i< ps->maxParticles; i++)
//    {
//	  glColor4fv(ps->p[i].colour);
//      //glPointSize(ps->particleSize);
//      
//      if (ps->p[i].active)
//       {
//	   //glPointSize(ps->p[i].pSize);
//       glVertex3fv(ps->p[i].prevPos.fv());
//       glVertex3fv(ps->p[i].pos.fv());
//
//       printf("%d %f %f %f\n",i,ps->p[i].pos[X],ps->p[i].pos[Y],ps->p[i].pos[Z]);
//       }
//    } 
//glEnd();
//}
//
//void drawPsystemType2(PsystemStruct *ps)
//{
//	int i;
//	GLfloat temp;
//    if (!ps->active && !ps->particlesStillActive) return;
//
//    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
//    glEnable(GL_TEXTURE_2D);
//    //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
//    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
//	glBindTexture(GL_TEXTURE_2D, ttex[ps->texHandle]);
//
//    direct(0,0,eyePos[X],eyePos[Z],&billboardAngle,&temp); // I look at 0,0
//
//for (i=0; i< ps->maxParticles; i++)
//    {
//      if (ps->p[i].active)
//        {
//		//glColor4fv(ps->p[i].colour);
//		glColor4f(1,1,1,1);
//        glPushMatrix();
//        glTranslatef(ps->p[i].pos[X],ps->p[i].pos[Y],ps->p[i].pos[Z]);
//		glScalef(ps->p[i].pSize,ps->p[i].pSize,ps->p[i].pSize);
//		if (ps->billboard)
//			{
//				glRotatef(billboardAngle-spinAngle, 0.0, 1.0, 0.0);
//			}
//	    glBegin(GL_QUADS);
//		glTexCoord2f(0,1);
//        glVertex3f(-0.5,  0.5,  0.0);
//        glTexCoord2f(1,1);
//        glVertex3f( 0.0,  0.5,  0.0);
//        glTexCoord2f(1,0);
//        glVertex3f( 0.0, -0.5,  0.0);
//        glTexCoord2f(0,0);
//        glVertex3f(-0.5, -0.5,  0.0);
//		glEnd();
//	    glPopMatrix();
//	    }
//	  }
//  glDisable(GL_TEXTURE_2D);
//}

void drawPsystem(PsystemStruct *ps)
{
	/*if (ps->typeOfPsystem==0)*/ drawPsystemType0(ps); // points of a fixed size
	//if (ps->typeOfPsystem==1) drawPsystemType1(ps); // lines
	//if (ps->typeOfPsystem==2) drawPsystemType2(ps); // 64 x 64 bmp with alpha
}

void updatePsystem(PsystemStruct *ps)
{
int i;
GLfloat f;
int cnt=0;

if (!ps->active && !ps->particlesStillActive) return;
if (ps->activeticks>0)
	{
	ps->activeticks--;
	if (ps->activeticks<=0) {ps->active=false;ps->residualEmmision=0;}
	}

// process each particle slot for this time step...
if (ps->active) 
	{
		ps->residualEmmision=ps->residualEmmision+ps->emissionRate;
		if (ps->emmisionRateRandom!=0) ps->residualEmmision=ps->residualEmmision+ rndToMaxf(ps->emmisionRateRandom)/100;
    }

ps->particlesStillActive=false;

for (i=0; i< ps->maxParticles; i++) // create particles here
    {
    if ((!ps->p[i].active) && ps->residualEmmision > 1)
		{
		// create a new particle
        ps->residualEmmision=ps->residualEmmision-1;
		ps->p[i].active=true;
		ps->numParticles++;
		ps->p[i].pSize=ps->particleSize;
		ps->p[i].time=0;
        v3CopyM(ps->p[i].pos1,ps->pos1);
		v3CopyM(ps->p[i].prevPos,ps->pos1);
		v3CopyM(ps->p[i].velosity,ps->initialVelosity);
		v4CopyM(ps->p[i].colour,ps->colour);
		//for(n=0;n<3;n++)
		  // {
		if ((int)ps->initialVelosityRandom[X]!=0) 
			{
			f=rndToMaxf(ps->initialVelosityRandom[X])/100;
		    ps->p[i].velosity.x=ps->p[i].velosity.x+f;
		    }
        if ((int)ps->initialVelosityRandom[Y]!=0)
			{
		    f=rndToMaxf(ps->initialVelosityRandom[Y])/100;
		    ps->p[i].velosity.y=ps->p[i].velosity.y+f;	   
			}
        if ((int)ps->initialVelosityRandom[Z]!=0)
			{		 
			f=rndToMaxf(ps->initialVelosityRandom[Z])/100;
		    ps->p[i].velosity.z=ps->p[i].velosity.z+f;	   
		    }
		}

      //... then move all the particles...
    if (ps->p[i].active)
		{
		cnt++;
		ps->particlesStillActive=true;
		ps->p[i].time++;
		v3CopyM(ps->p[i].prevPos,ps->p[i].pos1);
		if (ps->p[i].time>ps->max_ticks) {ps->p[i].active=false; ps->numParticles--; continue;}
		v3AddToVecM(ps->p[i].pos1, ps->p[i].velosity); // my velosity
		v3AddToVecM(ps->p[i].pos1, ps->wind); // wind
		if (v3isLess(ps->p[i].pos1,ps->dieIfLess)){ps->p[i].active=false; ps->numParticles--; continue;}
		
		v3AddToVecM(ps->p[i].velosity,ps->gravity); // gravity
        v3clipAbsM(ps->p[i].velosity,ps->absTerminalVelosity);
        v3dampM(ps->p[i].velosity,ps->airResistance); // air resistance
        v3clipAbsM(ps->p[i].velosity,ps->absTerminalVelosity); // terminal velosity

		v4AddToVecM(ps->p[i].colour, ps->colourChange);
		v4ClipMaxM(ps->p[i].colour, ps->colourMax);

		ps->p[i].pSize=ps->p[i].pSize+ps->particleSizeDelta;
		if (ps->p[i].pSize<0)ps->p[i].pSize=0;
        }

    }

	if (cnt >= ps->maxParticles && ps->noMoreThanMax)
	{
		ps->emissionRate=0;
		ps->residualEmmision=0;
		ps->emmisionRateRandom=0;
	}
}

void initPsystemToDefault(PsystemStruct *ps)
{       
ps->active=false;
ps->activeticks=-1;
ps->typeOfPsystem=0;       // future expansion
ps->texHandle=0;           // sane value
ps->billboard=false;       // sane value
v3FloatToVecM(ps->pos1,psCurr%4,0,psCurr/4);
ps->emissionRate = 1;      // number of particles per tick (can be fractional)
ps->emmisionRateRandom=0;  // a max random number which we div by 100 and add to emision rate
ps->residualEmmision=0;
ps->maxParticles=100;      // no more than this please
ps->numParticles=0;        // the actual number 
ps->particleSize=3;        // size in points 
ps->particleSizeDelta=0;   // no change
ps->max_ticks=120;         // after this many ticks die
v3FloatToVec(-10000, 0,-10000, ps->dieIfLess);      // kill the particl if its less than any of these values
v4FloatToVecM(ps->colour,0.1,0.8,0.1,1); // particle color       
v4FloatToVecM(ps->colourChange,0.06,0.06,0.06,0);   // colour drift + added each tick
v4FloatToVecM(ps->colourMax,1,1,1,1); 
v3FloatToVec(0,-0.02,0,ps->gravity);    // velosity change due to gravity
v3FloatToVec(0,0,0,ps->wind);           // movement due to wind
v3FloatToVec(-0.025,0.3,-0.025,ps->initialVelosity);
v3FloatToVec(5,5,5,ps->initialVelosityRandom); // max random number which we dive by 100 and add to initial velosity 
v3FloatToVec(0,0,0,ps->airResistance);       // how it slows
v3FloatToVec(5,5,5,ps->absTerminalVelosity);    // max speed

for (int i=0; i< MAX_PSYSTEM; i++)
    {
    ps->p[i].active=false;   // active Y or n
	ps->p[i].time=0;      // ticks of activity
    v3FloatToVec(0,0,0,ps->p[i].pos1);      // position
    v3FloatToVec(0,0,0,ps->p[i].velosity); // how fast do i move
    }
}

void initPsystems()
{
  for (psCurr=0;psCurr<10;psCurr++) initPsystemToDefault(&(ps[psCurr]));
}
//void initPsystem0()
//{
//	ps[0].gravity.x=0;
//	ps[0].gravity.y=0;
//	ps[0].gravity.z=0;
//
//	ps[0].pos.y=2;
//	v3FloatToVec(-0.025,-0.025,-0.025,ps[0].initialVelosity);
//    v3FloatToVec(5,5,5,ps[0].initialVelosityRandom);
//}
//void initPsystem1()
//{
//	ps[1].maxParticles=10;
//}
//
//void initPsystem2()
//{
//	ps[2].typeOfPsystem=1;
//}
//
//void initPsystem3()
//{
//	ps[3].typeOfPsystem=1;
//	ps[3].activeticks=3;
//	ps[3].emissionRate=100;
//	ps[3].maxParticles=300;
//	ps[3].emmisionRateRandom=100;
//}
//
//void initPsystem4()
//{
//	ps[4].typeOfPsystem=2;
//	ps[4].activeticks=30000;
//	ps[4].emissionRate=0.2;
//	ps[4].maxParticles=10;
//	ps[4].emmisionRateRandom=10;
//	ps[4].particleSize=0.5;         // size in points 
//    ps[4].particleSizeDelta=0.03;   // size change
//	ps[4].texHandle=1;
//	ps[4].billboard=true;
//}
//
//void initPsystem5()
//{
//	ps[5].typeOfPsystem=2;
//	ps[5].activeticks=30000;
//	ps[5].emissionRate=0.2;
//	ps[5].maxParticles=10;
//	ps[5].emmisionRateRandom=10;
//	ps[5].particleSize=0.5;         // size in points 
//    ps[5].particleSizeDelta=0.03;   // size change
//	ps[5].texHandle=1;
//	ps[5].billboard=false;
//}

void initPsystem9(float x, float y, float z)
{
	ps[9].typeOfPsystem=0;
	ps[9].activeticks=30000;
	ps[9].emissionRate=25.2;
	ps[9].maxParticles=200;
	ps[9].emmisionRateRandom=10;
	ps[9].particleSize=1;         // size in points 
    ps[9].particleSizeDelta=0.00;   // size change
	//ps[9].texHandle=1;
	//ps[9].billboard=false;
	ps[9].gravity.x=0;
	ps[9].gravity.y=0;
	ps[9].gravity.z=0;
	ps[9].pos1.x = x;
	ps[9].pos1.y = y;
	ps[9].pos1.z = z;
	//ps[9].pos1 = (x,y,z);
	/*ps[9].pos.x = 87;
	ps[9].pos.y = 66;
	ps[9].pos.z = 157;*/
	
	ps[9].noMoreThanMax=true;
	v3FloatToVec(0,-0.5,2,ps[9].initialVelosity);
    v3FloatToVec(100,100,100,ps[9].initialVelosityRandom);
}

float	lpos1[]	  = { 3,  3, 0.0, 1.0}; // point source close 
float	lpos2[]	  = { 6,  6, 0.0, 1.0}; // point source 
float	lpos3[]	  = {140, 140, 0.0, 0.0}; // distant light
float	ldif[]    = {0.6, 0.6, 0.6, 1.0};
float	lamb[]    = {0.1, 0.1, 0.1, 1.0};
float	lspe[]    = {0.6, 0.6, 0.6, 1.0};

void setUpLights()
{
GLfloat AmbientIntensity[4]; // RGBA color
AmbientIntensity[0]=1; //Red
AmbientIntensity[1]=1; //Green
AmbientIntensity[2]=1; //Blue
AmbientIntensity[3]=1; //Alpha

glEnable(GL_LIGHTING);

glEnable(GL_LIGHT0);
//glLightfv(GL_LIGHT0,GL_POSITION,lpos1);
 //glLightfv(GL_LIGHT0,GL_POSITION,lpos2);
glLightfv(GL_LIGHT0,GL_POSITION,lpos3);
glLightfv(GL_LIGHT0,GL_AMBIENT,lamb);
glLightfv(GL_LIGHT0,GL_DIFFUSE,ldif);
glLightfv(GL_LIGHT0,GL_SPECULAR,lspe);

glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.3);
//glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.3);

glLightModelfv(GL_LIGHT_MODEL_AMBIENT, AmbientIntensity);
//glShadeModel(GL_SMOOTH);

}



/****		Window events		****/

static void seteyePoint()
{
  glLoadIdentity();
  gluLookAt(eyePoint[0], eyePoint[1], eyePoint[2],
	    lookAtPoint[0],lookAtPoint[1],lookAtPoint[2],
	    upVec[0],upVec[1],upVec[2]);
}

static void setProjection3D()
{
  GLfloat aspect;
  /* Set projection Mode */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  aspect = (float)WinWidth / (float)WinHeight;
  gluPerspective(fov, aspect, nearPlane, farPlane);
  /* Back to normal */
  glMatrixMode(GL_MODELVIEW);
  seteyePoint();

  if (hiddenDepth)
  {
	// Now do z buffer
	glEnable(GL_DEPTH_TEST);
	glDepthRange(nearPlane,farPlane);
	glClearDepth(farPlane);
  }
  else
  {
	glDisable(GL_DEPTH_TEST);
  }

  if (hiddenCull)
  {
	  glEnable(GL_CULL_FACE);
	  glCullFace(GL_BACK);
	  if (clockWise) glFrontFace(GL_CW);
	  else glFrontFace(GL_CCW);
  }
  else
  {
	  glDisable(GL_CULL_FACE);
  }
}

static void setProjection2D()
{
  //GLfloat aspect;
  /* Set projection Mode */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  //aspect = (float)WinWidth / (float)WinHeight;
  glOrtho(0, WinWidth, 0, WinHeight, -1.0f, 1.0f);
  /* Back to normal but identity*/
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glDisable(GL_DEPTH_TEST);
}
void showFirstLight()
{
 	    glEnable(GL_LIGHTING);	
		 glEnable(GL_NORMALIZE);
		lightP1->renderA(false); // waring adjusts lighting params
		lightP1->setKind(lightClass::normalL);
		lightP1->setLightId(GL_LIGHT1); // eg GL_LIGHT0, GL_LIGHT1
		lightP1->setPos( 90, 60,  170,  0);
		lightP1->setDiffuse( 0.7f,  0.6f,  0.1f,  1);
		lightP1->setAmbient( 0.1f,  0.05f,  0.05f,  1);
		lightP1->setGlobalAmbient( 0.1f,  0.1f,  0.1f,  1);
		lightP1->setSpecular( 0.7f,  0.6f,  0.1f,  1);
		lightP1->letThereBeLight();
		//mat0->setAmbient(0.329412,0.223529,0.027451,1); // brass
  // mat0->setDiffuse(0.780392,0.568627,0.113725,1);
  // mat0->setShinyInt((int)128*0.21794872);
  // mat0->setSpecular(0.992157,0.941176,0.807843,1);
		////mat0->doMaterial();
	
	
}

void showSecondLight()
{
	    glEnable(GL_LIGHTING);
		glEnable(GL_NORMALIZE);
		lightP2->setKind(lightClass::normalL);
		lightP2->setLightId(GL_LIGHT2); // eg GL_LIGHT0, GL_LIGHT1
		lightP2->setPos( 90, 68,  160,  0);
		//lightP1->setDiffuse( 1.f,  1.f,  1.f,  1);
		//lightP1->setAmbient( 0.1f,  0.1f,  0.1f,  1);
		//lightP1->setGlobalAmbient( 0.1f,  0.1f,  0.1f,  1);
		//lightP1->setSpecular( 1.f,  1.f,  1.f,  1);
		lightP2->setAmbient(0.1,0.1,0.2,1);
        lightP2->setDiffuse(0.1,0.1,1,1);
        lightP2->setSpecular(0.1,0.1,1,1);
		lightP2->letThereBeLight();
		//mat1->doMaterial();
		glPushMatrix();
		glRotatef(spinAngle,0,0,1);
    	lightP2->renderA(false); // waring adjusts lighting params
        GLfloat lightpos[4] = {16 - 2,2 - 2,0,1};
        glLightfv(GL_LIGHT2,GL_POSITION,lightpos);
        glPopMatrix();
	
	
}

void showThirdLight()
{ 
	    glEnable(GL_LIGHTING);
	    glEnable(GL_NORMALIZE);
		lightP3->setKind(lightClass::spotL);
		lightP3->setSpotDir(0,cos(spinAngle),sin(spinAngle));
		lightP3->setSpotAngle(90);
		lightP3->setLightId(GL_LIGHT3); // eg GL_LIGHT0, GL_LIGHT1
		lightP3->setPos( 95, 60, 150,  0);
		lightP3->setRadius(0.5);
		lightP3->setDiffuse( 1.f,  1.f,  1.f,  1);
		lightP3->setAmbient( 0.1f,  0.1f,  0.1f,  1);
		lightP3->setGlobalAmbient( 0.1f,  0.1f,  0.1f,  1);
		lightP3->setSpecular( 1.f,  1.f,  1.f,  1);
		lightP3->letThereBeLight();
		//glPushMatrix();
        //glTranslatef(1,5,0);
		//glRotatef(3*spinAngle,0,0,0);
        lightP3->letThereBeLight();
		//glTranslatef(1,5,0);
        lightP3->renderA(false);
		/*GLfloat lightpos[4] = {1,1,1,1};
        glLightfv(GL_LIGHT3,GL_POSITION,lightpos);
        glPopMatrix();*/

	

}

void movingSpaceShip()
{
	ControlAngle = spinAngle/4;
	DatVar = -spinAngle;
	/*if (DatVar <= -90)
		DatVar = -90;*/
}


static void drawWorld ()
{
	if (showWireFrame==1)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); /* Wireframe mode */
		glLineWidth(1);
	}
	if (showWireFrame==0)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); /* fill mode */
	}

	if (smoothShade) 
	{
		glShadeModel(GL_SMOOTH);
	}
	else
	{
		glShadeModel(GL_FLAT);
	}

  glPushMatrix();
    if(showGrid)
	{
		glColor3f(0.5,0.5,0);
		grid->render();
	}

  if (showSpin)
	{
		glRotatef(spinAngle,0,1,0);
	}
	
	if (showAxes)
	  {
		  axes->render();
	  }

    //glRotatef(SpinAngle, 0.0, 1.0, 0.0);
	//axes->drawAxesP(1);
  
    glColor3fv(color3);
	if (light1)
	{
		showFirstLight();
	}
	else
	{
		glDisable(GL_LIGHT1);
	}

	if (light2)
	{
		showSecondLight();
	}
	else
	{
		glDisable(GL_LIGHT2);
	}

	if (light3)
	{
		showThirdLight();
	}
	else
	{
		glDisable(GL_LIGHT3);
	}

 
	if (showSStation1)
	{
	    //drawSStation1(); 
		//spaceShip->drawSStation1(angle,spinAngle);
		if (showWireFrame==2) hf->renderPoints();
		if (showWireFrame==0) hf->renderTriangles();
		if (showWireFrame==1) hf->renderCrystal();
		//if (showWireFrame==3) hf->renderPoints();
		if (aimMode) {fp->color = vec3(0.8,0.8,0.1);}
		else {fp->color = vec3(0.1,0.1,0.4);}
		fp->render();
		fp->renderProjectile();
	}
	if (showSStation2)
	{
	//drawSStation2(); //???
		glPushMatrix();
		//glScalef(1,1,1);
		glTranslatef(ss1->pos.x,ss1->pos.y - 8,ss1->pos.z - 40 );
		glRotatef(ss1->rotationInY,0,1,0);
		ss1->drawSStation1(0,DatVar,angle,spinAngle);
		if (showBoundingSphere) ss1->renderBoundingSphere();
		glPopMatrix();
		
	}

	if (showSStation3)
	{
		glPushMatrix();
		glTranslatef(ss2->pos.x ,ss2->pos.y + 4,ss2->pos.z);
		glRotatef(ss2->rotationInY,0,1,0);
		ss2->drawSStation1(5,5*DatVar,angle,spinAngle);
		if (showBoundingSphere) ss2->renderBoundingSphere();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(ss2->pos.x ,ss2->pos.y + 6,ss2->pos.z + 5);
		glRotatef(ss2->rotationInY,0,1,0);
		ss2->drawSStation1(5,4*DatVar,angle,spinAngle);
		if (showBoundingSphere) ss2->renderBoundingSphere();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(ss2->pos.x ,ss2->pos.y + 6,ss2->pos.z + 5);
		glRotatef(ss2->rotationInY,0,1,0);
		ss2->drawSStation1(5,2*DatVar,angle,spinAngle);
		if (showBoundingSphere) ss2->renderBoundingSphere();
		glPopMatrix();
		glPushMatrix();
		glTranslatef(90,64.324,139);
		glRotatef(ss2->rotationInY,0,1,0);
		ss2->drawSStation1(0,3*DatVar,angle,spinAngle);
		//if (showBoundingSphere) ss2->renderBoundingSphere();
		glPopMatrix();
	}

	if (showCollide)
	{
		
		glPushMatrix();
		glTranslatef(ss1->pos.x,ss1->pos.y,ss1->pos.z);
		ss1->renderBoundingSphere();
		glPopMatrix();
		showSStation2=FALSE;
	}

	if (movingSStation)
	{
		movingSpaceShip();
	}

  glPopMatrix();

}

static void SetUpLighting()
{
  glEnable(GL_LIGHTING);
 
  glEnable(GL_NORMALIZE);
  if (light0) lightP0->letThereBeLight(); else lightP0->letThereBeDark();
  /*if (lightOn2) light2->letThereBeLight(); else light2->letThereBeDark();
  if (lightOn3) {light3->letThereBeLight(); light3->setPos(-3,yLight3,3,0);} 
  else light3->letThereBeDark();*/
}


void draw2DStuff()
{
	glColor3f(1,1,0);
	glRasterPos2f(30,60);
	//glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, 'A');

	//drawText2d->drawstr(30, 30, "%s (%d)",dispString,45);

	drawText2d->drawstr(10, 530, "%s %8.2f,%8.2f,%8.2f, ","EyePoint",eyePoint[0],eyePoint[1],eyePoint[2]);

	drawText2d->drawstr(10, 500, "%s","2-2D mode and help, 3-3D mode 5-2D & 3D mode");
    drawText2d->drawstr(10, 480, "%s","5 - 2D & 3D mode");
    drawText2d->drawstr(10, 460, "%s","7,8,9 - switch on lights");
    drawText2d->drawstr(10, 440, "%s","l - (lowercase l) toggle lighting");
    drawText2d->drawstr(10, 400, "%s",", and . alter eye position");
    drawText2d->drawstr(10, 360, "%s","g-grid, a-axes, w-Wireframe");
    drawText2d->drawstr(10, 340, "%s","e-Aimmode/EyeMode  Space=file");
    drawText2d->drawstr(10, 320, "%s","o -Space station one  t-space station two");
    drawText2d->drawstr(10, 300, "%s","j -SmoothShade");
    
	//drawText2d->drawstr(10, 290, "%s","t - space station two");
    drawText2d->drawstr(10, 260, "%s","F1 - This help");
    drawText2d->drawstr(10, 230, "%s","Arrows and pgup/pgdown move eyepos or aim");
    drawText2d->drawstr(10, 200, "%s","s-start/stop spin");
    drawText2d->drawstr(10, 170, "%s","F2 - restore start view");

	drawText2d->drawstr(10, 150, "%s","d - depth buffer");
    drawText2d->drawstr(10, 130, "%s","c - cull hidden  @-swaps CW/CCW");
    drawText2d->drawstr(10, 110, "%s","/ - spin speed  b-bounding spheres");
    drawText2d->drawstr(10, 90 , "%s","+,- and = sets angle variable");

	drawText2d->drawstr(10, 70 , "%s","n - Show normals");

  }

static void display()
{
 // SetUpLighting();
	setUpLights();
  if (hiddenDepth) {glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);}
  else {glClear(GL_COLOR_BUFFER_BIT);}

  if (mode == 3 || mode == 5)
	{
		setProjection3D();
		seteyePoint();
		drawWorld();
	}

  if (mode == 2 || mode == 5)
	{
		setProjection2D();
		draw2DStuff();
	}


   //particleSystem.updateParticles();
  // DrawParticles();
  //drawGround();
   for (psCurr=0;psCurr<10;psCurr++) drawPsystem(&(ps[psCurr]));  // This is updated in the idleFunc & drawn here
  err = glGetError();
  if (err == GL_NO_ERROR) 
  /* Check everything OK and update screen */
  CheckGL();
  glutSwapBuffers();

}

static void resize (int width, int height)
{
  /* Save for event handlers */
  WinWidth  = width;
  WinHeight = height;
  
  /* Reset view in window. */
  glViewport(0, 0, WinWidth, WinHeight);
}


/****		User events		****/


static void menuChoice (int item)
{
  switch (item) {
    case cmdRed:
        color3[0]=1; // R
        color3[1]=0; // G
        color3[2]=0; // B
      break;
    case cmdGreen:
        color3[0]=0; // R
        color3[1]=1; // G
        color3[2]=0; // B
      break;
    case cmdExit:
      exit(0);
      break;
    default:
      break;
  }
}


/* In most GUI systems we would write just one event handler
   for all kinds of keystrokes. In GLUT, we need one for the
   standard ASCII keys and one for special cursor or function
   style keys that vary from system to system. Because the
   GLUT special key code range overlaps with ASCII lowercase,
   it isn't safe to use the same function for both.        */

static void asciiKey (unsigned char key, int x, int y)
{
  if (key == ESC)
    menuChoice(cmdExit);

   if (key =='1') {ps[3].active=!ps[3].active;ps[3].activeticks=2;}
  if (key =='2') {mode = 2; strcpy(dispString,"2D Mode");}
  if (key =='3') {mode = 3; strcpy(dispString,"3D Mode");}
  if (key =='5') {mode = 5; strcpy(dispString,"2D+3D Mode");}
  if (key=='0') {initPsystem9(87,66,157);ps[9].active=!ps[9].active;}
  if (key =='7') {light1=!light1;}
  if (key =='8') {light2=!light2;}
  if (key =='9') {light3=!light3;}
  if (key =='l') {showLights=showLights+1; if (showLights >3) showLights=0;}

  if (key =='g') {showGrid=!showGrid;}
  if (key =='a') { showAxes=!showAxes;}
  if (key =='o') { showSStation1=!showSStation1;}
  if (key =='t') { showSStation2=!showSStation2;}
  if (key =='i') { showSStation3=!showSStation3;}
  if (key =='w') { showWireFrame=showWireFrame+1;if(showWireFrame>2)showWireFrame=0;}
  if (key =='s') { showSpin=!showSpin;}
  if (key =='n') { showNormals = !showNormals;hf->hrenderNormal=(bool)showNormals;}
  if (key =='e') { aimMode = !aimMode;}
  if (key =='m')  {movingSStation=!movingSStation;}

  if (key =='d') {hiddenDepth=!hiddenDepth;}
  if (key =='c') {hiddenCull=!hiddenCull;}
  if (key =='@') {clockWise=!clockWise;}
  if (key =='/') {spinIncrement=spinIncrement+1; if (spinIncrement > 4) {spinIncrement=0.4f;}}
  if (key =='j') {smoothShade=!smoothShade;}
  if (key =='b') {showBoundingSphere=!showBoundingSphere;}
  if (key =='p') {showCollide=!showCollide;}

  if (key =='=') {if (angle < 50) angle = 90; else angle = 0;}
  if (key =='+') {angle = angle + 10; angle = (GLfloat)((int)angle % 360);}
  if (key =='-') {angle = angle - 10; angle = (GLfloat)((int)angle % 360);}

  if (key =='.') {eyePoint[0]=eyePoint[0]+2;eyePoint[2]=eyePoint[2]+2;}
  if (key ==',') {eyePoint[0]=eyePoint[0]-2;eyePoint[2]=eyePoint[2]-2;}

  if (key ==' ') {fp->fire();} // fire

}

void setStartValues()
{
  eyePoint[0] = 90.0;
  eyePoint[1] = 70.0;
  eyePoint[2] = 190.0;
  
  lookAtPoint[0]=90;
  lookAtPoint[1]=10;
  lookAtPoint[2]=0;


	/*eyePoint[0] = 0.0;
  eyePoint[1] = 0.0;
  eyePoint[2] = 0.0;
  
  lookAtPoint[0]=90;
  lookAtPoint[1]=10;
  lookAtPoint[2]=0;
*/
  upVec[0]=0;
  upVec[1]=1;
  upVec[2]=0;

  fov=60; // field of view
  nearPlane=0.5;
  farPlane=290;

  spinAngle = 0.0;
}

static void specialKey (int key, int x, int y)
{
  
  if (key==GLUT_KEY_F1) {mode = 2;}

  if (key==GLUT_KEY_F2)
  {
  setStartValues();
  }

  if (key==GLUT_KEY_LEFT  && !aimMode) eyePoint[0]=eyePoint[0]+2;
  if (key==GLUT_KEY_RIGHT && !aimMode) eyePoint[0]=eyePoint[0]-2;
  if (key==GLUT_KEY_UP && !aimMode) eyePoint[2]=eyePoint[2]-2;
  if (key==GLUT_KEY_DOWN && !aimMode) eyePoint[2]=eyePoint[2]+2;
  if (key==GLUT_KEY_PAGE_UP && !aimMode) eyePoint[1]=eyePoint[1]+2;
  if (key==GLUT_KEY_PAGE_DOWN && !aimMode) eyePoint[1]=eyePoint[1]-2;

  if (key==GLUT_KEY_LEFT  && aimMode) fp->aimLeft() ;
  if (key==GLUT_KEY_RIGHT && aimMode) fp->aimRight();
  if (key==GLUT_KEY_UP && aimMode) fp->aimUp();
  if (key==GLUT_KEY_DOWN && aimMode) fp->aimDown();
  //if (key==GLUT_KEY_PAGE_UP && aimMode) ;
  //if (key==GLUT_KEY_PAGE_DOWN && aimMode) ;

}

void exitFunction(void)
{
  delete cube;
  delete axes;
  delete drawText2d; 
  delete grid;
  delete hf;
  delete fp;
  delete ss1;
}


/****		Startup			****/

static void initGraphics (void)
{
  /* Black background 
  glClearColor(0, 0, 0, 0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  /* colour green */
  color3[0]=0;
  color3[0]=1;
  color3[0]=0;

  glEnable(GL_COLOR);
  glEnable(GL_DEPTH_TEST);

  /* Needed for vertex arrays */
  //glEnableClientState(GL_VERTEX_ARRAY);
  
  /* Popup menu attached to right mouse button */
  AppMenu = glutCreateMenu(menuChoice);
  glutSetMenu(AppMenu);
  glutAddMenuEntry("Red", cmdRed);
  glutAddMenuEntry("Green", cmdGreen);
  glutAddMenuEntry("----", 0);
  glutAddMenuEntry("Exit", cmdExit);
  glutAttachMenu(GLUT_RIGHT_BUTTON);
  
  /* Start values */
   setStartValues();

  menuChoice(cmdGreen);

  cube = new CubeClass();
  axes = new Axes();
  drawText2d = new Text2D();

  lightP0 = new lightClass();
  lightP1 = new lightClass();
  lightP2 = new lightClass();
  lightP3 = new lightClass();
	
 
//  psc = new particleSystemClass();
  grid = new Grid();
  grid->setUp(5,5,5,5);

   lightP0->setGlobalAmbient(0.2,0.2,0.2,1);

   lightP0->setSpotDir(0,20,0);
   lightP0->setLightId(GL_LIGHT0);
   lightP0->setAmbient(0.1,0.1,0.1,1); // just a bit of ambient white
   lightP0->setDiffuse(1,1,0.9f,1); // fair bit of diffuse white
   lightP0->setSpecular(1,1,0.9f,1); //fair bit of spec white 
   lightP0->setPos(3,7,3,0);


  hf = new HeightField();
  fp = new FiringPlatform();
  /*glPushMatrix();
  glTranslatef(90,64,184);
*/
 // hf->setupNormals();
 

  //glPopMatrix();
  ss1 = new SpaceStation1();
  ss2 = new SpaceStation1();
  ss3 = new SpaceStation1();

 /* psc->initPsystems();
  psc->initPsystem0();
  psc->initPsystem1();
  psc->initPsystem3();
  psc->initPsystem4();
  psc->initPsystem5();
 */ 
 /* particleSystem.setSystemType(1);
   particleSystem.createParticles();
*/
//	texture[0] = LoadTextureRAW( "particle_mask.raw",256,256); //load alpha for texture
//	texture[1] = LoadTextureRAW( "particle.raw",256,256); //load texture

  initPsystems();
  //initPsystem9();
  /*eyePos.x=6;
  eyePos.y=4;
  eyePos.z=6;*/
  atexit (exitFunction);
}


void timer(int value)
{
timeForNextFrame=true;
glutTimerFunc(timerMSecs,timer,0);
}

/****		Main control		****/


static void idleRoutine(void)
{
  bool colision=false;

  if (timeForNextFrame)
  {
	  for (psCurr=0;psCurr<10;psCurr++) updatePsystem(&(ps[psCurr]));
	  spinAngle += spinIncrement;
	if (spinAngle > 360.0)
			spinAngle -= 360.0;
	
	glutPostRedisplay();
	ss1->timer();
	fp->timer();

	if (ss1->active) // test for collision
	{
		printf("%6.3f",fp->projectilePos.x);
	 printf(" ");
     printf("%6.3f",fp->projectilePos.y);
	 printf(" ");
	 printf("%6.3f \n",fp->projectilePos.z);
		colision = ss1->collisionWithSphere( fp->projectilePos.x,  fp->projectilePos.y,  fp->projectilePos.z,  1);
		if (colision) 
		{
				/*ss1->active = false;
				ss1->renderBoundingSphere();*/
			//particleSystem.updateParticles();
				//DrawParticles();
			    //showCollide = TRUE;
				//psc->drawPsystem(&(ps1[3]));
			   // ps[9].pos = (fp->projectilePos.x,fp->projectilePos.y,fp->projectilePos.z - 30);
		      
				//collPos= (87,66,157);
				//collPos = (fp->projectilePos.x,fp->projectilePos.y,fp->projectilePos.z);
			    ps[9].active=true;initPsystem9(ss1->pos.x,ss1->pos.y - 8,ss1->pos.z - 40);
				  ss1->active = false;
				//ps[9]->colour,0.1,0.8,0.1,1;
				fp->reload();
				// need to deal wth bounding sphere of station
		}
	}

	timeForNextFrame=false;

	if (angleVar < angle )
	{
		angleVar = angleVar+2;
		if (angleVar > angle ) angleVar = angle;
	}

	if (angleVar > angle )
	{
		angleVar = angleVar-2;
		if (angleVar < angle ) angleVar = angle;
	}

  }

}

int main (int argc, char * argv[])
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);

  glutInitWindowSize(550, 550);
  glutInitWindowPosition(100, 75);
  glutCreateWindow("Cube");

  initGraphics();
  
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  
  glutKeyboardFunc(asciiKey);
  glutSpecialFunc(specialKey);
  
  glutIdleFunc(idleRoutine);
  glutTimerFunc(timerMSecs,timer,0);
  
  glutMainLoop();
  /* Should never get here, but keeps compiler happy */
  return 0;
}


// Functions to load RAW files
// I did not write the following functions.
// They are form the OpenGL tutorials at http://www.swiftless.com
GLuint LoadTextureRAW( const char * filename, int width, int height )
{
  GLuint texture;
  unsigned char * data;
  FILE * file;
  file = fopen( filename, "rb" );
  if ( file == NULL ) return 0;
  data = (unsigned char *)malloc( width * height * 3 );
  fread( data, width * height * 3, 1, file );
  fclose( file );
  glGenTextures(1, &texture );
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
  free( data );
  return texture;
}

void FreeTexture( GLuint texture )
{
  glDeleteTextures( 1, &texture );
}
