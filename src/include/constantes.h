#ifndef CONST_H
#define CONST_H

// Componentes opcionales en tiempo de compilación

//#define FBLEND
//#define GP2X
//#define WINDOWS__

/* ********************************************************************* */

// Número de versión
#define VMAJOR 1
#define VMINOR 0
#define VMICRO 3
#define VSTATUS "F"

//Datos de pantalla

#define ANCHOPANTALLA 512
#define ALTOPANTALLA 392

// Teclado

#define ARRIBA 1
#define ABAJO 2
#define IZQUIERDA 4
#define DERECHA 8
#define FUEGO 16
#define SELEC 32
#define ABORT 64

// Datos de los tiles

#define TILESANCHO 32
#define TILESALTO 17
#define LAYERS 3
#define TAMTILE 16

// Equipos de actores

#define AMIGO 1
#define ENEMIGO 2
#define PLATAFORMA 3
#define NEUTRO 4
#define OBSTACULO 5

 // Tipos de Sprite

#define TILE 1
#define MARIANO 2
#define CAPITAN 3
#define MARIANOASTR 4

#define FAROLG 100
#define FAROLP 101
#define FAROLPC 102
#define CONTENEDOR 103
#define SEMAFORO 104
#define VESPA 105
#define MACETERO 106
#define CVIDRIO 107
#define BOLARDO 108
#define SACOS 109
#define ARENA 110
#define FORJADO 111
#define CAJA 112
#define BARRIL 113
#define FAROLA 114
#define MORCILLA 115
#define PASAJE 116
#define PLATFORM 117
#define FOCAZO 118
#define CAMARERO 119
#define VENTANA 120
#define NEGRO 121
#define CURRO 122
#define GANGSTER 123
#define LANZAOBJETOS 124
#define CUBO 125
#define MUJER 126
#define BUSSTOP 127
#define ABEJAS 128
#define HORMI 129
#define NINO1 130
#define ELEFANT 131
#define SIERRA 132
#define CABINA 133
#define TORO 134
#define NINO2 135
#define OVNIJ 136
#define COCO 137
#define ASTRON 138
#define ALIEN 139
#define BURGUER 140
#define JERING 141
#define ASOLDIER 142
#define HUMOJ 143
#define FINFASE 144

#define VIGONCIO 145
#define COLUMNA 146
#define LUZVERDE 147
#define ANUNCIOWIN 148
#define SALTARIN 149
#define ROBOTIN 150
#define ASTRONAUTA2 151
#define MONSTRUO 152
#define ASPIRADOR 153
#define SHORTCIRCUIT 154
#define GRUA 155
#define RODADOR 156
#define TENTACLES 157
#define PC 158
#define TORRETA 159
#define MIRA 160
#define ASTRONAUTA3 161
#define TORRES 162
#define PANTALLA8B 163


#define HALOG 200
#define HALOP 201
#define HALOPC 202
#define HALOGB 203
#define DMORCILLA 204
#define ONOMAT 205
#define CAMARSIN 206
#define HALOMORCI 207
#define CURRO1 208
#define CURRO2 209
#define CURRO3 210
#define OBJVENTANA 211
#define VENTANA1 212
/*#define VENTANA2 213
#define VENTANA3 214
#define VENTANA4 215
#define VENTANA5 216
#define VENTANA6 217
#define VENTANA7 218
#define VENTANA8 219*/
#define BUS 220
#define CRISTALBUS 221
#define LUZBUS 222
#define MARIANOMUERTO 223
#define ALMA 224
#define CUORE 225
#define GLOBO 226
#define CRCABINA 227
#define BALON 228
#define NINO3 229
#define BURBUJA 230
#define CHEPPY1 231
#define CHEPPY2 232
#define CHEPPY3 233
#define LASERJ 234
#define CEMENTO 235
#define CIERRE1 236
#define CIERRE2 237
#define LANZADERA 238
#define BOLITA 239
#define METEORITO 240
#define COLUMNON 241
#define PANTALLA 242
#define TORREC 243
#define LUCECITA 244
#define RAYOTORRE 245
#define ESCAFANDRAM 246


