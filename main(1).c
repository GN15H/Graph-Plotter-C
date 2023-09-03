#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "Librerias/Grafica.h"

#define MAX_TEXTS 3

const int WIDTH = 800, HEIGHT = 600;

typedef struct {
    char content[100];
    SDL_Surface* surface;
    SDL_Texture* texture;
    SDL_Rect destination;
} Text;

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    SDL_Window* window = SDL_CreateWindow("SDL2 Text Rendering", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    TTF_Font* font = TTF_OpenFont("fonts/arial.ttf", 28);
    if (font == NULL) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Color color = {255, 255, 255}; // White color

    Text texts[MAX_TEXTS];

    // Initialize Text 1
    strcpy(texts[0].content, "Hello, SDL2!");
    texts[0].surface = TTF_RenderText_Solid(font, texts[0].content, color);
    texts[0].texture = SDL_CreateTextureFromSurface(renderer, texts[0].surface);
    texts[0].destination.x = 100;
    texts[0].destination.y = 100;
    texts[0].destination.w = texts[0].surface->w;
    texts[0].destination.h = texts[0].surface->h;

    // Initialize Text 2
    strcpy(texts[1].content, "Welcome to SDL2!");
    texts[1].surface = TTF_RenderText_Solid(font, texts[1].content, color);
    texts[1].texture = SDL_CreateTextureFromSurface(renderer, texts[1].surface);
    texts[1].destination.x = 200;
    texts[1].destination.y = 200;
    texts[1].destination.w = texts[1].surface->w;
    texts[1].destination.h = texts[1].surface->h;

    // Initialize Text 3
    strcpy(texts[2].content, "Have a great day!");
    texts[2].surface = TTF_RenderText_Solid(font, texts[2].content, color);
    texts[2].texture = SDL_CreateTextureFromSurface(renderer, texts[2].surface);
    texts[2].destination.x = 300;
    texts[2].destination.y = 300;
    texts[2].destination.w = texts[2].surface->w;
    texts[2].destination.h = texts[2].surface->h;

    // Render loop
    int quit = 0;
    SDL_Event event;
    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = 1;
            }
        }

        SDL_RenderClear(renderer);

        // Render all texts
        for (int i = 0; i < MAX_TEXTS; i++) {
            SDL_RenderCopy(renderer, texts[i].texture, NULL, &texts[i].destination);
        }

        SDL_RenderPresent(renderer);
    }

    // Cleanup and exit
    for (int i = 0; i < MAX_TEXTS; i++) {
        SDL_DestroyTexture(texts[i].texture);
        SDL_FreeSurface(texts[i].surface);
    }

    TTF_CloseFont(font);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    TTF_Quit();
    SDL_Quit();

    return 0;
}
