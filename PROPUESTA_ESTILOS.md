# Propuesta: Sistema de Estilos Personalizables

## Problema actual
Los estilos están hardcodeados en el enum. El usuario NO puede crear estilos propios sin modificar el código fuente del menú.

## Solución propuesta

### 1. Definir estructura de estilo personalizable

```c
// En menu.h
typedef struct estilo_personalizado {
    char *borde_superior;
    char *borde_inferior;
    char *separador;
    char *prefijo_opcion;
    char *prefijo_seleccion;
    // Colores ANSI opcionales
    char *color_titulo;
    char *color_opciones;
    char *color_reset;
} estilo_personalizado_t;
```

### 2. Permitir registrar estilos externos

```c
// Crear estilo personalizado
estilo_personalizado_t *estilo_crear_personalizado(
    const char *borde_sup,
    const char *borde_inf,
    const char *separador,
    const char *prefijo_op,
    const char *prefijo_sel
);

// Registrar estilo en el menú
bool menu_registrar_estilo(menu_t *menu, estilo_personalizado_t *estilo, int id);

// Aplicar estilo por ID
void menu_aplicar_estilo(menu_t *menu, int estilo_id);
```

### 3. Uso desde el código del usuario (sin modificar menu.c)

```c
// El usuario crea su propio estilo
estilo_personalizado_t *mi_estilo = estilo_crear_personalizado(
    "╔═══════════════════════════╗",
    "╚═══════════════════════════╝",
    "─────────────────────────────",
    "  ▸ ",
    "  ✓ "
);

menu_t *menu = menu_crear("Mi Menú", ESTILO_MINIMALISTA);
menu_registrar_estilo(menu, mi_estilo, 100); // ID personalizado
menu_aplicar_estilo(menu, 100);
```

## Ventajas
✅ El usuario puede crear estilos sin modificar menu.c
✅ Compatible con estilos predefinidos
✅ Extensible con nuevos atributos (colores, iconos, etc.)
✅ Cumple con el requisito del enunciado

## Implementación mínima
Agregar estas funciones en menu.h y menu.c para permitir:
1. Crear estructuras de estilo personalizadas
2. Registrar estilos en el menú
3. Aplicar estilos por ID o puntero
