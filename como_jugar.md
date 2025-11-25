# 🎮 Cómo Jugar - Pokemon Memory

## 📋 Descripción

**Pokemon Memory** es un juego de memoria para 2 jugadores donde deben encontrar pares de cartas idénticas con pokemones. El objetivo es conseguir la mayor cantidad de pares posibles.

---

## 🚀 Inicio Rápido

### Compilar el juego

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

```
╔══════════════════════════════════════╗
║ MENÚ PRINCIPAL - POKEMON MEMORY      ║
╠══════════════════════════════════════╣
║ C) Cargar archivo                    ║
║ B) Buscar pokemon                    ║
║ M) Mostrar pokedex                   ║
║ J) Jugar (semilla aleatoria)         ║
║ S) Jugar con semilla                 ║
║ E) Cambiar estilo de menú            ║
║ X) X) Crear tu propio estilo         ║
║ Q) Salir                             ║
╚══════════════════════════════════════╝
```

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
```csv
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
```
Selecciona una opción: N

═══ BUSCAR POR NOMBRE ═══

Ingresa el nombre (o parte del nombre): ABC
                                         ↑ escribis ABC
```
**Resultado:** Muestra ABC, ABCD, ABCDE, ABCDEF, etc.

**Ver todos los pokemones:**
```
Ingresa el nombre (o parte del nombre): [presiona ENTER sin escribir nada]
```
**Resultado:** Muestra todos los 15 pokemones

### Búsqueda por ID:
```
Selecciona una opción: I

═══ BUSCAR POR ID ═══

Ingresa el ID: 5
```
**Resultado:** Muestra el pokemon con ID 5

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

Al comenzar, verás:

```
╔═══════════════════════════════════════╗
║     JUEGO DE MEMORIA POKEMON          ║
╠═══════════════════════════════════════╣
║  Jugador 1:  0 pts | Jugador 2:  0 pts ║
║  Turno: Jugador 1                      ║
╚═══════════════════════════════════════╝

╔════════════════════ TABLERO ════════════════════╗
║ [ 0][ 1][ 2][ 3][ 4][ 5] ║
║ [ 6][ 7][ 8][ 9][10][11] ║
║ [12][13][14][15][16][17] ║
╚═════════════════════════════════════════════════╝
```

### 2. Seleccionar Primera Carta

```
Selecciona una carta (0-17): 5
→ Primera carta seleccionada
```

El tablero mostrará la carta descubierta:
```
║ [ 0][ 1][ 2][ 3][ 4]Pik ║
```

### 3. Seleccionar Segunda Carta

```
Selecciona una carta (0-17): 12
```

**Si coinciden:**
```
✓ ¡Par encontrado! El Jugador 1 gana un punto
```
Las cartas quedan marcadas: `✓✓`

**Si NO coinciden:**
```
✗ No es un par. Turno del Jugador 2
```
Las cartas se voltean nuevamente

### 4. Historial de Jugadas

Durante el juego verás las últimas jugadas:
```
═══ ÚLTIMAS JUGADAS ═══
  J1: Cartas 5-12 → Pikachu ✓
  J2: Cartas 3-8 → Charmander ✗
  J1: Cartas 1-14 → Bulbasaur ✓
```

### 5. Fin de la Partida

Cuando todas las parejas sean encontradas:

```
╔═══════════════════════════════════════╗
║        ¡PARTIDA TERMINADA!            ║
╚═══════════════════════════════════════╝

PUNTUACIÓN FINAL:
  Jugador 1: 5 puntos
  Jugador 2: 4 puntos

🏆 ¡GANADOR: Jugador 1!
```

**En caso de empate:**
```
🤝 ¡EMPATE!
```

---

## 🎲 Jugar con Semilla (Tecla S)

Permite jugar con una configuración reproducible del tablero.

### ¿Para qué sirve?
- **Mismo tablero:** Usando la misma semilla, el tablero tendrá los mismos pokemones en las mismas posiciones
- **Reproducir partidas:** Útil para practicar o competir con la misma configuración
- **Pruebas:** Ideal para depurar o probar estrategias

### Uso:
1. Presiona **S** en el menú principal
2. Ingresa un número entero (ej: `12345`)
3. El tablero se generará usando esa semilla

**Ejemplo:**
```
Ingresa la semilla (número entero): 42
```

Si vuelves a jugar con semilla `42`, tendrás exactamente el mismo tablero.

---

## 🎨 Cambiar Estilo de Menú (Tecla E)

El juego tiene 3 estilos visuales:

### 1. ESTILO_BORDES (Por defecto)
```
╔══════════════════════════════════════╗
║ MENÚ PRINCIPAL - POKEMON MEMORY      ║
╠══════════════════════════════════════╣
║ C) Cargar archivo                    ║
╚══════════════════════════════════════╝
```

### 2. ESTILO_RETRO
```
+--------------------------------------+
| MENÚ PRINCIPAL - POKEMON MEMORY     |
+--------------------------------------+
| [C] Cargar archivo                  |
+--------------------------------------+
```

### 3. ESTILO_MINIMALISTA
```

** Presiona **E** para rotar entre los 3 estilos.


** Presiona **X** para crear tu propio estilo!!

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

```bash
** 1. Ejecutar el juego
./tp2 ejemplos/largo.csv

** 2. En el menú principal
✓ Archivo cargado: 15 pokemones

** Presionar M → I (para ver la lista de pokemones por ID)
** Presionar A (volver al menú principal)

** 3. Presionar J (jugar con semilla aleatoria)

** 4. Jugador 1 selecciona carta 0
Selecciona una carta (0-17): 0
→ Primera carta seleccionada

** 5. Jugador 1 selecciona carta 5
Selecciona una carta (0-17): 5
✗ No es un par. Turno del Jugador 2

** 6. Jugador 2 selecciona carta 1
Selecciona una carta (0-17): 1
→ Primera carta seleccionada

** 7. Jugador 2 selecciona carta 7
Selecciona una carta (0-17): 7
✓ ¡Par encontrado! El Jugador 2 gana un punto

** ... continuar hasta encontrar todos los pares ...


🏆 ¡GANADOR: Jugador 1 / 2!


¡Gracias por jugar! Hasta pronto.
```

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
