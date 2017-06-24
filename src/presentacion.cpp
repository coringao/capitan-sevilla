#include "presentacion.h"

volatile int tCounterPres = 0;
volatile int tKeyPausePres = 0;
static void tCounterUpdaterPres() {
    tCounterPres++;
    if (tKeyPausePres)
       tKeyPausePres++;
}
END_OF_STATIC_FUNCTION(tCounterUpdaterPres);
LOCK_FUNCTION(tCounterUpdaterPres);
LOCK_VARIABLE(tCounterPres);

Presentacion::Presentacion()
{
   install_int_ex(tCounterUpdaterPres, BPS_TO_TIMER(TEMPORIZADOR));
   LogWrite ("Creada Presentación.\n");
}

Presentacion::~Presentacion()
{
   remove_int (tCounterUpdaterPres);
   LogWrite ("Destruída Presentación.\n");
}


int Presentacion::CEZTeam(bool Comprueba)
{
    DATAFILE *FrameLogo[12];
    DATAFILE *CuboSolo;
    DATAFILE *HaloCubo;
    DATAFILE *Tuberia;
    char Titulo [256], Temp[256];
    int FrameAct, Velar, Ciclos, VelarSent, HaloTrans, HaloSent;
    bool Salir = false;

    for (int n = 0; n < 12; n++) {
        sprintf (Temp, "DFBM_CEZT_0");
        if (n < 10)
           strcat (Temp, "0");
        sprintf (Titulo, "%s%i", Temp, n);
        LogWrite ("Cargando %s...", Titulo);
        FrameLogo[n] = load_datafile_object ("./data/bitmaps1.dat",Titulo);
        LogWrite ("OK.\n");
        if (!FrameLogo[n])
           return 1;
        }
    LogWrite ("Cargando DFBM_CEZT_CS...");
    CuboSolo = load_datafile_object ("./data/bitmaps1.dat","DFBM_CEZT_CS");
    LogWrite ("OK.\nCargando DFBM_CEZT_HC...");
    HaloCubo = load_datafile_object ("./data/bitmaps1.dat","DFBM_CEZT_HC");
    LogWrite ("OK.\nCargando DFBM_CEZT_TUB...");
    Tuberia = load_datafile_object ("./data/bitmaps1.dat","DFBM_CEZT_TUB");
    LogWrite ("OK.\n");

    if (!HaloCubo || !CuboSolo)
       return 1;

    FrameAct = 0;
    Velar = 255;
    VelarSent = -2;
    HaloSent = 5;
    HaloTrans = 0;
    Ciclos = 0;
    maxSkip = 6;
    curSkip = 0;
    tCounterPres = 0;

    H->LocateBuffer (640, 480);

    do {

       if (tCounterPres > 0) {
          do {
             if (Comprueba && !Salir)
                 Salir = H->Esc();

             Salir = Salir || close_button_pressed;

              if (++Ciclos > 290)
                 VelarSent = 2;

              if (!(Ciclos % 3))
                 if (++FrameAct >= 12)
                    FrameAct = 0;

              if (VelarSent) {
                 Velar += VelarSent;
                 if (Velar <= 0) {
                    VelarSent = 0;
                    Velar = 0;
                    }
                 }

              HaloTrans += HaloSent;
              if (HaloTrans < 0) {
                 HaloTrans = 0;
                 HaloSent *= -1;
                 }
              if (HaloTrans > 254) {
                 HaloTrans = 254;
                 HaloSent *= -1;
                 }

              tCounterPres--;
              curSkip++;
              if (curSkip >= maxSkip) {
                 tCounterPres = 0;
                 break;
                 }

          } while (tCounterPres > 0);
          needsRefresh = TRUE;
       }

       if (needsRefresh) {
          needsRefresh = FALSE;

          H->cls(0);
          H->Dibuja2x(0,0,Tuberia->dat);
          H->Dibuja_TransAdd(0,0,HaloCubo->dat, HaloTrans);
          H->Dibuja2x(0,0,CuboSolo->dat);

          H->Dibuja2x(0, 0, FrameLogo[FrameAct]->dat);
          if (Velar)
             H->Vela (Velar,0,0,640,480);

//          H->TextNum (Ciclos, 0, 0, 7, 0);

          H->VuelcaBuffer();
       }

    } while (!Salir && !(Velar >= 255 && VelarSent > 0));

    for (int n = 0; n < 12; n++)
        unload_datafile_object (FrameLogo[n]);

    return 0;
}

