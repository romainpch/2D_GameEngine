#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

#include "Camera.hpp"
#include "World.hpp"

#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <tuple>

using namespace std ;

#ifndef light_HPP
#define light_HPP

struct sEdge
{
	float sx, sy; // Start coordinate
	float ex, ey; // End coordinate
};

struct sCell
{
	int edge_id[4];
	bool edge_exist[4];
	bool exist = false;
};

#define NORTH 0
#define SOUTH 1
#define EAST 2
#define WEST 3


class Light{
    private:
        int mPosRelX, mPosRelY ;

        sCell* mWorldCells;
	    vector<sEdge> mVecEdges;
        vector<tuple<float, float, float> > mVecVisibilityPolygonPoints;

    public: 
        Light() ;
        ~Light() ;

        void ConvertTileMapToPolyMap(vector<vector< Tile*> > TileMap) ;
        void CalculateVisibilityPolygon(float ox, float oy, float radius) ;
        void Update(int posXabs, int posYabs, Camera* playerCam, vector<vector< Tile*> > TileMap) ;
        void FillTriangle(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3, Uint8 r,  Uint8 g,  Uint8 b,  Uint8 a) ;
        void DrawTriangle(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3, Uint8 r,  Uint8 g,  Uint8 b,  Uint8 a) ;
        void Render(SDL_Renderer* renderer) ;
} ;

#endif