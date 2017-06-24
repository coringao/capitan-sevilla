#ifndef BACKGROUND_H
#define BACKGROUND_H

/*
 * Esta clase se encarga de generar el Fondo del decorado
 */
 
#include "hardware.h"
#include "sprite.h"
 
extern Hardware *H;
extern double FuncionSeno[FramesCirculo];

const int MaxEstrellas = 10;
const int MaxDisparos = 10;
const int MaxOvnis = 3;
const int MaxHumo = 3;

class Background
{
	public:
		Background();
		~Background();
		void Genera(int Num);
		void Actualiza ();
	private:
        BITMAP *FondoBack, *FondoStar;
        Estrella Stars [MaxEstrellas];
        EFugaz Fugaz;
        EFugaz Disparo[MaxDisparos];
        Sprite Ovni[MaxOvnis], Humo[MaxHumo];
        int Disparos, OrigOvni[MaxOvnis];

        int NumStars, NumHab;
        char CDepth;
        void DibujaEstrella (int Star);
        int BorraEstrella (int Star);
        void CreaEstrella ();
        void CreaFugaz ();
        void DibujaFugaz ();
        void PonPixel (int x, int y, int color);
        bool PixelValido (int x, int y);
        void DibujaDisparo(EFugaz &Disp);
        int Dispara(int NOvni);
        void MueveOvni();
};

#endif // BACKGROUND_H
