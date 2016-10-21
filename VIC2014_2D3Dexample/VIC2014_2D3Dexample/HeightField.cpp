// Heightfield.cpp

#include "HeightField.h"
#include "vec3.h"

#define SCALE_FACTOR 0.01

#define XX 0
#define YY 1
#define ZZ 2



#define TRI0 0
#define TRI1 1
#define TRI2 2
#define TRI3 3
#define TRI4 4
#define TRI5 5

#define POINT0 0
#define POINT1 1
#define POINT2 2


#define LOW_RED 0.9f
#define LOW_GREEN 0.9f
#define LOW_BLUE 0.9f

#define HIGH_RED 0.0f
#define HIGH_GREEN 0.0f
#define HIGH_BLUE 0.0f

#define WATER_LEVEL 0


vec3 pnormal1; // temp for polygon normal
vec3 pnormal2; // temp for polygon normal
vec3 pnormal3;
vec3 pnormal4;
vec3 pnormal5;
vec3 pnormal6;
vec3 pnormal7;
vec3 Averagepnormal;


vec3 pnormalEnd11; // temp for polygon normal render
vec3 pnormalEnd21; // temp for polygon normal render

vec3 p00;
vec3 p01;
vec3 p02;

vec3 p03;
vec3 p04;
vec3 p05;
vec3 p06;
vec3 p07;



vec3 p10;
vec3 p11;
vec3 p12;

vec3 p20;
vec3 p21;
vec3 p22;


vec3 temp01;
vec3 temp02;


bool hrenderNormal;

