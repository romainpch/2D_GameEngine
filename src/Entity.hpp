#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

#include <vector>
#include <string>
#include <map>

#include "Camera.hpp"
#include "Map.hpp" 

using namespace std ;

#ifndef entity_HPP
#define entity_HPP

class Entity{
    protected :
        int mPosXabs, mPosYabs ;
        int mPosXrel, mPosYrel ;
        int mWidth, mHeight ;
        
    public :
        Entity() ;
        ~Entity(){} ;

        int GetPosXabs(){return mPosXabs ;} ;
        int GetPosYabs(){return mPosYabs ;} ;
        int GetPosXrel(){return mPosXrel ;} ;
        int GetPosYrel(){return mPosYrel ;} ;
        int GetWidth(){return mWidth ;} ;
        int GetHeight(){return mHeight ;} ;

        void SetPosAbs(int Xabs, int Yabs) ;
        void SetPosRel(int Xrel, int Yrel) ;
        void SetDimension(int Width, int Height) ;

        

        void Render(SDL_Renderer * renderer) ;        
} ;


class Player : public Entity{
    private :
        int mVelX, mVelY ;
        double mTaccelX, mTaccelY ;
        bool isAccelX ;
        int mDirection ;

        Camera* mPlayerCam ;

        map< string, bool> mCollisionStatus;

    public : 
        Player() ;
        ~Player(){} ;

        void SetCamera(Camera* cam){mPlayerCam = cam ;} ;

        void SetFullScreen(bool isFULLSCREEN, int playerWidth, int playerHeight) ;

        bool CheckCollisionUp(SDL_Rect* rect) ;
        bool CheckCollisionDown(SDL_Rect* rect) ;
        bool CheckCollisionLeft(SDL_Rect* rect) ;
        bool CheckCollisionRight(SDL_Rect* rect) ;

        bool CheckCollisions(SDL_Rect* rect) ;
        vector<SDL_Rect*> GetCollisionsList(Map* map, SDL_Renderer* renderer) ;
        void Collide(Map* map, SDL_Renderer* renderer) ;

        void HandleEvents(SDL_Event e) ;
        void Move(Map* map, SDL_Renderer* renderer) ;

        void UpdateCam() ;
} ;

#endif