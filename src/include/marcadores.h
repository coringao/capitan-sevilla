#ifndef MARCADORES_H
#define MARCADORES_H

#include "constantes.h"
#include "hardware.h"
#include "sprmanager.h"
#include "actor.h"

const int NSprMarc = 6;

class Marcadores
{
	public:
		Marcadores();
		~Marcadores();
		void Show (int curSkip, int afps, int Temporizador);
		void Torrebruno (int Estado);
        void SituaTorre ();
  private:
        Sprite SprMarc[NSprMarc];
        int CiclosQTorre;
    
};

extern Hardware *H;
extern SprManager *M;
extern VariablesJuego VJ;

#endif // MARCADORES_H
