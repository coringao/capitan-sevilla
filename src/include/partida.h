#ifndef PARTIDA_H
#define PARTIDA_H

#include "constantes.h"
#include "hardware.h"
#include "configuracion.h"
#include "gestordemundos.h"
#include "sprmanager.h"
#include "actormanager.h"
#include "background.h"
#include "marcadores.h"
#include "messages.h"
#include "sistparticulas.h"
#include "guagua.h"
#include "panel.h"
#include "creditos.h"
#include "presentacion.h"

extern Coord2D CAct;
extern Hardware *H;
extern SprManager *M;
extern Configuracion *Config;
extern GestorDeMundos *GMundos;
extern Messages *SMS;
extern int fps, cfps, afps, Media, Ciclos;
extern const int MaxSonido;
extern bool Suena[];
extern SistParticulas *SP;

extern VariablesJuego VJ;
extern volatile int close_button_pressed;

class Partida
{
     public:
        Partida();
       ~Partida();
        int Init (int Attract = 0);
        int Ciclo();
     private:
        bool CheatRoom;
        char Vidas;
        int maxSkip, curSkip, XGrab, YGrab, Salir, RetMov, Capa, Temporizador, CiclosMuerto;
        int Fotograma, CheckFS, FSent;
        int Tecla;
        int DesplTile;
        int AttractMode, ContTextAM;
        Mundo *MAct;
        Hab *HAct;
        ActorManager *AM;
        Actor *Mariano;
        Background *Backgd;
        Marcadores *Marcador;
        bool needsRefresh;
        void Situa ();
        int VidaMenos ();
        void NuevaVida ();
        int GameOver ();
        void LogSalida (int Resultado);
        int CicloLogico ();
        void CicloVisual ();
        int Pausa();
        void BorraBus();
        void InterFase();
        int FinAgujero();
        void FindeJuego();
        void StopSounds();

};

#endif // PARTIDA_H
