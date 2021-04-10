#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <vector>
#include <string>
#include <unordered_map>

using namespace std ;

#ifndef animation_HPP
#define animation_HPP

class Animation{
    private :
        vector<SDL_Texture*> mAnimationTextures ;
        vector<int> mLengths ;
        string mNextAnimation ;


    public : 
        Animation(string name, string path, vector<int> lengths, string nextAnimation, SDL_Renderer* renderer) ;
        ~Animation() ;

        SDL_Texture* GetImage(int actionFrameNumber){return mAnimationTextures[actionFrameNumber] ;}
        string GetNext(){return mNextAnimation ;}
        int GetSize(){return mAnimationTextures.size() ;}
} ;

#endif