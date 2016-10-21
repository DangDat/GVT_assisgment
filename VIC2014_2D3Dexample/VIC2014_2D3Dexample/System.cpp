/******************************************************
 * Georg Albrecht                                     *
 * Final Project for CMPS 161, Winter 2009            *
 *                                                    *
 * System.cpp                                         *
 *    This is the source file for System.h. It        *
 *    contains the code necessary to initalize, update*
 *    and manage and array of particles               *
 ******************************************************/

#include "System.h"

System::System(void)
{
}

System::~System(void)
{
}

/*
 * initalizes a single particle according to its type
 */
void System::createParticle(Particle *p)
{
   if(systemType == Fire || systemType == FireWithSmoke)
   {
      p->lifespan = (((rand()%10+1)))/10.0f;
      if(systemType == FireWithSmoke)
         p->type = 0;
   }
   else
   {
      p->lifespan = (((rand()%125+1)/10.0f)+5);
      p->type = 2;
   }

   p->age = 0.0f;
   p->scale = 0.25f;
   p->direction = 0;
   
   if(systemType == Smoke || systemType == Fire || systemType == FireWithSmoke)
      p->position[X1] = ((rand()%2)-(rand()%2));
   else
      p->position[X1] = 0;

   if(systemType == Smoke)
      p->position[Y1] = -30;
   else
      p->position[Y1] = -15;

   p->position[Z1] = 0;

   if(systemType == Smoke)
   {
      p->movement[X1] = (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.0035) - (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.0035);
      p->movement[Y1] = ((((((5) * rand()%11) + 3)) * rand()%11) + 7) * 0.015; 
      p->movement[Z1] = (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.0015) - (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.0015);
   }
   else if(systemType == Fountain)
   {
      p->movement[X1] = (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.005) - (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.005);   
      p->movement[Y1] = ((((((5) * rand()%11) + 5)) * rand()%11) + 10) * 0.02;
      p->movement[Z1] = (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.005) - (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.005);
   }
   else if(systemType == Fire || systemType == FireWithSmoke)
   { 
      p->movement[X1] = (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.007) - (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.007);   
      p->movement[Y1] = ((((((5) * rand()%11) + 5)) * rand()%11) + 1) * 0.02;
      p->movement[Z1] = (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.007) - (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.007);
   }
   else //just in case
   { 
      p->movement[X1] = (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.005) - (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.005);   
      p->movement[Y1] = ((((((5) * rand()%11) + 5)) * rand()%11) + 1) * 0.02;
      p->movement[Z1] = (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.005) - (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.005);
   }

   if(systemType == Smoke)
   {//greyish-white for smoke
      p->color[X1] = 0.7f;
      p->color[Y1] = 0.7f;
      p->color[Z1] = 0.7f;
   }
   else if(systemType == Fountain)
   {//blue for water
      p->color[X1] = 0.0f;
      p->color[Y1] = 0.0f;
      p->color[Z1] = 1.0f;
   }
   else if(systemType == Fire || systemType == FireWithSmoke)
   {//red with mix of yellow for fire
      p->color[X1] = 1.0f;
      p->color[Y1] = 0.95f;
      p->color[Z1] = 0.8f;
   }

   p->pull[X1] = 0.0f;
   p->pull[Y1] = 0.0f;
   p->pull[Z1] = 0.0f;
}

/*
 * initalizes a particle system according to its type
 * calls create particle() to initalize individual particles
 */
void System::createParticles(void)
{
   if(systemType == Fountain)
   {
      systemPull[Y1] = -0.0025;
      systemPull[X1] = systemPull[Z1] = 0.0f;
   }
   else if(systemType == Fire)
   {
      systemPull[Y1] = 0.005;
      systemPull[X1] = systemPull[Z1] = 0.0f;
   }
   else if(systemType == FireWithSmoke)
   {
      systemPull[Y1] = 0.0005;
      systemPull[X1] = systemPull[Z1] = 0.0f;
   }
   else
      systemPull[X1] = systemPull[Y1] = systemPull[Z1] = 0.0f;
   int i = 0;
   for(i; i < MAX_PARTICLES; i++)
   {
      createParticle(&particles[i]);
   }
}

/*
 * updates required particle attributes for all particles in a system
 * also responsible for killing and respawning (via createparticle()) individual particles
 */
