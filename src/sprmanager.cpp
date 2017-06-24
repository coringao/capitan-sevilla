#include "sprmanager.h"

SprManager::SprManager()
{
    CreateGraph();

    LogWrite ("Reservando memoria para máscaras...");

// Creación de máscaras **************************************************

    for (n = 0; n < PagesTiles; n++)
        for (m = 0; m < 256; m++)
            MTile[n][m] = CreaMascara (Tile[n][m],1);

    for (n = 0; n < EstMariano; n++)
        for (m = 0; m < 2; m++) {
            MMariano[n][m] = new mask*[FMariano[n]];
            MMarianoAstr[n][m] = new mask*[FMariano[n]];
            for (o = 0; o < FMariano[n]; o++) {
                MMariano[n][m][o] = CreaMascara (Mariano[n][m][o], FMariano[n]);
                MMarianoAstr[n][m][o] = CreaMascara (MarianoAstr[n][m][o], FMariano[n]);
                }
        }

    for (n = 0; n < AnimMariano; n++)
        for (m = 0; m < 2; m++) {
            MMarianoAnim[n][m] = new mask*[FAnimMariano[n]];
            MMarianoAstrAnim[n][m] = new mask*[FAnimMariano[n]];
            for (o = 0; o < FAnimMariano[n]; o++) {
                MMarianoAnim[n][m][o] = CreaMascara (MarianoAnim[n][m][o], FAnimMariano[n]);
                MMarianoAstrAnim[n][m][o] = CreaMascara (MarianoAstrAnim[n][m][o], FAnimMariano[n]);
                }
        }

    for (n = 0; n < EstCapitan; n++)
        for (m = 0; m < 2; m++) {
            MCapitan[n][m] = new mask*[FCapitan[n]];
            for (o = 0; o < FCapitan[n]; o++)
                MCapitan[n][m][o] = CreaMascara (Capitan[n][m][o], FCapitan[n]);
        }

    for (n = 0; n < AnimCapitan; n++)
        for (m = 0; m < 2; m++) {
            MCapitanAnim[n][m] = new mask*[FAnimCapitan[n]];
            for (o = 0; o < FAnimCapitan[n]; o++)
                MCapitanAnim[n][m][o] = CreaMascara (CapitanAnim[n][m][o], FAnimCapitan[n]);
        }

    CreaMascara (NumSpr, MSprites, Sprites, EstSpr, SentSpr, FSpr, ColSpr);
    CreaMascara (NumSprOt, MSpritesOt, SpritesOt, EstSprOt, SentSprOt, FSprOt, ColSprOt);
    CreaMascara (NumSprMarc, MSpritesMarc, SpritesMarc, EstSprMarc, SentSprMarc, FSprMarc);
    CreaMascara (NumSprMenu, MSpritesMenu, SpritesMenu, EstSprMenu, SentSprMenu, FSprMenu);

    LogWrite ("OK.\n");
}

SprManager::~SprManager()
{
   DeleteGraph();

// Destrucción de máscaras *****************************
    LogWrite ("Devolviendo memoria de Máscaras...");

    for (n = 0; n < PagesTiles; n++)
        for (m = 0; m < 256; m++)
            DestrMascara (MTile[n][m]);

    for (n = 0; n < EstMariano; n++)
        for (m = 0; m < 2; m++) {
            for (o = 0; o < FMariano[n]; o++) {
                DestrMascara (MMariano[n][m][o]);
                DestrMascara (MMarianoAstr[n][m][o]);
                }
            delete [] MMariano[n][m];
            delete [] MMarianoAstr[n][m];
        }

    for (n = 0; n < AnimMariano; n++)
        for (m = 0; m < 2; m++) {
            for (o = 0; o < FAnimMariano[n]; o++) {
                DestrMascara (MMarianoAnim[n][m][o]);
                DestrMascara (MMarianoAstrAnim[n][m][o]);
                }
            delete [] MMarianoAnim[n][m];
            delete [] MMarianoAstrAnim[n][m];
        }

    for (n = 0; n < EstCapitan; n++)
        for (m = 0; m < 2; m++) {
            for (o = 0; o < FCapitan[n]; o++)
                DestrMascara (MCapitan[n][m][o]);
            delete [] MCapitan[n][m];
        }

    for (n = 0; n < AnimCapitan; n++)
        for (m = 0; m < 2; m++) {
            for (o = 0; o < FAnimCapitan[n]; o++)
                DestrMascara (MCapitanAnim[n][m][o]);
            delete [] MCapitanAnim[n][m];
        }

    DestrMascara (NumSpr, MSprites, EstSpr, SentSpr, FSpr);
    DestrMascara (NumSprOt, MSpritesOt, EstSprOt, SentSprOt, FSprOt);
    DestrMascara (NumSprMarc, MSpritesMarc, EstSprMarc, SentSprMarc, FSprMarc);
    DestrMascara (NumSprMenu, MSpritesMenu, EstSprMenu, SentSprMenu, FSprMenu);

// Otras acciones del destructor **********************

    LogWrite ("OK.\n");

}