int Presentacion::CEZRD(bool Comprueba)
{
  int Velar, VelarSent, Ciclos;
  bool Salir = false;
  DATAFILE *Logo;

  LogWrite ("Cargando DFBM_CEZRD...");
  Logo = load_datafile_object ("./data/bitmaps1.dat","DFBM_CEZRD");
  LogWrite ("OK.\n");

  Velar = 255;
  VelarSent = -2;
  Ciclos = 0;
  maxSkip = 6;
  curSkip = 0;
  tCounterPres = 0;

  H->LocateBuffer (640, 480);
  H->cls(0);
  H->VuelcaBuffer();
        if (/*Ciclos == 30 && */!Comprueba)
            SFx->PlayMusic ("DFMUS_INTROLORD", false);

  do {

    if (tCounterPres > 0) {
      do {

        if (Ciclos == 30 && !Comprueba)
            SFx->PlayMusic ("DFMUS_INTROLORD", false);

        if (++Ciclos > 290)
          VelarSent = 2;

        if (VelarSent) {
          Velar += VelarSent;
          if (Velar <= 0) {
            VelarSent = 0;
            Velar = 0;
          }

       if (Comprueba && !Salir)
         Salir = H->Esc();

       Salir = Salir || close_button_pressed;

        }

        tCounterPres--;
        curSkip++;
        if (curSkip >= maxSkip) {
          tCounterPres = 0;
          break;
        }
      } while (tCounterPres > 0);
      needsRefresh = TRUE;
    }

    if (needsRefresh) {
      needsRefresh = FALSE;

      H->DibujaCentr (Logo->dat);
      if (Velar)
        H->Vela (Velar,0,0,640,480);

      H->VuelcaBuffer();
    }

  } while (!Salir && !(Velar >= 255 && VelarSent > 0));

  unload_datafile_object (Logo);

  return 0;
}

