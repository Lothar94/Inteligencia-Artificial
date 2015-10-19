#ifndef AGENT__
#define AGENT__

#include <string>
using namespace std;

// -----------------------------------------------------------
//				class Agent
// -----------------------------------------------------------
class Environment;
class Agent
{
public:
	Agent(){
        trufa_size_=-1;
        bump_=false;
        pos_x=pos_y=9;
        /*Inicializamos la matriz del mapa:
            0 significa no explorado.
            -1 significa pared (bump==1).
            [1,2000] pasos que indicaran la antiguedad.
        */
        for(int i=0; i<19; i++){
            for(int k=0; k<19;k++){
                mapa[i][k]=0;
            }
        }
	}

	enum ActionType
	{
	    actFORWARD,
	    actTURN_L,
	    actTURN_R,
	    actSNIFF,
	    actEXTRACT,
	    actIDLE
	};
//--------------------Funciones------------------------------------
    int obtenerDireccion();
    void Mostrarmat();
//-----------------------------------------------------------------
	void Perceive(const Environment &env);
	ActionType Think();
private:
	int trufa_size_;
	bool bump_;
//--------------------Variables-----------------------------------
    int mapa[19][19];
    int pos_x, pos_y;
    int paso=1;
    int director=0;
    int recogida=0;
    ActionType ultimo_paso;
//----------------------------------------------------------------
};

string ActionStr(Agent::ActionType);

#endif
