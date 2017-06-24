#ifndef MENU_H
#define MENU_H

#include "constantes.h"
#include "messages.h"
#include "hardware.h"
#include "sprite.h"
#include "configuracion.h"
#include "background.h"
#include "sonido.h"

extern Messages *SMS;
extern Hardware *H;
extern Configuracion *Config;
extern Sonido *SFx;
extern volatile int tCounterMenu;
extern Sprite SpritesMenu[];
extern const int NSprMenu;
extern double FuncionCoseno[FramesCirculo];
extern double FuncionSeno[FramesCirculo];
extern volatile int close_button_pressed;
extern volatile int tAttractMode;

class Menu
{
	public:
           Menu(int OrX, int OrY, int Op, int JumpY, int Just, const char *Tit, Background *Fond);
           ~Menu();
           void DActivar (int Opc, bool Accion);
           int SetTituloOp (int Opc, char* Tit, int Tipo = 0);
           int Run(int &Sent, Background *Fond, int Sel = 0, bool EsperaTecla = false);
           int DrawWSpr();
           void Spr();
           bool Input (int x, int y, int Length, bool Centr, int R, int G, int B, int RB = -1, int GB = -1, int BB = -1);
           void ClrTitulo();
           void Fade (int Sent);
	private:
	        Background *Fondo;
	        int maxSkip, curSkip, OrigX, OrigY, SaltoY, Opciones, Justify, *Opcion;
	        int Green, GreenSent, FramesCirc;
	        bool SprInic, needsRefresh, CambioOpcion;
	        Coord2D InicSpr [6], Ovalo[5];
	        int Foco[2], CFoco[2], TFoco[2];
	        char *Titulo, **TitOp, Temp[256];
	        bool *Enabled;
	        void Effects(int Select);
	        void InitSprites ();
	        void BorraSprites();
	        int Draw();
	        void EasterEggGandulfo();
	        void DibujaSprites();
            void ComputerWrite (const char* Frase, int y);
};

#endif // MENU_H