void Presentacion::IntroComic()
{
  int Ciclos, CiclosMax;
  FraseComic Frases[] = {
      {-18, 54, 3}, {306, 244, 4}, {420, 76, 2}, {-346, 188, 2},
      {-74, 138, 3}, {80, 212, 4}, {252, 76, 10}, {-356, 152, 1}, {350, 232, 3}, {524, 104, 7},
      {104, 278, 2}, {228, 216, 5}, {352, 172, 4}, {476, 120, 6},
      {-68, 56, 3}, {145, 222, 2}, {558, 114, 4},
      {272, 84, 3}, {468, 190, 5},
      {-120, 32, 3}, {96, 94, 4}, {180, 132, 2}, {344, 96, 4}, {352, 164, 1}, {496, 112, 2}, {-120, 366, 4},
      {410, 180, 3}, {528, 210, 4},
      {-72, 60, 3}, {528, 220, 6}, {528, 318, 2},
      {-66, 35, 4}, {246, 198, 5},
      {104, 28, 5}, {94, 274, 4}, {390, 22, 3}, {360, 162, 5}, {280, 236, 2}, {558, 198, 5}, {538, 388, 5},
      {272, 56, 4}, {-416, 98, 10}, {-416, 240, 3}, {-416, 302, 3}, {-416, 350, 2}
  };
//  int ComicPause[] = {388, 876, 428, 322, 230, 264, 1014, 214, 514, 622, 738, 624};
  int ComicPause[] = {388, 840, 464, 322, 250, 284, 1014, 214, 514, 622, 738, 624};

  int NFrases [] = {4, 6, 4, 3, 0, 2, 7, 2, 3, 2, 7, 5};

  DATAFILE *Logo;
  char Titulo[20];

  int Cont = 0;

  H->LocateBuffer (640, 480);
  H->cls(0);
  H->VuelcaBuffer();

  H->SetFont_af ("TTF_EVILGENIUS", 14);


  for (int n = 0, PFrase = 0, PDFrase = 0; n < 12 && !close_button_pressed; n++) {

      CiclosMax = 0;

      if (n != 5) {
        H->Desatura ();
        H->Vela (192, 0, 0, 640, 480);
        }
       else
        H->cls(0);
      H->CopyFondo();


      sprintf (Titulo, "DFBM_INTRO_0%i", n);
      if (n < 10)
        sprintf (Titulo, "DFBM_INTRO_00%i", n);
      LogWrite ("Cargando %s...", Titulo);
      Logo = load_datafile_object ("./data/bitmaps1.dat",Titulo);
      if (Logo)
        LogWrite ("OK.\n");
      else
        LogWrite ("ERROR!!!!");

      Ciclos = 0;
      maxSkip = 6;
      curSkip = 0;
      tCounterPres = 0;

      H->DibujaCentr (Logo->dat);
      for (int m = 0; m < NFrases [n]; m++, PDFrase++)
          for (int o = 0; o < Frases[PDFrase].NFrases; o++, PFrase++) {
//              CiclosMax += (strlen(SMS->ReadMens(100 + PFrase))*2);
//                LogWrite ((char *) "%i (%i): %s (%i, %i)\n",m, 100 + PFrase, (char*)SMS->ReadMens(100 + PFrase), Frases[PDFrase].X, Frases[PDFrase].Y);
                if (Frases[PDFrase].X > 0)
                    H->TextOutCentre_aa_ex (SMS->ReadMens(100 + PFrase), Frases[PDFrase].X, Frases[PDFrase].Y + o * H->TextHeight_af(), 0, 0, 0);
                  else
                    H->TextOut_aa (SMS->ReadMens(100 + PFrase), -Frases[PDFrase].X, Frases[PDFrase].Y + o * H->TextHeight_af(), 0, 0, 0);
      }

      switch (n) {
            case 4:
/*                if (!Suena[DF_SEXPL]) {
                    LogWrite ("Sample explosión.\n");
                    Logo = load_datafile_object ("./data/sfx.dat", "DF_SEXPL");
                    play_sample ((SAMPLE*)Logo->dat, Config->VerVolMus(), 127, 1000, 0);
                    }
                Suena[DF_SEXPL] = true;*/
                H->Terremoto (1);
//                CiclosMax = 230;
                break;
            case 5:
                Suena[DF_SEXPL] = false;
                H->Terremoto (0, 0);
//                CiclosMax += 30;
                break;
            case 7:
                H->SetFont_af ("TTF_CAPITAN", 75);
                H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(98), 320, 92, 0, 0, 0);
                H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(98), 320, 96, 0, 0, 0);
                H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(98), 318, 94, 0, 0, 0);
                H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(98), 322, 94, 0, 0, 0);
                H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(98), 326, 100, 252, 88, 0);
                H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(98), 322, 96, 0, 0, 0);
                H->TextoPartido ((char*) SMS->ReadMens(98), 320, 94, 255,255,255, 252,224,168);

                H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(99), 320, 288, 0, 0, 0);
                H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(99), 320, 292, 0, 0, 0);
                H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(99), 318, 290, 0, 0, 0);
                H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(99), 322, 290, 0, 0, 0);
                H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(99), 326, 296, 252, 88, 0);
                H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(99), 322, 292, 0, 0, 0);
                H->TextoPartido ((char*) SMS->ReadMens(99), 320, 290, 255,255,255, 252,224,168);
                H->SetFont_af ("TTF_EVILGENIUS", 14);
      }

