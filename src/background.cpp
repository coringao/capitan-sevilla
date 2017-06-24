#include "background.h" // class's header file

Background::Background()
{
    FondoStar = NULL;
    FondoBack = NULL;
}

Background::~Background()
{
    destroy_bitmap (FondoStar);
    destroy_bitmap (FondoBack);
}

void Background::Genera (int Num)
{
     if (Num < 0) {
       H->CopyFondo();
       if (NumHab == Num)
         return;
     }

     NumStars = 0;
     for (int n = 0; n < MaxEstrellas; n ++)
         Stars[n].Estado = -1;

     CDepth = get_color_depth();

     for (int n = 0; n < 10; n++){
         Fugaz.x[n] = -1;
         Fugaz.y[n] = -1;
         }

     destroy_bitmap (FondoStar);
     destroy_bitmap (FondoBack);

     if (Num >= 0) { // Si es una habitación
        FondoBack = create_bitmap (TILESANCHO * TAMTILE / 2, TILESALTO * TAMTILE / 2);
        FondoStar = create_bitmap (TILESANCHO * TAMTILE / 2, TILESALTO * TAMTILE / 2);

        clear_to_color (FondoBack, makecol (0, 0, 58));

        switch (Num) {
               case 24:
               case 25:
               case 26:
               case 27:
                    draw_sprite (FondoBack, (BITMAP*)H->CargaBitMap(DFBM_BCK_24 + Num - 24), 0, 0);
                    break;
               case 37:
               case 38:
                    draw_sprite (FondoBack, (BITMAP*)H->CargaBitMap(DFBM_BCK_37 + Num - 37), 0, 0);
                    break;
               default:
                    if (Num < 16)
                       draw_sprite (FondoBack, (BITMAP*)H->CargaBitMap(DFBM_BCK_00), 0, 0);
               }
        draw_sprite (FondoStar, FondoBack, 0, 0);
        draw_sprite (FondoBack, (BITMAP*)H->CargaScreen(Num), 0, 0);
        H->Dibuja2x (0, 0, FondoBack);
        }
      else { // Si es el menú
        FondoBack = create_bitmap (640, 240);
        FondoStar = create_bitmap (640, 240);
        draw_sprite (FondoBack, (BITMAP*)H->VerFondo(), 0, 0);
        draw_sprite (FondoStar, FondoBack, 0, 0);
        H->Dibuja (0, 0, FondoBack);
        }

     for (int n = 0; n < MaxOvnis; n++) {
         Ovni[n].SetTipo (OVNIMINI);
         Ovni[n].SetSent (0);
         Ovni[n].ClrFotog();
         Ovni[n].SetEst (0);
         Ovni[n].SetX (-100 * ((n % 2) * 2 - 1));
         Ovni[n].SetVelX (2 + (int) rand() % 2);
         OrigOvni[n] = 100;
         }
     Disparos = 0;

     if (Num < 24) {
         for (int n = 0; n < MaxHumo; n++) {
             Humo[n].SetTipo (HUMO);
             Humo[n].SetSent (0);
             Humo[n].SetEst (0);
             Humo[n].SetX(-100 + (int) rand() % 320);
             Humo[n].SetY (0);
             Humo[n].SetKF (3 + (int) rand() % 3);
             Humo[n].SetFotog((int) rand() % 10);
             Humo[n].SetTrans (129);
             }

         for (int n = 1; n < MaxHumo; n++)
             Humo[n].SetX (Humo[n-1].VerX(true) - 40 + ((int) rand() % 300));
         }

     NumHab = Num;

     H->CopyFondo(); // Es imprescindible, porque si no vamos a usar "fondos dinámicos", hace falta tener la habitación en el búfer "Fondo"
}