void SprManager::CreateGraph()
{
    LogWrite ("Reservando memoria para gráficos...");

// Creación de BitMaps *********************************************************

    TileTemp = create_bitmap (TAMTILE, TAMTILE);
    TileRet = create_bitmap (TAMTILE, TAMTILE);

    for (n = 0; n < PagesTiles; n++)
        for (m = 0; m < 256; m++)
            Tile[n][m] = create_bitmap (TAMTILE, TAMTILE);

    for (n = 0; n < EstMariano; n++)
        for (m = 0; m < 2; m++) {
            Mariano[n][m] = new BITMAP*[FMariano[n]];
            MarianoAstr[n][m] = new BITMAP*[FMariano[n]];
            Escafandra [n][m] = new BITMAP*[FMariano[n]];
            for (o = 0; o < FMariano[n]; o++) {
                Mariano[n][m][o] = create_bitmap (AnchoMariano, AltoMariano);
                MarianoAstr[n][m][o] = create_bitmap (AnchoMarianoAstr, AltoMariano);
                Escafandra[n][m][o] = create_bitmap (AnchoMarianoAstr, AltoMariano);
                }
        }

    for (n = 0; n < AnimMariano; n++)
        for (m = 0; m < 2; m++) {
            MarianoAnim[n][m] = new BITMAP*[FAnimMariano[n]];
            MarianoAstrAnim[n][m] = new BITMAP*[FAnimMariano[n]];
            EscafandraAnim [n][m] = new BITMAP*[FAnimMariano[n]];
            for (o = 0; o < FAnimMariano[n]; o++) {
                MarianoAnim[n][m][o] = create_bitmap (AnchoMariano, AltoMariano);
                MarianoAstrAnim[n][m][o] = create_bitmap (AnchoMarianoAstr, AltoMariano);
                EscafandraAnim[n][m][o] = create_bitmap (AnchoMarianoAstr, AltoMariano);
                }
        }

    for (n = 0; n < EstCapitan; n++)
        for (m = 0; m < 2; m++) {
            Capitan[n][m] = new BITMAP*[FCapitan[n]];
            for (o = 0; o < FCapitan[n]; o++)
                Capitan[n][m][o] = create_bitmap (AnchoCapitan, AltoCapitan);
        }

    for (n = 0; n < AnimCapitan; n++)
        for (m = 0; m < 2; m++) {
            CapitanAnim[n][m] = new BITMAP*[FAnimCapitan[n]];
            for (o = 0; o < FAnimCapitan[n]; o++)
                CapitanAnim[n][m][o] = create_bitmap (AnchoCapitan, AltoCapitan);
        }

    CreaGrafico (NumSpr, Sprites, EstSpr, SentSpr, FSpr, AnchoSpr, AltoSpr);
    CreaGrafico (NumSprOt, SpritesOt, EstSprOt, SentSprOt, FSprOt, AnchoSprOt, AltoSprOt);
    CreaGrafico (NumSprMarc, SpritesMarc, EstSprMarc, SentSprMarc, FSprMarc, AnchoSprMarc, AltoSprMarc);
    CreaGrafico (NumSprMenu, SpritesMenu, EstSprMenu, SentSprMenu, FSprMenu, AnchoSprMenu, AltoSprMenu);

    LogWrite ("OK.\n");
}


void SprManager::DeleteGraph()
{
    LogWrite ("Devolviendo memoria reservada para gráficos...");
// Destrucción de BitMaps

    destroy_bitmap (TileTemp);
    destroy_bitmap (TileRet);

    for (n = 0; n < PagesTiles; n++)
        for (m = 0; m < 256; m++)
            destroy_bitmap (Tile[n][m]);

    for (n = 0; n < EstMariano; n++)
        for (m = 0; m < 2; m++) {
            for (o = 0; o < FMariano[n]; o++) {
                destroy_bitmap (Mariano[n][m][o]);
                destroy_bitmap (MarianoAstr[n][m][o]);
                destroy_bitmap (Escafandra[n][m][o]);
                }
            delete [] Mariano[n][m];
            delete [] MarianoAstr[n][m];
            delete [] Escafandra[n][m];
        }

    for (n = 0; n < AnimMariano; n++)
        for (m = 0; m < 2; m++) {
            for (o = 0; o < FAnimMariano[n]; o++) {
                destroy_bitmap (MarianoAnim[n][m][o]);
                destroy_bitmap (MarianoAstrAnim[n][m][o]);
                destroy_bitmap (EscafandraAnim[n][m][o]);
                }
            delete [] MarianoAnim[n][m];
            delete [] MarianoAstrAnim[n][m];
            delete [] EscafandraAnim[n][m];
        }

    for (n = 0; n < EstCapitan; n++)
        for (m = 0; m < 2; m++) {
            for (o = 0; o < FCapitan[n]; o++)
                destroy_bitmap (Capitan[n][m][o]);
            delete [] Capitan[n][m];
        }

    for (n = 0; n < AnimCapitan; n++)
        for (m = 0; m < 2; m++) {
            for (o = 0; o < FAnimCapitan[n]; o++)
                destroy_bitmap (CapitanAnim[n][m][o]);
            delete [] CapitanAnim[n][m];
        }

    DestrGrafico (NumSpr, Sprites, EstSpr, SentSpr, FSpr);
    DestrGrafico (NumSprOt, SpritesOt, EstSprOt, SentSprOt, FSprOt);
    DestrGrafico (NumSprMarc, SpritesMarc, EstSprMarc, SentSprMarc, FSprMarc);
    DestrGrafico (NumSprMenu, SpritesMenu, EstSprMenu, SentSprMenu, FSprMenu);

    LogWrite ("OK.\n");
}

