#ifndef PRESENTACION_H
#define PRESENTACION_H

#include "constantes.h"
#include "hardware.h"
#include "sprite.h"
#include "sonido.h"
#include "configuracion.h"
#include "messages.h"
#include "actormanager.h"

extern Hardware *H;
extern Sonido *SFx;
extern Configuracion *Config;
extern Messages *SMS;
extern volatile int close_button_pressed;
extern bool Suena[];

class Presentacion
{
	public:
		Presentacion();
		~Presentacion();
		int Run (bool Comprueba);
        int FinComic ();
        int FinPeriodico();
        int FinFeria ();
	private:
        int CiclosQ, TransLogo, SentTL, NSprites;
        int maxSkip, curSkip, EstPres;
	    unsigned int Frm;
        bool needsRefresh;

        void ActualizaSpr();
        void SetEst (int Est, int KF = -1);
        int CEZTeam (bool Comprueba);
        int CEZRD (bool Comprueba);
        void IntroComic ();
        void GiraPeriodico (void *Pointer);
        void TextoPeriodico (void *BitMap, int Tamano, const void *Texto, int &y, int x = -1);
};

#endif // PRESENTACION_H
