#include "particula.h"

Particula::Particula(int X, int Y, int ox, int oy, BITMAP *BM, Particula *Ant)
{
  int PixelTrans = 0;
  x = (X + ox) << 7;
  y = (Y + oy) << 7;
  BitMap = create_bitmap (TamanoPart, TamanoPart);
  clear_to_color (BitMap, makecol (255,0,255));
  blit (BM, BitMap, ox, oy, 0, 0, TamanoPart, TamanoPart);
  Siguiente = NULL;
  Anterior = Ant;
  Angle = 0;
  Transp = 255;
//  VelAng = (int) (rand() % 4) * 5;
//  VelAng *= ((int) (rand() % 2) * 2) - 1;
  VelAng = 0;
  VelY = ((int) (rand() % 4) + 2) << 6;
  VelY *= -1; //((int) (rand() % 2) * 2) - 1;
  VelX = (2 * (int) (rand() % 4)) << 6;
  VelX *= ((int) (rand() % 2) * 2) - 1;
  CiclosVida = 80;
  for (int n = 0; n < TamanoPart; n++)
      for (int m = 0; m < TamanoPart; m++)
          if (getpixel (BitMap, n, m) == makecol (255,0,255))
             PixelTrans++;
  if (PixelTrans > TamanoPart * TamanoPart / 2)
     y = -100;
//  LogWrite ("Creada partícula (%p).\n", this);

  HAct = (Hab*)VJ.HAct;
}


Particula::~Particula()
{
//  LogWrite ("Destruída partícula (%p).\n", this);
  destroy_bitmap (BitMap);
}


void Particula::SetSiguiente(Particula *Next)
{
  Siguiente = Next;
}

Particula* Particula::VerSiguiente()
{
  return Siguiente;
}

void Particula::SetAnterior(Particula *Prev)
{
  Anterior = Prev;
}

Particula* Particula::VerAnterior()
{
  return Anterior;
}

int Particula::Mueve ()
{
  int Retorno = TODOOK;

  Tile.X = (x >> 7) / TAMTILE;
  Tile.Y = (TamanoPart - 1 + ((VelY + y) >> 7)) / TAMTILE;

  if (Tile.X < 0 || Tile.X > TILESANCHO || Tile.Y > TILESALTO)
    Retorno = SALEPANTALLA;

  if (y >> 7 < 0)
    Retorno = SALEPANTALLA;

  if  (HAct->CContenido(Tile)/*y >> 7 > TAMTILE * (TILESALTO - 1)  - (TamanoPart + (VelY >> 7))*/) {
      if ((VelY >> 7) > 1) {
         VelY = -VelY / 5;
         VelX = VelX * 4 / 6;
         }
       else {
//        y = (TAMTILE * (TILESALTO - 1) - TamanoPart) << 7;
/*     while (HAct->CContenido (Tile)) {
            y -= 1 << 7;
            Tile.Y = (TamanoPart - 4 + (y >> 7)) / TAMTILE;
            }*/

         VelY = 0;
         VelX = VelX * 5 / 6;
        }
      }

  if (VelX >> 7 == 0) {
     Transp -= 10;
     if (Transp <= 0)
        Retorno = SALEPANTALLA;
     }

  x += VelX;
  y += VelY;
  Angle += VelAng;
  if (Angle >= 256)
    Angle -= 256;

  VelY += 1 << 6;

  return Retorno;
}

int Particula::Dibuja ()
{
  H->Dibuja_Trans (x >> 7,y >> 7, (void*)BitMap, Transp);
  return TODOOK;
}

int Particula::Borra ()
{
  H->Borra ((x >> 7) - TamanoPart, (y >> 7) - TamanoPart, TamanoPart * 2, TamanoPart * 2);
  return TODOOK;
}

void Particula::DibujaSombra ()
{
     H->Dibuja_Sombra (x >> 7, y >> 7, (void*)BitMap);
}

void Particula::BorraSombra ()
{
  H->Borra_Sombra ((x >> 7) - TamanoPart, (y >> 7) - TamanoPart, TamanoPart * 2, TamanoPart * 2);
}
