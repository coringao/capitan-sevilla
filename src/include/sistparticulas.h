#ifndef SISTPARTICULAS_H
#define SISTPARTICULAS_H

#include "constantes.h"
#include "particula.h"
#include "hardware.h"
#include "sprmanager.h"

extern Hardware *H;
extern SprManager *M;

class SistParticulas{
  public:
      SistParticulas();
      ~SistParticulas();
      void CreaSistema (int x, int y, void *BitMap);
      int MueveSistema ();
      void DibujaSistema ();
      void BorraSistema ();
      void DibujaSombra();
      void BorraSombra();
      void Reset();
  private:
      int NumParticulas;
      Particula *Inicial, *Final;
};

#endif
