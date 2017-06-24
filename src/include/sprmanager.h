#ifndef SPRMANAGER_H
#define SPRMANAGER_H

#include "constantes.h"
#include "configuracion.h"
#include "hardware.h"

extern const int Tamano;
extern Configuracion *Config;
extern Hardware *H;
extern VariablesJuego VJ;

const int AltoMariano = 64;
const int AnchoMariano = 52;
const int AnchoMarianoAstr = 56;
const int AltoCapitan = 68;
const int AnchoCapitan = 60;

const int EstMariano = 12;
const int AnimMariano = 7;

const int EstCapitan = 15;
const int AnimCapitan = 3;

const int FMariano [] = { 19, 1, 4, 5, 6, 6, 3, 2, 5 , 11, 17, 16};
const int FCapitan [] = { 20, 3, 7, 4, 5, 5, 5, 4, 5 , 12, 17, 2, 4, 7, 17};

const int FAnimMariano [] = { 7 , 19 , 63 , 10, 8, 24, 15};
const int FAnimCapitan [] = { 13, 11 , 12};

const int NumSpr = 64;

const int AnchoSpr [NumSpr] = {  32,         26,              26,           60,      32,      68,               46,         52,  10,     64,
                                 58,         28,              46,           34,      24,      34,              100,         76, 122,     70,
                                 32,         46,              54,           54,       1,      46,               52,         38,  28,     46,
                                 42,         84,              46,           58,      66,      42,               54,         36,  48,     60,
                                 54,         42,              82,          216,      20,      40,               28,         30,  22,     70,
                                 60,         54,             134,           70,      80,     146,               66,         50,  42,     32,
                                 14,         54,             120,           96};

const int AltoSpr [NumSpr]  = { 116,         46,              46,           46,      42,      60,               78,         50,  30,     34,
                                 16,         34,              32,           50,      92,      32,               84,         16,  44,     70,
                                 30,         64,              66,           66,       1,      56,               64,        110,  28,     46,
                                 58,         66,              24,           98,      56,      58,               44,         38,  78,    120,
                                 44,         46,              92,           40,       1,     272,              142,          6,  72,     52,
                                 58,         66,              84,           80,      60,     132,               54,         96,  30,     32,
                                 14,         84,              68,           108};

const int EstSpr [NumSpr] =   {   1,          1,               1,            1,       2,       1,                1,          1,   1,      1,
                                  1,          1,               1,            2,       1,       1,                1,          1,   1,      5,
                                  1,          3,               1,            3,       1,       5,                6,          1,   1,      1,
                                  4,          3,               1,            1,       2,       3,                2,          2,   1,      3,
                                  2,          3,               4,            2,       1,       1,                1,          1,   1,      7,
                                  4,          5,               5,            5,       3,       4,                3,          1,   1,      3,
                                  1,          3,               7,            3};

const int ColSpr [NumSpr] =   {  0,           0,               0,            1,       0,       0,                0,          1,   0,       1,
                                 0,           0,               0,            0,       0,       1,                1,          1,   0,       1,
                                 0,           1,               0,            1,       0,       1,                1,          0,   1,       0,
                                 0,           1,               1,            0,       1,       1,                1,          1,   0,       1,
                                 1,           1,               1,            1,       1,       0,                0,          0,   0,       1,
                                 1,           1,               1,            1,       1,       1,                1,          0,   1,       0,
                                 0,           1,               1,            0};

const int TranspSpr [NumSpr] ={  0,           0,               0,            0,       0,       0,                0,          0,   0,       0,
                                 0,           0,               0,            0,       0,       0,                0,          0,   0,       0,
                                 0,           0,               0,            0,       0,       0,                0,          0,   0,       0,
                                 0,           0,               0,            0,       0,       0,                0,          0,   0,       0,
                                 0,           0,               0,            2,       0,       0,                0,          0,   0,       0,
                                 0,           0,               0,            0,       0,       0,                0,          0,   0,       0,
                                 0,           0,               0,            0};

const int SentSpr [NumSpr] =  {  1,           2,               1,            1,       1,       1,                1,          1,   1,       1,
                                 1,           1,               1,            1,       1,       1,                1,          1,   1,       2,
                                 1,           2,               1,            2,       1,       1,                2,          1,   1,       1,
                                 2,           2,               1,            1,       2,       1,                1,          1,   1,       2,
                                 1,           2,               2,            1,       1,       1,                1,          2,   1,       2,
                                 2,           2,               2,            2,       2,       2,                2,          1,   2,       2,
                                 1,           2,               2,            1};

