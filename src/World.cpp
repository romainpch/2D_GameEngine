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
        cout<<"droite "<<mVisibleI<<" : "<<mVisibleJ<<endl ;
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
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF) ;
            }
        }
    }
}




void World::ConvertTileMapToPolyMap(){

    //Clear all edges : 
    mVecEdges.clear() ;
    for (int x = 0; x < 18 ; x++){
        for (int y = 0; y < 11; y++){
            for (int j = 0; j < 4; j++){
                mWorldCells[y * 18 + x].edge_exist[j] = false;
                mWorldCells[y * 18 + x].edge_id[j] = 0;
			}
        }
    }

    for (int x = 1; x < 17; x++)
        for (int y = 1; y < 10; y++)
        {
            // Create some convenient indices
            int i = y * 18 + x;			// This
            int n = (y - 1) * 18 + x;		// Northern Neighbour
            int s = (y + 1) * 18 + x;		// Southern Neighbour
            int w = y * 18 + (x - 1);	// Western Neighbour
            int e = y * 18 + (x + 1);	// Eastern Neighbour

            // If this cell exists, check if it needs edges
            if (mWorldCells[i].exist)
            {
                // If this cell has no western neighbour, it needs a western edge
                if (!mWorldCells[w].exist)
                {
                    // It can either extend it from its northern neighbour if they have
                    // one, or It can start a new one.
                    if (mWorldCells[n].edge_exist[WEST])
                    {
                        // Northern neighbour has a western edge, so grow it downwards
                        mVecEdges[mWorldCells[n].edge_id[WEST]].ey += 120;
                        mWorldCells[i].edge_id[WEST] = mWorldCells[n].edge_id[WEST];
                        mWorldCells[i].edge_exist[WEST] = true;
                    }
                    else
                    {
                        // Northern neighbour does not have one, so create one
                        sEdge edge;
                        edge.sx = x * 120; edge.sy = y * 120;
                        edge.ex = edge.sx; edge.ey = edge.sy + 120;

                        // Add edge to Polygon Pool
                        int edge_id = mVecEdges.size();
                        mVecEdges.push_back(edge);

                        // Update tile information with edge information
                        mWorldCells[i].edge_id[WEST] = edge_id;
                        mWorldCells[i].edge_exist[WEST] = true;
                    }
                }

                // If this cell dont have an eastern neignbour, It needs a eastern edge
                if (!mWorldCells[e].exist)
                {
                    // It can either extend it from its northern neighbour if they have
                    // one, or It can start a new one.
                    if (mWorldCells[n].edge_exist[EAST])
                    {
                        // Northern neighbour has one, so grow it downwards
                        mVecEdges[mWorldCells[n].edge_id[EAST]].ey += 120;
                        mWorldCells[i].edge_id[EAST] = mWorldCells[n].edge_id[EAST];
                        mWorldCells[i].edge_exist[EAST] = true;
                    }
                    else
                    {
                        // Northern neighbour does not have one, so create one
                        sEdge edge;
                        edge.sx = (x + 1) * 120; edge.sy = y * 120;
                        edge.ex = edge.sx; edge.ey = edge.sy + 120;

                        // Add edge to Polygon Pool
                        int edge_id = mVecEdges.size();
                        mVecEdges.push_back(edge);

                        // Update tile information with edge information
                        mWorldCells[i].edge_id[EAST] = edge_id;
                        mWorldCells[i].edge_exist[EAST] = true;
                    }
                }

                // If this cell doesnt have a northern neignbour, It needs a northern edge
                if (!mWorldCells[n].exist)
                {
                    // It can either extend it from its western neighbour if they have
                    // one, or It can start a new one.
                    if (mWorldCells[w].edge_exist[NORTH])
                    {
                        // Western neighbour has one, so grow it eastwards
                        mVecEdges[mWorldCells[w].edge_id[NORTH]].ex += 120;
                        mWorldCells[i].edge_id[NORTH] = mWorldCells[w].edge_id[NORTH];
                        mWorldCells[i].edge_exist[NORTH] = true;
                    }
                    else
                    {
                        // Western neighbour does not have one, so create one
                        sEdge edge;
                        edge.sx = x * 120; edge.sy = y * 120;
                        edge.ex = edge.sx + 120; edge.ey = edge.sy;

                        // Add edge to Polygon Pool
                        int edge_id = mVecEdges.size();
                        mVecEdges.push_back(edge);

                        // Update tile information with edge information
                        mWorldCells[i].edge_id[NORTH] = edge_id;
                        mWorldCells[i].edge_exist[NORTH] = true;
                    }
                }

                // If this cell doesnt have a southern neignbour, It needs a southern edge
                if (!mWorldCells[s].exist)
                {
                    // It can either extend it from its western neighbour if they have
                    // one, or It can start a new one.
                    if (mWorldCells[w].edge_exist[SOUTH])
                    {
                        // Western neighbour has one, so grow it eastwards
                        mVecEdges[mWorldCells[w].edge_id[SOUTH]].ex += 120;
                        mWorldCells[i].edge_id[SOUTH] = mWorldCells[w].edge_id[SOUTH];
                        mWorldCells[i].edge_exist[SOUTH] = true;
                    }
                    else
                    {
                        // Western neighbour does not have one, so I need to create one
                        sEdge edge;
                        edge.sx = x * 120; edge.sy = (y + 1) * 120;
                        edge.ex = edge.sx + 120; edge.ey = edge.sy;

                        // Add edge to Polygon Pool
                        int edge_id = mVecEdges.size();
                        mVecEdges.push_back(edge);

                        // Update tile information with edge information
                        mWorldCells[i].edge_id[SOUTH] = edge_id;
                        mWorldCells[i].edge_exist[SOUTH] = true;
                    }
                }

            }

        }
}

