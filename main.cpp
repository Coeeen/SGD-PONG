#include "SDL.h"
#include <iostream>

const int SZEROKOSC_EKRANU = 1100;
const int WYSOKOSC_EKRANU = 720;
const int SZEROKOSC_PALETKI = 5;
const int WYSOKOSC_PALETKI = 100;
const int POZYCJA_PALETEK = 2;
const int WIELKOSC_PILKI = 10;
int PREDKOSC_PILKI = 2;

const int TRYB_GRY = 1;

 int WYNIK_PALETKA_PIERWSZA = 0;
 int WYNIK_PALETKA_DRUGA = 0;

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

    SDL_Surface* tloSurface = SDL_LoadBMP("C:/Users/kryst/CLionProjects/GRA/Galeria/tlo1.bmp");
    if (!tloSurface) {
        std::cerr << "Wystąpił błąd z ładowaniem obrazu tła: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }
    SDL_Texture* tloTexture = SDL_CreateTextureFromSurface(renderer, tloSurface);
    SDL_FreeSurface(tloSurface);



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

        void reset() {
            pilka.x = SZEROKOSC_EKRANU / 2 - WIELKOSC_PILKI / 2;
            pilka.y = WYSOKOSC_EKRANU / 2 - WIELKOSC_PILKI / 2;
        }
    };


    //POŁOŻENIE I TWORZENIE :)
    Paletka paletkaPierwsza(10, WYSOKOSC_EKRANU / 2 - WYSOKOSC_PALETKI);
    Paletka paletkaDruga(1090, WYSOKOSC_EKRANU / 2 - WYSOKOSC_PALETKI);
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
                        paletkaPierwsza.wysokosc = -POZYCJA_PALETEK;
                    break;
                    case SDLK_DOWN:
                        paletkaPierwsza.wysokosc = POZYCJA_PALETEK;
                    break;
                    case SDLK_w:
                        paletkaDruga.wysokosc = -POZYCJA_PALETEK;
                    break;
                    case SDLK_s:
                        paletkaDruga.wysokosc = POZYCJA_PALETEK;
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

        if (TRYB_GRY == 1) {
            if (pilka.pilka.y < paletkaDruga.prostokat.y) {
                paletkaDruga.wysokosc = -POZYCJA_PALETEK;
            } else if (pilka.pilka.y > paletkaDruga.prostokat.y) {
                paletkaDruga.wysokosc = POZYCJA_PALETEK;
            } else {
                paletkaDruga.wysokosc = 0;
            }
        }

        //KOLOROWANIE NA EKRANIE OBIEKTÓW + DETEKCJA KOLIZJI ODBICIA + WYNIK
        paletkaPierwsza.move();
        paletkaDruga.move();
        pilka.move();


        SDL_Rect pilkaKolizja = pilka.pilka;
        SDL_Rect paletkaPierwszakolizja = paletkaPierwsza.prostokat;
        SDL_Rect paletkaDrugakolizja = paletkaDruga.prostokat;

        if(SDL_HasIntersection(&pilkaKolizja, &paletkaPierwszakolizja )|| SDL_HasIntersection(&pilkaKolizja, &paletkaDrugakolizja)) {
            pilka.predkoscX = -pilka.predkoscX;

            if (pilka.predkoscX > 0) {
                pilka.predkoscX += 1;
            } else {
                pilka.predkoscX -= 1;
            }
        }

        if (pilka.pilka.x < 0 ) {
            WYNIK_PALETKA_PIERWSZA++;
            std::cout << " Wynik: " << WYNIK_PALETKA_PIERWSZA << " : " << WYNIK_PALETKA_DRUGA << std::endl;
            pilka.predkoscX = PREDKOSC_PILKI;
            pilka.reset();
        }
        if(pilka.pilka.x > SZEROKOSC_EKRANU) {
            WYNIK_PALETKA_DRUGA++;
            std::cout << " Wynik: " << WYNIK_PALETKA_PIERWSZA << " : " << WYNIK_PALETKA_DRUGA << std::endl;
            pilka.predkoscX = -PREDKOSC_PILKI;
            pilka.reset();
        }

        if(WYNIK_PALETKA_PIERWSZA == 3 || WYNIK_PALETKA_DRUGA == 3) {
            SDL_Quit();
            std::cout << "GRA ZAKONCZYLA SIE Z WYNIKIEM" << WYNIK_PALETKA_PIERWSZA << " : " << WYNIK_PALETKA_DRUGA << std::endl;
            return 0;
        }

        SDL_RenderCopy(renderer, tloTexture, NULL, NULL);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderFillRect(renderer, &paletkaPierwsza.prostokat);

        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &paletkaDruga.prostokat);

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &pilka.pilka);

        /*SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderDrawLine(renderer, SZEROKOSC_EKRANU / 2, 0, SZEROKOSC_EKRANU / 2, WYSOKOSC_EKRANU);*/

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawLine(renderer, 1, 0, 0, WYSOKOSC_EKRANU);
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderDrawLine(renderer, SZEROKOSC_EKRANU - 1, 0, SZEROKOSC_EKRANU - 1, WYSOKOSC_EKRANU);

        SDL_RenderPresent(renderer);

        SDL_Delay(5);
    }


    //WYCZYSZCZENIE
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(okno);
    SDL_Quit();
    return 0;
}
