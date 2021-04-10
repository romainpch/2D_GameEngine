#include "Light.hpp"
#include "SDL2/SDL2_gfxPrimitives.h"

Light::Light():mPosRelX(0), mPosRelY(0){
    mWorldCells = new sCell[20 * 14];
    LightSphere = IMG_Load("../data/LightSphere.png") ;
}

Light::~Light(){
    delete mWorldCells ;
}



void Light::ConvertTileMapToPolyMap(vector<vector< Tile*> > TileMap){
    //Clear all edges : 
    mVecEdges.clear() ;
    for (int x = 0; x < 20 ; x++){
        for (int y = 0; y < 14; y++){
            if(TileMap[y][x] != nullptr){ //x <= 1 or y<=0 or x>=19 or y>=13 or 
                mWorldCells[y * 20 + x].exist = true ;
            }
            else{
                mWorldCells[y * 20 + x].exist = false ;
            }

            for (int j = 0; j < 4; j++){
                mWorldCells[y * 20 + x].edge_exist[j] = false;
                mWorldCells[y * 20 + x].edge_id[j] = 0;
			}
        }
    }

    for (int x = 1; x < 19; x++)
        for (int y = 1; y < 13; y++)
        {
            // Create some convenient indices
            int i = y * 20 + x;			// This
            int n = (y - 1) * 20 + x;		// Northern Neighbour
            int s = (y + 1) * 20 + x;		// Southern Neighbour
            int w = y * 20 + (x - 1);	// Western Neighbour
            int e = y * 20 + (x + 1);	// Eastern Neighbour

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
                        mVecEdges[mWorldCells[n].edge_id[WEST]].ey += 80;
                        mWorldCells[i].edge_id[WEST] = mWorldCells[n].edge_id[WEST];
                        mWorldCells[i].edge_exist[WEST] = true;
                    }
                    else
                    {
                        // Northern neighbour does not have one, so create one
                        sEdge edge;
                        edge.sx = TileMap[y][x]->mRectRel->x  ; edge.sy = TileMap[y][x]->mRectRel->y ;
                        edge.ex = edge.sx; edge.ey = edge.sy + 80;

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
                        mVecEdges[mWorldCells[n].edge_id[EAST]].ey += 80;
                        mWorldCells[i].edge_id[EAST] = mWorldCells[n].edge_id[EAST];
                        mWorldCells[i].edge_exist[EAST] = true;
                    }
                    else
                    {
                        // Northern neighbour does not have one, so create one
                        sEdge edge;
                        edge.sx = TileMap[y][x]->mRectRel->x + 80  ; edge.sy = TileMap[y][x]->mRectRel->y ;
                        edge.ex = edge.sx; edge.ey = edge.sy + 80;

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
                        mVecEdges[mWorldCells[w].edge_id[NORTH]].ex += 80;
                        mWorldCells[i].edge_id[NORTH] = mWorldCells[w].edge_id[NORTH];
                        mWorldCells[i].edge_exist[NORTH] = true;
                    }
                    else
                    {
                        // Western neighbour does not have one, so create one
                        sEdge edge;
                        edge.sx = TileMap[y][x]->mRectRel->x ; edge.sy = TileMap[y][x]->mRectRel->y ;
                        edge.ex = edge.sx + 80; edge.ey = edge.sy;

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
                        mVecEdges[mWorldCells[w].edge_id[SOUTH]].ex += 80;
                        mWorldCells[i].edge_id[SOUTH] = mWorldCells[w].edge_id[SOUTH];
                        mWorldCells[i].edge_exist[SOUTH] = true;
                    }
                    else
                    {
                        // Western neighbour does not have one, so I need to create one
                        sEdge edge;
                        edge.sx = TileMap[y][x]->mRectRel->x ; edge.sy = TileMap[y][x]->mRectRel->y + 80 ;
                        edge.ex = edge.sx + 80; edge.ey = edge.sy;

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

        //Prendre en compte x = 0, 20 et y= 0, 14
}

void Light::CalculateVisibilityPolygon(float ox, float oy, float radius){
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
            rdx = (i == 0 ? e1.sx : e1.ex) - ox ;
            rdy = (i == 0 ? e1.sy : e1.ey) - oy ;

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
    
    //Remove duplicated rays
    auto it = unique(
    mVecVisibilityPolygonPoints.begin(),
    mVecVisibilityPolygonPoints.end(),
    [&](const tuple<float, float, float> &t1, const tuple<float, float, float> &t2)
    {
        return fabs(get<1>(t1) - get<1>(t2)) < 0.1f && fabs(get<2>(t1) - get<2>(t2)) < 0.1f;
    });

    mVecVisibilityPolygonPoints.resize(distance(mVecVisibilityPolygonPoints.begin(), it));
}


void Light::FillTriangle(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3, Uint8 r,  Uint8 g,  Uint8 b,  Uint8 a){
    filledTrigonRGBA(renderer,  x1,  y1,  x2,  y2,  x3,  y3,  r,  g,  b,  a);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF) ;
//     //Sorting (x,y) by y ascending
//     int xS1, yS1, xS2, yS2, xS3, yS3 ;
//     yS1 = min(y1,min(y2,y3)) ;
//     if(yS1==y1){
//         xS1 = x1 ;
//         yS2 = min(y2,y3) ;
//         if(yS2 == y2){
//             xS2 = x2 ;
//             xS3 = x3 ;
//             yS3 = y3 ;
//         }
//         else if(yS2 == y3){
//             xS2 = x3 ;
//             xS3 = x2 ;
//             yS3 = y2 ;
//         }
//     }
//     else if(yS1 == y2){
//         xS1 = x2 ;
//         yS2 = min(y1,y3) ;
//         if(yS2 == y1){
//             xS2 = x1 ;
//             xS3 = x3 ;
//             yS3 = y3 ;
//         }
//         else if(yS2 == y3){
//             xS2 = x3 ;
//             xS3 = x1 ;
//             yS3 = y1 ;
//         }
//     }
//     else if(yS1 == y3){
//         xS1 = x3 ;
//         yS2 = min(y1,y2) ;
//         if(yS2 == y1){
//             xS2 = x1 ;
//             xS3 = x2 ;
//             yS3 = y2 ;
//         }
//         else if(yS2 == y2){
//             xS2 = x2 ;
//             xS3 = x1 ;
//             yS3 = y1 ;
//         }      
//     }

//     SDL_SetRenderDrawColor(renderer, r, g, b, a) ;
//     int xS4((int)(xS1 + ((float)(yS2 - yS1) / (float)(yS3 - yS1)) * (xS3 - xS1))) ;
//     int yS4(yS2) ;

//     //Draw Bottom Flat Triangle
//     float invslope1 = (float)(xS2 - xS1) / (float)(yS2 - yS1);
//     float invslope2 = (float)(xS4 - xS1) / (float)(yS4 - yS1);
//     float curx1 = xS1;
//     float curx2 = xS1;
//     for (int scanlineY = yS1; scanlineY <= yS2; scanlineY++){
//         SDL_RenderDrawLine(renderer, (int)curx1, scanlineY, (int)curx2, scanlineY);
//         curx1 += invslope1;
//         curx2 += invslope2;
//     }

//     //Draw Top Flat Triangle
//     invslope1 = (float)(xS3 - xS2) / (float)(yS3 - yS2);
//     invslope2 = (float)(xS3 - xS4) / (float)(yS3 - yS4);
//     curx1 = xS3;
//     curx2 = xS3;
//     for (int scanlineY = yS3; scanlineY > yS2; scanlineY--){
//         SDL_RenderDrawLine(renderer, (int)curx1, scanlineY, (int)curx2, scanlineY);
//         curx1 -= invslope1;
//         curx2 -= invslope2;
//     }

//     SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF) ;
}

