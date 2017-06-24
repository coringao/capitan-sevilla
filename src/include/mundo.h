#ifndef MUNDO_H
#define MUNDO_H

/*
 * Objetos de la lista Gestor de Mundos -> "Mundo" de habitaciones interconectadas en el mapa
 */

#include "constantes.h"
#include "hab.h"
 
class Mundo
{
	public:
        Mundo (unsigned char Anc, unsigned char Alt, Mundo *Ant, unsigned char Num);
		~Mundo();
		const int VerNumero () {return MiNumero;}
		Mundo* VerSiguiente () {return Siguiente;}
		Mundo* VerAnterior () {return Anterior;}
		void SetSiguiente (Mundo *Sig) {Siguiente = Sig;}
		Hab* IrHab (Coord2D Habit);
		const Coord2D HabInic() {return Inic;}
		const bool ExisteHab (Coord2D &Coord);
		int Load ();
        void SetInitRoom (int x = 255, int y = 255);
		const int VerAncho () {return Ancho;}
		const int VerAlto () {return Alto;}
	private:
        unsigned char MiNumero, Ancho, Alto;
        Coord2D Inic;
        bool **Habs;
        Hab ***Habitacion;
        Mundo *Anterior, *Siguiente;
        void Reset ();
        Hab* CreateRoom (int x, int y, int w, int h);
};

#endif // MUNDO_H
