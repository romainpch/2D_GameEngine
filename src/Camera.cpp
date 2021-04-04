#include "Camera.hpp"

Camera::Camera(int Xoffset, int Yoffset):mScrollX(0), mScrollY(0), mTrueScrollX(-Xoffset), mTrueScrollY(-Yoffset), mCamSpeedX(10), mCamSpeedY(25), mXoffset(Xoffset) , mYoffset(Yoffset) {}

void Camera::SetFullScreen(bool isFullScreen, int XoffsetFULLSCREEN, int YoffsetFULLSCREEN){
    if(isFullScreen){
        mXoffset = XoffsetFULLSCREEN ;
        mYoffset = YoffsetFULLSCREEN ;
    }
    else{
        mXoffset = XoffsetFULLSCREEN/2 ;
        mYoffset = YoffsetFULLSCREEN/2 ;
    }
}


void Camera::Update(int posXabs, int posYabs){
    mTrueScrollX += (posXabs-mTrueScrollX-mXoffset)/mCamSpeedX ;
    mTrueScrollY += (posYabs-mTrueScrollY-mYoffset)/mCamSpeedY ;
    mScrollX = (int)floor(mTrueScrollX);
    mScrollY = (int)floor(mTrueScrollY);
}