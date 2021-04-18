#include "World.hpp"

// MAP : 16x9 avec des tuiles de 120*120
Tile::Tile(){
    mRectAbs = new SDL_Rect ;
    mRectAbs->x = 0 ;
    mRectAbs->y = 0 ;
    mRectAbs->w = 80 ;
    mRectAbs->h = 80 ;
    mRectRel = new SDL_Rect ;
    mRectRel->x = 0 ;
    mRectRel->y = 0 ;
    mRectRel->w = 80 ;
    mRectRel->h = 80 ;
}

Tile::~Tile(){
    delete mRectAbs ;
    delete mRectRel ;
}


// ___________________

World::World() : mVisibleI(0), mVisibleJ(0){}

World::~World(){
    delete mPlayerCam ;

    for(int i= 0 ; i<mTiles.size() ; i++){
        for(int j=0 ; j<mTiles[i].size() ; j++ ){
            delete mTiles[i][j] ;
        }
    }
}

// 16+2+2=20 x 10+2+2=14  -- tile = 80px

void World::LoadFromFile(string path){
    //Initialisation d'une matrice 18*12 de nullptr pour mVisible tiles
    for(int i=0 ; i< 14 ; i++){
        vector<Tile*> row ;
        for(int j=0 ; j<20 ; j++){
            row.push_back(nullptr) ;
        }
        mVisibleTiles.push_back(row) ;
    }

    //Lecture de Map.txt
    ifstream MapFile(path) ;
    string row ;
    int i = 0 ;
    while(getline(MapFile,row)){
        vector<Tile*> layer ;
        for(int j = 0 ; j< row.length() ; j++){
            char c = row[j] ;
            if(int(c-'0')==1){
                Tile* tile  = new Tile;
                tile->mRectAbs->x = 80*(j-2); 
                tile->mRectAbs->y = 80*(i-2);
                tile->mRectRel->x = 80*(j-2); 
                tile->mRectRel->y = 80*(i-2);
                layer.push_back(tile) ;
                if(i < 14 and j < 20){
                    mVisibleTiles[i][j] = tile ;
                }
            }
            else if(int(c-'0')==2){
                Tile* tile  = new Tile;
                tile->mRectAbs->x = 80*(j-2); 
                tile->mRectAbs->y = 80*(i-2);
                tile->mRectAbs->h = 40;
                tile->mRectRel->x = 80*(j-2); 
                tile->mRectRel->y = 80*(i-2);
                tile->mRectRel->h = 40;
                layer.push_back(tile) ;
                if(i < 14 and j < 20){
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
    if(mPlayerCam->GetScrollY() + mPlayerCam->GetYoffset() > (mVisibleI*80 + 40)){
        vector<Tile*> newRow ;
        //On enleve la première ligne du haut de mVisibleTiles
        mVisibleTiles.erase(mVisibleTiles.begin()) ;
        if(mVisibleI+14 < mTiles.size()){//S'il existe des tiles à ajouter (qui sont dans mTiles)
            for(int j = mVisibleJ ; j < mVisibleJ + 20 ; j++){
                if(j<mTiles[mVisibleI+14].size()){
                    newRow.push_back(mTiles[mVisibleI+14][j]) ;
                }
                else{
                    newRow.push_back(nullptr) ;
                }
            }
        }
        else{
            for(int j = mVisibleJ ; j < mVisibleJ + 20 ; j++){
                newRow.push_back(nullptr) ;
            }
        }
        //On ajoute la nouvelle ligne en bas de mVisibleTiles et on décale I vers le bas
        mVisibleTiles.push_back(newRow) ;
        mVisibleI ++ ;
    }

    // Le joueur va en haut
    if(mPlayerCam->GetScrollY() + mPlayerCam->GetYoffset() < (mVisibleI*80 - 40)){
        vector<Tile*> newRow ;
        //On enleve la dernière ligne du bas de mVisibleTiles
        mVisibleTiles.erase(mVisibleTiles.end() - 1) ;
        if(mVisibleI > 0){//S'il existe des tiles à ajouter (qui sont dans mTiles)
            for(int j = mVisibleJ ; j < mVisibleJ + 20 ; j++){
                if(j<mTiles[mVisibleI-1].size()){
                    newRow.push_back(mTiles[mVisibleI-1][j]) ;
                }
                else{
                    newRow.push_back(nullptr) ;
                }
            }
        }
        else{
            for(int j = mVisibleJ ; j < mVisibleJ + 20 ; j++){
                newRow.push_back(nullptr) ;
            }
        }
        //On ajoute la nouvelle ligne en haut de mVisibleTiles et on décale I vers le haut
        mVisibleTiles.insert(mVisibleTiles.begin(), newRow) ;
        mVisibleI -- ;
    }

    //Le joueur va à droite
    if(mPlayerCam->GetScrollX() + mPlayerCam->GetXoffset() > (mVisibleJ*80 + 40)){
        for(int i=0 ; i<14 ; i++){
            //On supprime à gauche l'elément de mVisibleTiles
            mVisibleTiles[i].erase(mVisibleTiles[i].begin()) ;
            //On ajoute à droite le bon élement de mTiles
            if(mVisibleI+i >= 0 and  mVisibleI+i < mTiles.size() and mVisibleJ+20 < mTiles[mVisibleI+i].size()){
                mVisibleTiles[i].push_back(mTiles[mVisibleI+i][mVisibleJ+20]) ;
            }
            else{
                mVisibleTiles[i].push_back(nullptr) ;
            }
        }
        mVisibleJ ++ ;
    }

    //Le joueur va à gauche
    if(mPlayerCam->GetScrollX() + mPlayerCam->GetXoffset() < (mVisibleJ*80 - 40)){
        for(int i=0 ; i<14 ; i++){
            //On supprime à droite l'elément de mVisibleTiles
            mVisibleTiles[i].erase(mVisibleTiles[i].end() - 1) ;
            //On ajoute à gauche le bon élement de mTiles
            if(mVisibleI+i >= 0 and mVisibleI+i < mTiles.size() and mVisibleJ > 0 and mVisibleJ - 1 < mTiles[mVisibleI+i].size()){
                mVisibleTiles[i].insert(mVisibleTiles[i].begin(), mTiles[mVisibleI+i][mVisibleJ - 1] ) ;
            }
            else{
                mVisibleTiles[i].insert(mVisibleTiles[i].begin(), nullptr) ;
            }
        }
        mVisibleJ -- ;
    }



    for(int i=0 ; i<14; i++){
        for(int j=0 ; j<20 ; j++){
            if( mVisibleTiles[i][j]!= nullptr){
                mVisibleTiles[i][j]->mRectRel->x = mVisibleTiles[i][j]->mRectAbs->x  - (mPlayerCam->GetScrollX() + mPlayerCam->GetXoffset()) ;
                mVisibleTiles[i][j]->mRectRel->y = mVisibleTiles[i][j]->mRectAbs->y - (mPlayerCam->GetScrollY()  + mPlayerCam->GetYoffset()) ;
            }
        }
    }
}


void World::Render(SDL_Renderer* renderer){
    SDL_Rect RectToRender ;
    for(int i=0 ; i<14; i++){
        for(int j=0 ; j<20 ; j++){
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
