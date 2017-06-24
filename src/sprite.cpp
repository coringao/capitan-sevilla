#include "sprite.h"

Sprite::Sprite()
{
 Fotograma = 0;
 Frames = 0;
 KeyFrame = 3;
 FotoMax = 0;
 Repeat = true;
 Animacion = 0;
 FinAnimac = false;
 Trans = 128;
 Halo = 0;
 HaloSent = 10;
 Parpadeo = 0;
 VerHalo = false;
 EstDest = -1;
}

Sprite::~Sprite()
{
}

void Sprite::CopyData (void *From)
{
    Sprite *Spr;
    Spr = (Sprite*)From;

    Tipo = Spr->Tipo;
    Fotograma = Spr->Fotograma;
    Frames = Spr->Frames;
    Sentido = Spr->Sentido;
    Estado = Spr->Estado;
    Trans = Spr->Trans;
    KeyFrame = Spr->KeyFrame;
    FotoMax = Spr->FotoMax;
    Animacion = Spr->Animacion;
    Dibujo = M->Sprite(Tipo, Sentido, Estado, Fotograma, Animacion);
    Mascara = M->Mask(Tipo, Sentido, Estado, Fotograma, Animacion);
}

void Sprite::PasaFrame () {
     FinAnimac = false;
     if (FotoMax == 0) return;
     if (++Frames > KeyFrame) Frames = 1;
     if (Frames == KeyFrame && Fotograma == FotoMax)
        FinAnimac = true;
     if (Frames == KeyFrame)
        if (++Fotograma>FotoMax)
           if (Repeat)
             Fotograma=0;
           else
             Fotograma--;
     Dibujo = M->Sprite(Tipo, Sentido, Estado, Fotograma, Animacion);
     Mascara = M->Mask(Tipo, Sentido, Estado, Fotograma, Animacion);
}

void Sprite::Actualiza ()
{
     int Fotog = Fotograma;
     Fotograma = 0;
     Dibujo = M->Sprite(Tipo, Sentido, Estado, Fotograma, Animacion);

     if (!Dibujo)
        LogWrite ("ERROR: NO HAY BITMAP PARA ESTE SPRITE: %i.\n", Tipo);

     Mascara = M->Mask(Tipo, Sentido, Estado, Fotograma, Animacion);

     if (!Mascara)
        LogWrite ("ERROR: NO HAY MASCARA PARA ESTE SPRITE: %i.\n", Tipo);

     FotoMax = Mascara->Fotog - 1;
     if (Fotog <= FotoMax) Fotograma = Fotog;
     SetRepeat (Estado == ANDANDO || Tipo == SEMAFORO || Estado == VOLANDO ||
                Tipo == BUS || Tipo == CRISTALBUS || Estado == ANDASIN || Tipo == ELEFANT ||
                Tipo == QUEHASE || Estado == BABEANDO || Tipo == BALON || Tipo == COCO ||
                Tipo == HUMOJ || Tipo == SALTARIN || Estado == ATURDIDO || Tipo == LUCECITA);
     if (Tipo == MARIANOMUERTO && Estado == CAYENDO)
       SetRepeat(true);
     if (Tipo == MUJER && Estado == PARADO)
       SetRepeat (true);
     if (Estado == MUERTE || Tipo >= MESACAPI)
       SetRepeat (false);
}

void Sprite::Borra () {
     if (!VerHalo)
        H->Borra (VerX(), VerY(), Mascara->LongX,Mascara->LongY);
      else
        H->Borra (VerX() - 2, VerY() - 2, Mascara->LongX + 4, Mascara->LongY + 4);
}

void Sprite::Dibuja (bool actualiza)
{
     int Transp = Mascara->Transp;
     Dibujo = M->Sprite(Tipo, Sentido, Estado, Fotograma, Animacion);
     Mascara = M->Mask(Tipo, Sentido, Estado, Fotograma, Animacion);

     if (Tipo == FINFASE)
        return;

     if (VerHalo)
        DibujaHalo();

     if (VerParpadeo()) {
        if (!(--Parpadeo % 2)) {
           if (actualiza)
              PasaFrame ();
           return;
           }

/*        if (Config->VerBlending() || !((int)rand() % 2)) {
           if (Config->VerBlending())
              H->Dibuja_Trans (VerX(), VerY(), Dibujo, 255 - Parpadeo);
           if (actualiza)
              PasaFrame ();
           return;
           }*/
        }

     if (!Config->VerBlending() &&
        (VerTipo() == HUMOJ || VerTipo() == LASERJ || VerTipo() == ALMA || VerTipo() == BURBUJA))
         Transp = 0;

     if (Estado == MUERTE) // Aunque no sea transparente, si se está muriendo, se usa la transparencia
      Transp = 2;

     if (Config->VerBlending() && Tipo == MARIANO && VJ.Fase == 2)
        H->Dibuja_TransAdd (VerX(), VerY(), M->Sprite(ESCAFANDRA, Sentido, Estado, Fotograma, Animacion), 160);

     switch (Transp) {
            case 0: // Dibuja normal*/
                 H->Dibuja (VerX(), VerY(), Dibujo);
                 break;
            case 1: // Dibuja usando mezcla aditiva
                 H->Dibuja_TransAdd (VerX(), VerY(), Dibujo, Trans);
                 break;
            case 2: // Dibuja usando transparencia normal
                 H->Dibuja_Trans (VerX(), VerY(), Dibujo, Trans);
                 if (EstDest >= 0)
                    H->Dibuja_Trans(VerX(), VerY(), M->Sprite(Tipo, Sentido, VerEstDest()), 255 - VerTrans());
                 break;
            }

     if (actualiza) PasaFrame ();
}

