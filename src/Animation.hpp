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
        vector<SDL_Surface*> mAnimationSurfaces ;
        vector<int> mLengths ;
        string mNextAnimation ;


    public : 
        Animation(string name, string path, vector<int> lengths, string nextAnimation) ;
        ~Animation() ;

        SDL_Surface* GetImage(int actionFrameNumber){return mAnimationSurfaces[actionFrameNumber] ;}
        string GetNext(){return mNextAnimation ;}
        int GetSize(){return mAnimationSurfaces.size() ;}
} ;

#endif