void Background::Actualiza ()
{
     int n, m;

     if ((NumHab > 15 && NumHab < 24) || (NumHab > 32 && NumHab < 37)) { // Desde la 16 no hay cielo
        H->Dibuja2x (0, 0, FondoBack);
        return;
        }

     if (NumHab >= 0 && NumHab < 24) // Si es una habitación
        H->Dibuja2x (0, 0, H->CargaBitMap(DFBM_BCK_00));

     if (!(int(rand()%30)) && NumStars < MaxEstrellas)
        CreaEstrella();

     if (Fugaz.x[0] < 0 && Fugaz.x[9] < 0)
        CreaFugaz();
      else
        DibujaFugaz();

    for (n = 0; n < NumStars; n++)
         if (BorraEstrella (n)) {
            for (m = n; m < MaxEstrellas - 1; m++)
                 Stars[m] = Stars[m+1];
            Stars[MaxEstrellas-1].Estado = -1;
            NumStars--;
            }

     for (n = 0; n < NumStars && Stars[n].Estado >= 0; n++)
         DibujaEstrella (n);

     if (NumHab < 24)
        for (int n = 0; n < Disparos; n++) {
            if (Disparo[n].x[0] >= 0) {
               DibujaDisparo (Disparo[n]);
               if (Disparo[n].x[0] < 0) { // Hay que borrar este disparo
                  for (int j = n; j < MaxDisparos - 1; j++)
                      Disparo[j] = Disparo[j+1];
                  Disparos--;
                  }
               }
            }

#ifdef FBLEND
     fblend_trans (FondoBack, FondoStar, 0, 0, 64);  // Al hacer esto se borran las estrellas, y, ademas, queda mas "Bonito"
#endif

#ifndef FBLEND
     set_trans_blender (0,0,0,64);
     draw_trans_sprite(FondoStar, FondoBack, 0, 0);  // Al hacer esto se borran las estrellas, y, ademas, queda mas "Bonito"
     solid_mode();
#endif

     if (NumHab >= 0) // Habitación (que están reducidas al tamaño normal)
        H->Dibuja2x (0, 0, FondoStar);
      else // Menú
        H->Dibuja (0, 0, FondoStar);

     if (NumHab < 33)
        MueveOvni();

     if (NumHab < 24)
         for (int n = 0; n < MaxHumo; n++)
             Humo[n].Dibuja();

     if (NumHab >= 0) // Si es una habitación
        H->Dibuja2x (0, 0, (BITMAP*)H->CargaScreen(NumHab)); // Al poner de nuevo el fondo de la habitación, borramos todo aquello que haya dibujado encima de los edificios
     else { // Si es el menú
        H->Dibuja (0, 0, (BITMAP*)H->CargaBitMap(DFBM_MENU_001));

        H->ResizeFont_af (80); // Restaura la zona del título
        H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(2), 326, 6, 252, 88, 0);
        H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(2), 322, 2, 0, 0, 0);
        H->TextoPartido ((char*) SMS->ReadMens(2), 320, 0, 255,255,255, 252,224,168);

        H->Borra(272, 232, 596, 426); // del menú
        H->Borra(0, 460, 640, 480); // y del copyright de abajo
        }
}

void Background::MueveOvni ()
{
     int Velocidad;
     for (int n = 0; n < MaxOvnis; n++) {
         Ovni[n].IncX ((int) Ovni[n].VerVelX());
         Ovni[n].SetY ((int) (OrigOvni[n] + (3 + n) * FuncionSeno[Ovni[n].VerX()]));

         if ((Ovni[n].VerX(true) < 0 || Ovni[n].VerX(false) > 640) && !((int)(rand() % 60))) {
            if ((int)rand() % 2)
               Ovni[n].SetX(0 - Ovni[n].VerLongX());
              else
               Ovni[n].SetX(640);
            Velocidad = 2 + (int) rand() % 2;
            if (Ovni[n].VerVelX() > 0)
		       Ovni[n].SetVelX(-Velocidad);
             else
                Ovni[n].SetVelX(Velocidad);
            OrigOvni[n] = (int) (rand() % 60);
            if (NumHab < 0)
               OrigOvni[n] += (rand() % 84);
            Ovni[n].SetY (OrigOvni[n]);
            }

         if (Ovni[n].VerX(false) > 0 && Ovni[n].VerX(true) < 640) {
            if (!(int) (rand() % 20))
               Dispara(n);
            }
         Ovni[n].Dibuja();
         }
}