void SprManager::InitGraph () {

     LogWrite ("Inicializando gráficos...");

     Trans = makecol (255,0,255);

// Inicialización de BitMaps ************************************************

     // Mariano

     BITMAP *Origen = (BITMAP*)H->CargaSprite(DFSP_MAR_SPR);
     int PixelOrigen = 1;

     for (n = 0; n < EstMariano; n++){
         for (m = 0; m < FMariano[n]; m++) {
             blit (Origen, Mariano[n][0][m], 1 + (m * (AnchoMariano+1)), PixelOrigen, 0, 0,
                  AnchoMariano, AltoMariano);
             clear_to_color (Mariano[n][1][m], Trans);
             draw_sprite_h_flip (Mariano[n][1][m],Mariano[n][0][m],0,0);
         }
         ++PixelOrigen += AltoMariano;
     }


     Origen = (BITMAP*)H->CargaSprite(DFSP_MAR_ANI);
     PixelOrigen = 1;

     for (n = 0; n < AnimMariano; n++){
         for (m = 0; m < FAnimMariano[n]; m++) {
             blit (Origen, MarianoAnim[n][0][m], 1 + (m * (AnchoMariano+1)), PixelOrigen, 0, 0,
                  AnchoMariano, AltoMariano);
             clear_to_color (MarianoAnim[n][1][m], Trans);
             draw_sprite_h_flip (MarianoAnim[n][1][m],MarianoAnim[n][0][m],0,0);
         }
         ++PixelOrigen += AltoMariano;
     }


     Origen = (BITMAP*)H->CargaSprite(DFSP_MAS_SPR);
     PixelOrigen = 1;

     for (n = 0; n < EstMariano; n++){
         for (m = 0; m < FMariano[n]; m++) {
             blit (Origen, MarianoAstr[n][0][m], 1 + (m * (AnchoMarianoAstr+1)), PixelOrigen, 0, 0,
                  AnchoMarianoAstr, AltoMariano);
             clear_to_color (MarianoAstr[n][1][m], Trans);
             draw_sprite_h_flip (MarianoAstr[n][1][m],MarianoAstr[n][0][m],0,0);
         }
         ++PixelOrigen += AltoMariano;
     }

     Origen = (BITMAP*)H->CargaSprite(DFSP_ESC_SPR);
     PixelOrigen = 1;

     for (n = 0; n < EstMariano; n++){
         for (m = 0; m < FMariano[n]; m++) {
             blit (Origen, Escafandra[n][0][m], 1 + (m * (AnchoMarianoAstr+1)), PixelOrigen, 0, 0,
                  AnchoMarianoAstr, AltoMariano);
             clear_to_color (Escafandra[n][1][m], Trans);
             draw_sprite_h_flip (Escafandra[n][1][m],Escafandra[n][0][m],0,0);
         }
         ++PixelOrigen += AltoMariano;
     }

     Origen = (BITMAP*)H->CargaSprite(DFSP_MAS_ANI);
     PixelOrigen = 1;

     for (n = 0; n < AnimMariano; n++){
         for (m = 0; m < FAnimMariano[n]; m++) {
             blit (Origen, MarianoAstrAnim[n][0][m], 1 + (m * (AnchoMarianoAstr+1)), PixelOrigen, 0, 0,
                  AnchoMarianoAstr, AltoMariano);
             clear_to_color (MarianoAstrAnim[n][1][m], Trans);
             draw_sprite_h_flip (MarianoAstrAnim[n][1][m],MarianoAstrAnim[n][0][m],0,0);
         }
         ++PixelOrigen += AltoMariano;
     }

     Origen = (BITMAP*)H->CargaSprite(DFSP_ESC_ANI);
     PixelOrigen = 1;

     for (n = 0; n < AnimMariano; n++){
         for (m = 0; m < FAnimMariano[n]; m++) {
             blit (Origen, EscafandraAnim[n][0][m], 1 + (m * (AnchoMarianoAstr+1)), PixelOrigen, 0, 0,
                  AnchoMarianoAstr, AltoMariano);
             clear_to_color (EscafandraAnim[n][1][m], Trans);
             draw_sprite_h_flip (EscafandraAnim[n][1][m],EscafandraAnim[n][0][m],0,0);
         }
         ++PixelOrigen += AltoMariano;
     }

     // Capitan

     Origen = (BITMAP*)H->CargaSprite(DFSP_CAP_SPR);
     PixelOrigen = 1;

     for (n = 0; n < EstCapitan; n++){
         for (m = 0; m < FCapitan[n]; m++) {
             blit (Origen, Capitan[n][0][m], 1 + (m * (AnchoCapitan+1)), PixelOrigen, 0, 0,
                   AnchoCapitan, AltoCapitan);
             clear_to_color (Capitan[n][1][m], Trans);
             draw_sprite_h_flip (Capitan[n][1][m],Capitan[n][0][m],0,0);
         }
         ++PixelOrigen += AltoCapitan;
     }

     Origen = (BITMAP*)H->CargaSprite(DFSP_CAP_ANI);
     PixelOrigen = 1;

     for (n = 0; n < AnimCapitan; n++){
         for (m = 0; m < FAnimCapitan[n]; m++) {
             blit (Origen, CapitanAnim[n][0][m], 1 + (m * (AnchoCapitan+1)), PixelOrigen, 0, 0,
                  AnchoCapitan, AltoCapitan);
             clear_to_color (CapitanAnim[n][1][m], Trans);
             draw_sprite_h_flip (CapitanAnim[n][1][m],CapitanAnim[n][0][m],0,0);
         }
         ++PixelOrigen += AltoCapitan;
     }

     InitGrafico (NumSpr, Sprites, EstSpr, SentSpr, FSpr, AnchoSpr, AltoSpr, DFSP_SPR_000);
     InitGrafico (NumSprOt, SpritesOt, EstSprOt, SentSprOt, FSprOt, AnchoSprOt, AltoSprOt, DFSP_SPO_000);
     InitGrafico (NumSprMarc, SpritesMarc, EstSprMarc, SentSprMarc, FSprMarc, AnchoSprMarc, AltoSprMarc, DFSP_SCO_000);
     InitGrafico (NumSprMenu, SpritesMenu, EstSprMenu, SentSprMenu, FSprMenu, AnchoSprMenu, AltoSprMenu, DFSP_MEN_000);

     // Inicialización de Tiles

     for (n = 0; n < PagesTiles; n++)
         for (m = 0; m < 16; m++)
             for (o = 0; o < 16; o++)
                 blit ((BITMAP*)H->CargaSprite(DFSP_TIL_000 + n), Tile[n][16*m+o],
                       o + 1 + TAMTILE * o, m + 1 + TAMTILE * m, 0, 0, TAMTILE, TAMTILE);

     H->Unload_Sprites();

     LogWrite ("OK.\n");
}

