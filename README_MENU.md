# TDA Menú - TP2  - JERONIMO PEREZ CORDOBA

## Descripción

Este programa demuestra el funcionamiento completo del TDA Menú implementado para el TP2.

## Funcionalidades Probadas

### 1. **Creación de Menús**
- Menú principal
- Submenús anidados

### 2. **Gestión de Opciones**
- Agregar opciones con acciones
- Agregar opciones que abren submenús
- Reemplazar opciones existentes (por tecla)

### 3. **Contextos**
- **Contexto Interno**: Datos propios de cada opción (ej: mensajes personalizados)
- **Contexto Externo**: Datos compartidos entre todas las acciones (ej: contador, nombre de usuario)

### 4. **Estilos Visuales**
- `ESTILO_MINIMALISTA`: Menú simple sin decoraciones
- `ESTILO_BORDES`: Menú con bordes usando caracteres Unicode
- `ESTILO_RETRO`: Menú estilo retro con caracteres ASCII
- `ESTILO_PERSONALIZADO` (PRECIONANDO TECLA X): Estilos creados por el usuario sin modificar menu.c
- Cambio dinámico de estilo durante la ejecución


### 5. **Acciones Implementadas**
- `S`: Saludar (usa contexto externo para mostrar el nombre)
- `O`: Ir al submenú de opciones (navegación a submenú)
- `E`: Cambiar estilo del menú (cambia entre los 3 estilos)
- `1`: Mensaje personalizado 1 (usa contexto interno)
- `2`: Mensaje personalizado 2 (usa contexto interno)
- `Q`: Salir del programa

### 6. **Opciones del Submenú**
- `I`: Incrementar contador (modifica contexto externo)
- `M`: Mostrar contador (lee contexto externo)
- `N`: Ingresar nombre (modifica contexto externo)
- `V`: Volver al menú principal (retorna false para salir del submenú)

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
2. Presiona `E` para cambiar el estilo → verás el menú con bordes
3. Presiona `E` nuevamente → verás el estilo retro
4. Presiona `E` una vez más → volverás al estilo minimalista
5. Presiona `O` para entrar al submenú
6. En el submenú, presiona `I` varias veces para incrementar el contador
7. Presiona `M` para ver el valor del contador
8. Presiona `N` para ingresar tu nombre
9. Presiona `V` para volver al menú principal
10. Presiona `S` para ver el saludo con tu nombre
11. Presiona `1` o `2` para ver mensajes con contexto interno
12. Presiona `Q` para salir

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
├── contexto_t: Estructura para compartir datos
├── Funciones de acción (callback):
│   ├── accion_saludar()
│   ├── accion_mensaje_personalizado()
│   ├── accion_incrementar()
│   ├── accion_mostrar_contador()
│   ├── accion_cambiar_estilo()
│   ├── accion_pedir_nombre()
│   ├── accion_volver()
│   └── accion_salir()
└── main():
    ├── Inicialización del contexto
    ├── Creación del menú principal
    ├── Creación del submenú
    ├── Configuración de opciones
    ├── Ejecución del menú
    └── Limpieza de memoria
```
