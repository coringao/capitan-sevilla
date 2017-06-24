#ifndef ACTOR_H
#define ACTOR_H

#include "hab.h"
#include "mundo.h"
#include "constantes.h"
#include "hardware.h"
#include "sonido.h"
#include "sprite.h" // Todos los Actores son Sprites
#include "sistparticulas.h"

extern Coord2D CAct;
extern Hardware *H;
extern Sonido *SFx;
extern const int AnimMariano;
extern const int AnimCapitan;
extern bool Suena[];
extern SistParticulas *SP;
extern double FuncionCoseno[FramesCirculo];
extern double FuncionSeno[FramesCirculo];

extern VariablesJuego VJ;

class Actor : public Sprite
{
	public:
		Actor(Actor *Pointer, int Clase, int Est, int Sent, char Equi, int X, int Y, int Power, int ref, int CiclEsp, int Cap);
		Actor(Actor *Pointer, Objeto* Obj, int ref);
		~Actor();
		Actor *VerSiguiente () {return Siguiente;}
		Actor *VerAnterior () {return Anterior;}
		Actor *VerPertenencia () {return Pertenencia;}
		const int VerEquipo () {return Equipo;}
		void SetEquipo (int Equi) {Equipo = Equi;}
        const int VerCapa () {return Capa;}
		void SetSiguiente (Actor *Pointer) {Siguiente = Pointer;}
		void SetAnterior (Actor *Pointer) {Anterior = Pointer;}
		void SetPertenencia (Actor *Pointer) {Pertenencia = Pointer;}
        int PreparaMov (Actor *Inicial, Disparo &Retorno, int Tecla);
        int Mueve (Mundo *MAct, Hab *HAct, Actor *Inicial);
        int ValoraColisiones (Hab *HAct);
        void SendMessage (int Mess) {Message = Mess;}
        int ReadMessage () {return Message;}
        int VerRef () {return Ref;}
        int IncPow (int Inc) {Energia += Inc; return Energia;}
        int VerPow () {return Energia;}
        int SetPow (int Pow) {Energia = Pow; return Energia;}
        void Dibuja_Sombra ();
        void Borra_Sombra ();
        int VerEspera ();
        void Explota ();
        void Muerte ();
        void TiraAbajo();
        void ClrCiclos ();
        bool VerSombra();
        bool VerRad();
        void SetCapa(int Cap);
        void SetGrav (bool Grav);
        const bool VerGrav ();
	private:
        int CiclosQuieto, CiclosCayendo, CiclosEspera, Xi, Yi, Ref;
        int Energia, Message, Capa, Senti, Rebote, CiclosRad, Vidas;
        bool Gravedad, Activo, Colisionable;
        char Equipo;
        Actor *Siguiente, *Anterior, *Pertenencia;
        void Ajustes1(Objeto *Obj);
        void Ajustes2();
        void GeneraFX();
        int AjustaKF();
        void AjustaEstado();
        inline void Frena (long double &Variable);
};

#endif // ACTOR_H
