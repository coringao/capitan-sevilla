#ifndef PANEL_H
#define PANEL_H

#include "constantes.h"
#include "hardware.h"
#include "guagua.h"
#include "messages.h"

extern Hardware *H;
extern VariablesJuego VJ;
extern Messages *SMS;

const int MensPanel = 14;

class Panel
{
	public:
		Panel(int X, int Y);
		~Panel();
		void Dibuja ();
		void AjustaCiclos();
	private:
        int x, y, Offset, Ciclos, CTiempo, Segundos, SegAnterior, MaxSec;
        bool MostrarTiempo, DosPuntos;
        char Mensaje[255];
        BITMAP *Texto, *Pantalla;
        void CreaMensaje();
};

#endif // PANEL_H