//      LogWrite ("%i: %i\n", n, CiclosMax);
      CiclosMax = ComicPause [n];

      H->ClearKeyBuf();

      if (n != 5)
        H->FundidoBM (20);
       else
        H->CopyFondo();

      do {

        if (tCounterPres > 0) {
          do {
              ++Ciclos;

              if (H->Esc()) {
                unload_datafile_object (Logo);
                return;
                }

            if (n == 4 && Ciclos > 180) {
                H->RectFill (0, 0, 640, 480, 7);
                H->CopyFondo();
                H->Terremoto (0, 0);
            }

            tCounterPres--;
            curSkip++;
            if (curSkip >= maxSkip) {
              tCounterPres = 0;
              break;
            }
          } while (tCounterPres > 0);
          needsRefresh = TRUE;
        }

        if (needsRefresh) {
          needsRefresh = FALSE;

          H->DrawFondoBuffer(0, 0, 640, 480, 0, 0);

          //H->TextNum(Ciclos,0,0,7,0);

          if (n == 4 && Ciclos > 120)
            H->Ilumina (4 * Ciclos  - 480, 0, 0, 640, 480);

          if (n == 5 && Ciclos < 30)
             H->Ilumina (255 - (255 * Ciclos / 30), 0, 0, 640, 480);

          H->VuelcaBuffer();

          if (H->Esc()) {
             unload_datafile_object (Logo);
             return;
             }
        }

      } while (/*!H->KeyPressed() && */Ciclos < CiclosMax);

      unload_datafile_object (Logo);
  }
  H->Fundido (-1);
  Suena[DF_SEXPL] = false;
}

int Presentacion::FinComic ()
{
    int Ciclos, CiclosMax;

    FraseComic Frases[] = {
        {-250, 32, 8},

        {-48, 16, 3},
        {530, 150, 4},
        {314, 80, 5},
    };

    const int NVinetas = 2;
    const int FrInicial = 307;

    int NFrases [NVinetas] = {1, 3};
    int PauseComic [NVinetas] = {500, 500};
    DATAFILE *Logo;
    char Titulo[20];

    LogWrite ("¡¡¡FIN DEL JUEGO!!!\n\n");

    //AM->Destroy(true);


    H->CopyFondo();
    H->Fundido (-1, 5);
    H->Terremoto(0);

    H->cls(0);
    H->VuelcaBuffer();

    H->SetFont_af ("TTF_EVILGENIUS", 14);

    for (int n = 0, PFrase = 0, PDFrase = 0; n < NVinetas && !close_button_pressed; n++) {

      CiclosMax = 0;

      H->Desatura ();
      H->Vela (192, 0, 0, 640, 480);
      H->CopyFondo();

      sprintf (Titulo, "DFBM_ENDCOMIC_00%i", n);

      LogWrite ("Cargando %s...", Titulo);
      Logo = load_datafile_object ("./data/bitmaps1.dat",Titulo);
      LogWrite ("OK.\n");

      Ciclos = 0;
      maxSkip = 6;
      curSkip = 0;
      tCounterPres = 0;

      H->DibujaCentr (Logo->dat);
      for (int m = 0; m < NFrases [n]; m++, PDFrase++)
          for (int o = 0; o < Frases[PDFrase].NFrases; o++, PFrase++) {
//              CiclosMax += (strlen(SMS->ReadMens(FrInicial + PFrase))*2);
//                LogWrite ((char *) "%i (%i): %s (%i, %i)\n",m, 100 + PFrase, (char*)SMS->ReadMens(100 + PFrase), Frases[PDFrase].X, Frases[PDFrase].Y);
                if (Frases[PDFrase].X > 0)
                    H->TextOutCentre_aa_ex (SMS->ReadMens(FrInicial + PFrase), Frases[PDFrase].X, Frases[PDFrase].Y + o * H->TextHeight_af(), 0, 0, 0);
                  else
                    H->TextOut_aa (SMS->ReadMens(FrInicial  + PFrase), -Frases[PDFrase].X, Frases[PDFrase].Y + o * H->TextHeight_af(), 0, 0, 0);
      }

      CiclosMax = PauseComic [n];

      H->ClearKeyBuf();

      H->FundidoBM (20);

      do {

        if (tCounterPres > 0) {
          do {
              ++Ciclos;

              if (H->Esc()) {
                unload_datafile_object (Logo);
                while (H->Esc());
                return 1;
                }

            tCounterPres--;
            curSkip++;
            if (curSkip >= maxSkip) {
              tCounterPres = 0;
              break;
            }
          } while (tCounterPres > 0);
          needsRefresh = TRUE;
        }

        if (needsRefresh) {
          needsRefresh = FALSE;

//          H->TextNum(Ciclos,0,0,7,0);
          H->DrawFondoBuffer(0, 0, 640, 480, 0, 0);

          H->VuelcaBuffer();

              if (H->Esc()) {
                unload_datafile_object (Logo);
                return 1;
                }
        }

      } while (/*!H->KeyPressed() && */Ciclos < CiclosMax);

      unload_datafile_object (Logo);
  }
  H->Fundido (-1);

  return (H->Esc() + close_button_pressed);
}

