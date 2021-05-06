#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>

#include <vector>
#include <string>
#include <map>


using namespace std ;

#ifndef camera_HPP
#define camera_HPP

class Camera{
    private :
        double mTrueScrollX, mTrueScrollY ;
        int mScrollX, mScrollY ;
        const int mCamSpeedX, mCamSpeedY ;
        int mXoffset, mYoffset ;

    public :
        Camera(int Xoffset, int Yoffset) ;

        void SetTrueScrollX(int trueScrollX){mScrollX = trueScrollX;} ; 
        void SetTrueScrollY(int trueScrollY){mScrollX = trueScrollY;} ;  
        void SetScrollX(int scrollX){mScrollX = scrollX;} ; 
        void SetScrollY(int scrollY){mScrollX = scrollY;} ;

        int GetTrueScrollX(){return mTrueScrollX ;} ; 
        int GetTrueScrollY(){return mTrueScrollY ;} ; 
        int GetScrollX(){return mScrollX ;} ; 
        int GetScrollY(){return mScrollY ;} ; 
        int GetXoffset(){return mXoffset ;} ;
        int GetYoffset(){return mYoffset ;} ;
        int GetCamSpeedX(){return mCamSpeedX ;} ;
        int GetCamSpeedY(){return mCamSpeedY ;} ;

        void SwitchDirection() ;

        void SetFullScreen(bool isFullScreen, int XoffsetFULLSCREEN, int YoffsetFULLSCREEN) ;

        void Update(int posXabs, int posYabs) ;
} ;

#endif