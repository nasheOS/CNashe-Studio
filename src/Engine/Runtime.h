#pragma once
#include "Engine/Scene.h"
#include <string>
#include <unordered_map>
#include <vector>

namespace CNashe {
class Runtime {
public:
    bool ejecutarArchivo(const std::string& ruta, Escena& escena, std::string& salida);
    void actualizar(float dt);
    bool activo() const { return activo_; }
    void detener() { activo_ = false; }
private:
    bool activo_ = false;
    std::unordered_map<std::string, float> variables_;
    std::vector<std::string> lineas_;
    size_t linea_ = 0;
    float espera_ = 0;
    std::string quitarComentarios(const std::string& s) const;
};
}
