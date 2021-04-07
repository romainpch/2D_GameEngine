#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

#include "Camera.hpp"

#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <tuple>

using namespace std;

#ifndef world_HPP
#define world_HPP

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

class Tile{
    public : 
        SDL_Rect* mRectAbs ;
        SDL_Rect* mRectRel ;

        Tile() ;
        ~Tile() ;
} ;


struct sTile{
    int size = 120 ;
    int sPosXabs ;
    int sPosYabs ;
    int sPosXrel ;
    int sPosYrel ; 
} ;


class World{
    private :
        int mWidth, mHeight ;

        int mVisibleI ;
        int mVisibleJ ;

        vector<vector< Tile*> > mTiles ;
        vector<vector< Tile*> > mVisibleTiles ;

        sCell* mWorldCells;
        vector<sEdge> mVecEdges;

        vector<tuple<float, float, float> > mVecVisibilityPolygonPoints ;

        Camera* mPlayerCam ;

    public :
        World() ;
        ~World(){} ;

        void SetPlayerCam(Camera* cam){mPlayerCam = cam ;}

        vector<vector< Tile*> > GetGameMap(){return mVisibleTiles ;}

        void LoadFromFile(string path) ;
        void ConvertTileMapToPolyMap() ;
        void CalculateVisibilityPolygon(float ox, float oy, float radius) ;

        void Update() ;
        void Render(SDL_Renderer* renderer) ;
};

#endif


// https://www.youtube.com/watch?v=fc3nnG2CG8U