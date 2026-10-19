#include "Editor/Editor.h"
#include <SDL.h>
#include <iostream>

int main(int, char**) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0) {
        std::cerr << "SDL_Init fallo: " << SDL_GetError() << "\n";
        return 1;
    }
    SDL_Window* w = SDL_CreateWindow("CNashe Studio", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                     1200, 720, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!w) { std::cerr << SDL_GetError() << "\n"; SDL_Quit(); return 1; }
    SDL_Renderer* r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!r) r = SDL_CreateRenderer(w, -1, SDL_RENDERER_SOFTWARE);
    if (!r) { std::cerr << SDL_GetError() << "\n"; SDL_DestroyWindow(w); SDL_Quit(); return 1; }
    CNashe::Editor editor(r);
    bool running=true;
    while(running && !editor.salir) {
        SDL_Event e;
        while(SDL_PollEvent(&e)) editor.procesarEvento(e);
        editor.dibujar();
        SDL_Delay(16);
    }
    SDL_DestroyRenderer(r); SDL_DestroyWindow(w); SDL_Quit();
    return 0;
}
