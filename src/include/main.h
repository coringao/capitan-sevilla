#include "constantes.h"
#include "hardware.h"
#include "configuracion.h"
#include "gestordemundos.h"
#include "sprmanager.h"
#include "partida.h"
#include "presentacion.h"
#include "sonido.h"
#include "messages.h"
#include "menuplayer.h"
#include "sprite.h"
#include "sistparticulas.h"

Hardware *H;
SprManager *M;
Configuracion *Config;
GestorDeMundos *GMundos;
Partida *Game;
Presentacion *Presen;
Sonido *SFx;
Messages *SMS;
MenuPlayer *Menus;
SistParticulas *SP;

const int NSprMenu = 6;
Sprite SpritesMenu[NSprMenu];

volatile int close_button_pressed = false;

double FuncionCoseno[FramesCirculo];
double FuncionSeno[FramesCirculo];

const int MaxSonido = 256;
bool Suena[MaxSonido];

Coord2D CAct;
int ObjCargados;
bool Prim[3] = {1,0,0};
bool Seg[3] = {0,1,0};
bool Pre[3] = {1,1,0};
bool Post[3] = {0,0,1};
bool All[3] = {1,1,1};
const int Tamano = sizeof(char) * sizeof(word);
int fps, cfps, afps, Ciclos, Media; // Conteo de fps

VariablesJuego VJ;

int Creacion (int argc, char *argv[]);
void Destruccion ();
void IndicadorCarga (DATAFILE *dat_obj);
void close_button_handler(void);

