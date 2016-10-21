// shapes.cpp
#include <windows.h>
#include "lights.h"

lightClass::lightClass()
{
	init();
	mat=new materialClass();
	mat->setAmbient(0,0,0,1);
	mat->setDiffuse(0,0,0,1);
	mat->setSpecular(0,0,0,1);
}

lightClass::~lightClass()
{
	gluDeleteQuadric(quadric1);
	delete mat;
}

void lightClass::setLightId(int id) // eg GL_LIGHT0, GL_LIGHT1
{
	lightID=id;
}

void lightClass::init()
{
    // some 'sane' defaults
	//d=0.5; green=0.7; blue=0.3; // puse green colour
	radius=0.2f;
	setKind(normalL); 
	quadric1=gluNewQuadric();
	setPos(5,5,5,0);
	setDiffuse(1,1,1,1);
    setAmbient(0,0,0,1);
    setSpecular(0,0,0,1);
	setGlobalAmbient(0,0,0,1);
	setSpotDir(-1,-1,-1);
	setSpotAngle(45);
    lightID=GL_LIGHT0;
}

void lightClass::setPos(GLfloat x, GLfloat y, GLfloat z, GLfloat w)
{
	lpos[0]=x;
	lpos[1]=y;
	lpos[2]=z;
	lpos[3]=w;
}

void lightClass::setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
    ldif[0]=r;    
    ldif[1]=g;    
    ldif[2]=b;    
    ldif[3]=a;    
}

void lightClass::setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{ 
	lamb[0]=r; 
	lamb[1]=g; 
	lamb[2]=b; 
	lamb[3]=a; 
}

void lightClass::setGlobalAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{ 
	lmodel_ambient[0]=r; 
	lmodel_ambient[1]=g; 
	lmodel_ambient[2]=b; 
	lmodel_ambient[3]=a;
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient); // just for main light 0

}

void lightClass::setSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{ 
	lspe[0]=r;
	lspe[1]=g;
	lspe[2]=b;
	lspe[3]=a;
}

void lightClass::setRadius(GLfloat radiuz)
{
	radius=radiuz;
}


void lightClass::setKind(lightKind k)
{
	kind=k;
}

int lightClass::render()
{
renderA(false);
return true;
}

void lightClass::renderA(bool notColour)
{
if (!showAsSphere) return ;

//glColor3f(red, green, blue);

glPushMatrix();
mat->setEmissive(ldif[0],ldif[1],ldif[2],1);
if (notColour) mat->setEmissive(0.5,0.5,0.5,1);
mat->doMaterial();

glutSolidSphere(radius, 11, 11);

if (kind==spotL)
	{
		glBegin(GL_LINES );
		glVertex3f(0,0,0);
		glVertex3f(spotLightDir[0],spotLightDir[1],spotLightDir[2]);
		glEnd();
	}
glPopMatrix();
return ;
}

void lightClass::letThereBeDark() // put the light into the scene
{
glDisable(lightID);
}

void lightClass::letThereBeLight() // put the light into the scene
{
//glPushMatrix();
glEnable(lightID);
glLightfv(lightID,GL_POSITION,lpos);
glLightfv(lightID,GL_AMBIENT,lamb);
glLightfv(lightID,GL_DIFFUSE,ldif);
glLightfv(lightID,GL_SPECULAR,lspe);
if (kind==spotL)
	{
		glLightfv(lightID, GL_SPOT_DIRECTION, spotLightDir);
		glLightf(lightID, GL_SPOT_CUTOFF,spotAngle);
	}
else
	{
		glLightf(lightID, GL_SPOT_CUTOFF,180);
	}
//glPopMatrix();
}

void lightClass::setSpotDir(GLfloat x, GLfloat y, GLfloat z)
{
	spotLightDir[0]=x;
	spotLightDir[1]=y;
	spotLightDir[2]=z;
}

void lightClass::setSpotAngle(GLfloat a)
{
	spotAngle=a;
}

// ---------------------------------------------------------------------------------------------------

materialClass::materialClass()
{
// white rubber aS DEFAULT 
setMaterialWhiteRubber();
}

void materialClass::setDiffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	dif[0]=r;
	dif[1]=g;
	dif[2]=b;
	dif[3]=a;
}

void materialClass::setAmbient(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	amb[0]=r;
	amb[1]=g;
	amb[2]=b;
	amb[3]=a;
}

