#!/bin/bash

cd /home/jeropc/tp2_algo2_menu/TP2-ENUNCIADO

echo "=== PRUEBA PARTIDA COMPLETA ==="
echo "Con semilla 99, vamos a encontrar todos los pares"
echo ""

# Jugar con semilla 99
# Cada línea después de la semilla representa una carta seleccionada
# Necesitamos 18 selecciones (2 por cada turno hasta completar 9 pares)
./tp2 ejemplos/largo.csv << EOF
S
99
0
11
1
16
2
6
3
12
4
17
5
14
7
13
8
10
9
15
Q
EOF