void Presentacion::GiraPeriodico(void *Pointer)
{
    char angle;
    float n;
    for (n = 6, angle = 255; n > 2 && !close_button_pressed && !H->Esc() ; n -= .1) {
        H->DrawRotate (Pointer, 320, 240, angle, n);
        H->VuelcaBuffer();
        H->DrawFondoBuffer(0,0,640,480);
        H->Rest (10);
        angle -= 15;
    }

    H->DrawFondoBuffer(0,0,640,480);
    H->Dibuja2x (86, 6, Pointer);
    H->CopyFondo();

    H->VuelcaBuffer();
}

void Presentacion::TextoPeriodico (void *BitMap, int Tamano, const void *Texto, int &y, int x)
{
    H->SetFont_af ("TTF_BOOKMAN", Tamano);
    if (x < 0)
        x = 105 - H->TextLength_af ((char*) Texto) / 2;

    H->TextOut_aa ((char*) Texto, x, y, 0, 0, 0, -1, -1, -1, BitMap);
    y += H->TextHeight_af ();
}


int Presentacion::FinPeriodico ()
{

    DATAFILE *Logo, *Text;
    int y;
    int xFoto[] = {0, 40, 81, 122, 164};
    char Titulo [32];

    H->LocateBuffer(640, 480);
    H->cls (0);
    H->CopyFondo();

    Logo = load_datafile_object ("./data/bitmaps1.dat", "DFBM_PERIODICO");

    y = 50;

    TextoPeriodico (Logo->dat, 27, SMS->ReadMens (327), y);
    TextoPeriodico (Logo->dat, 27, SMS->ReadMens (328), y);

    TextoPeriodico (Logo->dat, 5, SMS->ReadMens (329), y);


    H->DrawRotate (Logo->dat, 320, 240, 23, 2);
    H->Vela(20, 0, 0, 640, 480);
    H->DrawRotate (Logo->dat, 320, 240, 232, 2);
    H->Vela(20, 0, 0, 640, 480);

    H->FundidoBM();

    H->DrawRotate (Logo->dat, 320, 240, 23, 2);
    H->Vela(20, 0, 0, 640, 480);
    H->DrawRotate (Logo->dat, 320, 240, 232, 2);
    H->Vela(20, 0, 0, 640, 480);
    H->CopyFondo();

    GiraPeriodico (Logo->dat);

    Text = load_datafile_object ("./data/bitmaps1.dat", "DFBM_PERTXT");

    for (int n = 0; n < 9; n ++) {

        for (int pausa = 0; pausa < 3000; pausa++) {
            H->Rest(1);
            if (H->Esc() || close_button_pressed)
                return 1;
            }

        unload_datafile_object (Logo);
        sprintf (Titulo, "DFBM_PERIOD_00%i", (int) (rand() % 3));
        Logo = load_datafile_object ("./data/bitmaps1.dat", Titulo);
        y = 50;

        switch (n) {
            case 0:
                TextoPeriodico (Logo->dat, 20, SMS->ReadMens (42), y);
                TextoPeriodico (Logo->dat, 10, SMS->ReadName (5), y);

                TextoPeriodico (Logo->dat, 20, SMS->ReadMens (40), y);
                TextoPeriodico (Logo->dat, 10, SMS->ReadName (8), y);
                y += 20;

                H->Blit (Text->dat, Logo->dat, 0, y, 0, y, 211, 234 - y);
                H->Dibuja("bitmaps1", "DFBM_FOTO_000", xFoto[0] - 2, y + 10, Logo->dat);
                break;
            case 1:
                TextoPeriodico (Logo->dat, 15, SMS->ReadMens (55), y);
                TextoPeriodico (Logo->dat, 10, SMS->ReadName (8), y);
                y += 20;

                H->Blit (Text->dat, Logo->dat, 0, y, 0, y, 211, 234 - y);
                H->Dibuja("bitmaps1", "DFBM_FOTO_001", xFoto[3], y + 20, Logo->dat);
                break;
            case 2:
                TextoPeriodico (Logo->dat, 20, SMS->ReadMens (41), y);
                TextoPeriodico (Logo->dat, 10, SMS->ReadName (8), y);
                TextoPeriodico (Logo->dat, 10, SMS->ReadName (17), y);
                TextoPeriodico (Logo->dat, 10, SMS->ReadName (3), y);
                y += 20;

                H->Blit (Text->dat, Logo->dat, 0, y, 0, y, 211, 234 - y);
                H->Dibuja("bitmaps1", "DFBM_FOTO_002", xFoto[2], y, Logo->dat);
                break;
            case 3:
                TextoPeriodico (Logo->dat, 20, SMS->ReadMens (43), y);
                TextoPeriodico (Logo->dat, 10, SMS->ReadName (4), y);
                TextoPeriodico (Logo->dat, 10, SMS->ReadName (3), y);
                TextoPeriodico (Logo->dat, 10, SMS->ReadName (5), y);
                y += 20;

                H->Blit (Text->dat, Logo->dat, 0, y, 0, y, 211, 234 - y);
                H->Dibuja("bitmaps1", "DFBM_FOTO_003", xFoto[1], y, Logo->dat);
                break;
            case 4:
                TextoPeriodico (Logo->dat, 20, SMS->ReadMens (44), y);
                TextoPeriodico (Logo->dat, 10, SMS->ReadName (8), y);
                TextoPeriodico (Logo->dat, 10, SMS->ReadName (19), y);
                TextoPeriodico (Logo->dat, 10, SMS->ReadName (3), y);
                TextoPeriodico (Logo->dat, 10, SMS->ReadName (11), y);
                y += 10;

                H->Blit (Text->dat, Logo->dat, 0, y, 0, y, 211, 234 - y);
                H->Dibuja("bitmaps1", "DFBM_FOTO_004", xFoto[1], y, Logo->dat);
                H->Dibuja("bitmaps1", "DFBM_FOTO_005", xFoto[4], y + 30, Logo->dat);
                break;

            case 5:
                TextoPeriodico (Logo->dat, 20, SMS->ReadMens (54), y);
                TextoPeriodico (Logo->dat, 10, SMS->ReadName (0), y);
                y += 20;

                H->Blit (Text->dat, Logo->dat, 0, y, 0, y, 211, 234 - y);
                H->Dibuja("bitmaps1", "DFBM_FOTO_006", 0, y + 60, Logo->dat);
                break;

            case 6:
                TextoPeriodico (Logo->dat, 15, SMS->ReadMens (49), y);
                TextoPeriodico (Logo->dat, 10, SMS->ReadName (21), y);
                y += 20;

                H->Blit (Text->dat, Logo->dat, 0, y, 0, y, 211, 234 - y);
                H->Dibuja("bitmaps1", "DFBM_FOTO_007", xFoto[3], y + 20, Logo->dat);
                break;

            case 7:
                TextoPeriodico (Logo->dat, 15, SMS->ReadMens (45), y);
                TextoPeriodico (Logo->dat, 10, SMS->ReadName (13), y);
                y += 20;

                H->Blit (Text->dat, Logo->dat, 0, y, 0, y, 211, 234 - y);
                H->Dibuja("bitmaps1", "DFBM_FOTO_008", xFoto[3], y + 20, Logo->dat);
                break;

            case 8:
                TextoPeriodico (Logo->dat, 20, SMS->ReadMens (26), y);
                TextoPeriodico (Logo->dat, 10, SMS->ReadMens (27), y);
                TextoPeriodico (Logo->dat, 10, SMS->ReadMens (29), y);
                y += 20;

                H->Blit (Text->dat, Logo->dat, 0, y, 0, y, 211, 234 - y);
                H->Dibuja("bitmaps1", "DFBM_FOTO_006", 0, y + 60, Logo->dat);
                break;
        }

        GiraPeriodico (Logo->dat);
    }


    unload_datafile_object (Logo);
    unload_datafile_object (Text);

    for (int pausa = 0; pausa < 2000 && !close_button_pressed && !H->Esc(); pausa++)
         H->Rest(1);

    H->Fundido (-1);


    return (H->Esc() + close_button_pressed);
}

