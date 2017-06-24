#include "creditos.h"

volatile int ContCred = 0;
static void ContUpDate() {
    ContCred++;
}
END_OF_STATIC_FUNCTION(ContUpDate);
LOCK_FUNCTION(ContUpDate);
LOCK_VARIABLE(ContCred);

Creditos::Creditos()
{
   install_int_ex(ContUpDate, BPS_TO_TIMER(10));
}

Creditos::~Creditos()
{
   remove_int (ContUpDate);
}

void Creditos::PreparaBM ()
{
     char Titulo[255];
     int x;
     if (++BitMap >= MaxBitMap)
        BitMap = 0;
     x = BitMap % 2;
     if (Fase == 0)
        x = -1;
     H->cls(0);
     if (BitMap < 10)
        sprintf (Titulo,"DFBM_CREDIT_00%i",BitMap);
      else
        sprintf (Titulo,"DFBM_CREDIT_0%i",BitMap);
     H->DibujaCentr ("bitmaps1",Titulo, x);
}

void Creditos::Texto (const char* Texto, int x, int y, int RI, int GI, int BI)
{
     if (x < 0)
        x = 320;

     H->TextOutCentre_aa_ex(Texto, x - 1, y, 0,0,0);
     H->TextOutCentre_aa_ex(Texto, x + 1, y, 0,0,0);
     H->TextOutCentre_aa_ex(Texto, x, y - 1, 0,0,0);
     H->TextOutCentre_aa_ex(Texto, x, y + 1, 0,0,0);
     H->TextOutCentre_aa_ex(Texto, x - 1, y - 1, 0,0,0);
     H->TextOutCentre_aa_ex(Texto, x + 1, y + 1, 0,0,0);
     H->TextOutCentre_aa_ex(Texto, x + 1, y - 1, 0,0,0);
     H->TextOutCentre_aa_ex(Texto, x - 1, y + 1, 0,0,0);
     H->TextOutCentre_aa_ex(Texto, x, y, RI, GI, BI);

     if (y == TextY)
        TextY += H->TextHeight_af();
}


