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
   
   float Entity::getAngle()
   {
       return absAngle;
   }

   bool Entity::isCollide(Entity *a,Entity *b)
   {

   //dimensions of 

      Vector2f al, ar, bl, br;
      al.x = (a->sprite.getPosition().x - (a->w)/2);
      al.y = (a->sprite.getPosition().y + (a->h)/2);
    
      ar.x = (a->sprite.getPosition().x + (a->w)/2);
      ar.y = (a->sprite.getPosition().y - (a->h)/2);
    
      bl.x = (b->sprite.getPosition().x - (b->w)/2);
      bl.y = (b->sprite.getPosition().y + (b->h)/2);
    
      br.x = (b->sprite.getPosition().x + (b->w)/2);
      br.y = (b->sprite.getPosition().y - (b->h)/2);
    
      // If one rectangle is on left side of other
      if (al.x >= br.x or bl.x >= ar.x)
         return false;
      // If one rectangle is above other
      if (al.y <= br.y or bl.y <= ar.y)
         return false;
      else
         return true;
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