void SprManager::InitMasks ()
{
    int Cont;
    LogWrite ("\nInicializando máscaras:\n");

// Inicialización de máscaras ***********************************************

    LogWrite ("\nTiles:");

    for (n = 0; n < PagesTiles; n++)
        for (m = 0; m < 256; m++)
            InitMascara (Tile[n][m], MTile[n][m], false);

    LogWrite ("\nJugador:");

    for (n = 0; n < EstMariano; n++)
        for (m = 0; m < 2; m++)
            for (o = 0; o < FMariano[n]; o++) {
                InitMascara (Mariano[n][m][o], MMariano[n][m][o], false);
                InitMascara (MarianoAstr[n][m][o], MMarianoAstr[n][m][o], false);
                }

    for (n = 0; n < AnimMariano; n++)
        for (m = 0; m < 2; m++)
            for (o = 0; o < FAnimMariano[n]; o++) {
                InitMascara (MarianoAnim[n][m][o], MMarianoAnim[n][m][o], false);
                InitMascara (MarianoAstrAnim[n][m][o], MMarianoAstrAnim[n][m][o], false);
                }

    for (n = 0; n < EstCapitan; n++)
        for (m = 0; m < 2; m++)
            for (o = 0; o < FCapitan[n]; o++)
                InitMascara (Capitan[n][m][o], MCapitan[n][m][o], false);

    for (n = 0; n < AnimCapitan; n++)
        for (m = 0; m < 2; m++)
            for (o = 0; o < FAnimCapitan[n]; o++)
                InitMascara (CapitanAnim[n][m][o], MCapitanAnim[n][m][o], false);

    LogWrite ("\nSprites:");

    for (n = 0, Cont = 0; n < NumSpr; n++)
        for (m = 0; m < EstSpr[n]; m++, Cont++)
            for (o = 0; o < SentSpr[n]; o++)
                for (p = 0; p < FSpr[Cont]; p++) {
                    Rec1 = 0;
                    Rec2 = AltoSpr[n];
                    switch (n + FAROLG) {
                           case CONTENEDOR:
                                Rec1 = 2;
                                Rec2 = 3;
                                break;
                           case CVIDRIO:
                                Rec1 = 10;
                                Rec2 = AltoSpr[n] - 21;
                                break;
                           }
                    InitMascara (Sprites[n][m][o][p], MSprites[n][m][o][p], TranspSpr[n], Rec1, Rec2);
                }

    LogWrite ("\nSprOt:");
    InitMascara (NumSprOt, SpritesOt, MSpritesOt, EstSprOt, SentSprOt, FSprOt, TranspSprOt);
    LogWrite ("\nSpritesMarc:");
    InitMascara (NumSprMarc, SpritesMarc, MSpritesMarc, EstSprMarc, SentSprMarc, FSprMarc, TranspSprMarc);
    LogWrite ("\nSpritesMenu:");
    InitMascara (NumSprMenu, SpritesMenu, MSpritesMenu, EstSprMenu, SentSprMenu, FSprMenu, TranspSprMenu);
    LogWrite ("\nOK.\n");

/* //  ******************************************************* Prueba de máscara
     mask *Prueba;
     Prueba = MSprites [43][0][0][0]; //MCapitan[DESPEGANDO][0][0];

     for (n = 0; n < Prueba->LongY; n++)
         for (m = 0; m < Prueba->Words; m++)
             for (o = 0; o < Tamano; o++)
                 if (Prueba->MaskData[n][m] & 0x0001 << o)
                    textout_ex (screen, font, "*",(m * Tamano + o)* 4, 4 * n,
                               makecol(255,255,255),0);
     readkey();//*/
}

void SprManager::CreaGrafico (const int &NumSpr, BITMAP *****Sprite, const int *Estados, const int *Sentidos, const int *Fotogramas, const int *Ancho, const int *Alto)
{
    for (int n = 0, Cont = 0; n < NumSpr; n++) {
        Sprite[n] = new BITMAP***[Estados[n]];
        for (int m = 0; m < Estados[n]; m++, Cont++) {
            Sprite[n][m] = new BITMAP**[Sentidos[n]];
            for (int o = 0; o < Sentidos[n]; o ++) {
                Sprite[n][m][o] = new BITMAP*[Fotogramas[Cont]];
                for (int p = 0; p < Fotogramas[Cont]; p++)
                    Sprite[n][m][o][p] = create_bitmap (Ancho[n], Alto[n]);
                }
            }
        }
}

void SprManager::CreaMascara (const int &NumSpr, mask *****Mascara, BITMAP *****Sprite, const int *Estados, const int *Sentidos, const int *Fotogramas, const int *Colisionable)
{
    bool GenCol = false;
    for (int n = 0, Cont = 0; n < NumSpr; n++) {
        Mascara[n] = new mask***[Estados[n]];
        for (int m = 0; m < Estados[n]; m++, Cont++) {
            Mascara[n][m] = new mask**[Sentidos[n]];
            for (int o = 0; o < Sentidos[n]; o ++) {
                Mascara [n][m][o] = new mask*[Fotogramas[Cont]];
                for (int p = 0; p < Fotogramas[Cont]; p++) {
                    if (Colisionable != NULL)
                       GenCol = Colisionable[n];
                    Mascara [n][m][o][p] = CreaMascara (Sprite[n][m][o][p], Fotogramas [Cont], GenCol);
                    }
                }
            }
        }
}

void SprManager::InitGrafico (const int &NumSpr, BITMAP *****Sprite, const int *Estados, const int *Sentidos, const int *Fotogramas, const int *Ancho, const int *Alto, int BMSpr)
{

     BITMAP *Origen = (BITMAP*)H->CargaSprite(BMSpr);
     int PixelOrigen = 1;
     int Archivo = 0;

     for (int n = 0, Cont = 0; n < NumSpr; n++) {
         if (n / 10 > Archivo) {
            Origen = (BITMAP*)H->CargaSprite(BMSpr + ++Archivo);
            PixelOrigen = 1;
            }
         for (int m = 0; m < Estados[n]; m++, Cont++) {
             for (int o = 0; o < Fotogramas[Cont]; o++) {
                 blit (Origen, Sprite [n][m][0][o], 1 + (o * (Ancho[n] + 1)), PixelOrigen, 0, 0,
                       Ancho[n], Alto[n]);
                 if (Sentidos[n] > 1) {
                    clear_to_color (Sprite [n][m][1][o], Trans);
                    draw_sprite_h_flip (Sprite[n][m][1][o], Sprite[n][m][0][o], 0, 0);
                    }
                 }
             ++PixelOrigen += Alto[n];
             }
         }
}

