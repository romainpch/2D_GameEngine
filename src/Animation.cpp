#include "Animation.hpp"

Animation::Animation(string name, string path, vector<int> lengths, string nextAnimation) : mLengths(lengths), mNextAnimation(nextAnimation){
    vector<SDL_Surface*> * imageList = new vector<SDL_Surface*> ;

    for(int i=0 ; i< lengths.size() ; i++){
        int nb_frame = lengths[i] ;
        string imagePath = path + name + "_" + to_string(i) + ".png" ;
        SDL_Surface* frame = IMG_Load(imagePath.c_str()) ;
        for(int j=0 ; j<nb_frame ; j++){
            imageList->push_back(frame) ;
        }
    }
    mAnimationSurfaces = *imageList ;
}

Animation::~Animation(){
    for(int i=0 ; i< mAnimationSurfaces.size() ; i++){
        delete mAnimationSurfaces[i] ;
    }
}