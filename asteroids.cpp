#include <iostream>
#include <string>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
#include <math.h>
#define PI 3.14159265

using namespace sf;
using namespace std;

const int W = 1200;
const int H = 800;

sf::Text source;
sf::Font mainGameFont;

float DEGTORAD = 0.017453f;

class TileMap;

struct position {
  int xCoord;
  int yCoord;
} ;


class Entity
{
   public:
   float x,y,w,h,dx,dy,R,angle;
   float absAngle;
   bool life;
   std::string name;
   Sprite sprite;

   Entity()
   {
     life=1;
   }
   
   void settings(Sprite &a,int X,int Y, int W, int H, float Angle=0,int radius=1)
   {
     sprite = a;
     x=X; y=Y;
     angle = Angle;
     R = radius;
     w = W; h = H;
   }
   
   float getAngle()
   {
       return absAngle;
   }

   virtual void update(){};

   void draw(RenderWindow &app)
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

   virtual ~Entity(){};
};

class Quest
{



};

class Items
{





};

class ShopGood: public Items
{


};

class Port: public Entity
{

};

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
bool thrust = false;
bool backThrust = false;
bool beganThrust = false;
float xVel,yVel,dx,dy, xDir, yDir, xPos, yPos, xSpeed, ySpeed, R,angle, maxSpeed, curSpeed, accelerationRate;

    void resetMap()
    {
        xVel = 0;
        yVel = 0;
        xSpeed = 0.1;
        ySpeed = 0.1;
        xDir = 0;
        yDir = 0;
        maxSpeed = 3.5;
    }

    bool load(const std::string& tileset, sf::Vector2u tileSize, const int* tiles, unsigned int width, unsigned int height, Entity ship)
    {
        // load the tileset texture
        if (!m_tileset.loadFromFile(tileset))
            return false;

        // resize the vertex array to fit the level size
        m_vertices.setPrimitiveType(sf::Quads);
        m_vertices.resize(width * height * 4);

        // populate the vertex array, with one quad per tile
        for (unsigned int i = 0; i < width; ++i)
            for (unsigned int j = 0; j < height; ++j)
            {
                // get the current tile number
                int tileNumber = tiles[i + j * width];

                // find its position in the tileset texture
                int tu = tileNumber % (m_tileset.getSize().x / tileSize.x);
                int tv = tileNumber / (m_tileset.getSize().x / tileSize.x);

                // get a pointer to the current tile's quad
                sf::Vertex* quad = &m_vertices[(i + j * width) * 4];

                // define its 4 corners 
                               
	        curSpeed = sqrt((xDir*xDir) + (yDir*yDir));
                
                if (thrust)
	        {
	            xDir += cos((ship.getAngle() + 90) * PI/180) * 0.001;
                    yDir += sin((ship.getAngle() + 90) * PI/180) * 0.001;
	            if (curSpeed > maxSpeed){
	                xDir *= maxSpeed/curSpeed;
	                yDir *= maxSpeed/curSpeed;}
		}
		
		else if (backThrust)
		{
		    if (curSpeed > 0){
	                xDir *= 0.999;
	                yDir *= 0.999;}
		
		}

		xVel = xVel + xDir;
                yVel = yVel + yDir;                
                
                xPos = (i * tileSize.x) + (xVel * .01);
                yPos = (j * tileSize.y) + (yVel * .01);
                float xSize = ((i + 1) * tileSize.x) + (xVel * .01);
                float ySize = ((j + 1) * tileSize.y) + (yVel * .01);
                quad[0].position = sf::Vector2f(xPos, yPos);
                quad[1].position = sf::Vector2f(xSize, yPos);
                quad[2].position = sf::Vector2f(xSize, ySize);
                quad[3].position = sf::Vector2f(xPos, ySize);

                // define its 4 texture coordinates
                quad[0].texCoords = sf::Vector2f(tu * tileSize.x, tv * tileSize.y);
                quad[1].texCoords = sf::Vector2f((tu + 1) * tileSize.x, tv * tileSize.y);
                quad[2].texCoords = sf::Vector2f((tu + 1) * tileSize.x, (tv + 1) * tileSize.y);
                quad[3].texCoords = sf::Vector2f(tu * tileSize.x, (tv + 1) * tileSize.y);
            }

        return true;
    }

private:

    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the transform
        states.transform *= getTransform();

        // apply the tileset texture
        states.texture = &m_tileset;

        // draw the vertex array
        target.draw(m_vertices, states);
    }

    sf::VertexArray m_vertices;
    sf::Texture m_tileset;
};

class Station: public Entity
{
    public:
    string name;
    std::list<Quest*> quests;
    std::list<ShopGood*> shopGoods;
    Port port;
    
    void createStation(Sprite &Image,int X,int Y, int W, int H, string Name, std::list<Quest*> Quests, std::list<ShopGood*> ShopGoods, Port ThisPort)
    {
        port = ThisPort;
        shopGoods = ShopGoods;
        quests = Quests;
        name = Name;
        sprite = Image;
        x=X; y=Y;
        w = W; h = H;
    }
   
   void updatePos(TileMap *m)
   {
       x = m->xVel * (.01);
       y = m->yVel * (.01);
   
   }
};

class player: public Entity
{
   public:
   bool thrust;
   int xPos, yPos;

   player()
   {
     name="player";
   }
   
   void updatePos(TileMap *m)
    {
        xPos = m->xVel + 600;
        yPos = m->yVel + 400;   
    }
};

class bullet: public Entity
{
   public:
   bullet()
   {
     name="bullet";
   }

   void update()
   {
     dx=cos((angle - 90)*DEGTORAD)*12;
     dy=sin((angle - 90)*DEGTORAD)*12;
     // angle+=rand()%7-3;  /*try this*/
     x+=dx;
     y+=dy;

     if (x>W || x<0 || y>H || y<0) life=0;
   }

};


