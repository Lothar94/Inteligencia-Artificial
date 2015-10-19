#include "agent.h"
#include "environment.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>
#include <climits>

using namespace std;
// -----------------------------------------------------------
Agent::ActionType Agent::Think()
{
	ActionType accion;
	static int tamanio = 10;            //Tamanio de trufas a recoger.
	static int ultima_recogida=0;

	/*Si el ultimo paso fue avanzar y no se ha chocado y llevamos mas de 100 pasos olemos.*/
    if(ultimo_paso==actFORWARD && bump_!=true && paso>100){
        accion=actSNIFF;
    }
    /*Si el ultimo paso fue oler y el tamanio de las trufas en la casilla es mayor o igual a 10 recogemos las trufas.*/
    else if(ultimo_paso==actSNIFF && trufa_size_>=tamanio){
        accion=actEXTRACT;
        recogida+=trufa_size_;                                  //Total trufas recogidas
        /*if(recogida-ultima_recogida>=200){
            tamanio=8;
        }
        else{
            tamanio=10;
        }*/
    }
    else{
        /*Si no nos hemos chocado y la dirección en la que vamos es correcta entonces avanzamos
        y vamos incrementando o decrementando la posición segun nuestra orientación*/
        if(bump_ != true && obtenerDireccion()==director){
            accion=actFORWARD;
            if(director==0){
                pos_y--;
            }
            else if(director==1){
                pos_x--;
            }
            else if(director==2){
                pos_y++;
            }
            else if(director==3){
                pos_x++;
            }
        }
        else{
            /*Para detectar las paredes si nos hemos chocado depende de la direccion colocamos el valor -1 y aumentamos o decrementamos
            la posicion del agente en la matriz segun convenga.*/
            if(director==0 && bump_==true){
                mapa[pos_y][pos_x] = -1;
                pos_y++;
            }
            else if(director==1 && bump_==true){
                mapa[pos_y][pos_x] = -1;
                pos_x++;
            }
            else if(director==2 && bump_==true){
                mapa[pos_y][pos_x] = -1;
                pos_y--;
            }
            else if(director==3 && bump_==true){
                mapa[pos_y][pos_x] = -1;
                pos_x--;
            }
            /*Depende de la direccion a la que quiera dirigirse el agente giraremos a la izquierda o la
            derecha, si cuesta menos pasos girar a la derecha lo hace si no lo hace a la izquierda.*/
            if(director==0 && obtenerDireccion()==3){
                accion=actTURN_R;
                director =3;
            }
            else if(director==1 && obtenerDireccion()==0){
                accion=actTURN_R;
                director--;
            }
            else if(director==2 && obtenerDireccion()==1){
                accion=actTURN_R;
                director--;
            }
            else if(director==3 && obtenerDireccion()==2){
                accion=actTURN_R;
                director--;
            }
            else{
                accion=actTURN_L;
                director++;
                if(director>=4)
                    director =0;
            }
        }
    }
    if(paso%100==0){
        ultima_recogida = recogida;
    }
    mapa[pos_y][pos_x]=paso;                //Vamos colocando en nuestra matriz el paso por el que vamos para dirigirnos siempre al mas antiguo.
    paso++;
    ultimo_paso=accion;                     //Guardamos la ultima accion.
    //Mostrarmat();
	return accion;
}
//------------------------------------------------------------
int Agent::obtenerDireccion(){
    int direccion, minimo, arr_pos[4];
    minimo=INT_MAX;
    arr_pos[0] = mapa[pos_y-1][pos_x];
    arr_pos[1] = mapa[pos_y][pos_x-1];
    arr_pos[3] = mapa[pos_y][pos_x+1];
    arr_pos[2] = mapa[pos_y+1][pos_x];

    if(arr_pos[director]==0){               // si la posicion justo delante de la direccion en la nos dirigimos no la hemos visitado seguimos en esa diraccion.
        direccion = director;
    }
    else{
        for(int i=0;i<4;i++){
            if(arr_pos[i]<minimo && arr_pos[i]!=-1){
                minimo = arr_pos[i];
                direccion = i;
            }
        }
    }
    return direccion;
}

void Agent::Mostrarmat(){
    for(int i=0;i<19;i++){
        for(int k=0;k<19;k++){
            printf("%4d",mapa[i][k]);
        }
        cout << endl;
    }
    cout << endl;
}
// -----------------------------------------------------------
void Agent::Perceive(const Environment &env)
{
	trufa_size_ = env.TrufaAmount();
	bump_ = env.isJustBump();
}
// -----------------------------------------------------------
string ActionStr(Agent::ActionType accion)
{
	switch (accion)
	{
	case Agent::actFORWARD: return "FORWARD";
	case Agent::actTURN_L: return "TURN LEFT";
	case Agent::actTURN_R: return "TURN RIGHT";
	case Agent::actSNIFF: return "SNIFF";
	case Agent::actEXTRACT: return "EXTRACT";
	case Agent::actIDLE: return "IDLE";
	default: return "???";
	}
}
