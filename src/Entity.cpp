#include "Entity.hpp"


// Entity______________________________________________________________________________________________________________
Entity::Entity() : mFlip(SDL_FLIP_NONE), mAnimationDatabase(0), mAction(""), mActionFrameNumber(0) {
    mHitboxAbs = new SDL_Rect;
    mHitboxAbs->x = 0 ;
    mHitboxAbs->y = 0 ;
    mHitboxAbs->w = 0 ;
    mHitboxAbs->h = 0 ;

    mHitboxRel = new SDL_Rect;
    mHitboxRel->x = 0 ;
    mHitboxRel->y = 0 ;
    mHitboxRel->w = 0 ;
    mHitboxRel->h = 0 ;
}

Entity::~Entity(){
    delete mHitboxAbs ;
    delete mHitboxRel ;
}

void Entity::SetPosAbs(int Xabs, int Yabs){
    mHitboxAbs->x = Xabs ;
    mHitboxAbs->y = Yabs ;
}

void Entity::SetPosRel(int Xrel, int Yrel){
    mHitboxRel->x = Xrel ;
    mHitboxRel->y = Yrel ;
}

void Entity::SetDimension(int Width, int Height){
    mHitboxAbs->w = Width ;
    mHitboxAbs->h = Height ;
    mHitboxRel->w = Width ;
    mHitboxRel->h = Height ;
}

void Entity::AddAnimation(string animationName, string path, vector<int> animationLengths, string nextAnimation){
    Animation* animation = new Animation(animationName, path, animationLengths, nextAnimation) ;
    mAnimationDatabase[animationName] = animation ;
}

void Entity::Render(SDL_Renderer * renderer){
    SDL_Surface* frameSurface = mAnimationDatabase[mAction]->GetImage(mActionFrameNumber) ;
    SDL_Rect frameRect ;
    frameRect.x = mHitboxRel->x - 80 ;
    frameRect.y = mHitboxRel->y ;
    frameRect.w = 256 ;
    frameRect.h = 256 ;

    SDL_SetColorKey( frameSurface, SDL_TRUE, SDL_MapRGBA( frameSurface->format, 0xFF, 0xFF, 0xFF, 0xFF) );
    SDL_Texture* frameTexture = SDL_CreateTextureFromSurface(renderer, frameSurface);
    SDL_RenderCopyEx(renderer, frameTexture,  NULL, &frameRect, 0, NULL, mFlip);  
}

void Entity::ShowHitbox(SDL_Renderer * renderer){
    SDL_SetRenderDrawColor(renderer, 0, 0xFF, 0, 0xFF) ;
    SDL_RenderDrawRect( renderer, mHitboxRel );
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
        mHitboxAbs->w = playerWidth ;
        mHitboxAbs->h = playerHeight ;
        mVelX = 20 ;
        mVelY = 40 ;
    }
    else{
        mHitboxAbs->w = playerWidth/2 ;
        mHitboxAbs->h = playerHeight/2 ;
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
                    mFlip = SDL_FLIP_HORIZONTAL ;
                    mDirection = -1 ;
                    isAccelX = true ;
                    break;
                case SDLK_RIGHT:
                    mFlip = SDL_FLIP_NONE ;
                    mDirection = 1 ;
                    isAccelX = true ;
                    break;
                case SDLK_SPACE :
                    if(mTaccelY==0){
                        mTaccelY = -1 ;
                    }
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
    leftPlayer = mHitboxAbs->x + mPlayerCam->GetXoffset();
    rightPlayer = mHitboxAbs->x + mPlayerCam->GetXoffset() + mHitboxAbs->w;
    topPlayer =  mHitboxAbs->y + mPlayerCam->GetYoffset();
    bottomPlayer = mHitboxAbs->y + mPlayerCam->GetYoffset() + mHitboxAbs->h;

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
    mHitboxAbs->x += mDirection*int(mTaccelX*mVelX);
    
    // Handle Left Right Collisions 
    CollisionsList = GetCollisionsList(map, renderer) ;
    for(int i=0 ; i< CollisionsList.size() ; i++){
        SDL_Rect* tile = CollisionsList[i] ;
        
        // Handle Left Right Collisions
        switch (mDirection){
        case 1:
            mHitboxAbs->x = tile->x - mHitboxAbs->w - mPlayerCam->GetXoffset();
            mCollisionStatus["right"] = true ;
            break;
        case -1:
            mHitboxAbs->x = tile->x + tile->w - mPlayerCam->GetXoffset();
            mCollisionStatus["left"] = true ;
            break;
        }
    }

    //The player moves along Y axis
    mTaccelY += 0.08 ;
    if (mTaccelY > 1){mTaccelY = 1 ;}
    mHitboxAbs->y += int(floor(mTaccelY*mVelY));

    // Handle Up Down Collisions
    CollisionsList = GetCollisionsList(map, renderer) ;
    for(int i=0 ; i< CollisionsList.size() ; i++){
        SDL_Rect* tile = CollisionsList[i] ;
        if(mTaccelY > 0){
            mTaccelY = 0 ;
            mHitboxAbs->y = tile->y - mHitboxAbs->h - mPlayerCam->GetYoffset() ;
            mCollisionStatus["down"] = true ;
        }
        else if(mTaccelY < 0){
            mHitboxAbs->y = tile->y - tile->h + mHitboxAbs->h - mPlayerCam->GetYoffset();
            mTaccelY = 0 ;
            mCollisionStatus["up"] = true ;
        }
    }

    //Animation of the player 
    mActionFrameNumber ++ ;

    if(mActionFrameNumber == mAnimationDatabase[mAction]->GetSize()){
        mAction = mAnimationDatabase[mAction]->GetNext() ;
        mActionFrameNumber = 0 ;
    }
}

void Player::UpdateCam(){
    mPlayerCam->Update(mHitboxAbs->x,mHitboxAbs->y) ; 
    mHitboxRel->x = mHitboxAbs->x-mPlayerCam->GetScrollX() ;
    mHitboxRel->y = mHitboxAbs->y-mPlayerCam->GetScrollY() ;
}