void SprManager::InitMascara (const int &NumSpr, BITMAP *****Sprite, mask *****Mascara, const int *Estados, const int *Sentidos, const int *Fotogramas, const int *Transp)
{
    for (int n = 0, Cont = 0; n < NumSpr; n++)
        for (int m = 0; m < Estados[n]; m++, Cont++)
            for (int o = 0; o < Sentidos[n]; o++)
                for (int p = 0; p < Fotogramas[Cont]; p++)
                    InitMascara (Sprite[n][m][o][p], Mascara[n][m][o][p], Transp[n]);
}


void SprManager::DestrGrafico (const int &NumSpr, BITMAP *****Sprite, const int *Estados, const int *Sentidos, const int *Fotogramas)
{
    for (int n = 0, Cont = 0; n < NumSpr; n++) {
        for (int m = 0; m < Estados[n]; m++, Cont++) {
            for (int o = 0; o < Sentidos[n]; o ++) {
                for (p = 0; p < Fotogramas[Cont]; p++)
                    destroy_bitmap (Sprite[n][m][o][p]);
                delete [] Sprite [n][m][o];
            }
            delete [] Sprite [n][m];
        }
        delete [] Sprite [n];
    }
}

void SprManager::DestrMascara (const int &NumSpr, mask *****Mascara, const int *Estados, const int *Sentidos, const int *Fotogramas)
{
    for (int n = 0, Cont = 0; n < NumSpr; n++) {
        for (int m = 0; m < Estados[n]; m++, Cont++) {
            for (int o = 0; o < Sentidos[n]; o ++) {
                for (int p = 0; p < Fotogramas[Cont]; p++)
                    DestrMascara (Mascara[n][m][o][p]);
                delete []Mascara [n][m][o];
            }
            delete [] Mascara[n][m];
        }
        delete [] Mascara[n];
    }
}

mask * SprManager::CreaMascara (BITMAP *BitMap, int Fotogr, bool PixMap)
{
      mask *Retorno;
      int b1, b2;
      Retorno = new mask;
      Retorno->LongX = BitMap->w;
      Retorno->LongY = BitMap->h;
      Retorno->Fotog = Fotogr;
      Retorno->Colisiona = PixMap;
      if (!PixMap)
         return Retorno;
      Retorno->Words = Retorno->LongX / Tamano;
      if (Retorno->LongX % Tamano) Retorno->Words += 1;
      Retorno->MaskData = new word*[Retorno->LongY];
      for (b1 = 0; b1 < Retorno->LongY; b1++)
          Retorno->MaskData[b1] = new word [Retorno->Words];
      for (b1 = 0; b1 < Retorno->LongY; b1++)
          for (b2 = 0; b2 < Retorno->Words; b2++)
              Retorno->MaskData[b1][b2] = 0;

      return Retorno;
}

long int SprManager::MGetPixel32(BITMAP *Bitmap, int x, int y)
{
    return ((long int *)Bitmap->line[y])[x];
}

short int SprManager::MGetPixel16(BITMAP *Bitmap, int x, int y)
{
    return ((short int *)Bitmap->line[y])[x];
}

void SprManager::InitMascara (BITMAP *BitMap, mask *Mascara, int Transp, int YOrig, int YDest)
 {
    int b1,b2,b3;
    long int gp;

    Mascara->Transp = Transp;

    if (!Mascara->Colisiona) {
       LogWrite ("^");
       return;
       }

    H->Indicador();

    if (YDest > Mascara->LongY || YDest <= YOrig)
       YDest = Mascara->LongY;

    for (b1 = YOrig; b1 < YDest; b1++)
        for (b2 = 0; b2 < Mascara->Words; b2++)
            for (b3 = 0; b3 < Tamano; b3++) {
/*                if (get_color_depth() == 16)
                   gp = MGetPixel16 (BitMap, (b2 * Tamano) + b3, b1) & 0x0000FFFF;
                else
                   gp = MGetPixel32 (BitMap, (b2 * Tamano) + b3, b1);*/
                gp = getpixel (BitMap, (b2 * Tamano) + 3, b1);
                if (b2 * Tamano + b3 < Mascara->LongX &&
                   (gp != Trans && gp >= 0))
                       Mascara->MaskData[b1][b2] += (1 << b3);
                }
}

int SprManager::DestrMascara (mask *Mascara)
{
    int b1;
    if (!Mascara)
       return -2;
    if (!Mascara->Colisiona)
       return -1;
    for (b1 = 0; b1 < Mascara->LongY; b1++)
        delete[] Mascara->MaskData[b1];
    delete[] Mascara->MaskData;
    delete Mascara;
    if (Mascara) return 1;
    return 0;
}

