# 🎮 Cómo Jugar - Pokemon Memory

## 📋 Descripción

**Pokemon Memory** es un juego de memoria para 2 jugadores donde deben encontrar pares de cartas idénticas con pokemones. El objetivo es conseguir la mayor cantidad de pares posibles.

---

## 🚀 Inicio Rápido

### Compilar el juego

Ejecuta en la terminal:

```bash
cd /home/jeropc/tp2_algo2_menu/TP2-ENUNCIADO
gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0 -g -o tp2 tp2.c src/*.c -lm
```

### Ejecutar el juego

**Opción 1: Sin archivo precargado**

```bash
./tp2
```

**Opción 2: Con archivo precargado** 

```bash
./tp2 ejemplos/normal.csv
```

o

```bash
./tp2 ejemplos/largo.csv
```

---

## 📖 Menú Principal

Al iniciar, verás el **MENÚ PRINCIPAL** con las siguientes opciones:

- **C)** Cargar archivo
- **B)** Buscar pokemon
- **M)** Mostrar pokedex
- **J)** Jugar (semilla aleatoria)
- **S)** Jugar con semilla
- **E)** Cambiar estilo de menú
- **X)** Crear tu propio estilo
- **Q)** Salir

### ⌨️ Opciones del Menú

| Tecla | Acción | Descripción |
|-------|--------|-------------|
| **C** | Cargar archivo | Carga un archivo CSV con pokemones |
| **B** | Buscar pokemon | Abre submenú para buscar por nombre o ID |
| **M** | Mostrar pokedex | Abre submenú para listar todos los pokemones |
| **J** | Jugar | Inicia una partida con pokemones aleatorios |
| **S** | Jugar con semilla | Inicia una partida con semilla específica (reproducible) |
| **E** | Cambiar estilo | Cambia entre 3 estilos visuales del menú |
| **Q** | Salir | Cierra el programa |

---

## 📂 Cargar Archivo (Tecla C)

Antes de jugar, necesitas cargar pokemones desde un archivo CSV.

### Formato del archivo CSV:

```
ID,Nombre,Tipo,Ataque,Defensa,Velocidad
```

### Ejemplo:

```
1,Pikachu,ELEC,55,40,90
2,Charmander,FUEG,52,43,65
3,Bulbasaur,PLAN,49,49,45
```

### Tipos válidos:
- `ELEC` - Eléctrico
- `FUEG` - Fuego
- `PLAN` - Planta
- `AGUA` - Agua
- `FANT` - Fantasma
- `NORM` - Normal

### Pasos:
1. Presiona **C** en el menú principal
2. Ingresa la ruta del archivo (ej: `ejemplos/normal.csv`)
3. Presiona **ENTER**

**✓ Éxito:** Verás el mensaje "Archivo cargado exitosamente: X pokemones"  
**✗ Error:** Si falla, verifica que el archivo exista y tenga el formato correcto

---

## 🔍 Buscar Pokemon (Tecla B)

Permite buscar pokemones específicos en la Pokedex cargada.

### Submenú de Búsqueda:
- **N** - Buscar por nombre (búsqueda parcial)
- **I** - Buscar por ID (búsqueda exacta)
- **A** - Volver al menú anterior

### Búsqueda por Nombre:

1. Presiona **N** en el submenú
2. Verás el prompt: `Ingresa el nombre (o parte del nombre):`
3. escribí el nombre o parte del nombre que buscas
4. Presiona **ENTER**

### Ejemplos de búsqueda:

**Buscar pokemones con "ABC":**

1. Selecciona la opción **N**
2. Verás: `Ingresa el nombre (o parte del nombre):`
3. Escribe: `ABC`
4. Presiona ENTER

**Resultado:** Muestra todos los pokemones que contengan "ABC" (ABC, ABCD, ABCDE, ABCDEF, etc.)

**Ver todos los pokemones:**

- Simplemente presiona ENTER sin escribir nada cuando te pida el nombre

**Resultado:** Muestra todos los 15 pokemones

### Búsqueda por ID:

1. Selecciona la opción **I**
2. Verás: `Ingresa el ID:`
3. Escribe el número del ID (ejemplo: `5`)
4. Presiona ENTER

**Resultado:** Muestra el pokemon con ese ID específico

---

