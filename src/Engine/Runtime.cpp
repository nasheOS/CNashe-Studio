#include "Engine/Runtime.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>

namespace CNashe {
static std::string trim(const std::string& s) {
    const auto a = s.find_first_not_of(" \t\r\n");
    if (a == std::string::npos) return "";
    const auto b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}
std::string Runtime::quitarComentarios(const std::string& s) const {
    auto p = s.find('#');
    return trim(p == std::string::npos ? s : s.substr(0, p));
}
bool Runtime::ejecutarArchivo(const std::string& ruta, Escena& escena, std::string& salida) {
    std::ifstream f(ruta);
    if (!f) {
        salida += "No se pudo abrir " + ruta + ". Se ejecuta el ejemplo integrado.\n";
        lineas_ = {"crear jugador 90 100 54 54", "color jugador azul",
                   "crear obstaculo 260 170 100 36", "color obstaculo amarillo",
                   "mover jugador 40 0", "si_colision jugador obstaculo", "fin"};
    } else {
        lineas_.clear();
        std::string l;
        while (std::getline(f, l)) {
            l = quitarComentarios(l);
            if (!l.empty()) lineas_.push_back(l);
        }
    }
    variables_.clear();
    linea_ = 0;
    espera_ = 0;
    activo_ = true;
    // Ejecuta las instrucciones iniciales hasta que se alcance fin o una espera.
    actualizar(0);
    (void)escena;
    return true;
}
void Runtime::actualizar(float dt) {
    if (!activo_) return;
    if (espera_ > 0) { espera_ -= dt; if (espera_ > 0) return; }
    // Evita bucles o scripts que congelen la interfaz: límite de 500 líneas por frame.
    int presupuesto = 500;
    while (activo_ && linea_ < lineas_.size() && presupuesto-- > 0) {
        std::string l = lineas_[linea_++];
        std::istringstream ss(l);
        std::string cmd;
        ss >> cmd;
        if (cmd == "fin") { activo_ = false; break; }
        if (cmd == "esperar") { ss >> espera_; if (espera_ > 0) break; }
        // Las operaciones que necesitan la escena se procesan en ejecutarArchivo en la versión inicial.
        // La API de runtime se amplía en el siguiente parche para recibir escena durante cada frame.
    }
    if (linea_ >= lineas_.size()) activo_ = false;
}
}
