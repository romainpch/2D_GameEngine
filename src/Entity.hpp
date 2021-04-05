#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

#include <vector>
#include <string>
#include <map>
#include <unordered_map>

#include "Camera.hpp"
#include "Map.hpp" 
#include "Animation.hpp"

using namespace std ;

#ifndef entity_HPP
#define entity_HPP

class Entity{
    protected :
        SDL_Rect* mHitboxAbs ;
        SDL_Rect* mHitboxRel ;
        
        SDL_RendererFlip mFlip ;

        //Animation
        unordered_map< string, Animation* > mAnimationDatabase ; //Dictionnary of every animations
        string mAction ; //Current Animation
        int mActionFrameNumber ;


    public :
        Entity() ;
        ~Entity() ;

        int GetPosXabs(){return mHitboxAbs->x ;}
        int GetPosYabs(){return mHitboxAbs->y ;} 
        int GetPosXrel(){return mHitboxRel->x ;}
        int GetPosYrel(){return mHitboxRel->y ;}
        int GetWidth(){return mHitboxAbs->w ;}
        int GetHeight(){return mHitboxAbs->h ;}

        void SetPosAbs(int Xabs, int Yabs) ;
        void SetPosRel(int Xrel, int Yrel) ;
        void SetDimension(int Width, int Height) ;

        void AddAnimation(string animationName, string path, vector<int> animationLengths, string nextAnimation) ;
        void ChangeAction(string actionName){mAction = actionName ;}
        
        void Render(SDL_Renderer * renderer) ;
        void ShowHitbox(SDL_Renderer * renderer) ;        
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

        void SetCamera(Camera* cam){mPlayerCam = cam ;}

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