HeightField::HeightField()
{
hrenderNormal=false;
mat= new materialClass();
matLines= new materialClass(); 
mat->setMaterialJade();
matLines->setMaterialBlackRubber();

matHigh=new materialClass();
matBeach=new materialClass();
matLand=new materialClass();

matHigh->setMaterialWhiteStuff();
matBeach->setMaterialYellowSand();
matLand->setMaterialGreenGrass();
int rc; 
maxHeight = 0;
rc = readHeightField("X22_0708.txt", &hfWidth, &hfDepth, heights);
//rc = readHeightField("X22_0214.txt", &hfWidth, &hfDepth, heights);
for (int x=0; x<hfWidth; x++)
	{
	for (int y=0; y<hfDepth; y++) 
		{
		h[x][y]=((float)heights[x][y])/100;
		if (h[x][y]> maxHeight)
		{
			maxHeight = h[x][y];
		}
		}
	}

for (int x=0; x<hfWidth-1; x++)
{
	for (int z=0;z<hfDepth-1;z++)
	{
	
		htri[x][z][TRI0][POINT0][XX]=x;
		htri[x][z][TRI0][POINT0][YY]=h[x][z];
		htri[x][z][TRI0][POINT0][ZZ]=z;
		p00 = htri[x][z][TRI0][POINT0];
		setColour(htri[x][z][TRI0][POINT0][YY]);
		htri[x][z][TRI0][POINT1][XX]=x;
		htri[x][z][TRI0][POINT1][YY]=h[x][z+1];
		htri[x][z][TRI0][POINT1][ZZ]=z+1;
		p01 =htri[x][z][TRI0][POINT1];
		setColour(p01.y);
		htri[x][z][TRI0][POINT2][XX]=x+1;
		htri[x][z][TRI0][POINT2][YY]=h[x+1][z];
		htri[x][z][TRI0][POINT2][ZZ]=z;
		p02 =htri[x][z][TRI0][POINT2];
		setColour(p02.y);
		
		htri[x][z][TRI1][POINT0][XX]=x+1;
		htri[x][z][TRI1][POINT0][YY]=h[x+1][z];
		htri[x][z][TRI1][POINT0][ZZ]=z;
	
		htri[x][z][TRI1][POINT1][XX]=x;
		htri[x][z][TRI1][POINT1][YY]=h[x][z+1];
		htri[x][z][TRI1][POINT1][ZZ]=z+1;
	
		htri[x][z][TRI1][POINT2][XX]=x+1;
		htri[x][z][TRI1][POINT2][YY]=h[x+1][z+1];
		htri[x][z][TRI1][POINT2][ZZ]=z+1;
		p03 =htri[x][z][TRI1][POINT2];
		htri[x][z][TRI2][POINT0][XX]=x+1;
		htri[x][z][TRI2][POINT0][YY]=h[x+1][z+1];
		htri[x][z][TRI2][POINT0][ZZ]=z+1;
	
		htri[x][z][TRI2][POINT1][XX]=x;
		htri[x][z][TRI2][POINT1][YY]=h[x][z+1];
		htri[x][z][TRI2][POINT1][ZZ]=z+1;
	
		htri[x][z][TRI2][POINT2][XX]=x+2;
		htri[x][z][TRI2][POINT2][YY]=h[x+2][z+1];
		htri[x][z][TRI2][POINT2][ZZ]=z+1;
		p04 =htri[x][z][TRI2][POINT2];
		htri[x][z][TRI3][POINT0][XX]=x+2;
		htri[x][z][TRI3][POINT0][YY]=h[x+2][z+1];
		htri[x][z][TRI3][POINT0][ZZ]=z+1;
	
		htri[x][z][TRI3][POINT1][XX]=x;
		htri[x][z][TRI3][POINT1][YY]=h[x][z+1];
		htri[x][z][TRI3][POINT1][ZZ]=z+1;
	
		htri[x][z][TRI3][POINT2][XX]=x+2;
		htri[x][z][TRI3][POINT2][YY]=h[x+2][z+2];
		htri[x][z][TRI3][POINT2][ZZ]=z+2;
		p05 =htri[x][z][TRI3][POINT2];
		htri[x][z][TRI4][POINT0][XX]=x+2;
		htri[x][z][TRI4][POINT0][YY]=h[x+2][z+2];
		htri[x][z][TRI4][POINT0][ZZ]=z+2;
	
		htri[x][z][TRI4][POINT1][XX]=x;
		htri[x][z][TRI4][POINT1][YY]=h[x][z+1];
		htri[x][z][TRI4][POINT1][ZZ]=z+1;
	
		htri[x][z][TRI4][POINT2][XX]=x+3;
		htri[x][z][TRI4][POINT2][YY]=h[x+3][z+2];
		htri[x][z][TRI4][POINT2][ZZ]=z+2;
		p06 =htri[x][z][TRI4][POINT2];
		htri[x][z][TRI5][POINT0][XX]=x+3;
		htri[x][z][TRI5][POINT0][YY]=h[x+3][z+2];
		htri[x][z][TRI5][POINT0][ZZ]=z+2;
	
		htri[x][z][TRI5][POINT1][XX]=x;
		htri[x][z][TRI5][POINT1][YY]=h[x][z+1];
		htri[x][z][TRI5][POINT1][ZZ]=z+1;
	
		htri[x][z][TRI5][POINT2][XX]=x+3;
		htri[x][z][TRI5][POINT2][YY]=h[x+3][z+3];
		htri[x][z][TRI5][POINT2][ZZ]=z+3;
	    p07 =htri[x][z][TRI5][POINT2];

		pnormal1 = calculateNormal(p00,p01,p02);
		pnormal2 = calculateNormal(p02,p01,p03);
		pnormal3 = calculateNormal(p03,p01,p04);
		pnormal4 = calculateNormal(p04,p01,p05);
		pnormal5 = calculateNormal(p05,p01,p06);
		pnormal6 = calculateNormal(p06,p01,p07);
		pnormal7 = calculateNormal(p07,p01,p00);
		//Averagepnormal	= (pnormal1 + pnormal2 + pnormal3 + pnormal4 + pnormal5 + pnormal6 + pnormal7)/6.0f;
		Averagepnormal	= (pnormal1 + pnormal2 )/2.0f;
		
		hnorm[x][z][XX] = Averagepnormal.x;
		hnorm[x][z][YY] = Averagepnormal.y;
		hnorm[x][z][ZZ] = Averagepnormal.z;
	
		/*hnorm[x][z][XX] = pnormal1.x;
		hnorm[x][z][YY] = pnormal1.y;
		hnorm[x][z][ZZ] = pnormal1.z;*/
	/*    if (htri[x][z][TRI0][POINT1][YY]=h[x][z+1]  < 0 )
		{
		hnorm[x][z][XX] = pnormal1.x;
		hnorm[x][z][YY] = pnormal1.y;
		hnorm[x][z][ZZ] = pnormal1.z;
		
		}
		else
		{
		hnorm[x][z][XX] = Averagepnormal.x;
		hnorm[x][z][YY] = Averagepnormal.y;
		hnorm[x][z][ZZ] = Averagepnormal.z;
		}*/
	}
		
}


setup(hfWidth,hfDepth,hfWidth,hfDepth);

//for (int x=0; x<hfWidth-1; x++)
//{
//	for (int z=0;z<hfDepth-1;z++)
//	{
//		pnormalEnd11 = htri[x][z][TRI0][POINT1];
//		pnormalEnd21 = pnormalEnd11 + hnorm[x][z];
//		pnormal2 =  hnorm[x][z];
//
//		if (hrenderNormal)
//		{
//			glBegin(GL_LINES);
//			    glColor3f(1,0,0);
//				glVertex3fv(pnormalEnd11.fv());
//				glColor3f(0,1,0);
//				glVertex3fv(pnormalEnd21.fv());
//			glEnd();
//		}
//	}
//}

highHeight=20;
grassHeight=0.5;

}