int SprManager::AjustaEstado (int &Tipo, int &Sentido, int &Estado, int &Fotograma)
{
    if (Tipo >= HALOG && Tipo < BASE && Tipo != OBJVENTANA)
       if (Sentido >= SentSprOt [Tipo - HALOG])
          Sentido = SentSprOt [Tipo - HALOG] - 1;

    if (Tipo >= FAROLG && Tipo < HALOG && Tipo != VENTANA)
       if (Sentido >= SentSpr [Tipo - FAROLG])
          Sentido = SentSpr [Tipo - FAROLG] - 1;

	switch (Tipo) {
        case MARIANO:
             if (Estado == MUERTE) {
                Estado = PARADO;
                Fotograma = 0;
                }
             break;
		case CAMARERO:
		case CAMARSIN:
			switch (Estado) {
                case FRENANDO:
                     Estado = 2;
                     return 1;
				case GIRANDO:
					Estado = 3;
					return 1;
				case MUERTE:
                    Estado = 4;
                    return 1;
				}
			break;
		case VENTANA:
            Tipo = Sentido;
            Sentido = 0;
			switch (Estado) {
				case SALIENDO:
					Estado = 0;
					return 1;
				case METIENDOSE:
                case MUERTE:
					Estado = 2;
					return 1;
				}
			break;
		case OBJVENTANA:
             Estado = Sentido;
             Sentido = 0;
             return 1;
        case CURRO1:
        case CURRO2:
        case CURRO3:
             switch (Estado) {
                    case GIRANDO:
                         Estado = 1;
                         return 1;
                    case CAYENDO:
                         Estado = 2;
                         return 1;
                    case POSTSALTO:
                         Estado = 3;
                         return 1;
                    case LEVANTANDO:
                         Estado = 4;
                         return 1;
                    case MUERTE:
                         Estado = 5;
                         return 1;
                    case NACIENDO:
                         Estado = 6;
                         return 1;
                     }
             break;
        case NEGRO:
        case GANGSTER:
             switch (Estado) {
                    case GIRANDO:
                         Estado = 1;
                         return 1;
                    case MUERTE:
                         Estado = 2;
                         return 1;
                    }
             break;
        case CUBO:
             switch (Estado) {
                    case PRESALTO:
                         Estado = 0;
                         return 1;
                    case SUBIENDO:
                         Estado = 1;
                         return 1;
                    case CAYENDO:
                         Estado = 2;
                         return 1;
                    case POSTSALTO:
                         Estado = 3;
                         return 1;
                    case MUERTE:
                         Estado = 4;
                         return 1;
                    }
             break;
        case MARIANOMUERTO:
             switch (Estado) {
                    case SUBIENDO:
                         Estado = 0;
                         return 1;
                    case CAYENDO:
                         Estado = 1;
                         return 1;
                    case POSTSALTO:
                         Estado = 2;
                         return 1;
                    default:
                         Estado = 0;
                         return 1;
                    }
             break;
        case ALMA:
             switch (Estado) {
                    case VOLANDO:
                         Estado = 0;
                         return 1;
                    case SUBIENDO:
                         Estado = 1;
                         return 1;
                         }
             break;
        case MUJER:
             switch (Estado) {
                    case ARRANCANDO:
                         Estado = 2;
                         return 1;
                    case GIRAQUIETA:
                         Estado = 3;
                         return 1;
                    case GIRANDO:
                         Estado = 4;
                         return 1;
                    case MUERTE:
                         Estado = 5;
                         return 1;
                         }
             break;
        case NINO1:
             switch (Estado) {
                    case GIRANDO:
                         Estado = 2;
                         return 1;
                    case ANDASIN:
                         Estado = 3;
                         return 1;
                         }
             break;
        case NINO2:
        case NINO3:
             switch (Estado) {
                    case BABEANDO:
                         Estado = 0;
                         return 1;
                    case COGEBAL:
                         Estado = 1;
                         return 1;
                    case TIRABAL:
                         Estado = 2;
                         return 1;
                    }
             break;
        case BALON:
          if (Estado == PARADO) {
             Estado = 0;
             Fotograma = 0;
             }
          if (Estado > PARADO)
             Estado = 1;
          return 1;
        case CUORE:
        case GLOBO:
             Estado = 0;
             Sentido = 0;
             Fotograma = 0;
             return 1;
        case ELEFANT:
             switch (Estado) {
                    case GIRANDO:
                         Estado = 1;
                         return 1;
                    case MUERTE:
                         Estado = 2;
                         return 1;
                    default:
                         Estado = 0;
                         return 1;
                         }
        case COCO:
             switch (Estado) {
                    case MUERTE:
                         Estado = 1;
                         return 1;
                    default:
                         Estado = 0;
                         return 1;
                    }
        case TORO:
        case OVNIJ:
             if (Estado == ANDANDO)
                return 0;
             Estado = 1;
             return 1;
        case JERING:
             if (Estado == DISPARANDO) {
                Estado = 1;
                return 1;
                }
             if (Estado == VOLANDO) {
                Estado = 0;
                return 1;
                }
             Estado = 2;
             return 1;
        case CHEPPY1:
        case CHEPPY2:
        case CHEPPY3:
             switch (Estado) {
                case GIRANDO:
                  Estado = 1;
                  return 1;
                case MUERTE:
                  Estado = 2;
                  return 1;
                default:
                  Estado = 0;
                  return 1;
                }
              break;
        case ASOLDIER:
             switch (Estado) {
                case DISPARANDO:
                     Estado = 1;
                     return 1;
                case GIRANDO:
                     Estado = 2;
                     return 1;
                case MUERTE:
                     Estado = 3;
                     return 1;
                default:
                     Estado = 0;
                     return 1;
                }
             break;
        case ALIEN:
             switch (Estado) {
                case MUERTE:
                     Estado = 2;
                     return 1;
                case GIRANDO:
                     Estado = 1;
                     return 1;
                default:
                     Estado = 0;
                     return 1;
                     }
             break;
        case BURGUER:
             if (Estado == CAYENDO)
                Estado = 1;
              else
                Estado = 0;
             return 1;
        case HUMOJ:
             if (Estado == PARADO)
                Estado = 1;
              else
                Estado = 0;
             return 1;
        case SALTARIN:
             switch (Estado) {
                case PARADO:
                     Estado = 0;
                     return 1;
                case PRESALTO:
                     Estado = 1;
                     return 1;
                case SUBIENDO:
                     Estado = 2;
                     return 1;
                case CAYENDO:
                     Estado = 3;
                     return 1;
                case POSTSALTO:
                     Estado = 4;
                     return 1;
                case GIRANDO:
                     Estado = 5;
                     return 1;
                case MUERTE:
                     Estado = 6;
                     return 1;
                }
             return 0;

        case ROBOTIN:
             if (Estado == DISPARANDO) {
                Estado = 1;
                return 1;
                }
             if (Estado == VOLANDO) {
                Estado = 0;
                return 1;
                }
             if (Estado == GIRANDO) {
                Estado = 2;
                return 1;
                }
             Estado = 3;
             return 1;

        case ASTRONAUTA2:
             switch (Estado) {
                    case GIRANDO:
                         Estado = 2;
                         return 1;
                    case DISPARANDO:
                         Estado = 3;
                         return 1;
                    case MUERTE:
                         Estado = 4;
                         return 1;
                         }
             return 0;

        case METEORITO:
             if (Estado == MUERTE)
                Estado = 1;
              else
                Estado = 0;
              return 1;

        case MONSTRUO:
             if (Estado <= PARADO)
                return 0;
             switch (Estado) {
                    case NACIENDO:
                         Estado = 2;
                         return 1;
                    case SURGIENDO:
                         Estado = 3;
                         return 1;
                    case ENTERRANDOSE:
                    case MUERTE:
                         Estado = 4;
                         return 1;
                         }
             return 1;

        case ASPIRADOR:
             switch (Estado) {
                    case GIRANDO:
                         Estado = 2;
                         return 1;
                    case DISPARANDO:
                         Estado = 3;
                         return 1;
                    case MUERTE:
                         Estado = 4;
                         return 1;
                    default:
                         Estado = 0;
                         return 1;
                    }
            return 0;
        case SHORTCIRCUIT:
        case RODADOR:
        case ASTRONAUTA3:
             switch (Estado) {
                    case GIRANDO:
                         Estado = 1;
                         return 1;
                    case MUERTE:
                         Estado = 2;
                         return 1;
                    }
            return 0;
        case GRUA:
             switch (Estado) {
                    case ANDANDO:
                         Estado = 0;
                         return 1;
                    case PARADO:
                         Estado = 1;
                         return 1;
                    case GIRANDO:
                         Estado = 2;
                         return 1;
                    case MUERTE:
                         Estado = 3;
                         return 1;
                    }
            return 0;
        case TORRES:
             switch (Estado) {
                    case OCULTO:
                        Estado = 0;
                        return 1;
                    case APARECE:
                        Estado = 2;
                        return 1;
                    case GIRANDO:
                        Estado = 3;
                        return 1;
                    case DESENFUNDA:
                        Estado = 4;
                        return 1;
                    case ATURDIDO:
                        Estado = 5;
                        return 1;
                    case DESAPARECE:
                        Estado = 6;
                        return 1;
             }
            return 0;
        case TORREC:
             switch (Estado) {
                    case DISPARANDO:
                         Estado = 0;
                         return 1;
                    case RIENDO:
                         Estado = 1;
                         return 1;
                    case GIRANDO:
                         Estado = 2;
                         return 1;
                    case STAND:
                         Estado = 3;
                         return 1;
                    case STAND2:
                         Estado = 4;
                         return 1;
                    case ATURDIDO:
                         Estado = 5;
                         return 1;
                    case ENFUNDA:
                         Estado = 6;
                         return 1;
                    case PARADO:
                         Estado = 7;
                         return 1;
                    case MUERTE:
                         Estado = 8;
                         return 1;
                    }
             return 0;
        case RAYOTORRE:
        case ANUNCIOWIN:
        case TENTACLES:
        case LUZVERDE:
        case BURBUJA:
        case CEMENTO:
        case BOLITA:
        case MORCILLA:
        case HALOMORCI:
        case COLUMNON:
             Estado = 0;
             return 1;
		}
	return 0;
}

