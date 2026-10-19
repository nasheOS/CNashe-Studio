# CNashe Studio — OPEN SOURCE

Este es un punto de partida funcional en C++17 + SDL2 para Windows con MinGW/GCC.
Incluye:
- Editor oscuro con lista de objetos, vista de escena e Inspector.
- Campos X/Y editables desde el teclado.
- Guardar/cargar escenas `.cns`.
- Ejecutar un subconjunto inicial del lenguaje `.cn`.
- Rectángulos, sprites opcionales con SDL2_image, animación por recortes de spritesheet y colisiones AABB.
- Modelo de escena 3D básico en datos (posición/rotación/escala), preparado para integrar un renderizador 3D después.
- Separación inicial de módulos para adaptar el runtime a Android y PS3.

## Requisitos en Windows
Instalá MSYS2 y abrí la terminal **MSYS2 UCRT64**. Ejecutá:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-pkgconf
```

Opcional para sprites:
```bash
pacman -S --needed mingw-w64-ucrt-x86_64-SDL2_image
```

Desde la carpeta del proyecto:
```bash
cmake -S . -B build -G Ninja
cmake --build build
./build/cnashe_studio.exe
```

## Controles del editor
- Clic en `+ OBJETO`: crea un objeto.
- Clic sobre un objeto de la escena: lo selecciona.
- En Inspector, clic en X o Y y usá las teclas numéricas, Backspace y Enter.
- `GUARDAR`: guarda en `scenes/escena.cns`.
- `CARGAR`: carga `scenes/escena.cns`.
- `EJECUTAR`: abre `scripts/demo.cn` si existe; si no, usa el ejemplo integrado.
- `2D / 3D`: cambia entre escena 2D y una vista esquemática de los datos 3D.

## Lenguaje `.cn` inicial
Ver `scripts/demo.cn`. Instrucciones soportadas:
```
ventana 800 600 "Mi juego"
variable x = 80
variable y = 100
rectangulo x y 50 50 azul
esperar 1
```
También se soporta:
- `mover nombre dx dy` (si existe un objeto con ese nombre)
- `crear nombre x y ancho alto`
- `color nombre rojo|verde|azul|amarillo|blanco`
- `si_colision A B`
- `fin`

El intérprete es deliberadamente pequeño y no es todavía un compilador completo. El botón Ejecutar corre el script dentro de la ventana del motor. No ejecutar scripts de fuentes desconocidas.

## Estado real de las plataformas
- **Windows:** objetivo de compilación inicial.
- **Android:** se necesita configurar SDL2/NDK y crear un backend de entrada/ventana; esta carpeta incluye el punto de separación, no un APK ya compilado.
- **PS3:** requiere toolchain/SDK de PS3 y un backend de gráficos/entrada específico. No produce todavía un ELF/PKG de PS3.
- **3D:** la escena guarda transformaciones 3D, pero el render actual es una previsualización esquemática 2D; todavía no es un motor 3D con cámara y GPU.
