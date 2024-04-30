#include "SDL.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Błąd inicjalizacji SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    const int SZEROKOSC_EKRANU = 800;
    const int WYSOKOSC_EKRANU = 600;

    SDL_Window* okno = SDL_CreateWindow("SGD - PONG", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SZEROKOSC_EKRANU, WYSOKOSC_EKRANU, SDL_WINDOW_SHOWN);
    if (!okno) {
        std::cerr << "Wystąpił błąd z wygenerowaniem okna gry: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(okno, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Wystąpił błąd z wygenerowaniem renderera: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(okno);
        SDL_Quit();
        return 1;
    }

    bool koniecGry = false;
    SDL_Event event;
    while (!koniecGry) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                koniecGry = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(okno);
    SDL_Quit();
    return 0;
}
