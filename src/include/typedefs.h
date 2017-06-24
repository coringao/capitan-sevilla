#ifndef TYPEDEFS_H
#define TYPEDEFS_H

typedef struct {
        char Tipo;
        char Equipo;
        char X;
        char Y;
        char OffX;
        char OffY;
        char Sent;
        char CiclosEspera;
        char KeyF;
        char Capa;
        char Grav;
        char byte1;
        char byte2;
        char byte3;
        char byte4;
        } Objeto;

typedef unsigned long int word;

typedef struct {
        int LongX;
        int LongY;
        int Fotog;
        int Transp;
        int Colisiona;
        int Words;
        word** MaskData;} mask;

typedef struct {
        int X;
        int Y; } Coord2D;

typedef struct {
        int X;
        int Y;
        int NFrases;} FraseComic;

typedef struct {
        int Tipo;
        int Sentido;
        int Estado;
        int Equipo;
        int X;
        int Y;
        int Capa;
        char byte;} Disparo;

typedef struct {
        int x;
        int y;
        int Estado;
        int Ciclos; } Estrella;

typedef struct {
        int x[10];
        int y[10];
        int Color[10];
        int Ciclos;
        int vx;
        int vy; } EFugaz;

typedef struct {
        bool Cheats;
        int Morcillas;
        int ObjCargados;
        int Fase;
        int Vidas;
        int QueHase;
        int QueHasia;
        bool OSD;
        void* Jugador;
        void* Autobus;
        void* Panel;
        void* HAct;
        void* Torrebruno;
        void* SSystem;} VariablesJuego;

#endif
