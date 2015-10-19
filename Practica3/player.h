#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

class Player{
    public:
      Player(int jug);
      Environment::ActionType Think();
      void Perceive(const Environment &env);
    private:
      int jugador_;
      Environment actual_;
};
#endif

/*Funcion valoracion es la función heuristica que se va a usar en el juego se encarga dar valor a un estado que en este caso es
cada una de las situaciones en el tablero*/
double Valoracion(const Environment &estado, int jugador);
/*Funcion que devuelve el numero de filas abiertas con al menos una pieza de jugador pasado por argumento*/
double filas_abiertas(const Environment &estado, int jugador);
/*Funcion que devuelve el numero de columnas abiertas con al menos una pieza de jugador pasado por argumento*/
double columnas_abiertas(const Environment &estado, int jugador);
/*Funcion que devuelve el numero de diagonales abiertas con al menos una pieza de jugador pasado por argumento*/
double diagonales_abiertas(const Environment &estado, int jugador);
/*Funcion para evaluar la bomba que fue deshechada*/
double evalua_bomba(const Environment &estado, int jugador);
/*Funcion que devuelve el numero de filas de 3 piezas del jugador dado por argumento*/
double nfilas_3(const Environment &estado, int jugador);
/*Funcion que devuelve el numero de columnas de 3 piezas del jugador dado por argumento*/
double ncolumnas_3(const Environment &estado, int jugador);
/*Funcion que devuelve el numero de diagonales de 3 piezas del jugador dado por argumento*/
double ndiagonales_3(const Environment &estado, int jugador);
/*Funcion minimax con poda alfa beta que se pedia implementar en el guion*/
double Poda_AlfaBeta(Environment &actual, int jugador, int profundidad, int PROFUNDIDAD_ALFABETA, Environment::ActionType &accion, double alpha, double beta);

