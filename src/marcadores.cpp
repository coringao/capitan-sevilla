#include "marcadores.h" // class's header file

Marcadores::Marcadores()
{
    LogWrite ("Creando Marcadores del juego (%p)...", this);

    SprMarc[0].SetTipo(MORCILLON);
    SprMarc[0].SetSent (0);
    SprMarc[0].SetX (206);
    SprMarc[0].SetY (324);
    SprMarc[1].SetTrans (0);
    SprMarc[0].SetEst (0);
    SprMarc[0].ClrFotog();

    SprMarc[1].SetTipo(NIEBLA);
    SprMarc[1].SetSent (0);
    SprMarc[1].SetX (206);
    SprMarc[1].SetY (324);
    SprMarc[1].SetKF (3);
    SprMarc[1].SetTrans (255);
    SprMarc[1].SetEst (0);
    SprMarc[1].ClrFotog();

    SprMarc[2].SetTipo(QUEHASE);
    SprMarc[2].SetSent (0);
    SprMarc[2].SetX (304);
    SprMarc[2].SetY (310);
    SprMarc[2].SetEst (0);
    SprMarc[2].ClrFotog();

    SprMarc[3].SetTipo(TORREMARCA);
    SprMarc[3].SetKF (3);
    SprMarc[3].SetSent (0);
    SprMarc[3].SetX (32);
    SprMarc[3].SetY (302);
    SprMarc[3].SetEst (0);
    SprMarc[3].ClrFotog();

    SprMarc[4].SetTipo(INTERFERENC);
    SprMarc[4].SetSent (0);
    SprMarc[4].SetX (32);
    SprMarc[4].SetY (316);
    SprMarc[4].SetEst (0);
    SprMarc[4].SetTrans (128);
    SprMarc[4].ClrFotog();

    SprMarc[5].SetTipo(NIEBLATORRE);
    SprMarc[5].SetSent (0);
    SprMarc[5].SetX (32);
    SprMarc[5].SetY (302);
    SprMarc[5].SetEst (0);
    SprMarc[5].SetTrans (32);
    SprMarc[5].ClrFotog();

    CiclosQTorre = 0;


    LogWrite ("OK.\n");
}

Marcadores::~Marcadores()
{
    LogWrite ("Destruyendo Marcadores del juego (%p)...", this);
    LogWrite ("OK.\n");
}

void Marcadores::Show (int curSkip, int afps, int Temporizador)
{
     int Result, Tipo, Estado;

     Tipo = ((Actor*)(VJ.Jugador))->VerTipo();
     Estado = ((Actor*)(VJ.Jugador))->VerEst();
     if (Tipo == MARIANOMUERTO) {
        Tipo = MARIANO;
        Estado = PARADO;
      }

     SprMarc[2].SetEst (VJ.QueHase);
     SprMarc[2].SetKF (2);

     H->RectFill (-2, 272, ANCHOPANTALLA + 2, ALTOPANTALLA + 2, 0);
     H->Dibuja (0, 272, M->Sprite(BASE));

     for (int n = 0; n < VJ.Vidas; n++)
         H->Dibuja (424 + n * 24, 312, M->Sprite (CARAVIDA, 0, Tipo - MARIANO));

     for (int n = 0; n < VJ.Morcillas; n++)
         H->Dibuja (512 - (n + 1) * 25, 374, M->Sprite (MORCILLITA));

     if (Tipo == MARIANO)
        if (Estado == TRANSFOR || Estado == TRANSFOR + 1) {
            SprMarc[0].IncTrans (6);
            SprMarc[1].IncTrans (-6);
            }
         else {
           SprMarc[0].SetTrans (0);
           SprMarc[1].SetTrans (255);
           }


     if (Tipo == CAPITAN)
        if (Estado == TRANSFOR) {
           SprMarc[0].IncTrans (-6);
           SprMarc[1].IncTrans (6);
	       }
         else {
           SprMarc[0].SetTrans (255);
           SprMarc[1].SetTrans (0);
           }

     SprMarc[0].Dibuja();

     if (Tipo == CAPITAN) {
        Result = ((Actor*)(VJ.Jugador))->VerPow() * 81 / (140 - 20 * Config->VerDif());
        if (Result < 0) Result = 0;
        H->RectFill(206 + 81, 324, 206 + Result, 324 + 31, 0);
        }

     if (SprMarc[3].VerTipo() == TORREMARCA) {
        if (SprMarc[3].FinAnim()) {
            SprMarc[3].SetEst(0);
            SprMarc[3].ClrFotog();
            }

        if (SprMarc[3].VerEst() == 0)
            if (++CiclosQTorre >= 150) {
                SprMarc[3].SetEst(3 + ((int) rand() % 4));
                SprMarc[3].ClrFotog();
                }

        if (SprMarc[3].VerEst() != 0)
            CiclosQTorre = 0;
        }
      else {
        SprMarc[3].CopyData((Actor*)VJ.Torrebruno);
      }


     SprMarc[4].SetTrans (92 + (2*((int)rand()%2) - 1) * (int) rand()%50);

     for (int n = 1; n < NSprMarc; n++)
          SprMarc[n].Dibuja();

     H->Dibuja (8, 302, M->Sprite(MARCOTORRE));

     if (VJ.OSD) {
        H->TextOut("FPS:", 0, 0, 4, 0);
        H->TextNum (afps, 32, 0, 4, 0);
        H->TextOut ("CPS:", 80, 0, 4, 0);
        H->TextNum (Temporizador, 112, 0 , 4, 0);
        H->TextOut("FS:", 0, text_height(font), 4, 0);
        H->TextNum (curSkip-1, 32, text_height(font), 8 - curSkip, 0);
        if (VJ.Cheats)
            H->TextOut ("CHEATS ON", 80, text_height (font), 4, 0);
          else
            H->TextOut ("CHEATS OFF", 80, text_height (font), 2, 0);
        }
}

void Marcadores::Torrebruno (int Estado)
{
     if (Estado == COLISION && SprMarc[3].VerEst() != 2) {
        SprMarc[3].SetEst(2); // Descojonándose
        SprMarc[3].ClrFotog();
        }
     if (Estado == MUERTEENEMIGO && SprMarc[3].VerEst() != 1) {
        SprMarc[3].SetEst(1); // ¡Me las pagarás!
        SprMarc[3].ClrFotog();
        }
}

void Marcadores::SituaTorre ()
{
    if (VJ.Torrebruno != NULL) {
        SprMarc[3].CopyData((Actor*)VJ.Torrebruno);
        SprMarc[3].SetX (12);
        SprMarc[3].SetY (306);
        }
     else
        if (SprMarc[3].VerTipo() != TORREMARCA) {
            SprMarc[3].SetTipo(TORREMARCA);
            SprMarc[3].SetKF (3);
            SprMarc[3].SetSent (0);
            SprMarc[3].SetX (32);
            SprMarc[3].SetY (302);
            SprMarc[3].SetEst (0);
            SprMarc[3].ClrFotog();
            }
}