int Sprite::DibujaHalo ()
{
     Halo += HaloSent;
     if (Halo < 10 || Halo > 250)
        HaloSent = -HaloSent;
     H->DibujaHalo (VerX(), VerY(), Dibujo, Halo,
                    (Estado == VOLANDO || Estado == DESPEGANDO || Estado == CAYENDO || Estado == SUBIENDO));
     return Halo;
}

const int Sprite::VerX (bool Final) {
    if (Final) return (int) (x + (Mascara->LongX));
    return (int) x;}

const int Sprite::VerY (bool Final) {
    if (Final) return (int) (y + (Mascara->LongY));
    return (int) y;}

bool Sprite::Colision (Sprite *Otro, bool CheckPP) {

       if (!Otro) return false;
       if (Otro == this) return false;

       int cx, cy, clx, cly;

       cx = Otro->VerX(false);
       clx = Otro->VerX(true);
       cy = Otro->VerY(false);
       cly = Otro->VerY(true);

        if ((VerX(true) > cx) && (VerX(false) < clx) &&
            (VerY(true) > cy) && (VerY(false) < cly)){  // Existe colision de bouncing box ...
            if (!CheckPP) return true; // Si no tienes que mirar la Pixel Perfect, ya hay colision
            return (ColisionPP (Otro)); // Si hay que mirarla, hazlo...
        }
        return false;
     }

bool Sprite::ColisionPP (void *Otro, int xt, int yt, mask *Masc, int Altura)
{
    int x1, x2, y1, y2;
    int dx1, dx2, w1, w2, iw2, iw1, dy1, dy2;
    mask *Mask1, *Mask2;
    word Comp1, Comp2;
    Sprite *Temp;
    w2 = 0; w1 = 0;

    Mask1 = VerMascara();
    x1 = VerX(false);
    y1 = VerY(false);

    if (xt < 0 && yt < 0) { // Si has pasado un sprite
       Temp = (Sprite *) Otro;
       Mask2 = Temp->VerMascara();
       x2 = Temp->VerX(false);
       y2 = Temp->VerY(false);
       }
     else { // Si has pasado una mascara y unas coordenadas
       Mask2 = (mask *)Otro;
       x2 = xt;
       y2 = yt;
       }

     if (Masc)
        Mask1 = Masc; // Si pasamos una máscara, hay que usarla ;)

     if (!Mask1->Colisiona || !Mask2->Colisiona)
        return false; // Si uno de los dos es no colisionable... no hay más que mirar*/

      if (x1>x2) {
           dx1 = 0;
           dx2 = (x1 - x2);
           while (dx2>=Tamano) {
                 dx2-=Tamano;
                 w2++;}
           }
        else {
           dx1 = (x2 - x1);
           dx2 = 0;
           while (dx1>=Tamano) {
                 dx1-=Tamano;
                 w1++;}
           }

       if (y1>y2) {
           dy1 = 0;
           dy2 = y1 - y2;}
         else {
           dy1 = y2 - y1;
           dy2 = 0;}

       while ((dy1 < Mask1->LongY) && (dy2 < Mask2->LongY)) { // Bucle vertical

          iw1 = w1; iw2 = w2;
          while   ((iw1 < Mask1->Words) && (iw2 < Mask2->Words)) { // Bucle horizontal

             Comp1 = Mask1->MaskData [dy1][iw1] >> dx1; // El comp del 1 es el 1 desplazado a la derecha
             Comp2 = Mask2->MaskData [dy2][iw2] >> dx2; // Id con el 2

             if ((dx1!=0) && (iw1 < Mask1->Words-1)) // Si existe...
                Comp1 += Mask1->MaskData [dy1][iw1+1] << (Tamano-dx1); // Añadir la parte de la izqda

             if ((dx2!=0) && (iw2 < Mask2->Words-1))
                Comp2 += Mask2->MaskData [dy2][iw2+1] << (Tamano-dx2);

             if (Altura > dy1) Comp1 = 0;

             if (Comp1 & Comp2) return true;

             iw1++;
             iw2++;}
          dy1++;
          dy2++;}

     return false;
}

void Sprite::SetEst (int Est)
{
     Estado = Est;
     Animacion = 0;
     FinAnimac = false;
     Actualiza();
}

bool Sprite::VerParpadeo()
{
     return (Parpadeo > 0);
}

void Sprite::SetParpadeo(int Valor)
{
     Parpadeo = Valor;
}
