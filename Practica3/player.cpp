#include <iostream>
#include <cstdlib>
#include <cstddef>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"

using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;
int terminales = 0, pierde=0;

// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){
    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}

// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
          return Puntuacion(jugador,estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar

double filas_abiertas(const Environment &estado, int jugador){
    double suma=0;
    int casilla_bomba,rival, casilla_bomba_rival;
    if(jugador==1){
        casilla_bomba=4;
        casilla_bomba_rival=5;
        rival = 2;
    }
    else{
        casilla_bomba=5;
        casilla_bomba_rival=4;
        rival = 1;
    }
    for (int i=0; i<7; i++){
        for (int j=0; j<7; j++){
            if((int)estado.See_Casilla(i,j)==jugador || (int)estado.See_Casilla(i,j)==casilla_bomba){
                if(j<4)
                    if(((int)estado.See_Casilla(i,j+1)==0 || (int)estado.See_Casilla(i,j+1)==casilla_bomba || (int)estado.See_Casilla(i,j+1)==jugador)
                       && ((int)estado.See_Casilla(i,j+2)==0 || (int)estado.See_Casilla(i,j+2)==casilla_bomba || (int)estado.See_Casilla(i,j+2)==jugador)
                       && ((int)estado.See_Casilla(i,j+3)==0 || (int)estado.See_Casilla(i,j+3)==casilla_bomba || (int)estado.See_Casilla(i,j+3)==jugador)){
                        suma++;
                    }
                if(j>=3)
                    if(((int)estado.See_Casilla(i,j-1)==0 || (int)estado.See_Casilla(i,j-1)==casilla_bomba || (int)estado.See_Casilla(i,j-1)==jugador)
                       && ((int)estado.See_Casilla(i,j-2)==0 || (int)estado.See_Casilla(i,j-2)==casilla_bomba || (int)estado.See_Casilla(i,j-2)==jugador)
                       && ((int)estado.See_Casilla(i,j-3)==0 || (int)estado.See_Casilla(i,j-3)==casilla_bomba || (int)estado.See_Casilla(i,j-3)==jugador)){
                        suma++;
                    }
                if(1==j || j==5){
                    if(j == 1){
                        if(((int)estado.See_Casilla(i,j+1)==0 || (int)estado.See_Casilla(i,j+1)==casilla_bomba || (int)estado.See_Casilla(i,j+1)==jugador)
                           && ((int)estado.See_Casilla(i,j+2)==0 || (int)estado.See_Casilla(i,j+2)==casilla_bomba || (int)estado.See_Casilla(i,j+2)==jugador)
                           && ((int)estado.See_Casilla(i,j-1)==0 || (int)estado.See_Casilla(i,j-1)==casilla_bomba || (int)estado.See_Casilla(i,j-1)==jugador)){
                            suma++;
                           }
                    }
                    if(j == 5){
                        if(((int)estado.See_Casilla(i,j-1)==0 || (int)estado.See_Casilla(i,j-1)==casilla_bomba || (int)estado.See_Casilla(i,j-1)==jugador)
                           && ((int)estado.See_Casilla(i,j-2)==0 || (int)estado.See_Casilla(i,j-2)==casilla_bomba ||(int)estado.See_Casilla(i,j-2)==jugador)
                           && ((int)estado.See_Casilla(i,j+1)==0 || (int)estado.See_Casilla(i,j+1)==casilla_bomba || (int)estado.See_Casilla(i,j+1)==jugador)){
                            suma++;
                           }
                    }
                }
                if(2<=j && j<=4){
                    if(j >= 2){
                        if(((int)estado.See_Casilla(i,j+1)==0 || (int)estado.See_Casilla(i,j+1)==casilla_bomba || (int)estado.See_Casilla(i,j+1)==jugador)
                           &&((int)estado.See_Casilla(i,j-1)==jugador || (int)estado.See_Casilla(i,j-1)==casilla_bomba ||(int)estado.See_Casilla(i,j-1)==0 )
                           &&((int)estado.See_Casilla(i,j-2)==jugador || (int)estado.See_Casilla(i,j-1)==casilla_bomba ||(int)estado.See_Casilla(i,j-2)==0 )){
                            suma++;
                        }
                    }
                    if(j <= 4){
                        if(((int)estado.See_Casilla(i,j-1)==0 || (int)estado.See_Casilla(i,j-1)==casilla_bomba || (int)estado.See_Casilla(i,j-1)==jugador)
                           && ((int)estado.See_Casilla(i,j+1)==jugador || (int)estado.See_Casilla(i,j+1)==casilla_bomba || (int)estado.See_Casilla(i,j+1)==0 )
                           &&((int)estado.See_Casilla(i,j+2)==jugador || (int)estado.See_Casilla(i,j+2)==casilla_bomba || (int)estado.See_Casilla(i,j+2)==0 )){
                            suma++;
                        }
                    }
                }
            }
        }
    }
    return suma;
}

double columnas_abiertas(const Environment &estado, int jugador){
    double suma=0;
    int casilla_bomba, casilla_bomba_rival, rival;
    if(jugador==1){
        casilla_bomba=4;
        casilla_bomba_rival=5;
        rival = 2;
    }
    else{
        casilla_bomba=5;
        casilla_bomba_rival=4;
        rival = 1;
    }
    for (int i=0; i<7; i++){
        for (int j=0; j<7; j++){
            if((int)estado.See_Casilla(i,j)==jugador || (int)estado.See_Casilla(i,j)==casilla_bomba){
                if(i<4)
                    if(((int)estado.See_Casilla(i+1,j)==0|| (int)estado.See_Casilla(i+1,j)==casilla_bomba || (int)estado.See_Casilla(i+1,j)==jugador)
                       && ((int)estado.See_Casilla(i+2,j)==0 || (int)estado.See_Casilla(i+2,j)==casilla_bomba || (int)estado.See_Casilla(i+2,j)==jugador)
                       && ((int)estado.See_Casilla(i+3,j)==0 || (int)estado.See_Casilla(i+3,j)==casilla_bomba || (int)estado.See_Casilla(i+3,j)==jugador)){
                        suma++;
                    }
                else if(i == 4)
                    if(((int)estado.See_Casilla(i+1,j)==0 || (int)estado.See_Casilla(i+1,j)==casilla_bomba || (int)estado.See_Casilla(i+1,j)==jugador)
                       && ((int)estado.See_Casilla(i+2,j)==0 || (int)estado.See_Casilla(i+2,j)==casilla_bomba || (int)estado.See_Casilla(i+2,j)==jugador)
                       && ((int)estado.See_Casilla(i-1,j)==jugador || (int)estado.See_Casilla(i-1,j)==casilla_bomba))
                        suma++;
                else if(i==5)
                    if(((int)estado.See_Casilla(i+1,j)==0 || (int)estado.See_Casilla(i+1,j)==casilla_bomba|| (int)estado.See_Casilla(i+1,j)==jugador)
                       && ((int)estado.See_Casilla(i-1,j)==jugador || (int)estado.See_Casilla(i-1,j)==casilla_bomba)
                       && ((int)estado.See_Casilla(i-2,j)==jugador || (int)estado.See_Casilla(i-2,j)==casilla_bomba))
                        suma++;
                else if(i==6)
                    if(((int)estado.See_Casilla(i-1,j)==jugador || (int)estado.See_Casilla(i-1,j)==casilla_bomba)
                       && ((int)estado.See_Casilla(i-2,j)==jugador || (int)estado.See_Casilla(i-2,j)==casilla_bomba)
                       && ((int)estado.See_Casilla(i-3,j)==jugador || (int)estado.See_Casilla(i-3,j)==casilla_bomba))
                        suma++;
                }
        }
    }
    return suma;
}

double diagonales_abiertas(const Environment &estado, int jugador){
    double suma=0;
    int casilla_bomba,rival, casilla_bomba_rival;
    if(jugador==1){
        casilla_bomba=4;
        casilla_bomba_rival=5;
        rival = 2;
    }
    else{
        casilla_bomba=5;
        casilla_bomba_rival=4;
        rival = 1;
    }
    for (int i=0; i<7; i++){
        for (int j=0; j<7; j++){
            if((int)estado.See_Casilla(i,j)==jugador || (int)estado.See_Casilla(i,j)==casilla_bomba){
                if(i<4 && j<4){
                    if(((int)estado.See_Casilla(i+1,j+1)==0 || (int)estado.See_Casilla(i+1,j+1)==casilla_bomba|| (int)estado.See_Casilla(i+1,j+1)==jugador)
                       && ((int)estado.See_Casilla(i+2,j+2)==0 || (int)estado.See_Casilla(i+2,j+2)==casilla_bomba || (int)estado.See_Casilla(i+2,j+2)==jugador)
                       && ((int)estado.See_Casilla(i+3,j+3)==0 || (int)estado.See_Casilla(i+3,j+3)==casilla_bomba || (int)estado.See_Casilla(i+3,j+3)==jugador)){
                        suma++;
                    }
                    if(i>0 && i<4 && j>0 && j<4){
                        if(((int)estado.See_Casilla(i-1,j-1)==0 || (int)estado.See_Casilla(i-1,j-1)==casilla_bomba || (int)estado.See_Casilla(i-1,j-1)==jugador)
                           && ((int)estado.See_Casilla(i+1,j+1)==0 || (int)estado.See_Casilla(i+1,j+1)==casilla_bomba || (int)estado.See_Casilla(i+1,j+1)==jugador)
                           && ((int)estado.See_Casilla(i+2,j+2)==0 || (int)estado.See_Casilla(i+2,j+2)==casilla_bomba || (int)estado.See_Casilla(i+2,j+2)==jugador)){
                            suma++;
                        }
                    }
                    if(i>1 && i<4 && j>1 && j<4){
                        if(((int)estado.See_Casilla(i-1,j-1)==0 || (int)estado.See_Casilla(i-1,j-1)==casilla_bomba || (int)estado.See_Casilla(i-1,j-1)==jugador)
                           && ((int)estado.See_Casilla(i-2,j-2)==0 || (int)estado.See_Casilla(i-2,j-2)==casilla_bomba || (int)estado.See_Casilla(i-2,j-2)==jugador)
                           && ((int)estado.See_Casilla(i+1,j+1)==0 || (int)estado.See_Casilla(i+1,j+1)==casilla_bomba || (int)estado.See_Casilla(i+1,j+1)==jugador)){
                            suma++;
                        }
                    }
                }
                if(i<4 && j>=3){
                    if(((int)estado.See_Casilla(i+1,j-1)==0 || (int)estado.See_Casilla(i+1,j-1)==casilla_bomba || (int)estado.See_Casilla(i+1,j-1)==jugador)
                       && ((int)estado.See_Casilla(i+2,j-2)==0 || (int)estado.See_Casilla(i+2,j-2)==casilla_bomba || (int)estado.See_Casilla(i+2,j-2)==jugador)
                       && ((int)estado.See_Casilla(i+3,j-3)==0 || (int)estado.See_Casilla(i+3,j-3)==casilla_bomba || (int)estado.See_Casilla(i+3,j-3)==jugador)){
                        suma++;
                    }
                    if(i>0 && i<4 && j>=3 && j<6){
                        if(((int)estado.See_Casilla(i-1,j+1)==0 || (int)estado.See_Casilla(i-1,j+1)==casilla_bomba || (int)estado.See_Casilla(i-1,j+1)==jugador)
                           && ((int)estado.See_Casilla(i+1,j-1)==0 || (int)estado.See_Casilla(i+1,j-1)==casilla_bomba || (int)estado.See_Casilla(i+1,j-1)==jugador)
                           && ((int)estado.See_Casilla(i+2,j-2)==0 || (int)estado.See_Casilla(i+2,j-2)==casilla_bomba || (int)estado.See_Casilla(i+2,j-2)==jugador)){
                            suma++;
                        }
                    }
                    if(i>1 && i<4 && j>=3 && j<5){
                        if(((int)estado.See_Casilla(i-1,j+1)==0 || (int)estado.See_Casilla(i-1,j+1)==casilla_bomba || (int)estado.See_Casilla(i-1,j+1)==jugador)
                           && ((int)estado.See_Casilla(i-2,j+2)==0 || (int)estado.See_Casilla(i-2,j+2)==casilla_bomba || (int)estado.See_Casilla(i-2,j+2)==jugador)
                           && ((int)estado.See_Casilla(i+1,j-1)==0 || (int)estado.See_Casilla(i+1,j-1)==casilla_bomba || (int)estado.See_Casilla(i+1,j-1)==jugador)){
                            suma++;
                        }
                    }
                }
                if(i>=3 && j<4){
                    if(((int)estado.See_Casilla(i-1,j+1)==0 || (int)estado.See_Casilla(i-1,j+1)==casilla_bomba || (int)estado.See_Casilla(i-1,j+1)==jugador)
                       && ((int)estado.See_Casilla(i-2,j+2)==0 || (int)estado.See_Casilla(i-2,j+2)==casilla_bomba || (int)estado.See_Casilla(i-2,j+2)==jugador)
                       && ((int)estado.See_Casilla(i-3,j+3)==0 || (int)estado.See_Casilla(i-3,j+3)==casilla_bomba || (int)estado.See_Casilla(i-3,j+3)==jugador)){
                        suma++;
                    }
                    if(i>=3 && i<6 && j>0 && j<4){
                        if(((int)estado.See_Casilla(i+1,j-1)==0 || (int)estado.See_Casilla(i+1,j-1)==casilla_bomba || (int)estado.See_Casilla(i+1,j-1)==jugador)
                           && ((int)estado.See_Casilla(i-1,j+1)==0 || (int)estado.See_Casilla(i-1,j+1)==casilla_bomba || (int)estado.See_Casilla(i-1,j+1)==jugador)
                           && ((int)estado.See_Casilla(i-2,j+2)==0 || (int)estado.See_Casilla(i-2,j+2)==casilla_bomba || (int)estado.See_Casilla(i-2,j+2)==jugador)){
                            suma++;
                        }
                    }
                    if(i>=3 && i<5 && j>1 && j<4){
                        if(((int)estado.See_Casilla(i+2,j-2)==0 || (int)estado.See_Casilla(i+2,j-2)==casilla_bomba || (int)estado.See_Casilla(i+2,j-2)==jugador)
                           && ((int)estado.See_Casilla(i+1,j-1)==0 || (int)estado.See_Casilla(i+1,j-1)==casilla_bomba || (int)estado.See_Casilla(i+1,j-1)==jugador)
                           && ((int)estado.See_Casilla(i-1,j+1)==0 || (int)estado.See_Casilla(i-1,j+1)==casilla_bomba || (int)estado.See_Casilla(i-1,j+1)==jugador)){
                            suma++;
                        }
                    }
                }
                if(i>=3 && j>=3){
                    if(((int)estado.See_Casilla(i-1,j-1)==0 || (int)estado.See_Casilla(i-1,j-1)==casilla_bomba || (int)estado.See_Casilla(i-1,j-1)==jugador)
                       && ((int)estado.See_Casilla(i-2,j-2)==0 || (int)estado.See_Casilla(i-2,j-2)==casilla_bomba || (int)estado.See_Casilla(i-2,j-2)==jugador)
                       && ((int)estado.See_Casilla(i-3,j-3)==0 || (int)estado.See_Casilla(i-3,j-3)==casilla_bomba || (int)estado.See_Casilla(i-3,j-3)==jugador)){
                        suma++;
                    }
                    if(i>=3 && i<6 && j>=3 && j<6){
                        if(((int)estado.See_Casilla(i+1,j+1)==0 || (int)estado.See_Casilla(i+1,j+1)==casilla_bomba || (int)estado.See_Casilla(i+1,j+1)==jugador)
                           && ((int)estado.See_Casilla(i-1,j-1)==0 || (int)estado.See_Casilla(i-1,j-1)==casilla_bomba || (int)estado.See_Casilla(i-1,j-1)==jugador)
                           && ((int)estado.See_Casilla(i-2,j-2)==0 || (int)estado.See_Casilla(i-2,j-2)==casilla_bomba || (int)estado.See_Casilla(i-2,j-2)==jugador)){
                            suma++;
                        }
                    }
                    if(i>=3 && i<5 && j>=3 && j<5){
                        if(((int)estado.See_Casilla(i+1,j+1)==0 || (int)estado.See_Casilla(i+1,j+1)==casilla_bomba || (int)estado.See_Casilla(i+1,j+1)==jugador)
                           && ((int)estado.See_Casilla(i+2,j+2)==0 || (int)estado.See_Casilla(i+2,j+2)==casilla_bomba || (int)estado.See_Casilla(i+2,j+2)==jugador)
                           && ((int)estado.See_Casilla(i-1,j-1)==0 || (int)estado.See_Casilla(i-1,j-1)==casilla_bomba || (int)estado.See_Casilla(i-1,j-1)==jugador)){
                            suma++;
                        }
                    }
                }
            }
        }
    }
    return suma;
}

double evalua_bomba2(const Environment &estado, int jugador){
    double valor_explotado=0, valor_actual=0;
    int boom=7,rival;
    if(jugador==1)
        rival=2;
    else
        rival=1;
    Environment estado_explotado;
    estado_explotado = estado.GenerateNextMove(boom);
    valor_actual=(filas_abiertas(estado,jugador)+columnas_abiertas(estado,jugador)+diagonales_abiertas(estado,jugador))
    -(filas_abiertas(estado,rival)+columnas_abiertas(estado,rival)+diagonales_abiertas(estado,rival));
    valor_explotado=(filas_abiertas(estado_explotado,jugador)+columnas_abiertas(estado_explotado,jugador)+diagonales_abiertas(estado_explotado,jugador))
    -(filas_abiertas(estado_explotado,rival)+columnas_abiertas(estado_explotado,rival)+diagonales_abiertas(estado_explotado,rival));

    if(valor_actual<valor_explotado)
        return valor_explotado;
    else
        return valor_actual;
}

double nfilas_3(const Environment &estado, int jugador){
    double suma=0;
    int casilla_bomba,rival, casilla_bomba_rival;
    if(jugador==1){
        casilla_bomba=4;
        casilla_bomba_rival=5;
        rival = 2;
    }
    else{
        casilla_bomba=5;
        casilla_bomba_rival=4;
        rival = 1;
    }
    for (int i=0; i<7; i++){
        for (int j=0; j<7; j++){
            if((int)estado.See_Casilla(i,j)==0){
                if(j<4)
                    if(((int)estado.See_Casilla(i,j+1)==casilla_bomba || (int)estado.See_Casilla(i,j+1)==jugador)
                       && ((int)estado.See_Casilla(i,j+2)==casilla_bomba || (int)estado.See_Casilla(i,j+2)==jugador)
                       && ((int)estado.See_Casilla(i,j+3)==casilla_bomba || (int)estado.See_Casilla(i,j+3)==jugador)){
                        suma++;
                    }
                if(j>=3)
                    if(((int)estado.See_Casilla(i,j-1)==casilla_bomba || (int)estado.See_Casilla(i,j-1)==jugador)
                       && ((int)estado.See_Casilla(i,j-2)==casilla_bomba || (int)estado.See_Casilla(i,j-2)==jugador)
                       && ((int)estado.See_Casilla(i,j-3)==casilla_bomba || (int)estado.See_Casilla(i,j-3)==jugador)){
                        suma++;
                    }
                if(1==j || j==5){
                    if(j == 1){
                        if(((int)estado.See_Casilla(i,j+1)==casilla_bomba || (int)estado.See_Casilla(i,j+1)==jugador)
                           && ((int)estado.See_Casilla(i,j+2)==casilla_bomba || (int)estado.See_Casilla(i,j+2)==jugador)
                           && ((int)estado.See_Casilla(i,j-1)==casilla_bomba || (int)estado.See_Casilla(i,j-1)==jugador)){
                            suma++;
                           }
                    }
                    if(j == 5){
                        if(((int)estado.See_Casilla(i,j-1)==casilla_bomba || (int)estado.See_Casilla(i,j-1)==jugador)
                           && ((int)estado.See_Casilla(i,j-2)==casilla_bomba ||(int)estado.See_Casilla(i,j-2)==jugador)
                           && ((int)estado.See_Casilla(i,j+1)==casilla_bomba || (int)estado.See_Casilla(i,j+1)==jugador)){
                            suma++;
                           }
                    }
                }
                if(2<=j && j<=4){
                    if(j >= 2){
                        if(((int)estado.See_Casilla(i,j+1)==casilla_bomba || (int)estado.See_Casilla(i,j+1)==jugador)
                           &&((int)estado.See_Casilla(i,j-1)==jugador || (int)estado.See_Casilla(i,j-1)==casilla_bomba)
                           &&((int)estado.See_Casilla(i,j-2)==jugador || (int)estado.See_Casilla(i,j-1)==casilla_bomba)){
                            suma++;
                        }
                    }
                    if(j <= 4){
                        if(((int)estado.See_Casilla(i,j-1)==casilla_bomba || (int)estado.See_Casilla(i,j-1)==jugador)
                           && ((int)estado.See_Casilla(i,j+1)==jugador || (int)estado.See_Casilla(i,j+1)==casilla_bomba)
                           &&((int)estado.See_Casilla(i,j+2)==jugador || (int)estado.See_Casilla(i,j+2)==casilla_bomba)){
                            suma++;
                        }
                    }
                }
            }
        }
    }
    return suma;
}

double ncolumnas_3(const Environment &estado, int jugador){
     double suma=0;
    int casilla_bomba, casilla_bomba_rival, rival;
    if(jugador==1){
        casilla_bomba=4;
        casilla_bomba_rival=5;
        rival = 2;
    }
    else{
        casilla_bomba=5;
        casilla_bomba_rival=4;
        rival = 1;
    }
    for (int i=0; i<7; i++){
        for (int j=0; j<7; j++){
            if((int)estado.See_Casilla(i,j)==0){
                if(i>=3)
                    if(((int)estado.See_Casilla(i-1,j)==casilla_bomba || (int)estado.See_Casilla(i-1,j)==jugador)
                       && ((int)estado.See_Casilla(i-2,j)==casilla_bomba || (int)estado.See_Casilla(i-2,j)==jugador)
                       && ((int)estado.See_Casilla(i-3,j)==casilla_bomba || (int)estado.See_Casilla(i-3,j)==jugador)){
                        suma++;
                    }
            }
        }
    }
    return suma;
}

double ndiagonales_3(const Environment &estado, int jugador){
    double suma=0;
    int casilla_bomba,rival, casilla_bomba_rival;
    if(jugador==1){
        casilla_bomba=4;
        casilla_bomba_rival=5;
        rival = 2;
    }
    else{
        casilla_bomba=5;
        casilla_bomba_rival=4;
        rival = 1;
    }
    for (int i=0; i<7; i++){
        for (int j=0; j<7; j++){
            if((int)estado.See_Casilla(i,j)==0){
                if(i<4 && j<4){
                    if(((int)estado.See_Casilla(i+1,j+1)==casilla_bomba|| (int)estado.See_Casilla(i+1,j+1)==jugador)
                       && ((int)estado.See_Casilla(i+2,j+2)==casilla_bomba || (int)estado.See_Casilla(i+2,j+2)==jugador)
                       && ((int)estado.See_Casilla(i+3,j+3)==casilla_bomba || (int)estado.See_Casilla(i+3,j+3)==jugador)){
                        suma++;
                       }
                    if(i>0 && i<6 && j>0 && j<6){
                        if(((int)estado.See_Casilla(i-1,j-1)==casilla_bomba || (int)estado.See_Casilla(i-1,j-1)==jugador)
                           && ((int)estado.See_Casilla(i+1,j+1)==casilla_bomba || (int)estado.See_Casilla(i+1,j+1)==jugador)
                           && ((int)estado.See_Casilla(i+2,j+2)==casilla_bomba || (int)estado.See_Casilla(i+2,j+2)==jugador)){
                            suma++;
                        }
                    }
                    if(i>1 && i<5 && j>1 && j<5){
                        if(((int)estado.See_Casilla(i-1,j-1)==casilla_bomba || (int)estado.See_Casilla(i-1,j-1)==jugador)
                           && ((int)estado.See_Casilla(i-2,j-2)==casilla_bomba || (int)estado.See_Casilla(i-2,j-2)==jugador)
                           && ((int)estado.See_Casilla(i+1,j+1)==casilla_bomba || (int)estado.See_Casilla(i+1,j+1)==jugador)){
                            suma++;
                        }
                    }
                }
                if(i<4 && j>=3){
                    if(((int)estado.See_Casilla(i+1,j-1)==casilla_bomba || (int)estado.See_Casilla(i+1,j-1)==jugador)
                       && ((int)estado.See_Casilla(i+2,j-2)==casilla_bomba || (int)estado.See_Casilla(i+2,j-2)==jugador)
                       && ((int)estado.See_Casilla(i+3,j-3)==casilla_bomba || (int)estado.See_Casilla(i+3,j-3)==jugador)){
                        suma++;
                    }
                    if(i>0 && i<6 && j>0 && j<6){
                        if(((int)estado.See_Casilla(i-1,j+1)==casilla_bomba || (int)estado.See_Casilla(i-1,j+1)==jugador)
                           && ((int)estado.See_Casilla(i+1,j-1)==casilla_bomba || (int)estado.See_Casilla(i+1,j-1)==jugador)
                           && ((int)estado.See_Casilla(i+2,j-2)==casilla_bomba || (int)estado.See_Casilla(i+2,j-2)==jugador)){
                            suma++;
                        }
                    }
                    if(i>1 && i<5 && j>1 && j<5){
                        if(((int)estado.See_Casilla(i-1,j+1)==casilla_bomba || (int)estado.See_Casilla(i-1,j+1)==jugador)
                           && ((int)estado.See_Casilla(i-2,j+2)==casilla_bomba || (int)estado.See_Casilla(i-2,j+2)==jugador)
                           && ((int)estado.See_Casilla(i+1,j-1)==casilla_bomba || (int)estado.See_Casilla(i+1,j-1)==jugador)){
                            suma++;
                        }
                    }
                }
                if(i>=3 && j<4){
                    if(((int)estado.See_Casilla(i-1,j+1)==casilla_bomba || (int)estado.See_Casilla(i-1,j+1)==jugador)
                       && ((int)estado.See_Casilla(i-2,j+2)==casilla_bomba || (int)estado.See_Casilla(i-2,j+2)==jugador)
                       && ((int)estado.See_Casilla(i-3,j+3)==casilla_bomba || (int)estado.See_Casilla(i-3,j+3)==jugador)){
                        suma++;
                       }
                    if(i>0 && i<6 && j>0 && j<6){
                        if(((int)estado.See_Casilla(i+1,j-1)==casilla_bomba || (int)estado.See_Casilla(i+1,j-1)==jugador)
                           && ((int)estado.See_Casilla(i-1,j+1)==casilla_bomba || (int)estado.See_Casilla(i-1,j+1)==jugador)
                           && ((int)estado.See_Casilla(i-2,j+2)==casilla_bomba || (int)estado.See_Casilla(i-2,j+2)==jugador)){
                            suma++;
                        }
                    }
                    if(i>1 && i<5 && j>1 && j<5){
                        if(((int)estado.See_Casilla(i+2,j-2)==casilla_bomba || (int)estado.See_Casilla(i+2,j-2)==jugador)
                           && ((int)estado.See_Casilla(i+1,j-1)==casilla_bomba || (int)estado.See_Casilla(i+1,j-1)==jugador)
                           && ((int)estado.See_Casilla(i-1,j+1)==casilla_bomba || (int)estado.See_Casilla(i-1,j+1)==jugador)){
                            suma++;
                        }
                    }
                }
                if(i>=3 && j>=3){
                    if(((int)estado.See_Casilla(i-1,j-1)==casilla_bomba || (int)estado.See_Casilla(i-1,j-1)==jugador)
                       && ((int)estado.See_Casilla(i-2,j-2)==casilla_bomba || (int)estado.See_Casilla(i-2,j-2)==jugador)
                       && ((int)estado.See_Casilla(i-3,j-3)==casilla_bomba || (int)estado.See_Casilla(i-3,j-3)==jugador)){
                        suma++;
                    }
                    if(i>0 && i<6 && j>0 && j<6){
                        if(((int)estado.See_Casilla(i+1,j+1)==casilla_bomba || (int)estado.See_Casilla(i+1,j+1)==jugador)
                           && ((int)estado.See_Casilla(i-1,j-1)==casilla_bomba || (int)estado.See_Casilla(i-1,j-1)==jugador)
                           && ((int)estado.See_Casilla(i-2,j-2)==casilla_bomba || (int)estado.See_Casilla(i-2,j-2)==jugador)){
                            suma++;
                        }
                    }
                    if(i>1 && i<5 && j>1 && j<5){
                        if(((int)estado.See_Casilla(i+1,j+1)==casilla_bomba || (int)estado.See_Casilla(i+1,j+1)==jugador)
                           && ((int)estado.See_Casilla(i+2,j+2)==casilla_bomba || (int)estado.See_Casilla(i+2,j+2)==jugador)
                           && ((int)estado.See_Casilla(i-1,j-1)==casilla_bomba || (int)estado.See_Casilla(i-1,j-1)==jugador)){
                            suma++;
                        }
                    }
                }
            }
        }
    }
    return suma;
}

// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Valoracion(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();
    int rival;
    double val_rival = 0, val_jugador = 0;

    if (ganador==jugador){
       return 999999.0; // Gana el jugador que pide la valoracion
    }
    else if (ganador!=0){
            return -999999.0; // Pierde el jugador que pide la valoracion
    }
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else{
        if(jugador == 1)
            rival = 2;
        else if(jugador == 2)
            rival = 1;
        val_rival = (filas_abiertas(estado,rival)+columnas_abiertas(estado,rival)+diagonales_abiertas(estado,rival));
        val_rival += 1000*(nfilas_3(estado,rival)+ncolumnas_3(estado,rival)+ndiagonales_3(estado,rival));
        val_jugador = filas_abiertas(estado,jugador)+columnas_abiertas(estado,jugador)+diagonales_abiertas(estado,jugador);
        val_jugador += nfilas_3(estado,jugador)+ncolumnas_3(estado,jugador)+1000*ndiagonales_3(estado,jugador);
        return val_jugador - val_rival;
    }
}

// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
    j=0;
    for (int i=0; i<8; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}

double Poda_AlfaBeta(Environment &actual, int jugador, int profundidad, int PROFUNDIDAD_ALFABETA, Environment::ActionType &accion, double alpha, double beta){
    double mejor_solucion;
    bool poda = false;
    Environment::ActionType accion_anterior;

    if(profundidad == PROFUNDIDAD_ALFABETA || actual.JuegoTerminado()){
        terminales++;
        return Valoracion(actual, jugador);
    }
    else{
        if(profundidad%2 ==0){
            mejor_solucion=menosinf;
        }
        else{
            mejor_solucion=masinf;
        }

        Environment sucesor[8];
        int n = actual.GenerateAllMoves(sucesor);

        if(profundidad % 2==0){
            for(int i=0; i < n && poda == false; i++){
                mejor_solucion = Poda_AlfaBeta(sucesor[i], jugador, profundidad+1, PROFUNDIDAD_ALFABETA, accion_anterior, alpha, beta);
                if(mejor_solucion > alpha){
                    alpha = mejor_solucion;
                    accion = static_cast <Environment::ActionType> (sucesor[i].Last_Action(jugador));
                }
                if(alpha >= beta){
                    poda = true;
                }
            }
            return alpha;
        }
        else{
            for(int i=0; i < n && poda == false; i++){
                mejor_solucion = Poda_AlfaBeta(sucesor[i], jugador, profundidad+1, PROFUNDIDAD_ALFABETA, accion_anterior, alpha, beta);
                if(mejor_solucion < beta){
                    beta = mejor_solucion;
                    accion = static_cast <Environment::ActionType> (sucesor[i].Last_Action(jugador));
                }
                if(alpha >= beta){
                    poda = true;
                }
            }
            return beta;
        }
    }
}


// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // acción que se va a devolver
    bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable
                        // aplicables[7]==true si BOOM es aplicable



    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha, beta; // Cotas de la poda AlfaBeta

    int n_act; //Acciones posibles en el estado actual


    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    actual_.PintaTablero();
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;
    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------


    // Opcion: Poda AlfaBeta
    // NOTA: La parametrizacion es solo orientativa
    valor = Poda_AlfaBeta(actual_, jugador_, 0, 8, accion, menosinf, masinf);
    cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;
    return accion;
}

