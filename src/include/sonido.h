#ifndef SONIDO_H
#define SONIDO_H

#include "hardware.h"
#include "constantes.h"
#include "configuracion.h"

extern Hardware *H;
extern Configuracion *Config;

class Sonido
{
	public:
		Sonido();
		~Sonido();
		int PlayFX (int FX, int loop = 0);
        void StopFX (int FX);
		int PlayMusic (const char *Nombre, bool Loop = false, int Vol = -1);
		int StopSound ();
		void ActVolume (int Vol);
	private:
                ALOGG_OGG *ActFx;
                DATAFILE *dfMusic;
                bool loop;
};

#endif