## 📜 Mostrar Pokedex (Tecla M)

Lista todos los pokemones cargados.

### Submenú de Mostrar:
- **N** - Mostrar ordenado por nombre (alfabético)
- **I** - Mostrar ordenado por ID (numérico)
- **A** - Volver al menú anterior

### Formato de salida:

```
[ID:   1] Pikachu         | Tipo: ELEC | ATK:  55 | DEF:  40 | VEL:  90
[ID:   2] Charmander      | Tipo: FUEG | ATK:  52 | DEF:  43 | VEL:  65
```

---

## 🎲 Jugar (Tecla J)

Inicia una partida con pokemones seleccionados aleatoriamente.

### Requisitos:
- ✓ Debe haber un archivo cargado
- ✓ Se necesitan **al menos 9 pokemones** en la Pokedex

### Reglas del juego:
1. El tablero tiene **18 cartas** (9 pares de pokemones)
2. Las cartas están numeradas del **0 al 17**
3. Dos jugadores se turnan para seleccionar cartas
4. Cada turno, el jugador selecciona **2 cartas**
5. Si las cartas coinciden (mismo pokemon), el jugador **gana 1 punto** y juega de nuevo
6. Si no coinciden, las cartas se voltean y pasa el turno al otro jugador
7. El juego termina cuando todas las parejas son encontradas
8. **Gana** el jugador con más puntos

---

## 🎯 Cómo Jugar una Partida

### 1. Tablero Inicial

Al comenzar, verás el estado del juego con:

- **Puntuación:** Jugador 1: 0 pts | Jugador 2: 0 pts
- **Turno actual:** Jugador 1
- **Tablero:** 18 cartas numeradas del 0 al 17, organizadas en 3 filas
  - Fila 1: cartas 0-5
  - Fila 2: cartas 6-11
  - Fila 3: cartas 12-17

### 2. Seleccionar Primera Carta

Cuando sea tu turno:

1. Verás el mensaje: `Selecciona una carta (0-17):`
2. Escribe un número (ejemplo: `5`)
3. Presiona ENTER
4. Verás el mensaje: `Primera carta seleccionada`
5. El tablero mostrará la carta descubierta (ejemplo: si era Pikachu, verás "Pik" en esa posición)

### 3. Seleccionar Segunda Carta

1. Selecciona otra carta diferente (ejemplo: `12`)
2. Presiona ENTER

**Si las cartas coinciden (mismo pokemon):**

- Verás: `¡Par encontrado! El Jugador X gana un punto`
- Las cartas quedan marcadas permanentemente
- El mismo jugador juega otra vez

**Si NO coinciden:**

- Verás: `No es un par. Turno del Jugador X`
- Las cartas se voltean de nuevo
- Pasa el turno al otro jugador

### 4. Historial de Jugadas

Durante el juego verás un registro de las últimas jugadas:

```
ÚLTIMAS JUGADAS:
  J1: Cartas 5-12 → Pikachu (Par encontrado)
  J2: Cartas 3-8 → Charmander (No coinciden)
  J1: Cartas 1-14 → Bulbasaur (Par encontrado)
```

### 5. Fin de la Partida

Cuando todas las parejas sean encontradas verás:

**PARTIDA TERMINADA**

**PUNTUACIÓN FINAL:**
- Jugador 1: 5 puntos
- Jugador 2: 4 puntos

**GANADOR: Jugador 1**

**En caso de empate:**
- EMPATE - Ambos jugadores tienen la misma puntuación

---

## 🎲 Jugar con Semilla (Tecla S)

Permite jugar con una configuración reproducible del tablero.

### ¿Para qué sirve?
- **Mismo tablero:** Usando la misma semilla, el tablero tendrá los mismos pokemones en las mismas posiciones
- **Reproducir partidas:** Útil para practicar o competir con la misma configuración
- **Pruebas:** Ideal para depurar o probar estrategias

### Uso:
1. Presiona **S** en el menú principal
2. Verás: `Ingresa la semilla (número entero):`
3. Escribe un número (ejemplo: `42`)
4. Presiona ENTER
5. El tablero se generará usando esa semilla

**Nota:** Si vuelves a jugar con la misma semilla (ejemplo: 42), tendrás exactamente el mismo tablero.

---

## 🎨 Cambiar Estilo de Menú (Tecla E)

