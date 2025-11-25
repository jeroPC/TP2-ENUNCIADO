# TDA Menú - TP2  - JERONIMO PEREZ CORDOBA

## Descripción

Este programa demuestra el funcionamiento completo del TDA Menú implementado para el TP2.

## Funcionalidades Probadas

### 1. **Creación de Menús**
- Menú principal
- Submenús anidados

### 2. **Contextos**
- **Contexto Interno**: Datos propios de cada opción (ej: mensajes personalizados)
- **Contexto Externo**: Datos compartidos entre todas las acciones (ej: contador, nombre de usuario)

### 3. **Estilos Visuales**
- `ESTILO_MINIMALISTA`: Menú simple sin decoraciones
- `ESTILO_BORDES`: Menú con bordes usando caracteres Unicode
- `ESTILO_RETRO`: Menú estilo retro con caracteres ASCII
- `ESTILO_PERSONALIZADO` (PRECIONANDO TECLA X): Estilos creados por el usuario sin modificar menu.c
- Cambio dinámico de estilo durante la ejecución


### 4. **Acciones Implementadas (Menú Principal)**
- `C`: Cargar archivo de pokemones desde CSV
- `B`: Abrir submenú de búsqueda de pokemones
- `M`: Abrir submenú para mostrar la pokedex
- `J`: Jugar partida con semilla aleatoria
- `S`: Jugar partida con semilla personalizada
- `E`: Cambiar entre estilos predefinidos (Minimalista/Bordes/Retro)
- `P`: Aplicar estilo personalizado predefinido
- `X`: Crear tu propio estilo de forma interactiva
- `Q`: Salir del programa

### 5. **Opciones del Submenú de Búsqueda**
- `N`: Buscar pokemon por nombre (búsqueda parcial)
- `I`: Buscar pokemon por ID
- `A`: Volver al menú principal

### 6. **Opciones del Submenú de Mostrar**
- `N`: Listar todos los pokemones ordenados por nombre
- `I`: Listar todos los pokemones ordenados por ID
- `A`: Volver al menú principal

## Compilación

```bash
gcc -Wall -Wextra -std=c99 -g -o tp2 tp2.c src/menu.c -I.
```

## Ejecución

```bash
./tp2
```

## Flujo de Prueba Recomendado

1. **Al iniciar**, verás el menú principal en estilo minimalista
2. Presiona `C` para cargar el archivo de pokemones (ejemplo: `pokemones.csv`)
3. Presiona `E` para cambiar el estilo → verás el menú con bordes
4. Presiona `E` nuevamente → verás el estilo retro  
5. Presiona `E` una vez más → volverás al estilo minimalista
6. Presiona `X` para crear tu propio estilo personalizado de forma interactiva
7. Presiona `B` para entrar al submenú de búsqueda:
   - Presiona `N` para buscar un pokemon por nombre (búsqueda parcial)
   - Presiona `I` para buscar un pokemon por ID
   - Presiona `A` para volver al menú principal
8. Presiona `M` para entrar al submenú de mostrar:
   - Presiona `N` para ver todos los pokemones ordenados por nombre
   - Presiona `I` para ver todos los pokemones ordenados por ID
   - Presiona `A` para volver al menú principal
9. Presiona `J` para jugar una partida con semilla aleatoria
10. Presiona `S` para jugar una partida con semilla personalizada
11. Presiona `Q` para salir

## Características Destacadas

### ✅ Gestión de Memoria
- Todos los menús y opciones se liberan correctamente al finalizar
- Los submenús se destruyen manualmente (no automáticamente)

### ✅ Validación de Entrada
- Opciones inválidas muestran mensaje de error
- El menú se vuelve a mostrar después de cada acción

### ✅ Navegación
- Soporte para submenús anidados
- Las acciones pueden retornar `true` para continuar o `false` para salir

### ✅ Colores ANSI
- Uso de colores para mejorar la experiencia visual
- Compatible con terminales que soporten ANSI

## Estructura del Código

```
tp2.c
├── contexto_juego_t: Estructura para compartir datos del juego y estilos
├── Funciones auxiliares privadas:
│   ├── limpiar_pantalla()
│   ├── pausar()
│   ├── verificar_archivo_cargado()
│   ├── obtener_color_por_indice()
│   ├── crear_estilo_interactivo()
│   └── ...
├── Funciones de acción (callback):
│   ├── accion_cargar_archivo()
│   ├── accion_buscar_nombre()
│   ├── accion_buscar_id()
│   ├── accion_mostrar_nombre()
│   ├── accion_mostrar_id()
│   ├── accion_jugar()
│   ├── accion_jugar_semilla()
│   ├── accion_cambiar_estilo()
│   ├── accion_estilo_personalizado()
│   ├── accion_crear_estilo_interactivo()
│   ├── accion_volver()
│   └── accion_salir()
├── Funciones de visualización del juego:
│   ├── dibujar_tablero()
│   ├── mostrar_ultimas_jugadas()
│   ├── ejecutar_partida()
│   └── mostrar_pokemon()
└── main():
    ├── Inicialización del contexto del juego
    ├── Creación del menú principal
    ├── Creación de submenús (búsqueda y mostrar)
    ├── Configuración de opciones
    ├── Ejecución del menú
    └── Limpieza de memoria (juego, menús, estilos)
```
