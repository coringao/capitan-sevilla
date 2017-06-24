#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include "constantes.h"

const char CfgFileVersion = 15;

class Configuracion
{
	public:
		Configuracion();
		~Configuracion();
		const char VerIdioma ();
		void ToggleIdioma (char Rec = -1);
		const bool VerSonido ();
		void ToggleSonido (char Rec = -1);
		const bool VerBlending ();
		void ToggleBlending (char Rec = -1);
		const bool VerWindowed ();
		void ToggleWindowed (char Rec = -1);
		const bool VerDinamic ();
		void ToggleDinamic (char Rec = -1);
		const bool VerScanL ();
		void ToggleScanL (char Rec = -1);
		const bool VerShadow ();
		void ToggleShadow (char Rec = -1);
		const int VerResAlto ();
		const int VerResAncho ();
		void ToggleRes (char Rec = -1);
		const char VerCDepth ();
		const char VerGraphDet ();
		bool VerIntro();
		const char VerFase ();
		void ToggleFase(char Rec = -1);
		const char VerPass ();
		void SetPass(char Pass);
		char IncGraphDetail (char Inc);
        const unsigned char VerVolFX();
        void IncVolFX (int Inc);
        const unsigned char VerVolMus();
        void IncVolMus (int Inc);
        void GrabaTecla (int Sentido, char Key);
		const char VerTecla (int Sentido);
        void ToggleJoy(char Rec = -1);
        const char VerJoy();
        void ToggleDif (char Rec = -1);
        const char VerDif ();
	private:
                char Idioma, Resol, GraphDet, ColorDepth, Tecla[7], Fase, Passwd, Joystick, Dif;
                unsigned char VolFX, VolMus;
                bool Sound, Windowed, Blending, Dinamic, ScanL, Shadow, Intro;
                int Save();
                char AdjustDet ();
};

#endif
