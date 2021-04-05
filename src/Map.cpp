#include "Map.hpp"


// BG_Object___________________________________________________________________________________________________________
BG_Object::BG_Object(double speed,int x, int y, int w, int h, Uint8 R, Uint8 G, Uint8 B ) : mSpeed(speed), mRcolor(R), mGcolor(G), mBcolor(B){
    SDL_Rect* Rect  = new SDL_Rect ;
    Rect->x = x ;
    Rect->y = y ;
    Rect->w = w ;
    Rect->h = h ;

    mRect = Rect ;

    SDL_Rect* RenderRect  = new SDL_Rect ;
    RenderRect->x = x ;
    RenderRect->y = y ;
    RenderRect->w = w ;
    RenderRect->h = h ;

    mRenderRect = RenderRect ;
}

BG_Object::~BG_Object(){
    delete mRect ;
    delete mRenderRect ;
}

double BG_Object::GetSpeed(){
    return mSpeed ;
}

SDL_Rect* BG_Object::GetRect(){
    return mRect ;
}

void BG_Object::SetPosition(int x, int y){
    mRect->x = x ;
    mRect->y = y ;
}

void BG_Object::SetRenderPosition(int x, int y){
    mRenderRect->x = x ;
    mRenderRect->y = y ;
}


void BG_Object::Render(SDL_Renderer* renderer){
    SDL_SetRenderDrawColor(renderer, mRcolor, mGcolor, mBcolor, 0xFF) ;
    SDL_RenderFillRect( renderer, mRenderRect);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF) ;
}


// Map_________________________________________________________________________________________________________________
Map::Map():mTileWidth(128){}
Map::~Map(){
    delete mPlayerCam ;
}

void Map::LoadMapFromFile(string path){
    ifstream MapFile(path) ;
    string row ;
    int i = 0 ;
    while(getline(MapFile,row)){
        vector<SDL_Rect*> layer ;
        for(int j = 0 ; j< row.length() ; j++){
            char c = row[j] ;
            if(int(c-'0')==1){
                SDL_Rect * tile  = new SDL_Rect;
                tile->x = mTileWidth*j; 
                tile->y = mTileWidth*i; 
                tile->w = mTileWidth; 
                tile->h = mTileWidth; 
                layer.push_back(tile) ;
            }
            else{
                layer.push_back(nullptr) ;
            }
            
        }
        mGameMap.push_back(layer) ;
        
        i++ ;
    }
}


void Map::SetFullScreen(bool isFullScreen){
    if(isFullScreen){
        mTileWidth = 128 ;
    }
    else{
        mTileWidth = 64 ;
    }
    // Updating Tiles
    for(int i=0 ; i<mGameMap.size(); i++){
        for(int j=0 ; j<mGameMap[i].size() ; j++){
            if(mGameMap[i][j] != nullptr){
                mGameMap[i][j]->x = mTileWidth*j;
                mGameMap[i][j]->y = mTileWidth*i;
                mGameMap[i][j]->w = mTileWidth ;
                mGameMap[i][j]->h = mTileWidth ;
            }
        }
    }
}

void Map::AddBGObject(BG_Object* object){
    mBackGround.push_back(object) ;
}

void Map::Update(){
    // Updating BackGround
    for(int i=0 ; i<mBackGround.size(); i++){
        mBackGround[i]->SetRenderPosition(mBackGround[i]->GetRect()->x-(mBackGround[i]->GetSpeed())*mPlayerCam->GetScrollX(), mBackGround[i]->GetRect()->y-(mBackGround[i]->GetSpeed())*mPlayerCam->GetScrollY()) ;
    } ;
}

void Map::RenderBackGround(SDL_Renderer* renderer){
    for(int i=0 ; i<mBackGround.size(); i++){
        mBackGround[i]->Render(renderer) ;
    } ;
}

void Map::Render(SDL_Renderer* renderer){
    SDL_Rect RectToRender ;
    for(int i=0 ; i<mGameMap.size(); i++){
        for(int j=0 ; j<mGameMap[i].size() ; j++){
            if( mGameMap[i][j]!= nullptr){
                RectToRender.x = mTileWidth*j - (mPlayerCam->GetScrollX() + mPlayerCam->GetXoffset()) ;
                RectToRender.y = mTileWidth*i - (mPlayerCam->GetScrollY()  + mPlayerCam->GetYoffset()) ;
                RectToRender.h = mTileWidth ;
                RectToRender.w = mTileWidth ;

                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF) ;
                SDL_RenderFillRect(renderer, &RectToRender);
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF) ;
            }
            
        }
    }
}

