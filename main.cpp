#include "SDL.h"
#include <iostream>

const int SZEROKOSC_EKRANU = 800;
const int WYSOKOSC_EKRANU = 600;
const int SZEROKOSC_PALETKI = 50;
const int WYSOKOSC_PALETKI = 20;
const int WIELKOSC_PILKI = 5;

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "Błąd inicjalizacji SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

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

    struct Paletka {
        SDL_Rect prostokat;
        int predkosc;

        Paletka(int x, int y) {
            prostokat = {x, y, SZEROKOSC_PALETKI, WYSOKOSC_PALETKI};
            predkosc = 0;
        }

        void move() {
            prostokat.y += predkosc;

            if (prostokat.y < 0) {
                prostokat.y = 0;
            }
            else if (prostokat.y > WYSOKOSC_EKRANU - WYSOKOSC_PALETKI) {
                prostokat.y = WYSOKOSC_EKRANU - WYSOKOSC_PALETKI;
            }
        }
    };

    struct Pilka {
        SDL_Rect pilka;
        int predkoscX;
        int predkoscY;

        Pilka(int x, int y, int predX, int predY) {
            pilka = {x, y, WIELKOSC_PILKI, WIELKOSC_PILKI};
            predkoscX = predX;
            predkoscY = predY;
        }

        void move() {
            pilka.x += predkoscX;
            pilka.y += predkoscY;

            if (pilka.y < 0 || pilka.y > WYSOKOSC_EKRANU - WIELKOSC_PILKI) {
                predkoscY = -predkoscY;
            }
        }

        void reset(int x, int y, int predX, int predY) {
            pilka.x = x;
            pilka.y = y;
            predkoscX = predX;
            predkoscY = predY;
        }
    };

    Paletka paletka(SZEROKOSC_EKRANU / 2 - SZEROKOSC_PALETKI / 2, WYSOKOSC_EKRANU - WYSOKOSC_PALETKI - 10);
    Pilka pilka(SZEROKOSC_EKRANU / 2 - WIELKOSC_PILKI / 2, WYSOKOSC_EKRANU / 2 - WIELKOSC_PILKI / 2, 1, 1);

    bool koniecGry = false;
    SDL_Event event;
    while (!koniecGry) {
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                koniecGry = true;
            }
        }

        pilka.move();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &paletka.prostokat);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &pilka.pilka);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(okno);
    SDL_Quit();
    return 0;
}
