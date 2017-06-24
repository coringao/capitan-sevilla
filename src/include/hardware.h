#ifndef HARDWARE_H
#define HARDWARE_H

#include "constantes.h"
#include "configuracion.h"
#include "messages.h"

extern int ObjCargados;
extern Configuracion *Config;
extern Messages *SMS;
extern void *VPointer;
extern volatile int close_button_pressed;

extern void IndicadorCarga (DATAFILE *dat_obj);
extern void close_button_handler(void);

class Hardware
{
	public:
		Hardware();
		~Hardware();
		void Indicador ();
		int PreConfig (int argc, char *argv[]);
        int InitGraph ();
		int Init ();
		int SelectLang ();
		void ReInit ();
		void DeInit ();
		void Unload_Sprites();
        int LoadDF1 ();
        int LoadDF2 ();
        int LoadDF3 ();
        void LocateBuffer (int Ancho = ANCHOPANTALLA, int Alto = ALTOPANTALLA);
		const int VerTecla (int Sentido);
		bool KeyPressed ();
		void GrabarTecla (int Sentido, int Tecla);
        int Tecla (bool Menu = false);
        int VerTecla ();
        void EsperaTecla ();
        void Rest(int Ms);
        void TextOut (const char *s, int x,int y, int c1, int c2 = -1, int c3 = -1, int b1 = -1, int b2 = -1, int b3 = -1);
        void TextOut_aa (const char *s, int x,int y, int RI, int GI, int BI, int RB = -1, int GB = -1, int BB = -1, void *Bitmap = NULL);
        void TextOutCentre (const char *s,int y, int color, int fondo=-1);
        void TextOutCentre_aa (const char *s,int y, int RI, int GI, int BI, int RB = -1, int GB = -1, int BB = -1, void *Bitmap = NULL);
        void TextOutCentre_ex (const char *s,int x, int y, int color, int fondo=-1);
        void TextOutCentre_aa_ex (const char *s,int x, int y, int RI, int GI, int BI, int RB = -1, int GB = -1, int BB = -1);
        //int TextOut_SC (const char *Str, int x, int y, int xf, int R, int G, int B, int Length = -1, int RB = -1, int GB = -1, int BB = -1);
        void TextoPartido (const char *s, int x, int y, int R1, int G1, int B1, int R2, int G2, int B2);
        void TextNum (int Num, int x, int y, int color, int fondo = -1);
        int SetFont_af (const char *s1, int points);
        void DelFont_af ();
        int ResizeFont_af (int Points);
        void SetFont (const char *s1, const char *s2);
        void DelFont ();
        void cls (int color);
        void DrawFondoBuffer (int x = 0, int y = 0, int LongX = ANCHOPANTALLA, int LongY = ALTOPANTALLA, int xo = 0, int yo = 0);
        void VuelcaBuffer ();
        void RectFill (int x1, int y1, int x2, int y2, int color);
        void RectFill_ex (int x1, int y1, int x2, int y2, int color);
        void Circle_cut (int x, int y, int rad);
        int TextHeight ();
        int TextHeight_af ();
        int TextLength (const char *String);
        int TextLength_af (const char *String);
        void ClearKeyBuf ();
        void SetFondo (int NumFondo);
        void SetFondo_ex (int NumFondo);
        void SetFondo (void* Backgrd);
        void CopyFondo ();
        void ClrFondo (int Color);
        void* VerFondo();
        void Dibuja2x (int x, int y, void* Spr);
        void Borra (int x, int y, int LongX, int LongY, int xm = 0, int ym = 0);
        void Dibuja (int x, int y, void *Spr, void *BitMap = NULL);
        void DibujaCentr (void *Spr, int x = -1, int y = -1);
        void Dibuja (const char* File, const char* Obj, int x, int y, void *BitMap = NULL);
        void Dibuja2x (const char* File, const char* Obj, int x, int y);
        void DibujaCentr (const char* File, const char* Obj, int x=-1, int y=-1);
        void DibujaHalo (int x, int y, void *Spr, int Color, bool Suelo);
        void DibujaNegro (int x, int y, void *Spr);
        void Dibuja_Outline (const char *File, const char *Obj, int x, int y, int w);
        void Dibuja_Trans (int x, int y, void *Spr, int Trans, bool Force = false);
        void Dibuja_TransAdd (int x, int y, void *Spr, int Trans, bool Force = false);
        void Dibuja_Trans  (int x, int y, int Trans, const char *File, const char *Obj, bool Force = false);
        void Dibuja_TransAdd (int x, int y, int Trans, const char *File, const char *Obj, bool Force = false);
        void Borra_Sombra (int x, int y, int LongX, int LongY);
        void Dibuja_Sombra (int x, int y, void *Spr);
        void Vuelca_Sombra(int NumHab, int x = -1, int y = -1, int Transp = 70);
        bool Esc ();
        void Fundido (int Sentido, int Vel = 10, bool Sound = false);
        void FundidoBM (int Vel = 10);
        void SolidMode ();
        void TransMode (int n);
        void Vela (int Trans, int X = 0, int Y = 0, int W = ANCHOPANTALLA, int H = ALTOPANTALLA);
        void Ilumina (int Trans, int X = 0, int Y = 0, int W = ANCHOPANTALLA, int H = ALTOPANTALLA);
        void Desatura(int Transp = 255);
        void* CargaSprite (int Descrip);
        void* CargaBitMap (int Descrip);
        void* CargaScreen (int Screen);
        char* VerVersion ();
        int ScreenShot();
        int MapLoadInit();
        unsigned char MapLoadChar();
        int MapLoadClose ();
        void MueveLava ();
        void HeatDist ();
        char* NombreTecla (int Tecla);
        char *Input (int x, int y, int Length, bool Centr, int R, int G, int B, int RB, int GB, int BB);
        void *DatSFx ();
        void Persiana (int Sentido);
        void Persiana2 (int Sentido);
        void Mamporro (int Cuanto);
        void Terremoto (int Cuanto, bool cls = true);
        void InitJoy ();
        void Overlay (int R, int G, int B, int Factor);
        void DrawRotate (void *Sprite, int x, int y, int angle, float scale = 2.0);
        void Blit (void *Source, void *BitMap, int x, int y, int dx, int dy, int w, int h);

