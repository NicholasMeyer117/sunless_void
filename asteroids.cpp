#include <iostream>
#include <string>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <list>
#include <math.h>
#include <cstring>
#include "Entity.h"
#include "TileMap.h"
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

class Minimap
{
    public:
    std::string tilemap;
    sf::RectangleShape rectangle;
    sf::CircleShape playerIcon;
    std::vector<RectangleShape> tileRectangles;
    int map[400];

    
    void createMinimap(int mapArray[], float xDim, float yDim, float xPos, float yPos)
    {
    
    rectangle.setSize(sf::Vector2f(xDim, yDim));
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(5);
    rectangle.setOrigin(xDim/2, yDim/2);
    rectangle.setPosition(xPos, yPos);
    memcpy(map, mapArray, sizeof(map));
    
    playerIcon.setRadius(10);
    playerIcon.setPointCount(3);
    playerIcon.setFillColor(sf::Color::Red);
    playerIcon.setOutlineThickness(0);
    playerIcon.setOrigin(10, 10);
    playerIcon.setPosition(xPos, yPos);
    
    int tileNum = 0;
    int tileDim = xDim/20;
    for (auto i = 0; i < 20; i++)
        for (auto j = 0; j < 20; j++)
        {
            addTile(xPos - (i * tileDim),yPos - (j * tileDim), tileDim, tileNum, xDim, yDim);
            tileNum++;
        }
    
    
    }
    
    void addTile(int xTile, int yTile, int dim, int num, int mapXPos, int mapDim)
    {
        sf::RectangleShape tileRectangle;
        tileRectangle.setSize(sf::Vector2f(dim, dim));
        tileRectangle.setOutlineColor(sf::Color::Black);
        tileRectangle.setOutlineThickness(1);
        tileRectangle.setOrigin(dim, dim);
        tileRectangle.setPosition(xTile + mapXPos/2, yTile + mapDim/2);
        tileRectangles.push_back(tileRectangle);
    }
    
    void updateMinimap(TileMap tilemap, Entity ship)
    {
        
        //playerIcon.move((tilemap.xVel*.0025), (tilemap.yVel*.0025));
        playerIcon.setRotation(ship.getAngle());
        playerIcon.setPosition(600 + (tilemap.xVel*-.001), 400 + (tilemap.yVel*-.001));
    }

};

class Station: public Entity
{
    public:
    string name;
    std::vector<Quest*> quests;
    std::vector<ShopGood*> shopGoods;
    std::vector<Entity*> docks;
    Port port;
    
    void createStation(string Name, std::vector<Quest*> Quests, std::vector<ShopGood*> ShopGoods, Port ThisPort)
    {
        port = ThisPort;
        shopGoods = ShopGoods;
        quests = Quests;
        name = Name;
    }
   
   void updatePos(TileMap *m)
   {
       x = (m->xVel * .01) + xPos;
       y = (m->yVel * .01) + yPos;
       docks[0]->x = (m->xVel * .01) + docks[0] -> xPos;
       docks[0]->y = (m->yVel * .01) + docks[0] -> yPos;
       docks[1]->x = (m->xVel * .01) + docks[1] -> xPos;
       docks[1]->y = (m->yVel * .01) + docks[1] -> yPos;
   }
   
   void addDockSprites(Sprite dockSprite)
   {
       Entity *dock1 = new Entity();
       dock1->settings(dockSprite,xPos - 50,yPos + 200 ,30,150,0,0);
       docks.push_back(dock1);
       Entity *dock2 = new Entity();
       dock2->settings(dockSprite,xPos + 50,yPos + 200 ,30,150,0,0);
       docks.push_back(dock2);
   }
};

class player: public Entity
{
   public:
   bool thrust;

   player()
   {
     name="player";
   }
   
