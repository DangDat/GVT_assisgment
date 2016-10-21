#ifndef RENDERABLE
#define RENDERABLE
//renderable.h 

#include "glut.h"
#include "vec3.h"

class renderableParent
{
protected:
GLfloat	xScale, yScale, zScale;
GLfloat	xPos, yPos, zPos;
GLfloat	xRot, yRot, zRot;

public:
virtual void setTranslate(GLfloat xx, GLfloat yy, GLfloat zz);
virtual void setRot(GLfloat xx, GLfloat yy, GLfloat zz); 
virtual void setScale(GLfloat xx, GLfloat yy, GLfloat zz); 
virtual int render()=0; // returns (true)if succeed false if not
virtual void preset(); // presets rotation and position and scale
virtual void init();
virtual void doTranslate();
virtual void doScale();
virtual void doRotate();
virtual void undoRotate();
renderableParent();

};

// end
#endif 