bool Background::PixelValido (int x, int y)
{
     long int Color;
     x *= (1 + (NumHab < 0));
     y *= (1 + (NumHab < 0));

     if (x < 0 || x >= FondoBack->w ||
         y < 0 || y >= FondoBack->h)
           return false;

     Color = (FondoBack->line[y])[x];

     if (getb (Color) <= 100 && getr (Color) <= 64 && getg (Color) <= 64)
        return true;

     return false;
}

void Background::CreaEstrella ()
{
     if (NumStars >= MaxEstrellas) return;
     do {
        Stars[NumStars].x = 5 + (int(rand() % (310 * (1 + (NumHab < 0)))));
        Stars[NumStars].y = 5 + (int(rand() % (110 * (1 + (NumHab < 0)))));
        Stars[NumStars].Estado = 0;
        } while (!PixelValido (Stars[NumStars].x, Stars[NumStars].y));
     Stars[NumStars++].Ciclos = 0;
}

void Background::CreaFugaz()
{
     if ((int)(rand()%20))
        return;

     for (int n = 0; n < 10; n++){
         Fugaz.x[n] = -1;
         Fugaz.y[n] = -1;
         }

     do {
         Fugaz.x[0] = 5 + (int(rand() % (310 * (1 + (NumHab < 0)))));
         Fugaz.y[0] = 5 + (int(rand() % (110 * (1 + (NumHab < 0)))));
         } while (!PixelValido (Fugaz.x[0], Fugaz.y[0]));
     Fugaz.Color[0] = 4;
     Fugaz.Ciclos = 0;
     Fugaz.vx = ((int) (rand() % 2)) * 2 - 1;
     Fugaz.vy = ((int) (rand() % 3)) + 1;
}

int Background::Dispara(int NOvni)
{
    if (Disparos >= MaxDisparos)
       return 1;
    for (int n = 0; n < 10; n++){
        Disparo[Disparos].x[n] = -1;
        Disparo[Disparos].y[n] = -1;
        }
    Disparo[Disparos].x[0] = (Ovni[NOvni].VerX() + Ovni[NOvni].VerLongX() / 2) / 2 + (int) Ovni[NOvni].VerVelX();
    Disparo[Disparos].y[0] = Ovni[NOvni].VerY(true) / 2;
    Disparo[Disparos].Color[0] = 14;
    Disparo[Disparos].Ciclos = 0;
    Disparo[Disparos].vx = ((int) (rand() % 3)) + 1;
    Disparo[Disparos++].vy = ((int) (rand() % 2)) * 2 - 1;
    return 0;
}

void Background::DibujaFugaz()
{
     for (int n = 0; n < 10; n++)
         if (Fugaz.x[n] >= 0)
            PonPixel (Fugaz.x[n], Fugaz.y[n], Fugaz.Color[n]);

     for (int n = 9; n > 0; n--) {
         Fugaz.x[n] = Fugaz.x[n-1];
         Fugaz.y[n] = Fugaz.y[n-1];
         Fugaz.Color[n] = Fugaz.Color[n-1] - 1;
         if (Fugaz.Color[n] < 0) {
            Fugaz.x[n] = -1;
            Fugaz.y[n] = -1;
            }
         }

     if (!((Fugaz.Ciclos) % Fugaz.vy))
        Fugaz.y[0]++;

     Fugaz.x[0]+= Fugaz.vx;

     Fugaz.Ciclos++;

     if (Fugaz.Ciclos > 20 || Fugaz.x[0] < 0 || Fugaz.x[0] >= FondoStar->w || Fugaz.y[0] < 0 || Fugaz.y[0] >= FondoStar->h) {
        Fugaz.x[0] = -1;
        Fugaz.y[0] = -1;
        }
}

