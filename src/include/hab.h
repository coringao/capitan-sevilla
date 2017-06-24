#ifndef HAB_H
#define HAB_H

#include "constantes.h"
#include "hardware.h"
#include "sprmanager.h"
 
class Hab
{
	public:
		Hab(int Anc, int Alt, int Num);
		~Hab();
		void Dibuja (int CapaVis);
		int Load ();
		int CContenido (Coord2D &Coord, bool Nube = true);
		int NumObj () {return NumObjs;}
		Objeto* VerObjetos () {return Objects;}
        int InsertaObj (Objeto *ObjInsert);
        unsigned char VerNum ();
        unsigned char SetNum (unsigned char Num);
        void Dibuja_Sombra ();
        mask* CTileMask (Coord2D &Coord, bool Nube = true);

	private:
        unsigned char ***Datos;
        unsigned char NumObjs, Ancho, Alto, MiNum;
        Objeto Objects[256];
        bool Existe;

};

extern Hardware *H;
extern SprManager *M;

#endif // HAB_H
