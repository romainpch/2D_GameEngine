#include "Animation.hpp"

Animation::Animation(string name, string path, vector<int> lengths, string nextAnimation, SDL_Renderer* renderer) : mLengths(lengths), mNextAnimation(nextAnimation){
    vector<SDL_Texture*> * imageList = new vector<SDL_Texture*> ;

    for(int i=0 ; i< lengths.size() ; i++){
        int nb_frame = lengths[i] ;
        string imagePath = path + name + "_" + to_string(i) + ".png" ;
        SDL_Surface* frame = IMG_Load(imagePath.c_str()) ;
        SDL_SetColorKey( frame, SDL_TRUE, SDL_MapRGBA( frame->format, 0xFF, 0xFF, 0xFF, 0xFF) );
        SDL_Texture* frameTexture = SDL_CreateTextureFromSurface(renderer, frame);
        for(int j=0 ; j<nb_frame ; j++){
            imageList->push_back(frameTexture) ;
        }
    }
    mAnimationTextures = *imageList ;
}

Animation::~Animation(){
    // for(int i=0 ; i< mAnimationTextures.size() ; i++){
    //     delete mAnimationTextures[i] ;
    // }
}