void Creditos::PreparaTexto ()
{
     TextY = 0;
     switch (Fase) {
            case 0:
                 H->SetFont_af ("TTF_CAPITAN", 60);
                 Texto (SMS->ReadMens (2), -1, TextY); // CAPITAN SEVILLA

                 H->ResizeFont_af (15);
                 Texto (SMS->ReadMens (38), -1, TextY); // EL REMAKE

                 H->ResizeFont_af (40);
                 TextY+= 100;
                 Texto (SMS->ReadMens (39), -1, TextY, 255, 255, 255); // Diseño

                 TextY += 20;

                 H->ResizeFont_af (30);
                 Texto (SMS->ReadName (0), -1, TextY); // CEZ TEAM
                 break;

            case 1:
                 TextY = 140;
                 H->ResizeFont_af (30);
                 Texto (SMS->ReadMens (42), 480 - 320 * (BitMap % 2), TextY, 255, 255, 255); // GRAFICOS
                 TextY += 20;
                 H->ResizeFont_af (25);
                 Texto (SMS->ReadName (5), 480 - 320 * (BitMap % 2), TextY);

                 H->ResizeFont_af (30);
                 TextY += 60;
                 Texto (SMS->ReadMens (40), 480 - 320 * (BitMap % 2), TextY, 255, 255, 255); // PROGRAMACION
                 H->ResizeFont_af (25);
                 TextY += 20;
                 Texto (SMS->ReadName (8), 480 - 320 * (BitMap % 2), TextY);
                 break;

           case 2:
                 TextY = 170;
                 H->ResizeFont_af (30);
                 Texto (SMS->ReadMens (55), 480 - 320 * (BitMap % 2), TextY, 255, 255, 255); // TRABAJO GRÁFICO ADICIONAL
                 H->ResizeFont_af (25);
                 TextY += 20;
                 Texto (SMS->ReadName (8),  480 - 320 * (BitMap % 2), TextY);
                 break;

           case 3:
                 TextY = 160;
                 H->ResizeFont_af (30);
                 TextY += 60;
                 Texto (SMS->ReadMens (41), 480 - 320 * (BitMap % 2), TextY, 255, 255, 255); // EFECTOS VISUALES
                 TextY += 20;
                 H->ResizeFont_af (25);
                 Texto (SMS->ReadName (8), 480 - 320 * (BitMap % 2), TextY);
                 Texto (SMS->ReadName (17), 480 - 320 * (BitMap % 2), TextY);
                 Texto (SMS->ReadName (3), 480 - 320 * (BitMap % 2), TextY);
                 break;

           case 4:
                 H->ResizeFont_af (30);
                 TextY = 100;
                 Texto (SMS->ReadMens (43), 480 - 320 * (BitMap % 2), TextY, 255, 255, 255); // MUSICA
                 TextY += 20;
                 H->ResizeFont_af (25);
                 Texto (SMS->ReadName (4), 480 - 320 * (BitMap % 2), TextY);
                 Texto (SMS->ReadName (3), 480 - 320 * (BitMap % 2), TextY);
                 Texto (SMS->ReadName (5), 480 - 320 * (BitMap % 2), TextY);

                 H->ResizeFont_af (30);
                 TextY+= 30;
                 Texto (SMS->ReadMens (44), 480 - 320 * (BitMap % 2), TextY, 255, 255, 255); // EFECTOS SONOROS
                 TextY += 20;
                 H->ResizeFont_af (25);
                 Texto (SMS->ReadName (8), 480 - 320 * (BitMap % 2), TextY);
                 Texto (SMS->ReadName (19), 480 - 320 * (BitMap % 2), TextY);
                 Texto (SMS->ReadName (3), 480 - 320 * (BitMap % 2), TextY);
                 Texto (SMS->ReadName (11), 480 - 320 * (BitMap % 2), TextY);
                 break;

          case 5:
                 TextY = 0;
                 H->ResizeFont_af (30);
                 Texto (SMS->ReadMens (57), 480 - 320 * (BitMap % 2), TextY, 255, 255, 255); // EFECTOS SONOROS
                 TextY += 20;
                 H->ResizeFont_af (15);
                 for (int n = 0; n < 25; n++)
                    Texto (SMS->ReadNameS (n), 480 - 320 * (BitMap % 2), TextY);
                 break;

          case 6:
                 H->ResizeFont_af (30);
                 TextY = 20;
                 Texto (SMS->ReadMens (54), 480 - 320 * (BitMap % 2), TextY, 255, 255, 255);         // BETA TESTING
                 TextY += 30;
                 H->ResizeFont_af (20);
                 for (int n = 1; n < 14; n++)                              // TODOS LOS NOMBRES
                     Texto (SMS->ReadName (n), 480 - 320 * (BitMap % 2), TextY);
                 Texto ("..." , 480 - 320 * (BitMap % 2), TextY);

                 break;

          case 7:
                 H->ResizeFont_af (30);
                 TextY = 20;
                 Texto (SMS->ReadMens (54), 480 - 320 * (BitMap % 2), TextY, 255, 255, 255);         // BETA TESTING
                 TextY += 30;
                 H->ResizeFont_af (20);
                 Texto ("..." , 480 - 320 * (BitMap % 2), TextY);
                 for (int n = 14; n < 26; n++)                               // TODOS LOS NOMBRES
                     Texto (SMS->ReadName (n), 480 - 320 * (BitMap % 2), TextY);
                 Texto (SMS->ReadName (40), 480 - 320 * (BitMap % 2), TextY);
                 break;

          case 8:
                 TextY = 170;
                 H->ResizeFont_af (30);
                 Texto (SMS->ReadMens (49), 460 - 320 * (BitMap % 2), TextY, 255, 255, 255); // ILUSTRACIÓN PORTADA
                 H->ResizeFont_af (25);
                 TextY += 20;
                 Texto (SMS->ReadName (21),  460 - 320 * (BitMap % 2), TextY);
                 break;

          case 9:
                 TextY = 170;
                 H->ResizeFont_af (30);
                 Texto (SMS->ReadMens (45), 480 - 320 * (BitMap % 2), TextY, 255, 255, 255); // COORDINACION Y PRODUCCION
                 H->ResizeFont_af (25);
                 TextY += 20;
                 Texto (SMS->ReadName (13),  480 - 320 * (BitMap % 2), TextY);
                 break;

          case 10:
                H->ResizeFont_af (30);
                TextY = 90;
                Texto (SMS->ReadMens (46), 480 - 320 * (BitMap % 2), TextY, 255, 255, 255); // Basado en el juego ...
                TextY += 20;
                H->ResizeFont_af (25);
                Texto (SMS->ReadMens (2), 480 - 320 * (BitMap % 2), TextY);
                H->ResizeFont_af (30);
                TextY+= 30;
                Texto (SMS->ReadMens (47), 480 - 320 * (BitMap % 2), TextY, 255, 255, 255); // Realizado originalmente por
                H->ResizeFont_af (20);
                TextY += 10;
                for (int n = 26; n < 36; n++)                              // TODOS LOS NOMBRES
                    Texto (SMS->ReadName (n), 480 - 320 * (BitMap % 2), TextY);
                break;

          case 11:
                H->ResizeFont_af (30);
                TextY = 90;
                Texto (SMS->ReadMens (48), 480 - 320 * (BitMap % 2), TextY, 255, 255, 255); // Agradecimientos
                TextY += 10;
                H->ResizeFont_af (20);
                for (int n = 36; n < 40; n++)                               // TODOS LOS NOMBRES
                    Texto (SMS->ReadName (n), 480 - 320 * (BitMap % 2), TextY);
                Texto (SMS->ReadName (41), 480 - 320 * (BitMap % 2), TextY);
                break;
          case 12:
                H->ResizeFont_af (30);
                TextY = 90;
                Texto (SMS->ReadMens (56), 480 - 320 * (BitMap % 2), TextY, 255, 255, 255); // Dedicatorias
                TextY += 20;
                H->ResizeFont_af (20);
                Texto ("Por parte de Benway...", 480 - 320 * (BitMap % 2), TextY, 255, 255, 255);
                TextY += 10;
                Texto (SMS->ReadDedic (0), 480 - 320 * (BitMap % 2), TextY, 255, 255, 255);
                Texto (SMS->ReadDedic (1), 480 - 320 * (BitMap % 2), TextY);
                Texto (SMS->ReadDedic (2), 480 - 320 * (BitMap % 2), TextY);
                Texto (SMS->ReadDedic (3), 480 - 320 * (BitMap % 2), TextY);
                TextY += 20;
                Texto ("Por parte de LordFred...", 480 - 320 * (BitMap % 2), TextY, 255, 255, 255);
                TextY += 10;
                Texto (SMS->ReadDedic (4), 480 - 320 * (BitMap % 2), TextY, 255, 255, 255);
                Texto (SMS->ReadDedic (5), 480 - 320 * (BitMap % 2), TextY);
                Texto (SMS->ReadDedic (6), 480 - 320 * (BitMap % 2), TextY);
                Texto (SMS->ReadDedic (7), 480 - 320 * (BitMap % 2), TextY);
                TextY += 10;

                Fase = -255;
          }
}

void Creditos::Run ()
{
     SFx->PlayMusic("DFMUS_CREDF",true);
     BitMap = MaxBitMap - 1;
     H->LocateBuffer (640, 480);
     H->cls(0);
     ContCred = 0;
     Fase = 0;
     while (Fase >= 0 && !close_button_pressed) {
           H->CopyFondo();
           PreparaBM();
           PreparaTexto();
           H->FundidoBM(3);
           while (ContCred < 100 && !close_button_pressed)
                if (H->Esc())
                    return;
           ContCred = 0;
           Fase++;
           }
    H->Fundido (-1, 2, true);

}

