#pragma once
#include <string>
#include <vector>

namespace CNashe {
struct Objeto {
    std::string nombre = "Objeto";
    float x = 80, y = 80, ancho = 48, alto = 48;
    unsigned char r = 65, g = 145, b = 245;
    std::string spritePath;
    int frameWidth = 0, frameHeight = 0, frameCount = 1, frame = 0;
    float frameDuration = 0.12f, frameTimer = 0.0f;
    bool visible = true;
    // Transformaciones preparadas para futuras escenas 3D.
    float z = 0, rotX = 0, rotY = 0, rotZ = 0, scaleX = 1, scaleY = 1, scaleZ = 1;
};
class Escena {
public:
    std::vector<Objeto> objetos;
    int crearObjeto(const std::string& nombre = "Objeto");
    int buscar(const std::string& nombre) const;
    bool guardar(const std::string& ruta) const;
    bool cargar(const std::string& ruta);
};
}