   void updatePos(TileMap *m)
    {
        xPos = (m->xVel * .01) + 600;
        yPos = (m->yVel * .01) + 400;
        
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
    std::vector<Entity*> collidableEntities;
    
    t4.loadFromFile("images/bluePurpleStationMain.png");
    stationMainSprites = addSpriteToList(stationMainSprites, &t4);
    
    t5.loadFromFile("images/bluePurpleStationDock.png");
    stationDockSprites = addSpriteToList(stationDockSprites, &t5);
    
    int level[] =
    {
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
        0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1,
    };
    
    int bulletsFired = 0;
    bool mapOpen = false;
    
    std::list<Entity*> entities;
    
    Station *station = new Station();
    station->settings(stationMainSprites[0],300,200,600,300,0,0);
    stations.push_back(station);
    station->addDockSprites(stationDockSprites[0]);
    
    for(auto i:stations) {
        entities.push_back(i);
        entities.push_back(i -> docks[0]);
        entities.push_back(i -> docks[1]);
    }
    
    for(auto i:stations) {
        collidableEntities.push_back(i);
        collidableEntities.push_back(i -> docks[0]);
        collidableEntities.push_back(i -> docks[1]);
    }
        
    player *p = new player();
    p->settings(playerShip,600,400,32,33,0,20);
    entities.push_back(p);
    
    // create the tilemap from the level definition
    TileMap *m = new TileMap();
    m->resetMap();
    if (!m->load("images/spaceTilemap.png", sf::Vector2u(1280, 1280), level, 20, 20, *p, collidableEntities))
        return -1;
        
    Minimap *minimap = new Minimap();
    minimap -> createMinimap(level,700,700,600,400);
    
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
        m->thrust = true;
    else if (Keyboard::isKeyPressed(Keyboard::Down)) 
        m->backThrust = true;
    else {
        m->thrust=false;
        m -> backThrust = false;
    }
    
    if (Keyboard::isKeyPressed(Keyboard::M)) 
    {
        if (mapOpen)
            mapOpen = false;
        else
            mapOpen = true;
    }
        
    m->load("images/spaceTilemap.png", sf::Vector2u(1280, 1280), level, 10, 10, *p, collidableEntities);

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
   
   if (mapOpen){
       minimap->updateMinimap(*m, *p);
       app.draw(minimap -> rectangle);
       for(auto i:(minimap -> tileRectangles)) app.draw(i);
       app.draw(minimap->playerIcon);
   }
   
   drawText("First Tile Pos: " + std::to_string(minimap->tileRectangles[0].getPosition().y), 20, 90, 70, app);
   drawText("Player Position: " + std::to_string(p->xPos) + ", " + std::to_string(p->yPos), 20, 90, 140, app);
   drawText("Icon Position: " + std::to_string(minimap->playerIcon.getPosition().x) + ", " + std::to_string(minimap->playerIcon.getPosition().y), 20, 90, 190, app);
   //drawText("xDir: " + std::to_string(m->xDir), 20, 90, 140, app);
   //drawText("yDir: " + std::to_string(m->yDir), 20, 90, 190, app);
   //drawText("Station xPos: " + std::to_string(station -> docks[0] -> x), 20, 90, 10, app);
   //drawText("Station yPos: " + std::to_string(station -> docks[0] -> y), 20, 90, 40, app);
   //drawText("playerPos: " + std::to_string(p->sprite.getPosition().x) + " " + std::to_string(p->sprite.getPosition().y), 20, 90, 100, app);
   //drawText("playerTopL: " + std::to_string(p->sprite.getPosition().x - (p->w)/2) + ", " + std::to_string(p->sprite.getPosition().y + (p->h)/2), 20, 90, 100, app);
   //drawText("playerBottomR: " + std::to_string(p->sprite.getPosition().x + (p->w)/2) + ", " + std::to_string(p->sprite.getPosition().y - (p->h)/2), 20, 90, 150, app);
   //drawText("stationPos " + std::to_string(station->sprite.getPosition().x) + " " + std::to_string(station->sprite.getPosition().y), 20, 90, 200, app);
   //drawText("stationTopL " + std::to_string(station->sprite.getPosition().x - (station->w)/2) + ", " + std::to_string(station->sprite.getPosition().y + (station->h)/2), 20, 90, 250, app);
   //drawText("stationBottomR " + std::to_string(station->sprite.getPosition().x + (station->w)/2) + ", " + std::to_string(station->sprite.getPosition().y - (station->h)/2), 20, 90, 300, app);
   //drawText("Overlapping " + std::to_string(isCollide(p, station)), 20, 90, 50, app);
   

   app.display();
   app.clear();
   }
    
    return 0;
}