void System::updateParticles(void)
{
   int i = 0;
   for(i; i < MAX_PARTICLES; i++)
   {
      particles[i].age = particles[i].age + 0.02;
      
      if(systemType == Smoke || particles[i].type == 1)
         particles[i].scale = particles[i].scale + 0.001; //increasing scale makes textures bigger over lifetime

      particles[i].direction = particles[i].direction + ((((((int)(0.5) * rand()%11) + 1)) * rand()%11) + 1);

      particles[i].position[X1] = particles[i].position[X1] + particles[i].movement[X1] + particles[i].pull[X1];
      particles[i].position[Y1] = particles[i].position[Y1] + particles[i].movement[Y1] + particles[i].pull[Y1];
      particles[i].position[Z1] = particles[i].position[Z1] + particles[i].movement[Z1] + particles[i].pull[Z1];
      
      particles[i].pull[X1] = particles[i].pull[X1] + systemPull[X1];
      particles[i].pull[Y1] = particles[i].pull[Y1] + systemPull[Y1]; // acleration due to gravity
      particles[i].pull[Z1] = particles[i].pull[Z1] + systemPull[Z1];

      // color changing for fire particles light yellow -> red with age
      if(systemType == Fire || particles[i].type == 0)
      {
         float temp = particles[i].lifespan/particles[i].age;
         if((temp) < 1.75)
         {//red
            particles[i].color[X1] = 1.0f;
            particles[i].color[Y1] = 0.25f;
            particles[i].color[Z1] = 0.0f;
         }
         else if((temp) < 3.0)
         {//gold
            particles[i].color[X1] = 1.0f;
            particles[i].color[Y1] = 0.9f;
            particles[i].color[Z1] = 0.0f;
         }
         else if((temp) < 10.0)
         {//yellow
            particles[i].color[X1] = 1.0f;
            particles[i].color[Y1] = 1.0f;
            particles[i].color[Z1] = 0.0f;
         }
         else
         {// initial light yellow
            particles[i].color[X1] = 1.0f;
            particles[i].color[Y1] = 0.95f;
            particles[i].color[Z1] = 0.8f;
         }
      }

      if(systemType == Smoke)
      {
         if (particles[i].age > particles[i].lifespan || particles[i].position[Y1] > 45 || particles[i].position[Y1] < -35 || particles[i].position[X1] > 80 || particles[i].position[X1] < -80)
            createParticle(&particles[i]);
      }
      else if(systemType == Fountain)
      {
         if (particles[i].position[Y1] > 35 || particles[i].position[Y1] < -25 || particles[i].position[X1] > 40 || particles[i].position[X1] < -40)
            createParticle(&particles[i]);
      }
      else if(systemType == Fire)
      {
         if (particles[i].age > particles[i].lifespan || particles[i].position[Y1] > 35 || particles[i].position[Y1] < -25 || particles[i].position[X1] > 40 || particles[i].position[X1] < -40)
            createParticle(&particles[i]);
      }
      else if(systemType == FireWithSmoke)
      {
         if(particles[i].type == 0)
         {
            if (particles[i].age > particles[i].lifespan || particles[i].position[Y1] > 35 || particles[i].position[Y1] < -25 || particles[i].position[X1] > 40 || particles[i].position[X1] < -40)
            {
               int temp = rand()%100;
               if(temp < 10)
                  turnToSmoke(&particles[i]);
               else
                  createParticle(&particles[i]);
            }
         }
         else if(particles[i].type == 1)
         {
            if (particles[i].age > particles[i].lifespan || particles[i].position[Y1] > 45 || particles[i].position[Y1] < -35 || particles[i].position[X1] > 80 || particles[i].position[X1] < -80)
               createParticle(&particles[i]);
         }
      }
   }
}

/*
 * used only by updateparticles() and only when the fire and smoke system is active
 * used to turn selected fire particles into smoke
 */
void System::turnToSmoke(Particle *p)
{
   p->lifespan = (((rand()%125+1)/10.0f)+5);
   p->age = 0.0f;
   p->scale = 0.25f;
   p->direction = 0;
   p->type = 1;

   p->movement[X1] = (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.0035) - (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.0035);
   p->movement[Y1] = ((((((5) * rand()%11) + 3)) * rand()%11) + 7) * 0.015; 
   p->movement[Z1] = (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.0015) - (((((((2) * rand()%11) + 1)) * rand()%11) + 1) * 0.0015);

   p->color[X1] = 0.7f;
   p->color[Y1] = 0.7f;
   p->color[Z1] = 0.7f;
}

void System::setSystemType(int type)
{
   systemType = type;
}

int System::getNumOfParticles(void)
{
   return MAX_PARTICLES;
}

float System::getXPos(int i)
{
   return particles[i].position[X1];
}

float System::getYPos(int i)
{
   return particles[i].position[Y1];
}
float System::getZPos(int i)
{
   return particles[i].position[Z1];
}

float System::getR(int i)
{
   return particles[i].color[X1];
}

float System::getG(int i)
{
   return particles[i].color[Y1];
}
float System::getB(int i)
{
   return particles[i].color[Z1];
}

float System::getScale(int i)
{
   return particles[i].scale;
}

float System::getDirection(int i)
{
   return particles[i].direction;
}

float System::getAlpha(int i)
{
   return (1 - particles[i].age/particles[i].lifespan);
}

void System::modifySystemPull(float x, float y, float z)
{
   systemPull[X1] += x;
   systemPull[Y1] += y;
   systemPull[Z1] += z;
}