#define BASE 300
#define CARAVIDA 301
#define MORCILLITA 302
#define QUEHASE 303
#define NIEBLA 304
#define MORCILLON 305
#define MARCOTORRE 306
#define INTERFERENC 307
#define TORREMARCA 308
#define NIEBLATORRE 309

#define CAPMENU 400
#define OVNI1 401
#define OVNI2 402
#define OVNI3 403
#define GANDULFO 404
#define OVNIPEQ 405
#define OVNIMINI 406
#define EXPLPOLVO 407
#define HUMO 408
#define MESACAPI 409
#define MARIANOBEBE 410
#define SIRFRED 411


#define ESCAFANDRA 999

// Estado de los actores
#define ANDANDO 0
#define PARADO 1
#define AGACHADO 2
#define PRESALTO 3
#define SUBIENDO 4
#define CAYENDO 5
#define POSTSALTO 6
#define LEVANTANDO 7
#define GIRANDO 8
#define DISPARANDO 9
#define TRANSFOR 10
#define ARRANCANDO 11
#define DESPEGANDO 12
#define VOLANDO 13
#define SOPLO 14

#define MUERTE 20
#define NACIENDO 21
#define SALIENDO 22
#define METIENDOSE 23
#define FRENANDO 24
#define GIRAQUIETA 25
#define ANDASIN 26
#define BABEANDO 27
#define COGEBAL 28
#define TIRABAL 29
#define ENTERRANDOSE 30
#define ENTERRADO 31
#define SURGIENDO 32
#define DESENFUNDA 33
#define ATURDIDO 34
#define DESAPARECE 35
#define APARECE 36
#define OCULTO 37
#define RIENDO 38
#define STAND 39
#define STAND2 40
#define ENFUNDA 41


// Acciones del personaje al pulsar "Fuego"

#define PUNO 0
#define MORCILLAS 1
#define SOPLAR 2
#define RADIACTIVO 3
#define VOLAR 4

// "Salidas" de métodos y funciones y "mensajes" entre actores

#define TODOOK 0
#define SALEPANTALLA 1
#define COLISION 2
#define GAMEOVER 3
#define DESPLAZA 4
#define TERMINADO 5
#define ATTRACTMODE 6
#define GIRAR 7
#define MUERTEENEMIGO 8
#define CIERRAVENT -2
#define SSHOT 999 // <------------------------------ BETA TESTING

// OTROS

#define MAXCURRO 3
#define MAXCHEPPY 3
#define MAXVENTANA 8
#define MAXPANTALLA 8

#define COLFF makecol(255,255,255)

#define MAXOBJS 16
#define MAXCUESTA 4
#define TEMPORIZADOR 40
#define DAT_PNG  DAT_ID('P','N','G',' ')
#define PI 3.141592654
#define ATTRACTWAIT 15

#define PASSWORD "OZUMIARMA"


#include <allegro.h>   // Allegro
#ifdef FBLEND
  #include <fblend.h>    // Fblend
#endif
//#include <fmod.h>      // FMOD
#include <alogg.h>
#include <alfont.h>    // AlFont
#include <fstream>     // Manejo de ficheros de E/S
#include <string.h>      // Manejo de cadenas de caracteres
#include <ctype.h>     // Funciones tolower() y toupper()
#include <math.h>      // Funciones trigonométricas
#include "loadpng.h"   // Cargar archivos .png
#include "typedefs.h"  // Definiciones de tipos
#include "sprites.h"   // Cabecera para el archivo sprites.dat
#include "bitmaps.h"   // Cabecera para el archivo bitmaps.dat
#include "sfx.h"       // Cabecera para el archivo sfx.dat


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>


using namespace std;

extern int LogInit ();                  // Al poner estas definiciones aquí se pueden usar estas funciones en cualquier
extern void LogWrite(const char *format,...);
extern int LogEnd();
extern int GameLogInit (int AMode, int &Fase);      // Al poner estas definiciones aquí se pueden usar estas funciones en cualquier
extern void GameLogWrite(int data);
extern int GameLogRead();
extern int GameLogEnd();

//const int EnergiaInic = 100; Substituido por (140 - 20 * Config->VerDif())

const int FramesCirculo = 720;
const int TamanoPart = 2 * 2;

const int TeclaSent [] = {ARRIBA, ABAJO, IZQUIERDA, DERECHA, FUEGO, SELEC};

#endif
