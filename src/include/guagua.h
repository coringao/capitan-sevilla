#ifndef GUAGUA_H
#define GUAGUA_H

#include "constantes.h"
#include "sprite.h"
#include "sonido.h"

extern Sonido *SFx;

class Guagua{
      public:
          Guagua(int XStop);
          ~Guagua();
          void Dibuja();
          void Borra();
          int Mueve();
          int VerX ();
      private:
          Sprite Autobus, Cristal, Faros;
          int Stop, CiclosQuieto, Velocidad, Linea;
          char LineaCad[4];

};

#endif
