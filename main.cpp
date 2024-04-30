#include "SDL.h"
#include <iostream>

const int SZEROKOSC_EKRANU = 800;
const int WYSOKOSC_EKRANU = 600;
const int SZEROKOSC_PALETKI = 20;
const int WYSOKOSC_PALETKI = 100;
const int WIELKOSC_PILKI = 5;
const int PREDKOSC_PILKI = 1;
const int PREDKOSC_PALETEK = 1;

int main(int argc, char* argv[]) {

    //PODŁĄCZENIE MODUŁÓW
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


    //TWORZENIE OBIEKTÓW
    struct Paletka {
        SDL_Rect prostokat;
        int wysokosc;

        Paletka(int x, int y) {
            prostokat = {x, y, SZEROKOSC_PALETKI, WYSOKOSC_PALETKI};
            wysokosc = 0;
        }

        void move() {
            prostokat.y += wysokosc;

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
        int wysokoscX;
        int wysokoscY;

        Pilka(int x, int y, int predX, int predY) {
            pilka = {x, y, WIELKOSC_PILKI, WIELKOSC_PILKI};
            wysokoscX = predX;
            wysokoscY = predY;
        }

        void move() {
            pilka.x += wysokoscX;
            pilka.y += wysokoscY;

            if (pilka.y < 0 || pilka.y > WYSOKOSC_EKRANU - WIELKOSC_PILKI) {
                wysokoscY = -wysokoscY;
            }
        }

        void reset() {
            pilka.x = SZEROKOSC_EKRANU / 2 - WIELKOSC_PILKI / 2;
            pilka.y = WYSOKOSC_EKRANU / 2 - WIELKOSC_PILKI / 2;
            wysokoscX = PREDKOSC_PILKI;
            wysokoscY = PREDKOSC_PILKI;
        }
    };


    //POŁOŻENIE I TWORZENIE :)

    Paletka paletkaPierwsza(20, WYSOKOSC_EKRANU / 2 - WYSOKOSC_PALETKI);
    Paletka paletkaDruga(780, WYSOKOSC_EKRANU / 2 - WYSOKOSC_PALETKI);
    Pilka pilka(SZEROKOSC_EKRANU / 2 - WIELKOSC_PILKI / 2, WYSOKOSC_EKRANU / 2 - WIELKOSC_PILKI / 2, PREDKOSC_PILKI, PREDKOSC_PILKI);

    bool koniecGry = false;
    while (!koniecGry) {

        //KLAWISZE EVENTY
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0) {
            if (event.type == SDL_QUIT) {
                koniecGry = true;
            }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        paletkaPierwsza.wysokosc = -PREDKOSC_PALETEK;
                    break;
                    case SDLK_DOWN:
                        paletkaPierwsza.wysokosc = PREDKOSC_PALETEK;
                    break;
                    case SDLK_w:
                        paletkaDruga.wysokosc = -PREDKOSC_PALETEK;
                    break;
                    case SDLK_s:
                        paletkaDruga.wysokosc = PREDKOSC_PALETEK;
                    break;
                }
            }

            //remis
            else if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                    case SDLK_DOWN:
                        paletkaPierwsza.wysokosc = 0;
                    break;
                    case SDLK_w:
                    case SDLK_s:
                        paletkaDruga.wysokosc = 0;
                    break;
                }
            }
        }


        //KOLOROWANIE NA EKRANIE OBIEKTÓW + DETEKCJA KOLIZJI ODBICIA
        paletkaPierwsza.move();
        paletkaDruga.move();
        pilka.move();


        SDL_Rect pilkaKolizja = pilka.pilka;
        SDL_Rect paletkaPierwszakolizja = paletkaPierwsza.prostokat;
        SDL_Rect paletkaDrugakolizja = paletkaDruga.prostokat;

        if(SDL_HasIntersection(&pilkaKolizja, &paletkaPierwszakolizja )|| SDL_HasIntersection(&pilkaKolizja, &paletkaDrugakolizja)) {
            pilka.wysokoscX = -pilka.wysokoscX;
        }


        if (pilka.pilka.x < 0 || pilka.pilka.x > SZEROKOSC_EKRANU) {
            pilka.reset();
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &paletkaPierwsza.prostokat);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &paletkaDruga.prostokat);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &pilka.pilka);

        SDL_RenderPresent(renderer);

        SDL_Delay(5);
    }


    //WYCZYSZCZENIE
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(okno);
    SDL_Quit();
    return 0;
}
