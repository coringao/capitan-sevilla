#include "guagua.h"

Guagua::Guagua(int XStop)
{
  CiclosQuieto = -1;
  Stop = XStop;// * TAMTILE;
  Velocidad = 4;

  SFx->PlayFX (DF_SAUTOLLEGA);


  Autobus.SetTipo (BUS);
  Autobus.SetSent (0);
  Autobus.SetX (-346);
  Autobus.SetY (114);
  Autobus.SetKF (3);
  Autobus.SetEst (ANDANDO);
  Autobus.ClrFotog();

  Cristal.SetTipo (CRISTALBUS);
  Cristal.SetSent (0);
  Cristal.SetX (-346);
  Cristal.SetY (114);
  Cristal.SetKF (3);
  Cristal.SetEst (ANDANDO);
  Cristal.ClrFotog();

  Faros.SetTipo (LUZBUS);
  Faros.SetSent (0);
  Faros.SetX (-14);
  Faros.SetY (114);
  Faros.SetEst (ANDANDO);
  Faros.ClrFotog();

  Linea = (int)(rand() % 100) + 1;
  sprintf (LineaCad, "%i", Linea);
}

Guagua::~Guagua()
{
}

void Guagua::Dibuja ()
{
  int TextY = 0;
  switch (Autobus.VerEst()) {
         case ANDANDO:
              TextY = Autobus.VerY() + 88;
              switch (Autobus.VerFotog()) {
                     case 1:
                          TextY += 2;
                          break;
                     case 3:
                          TextY -= 2;
                          break;
                     }
              break;
         case PARADO:
              TextY = Autobus.VerY() + 90;
              if (Autobus.VerFotog() == 1)
                 TextY -= 2;
              break;
         }
  Autobus.Dibuja();
  Cristal.Dibuja();
  Faros.Dibuja();
  H->TextOut(LineaCad, Autobus.VerX() + (86 - text_length (font, LineaCad)), TextY, 252, 184, 0, -1);
  H->TextOut(LineaCad, Autobus.VerX() + (260 - text_length (font, LineaCad)), TextY, 252, 184, 0, -1);
}

void Guagua::Borra ()
{
  Autobus.Borra();
  Cristal.Borra();
  Faros.Borra();
}

int Guagua::Mueve ()
{
  if (Autobus.VerX(true) >= Stop && Autobus.VerEst() == ANDANDO && CiclosQuieto < 0) {
    Autobus.SetEst (PARADO);
    Autobus.ClrFotog();

    Cristal.SetEst (PARADO);
    Cristal.ClrFotog();

    CiclosQuieto = 0;
    Velocidad = 0;
    }

  if (Autobus.VerEst() == PARADO && ++CiclosQuieto > 60) {
    SFx->PlayFX (DF_SAUTOSALE);
    Autobus.SetEst (ANDANDO);
    Autobus.ClrFotog();

    Cristal.SetEst (ANDANDO);
    Cristal.ClrFotog();

    Velocidad = 4;
    }

    Autobus.IncX (Velocidad);
    Cristal.IncX (Velocidad);
    Faros.IncX (Velocidad);

    if (Autobus.VerX() >= ANCHOPANTALLA) {
      Autobus.SetX(-346);
      Cristal.SetX(-346);
      Faros.SetX(-14);
      return SALEPANTALLA;
      }
    return TODOOK;
}

int Guagua::VerX ()
{
  return Autobus.VerX(true);
}
