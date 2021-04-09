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

class Tile{
    public : 
        SDL_Rect* mRectAbs ;
        SDL_Rect* mRectRel ;

        Tile() ;
        ~Tile() ;
} ;

class World{
    private :
        int mWidth, mHeight ;

        int mVisibleI ;
        int mVisibleJ ;

        vector<vector< Tile*> > mTiles ;
        vector<vector< Tile*> > mVisibleTiles ;

        Camera* mPlayerCam ;

    public :
        World() ;
        ~World() ;

        void SetPlayerCam(Camera* cam){mPlayerCam = cam ;}

        vector<vector< Tile*> > GetGameMap(){return mVisibleTiles ;}

        void LoadFromFile(string path) ;

        void Update() ;
        void Render(SDL_Renderer* renderer) ;
};

#endif


// https://www.youtube.com/watch?v=fc3nnG2CG8U