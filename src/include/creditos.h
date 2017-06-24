#ifndef CREDITOS_H
#define CREDITOS_H

#include "constantes.h"
#include "hardware.h"
#include "messages.h"
#include "configuracion.h"
#include "sonido.h"

extern Hardware *H;
extern Messages *SMS;
extern Configuracion *Config;
extern Sonido *SFx;
extern volatile int close_button_pressed;

const int MaxBitMap = 12;

class Creditos
{
	public:
		Creditos();
		~Creditos();
		void Run();
	private:
        int Fase, BitMap, TextY;
        void PreparaTexto ();
        void PreparaBM ();
        void Texto (const char* Texto, int x, int y, int RI = 252, int GI = 224, int BI = 168);
};

#endif
