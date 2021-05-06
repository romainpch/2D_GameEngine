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
#include "World.hpp"
#include "Animation.hpp"
#include "Light.hpp"

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
        SDL_Renderer* mRenderer ;


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
        void SetRenderer(SDL_Renderer* renderer){ mRenderer = renderer ;}

        void LoadAnimations() ;

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
        int mDirectionOffset ;

        bool isClimbing ;

        Camera* mPlayerCam ;
        Light* mPlayerLight ;

        map< string, bool> mCollisionStatus;

    public : 
        Player() ;
        ~Player(){delete mPlayerCam ;} ;

        void SetCamera(Camera* cam){mPlayerCam = cam ;}
        void SetLight(Light* light){mPlayerLight = light ;}
        
        void SetFullScreen(bool isFULLSCREEN, int playerWidth, int playerHeight) ;

        bool CheckCollisions(SDL_Rect* rect) ;
        vector<Tile*> GetCollisionsList(World* world, SDL_Renderer* renderer) ;

        bool canClimb(Tile* tile, vector<vector<Tile*> > GameMap, string direction) ;

        void HandleEvents(SDL_Event e) ;
        void Move(World* world, SDL_Renderer* renderer) ;

        void UpdateCam() ;
        void UpdateLight(World* world) ;

        void RenderLight(SDL_Renderer* renderer) ;
} ;


vector<int> FindijForTile(vector<vector<Tile*> > GameMap, Tile* tile) ;

#endif