     private:
        BITMAP *Buffer, *Fondo, *Sombras, *SombrasRec, *Linea, *ScanL, *Buffer2;
        ALFONT_FONT *Fuente;
        ALFONT_FONT *FuenteTam [20];
        FONT *MyFont, *OldFont;
        DATAFILE *SpritesBM, *BitMapsBM, *ScreensBM, *MapaDat, *SFx, *DataP;
        unsigned char *Mapa;
        int PuntMapa;
        char Version [10], Temp[256];
        int SpeccyColor [8];
        int ModoGraf;
        int Izquierda, Derecha, Arriba, Abajo, Fuego, Selec, Abort;
        int PuntoLuzX, PuntoLuzY;
        int BufMargX, BufMargY;
        int ActScreenShot, Xor, Yor, AnchoPantalla, AltoPantalla;
        int ContHD, HDDesplC, HDDespl [64][64];
        int Mamporrazo, MObjetivo, Terremot;
        bool SueltaTecla [4];
        bool FontCambiado, Iniciado;
        void ScanLines (int Trans = 32);
        int ValidaVarSombra (int &Variable, int Profundo);
        void RecortaSombra (int x1, int y1, int x2, int y2, int Profundo);
        void BorraSombra (int x1, int y1, int x2, int y2);
        void SetPuntoLuz (int x = (TILESANCHO * TAMTILE) / 2, int y = 0);
        void RecortesSombras (int x, int y, int NumHab);
        void TextSLang (const char *Texto, int x, int y, bool Center = true);
};

#endif