void materialClass::setSpecular(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	spe[0]=r;
	spe[1]=g;
	spe[2]=b;
	spe[3]=a;
}

void materialClass::setEmissive(GLfloat r, GLfloat g, GLfloat b, GLfloat a)
{
	eme[0]=r;
	eme[1]=g;
	eme[2]=b;
	eme[3]=a;
}

void  materialClass::doMaterial()
{
  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT,amb);	
  glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE,dif);	
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,spe);	
  glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,eme);	
  glMateriali(GL_FRONT_AND_BACK,GL_SHININESS,shiny);
}

void materialClass::setShinyInt(int shinyZ)
{
	shiny = shinyZ;
}

void materialClass::setShinyFloat(GLfloat shinyZ)
{
	shiny = (int)shinyZ;
}

void materialClass::setMaterialJade()
{
setAmbient(0.135f, 0.2225f, 0.1575f ,1);
setDiffuse(0.54f, 0.89f, 0.63f ,1);
setSpecular(0.316228f, 0.316228f, 0.316228f ,1);
setEmissive(0,0,0,0);
setShinyInt((int)(0.1*128.0));
}

void materialClass::setMaterialRedPlastic()
{
setAmbient(0.0,0.0,0.0,1);
setDiffuse(0.5f,0,0,1);
setSpecular(0.7f,0.6f,0.6f,1);
setEmissive(0,0,0,0);
setShinyInt((int)(0.25*128.0));
}

void materialClass::setMaterialWhiteRubber()
{
setAmbient(0.05f,0.05f,0.05f,1);
setDiffuse(0.5f,0.5f,0.5f,1);
setSpecular(0.7f,0.7f,0.7f,1);
setEmissive(0,0,0,0);
setShinyInt((int)(0.078125*128.0));
}

void materialClass::setMaterialBlackRubber()
{
setAmbient(0.02f, 0.02f, 0.02f ,1);
setDiffuse(0.01f, 0.01f, 0.01f ,1);
setSpecular(0.4f, 0.4f, 0.4f ,1);
setEmissive(0,0,0,0);
setShinyInt((int)(0.078125*128.0));
}

//pearl	0.25	0.20725	0.20725	1	0.829	0.829	0.296648	0.296648	0.296648	0.088
// from http://devernay.free.fr/cours/opengl/materials.html
void materialClass::setMaterialPearl()
{
setAmbient(0.25f, 0.207f, 0.207f ,1);
setDiffuse(1.0f, 0.829f, 0.829f ,1);
setSpecular(0.297f, 0.297f, 0.297f ,1);
setEmissive(0,0,0,0);
setShinyInt((int)(0.088*128.0));
}

void materialClass::setMaterialWhiteStuff2()
{
setAmbient(0.25f, 0.207f, 0.207f ,1);
setDiffuse(1.0f, 0.829f, 0.829f ,1);
setSpecular(0.597f, 0.597f, 0.597f ,1); // shinier than whitestuff
setEmissive(0,0,0,0);
setShinyInt((int)(0.108*128.0));
}

void materialClass::setMaterialYellowSand()
{
setAmbient(0.4f, 0.4f, 0.1575f ,1);
setDiffuse(0.7f, 0.7f, 0.2f ,1);
setSpecular(0.316228f, 0.316228f, 0.216228f ,1);
setEmissive(0,0,0,0);
setShinyInt((int)(0.1*128.0));
}

void materialClass::setMaterialGreenGrass()
{
setAmbient(0.13f, 0.25f, 0.15f ,1);
setDiffuse(0.24f, 0.89f, 0.33f ,1);
setSpecular(0.116228f, 0.316228f, 0.116228f ,1);
setEmissive(0,0,0,0);
setShinyInt((int)(0.1*128.0));
}

void materialClass::setMaterialBlue()
{
setAmbient(0.1f, 0.1f, 0.2f ,1);
setDiffuse(0.3f, 0.3f, 0.8f ,1);
setSpecular(0.316228f, 0.316228f, 0.316228f ,1);
setEmissive(0,0,0,0);
setShinyInt((int)(0.1*128.0));
}

void materialClass::setMaterialWhiteStuff() // suitable for lighting a texture
{
setAmbient(0.7f, 0.7f, 0.7f ,1);
setDiffuse(0.9f, 0.9f, 0.9f ,1);
setSpecular(0.6f, 0.6f, 0.6f ,1);
setEmissive(0,0,0,0);
setShinyInt((int)(12));
}
// end