BITMAP* SprManager::Sprite (int Tipo, int Sentido, int Estado, int Fotograma, int Animacion)
{
    AjustaEstado (Tipo, Sentido, Estado, Fotograma);
    if (VJ.Fase == 2 && Tipo == MARIANO)
      Tipo = MARIANOASTR;
    switch (Tipo) {
           case TILE:
                clear_to_color (TileTemp, makecol (255,0,255));
                clear_to_color (TileRet, makecol (255,0,255));
                if (Fotograma & 1)
                   draw_sprite_h_flip (TileTemp, Tile[Estado][Sentido], 0, 0);
                 else
                   draw_sprite (TileTemp, Tile[Estado][Sentido], 0, 0);

                if (Fotograma & (1 << 1))
                   draw_sprite_v_flip (TileRet, TileTemp, 0, 0);
                else
                   draw_sprite (TileRet, TileTemp, 0, 0);

                return TileRet;

           case MARIANO:
                switch (Estado) {
                       case PARADO:
                            if (Animacion)
                               return MarianoAnim[Animacion-1][Sentido][Fotograma];
                       default:
                            return Mariano[Estado][Sentido][Fotograma];
                }

           case MARIANOASTR:
                switch (Estado) {
                       case PARADO:
                            if (Animacion)
                               return MarianoAstrAnim[Animacion-1][Sentido][Fotograma];
                       default:
                            return MarianoAstr[Estado][Sentido][Fotograma];
                }

           case ESCAFANDRA:
                switch (Estado) {
                       case PARADO:
                            if (Animacion)
                               return EscafandraAnim[Animacion-1][Sentido][Fotograma];
                       default:
                            return Escafandra[Estado][Sentido][Fotograma];
                }

           case CAPITAN:
                switch (Estado) {
                       case PARADO:
                            if (Animacion)
                               return CapitanAnim[Animacion-1][Sentido][Fotograma];
                       default:
                            return Capitan[Estado][Sentido][Fotograma];
                }
           default:
                if (Tipo >= CAPMENU) {
                   if (Estado > EstSprMenu [Tipo - CAPMENU] || Fotograma > MSpritesMenu [Tipo - CAPMENU][Estado][0][0]->Fotog)
                      LogWrite ("ERROR: -> Parámetros de sprite no válidos (T:%i (%i) E:%i (%i) S:%i (%i) F:%i (%i))\n", Tipo, CAPMENU + NumSprMenu, Estado, EstSprMenu [Tipo - CAPMENU], Sentido, 0, Fotograma, MSpritesMenu [Tipo - CAPMENU][Estado][0][0]->Fotog);
                   return SpritesMenu [Tipo - CAPMENU][Estado][0][Fotograma];
                   }
                if (Tipo >= BASE) {
                   if (Estado > EstSprMarc [Tipo - BASE] || Fotograma > MSpritesMarc [Tipo - BASE][Estado][0][0]->Fotog)
                      LogWrite ("ERROR: -> Parámetros de sprite no válidos (T:%i (%i) E:%i (%i) S:%i (%i) F:%i (%i))\n", Tipo, BASE + NumSprMarc, Estado, EstSprMarc [Tipo - BASE], Sentido, 0, Fotograma, MSpritesMarc [Tipo - BASE][Estado][0][0]->Fotog);
                   return SpritesMarc [Tipo - BASE][Estado][0][Fotograma];
                   }
                if (Tipo >= HALOG) {
                   if (Estado > EstSprOt [Tipo - HALOG] || Sentido > SentSprOt [Tipo - HALOG] || Fotograma > MSpritesOt [Tipo - HALOG][Estado][0][0]->Fotog)
                      LogWrite ("ERROR: -> Parámetros de sprite no válidos (T:%i (%i) E:%i (%i) S:%i (%i) F:%i (%i))\n", Tipo, HALOG + NumSprOt, Estado, EstSprOt [Tipo - HALOG], Sentido, SentSprOt [Tipo - HALOG], Fotograma, MSpritesOt [Tipo - HALOG][Estado][0][0]->Fotog);
                   return SpritesOt [Tipo - HALOG][Estado][Sentido][Fotograma];
                   }
                if (Estado > EstSpr [Tipo - FAROLG] || Sentido > SentSpr [Tipo - FAROLG] || Fotograma > MSprites [Tipo - FAROLG][Estado][0][0]->Fotog)
                   LogWrite ("ERROR: -> Parámetros de sprite no válidos (T:%i (%i) E:%i (%i) S:%i (%i) F:%i (%i))\n", Tipo, FAROLG + NumSpr, Estado, EstSpr [Tipo - FAROLG], Sentido, SentSpr [Tipo - FAROLG], Fotograma, MSprites [Tipo - FAROLG][Estado][0][0]->Fotog);
                return Sprites [Tipo - FAROLG][Estado][Sentido][Fotograma];
           }
    LogWrite ("ERROR: -> Sprite solicitado no válido\n");
    return NULL;
}

