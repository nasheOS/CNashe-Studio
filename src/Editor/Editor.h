#pragma once
#include "Engine/Scene.h"
#include "Engine/Runtime.h"
#include <SDL.h>
#include <string>

namespace CNashe {
class Editor {
public:
    explicit Editor(SDL_Renderer* renderer);
    void procesarEvento(const SDL_Event& e);
    void dibujar();
    bool salir = false;
private:
    SDL_Renderer* r_;
    Escena escena_;
    Runtime runtime_;
    int seleccionado_ = -1;
    int campoActivo_ = 0; // 0 ninguno, 1 X, 2 Y
    std::string buffer_;
    bool modo3D_ = false;
    std::string estado_ = "Listo";
    void rect(int x, int y, int w, int h, SDL_Color c);
    void texto(int x, int y, const std::string& s, SDL_Color c, int escala=1);
    void boton(int x, int y, int w, int h, const std::string& s, bool activo=false);
    void campo(int x, int y, const std::string& etiqueta, float valor, bool activo);
    void guardar();
    void cargar();
};
}