void Light::DrawTriangle(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3, Uint8 r,  Uint8 g,  Uint8 b,  Uint8 a){
    SDL_SetRenderDrawColor(renderer, r, g, b, a) ;
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
    SDL_RenderDrawLine(renderer, x3, y3, x2, y2);
    SDL_RenderDrawLine(renderer, x1, y1, x3, y3);
    SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF) ;
}

void Light::Update(int posXabs, int posYabs, Camera* playerCam, vector<vector< Tile*> > TileMap){
    mPosRelX =  posXabs - playerCam->GetScrollX() + 40 ;
    mPosRelY = posYabs - playerCam->GetScrollY() + 80 ;

    ConvertTileMapToPolyMap(TileMap) ;
    CalculateVisibilityPolygon(mPosRelX, mPosRelY, 1.0f) ;
}

void Light::IntesectTriangleWithCircle(SDL_Renderer* renderer, int x1, int y1, int x2, int y2, int x3, int y3, int radius){
    
}

void Light::Render(SDL_Renderer* renderer){
    // Draw each triangle in fan
    // cout<<mVecVisibilityPolygonPoints.size()<<" Triangles"<<endl ;
    for (int i = 0; i < mVecVisibilityPolygonPoints.size() - 1; i++){
        FillTriangle(renderer, mPosRelX, mPosRelY, get<1>(mVecVisibilityPolygonPoints[i]), get<2>(mVecVisibilityPolygonPoints[i]), get<1>(mVecVisibilityPolygonPoints[i + 1]), get<2>(mVecVisibilityPolygonPoints[i + 1]), 0xFF, 0xFF, 0, 0xFF) ;
    }

    // // Fan will have one open edge, so draw last point of fan to first
    FillTriangle(renderer, mPosRelX, mPosRelY, get<1>(mVecVisibilityPolygonPoints[mVecVisibilityPolygonPoints.size() - 1]), get<2>(mVecVisibilityPolygonPoints[mVecVisibilityPolygonPoints.size() - 1]), get<1>(mVecVisibilityPolygonPoints[0]), get<2>(mVecVisibilityPolygonPoints[0]), 0xFF, 0xFF, 0, 0xFF) ; 

}