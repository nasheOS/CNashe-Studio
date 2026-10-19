# Adaptadores por plataforma

## Windows
Objetivo inicial SDL2 + MinGW.

## Android
Crear un build con Android NDK y SDL2 para Android, usar entrada táctil y almacenamiento accesible. El runtime del lenguaje debe permanecer independiente de la ventana.

## PS3
Crear un backend separado con las funciones gráficas/entrada que soporte el SDK/toolchain instalado. No reutilizar binarios Windows ni su SDL2 directamente. El CMake actual no compila PS3 por sí solo.