HeightField::~HeightField()
{
}

vec3 HeightField::calculateNormal(vec3 p1,vec3 p2, vec3 p3)
{
	vec3 U = p2 - p1;
	vec3 V = p3 - p2;
	vec3 cross;
	cross.x = U.x*V.z - U.z*V.y;
	cross.y = U.z*V.x - U.x*V.z;
	cross.z = U.x*V.y - U.y*V.x;
	return normalize(cross);	
}


int HeightField::setupNormals()
{

//for (int x=0; x<hfWidth-1; x++)
//{
//	for (int z=0;z<hfDepth-1;z++)
//	{
//		pnormalEnd11 = htri[x][z][TRI0][POINT1];
//		pnormalEnd21 = pnormalEnd11 + hnorm[x][z];
//		pnormal2 =  hnorm[x][z];
//
//		if (hrenderNormal)
//		{
//			glBegin(GL_LINES);
//			    glColor3f(1,0,0);
//				glVertex3fv(pnormalEnd11.fv());
//				glColor3f(0,1,0);
//				glVertex3fv(pnormalEnd21.fv());
//			glEnd();
//		}
//	}
//}

return 0;
}

void HeightField::setMaterialHeight(GLfloat height)
{
//	materialClass* matHigh;
//materialClass* matBeach;
//materialClass* matLand;

//GLfloat highHeight;
//GLfloat grassHeight;
	if (height >= highHeight) 
		matHigh->doMaterial(); 
	else 
	{
		if (height > grassHeight) 
		     matLand->doMaterial(); 
	     else
			 matBeach->doMaterial();
	}
}


