#ifndef SPRITE_H
#define SPRITE_H

#include "hardware.h"
#include "sprmanager.h"
#include "constantes.h"
#include "typedefs.h"

extern const int Tamano;
extern Hardware *H;
extern SprManager *M;

class Sprite
{
	public:
		Sprite();
		~Sprite();
		void CopyData (void *From);
		void Actualiza ();
		void Borra ();
		void Dibuja (bool actualiza = true);
		void PasaFrame ();
		const int VerX (bool Final = false);
		const int VerY (bool Final = false);
		const int VerLongX () {return Dibujo->w;}
		const int VerLongY () {return Dibujo->h;}
		const int VerXConVel () {return (int)(VerX() + VerVelX());}
		const int VerYConVel () {return (int)(VerY() + VerVelY());}
		const long double VerVelX () {return VelX;}
		const long double VerVelY () {if (VelY > 0 && VelY < 1)
                                        return 1;
                                        return VelY;
                                     }
		const int VerTipo () {return Tipo;}
		const int VerEst() {return Estado;}
		const int VerFotog () {return Fotograma;}
		const int VerSent () {return Sentido;}
		mask * VerMascara () {return Mascara;}
		void SetVelY (long double Vel) {VelY = Vel;}
		void SetVelX (long double Vel) {VelX = Vel;}
		void IncVelY (long double Vel) {VelY += Vel;}
		void IncVelX (long double Vel) {VelX += Vel;}
		void SetY (long double Y) {y = Y;}
		void SetX (long double X) {x = X;}
		void IncY (long double Y) {y += Y;}
		void IncX (long double X) {x += X;}
		void SetSent (int Sent) {Sentido = Sent;}
		void ClrFotog () {Fotograma = 0; Frames = 0;}
		int Reverse () {if (!Sentido) Sentido = 1; else Sentido = 0; return Sentido;}
		void SetTipo (int Tipo_) {Tipo = Tipo_;}
		void SetKF (int KF) {KeyFrame = KF;}
		void SetEst (int Est);
        bool Colision (Sprite *Otro, bool CheckPP = true);
        bool ColisionPP (void *Otro, int xt = -1, int yt = -1, mask *Masc = NULL, int Altura = 0);
        int FinAnim () {return FinAnimac;}
        void AcabaAnim () {Fotograma = (Mascara->Fotog)-1;}
        void SetTrans (int NTrans) {Trans = NTrans;}
        void IncTrans (int NTrans) {Trans += NTrans; if (Trans < 0) Trans = 0; if (Trans > 255) Trans = 255;}
        const int VerTrans () {return Trans;}
        void SetFotog (int Fotog) {Fotograma = Fotog;}
        const int VerAnim() {return Animacion;}
        void SetAnim (int Anim) {Animacion = Anim; Actualiza();}
        int DibujaHalo ();
        void SetHalo (bool Switch) {VerHalo = Switch;}
        int VerKF () {return KeyFrame;}
        bool VerParpadeo();
        void SetParpadeo(int Valor);
        void SetEstDest (int ED) {EstDest = ED;}
        const int VerEstDest () {return EstDest;}
	protected:
        int  Fotograma, FotoMax, Frames, KeyFrame;
        int Parpadeo;
        long double VelX, VelY, x, y;
        int Tipo, Sentido, Estado, EstDest, Animacion, Trans;
        bool Repeat, FinAnimac, VerHalo;
        unsigned char Halo;
        char HaloSent;
        BITMAP *Dibujo;
        mask *Mascara;
        void SetRepeat (bool Rep) {Repeat = Rep;}
//    private:
};

#endif