int Presentacion::FinFeria ()
{
    ActorManager AMF;
    int Ciclos = 0;
    H->cls(0);
    H->LocateBuffer ();

    AMF.NuevoAct (MESACAPI, 0, 0, 0, 158, 206, 5, 3);
    AMF.NuevoAct (MARIANOBEBE, 0, 0, 0, 316, 212, 5, 3);
    AMF.NuevoAct (SIRFRED, 0, 0, 0, 70, 136, 5, 3);

    H->Dibuja2x("bitmaps1", "DFBM_FERIA", 0, 0);

    H->SetFont_af ("TTF_CAPITAN", 40);

    H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(52), 226, 306, 252, 88, 0);
    H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(52), 222, 302, 0, 0, 0);
    H->TextoPartido ((char*) SMS->ReadMens(52), 220, 300, 255,255,255, 252,224,168);

    H->TextOutCentre_aa_ex ((char*) SMS->ReadMens (53), 226, 346, 252, 88, 0);
    H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(53), 222, 342, 0, 0, 0);
    H->TextoPartido ((char*) SMS->ReadMens(53), 220, 340, 255,255,255, 252,224,168);

    H->CopyFondo();

    do {

        if (tCounterPres > 0) {
            do {
              ++Ciclos;
              AMF.ActualizaMov (H->Tecla());
              tCounterPres--;
              curSkip++;
              if (curSkip >= maxSkip) {
                tCounterPres = 0;
                break;
              }
            } while (tCounterPres > 0);
            needsRefresh = TRUE;
        }

        if (needsRefresh) {
            needsRefresh = FALSE;

            if (Config->VerScanL()) {
                H->Dibuja2x("bitmaps1", "DFBM_FERIA", 0, 0);
                H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(52), 226, 306, 252, 88, 0);
                H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(52), 222, 302, 0, 0, 0);
                H->TextoPartido ((char*) SMS->ReadMens(52), 220, 300, 255,255,255, 252,224,168);

                H->TextOutCentre_aa_ex ((char*) SMS->ReadMens (53), 226, 346, 252, 88, 0);
                H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(53), 222, 342, 0, 0, 0);
                H->TextoPartido ((char*) SMS->ReadMens(53), 220, 340, 255,255,255, 252,224,168);}
              else
                H->DrawFondoBuffer();

            AMF.Dibuja(2, true);

            if (Ciclos > (TEMPORIZADOR * 30) - 255) {
                H->Vela (255 - (TEMPORIZADOR * 30 - Ciclos),
                         0, 0, Config->VerResAncho(), Config->VerResAlto());
                if (Config->VerVolMus() - Ciclos + ((TEMPORIZADOR * 30) - 255) >= 0)
                    set_volume (Config->VerVolMus() - Ciclos + ((TEMPORIZADOR * 30) - 255), 0);
            }

            H->VuelcaBuffer();
        }

      } while (/*!H->KeyPressed() && */Ciclos < TEMPORIZADOR * 30 && !H->Esc());

      SFx->StopSound();

      return (H->Esc() + close_button_pressed);

}

int Presentacion::Run(bool Comprueba)
{
    SFx->StopSound();
    if (Comprueba)
        Comprueba = !Config->VerIntro();
    CEZRD(Comprueba);
    if (close_button_pressed)
       return CIERRAVENT;
    CEZTeam(Comprueba);
    if (close_button_pressed)
       return CIERRAVENT;
    if (Comprueba) {
       SFx->StopSound();
       return TODOOK;
       }
    IntroComic();
    H->Terremoto (0,0);
    if (close_button_pressed)
       return CIERRAVENT;
    SFx->StopSound();
    return TODOOK;
}
