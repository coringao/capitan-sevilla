#include "sistparticulas.h"

SistParticulas::SistParticulas()
{
  NumParticulas = 0;
  Inicial = NULL;
  Final = NULL;
}


SistParticulas::~SistParticulas()
{
  Reset();
}

void SistParticulas::CreaSistema (int x, int y, void *BitMap)
{
  BITMAP *BM;
  BM = (BITMAP*) BitMap;
  Particula *Nueva;

  for (int n = 0; n < BM->w; n += TamanoPart)
      for (int m = 0; m < BM->h; m += TamanoPart) {
          Nueva = new Particula (x, y, n, m, BM, Final);
          if (Final) Final->SetSiguiente (Nueva);
          Final = Nueva;
          if (!Inicial) Inicial = Final;
          ++NumParticulas;
      }
}

int SistParticulas::MueveSistema()
{
  Particula *Bucle, *BAnt, *BSig;
  Bucle = Inicial;

  while (Bucle) {
    BAnt = Bucle->VerAnterior();
    BSig = Bucle->VerSiguiente();
    if (Bucle->Mueve() == SALEPANTALLA) {
     
      if (BAnt)
        BAnt->SetSiguiente (BSig);
      else
        if (BSig) Inicial = BSig;
         else
           Inicial = NULL;
      
      if (BSig)
        BSig->SetAnterior (BAnt);
      else
        if (BAnt) Final = BAnt;
         else
           Final = NULL;

      NumParticulas--;
      delete Bucle;
    }
    Bucle = BSig;
  }

  if (!NumParticulas)
    return SALEPANTALLA;
    
  return TODOOK;
}

void SistParticulas::DibujaSistema()
{
  Particula *Bucle;
  Bucle = Inicial;
  
  while (Bucle) {
    Bucle->Dibuja();
    Bucle = Bucle->VerSiguiente();
  }
}

void SistParticulas::BorraSistema()
{
  Particula *Bucle;
  Bucle = Inicial;
  
  while (Bucle) {
    Bucle->Borra();
    Bucle = Bucle->VerSiguiente();
  }
}

void SistParticulas::DibujaSombra()
{
  Particula *Bucle;
  Bucle = Inicial;
  
  while (Bucle) {
    Bucle->DibujaSombra();
    Bucle = Bucle->VerSiguiente();
  }
}

void SistParticulas::BorraSombra()
{
  Particula *Bucle;
  Bucle = Inicial;
  
  while (Bucle) {
    Bucle->BorraSombra();
    Bucle = Bucle->VerSiguiente();
  }
}

void SistParticulas::Reset()
{
  Particula *Bucle, *Prox;
  Bucle = Inicial;
  
  while (Bucle) {
    Prox = Bucle->VerSiguiente();
    delete Bucle;
    Bucle = Prox;
  }
  NumParticulas = 0;
  Inicial = NULL;
  Final = NULL;
}