mask* SprManager::Mask (int Tipo, int Sentido, int Estado, int Fotograma, int Animacion)
{
    AjustaEstado (Tipo, Sentido, Estado, Fotograma);
    if (VJ.Fase == 2 && Tipo == MARIANO)
      Tipo = MARIANOASTR;
    switch (Tipo) {
           case TILE:
                return MTile[Sentido][Fotograma];

           case MARIANO:
                switch (Estado) {
                       case PARADO:
                            if (Animacion)
                               return MMarianoAnim[Animacion-1][Sentido][Fotograma];
                       default:
                            return MMariano[Estado][Sentido][Fotograma];
                            }

           case MARIANOASTR:
                switch (Estado) {
                       case PARADO:
                            if (Animacion)
                               return MMarianoAstrAnim[Animacion-1][Sentido][Fotograma];
                       default:
                            return MMarianoAstr[Estado][Sentido][Fotograma];
                }

           case CAPITAN:
                switch (Estado) {
                       case PARADO:
                            if (Animacion)
                               return MCapitanAnim[Animacion-1][Sentido][Fotograma];
                       default:
                            return MCapitan[Estado][Sentido][Fotograma];
                            }
           default:

                if (Tipo >= CAPMENU) {
                   if (Estado > EstSprMenu [Tipo - CAPMENU] || Fotograma > MSpritesMenu [Tipo - CAPMENU][Estado][0][0]->Fotog)
                      LogWrite ("ERROR: -> Parámetros de máscara no válidos (T:%i (%i) E:%i (%i) S:%i (%i) F:%i (%i))\n", Tipo, CAPMENU + NumSprMenu, Estado, EstSprMenu [Tipo - CAPMENU], Sentido, 0, Fotograma, MSpritesMenu [Tipo - CAPMENU][Estado][0][0]->Fotog);
                   return MSpritesMenu [Tipo - CAPMENU][Estado][0][Fotograma];
                   }
                if (Tipo >= BASE) {
                   if (Estado > EstSprMarc [Tipo - BASE] || Fotograma > MSpritesMarc [Tipo - BASE][Estado][0][0]->Fotog)
                      LogWrite ("ERROR: -> Parámetros de máscara no válidos (T:%i (%i) E:%i (%i) S:%i (%i) F:%i (%i))\n", Tipo, BASE + NumSprMarc, Estado, EstSprMarc [Tipo - BASE], Sentido, 0, Fotograma, MSpritesMarc [Tipo - BASE][Estado][0][0]->Fotog);
                   return MSpritesMarc [Tipo - BASE][Estado][0][Fotograma];
                   }
                if (Tipo >= HALOG) {
                   if (Estado > EstSprOt [Tipo - HALOG] || Sentido > SentSprOt [Tipo - HALOG] || Fotograma > MSpritesOt [Tipo - HALOG][Estado][0][0]->Fotog)
                      LogWrite ("ERROR: -> Parámetros de máscara no válidos (T:%i (%i) E:%i (%i) S:%i (%i) F:%i (%i))\n", Tipo, HALOG + NumSprOt, Estado, EstSprOt [Tipo - HALOG], Sentido, SentSprOt [Tipo - HALOG], Fotograma, MSpritesOt [Tipo - HALOG][Estado][0][0]->Fotog);
                   return MSpritesOt [Tipo - HALOG][Estado][Sentido][Fotograma];
                   }
                if (Estado > EstSpr [Tipo - FAROLG] || Sentido > SentSpr [Tipo - FAROLG] || Fotograma > MSprites [Tipo - FAROLG][Estado][0][0]->Fotog)
                   LogWrite ("ERROR: -> Parámetros de máscara no válidos (T:%i (%i) E:%i (%i) S:%i (%i) F:%i (%i))\n", Tipo, FAROLG + NumSpr, Estado, EstSpr [Tipo - FAROLG], Sentido, SentSpr [Tipo - FAROLG], Fotograma, MSprites [Tipo - FAROLG][Estado][0][0]->Fotog);
                return MSprites [Tipo - FAROLG][Estado][Sentido][Fotograma];
           }
    LogWrite ("ERROR: -> máscara solicitada no válida\n");
    return NULL;
}

