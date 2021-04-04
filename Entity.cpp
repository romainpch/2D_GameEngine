#include "Entity.hpp"


// Entity______________________________________________________________________________________________________________
Entity::Entity() : mPosXabs(0), mPosYabs(0), mPosXrel(0), mPosYrel(0), mWidth(0), mHeight(0) {}

void Entity::SetPosAbs(int Xabs, int Yabs){
    mPosXabs = Xabs ;
    mPosYabs = Yabs ;
}

void Entity::SetPosRel(int Xrel, int Yrel){
    mPosXrel = Xrel ;
    mPosYrel = Yrel ;
}

void Entity::SetDimension(int Width, int Height){
    mWidth = Width ;
    mHeight = Height ;
}


void Entity::Render(SDL_Renderer * renderer){
    SDL_Rect HitBox ;
    HitBox.x = mPosXrel ;
    HitBox.y = mPosYrel ;
    HitBox.w = mWidth ;
    HitBox.h = mHeight ;
    SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0, 0xFF) ;
    SDL_RenderDrawRect( renderer, &HitBox );
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF) ;
}

// Player______________________________________________________________________________________________________________
Player::Player() : mVelX(20), mVelY(40), mTaccelX(0.0), mTaccelY(0.0),isAccelX(false), mDirection(1){
    Entity() ;
    mCollisionStatus["up"] = false ;
    mCollisionStatus["down"] = false ;
    mCollisionStatus["left"] = false ;
    mCollisionStatus["right"] = false ;
}

void Player::SetFullScreen(bool isFULLSCREEN, int playerWidth, int playerHeight){
    if(isFULLSCREEN){
        mWidth = playerWidth ;
        mHeight = playerHeight ;
        mVelX = 20 ;
        mVelY = 40 ;
    }
    else{
        mWidth = playerWidth/2 ;
        mHeight = playerHeight/2 ;
        mVelX = 10 ;
        mVelY = 20 ;
    }
}

void Player::HandleEvents(SDL_Event e){
    //If a key is pressed
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 ){
            //Adjust the velocity
            switch( e.key.keysym.sym ){
                case SDLK_LEFT: 
                    mDirection = -1 ;
                    isAccelX = true ;
                    break;
                case SDLK_RIGHT:
                    mDirection = 1 ;
                    isAccelX = true ;
                    break;
                case SDLK_SPACE :
                    mTaccelY = -1 ;
                    break ;
                default:
                    break;
            }
        }
    //If a key is released
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 ){
        //Adjust the velocity
        switch( e.key.keysym.sym ){
            case SDLK_LEFT:
                isAccelX = false ;
                break ;
            case SDLK_RIGHT:
                isAccelX = false ;
                break;
            default:
                break ;
        }
    }
}

bool Player::CheckCollisions(SDL_Rect* rect){
    //The sides of the rectangles
    int leftPlayer, leftRect;
    int rightPlayer, rightRect;
    int topPlayer, topRect;
    int bottomPlayer, bottomRect;

    //Calculate the sides of Player
    leftPlayer = mPosXabs + mPlayerCam->GetXoffset();
    rightPlayer = mPosXabs + mPlayerCam->GetXoffset() + mWidth;
    topPlayer = mPosYabs + mPlayerCam->GetYoffset();
    bottomPlayer = mPosYabs + mPlayerCam->GetYoffset() + mHeight;

    //Calculate the sides of Rect
    leftRect = rect->x;
    rightRect = rect->x + rect->w;
    topRect = rect->y;
    bottomRect = rect->y + rect->h;

    //Checks collisions
    if( bottomPlayer <= topRect ){return false ;}
    if( topPlayer >= bottomRect ){return false ;}
    if( rightPlayer <= leftRect ){return false ;}
    if( leftPlayer >= rightRect ){return false;}
    //If none of the sides from Player are outside Rect
    return true;
}

vector<SDL_Rect*> Player::GetCollisionsList(Map* map, SDL_Renderer* renderer){
    vector<SDL_Rect*> CollisionsList ;
    vector<vector<SDL_Rect*> > GameMap = map->GetGameMap() ;

    for(int i=0 ; i<GameMap.size(); i++){
        for(int j=0 ; j<GameMap[i].size() ; j++){
            if(GameMap[i][j] != nullptr){
                if(this->CheckCollisions(GameMap[i][j])){
                    CollisionsList.push_back(GameMap[i][j]) ;
                }
            }
        }
    }
    return CollisionsList ;
}


void Player::Move(Map* map, SDL_Renderer* renderer){
    mCollisionStatus["up"] = false ;
    mCollisionStatus["down"] = false ;
    mCollisionStatus["left"] = false ;
    mCollisionStatus["right"] = false ;

    vector<SDL_Rect*> CollisionsList ;
    
    //The player moves along X axis (acceleration taken into account)
    if(isAccelX){mTaccelX = min(1.5*double(mVelX)*mTaccelX+1, double(mVelX))/double(mVelX) ;}
    else{mTaccelX = max(0.4*double(mVelX)*mTaccelX, 0.0)/double(mVelX) ;}
    mPosXabs += mDirection*int(mTaccelX*mVelX);
    
    // Handle Left Right Collisions 
    CollisionsList = GetCollisionsList(map, renderer) ;
    for(int i=0 ; i< CollisionsList.size() ; i++){
        SDL_Rect* tile = CollisionsList[i] ;
        
        // Handle Left Right Collisions
        switch (mDirection){
        case 1:
            mPosXabs = tile->x - mWidth - mPlayerCam->GetXoffset();
            mCollisionStatus["right"] = true ;
            break;
        case -1:
            mPosXabs = tile->x + tile->w - mPlayerCam->GetXoffset();
            mCollisionStatus["left"] = true ;
            break;
        }
    }

    //The player moves along Y axis
    mTaccelY += 0.08 ;
    if (mTaccelY > 1){mTaccelY = 1 ;}
    mPosYabs += int(floor(mTaccelY*mVelY));

    // Handle Up Down Collisions
    CollisionsList = GetCollisionsList(map, renderer) ;
    for(int i=0 ; i< CollisionsList.size() ; i++){
        SDL_Rect* tile = CollisionsList[i] ;
        if(mTaccelY > 0){
            mTaccelY = 0 ;
            mPosYabs = tile->y - mHeight - mPlayerCam->GetYoffset() ;
            mCollisionStatus["down"] = true ;
        }
        else if(mTaccelY < 0){
            mPosYabs = tile->y - tile->h + mHeight- mPlayerCam->GetYoffset();
            mTaccelY = 0 ;
            mCollisionStatus["up"] = true ;
        }
    }
}

void Player::UpdateCam(){
    mPlayerCam->Update(mPosXabs,mPosYabs) ; 
    mPosXrel = mPosXabs-mPlayerCam->GetScrollX() ;
    mPosYrel = mPosYabs-mPlayerCam->GetScrollY() ;
}
