#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

#include <fstream>
#include <algorithm>
#include <string>
#include <vector>

#include "Camera.hpp"

using namespace std ;


#ifndef map_HPP
#define map_HPP

class BG_Object{
    private :
        double mSpeed ;
        SDL_Rect* mRect ;
        SDL_Rect* mRenderRect ;
        Uint8 mRcolor ;
        Uint8 mGcolor ;
        Uint8 mBcolor ;

    public :
        BG_Object(double speed,int x, int y, int w, int h, Uint8 R, Uint8 G, Uint8 B );
        ~BG_Object() ;

        double GetSpeed() ;
        SDL_Rect* GetRect() ;

        void SetPosition(int x, int y) ;
        void SetRenderPosition(int x, int y) ;

        void Render(SDL_Renderer* renderer) ;
} ;



class Map{
    private :
        int mTileWidth ;
        vector<BG_Object*> mBackGround ;
        vector<vector<SDL_Rect* > > mGameMap ;

        Camera* mPlayerCam ;

    public :
        Map() ;
        ~Map() ; 

        vector<vector<SDL_Rect* > > GetGameMap(){return mGameMap ;} ;           
        int GetTileWidth(){return mTileWidth ;} ;

        void SetPlayerCam(Camera* cam){mPlayerCam = cam ;} ;
        void SetTileWidth(int w){mTileWidth = w ;} ;

        void SetFullScreen(bool isFullScreen) ;

        void LoadMapFromFile(string path) ;
        void AddBGObject(BG_Object* object) ;

        void Update() ;
        void RenderBackGround(SDL_Renderer* renderer) ;
        void Render(SDL_Renderer* renderer) ;

} ;

#endif