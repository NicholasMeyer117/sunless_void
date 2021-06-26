#include "Minimap.h"

void Minimap::createMinimap(int mapArray[], float xDim, float yDim, float xPos, float yPos)
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
    
    void Minimap::addTile(int xTile, int yTile, int dim, int num, int mapXPos, int mapDim)
    {
        sf::RectangleShape tileRectangle;
        tileRectangle.setSize(sf::Vector2f(dim, dim));
        tileRectangle.setOutlineColor(sf::Color::Black);
        tileRectangle.setOutlineThickness(1);
        tileRectangle.setOrigin(dim, dim);
        tileRectangle.setPosition(xTile + mapXPos/2, yTile + mapDim/2);
        tileRectangles.push_back(tileRectangle);
    }
    
    void Minimap::updateMinimap(TileMap tilemap, Entity ship)
    {
        
        //playerIcon.move((tilemap.xVel*.0025), (tilemap.yVel*.0025));
        playerIcon.setRotation(ship.getAngle());
        playerIcon.setPosition(600 + (tilemap.xVel*-.001), 400 + (tilemap.yVel*-.001));
    }
