#ifndef ACTORMANAGER_H
#define ACTORMANAGER_H

#include "actor.h"
#include "hab.h"
#include "guagua.h"
#include "panel.h"

extern Coord2D CAct;
extern VariablesJuego VJ;

class ActorManager
{
	public:
		ActorManager();
		~ActorManager();
		Actor* NuevoAct (int Clase, int Estado, int Sent, char Equi, int X, int Y, int Power, int Cap = -1, int ref = -1, int CiclEsp = 0);
		Actor* NuevoAct (Objeto *Obj, int ref = -1);
		int Destroy (bool Todos = false);
		void DestroyEnemies ();
		int Creation ();
		int BorraAct (Actor *Obj);
		void* VerInic () {return (void*) Inicial;}
		void* VerFin () {return (void*) Final;}
		int ActualizaMov (int Tecla);
		void Situa (Mundo *Wrld, Hab *Room)
		     {MAct = Wrld; HAct = Room; Objetos = HAct->VerObjetos();}
        void Dibuja(int CapaVis, bool Actualiza = true);
        void Borra();
        int Colisiones ();
        void Dibuja_Sombra (int CapaVis);
        void Borra_Sombra (int CapaVis);
        void PasaFrame ();
	private:
        int Numero, Xi, Yi;
        Actor *Inicial, *Final;
        Mundo* MAct;
        Hab *HAct;
        Objeto *Objetos;
};

#endif // ACTORMANAGER_H