void Background::DibujaDisparo(EFugaz &Disp)
{
     for (int n = 0; n < 10; n++)
         if (Disp.x[n] >= 0)
            PonPixel (Disp.x[n], Disp.y[n], Disp.Color[n]);

     for (int n = 9; n > 0; n--) {
         Disp.x[n] = Disp.x[n-1];
         Disp.y[n] = Disp.y[n-1];
         Disp.Color[n] = Disp.Color[n-1] - 1;
         if (Disp.Color[n] < 10) {
            Disp.x[n] = -1;
            Disp.y[n] = -1;
            }
         }

     if (!((Disp.Ciclos) % Disp.vx))
        Disp.x[0]+= Disp.vy;

     Disp.y[0]++;

     if (++Disp.Ciclos > 20 || Disp.x[0] < 0 || Disp.x[0] >= FondoStar->w || Disp.y[0] < 0 || Disp.y[0] >= FondoStar->h) {
        for (int n = 1; n < 10; n++)  // Para que no deje rastro al desaparecer el disparo
            if (Disp.x[n] >= 0)
               PonPixel (Disp.x[n], Disp.y[n], 0);
        Disp.x[0] = -1;
        Disp.y[0] = -1;
        }
}

int Background::BorraEstrella (int Star)
{
     if (Stars[Star].Estado > 7)
        return 1;

     return 0;
}

void Background::PonPixel (int x, int y, int color)
{
    char Tamano;
    long int Color = 0;
    switch (color) {
	case 0:
	    Color = makecol (64, 64, 128);
	    break;
	case 1:
	    Color = makecol (80, 80, 144);
	    break;
	case 2:
	    Color = makecol (100, 100, 164);
	    break;
	case 3:
	    Color = makecol (172, 172, 255);
	    break;
	case 4:
	    Color = makecol (255, 255, 255);
	    break;
    case 10:
        Color = makecol (128, 64, 128);
        break;
    case 11:
        Color = makecol (172, 64, 128);
        break;
    case 12:
        Color = makecol (255, 64, 92);
        break;
    case 13:
        Color = makecol (255, 64, 64);
        break;
    case 14:
        Color = makecol (255, 0, 0);
        break;
	}

     if (!PixelValido (x,y)) return;

     if (NumHab >= 0)
        Tamano = 1;
      else
        Tamano = 2;

      for (int n = 0; n < Tamano; n++)
          for (int m = 0; m < Tamano; m++)
              if (CDepth < 24)
                 ((short *)FondoStar->line[Tamano * y + m])[Tamano * x + n] = Color;
                else
                 ((long *)FondoStar->line[Tamano * y + m])[Tamano * x + n] = Color;
     return;
}

void Background::DibujaEstrella (int Star)
{
     switch (Stars[Star].Estado) {
            case 7:
            case 0:
                 PonPixel (Stars[Star].x, Stars[Star].y, 0);
                 break;
            case 6:
            case 1:
                 PonPixel (Stars[Star].x, Stars[Star].y, 1);
                 PonPixel (Stars[Star].x-1, Stars[Star].y, 0);
                 PonPixel (Stars[Star].x+1, Stars[Star].y, 0);
                 PonPixel (Stars[Star].x, Stars[Star].y-1, 0);
                 PonPixel (Stars[Star].x, Stars[Star].y+1, 0);
                 break;
            case 5:
            case 2:
                 PonPixel (Stars[Star].x, Stars[Star].y, 2);
                 PonPixel (Stars[Star].x-1, Stars[Star].y, 1);
                 PonPixel (Stars[Star].x+1, Stars[Star].y, 1);
                 PonPixel (Stars[Star].x, Stars[Star].y-1, 1);
                 PonPixel (Stars[Star].x, Stars[Star].y+1, 1);
                 break;
            case 4:
            case 3:
                 PonPixel (Stars[Star].x, Stars[Star].y, 4);
                 PonPixel (Stars[Star].x-1, Stars[Star].y, 3);
                 PonPixel (Stars[Star].x+1, Stars[Star].y, 3);
                 PonPixel (Stars[Star].x, Stars[Star].y-1, 3);
                 PonPixel (Stars[Star].x, Stars[Star].y+1, 3);
                 PonPixel (Stars[Star].x-2, Stars[Star].y, 2);
                 PonPixel (Stars[Star].x+2, Stars[Star].y, 2);
                 PonPixel (Stars[Star].x, Stars[Star].y-2, 2);
                 PonPixel (Stars[Star].x, Stars[Star].y+2, 2);
                 break;
     }
     if (Stars[Star].Ciclos++ > 2) {
        Stars[Star].Ciclos = 0;
        Stars[Star].Estado++;
        }
}
