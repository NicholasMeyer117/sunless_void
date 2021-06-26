#pragma once
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

class Minimap
{
    public:
    std::string tilemap;
    sf::RectangleShape rectangle;
    sf::CircleShape playerIcon;
    std::vector<RectangleShape> tileRectangles;
    int map[400];
    
    void createMinimap(int mapArray[], float xDim, float yDim, float xPos, float yPos);
    
    void addTile(int xTile, int yTile, int dim, int num, int mapXPos, int mapDim);
    
    void updateMinimap(TileMap tilemap, Entity ship);
};