void drawText( const sf::String &str, const int Size, const float xposition, const float yposition, sf::RenderWindow& window)
{
    source.setString(str);
    source.setCharacterSize(Size); //only the lower cased word size is reserved. A capital S fixes that.
    source.setPosition(xposition,yposition);
    source.setFillColor(Color(0,0,0,255));
    window.draw(source);
}

std::vector<Sprite> addSpriteToList(std::vector<Sprite> spriteList, Texture *t)
{
    Sprite sprite(*t);
    spriteList.push_back(sprite);
    return spriteList;

}

bool isCollide(Entity *a,Entity *b)
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

int main() {

    srand(time(0));
    
    RenderWindow app(VideoMode(W, H), "Asteroids!");
    app.setFramerateLimit(60);
    mainGameFont.loadFromFile("futura.ttf");
    source.setFont(mainGameFont);
    
    Texture t1,t2,t3,t4,t5,t6,t7;
    t1.loadFromFile("images/triShip.png");
    t2.loadFromFile("images/background.jpg");
    t3.loadFromFile("images/bullet.png");

    t1.setSmooth(true);
    t2.setSmooth(true);

    Sprite background(t2);
    Sprite playerShip(t1);
    Sprite bulletSprite(t3);
    std::vector<Sprite> stationMainSprites;
    std::vector<Sprite> stationDockSprites;
    std::vector<Station*> stations;
    
    t4.loadFromFile("images/bluePurpleStationMain.png");
    stationMainSprites = addSpriteToList(stationMainSprites, &t4);
    
    t5.loadFromFile("images/bluePurpleStationDock.png");
    stationDockSprites = addSpriteToList(stationDockSprites, &t5);
    
    const int level[] =
    {
        2, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        0, 1, 3, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0,
    };
    
    int bulletsFired = 0;
    
    std::list<Entity*> entities;
    
    player *p = new player();
    p->settings(playerShip,600,400,32,33,0,20);
    entities.push_back(p);
    
    Station *station = new Station();
    station->settings(stationMainSprites[0],300,400,600,300,0,0);
    stations.push_back(station);
    
    // create the tilemap from the level definition
    TileMap *m = new TileMap();
    m->resetMap();
    if (!m->load("images/spaceTilemap.png", sf::Vector2u(1280, 1280), level, 16, 8, *p))
        return -1;
    
    for(auto i:stations)
        entities.push_back(i);
    
    /////main loop/////
    while (app.isOpen())
    {
        
        p->updatePos(m);
        for(auto i:stations)
            i->updatePos(m);
        
        Event event;
        while (app.pollEvent(event))
        {
            if (event.type == Event::Closed)
                app.close();

            if (event.type == Event::KeyPressed)
             if (event.key.code == Keyboard::Space)
              {
                bullet *b = new bullet();
                b->settings(bulletSprite,p->x,p->y,5, 5, p->angle, 3);
                entities.push_back(b);
                bulletsFired++;
              }
        }
        

    if (Keyboard::isKeyPressed(Keyboard::Right)) 
        p->angle+=6;
    if (Keyboard::isKeyPressed(Keyboard::Left))  
        p->angle-=6;
    if (Keyboard::isKeyPressed(Keyboard::Up)) 
    {
        m->thrust = true;
        
    }
    else if (Keyboard::isKeyPressed(Keyboard::Down)) 
    {
        m->backThrust = true;

    }
    else {
        m->thrust=false;
        m -> backThrust = false;
    }
        
    m->load("images/spaceTilemap.png", sf::Vector2u(1280, 1280), level, 16, 8, *p);

    for(auto i=entities.begin();i!=entities.end();)
    {
      Entity *e = *i;

      e->update();
      //e->anim.update();

      if (e->life==false) {i=entities.erase(i); delete e;}
      else i++;
    }

   //////draw//////
   app.clear(Color(255, 255, 255, 255));
   //app.draw(background);
   app.draw(*m);
   for(auto i:entities) i->draw(app);
   /*drawText("xPos: " + std::to_string(m->xVel + 600), 20, 90, 90, app);
   drawText("yPos: " + std::to_string(m->yVel + 400), 20, 90, 140, app);
   drawText("Station xPos: " + std::to_string(station -> x), 20, 90, 10, app);
   drawText("Station yPos: " + std::to_string(station -> y), 20, 90, 40, app);*/
   //drawText("playerPos: " + std::to_string(p->sprite.getPosition().x) + " " + std::to_string(p->sprite.getPosition().y), 20, 90, 50, app);
   drawText("playerTopL: " + std::to_string(p->sprite.getPosition().x - (p->w)/2) + ", " + std::to_string(p->sprite.getPosition().y + (p->h)/2), 20, 90, 100, app);
   drawText("playerBottomR: " + std::to_string(p->sprite.getPosition().x + (p->w)/2) + ", " + std::to_string(p->sprite.getPosition().y - (p->h)/2), 20, 90, 150, app);
   //drawText("stationPos " + std::to_string(station->sprite.getPosition().x) + " " + std::to_string(station->sprite.getPosition().y), 20, 90, 200, app);
   drawText("stationTopL " + std::to_string(station->sprite.getPosition().x - (station->w)/2) + ", " + std::to_string(station->sprite.getPosition().y + (station->h)/2), 20, 90, 250, app);
   drawText("stationBottomR " + std::to_string(station->sprite.getPosition().x + (station->w)/2) + ", " + std::to_string(station->sprite.getPosition().y - (station->h)/2), 20, 90, 300, app);
   drawText("Overlapping " + std::to_string(isCollide(p, station)), 20, 90, 50, app);
   

   app.display();
   app.clear();
    }
    
    return 0;
}
