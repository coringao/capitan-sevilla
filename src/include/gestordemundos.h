#ifndef GESTORDEMUNDOS_H
#define GESTORDEMUNDOS_H

#include "mundo.h"
#include "constantes.h"

/*
 * Manejo de la lista enlazada "Mundos"
 */
 
class GestorDeMundos
{
	public:
		GestorDeMundos();
		~GestorDeMundos();
		Mundo* BuscaMundo (int Num);
		int Load ();
        void Reset ();
	private:
        int NumMundos;
        Mundo *Inicial, *Final, *Temp, *Temp2, *Temp3;
		Mundo* CreaMundo (unsigned char Anc, unsigned char Alt);
};

#endif