void HeightField::setColour(int height) // warning variable maxHeight must be set to use this routine
{
	GLfloat r,g,b;
	GLfloat factor;

	/*if (height >= highHeight)
		{
			matHigh -> doMaterial();
		}
		else 
	{
		if (height > grassHeight) 
		     matLand->doMaterial(); 
	     else
			 matBeach->doMaterial();
	}*/

	if (height<=WATER_LEVEL) 
		{
			r=0.1f; g=0.1f; b=0.9f;
			matBeach->doMaterial();
	    }
	if (height > grassHeight)
	{
		matLand->doMaterial(); 
		factor = ((float)height/(maxHeight-WATER_LEVEL));
		r=(HIGH_RED-LOW_RED)*factor+LOW_RED;
		g=(HIGH_GREEN-LOW_GREEN)*factor+LOW_GREEN;
		b=(HIGH_BLUE-LOW_BLUE)*factor+LOW_BLUE;
	}
	if (height >= highHeight)
	{
		matHigh->doMaterial(); 
		factor = ((float)height/(maxHeight-WATER_LEVEL));
		r=(HIGH_RED-LOW_RED)*factor+LOW_RED;
		g=(HIGH_GREEN-LOW_GREEN)*factor+LOW_GREEN;
		b=(HIGH_BLUE-LOW_BLUE)*factor+LOW_BLUE;
	}
	
	glColor3f(r,g,b);
	
}

int HeightField::setup(GLfloat xsizeZ, GLfloat zsizeZ, int xstepsZ, int zstepsZ)
{
xsize=xsizeZ;
zsize=zsizeZ;
xsteps=xstepsZ;
zsteps=zstepsZ;
return 0;
}


int HeightField::renderTriangles()
{
glPushMatrix();

//glTranslatef(80,64,160);
for (int x=0; x<hfWidth-1; x++)
{
	for (int z=0;z<hfDepth-1;z++)
//for (int x=0; x<3; x++)
//{
//	for (int z=0;z<3;z++)
	{
		glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
				
		//setColour(htri[x][z][TRI0][POINT0][YY]);
		//setMaterialHeight(htri[x][z][TRI0][POINT0][YY]);
		p10 = htri[x][z][TRI0][POINT0];
		//setMaterialHeight(p10.y);
		setColour(p10.y);
		p11 = htri[x][z][TRI0][POINT1];
		//setMaterialHeight(p11.y);
		setColour(p11.y);
		p12 = htri[x][z][TRI0][POINT2];
		//setMaterialHeight(p12.y);
		setColour(p12.y);
		pnormalEnd11 = htri[x][z][TRI0][POINT1];
		pnormalEnd21 = pnormalEnd11 + hnorm[x][z] + hnorm[x][z];
		pnormal2 =  hnorm[x][z];

		glBegin(GL_TRIANGLES);
		glNormal3fv(pnormal2.fv());
		glVertex3fv(p10.fv());
		glVertex3fv(p11.fv());
		glVertex3fv(p12.fv());
		glEnd();

		p20 = htri[x][z][TRI1][POINT0];
		//setMaterialHeight(p20.y);
		setColour(p20.y);
		p21 = htri[x][z][TRI1][POINT1];
		//setMaterialHeight(p21.y);
		setColour(p21.y);
		p22 = htri[x][z][TRI1][POINT2];
		//setMaterialHeight(p22.y);
		setColour(p22.y);
	
		glBegin(GL_TRIANGLES);
		glNormal3fv(pnormal2.fv());
		glVertex3fv(p20.fv());
		glVertex3fv(p21.fv());
		glVertex3fv(p22.fv());
		glEnd();

		if (hrenderNormal)
		{
			glLineWidth(1);
			glBegin(GL_LINES);	
			    glColor3f(1,0,0);
				glVertex3fv(pnormalEnd11.fv());
				glColor3f(0,1,0);
				glVertex3fv(pnormalEnd21.fv());
			   
			glEnd();
		}
	}
}

glPopMatrix();
return true;
}