El juego tiene 3 estilos visuales diferentes para el menú:

1. **ESTILO_BORDES** (por defecto) - Usa caracteres especiales de caja doble
2. **ESTILO_RETRO** - Usa caracteres simples de más y guiones
3. **ESTILO_PERSONALIZADO** - El estilo que crees con la opción X

---

### ESTILOS:

- Presiona **E** para rotar entre los 3 estilos
- Presiona **X** para crear tu propio estilo

---

## 💡 Estrategia y Consejos

### 🧠 Memoria
- **Memoriza las posiciones:** Recuerda dónde aparecieron los pokemones que viste
- **Observa al rival:** Cuando tu oponente voltea cartas, memoriza también esas posiciones

### 🎮 Controles Importantes
- **Búsqueda por nombre:** Presiona N, luego escribí el nombre cuando aparezca el prompt
- **Ver todos los pokemones:** Presiona N + ENTER sin escribir nada
- **Búsqueda parcial:** escribí solo parte del nombre (ej: "AB" encuentra ABC, ABCD, etc.)
- **Navegación:** Usa la tecla A para volver atrás en cualquier submenú
- **Estilos:** Cambia el estilo visual con E para encontrar tu preferido

---



## 💡 REQUISITOS

### ❌ "Se necesitan al menos 9 pokemones para jugar"
- El archivo debe tener mínimo 9 pokemones
- Usa `ejemplos/largo.csv` que tiene 15 pokemones (suficiente)

### ❌ "Primero debes cargar un archivo"
- Ejecuta el programa con: `./tp2 ejemplos/largo.csv`
- O presiona **C** y carga un archivo CSV antes de jugar o buscar

### ❌ "Carta inválida o ya seleccionada"
- El número debe estar entre 0 y 17
- No puedes seleccionar una carta ya emparejada (con ✓✓)
- No puedes seleccionar la misma carta dos veces en un turno

---

## 📊 Archivos de Ejemplo

El juego incluye 2 archivos de ejemplo:

### `ejemplos/normal.csv`
- **5 pokemones:** Pikachu, Charmander, Bulbasaur, Squirtle, Jigglypuff
- **Uso:** Pruebas básicas (NO suficiente para jugar - necesita 9)

### `ejemplos/largo.csv` 
- **15 pokemones** con nombres variados (A, AB, ABC, ABCD, etc.)
- **Uso:** Partidas completas del juego (suficiente para jugar)

---

## 🎮 Flujo de Juego Completo - Ejemplo

### Sesión de juego paso a paso:

**1. Ejecutar el juego**
```bash
./tp2 ejemplos/largo.csv
```

**2. En el menú principal**
- Verás: `Archivo cargado: 15 pokemones`
- Presiona **M** (Mostrar pokedex)
- Presiona **I** (ver por ID)
- Presiona **A** (volver al menú)

**3. Iniciar partida**
- Presiona **J** (jugar con semilla aleatoria)

**4. Turno Jugador 1 - Primera jugada**
- Escribe: `0` (selecciona carta 0)
- Verás: `Primera carta seleccionada`
- Escribe: `5` (selecciona carta 5)
- Resultado: `No es un par. Turno del Jugador 2`

**5. Turno Jugador 2 - Primera jugada**
- Escribe: `1` (selecciona carta 1)
- Verás: `Primera carta seleccionada`
- Escribe: `7` (selecciona carta 7)
- Resultado: `¡Par encontrado! El Jugador 2 gana un punto`

**6. Continuar jugando**
- Los jugadores se turnan hasta encontrar todos los pares

**7. Fin del juego**
- Verás la puntuación final
- Se anuncia el ganador
- Mensaje: `¡Gracias por jugar! Hasta pronto.`

---

## 📝 Resumen de Teclas

| Contexto | Tecla | Acción |
|----------|-------|--------|
| **Menú Principal** | C | Cargar archivo |
| | B | Buscar pokemon |
| | M | Mostrar pokedex |
| | J | Jugar (aleatorio) |
| | S | Jugar con semilla |
| | E | Cambiar estilo |
| | Q | Salir |
| **Submenús Buscar/Mostrar** | N | Por nombre |
| | I | Por ID |
| | A | Volver atrás |
| **Durante Partida** | 0-17 | Número de carta |


---
