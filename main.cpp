#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "./src/Timer.cpp"
#include "./src/Entity.cpp"
#include "./src/World.cpp"
#include "./src/Camera.cpp"
#include "./src/Light.cpp"
#include "./src/Animation.cpp"



using namespace std ;

// SDL Settings
const int SCREEN_WIDTH = 1920;
const int SCREEN_HEIGHT = 1080;
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL ;

//The frames per second timer
LTimer fpsTimer;

bool init(){
	//Initialization flag
	bool success = true;
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ){
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "Mysterious Fox", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN);
		if( gWindow == NULL ){
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
        else{
            // Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ); //| SDL_RENDERER_PRESENTVSYNC 
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "0"); //For pixel art
				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
        }
	}
	return success;
}

void close()
{
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

int main(int argc, char const *argv[]){   
    //Start up SDL and create window
	if( !init() ){
		printf( "Failed to initialize!\n" );
	}
    else{
        //Camera Initialisation
        int PlayerXOffset(360) ;
        // int PlayerYOffset(704) ;
        int PlayerYOffset(824) ;
        Camera* Game_Cam = new Camera(PlayerXOffset, PlayerYOffset) ;

        //Map Initialisation
        World* Game_Map = new World;
        Game_Map->LoadFromFile("./data/map.txt") ;
        Game_Map->SetPlayerCam(Game_Cam) ;

        //Player Initialisation
        int PlayerWidth(6*16) ;
        int PlayerHeight(16*16) ;
        Player Game_Player ;
        Game_Player.SetDimension(PlayerWidth,PlayerHeight) ;
        Game_Player.SetCamera(Game_Cam) ;

        //Light Initialisation
        Light* Player_Light = new Light;
        Game_Player.SetLight(Player_Light) ;

    
        bool isFULLSCREEN = true ;
        bool quit = false;

        SDL_Event e ;
        //Start counting frames per second
        int countedFrames = 0;
        fpsTimer.start();
        while(!quit){
            SDL_RenderClear( gRenderer );
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 ){
                // if(e.window.event == SDL_WINDOWEVENT_RESIZED ){
                //     cout<<e.window.data1<<" : "<<e.window.data2<<endl ;
                // }
                //User requests quit
                if(e.type == SDL_QUIT){
                    quit = true;
                }
                if(e.type == SDL_KEYDOWN){
                    if(e.key.keysym.sym == SDLK_ESCAPE){
                        quit = true ;
                    }
                    if(e.key.keysym.sym == SDLK_f){
                        isFULLSCREEN = !isFULLSCREEN ;
                        if(!(isFULLSCREEN)){
                            SDL_SetWindowSize(gWindow , SCREEN_WIDTH/2 , SCREEN_HEIGHT/2);
                            SDL_SetWindowFullscreen(gWindow , SDL_FALSE);
                            Game_Player.SetFullScreen(isFULLSCREEN, PlayerWidth, PlayerHeight) ;
                            Game_Cam->SetFullScreen(isFULLSCREEN, PlayerXOffset, PlayerYOffset) ;
                            // Game_Map->SetFullScreen(isFULLSCREEN) ;
                        }
                        else{
                            SDL_SetWindowSize(gWindow , SCREEN_WIDTH , SCREEN_HEIGHT);
                            SDL_SetWindowFullscreen(gWindow , SDL_TRUE);
                            Game_Player.SetFullScreen(isFULLSCREEN, PlayerWidth, PlayerHeight) ;
                            Game_Cam->SetFullScreen(isFULLSCREEN, PlayerXOffset, PlayerYOffset) ;
                            // Game_Map->SetFullScreen(isFULLSCREEN) ;
                        }
                    }

                    
                }
                Game_Player.HandleEvents(e) ;
            }
            

            Game_Player.Move(Game_Map, gRenderer) ;
            Game_Player.UpdateCam() ;
            Game_Map->Update() ;
            // Game_Player.UpdateLight(Game_Map) ;

            
            // Game_Map->RenderBackGround(gRenderer) ;
            Game_Map->Render(gRenderer) ;
            // Game_Player.RenderLight(gRenderer) ;
            Game_Player.Render(gRenderer) ;
            // Game_Player.ShowHitbox(gRenderer) ;


            //Update the surface
            SDL_RenderPresent( gRenderer );
            //Calculate and correct fps
            float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
            cout<<avgFPS<<endl ;
            ++countedFrames;
            // quit=true ;
        }
    }
    //Free resources and close SDL
    close() ;

    return 0;
}
// g++ main.cpp $(sdl2-config --cflags --libs) -lSDL2_Image




//Start counting frames per second
// int countedFrames = 0;
// fpsTimer.start();
//Calculate and correct fps
// float avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );
// cout<<avgFPS<<endl ;
// ++countedFrames;


        // BG_Object object1(0.25,300,400,700,800,131,131,131) ;
        // BG_Object object2(0.25,750,300,500,800,131,131,131) ;
        // BG_Object object3(0.5,1500,350,400,800,108,108,108) ;
        // BG_Object object4(0.5,250,100,100,800,108,108,108) ;
        // BG_Object object5(0.5,600,200,300,800,108,108,108) ;
        // Game_Map->AddBGObject(&object1) ;
        // Game_Map->AddBGObject(&object2) ;
        // Game_Map->AddBGObject(&object3) ;
        // Game_Map->AddBGObject(&object4) ;
        // Game_Map->AddBGObject(&object5) ;