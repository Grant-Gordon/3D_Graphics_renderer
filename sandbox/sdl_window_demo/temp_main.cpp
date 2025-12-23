#include <SDL.h>
#include <SDL_image.h>
#include <RenderWindow.h>


int SCREEN_WIDTH = 256;
int SCREEN_HEIGHT = 256;

int main(int argc, char* args[]) {
    //Create SDL Window
    if (SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("SDL could not initialize. SDL Error: %s\n", SDL_GetError());
    }
    
    //Create SDL Window and Renderer
    RenderWindow window("SDL Window", SCREEN_WIDTH, SCREEN_HEIGHT);
    SDL_Renderer* renderer = window.GetRenderer();
   
    if (renderer == NULL){
        printf("Renderer could not be created. SDL Error: %s\n", SDL_GetError());
    }


    //Main game loop
    bool gameRunning = true;
    SDL_Event event;

    while (gameRunning) {
        while (SDL_PollEvent(&event) != 0){
            if (event.type == SDL_QUIT) {
                gameRunning = false;
            }
        }

        //set Renderer colo
        SDL_SetRenderDrawColor(renderer, 169, 169, 169, 1);

        //Clear screen
        SDL_RenderClear(renderer);

        //update screen
        SDL_RenderPresent(renderer);

    }

    window.close();

    return 0;
}