#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
#include <math.h>
#include <cstring>
#define PI 3.14159265

using namespace sf;
using namespace std;

class Entity
{
   public:
   float x,y,xPos,yPos,w,h,dx,dy,R,angle;
   float absAngle;
   bool life = 1;
   std::string name;
   Sprite sprite;
   
   void settings(Sprite &a,float X,float Y, float W, float H, float Angle=0,int radius=1);
   
   float getAngle();
   
   bool isCollide(Entity *a,Entity *b);
   
   virtual void update();

   void draw(RenderWindow &app);

   virtual ~Entity();
};