int HeightField::renderCrystal()
{
glPushMatrix();
//glTranslatef(80,64,160);
for (int x=0; x<hfWidth-1; x++)
{
	for (int z=0;z<hfDepth-1;z++)

//for (int x=0; x<1; x++)
//{
//	for (int z=0;z<1;z++)

	{

		glColorMaterial(GL_FRONT,  GL_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
			
		
		p10 = htri[x][z][TRI0][POINT0];
		//setMaterialHeight(p10.y);
		setColour(p10.y);
		p11 = htri[x][z][TRI0][POINT1];
		//setMaterialHeight(p11.y);
		setColour(p11.y);
		p12 = htri[x][z][TRI0][POINT2];
		//setMaterialHeight(p12.y);
		setColour(p12.y);
		pnormalEnd11 = htri[x][z][TRI0][POINT1];
		pnormalEnd21 = pnormalEnd11 + hnorm[x][z];
		pnormal2 =  hnorm[x][z];

		glBegin(GL_TRIANGLES);
		glNormal3fv(pnormal2.fv());
		glVertex3fv(p10.fv());
		glVertex3fv(p11.fv());
		glVertex3fv(p12.fv());
		glEnd();

		p20 = htri[x][z][TRI1][POINT0];
		//setMaterialHeight(p20.y);
		setColour(p20.y);
		p21 = htri[x][z][TRI1][POINT1];
		//setMaterialHeight(p21.y);
		setColour(p21.y);
		p22 = htri[x][z][TRI1][POINT2];
		//setMaterialHeight(p22.y);
		setColour(p22.y);
	
		glBegin(GL_TRIANGLES);
		glNormal3fv(pnormal2.fv());
		glVertex3fv(p20.fv());
		glVertex3fv(p21.fv());
		glVertex3fv(p22.fv());
		glEnd();

		if (hrenderNormal)
		{
			glBegin(GL_LINES);
			    glColor3f(1,0,0);
				glVertex3fv(pnormalEnd11.fv());
				glColor3f(0,1,0);
				glVertex3fv(pnormalEnd21.fv());
			   
			glEnd();
		}
	}
}

glPopMatrix();
return true;
}

int HeightField::renderPoints()
{
glPushMatrix();

for (int x=0; x<hfWidth-1; x++)
{
	for (int z=0;z<hfDepth-1;z++)
	{
			glColorMaterial(GL_FRONT, GL_DIFFUSE);
		glEnable(GL_COLOR_MATERIAL);
	
		p10 = htri[x][z][TRI0][POINT0];
		//setMaterialHeight(p10.y);
		setColour(p10.y);
		p11 = htri[x][z][TRI0][POINT1];
		//setMaterialHeight(p11.y);
		setColour(p11.y);
		p12 = htri[x][z][TRI0][POINT2];
		//setMaterialHeight(p12.y);
		setColour(p12.y);
		pnormalEnd11 = htri[x][z][TRI0][POINT1];
		pnormalEnd21 = pnormalEnd11 + hnorm[x][z];
		pnormal2 =  hnorm[x][z];

		glBegin(GL_POINTS);
		glNormal3fv(pnormal2.fv());
		glVertex3fv(p10.fv());
		glVertex3fv(p11.fv());
		glVertex3fv(p12.fv());
		glEnd();

		p20 = htri[x][z][TRI1][POINT0];
		//setMaterialHeight(p20.y);
		setColour(p20.y);
		p21 = htri[x][z][TRI1][POINT1];
		//setMaterialHeight(p21.y);
		setColour(p21.y);
		p22 = htri[x][z][TRI1][POINT2];
		//setMaterialHeight(p22.y);
		setColour(p22.y);
	
		glBegin(GL_POINTS);
		glNormal3fv(pnormal2.fv());
		glVertex3fv(p20.fv());
		glVertex3fv(p21.fv());
		glVertex3fv(p22.fv());
		glEnd();


		if (hrenderNormal)
		{
			glLineWidth(1);
			glBegin(GL_LINES);
			    glColor3f(1,0,0);
				glVertex3fv(pnormalEnd11.fv());
				glColor3f(0,1,0);
				glVertex3fv(pnormalEnd21.fv());
			   
			glEnd();
		}
	}
}

glPopMatrix();
return true;
}

//