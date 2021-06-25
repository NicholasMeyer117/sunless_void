#include "Entity.h"
   
   void Entity::settings(Sprite &a,float X,float Y, float W, float H, float Angle,int radius)
   {
     sprite = a;
     x=X; y=Y; //current ever-changing position relative to window
     angle = Angle;
     R = radius;
     w = W; h = H;
     xPos = X, yPos = Y; //static original position on the map
   }
   
   float Entity:: getAngle()
   {
       return absAngle;
   }

   void Entity::update(){}

   void Entity::draw(RenderWindow &app)
   {
     sprite.setOrigin(w/2,h/2);
     sprite.setPosition(x,y);
     sprite.setRotation(angle);
     app.draw(sprite);
     absAngle = sprite.getRotation();

     CircleShape circle(R);
     circle.setFillColor(Color(255,0,0,170));
     circle.setPosition(x,y);
     circle.setOrigin(R,R);
     //app.draw(circle);
     
   }

   Entity:: ~Entity(){}

