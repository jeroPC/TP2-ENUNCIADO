#!/bin/bash

# Script para probar el juego manualmente
cd /home/jeropc/tp2_algo2_menu/TP2-ENUNCIADO

echo "=== PRUEBA DEL JUEGO ==="
echo ""
echo "Vamos a:"
echo "1. Cargar el archivo"
echo "2. Jugar una partida con semilla 42"
echo "3. Hacer algunas jugadas"
echo ""

# Simular una partida completa
# S = Jugar con semilla
# 42 = semilla
# Luego 18 turnos (9 pares), cada uno con 2 cartas
./tp2 ejemplos/largo.csv << EOF
S
42
0
1
2
3
4
5
6
7
8
9
10
11
12
13
14
15
16
17
Q
EOF
