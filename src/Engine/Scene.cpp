#include "Engine/Scene.h"
#include <fstream>
#include <iomanip>
#include <sstream>

namespace CNashe {
int Escena::crearObjeto(const std::string& nombre) {
    Objeto o;
    o.nombre = nombre.empty() ? "Objeto" : nombre;
    o.x = 45.0f + static_cast<float>((objetos.size() % 6) * 58);
    o.y = 45.0f + static_cast<float>((objetos.size() % 5) * 55);
    objetos.push_back(o);
    return static_cast<int>(objetos.size()) - 1;
}
int Escena::buscar(const std::string& nombre) const {
    for (size_t i = 0; i < objetos.size(); ++i)
        if (objetos[i].nombre == nombre) return static_cast<int>(i);
    return -1;
}
bool Escena::guardar(const std::string& ruta) const {
    std::ofstream f(ruta);
    if (!f) return false;
    f << "CNASHE_SCENE 1\n";
    for (const auto& o : objetos) {
        f << std::quoted(o.nombre) << ' ' << o.x << ' ' << o.y << ' '
          << o.ancho << ' ' << o.alto << ' ' << int(o.r) << ' ' << int(o.g) << ' '
          << int(o.b) << ' ' << std::quoted(o.spritePath) << ' '
          << o.z << ' ' << o.rotX << ' ' << o.rotY << ' ' << o.rotZ << ' '
          << o.scaleX << ' ' << o.scaleY << ' ' << o.scaleZ << ' '
          << o.frameWidth << ' ' << o.frameHeight << ' ' << o.frameCount << ' '
          << o.frame << ' ' << o.frameDuration << ' ' << o.visible << '\n';
    }
    return true;
}
bool Escena::cargar(const std::string& ruta) {
    std::ifstream f(ruta);
    if (!f) return false;
    std::string magic;
    int version = 0;
    f >> magic >> version;
    if (magic != "CNASHE_SCENE" || version != 1) return false;
    std::vector<Objeto> nuevos;
    Objeto o;
    while (f >> std::quoted(o.nombre) >> o.x >> o.y >> o.ancho >> o.alto) {
        int r, g, b;
        if (!(f >> r >> g >> b >> std::quoted(o.spritePath)
              >> o.z >> o.rotX >> o.rotY >> o.rotZ >> o.scaleX >> o.scaleY >> o.scaleZ
              >> o.frameWidth >> o.frameHeight >> o.frameCount >> o.frame
              >> o.frameDuration >> o.visible)) return false;
        o.r = static_cast<unsigned char>(r);
        o.g = static_cast<unsigned char>(g);
        o.b = static_cast<unsigned char>(b);
        nuevos.push_back(o);
        o = Objeto{};
    }
    objetos = std::move(nuevos);
    return true;
}
}
