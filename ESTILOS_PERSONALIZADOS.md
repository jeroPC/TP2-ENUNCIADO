# 🎨 Estilos Personalizados del Menú

## ✅ Requisito Cumplido

El TDA Menú permite que el usuario **cree estilos personalizados externamente sin modificar el código del menú** (`menu.c`).

## 📝 Descripción

El sistema de estilos personalizados permite al usuario definir completamente la apariencia visual del menú desde su propio código, sin necesidad de tocar la implementación interna del TDA.

## 🔧 Cómo Usar

### 1. Crear un Estilo Personalizado

```c
#include "src/menu.h"
#include "src/ansi.h"

// Crear un estilo personalizado
estilo_personalizado_t *mi_estilo = estilo_crear(
    "╭━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╮",  // borde superior
    "╰━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━╯",  // borde inferior
    "├────────────────────────────────────┤",   // separador
    "  ⚡ ",                                      // prefijo de opción
    ANSI_COLOR_BOLD ANSI_COLOR_CYAN,            // color del título
    ANSI_COLOR_WHITE,                           // color de opciones
    ANSI_COLOR_YELLOW ANSI_COLOR_BOLD          // color de teclas
);
```

### 2. Aplicar el Estilo a un Menú

```c
// Crear el menú
menu_t *menu = menu_crear("MI MENÚ", ESTILO_BORDES);

// Aplicar el estilo personalizado
menu_establecer_estilo_personalizado(menu, mi_estilo);

// Mostrar el menú
menu_mostrar(menu);
```

### 3. Liberar Recursos

```c
// Al finalizar, destruir el estilo
estilo_destruir(mi_estilo);
menu_destruir(menu);
```

## 🎯 Ejemplo Completo

En el archivo `ejemplo_estilo_personalizado.c` encontrarás un ejemplo completo funcional.

**Compilar:**
```bash
gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0 -g \
    -o ejemplo_estilo ejemplo_estilo_personalizado.c src/menu.c -lm
```

**Ejecutar:**
```bash
./ejemplo_estilo
```

## 🎮 Uso en el Juego Principal

El juego principal (`tp2`) demuestra cómo se pueden crear estilos personalizados externamente:

1. **Ejecutar el juego:**
   ```bash
   ./tp2 ejemplos/largo.csv
   ```

2. **Presionar la tecla `P`** en el menú principal para aplicar el estilo personalizado

3. El estilo se aplica instantáneamente, demostrando que:
   - ✅ Se creó externamente (en `main()` de `tp2.c`)
   - ✅ No se modificó `menu.c`
   - ✅ Es completamente configurable

## 📋 Parámetros del Estilo

La función `estilo_crear()` acepta 7 parámetros (todos opcionales - pueden ser NULL):

| Parámetro | Descripción | Ejemplo |
|-----------|-------------|---------|
| `borde_superior` | Línea superior del menú | `"╔═══════╗"` |
| `borde_inferior` | Línea inferior del menú | `"╚═══════╝"` |
| `separador` | Línea separadora (título/opciones) | `"─────────"` |
| `prefijo_opcion` | Prefijo antes de cada opción | `"  ▸ "` |
| `color_titulo` | Código ANSI para el título | `ANSI_COLOR_CYAN` |
| `color_opciones` | Código ANSI para las opciones | `ANSI_COLOR_WHITE` |
| `color_teclas` | Código ANSI para las teclas | `ANSI_COLOR_YELLOW` |

## 🌈 Colores ANSI Disponibles

```c
ANSI_COLOR_BLACK
ANSI_COLOR_RED
ANSI_COLOR_GREEN
ANSI_COLOR_YELLOW
ANSI_COLOR_BLUE
ANSI_COLOR_MAGENTA
ANSI_COLOR_CYAN
ANSI_COLOR_WHITE
ANSI_COLOR_BOLD          // Texto en negrita
ANSI_COLOR_RESET         // Resetear formato
```

## 💡 Ejemplos de Estilos

### Estilo Minimalista
```c
estilo_personalizado_t *minimalista = estilo_crear(
    NULL,                    // Sin borde superior
    NULL,                    // Sin borde inferior
    "═══════════════════",   // Separador simple
    "• ",                    // Bullet point
    ANSI_COLOR_WHITE,        // Título blanco
    ANSI_COLOR_WHITE,        // Opciones blancas
    ANSI_COLOR_CYAN          // Teclas cyan
);
```

### Estilo Retro
```c
estilo_personalizado_t *retro = estilo_crear(
    "+======================================+",
    "+======================================+",
    "+--------------------------------------+",
    "| ",
    ANSI_COLOR_GREEN,
    ANSI_COLOR_GREEN,
    ANSI_COLOR_YELLOW
);
```

### Estilo Futurista
```c
estilo_personalizado_t *futurista = estilo_crear(
    "▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓",
    "▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓▓",
    "░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░░",
    "  ◆ ",
    ANSI_COLOR_BOLD ANSI_COLOR_MAGENTA,
    ANSI_COLOR_CYAN,
    ANSI_COLOR_YELLOW ANSI_COLOR_BOLD
);
```

## ✅ Ventajas

1. **Sin modificar menu.c**: El código del TDA Menú permanece intacto
2. **Reutilizable**: El mismo estilo puede aplicarse a múltiples menús
3. **Dinámico**: Puedes cambiar estilos en tiempo de ejecución
4. **Flexible**: Todos los parámetros son opcionales
5. **Limpio**: Separación clara entre lógica y presentación

## 📚 API Completa

```c
// Crear un estilo personalizado
estilo_personalizado_t *estilo_crear(
    const char *borde_superior,
    const char *borde_inferior,
    const char *separador,
    const char *prefijo_opcion,
    const char *color_titulo,
    const char *color_opciones,
    const char *color_teclas
);

// Destruir un estilo personalizado
void estilo_destruir(estilo_personalizado_t *estilo);

// Aplicar un estilo personalizado a un menú
bool menu_establecer_estilo_personalizado(menu_t *menu, estilo_personalizado_t *estilo);
```

---

**Autor:** Jerónimo Pérez Córdoba  
**Matrícula:** 111939  
**Materia:** Algoritmos y Programación II  
**Fecha:** Noviembre 2025
