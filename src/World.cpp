#include "World.hpp"

// MAP : 16x9 avec des tuiles de 120*120
Tile::Tile(){
    mRectAbs = new SDL_Rect ;
    mRectAbs->x = 0 ;
    mRectAbs->y = 0 ;
    mRectAbs->w = 120 ;
    mRectAbs->h = 120 ;
    mRectRel = new SDL_Rect ;
    mRectRel->x = 0 ;
    mRectRel->y = 0 ;
    mRectRel->w = 120 ;
    mRectRel->h = 120 ;
}

Tile::~Tile(){
    delete mRectAbs ;
    delete mRectRel ;
}


// ___________________

World::World() : mWidth(21), mHeight(15), mVisibleI(0), mVisibleJ(0){}

World::~World(){
    delete mPlayerCam ;

    for(int i= 0 ; i<mTiles.size() ; i++){
        for(int j=0 ; j<mTiles[i].size() ; j++ ){
            delete mTiles[i][j] ;
        }
    }
}

void World::LoadFromFile(string path){
    for(int i=0 ; i< 15 ; i++){
        vector<Tile*> row ;
        for(int j=0 ; j<21 ; j++){
            row.push_back(nullptr) ;
        }
        mVisibleTiles.push_back(row) ;
    }

    ifstream MapFile(path) ;
    string row ;
    int i = 0 ;
    while(getline(MapFile,row)){
        vector<Tile*> layer ;
        for(int j = 0 ; j< row.length() ; j++){
            char c = row[j] ;
            if(int(c-'0')==1){
                Tile* tile  = new Tile;
                tile->mRectAbs->x = 120*(j-2); 
                tile->mRectAbs->y = 120*(i-2);
                tile->mRectRel->x = 120*(j-2); 
                tile->mRectRel->y = 120*(i-2);
                layer.push_back(tile) ;
                if(i < 15 and j < 21){
                    mVisibleTiles[i][j] = tile ;
                }
            }
            else{
                layer.push_back(nullptr) ;
            }
        }
        mTiles.push_back(layer) ;
        i++ ;
    }
}

void World::Update(){
    //Le joueur va en bas
    if(mPlayerCam->GetScrollY() + mPlayerCam->GetYoffset() > (mVisibleI*120 + 60)){
        vector<Tile*> newRow ;
        //On enleve la première ligne du haut de mVisibleTiles
        mVisibleTiles.erase(mVisibleTiles.begin()) ;
        if(mVisibleI+16 <= mTiles.size() ){//S'il existe des tiles à ajouter (qui sont dans mTiles)
            for(int j = mVisibleJ ; j < mVisibleJ + 21 ; j++){
                if(j<mTiles[mVisibleI+15].size()){
                    newRow.push_back(mTiles[mVisibleI+15][j]) ;
                }
                else{
                    newRow.push_back(nullptr) ;
                }
            }
        }
        else{
            for(int j = mVisibleJ ; j < mVisibleJ + 21 ; j++){
                newRow.push_back(nullptr) ;
            }
        }
        //On ajoute la nouvelle ligne en bas de mVisibleTiles et on décale I vers le bas
        mVisibleTiles.push_back(newRow) ;
        mVisibleI ++ ;
    }

    // Le joueur va en haut
    if(mPlayerCam->GetScrollY() + mPlayerCam->GetYoffset() < (mVisibleI*120 - 60)){
        vector<Tile*> newRow ;
        //On enleve la dernière ligne du bas de mVisibleTiles
        mVisibleTiles.erase(mVisibleTiles.end() - 1) ;
        if(mVisibleI > 0){//S'il existe des tiles à ajouter (qui sont dans mTiles)
            for(int j = mVisibleJ ; j < mVisibleJ + 21 ; j++){
                if(j<mTiles[mVisibleI-1].size()){
                    newRow.push_back(mTiles[mVisibleI-1][j]) ;
                }
                else{
                    newRow.push_back(nullptr) ;
                }
            }
        }
        else{
            for(int j = mVisibleJ ; j < mVisibleJ + 21 ; j++){
                newRow.push_back(nullptr) ;
            }
        }
        //On ajoute la nouvelle ligne en haut de mVisibleTiles et on décale I vers le haut
        mVisibleTiles.insert(mVisibleTiles.begin(), newRow) ;
        mVisibleI -- ;
    }

    //Le joueur va à droite
    if(mPlayerCam->GetScrollX() + mPlayerCam->GetXoffset() > (mVisibleJ*120 + 60)){
        for(int i=0 ; i<15 ; i++){
            //On supprime à gauche l'elément de mVisibleTiles
            mVisibleTiles[i].erase(mVisibleTiles[i].begin()) ;
            //On ajoute à droite le bon élement de mTiles
            if(mVisibleI >= 0 and  mVisibleI+i < mTiles.size() and mVisibleJ+21 < mTiles[mVisibleI+i].size()){
                mVisibleTiles[i].push_back(mTiles[mVisibleI+i][mVisibleJ+21]) ;
            }
            else{
                mVisibleTiles[i].push_back(nullptr) ;
            }
        }
        mVisibleJ ++ ;
    }

    //Le joueur va à gauche
    if(mPlayerCam->GetScrollX() + mPlayerCam->GetXoffset() < (mVisibleJ*120 - 60)){
        for(int i=0 ; i<15 ; i++){
            //On supprime à droite l'elément de mVisibleTiles
            mVisibleTiles[i].erase(mVisibleTiles[i].end() - 1) ;
            //On ajoute à gauche le bon élement de mTiles
            if(mVisibleI >= 0 and mVisibleI+i < mTiles.size() and mVisibleJ > 0 and mVisibleJ - 1 < mTiles[mVisibleI+i].size()){
                mVisibleTiles[i].insert(mVisibleTiles[i].begin(), mTiles[mVisibleI+i][mVisibleJ - 1] ) ;
            }
            else{
                mVisibleTiles[i].insert(mVisibleTiles[i].begin(), nullptr) ;
            }
        }
        mVisibleJ -- ;
    }



    for(int i=0 ; i<15; i++){
        for(int j=0 ; j<21 ; j++){
            if( mVisibleTiles[i][j]!= nullptr){
                mVisibleTiles[i][j]->mRectRel->x = mVisibleTiles[i][j]->mRectAbs->x  - (mPlayerCam->GetScrollX() + mPlayerCam->GetXoffset()) ;
                mVisibleTiles[i][j]->mRectRel->y = mVisibleTiles[i][j]->mRectAbs->y - (mPlayerCam->GetScrollY()  + mPlayerCam->GetYoffset()) ;
            }
        }
    }
}


void World::Render(SDL_Renderer* renderer){
    SDL_Rect RectToRender ;
    for(int i=0 ; i<15; i++){
        for(int j=0 ; j<21 ; j++){
            if( mVisibleTiles[i][j]!= nullptr){
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xFF) ;
                SDL_RenderFillRect(renderer, mVisibleTiles[i][j]->mRectRel);

                // SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0, 0xFF) ;
                // SDL_RenderDrawRect(renderer, mVisibleTiles[i][j]->mRectRel);
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF) ;
            }
        }
    }
}