const int FSpr [] =           {   1,          1,               1,            1,       5,5,     1,                 1,          1,   1,       1,
                                  1,          1,               1,           1,1,       1,      10,                1,          1,   1,  16,1,2,6,17,
                                  1,       16,5,26,            1,         20,8,23,     1,   6,3,4,3,12,   22,7,11,13,15,52,   1,   4,       9,
                             12,13,4,12,   6,12,11,            4,            1,      20,17,   13,11,10,         12,17,       6,9,  1,    7,15,13,
                                12,11,      5,6,9,         5,5,12,12,       9,1,       1,      1,                 1,         14,  47,    5,2,3,1,4,8,18,
                               5,5,5,6,  20,1,7,7,32,    27,7,7,14,16,   7,7,9,7,8, 8,7,28, 3,6,9,40,          4,4,44,       17,   1,    11,11,11,
                                  7,       20,7,32,    1,1,10,7,12,6,11, 25,23,20};

const int NumSprOt = 47;

const int AnchoSprOt [NumSprOt] = { 120,     64,             101,              120,               24,     62,            70,        34,       54,                    54,
                                     54,     40,              32,               32,               32,     32,            32,        32,       32,                    32,
                                    346,    346,             414,               66,               66,     18,            14,        58,       14,                    42,
                                    50,      54,              54,               54,               20,     22,           256,       256,      198,                    10,
                                    32,      28,              96,              120,               10,      8,            66};

const int AltoSprOt [NumSprOt]  = { 120,    102,             102,              120,               24,     20,            70,        32,       66,                    66,
                                     66,     40,              30,               30,               30,     30,            30,        30,       30,                    30,
                                    158,    158,             158,               76,               76,     16,            34,        98,       14,                    58,
                                     26,     66,              66,               66,               20,     22,            14,        14,       98,                    10,
                                     32,    272,             108,               68,               10,      6,            76};

const int EstSprOt [NumSprOt] =   {   1,      1,               1,                1,                1,      6,             5,         1,        7,                     7,
                                      7,      7,               3,                3,                3,      3,             3,         3,        3,                     3,
                                      2,      2,               1,                3,                2,      1,             1,         1,        2,                     3,
                                      1,      3,               3,                3,                3,      1,             1,         1,        1,                     1,
                                      2,      1,               8,                9,                2,      1,             1};

const int TranspSprOt[NumSprOt] = {   1,      1,               1,                1,                0,      2,             0,         1,        0,                     0,
                                      0,      0,               0,                0,                0,      0,             0,         0,        0,                     0,
                                      0,      2,               1,                0,                2,      0,             0,         2,        0,                     0,
                                      0,      0,               0,                0,                2,      0,             0,         0,        0,                     0,
                                      0,      0,               2,                0,                0,      0,             1};

const int ColSprOt[NumSprOt] =    {   0,      0,               0,                0,                1,      0,             1,         0,        1,                     1,
                                      1,      1,               1,                1,                1,      1,             1,         1,        1,                     1,
                                      0,      0,               0,                1,                0,      1,             1,         0,        1,                     1,
                                      1,      1,               1,                1,                1,      1,             0,         0,        0,                     1,
                                      1,      1,               0,                1,                0,      1,             0};

const int SentSprOt[NumSprOt] =   {   1,      2,               1,                1,                1,      1,             2,         1,        2,                     2,
                                      2,      1,               1,                1,                1,      1,             1,         1,        1,                     1,
                                      1,      1,               1,                1,                1,      1,             1,         1,        2,                     1,
                                      1,      2,               2,                2,                2,      1,             1,         1,        1,                     1,
                                      1,      1,               1,                2,                1,      1,             2};

const int FSprOt [] =             {   2,      2,               2,                2,                8,   1,1,1,1,1,1, 16,1,2,6,17,  10,   20,8,6,3,2,24,26,    20,8,6,3,2,24,26,
                       20,8,6,3,2,24,26,  12,4,14,1,1,1,2,  10,1,9,           10,1,9,           10,1,9,    10,1,9,      10,1,9,  10,1,9,    10,1,9,                10,1,9,
                                    4,2,     4,2,              1,             5,5,15,             3,9,        1,           1,        1,      12,5,                13,11,10,
                                    14,    20,7,22,        20,7,22,           20,7,22,           1,1,1,       6,           1,        1,        1,                     1,
                                    8,9,      1,        1,1,1,1,1,1,1,1, 2,2,22,2,18,6,12,1,67,  16,16,       1,           2};

const int NumSprMarc = 10;

const int AnchoSprMarc [NumSprMarc] = { 512,  24,  24,      88,      82,  82, 118, 72,          72,              72};
const int AltoSprMarc [NumSprMarc]  = { 120,  32,  16,      56,      32,  32,  90, 40,          58,              58};
const int TranspSprMarc[NumSprMarc] = {   0,   0,   0,       0,       2,   2,   0,  2,           0,               1};
const int SentSprMarc [NumSprMarc]  = {   1,   1,   1,       1,       1,   1,   1,  1,           1,               1};

