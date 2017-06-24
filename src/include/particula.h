#ifndef PARTICULA_H
#define PARTICULA_H

#include "constantes.h"
#include "hardware.h"
#include "hab.h"

extern Hardware *H;
extern VariablesJuego VJ;

class Particula{
  public:
      Particula(int X, int Y, int ox, int oy, BITMAP *BM, Particula *Ant);
      ~Particula();
      void SetSiguiente (Particula *Next);
      void SetAnterior (Particula *Prev);
      Particula * VerSiguiente ();
      Particula * VerAnterior ();
      int Mueve ();
      int Dibuja ();
      int Borra ();
      void DibujaSombra();
      void BorraSombra();
  private:
      int x, y, VelX, VelY, Angle, VelAng, CiclosVida, Transp;
      Coord2D Tile;
      BITMAP *BitMap;
      Hab *HAct;
      Particula *Siguiente, *Anterior;
};

#endif