void World::CalculateVisibilityPolygon(float ox, float oy, float radius){
    // Get rid of existing polygon
    mVecVisibilityPolygonPoints.clear();

    // For each edge in PolyMap
    for (auto &e1 : mVecEdges)
    {
        // Take the start point, then the end point (we could use a pool of
        // non-duplicated points here, it would be more optimal)
        for (int i = 0; i < 2; i++)
        {
            float rdx, rdy;
            rdx = (i == 0 ? e1.sx : e1.ex) - ox;
            rdy = (i == 0 ? e1.sy : e1.ey) - oy;

            float base_ang = atan2f(rdy, rdx);

            float ang = 0;
            // For each point, cast 3 rays, 1 directly at point
            // and 1 a little bit either side
            for (int j = 0; j < 3; j++)
            {
                if (j == 0)	ang = base_ang - 0.0001f;
                if (j == 1)	ang = base_ang;
                if (j == 2)	ang = base_ang + 0.0001f;

                // Create ray along angle for required distance
                rdx = radius * cosf(ang);
                rdy = radius * sinf(ang);

                float min_t1 = INFINITY;
                float min_px = 0, min_py = 0, min_ang = 0;
                bool bValid = false;

                // Check for ray intersection with all edges
                for (auto &e2 : mVecEdges)
                {
                    // Create line segment vector
                    float sdx = e2.ex - e2.sx;
                    float sdy = e2.ey - e2.sy;

                    if (fabs(sdx - rdx) > 0.0f && fabs(sdy - rdy) > 0.0f)
                    {
                        // t2 is normalised distance from line segment start to line segment end of intersect point
                        float t2 = (rdx * (e2.sy - oy) + (rdy * (ox - e2.sx))) / (sdx * rdy - sdy * rdx);
                        // t1 is normalised distance from source along ray to ray length of intersect point
                        float t1 = (e2.sx + sdx * t2 - ox) / rdx;

                        // If intersect point exists along ray, and along line 
                        // segment then intersect point is valid
                        if (t1 > 0 && t2 >= 0 && t2 <= 1.0f)
                        {
                            // Check if this intersect point is closest to source. If
                            // it is, then store this point and reject others
                            if (t1 < min_t1)
                            {
                                min_t1 = t1;
                                min_px = ox + rdx * t1;
                                min_py = oy + rdy * t1;
                                min_ang = atan2f(min_py - oy, min_px - ox);
                                bValid = true;
                            }
                        }
                    }
                }

                if(bValid)// Add intersection point to visibility polygon perimeter
                    mVecVisibilityPolygonPoints.push_back({ min_ang, min_px, min_py });
            }
        }
    }
    // Sort perimeter points by angle from source. This will allow
    // us to draw a triangle fan.
    sort(
        mVecVisibilityPolygonPoints.begin(),
        mVecVisibilityPolygonPoints.end(),
        [&](const tuple<float, float, float> &t1, const tuple<float, float, float> &t2)
        {
            return get<0>(t1) < get<0>(t2);
        });
}