const int EstSprMarc [NumSprMarc] =   {   1,   2,   1,       5,       1,   1,   1,  1,           7,               1};
const int FSprMarc [] =               {   1,  1,1,  1,   1,1,4,1,1,   5,   1,   1,  6,    1,19,13,4,13,20,20,     4};

const int NumSprMenu = 12;

const int AnchoSprMenu [NumSprMenu] = {  54,  82,  82,  80,  48,  30,  20,  40,  86, 120,
                                         48, 192};
const int AltoSprMenu [NumSprMenu]  = {  64,  84,  84,  82,  62,  30,  20,  40, 372,  66,
                                         60,  68};
const int SentSprMenu [NumSprMenu]  = {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
                                          1,   1};

const int EstSprMenu [NumSprMenu] =   {   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,
                                          1,   5};
const int TranspSprMenu[NumSprMenu] = {   0,   0,   0,   0,   0,   0,   0,   0,   2,   0,
                                          0,   0};
const int FSprMenu [] =               {  10,   6,   6,   6,   4,   6,   6,   5,  18,   4,
                                          4,   10,10,10,10,10};

const int PagesTiles = 1;

class SprManager
{
	public:
		SprManager();
		~SprManager();
		void InitGraph ();
		void InitMasks ();
		void CreateGraph ();
		void DeleteGraph ();
		BITMAP * Sprite (int Tipo, int Sentido = 0, int Estado = 0, int Fotograma = 0, int Animacion = 0);
		mask * Mask (int Tipo, int Sentido = 0, int Estado = 0, int Fotograma = 0, int Animacion = 0);
	private:
        int Trans,n,m,o,p, Rec1, Rec2;
        BITMAP **Mariano [EstMariano][2]   , **MarianoAnim[AnimMariano][2],
               **MarianoAstr [EstMariano][2],**MarianoAstrAnim [AnimMariano][2],
               **Escafandra [EstMariano][2], **EscafandraAnim [AnimMariano][2],
               **Capitan [EstCapitan][2]   , **CapitanAnim[AnimCapitan][2],
               ****Sprites [NumSpr]        , ****SpritesOt [NumSprOt],
               ****SpritesMarc [NumSprMarc], ****SpritesMenu [NumSprMenu],
               *Tile[PagesTiles][256];

        mask **MMariano [EstMariano][2]    , **MMarianoAnim[AnimMariano][2],
             **MMarianoAstr [EstMariano][2], **MMarianoAstrAnim [AnimMariano][2],
             **MCapitan [EstCapitan][2]    , **MCapitanAnim[AnimCapitan][2],
             ****MSprites [NumSpr]         , ****MSpritesOt [NumSprOt],
             ****MSpritesMarc [NumSprMarc] , ****MSpritesMenu [NumSprMenu],
             *MTile[PagesTiles][256];

        BITMAP *TileRet, *TileTemp;

        mask * CreaMascara (BITMAP *BitMap, int Fotogr, bool PixMap = true);
        void InitMascara (BITMAP *BitMap, mask *Mascara, int Transp, int YOrig = 0, int YDest = 0);
        int DestrMascara (mask *Mascara);
        int AjustaEstado (int &Tipo, int &Sentido, int &Estado, int &Fotograma);
        void CreaGrafico (const int &NumSpr, BITMAP *****Sprite, const int *Estados, const int *Sentidos, const int *Fotogramas, const int *Ancho, const int *Alto);
        void CreaMascara (const int &NumSpr, mask *****Mascara, BITMAP *****Sprite, const int *Estados, const int *Sentidos, const int *Fotogramas, const int *Colisionable = NULL);
        void InitGrafico (const int &NumSpr, BITMAP *****Sprite, const int *Estados, const int *Sentidos, const int *Fotogramas, const int *Ancho, const int *Alto, int BMSpr);
        void InitMascara (const int &NumSpr, BITMAP *****Sprite, mask *****Mascara, const int *Estados, const int *Sentidos, const int *Fotogramas, const int *Transp);
        void DestrGrafico (const int &NumSpr, BITMAP *****Sprite, const int *Estados, const int *Sentidos, const int *Fotogramas);
        void DestrMascara (const int &NumSpr, mask *****Mascara, const int *Estados, const int *Sentidos, const int *Fotogramas);
        inline long int MGetPixel32(BITMAP *Bitmap, int x, int y);
        inline short int MGetPixel16(BITMAP *Bitmap, int x, int y);
};

#endif // SPRMANAGER_H
