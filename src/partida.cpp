#include "partida.h"
#include "partida.h"

volatile int timerCounter = 0;
static void timerCounterUpdater() {
    timerCounter++;
    ++cfps;
}
END_OF_STATIC_FUNCTION(timerCounterUpdater);

LOCK_FUNCTION(timerCounterUpdater);
LOCK_VARIABLE(timerCounter);

Partida::Partida()
{
 LogWrite ("Creando nueva partida.\n");
 AM = new ActorManager;
 Backgd = new Background;
 Marcador = new Marcadores;
}

Partida::~Partida()
{
 LogWrite ("Borrando datos de partida y devolviendo memoria.\n");
 delete Backgd;
 delete AM;
 delete Marcador;
 remove_int (timerCounterUpdater);
 BorraBus();
}

void Partida::BorraBus()
{
  if (VJ.Autobus) {
    delete ((Guagua*)(VJ.Autobus));
    VJ.Autobus = NULL;
  }
}

int Partida::Init (int Attract)
{
   if (Attract)
     LogWrite ("Entrando en Attract Mode...\n");
   LogWrite((char*)"INICIALIZANDO PARTIDA.\nBuscando archivo de mapeado.\n");
   H->SetFont ("FNT_PANEL", NULL);
   if (GMundos->Load())
      return 1;

    fps = 0; cfps = 0; afps = TEMPORIZADOR; Media = 0; Ciclos = 0;

    Temporizador = 40;

    ContTextAM = 0;

    for (int n = 0; n < MaxSonido; n++)
        Suena[n] = false;

    VJ.Cheats = false;

    VJ.Fase = Config->VerFase();
    VJ.Vidas = 3;
    VJ.Morcillas = 0;
    VJ.QueHase = PUNO;
    VJ.QueHasia = SOPLAR;
    VJ.OSD = false;

    CiclosMuerto = 0;

    AttractMode = Attract;

    if (AttractMode)
       if (GameLogInit(AttractMode, VJ.Fase))
           return 1;

    MAct = GMundos->BuscaMundo(VJ.Fase);
    CAct = MAct->HabInic();


    Mariano = AM->NuevoAct (MARIANO, PARADO, 0, AMIGO, 0, 0, (140 - 20 * Config->VerDif()));
    VJ.Jugador = (void*) Mariano;
    Mariano->SetY (TAMTILE * (TILESALTO - 1) - Mariano->VerLongY());

    Situa();
    install_int_ex(timerCounterUpdater, BPS_TO_TIMER(Temporizador));
    H->ClearKeyBuf();
    H->LocateBuffer();

    VJ.Autobus = NULL;


    LogWrite ("PARTIDA INICIALIZADA CORRECTAMENTE.\n");

    return 0;
}

int Partida::Pausa()
{
    int Result = 0, RepCiclo = 1, Tecla, TamOp;

    if (AttractMode)
       return ABORT;


    CicloVisual();
    H->Vela (64);

    H->SetFont_af ("TTF_CAPITAN", 70);
    H->TextOutCentre_aa_ex (SMS->ReadMens(30), ANCHOPANTALLA / 2 + 6, 31, 252, 88, 0);
    H->TextOutCentre_aa_ex (SMS->ReadMens(30), ANCHOPANTALLA / 2 + 2, 27, 0, 0, 0);
    H->TextoPartido (SMS->ReadMens(30), ANCHOPANTALLA / 2, 25, 255,255,255, 252,224,168);
    H->CopyFondo();

    while (RepCiclo && !close_button_pressed) {

          Tecla = H->Tecla();

          if (Tecla & IZQUIERDA)
             Result = 1;
          if (Tecla & DERECHA)
             Result = 0;
          if (Tecla & SELEC || Tecla & FUEGO)
             RepCiclo = 0;

          if (Result)
             TamOp = 50;
           else
             TamOp = 20;

          H->ResizeFont_af (TamOp);
          H->TextOutCentre_aa_ex (SMS->ReadMens(31), ANCHOPANTALLA / 2 - ANCHOPANTALLA / 8 + 6 - 3 * (!Result), 101 + 15 * (!Result) - 3 * (!Result), 252, 88, 0);
          H->TextOutCentre_aa_ex (SMS->ReadMens(31), ANCHOPANTALLA / 2 - ANCHOPANTALLA / 8 + 2, 97 + 15 * (!Result), 0, 0, 0);
          H->TextoPartido (SMS->ReadMens(31), ANCHOPANTALLA / 2 - ANCHOPANTALLA / 8, 95 + 15 * (!Result), 255,255,255, 252,224,168);

          if (Result)
             TamOp = 20;
           else
             TamOp = 50;

          H->ResizeFont_af (TamOp);
          H->TextOutCentre_aa_ex (SMS->ReadMens(32), ANCHOPANTALLA / 2 + ANCHOPANTALLA / 8 + 6 - 3 * (Result), 101 + 15 * (Result) - 3 * (Result), 252, 88, 0);
          H->TextOutCentre_aa_ex (SMS->ReadMens(32), ANCHOPANTALLA / 2 + ANCHOPANTALLA / 8 + 2, 97 + 15 * (Result), 0, 0, 0);
          H->TextoPartido (SMS->ReadMens(32), ANCHOPANTALLA / 2 + ANCHOPANTALLA / 8, 95 + 15 * (Result), 255,255,255, 252,224,168);

          H->VuelcaBuffer();
          H->DrawFondoBuffer();
      }

      H->ClrFondo (0);
      Backgd->Genera(HAct->VerNum());
      H->DrawFondoBuffer(0,0,ANCHOPANTALLA,ALTOPANTALLA,0,0);
      H->CopyFondo();

      if (Result)
         return ABORT;

      return 0;
}

void Partida::StopSounds() {

     SFx->StopFX (DF_SLAVA01);
}


void Partida::Situa ()
{
     LogWrite ("Cambio de pantalla (%i,%i).\n", CAct.X, CAct.Y);
     HAct = MAct->IrHab(CAct);
     VJ.HAct = (void*)HAct;
     VJ.Torrebruno = NULL;

     H->ClrFondo (0);
     H->Terremoto (0);
     Backgd->Genera(HAct->VerNum());
     H->DrawFondoBuffer(0,0,ANCHOPANTALLA,ALTOPANTALLA,0,0);
     XGrab = (int) Mariano->VerX();
     YGrab = (int) Mariano->VerY();
     SP->Reset();
     BorraBus();
     if (VJ.Panel) {
        delete (Panel*)VJ.Panel;
        VJ.Panel = NULL;
        }
     LogWrite ("Destruyendo actores de la pantalla previa...\n");
     AM->Destroy(false);
     AM->Situa (MAct, HAct);
     LogWrite ("Creando actores de la pantalla actual...\n");
     AM->Creation();
     LogWrite ("Todos los actores creados correctamente.\n");

     if (HAct->VerNum()> 15 && HAct->VerNum() < 18)
        SFx->PlayFX (DF_SLAVA01, true);
      else
        SFx->StopFX (DF_SLAVA01);


     Marcador->SituaTorre();
     DesplTile = 0;
}

int Partida::VidaMenos ()
{
  if (HAct->VerNum()> 15 && HAct->VerNum() < 18)
     SFx->StopFX (DF_SLAVA01);

  CicloVisual();
  H->CopyFondo();
//  H->Persiana (1);
  Mariano->SetVelX(0);
  Mariano->SetVelY(0);
  BorraBus();
  if (VJ.Vidas-- == 0)
    return GameOver();
  NuevaVida();
  H->ClearKeyBuf();
  return VJ.Vidas;
}

void Partida::NuevaVida ()
{
    AM->BorraAct(Mariano);
    Mariano = AM->NuevoAct (MARIANO, PARADO, 0, AMIGO, 0, 0, (140 - 20 * Config->VerDif()));
    VJ.Jugador = (void*) Mariano;
    Mariano->SetX(XGrab);
    Mariano->SetY(YGrab);
    Situa();
    CicloVisual();
    H->CopyFondo();
    H->Persiana (-1);
    Situa();
    H->CopyFondo();
    Mariano->ClrCiclos();
    Mariano->SetParpadeo (3 * TEMPORIZADOR);
    CiclosMuerto = 0;
}

int Partida::GameOver ()
{
     H->LocateBuffer (640, 480); // Vamos a usar toda la pantalla
     H->cls(0);
     H->DibujaCentr ("bitmaps1","DFBM_GAMEOVER_000");
     H->ResizeFont_af(80);
     H->TextOutCentre_aa_ex (SMS->ReadMens(37), 320 + 6, 25 + 7, 252, 88, 0);
     H->TextOutCentre_aa_ex (SMS->ReadMens(37), 320 + 2, 25 + 2, 0, 0, 0);
     H->TextoPartido (SMS->ReadMens(37), 320, 25, 255,255,255, 252,224,168);
     H->CopyFondo();

     H->Fundido (1,2);

     H->ClearKeyBuf();
     H->VuelcaBuffer();

     for (int n = 0; n < 1000 && !close_button_pressed &&H->KeyPressed() == false; n++)
         H->Rest (1);

     H->Fundido (-1, 2);
     return -1;
}

void Partida::LogSalida (int Resultado)
{
    LogWrite ("\nFIN DE PARTIDA: %i\n", Resultado);
    if (Ciclos) Media /= Ciclos;
    LogWrite ("Media FPS = %i / %i. %i segs. transcurridos.", Media, TEMPORIZADOR, Ciclos);
}

void Partida::InterFase ()
{
    int ContadorCiclos = 0, SalidaInter = 0, OffCohete = 0;
    DATAFILE *Cohete, *Pantalla;
    Actor *Cierre1, *Cierre2, *Plataforma, *Humo[3];

    int Ciclos, CiclosMax;
    FraseComic Frases[] = {
        {-116, 74, 3},
        {402, 264, 6},

        {132, 20, 4},
        {134, 240, 4},
        {250, 298, 5},
        {366, 358, 5},

        {-98, 86, 2},
        {240, 216, 2},
        {352, 150, 5}
    };

    const int NVinetas = 4;
    const int FrInicial = 271;

    int NFrases [NVinetas] = {2, 4, 0, 3};
    int PauseComic [NVinetas] = {438, 400, 230, 366};

    DATAFILE *Logo;
    char Titulo[20];

    StopSounds();

    LogWrite ("¡¡¡FIN DE LA PRIMERA FASE!!!\n\n");

    LogWrite ("Cargando DFBM_COHETE...");
    Cohete = load_datafile_object ("./data/bitmaps1.dat","DFBM_COHETE");
    LogWrite ("OK.\nCargando DFBM_HANGAR...");
    Pantalla = load_datafile_object ("./data/bitmaps1.dat","DFBM_HANGAR");

    AM->DestroyEnemies();

    Cierre1 = AM->NuevoAct(CIERRE1,0,0,NEUTRO,0,0,1);
    Cierre1->SetVelX(-2);
    Cierre2 = AM->NuevoAct(CIERRE2,0,0,NEUTRO,256,0,1);
    Cierre2->SetVelX(2);
    Plataforma = AM->NuevoAct(LANZADERA,0,0,NEUTRO,314,174,1);
    Plataforma->SetVelX (1);

    Humo[0] = AM->NuevoAct (HUMOJ, 0, 0, NEUTRO, 44, 236, 1,2);
    Humo[1] = AM->NuevoAct (HUMOJ, 0, 0, NEUTRO, 156, 236, 1,2);
    Humo[2] = AM->NuevoAct (HUMOJ, 0, 0, NEUTRO, 268, 236, 1,2);

    AM->BorraAct(Mariano);

    SFx->PlayFX (DF_SCOUNTD);
    SFx->PlayFX (DF_SALARM);

    timerCounter = 1;

    do {                                            // Ciclo principal del juego
        maxSkip = 6;
        curSkip = 0;

        if (timerCounter > 0) { // Codigo "logico"
           do {
              ContadorCiclos++;

              SP->MueveSistema();

              if (!(ContadorCiclos % 80))
                SFx->PlayFX (DF_SALARM);

//              if (!(ContadorCiclos % 2))
//                 Plataforma->IncX(1);

              AM->ActualizaMov(0);

              if (ContadorCiclos == 480)
                 SFx->PlayFX(DF_SCOHETE);

              if (ContadorCiclos > 500)
                 OffCohete += (ContadorCiclos - 500) / 10 + 1;

              for (int n = 0; n < 3; n++) {
                Humo[n]->SetTrans (128);
                }

               if (++curSkip >= maxSkip) {
                  timerCounter = 0;
                  break;
                  }
              } while (--timerCounter > 0);
           needsRefresh = true;
        }

        if (needsRefresh) { // Codigo de visualizacion
           needsRefresh = false;

           H->Dibuja2x(0,0,Pantalla->dat);
           AM->Dibuja (0,true);

           H->Dibuja2x (10 + (((int) rand() % 2) * 2 - 1), 96 - OffCohete, Cohete->dat);



    for (int Capa = 0; Capa < 3; Capa++) {
        if (Capa < 3 && Config->VerShadow()) {            // La sombra siempre esta "detrÃ¡s" que lo que la genera
           AM->Dibuja_Sombra(Capa);
           H->Vuelca_Sombra(HAct->VerNum());
           AM->Borra_Sombra(Capa);
           }
	    AM->Dibuja(Capa, !Salir);             // Dibujamos personajes
	}

	SP->DibujaSistema();


    Marcador->Show(curSkip, afps, Temporizador);

    H->Vela((ContadorCiclos - 750)*2);

    H->VuelcaBuffer();       // ... volcamos todo a la pantalla!!!

           ++fps;      // Conteo de fps
           if (cfps >= Temporizador) {
              afps = fps;            // Esta variable marca los ciclos x seg que hay ACTUALMENTE
              Ciclos++;
              Media += fps;
              fps = 0;
              cfps = 0;
           }
        }

       if (ContadorCiclos > 900)
        SalidaInter = 1;

       } while (!SalidaInter && !close_button_pressed);

     AM->Destroy (false);


    Mariano = AM->NuevoAct (MARIANO, PARADO, 0, AMIGO, 0, 0, (140 - 20 * Config->VerDif()));
    VJ.Jugador = (void*) Mariano;
    Mariano->SetY (TAMTILE * (TILESALTO - 1) - Mariano->VerLongY());


    SFx->PlayMusic ("DFMUS_CRED", true);

     VJ.Fase = 2;
     Config->ToggleFase(2);

     MAct = GMundos->BuscaMundo(VJ.Fase);
     CAct = MAct->HabInic();

     Mariano->SetY (0);
     Mariano->SetX (0);
     Mariano->SetSent (0);
     Mariano->SetTipo (MARIANO);
     VJ.QueHase = PUNO;
     VJ.QueHasia = SOPLAR;

     H->LocateBuffer (640, 480);
     H->cls(0);
     H->VuelcaBuffer();

     H->SetFont_af ("TTF_EVILGENIUS", 14);


  for (int n = 0, PFrase = 0, PDFrase = 0; n < NVinetas && !close_button_pressed; n++) {

      CiclosMax = 0;

      H->Desatura ();
      H->Vela (192, 0, 0, 640, 480);
      H->CopyFondo();

      sprintf (Titulo, "DFBM_INTLV_0%i", n);
      if (n < 10)
        sprintf (Titulo, "DFBM_INTLV_00%i", n);

      LogWrite ("Cargando %s...", Titulo);
      Logo = load_datafile_object ("./data/bitmaps1.dat",Titulo);
      LogWrite ("OK.\n");

      Ciclos = 0;
      maxSkip = 6;
      curSkip = 0;
      timerCounter = 0;

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


      switch (n) {
            case 2:
                if (!Suena[DF_SEXPL])
                    SFx->PlayFX (DF_SEXPL);
                Suena[DF_SEXPL] = true;
                H->Terremoto(2);
//                CiclosMax = 230;
                break;
            default:
                H->Terremoto (0, 0);
                Suena[DF_SEXPL] = false;
      }

//      LogWrite ("%i: %i\n", n, CiclosMax);
      CiclosMax = PauseComic [n];

      H->ClearKeyBuf();

      H->FundidoBM (20);

      do {

        if (timerCounter > 0) {
          do {
              ++Ciclos;

              if (H->Esc()) {
                unload_datafile_object (Logo);
                while (H->Esc());
                return;
                }

            timerCounter--;
            curSkip++;
            if (curSkip >= maxSkip) {
              timerCounter = 0;
              break;
            }
          } while (timerCounter > 0);
          needsRefresh = TRUE;
        }

        if (needsRefresh) {
          needsRefresh = FALSE;

//          H->TextNum(Ciclos,0,0,7,0);
          H->DrawFondoBuffer(0, 0, 640, 480, 0, 0);

          H->VuelcaBuffer();

              if (H->Esc()) {
                unload_datafile_object (Logo);
                return;
                }
        }

      } while (/*!H->KeyPressed() && */Ciclos < CiclosMax);

      unload_datafile_object (Logo);
  }
/*H->Fundido (-1);


     H->cls (0);*/

     H->Terremoto (0,0);

     H->SetFont_af ("TTF_CAPITAN", 40);;

     H->Vela (192, 0, 0, 640, 480);

     H->LocateBuffer(640, 480);

     H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(87), 326, 306, 252, 88, 0);
     H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(87), 322, 302, 0, 0, 0);
     H->TextoPartido ((char*) SMS->ReadMens(87), 320, 300, 255,255,255, 252,224,168);

     H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(88), 326, 346, 252, 88, 0);
     H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(88), 322, 342, 0, 0, 0);
     H->TextoPartido ((char*) SMS->ReadMens(88), 320, 340, 255,255,255, 252,224,168);

     H->ResizeFont_af(60);

     H->TextOutCentre_aa_ex (PASSWORD, 326, 396, 252, 88, 0);
     H->TextOutCentre_aa_ex (PASSWORD, 322, 392, 0, 0, 0);
     H->TextoPartido (PASSWORD, 320, 400, 255,255,255, 252,224,168);

     H->CopyFondo();

     H->VuelcaBuffer();

     H->ClearKeyBuf();

     for (int pausa = 0; pausa < 5000 && !close_button_pressed && H->KeyPressed() == false; pausa++)
          H->Rest(1);

     Suena[DF_SEXPL] = false;

     return;
}

int Partida::FinAgujero ()
{
    int ContadorCiclos = 0, SalidaInter = 0;
    Actor *Torrebruno;
    Torrebruno = (Actor*) VJ.Torrebruno;
    Torrebruno->SetTipo (TORREC);
    Torrebruno->Muerte();

    Backgd->Genera(HAct->VerNum());
    H->DrawFondoBuffer(0,0,ANCHOPANTALLA,ALTOPANTALLA,0,0);

    timerCounter = 1;

    do {                                            // Ciclo principal del juego
        maxSkip = 6;
        curSkip = 0;

        if (timerCounter > 0) { // Codigo "logico"
           do {

               if (++ContadorCiclos > 140)
                   AM->ActualizaMov(0);

               if (++curSkip >= maxSkip) {
                  timerCounter = 0;
                  break;
                  }
              } while (--timerCounter > 0);
           needsRefresh = true;
        }

        if (needsRefresh) { // Codigo de visualizacion
           needsRefresh = false;

    if (Config->VerDinamic())
    	Backgd->Actualiza();   // Actualiza el fondo (pon las estrellitas, etc...)
      else
        H->DrawFondoBuffer() ; // Si no esta activo el fondo "dinamico", pero sÃ­ las scanlines o las sombras, hay que restaurar el fondo

               if (ContadorCiclos > 150)
                    AM->Dibuja (1, true);
                 else
                    AM->Dibuja (1, false);

        Marcador->Show(curSkip, afps, Temporizador);


            if (ContadorCiclos < 70) {
                H->Ilumina(ContadorCiclos * 255 / 70,0,0,640,480);
                }
               else
                    if (ContadorCiclos < 140) {
                        H->RectFill(0, 0, 640, 480, 7);
                        H->Vela((ContadorCiclos - 70) * 255 / 70, 0, 0, 640, 480);
                        }


            if (ContadorCiclos > 139 && ContadorCiclos < 189)
                H->Circle_cut (Torrebruno->VerX() + (Torrebruno->VerLongX() / 2), Torrebruno->VerY() + (Torrebruno->VerLongY() / 2), ContadorCiclos - 139);

            if (ContadorCiclos > 188 && ContadorCiclos < 550)
                H->Circle_cut (Torrebruno->VerX() + (Torrebruno->VerLongX() / 2), Torrebruno->VerY() + (Torrebruno->VerLongY() / 2), 50);

           if (ContadorCiclos > 549 && ContadorCiclos <= 610)
                H->Circle_cut (Torrebruno->VerX() + (Torrebruno->VerLongX() / 2), Torrebruno->VerY() + (Torrebruno->VerLongY() / 2), (600 - ContadorCiclos));
           if (ContadorCiclos > 610)
                H->cls(0);

           H->VuelcaBuffer();       // ... volcamos todo a la pantalla!!!

           ++fps;      // Conteo de fps
           if (cfps >= Temporizador) {
              afps = fps;            // Esta variable marca los ciclos x seg que hay ACTUALMENTE
              Ciclos++;
              Media += fps;
              fps = 0;
              cfps = 0;
           }
        }

       if (H->Esc() || ContadorCiclos >= 615)
        SalidaInter = 1;

       } while (!SalidaInter && !close_button_pressed);

       AM->Destroy(true);
      H->CopyFondo();
      H->Fundido (-1, 4, true);

      return (H->Esc() + close_button_pressed);
}

void Partida::FindeJuego()
{
    Presentacion TheEnd;
    if (FinAgujero())
        return;
    H->LocateBuffer (640, 480);
    SFx->PlayMusic("DFMUS_CREDF",true);
    if (TheEnd.FinComic() || TheEnd.FinPeriodico() || (TheEnd.FinFeria() & 1)) {
        SFx->StopSound();
        return;
        }

}

int Partida::CicloLogico()
{
    int Ret = 0;
    int ResColision;
    Actor* EscafandraM;

    if (Salir) return Salir;

   	if (AttractMode == 1)
       GameLogWrite (Tecla);
     else
        if (AttractMode == 2) {
           if (H->KeyPressed())
              return ABORT;
           Tecla = GameLogRead();
           }
          else
              Tecla = H->Tecla();

    if (Tecla == ABORT)
      	Ret = Pausa();

    RetMov = AM->ActualizaMov(Tecla);

    if (RetMov == SALEPANTALLA)
       if (Mariano->VerTipo() <= CAPITAN)
	      Situa();
       else {
          if (AttractMode)
             return ABORT;
          if (VidaMenos() < 0)
             return GAMEOVER;
            }

    ResColision = AM->Colisiones();
    if (!Mariano->VerRad())
       Marcador->Torrebruno (ResColision);

    if (ResColision == TERMINADO)
        switch (VJ.Fase) {
            case 1:
                InterFase();
                H->Fundido(-1, 4, true);
                SFx->StopSound();
                H->LocateBuffer();
                H->CopyFondo();
                CicloVisual();
                H->Persiana (-1);
                Situa();
                H->CopyFondo();
                break;
            case 2:
                FindeJuego();
                return TERMINADO;
            }

    if (ResColision == COLISION && // El jugador ha colisionado
       Mariano->VerParpadeo() == false &&
       !(Mariano->VerEst() == TRANSFOR || (Mariano->VerTipo() == MARIANO && Mariano->VerEst() == TRANSFOR +1))) {
      if (Mariano->VerTipo() == MARIANO && Mariano->VerEst() != MUERTE) { // Si es Mariano, preprate para quitarle una vida
         SFx->PlayFX (DF_SAGH);
         Mariano->SetTipo (MARIANOMUERTO);
         Mariano->SetEst (SUBIENDO);
         Mariano->ClrFotog();
         Mariano->SetVelX(0);
         Mariano->SetVelY(-6);
         Mariano->IncY (-18);
         if (Mariano->VerSent())
            Mariano->IncX (-10);
         if (VJ.Fase == 2) {
            SFx->PlayFX (DF_SCRISTAL);
            EscafandraM = AM->NuevoAct(ESCAFANDRAM, 0, Mariano->VerSent(), 0, Mariano->VerX(), Mariano->VerY(), 2, 2);
            EscafandraM->SetVelY (-6);
            EscafandraM->SetTrans (128);
            }
         }

      if (Mariano->VerTipo() == CAPITAN && !Mariano->VerRad() && Mariano->VerEst() != TRANSFOR) // Si es el Capi, resta energa y acta
        if (Mariano->IncPow(-1) <= 0) {
           Mariano->SetEst(TRANSFOR);
           Mariano->SetVelX (0);
           Mariano->ClrFotog();
           Mariano->SetPow(140 - 20 * Config->VerDif());
           }
    }

    if (Mariano->VerTipo() == MARIANOMUERTO && Mariano->VerEst() == POSTSALTO && Mariano->FinAnim()) // Si estamos esperando para quitar una vida
      if (++CiclosMuerto >= 170) { // y ya hay que quitarla...
          if (AttractMode)
             return ABORT;
          if (VidaMenos() < 0)
             return GAMEOVER;
          }

    SP->MueveSistema();

    if (VJ.Autobus != NULL)
       if (((Guagua*)VJ.Autobus)->Mueve() == SALEPANTALLA)
           BorraBus();

   if (VJ.Panel != NULL)
      ((Panel*)(VJ.Panel))->AjustaCiclos();

    if (!VJ.Cheats && (key [KEY_RCONTROL] || key [KEY_LCONTROL]) &&
        (key [KEY_RSHIFT] || key [KEY_LSHIFT]) &&
        key [KEY_1])
        VJ.Cheats = true;

    if (VJ.Cheats) {  // <------------------------------ BETA TESTING

    if ((key [KEY_RCONTROL] || key [KEY_LCONTROL]) &&
        (key [KEY_RSHIFT] || key [KEY_LSHIFT]) &&
        key [KEY_0])
        VJ.Cheats = false;

/*        if (key[KEY_F1]) { // <------------------------------ BETA TESTING
            FindeJuego();
            return TERMINADO;
            }*/

        if (key[KEY_F12]) Temporizador++; // <------------------------------ BETA TESTING
        if (key[KEY_F11]) Temporizador--; // <------------------------------ BETA TESTING
        if (key[KEY_F10]) Temporizador += 10; // <------------------------------ BETA TESTING
        if (key[KEY_F9]) Temporizador -= 10; // <------------------------------ BETA TESTING
        if (key[KEY_F8]) Temporizador = 40; // <------------------------------ BETA TESTING
        if (key[KEY_F7]) VJ.OSD = true; // <------------------------------ BETA TESTING
        if (key[KEY_F6]) VJ.OSD = false; // <------------------------------ BETA TESTING
        if (Temporizador < 1) Temporizador = 1;  // <------------------------------ BETA TESTING
        if (key[KEY_LSHIFT] || key [KEY_RSHIFT]) {  // <------------------------------ BETA TESTING
            if (key[KEY_LEFT] + key[KEY_RIGHT] + key[KEY_UP] + key[KEY_DOWN] == 0)
                CheatRoom = false;
                Coord2D CTemporal = CAct;
                Hab *HTemporal = NULL;
            if (key[KEY_LEFT]) {
                CTemporal.X--;
                HTemporal = MAct->IrHab (CTemporal);
                }
            if (key[KEY_RIGHT]) {
                CTemporal.X++;
                HTemporal = MAct->IrHab (CTemporal);
                }
            if (key[KEY_UP]) {
                CTemporal.Y--;
                HTemporal = MAct->IrHab (CTemporal);
                }
            if (key[KEY_DOWN]) {
                CTemporal.Y++;
                HTemporal = MAct->IrHab (CTemporal);
                }
            if (HTemporal && !CheatRoom) {
                HAct = HTemporal;
                CAct = CTemporal;
                CheatRoom = true;
                Situa();
                }
            }

        if (key[KEY_PLUS_PAD])    // <------------------------------ BETA TESTING
            if (++VJ.Morcillas > 6)
                VJ.Morcillas = 6;

        if (key[KEY_ENTER_PAD])   // <------------------------------ BETA TESTING
            install_int_ex(timerCounterUpdater, BPS_TO_TIMER(500));
          else
            install_int_ex(timerCounterUpdater, BPS_TO_TIMER(Temporizador));
    }

    return Ret;
}

void Partida::CicloVisual()
{
    if (Config->VerDinamic())
    	Backgd->Actualiza();   // Actualiza el fondo (pon las estrellitas, etc...)
      else
        if (Config->VerScanL() || Config->VerShadow())
            H->DrawFondoBuffer() ; // Si no esta activo el fondo "dinamico", pero sÃ­ las scanlines o las sombras, hay que restaurar el fondo

    SP->DibujaSistema();

    if (HAct->VerNum()> 15 && HAct->VerNum() < 18) {                  // Desplazamiento de la lava
       if (++DesplTile >= ANCHOPANTALLA * 6)
          DesplTile = 0;
       if (Config->VerDinamic()) {
           H->DrawFondoBuffer (0,(TILESALTO - 1) * TAMTILE, (DesplTile / 6), TAMTILE, ANCHOPANTALLA - (DesplTile / 6), (TILESALTO - 1) * TAMTILE);
           H->DrawFondoBuffer ((DesplTile / 6),(TILESALTO - 1) * TAMTILE, ANCHOPANTALLA - (DesplTile / 6), TAMTILE, 0, (TILESALTO - 1) * TAMTILE);
       }
     }


    if (HAct->VerNum() == 22 && Config->VerDinamic()) { // Overlay de las luces de la nave
       if (abs(FSent) != 10) { // La primera vez que entramos en la pantalla
          Fotograma = 0;
          FSent = 10;
          }
       Fotograma += FSent;
       if (Fotograma > (190-FSent) && FSent > 0)
          FSent *= -1;
       if (Fotograma < -95 && FSent < 0)
          FSent *= -1;
       if (Fotograma > 0)
          H->Dibuja_TransAdd (0, 0, H->CargaBitMap (DFBM_OVERLAY00), Fotograma);
       }

    for (Capa = 0; Capa < 3; Capa++) {
        if (Capa < 3 && Config->VerShadow()) {            // La sombra siempre esta "detrÃ¡s" que lo que la genera
           AM->Dibuja_Sombra(Capa);
           H->Vuelca_Sombra(HAct->VerNum());
           AM->Borra_Sombra(Capa);
           }

        if (Capa != 1) HAct->Dibuja(Capa);       // Dibujamos tiles
	    AM->Dibuja(Capa, !Salir);             // Dibujamos personajes
	}

    if (HAct->VerNum()> 15 && HAct->VerNum() < 18) {
       if (!((int) rand() % 20)) {
          AM->NuevoAct (BURBUJA, MUERTE, 0, ENEMIGO, ((int)rand()%(ANCHOPANTALLA - 100)), 232 + 2 * ((int)rand()%5), 0);
          SFx->PlayFX (DF_SLAVA02);
       }

       if (Config->VerDinamic()) {                  // Efectos de la lava
  	      H->HeatDist();
          H->Dibuja_TransAdd (0, 76 + Fotograma, H->CargaBitMap (DFBM_HALOROJO), 128);
          if (abs(FSent) != 1) // No se ha definido FSent o el valor no es correcto (venimos de la pantalla 22)
             FSent = 1;
          if (++CheckFS > 3) {
	         Fotograma += FSent;
	         if (Fotograma == 20)
	            FSent = -1;
             if (Fotograma == 0)
                FSent = 1;
             CheckFS = 0;
	         }
          }
       }

   if (VJ.Panel != NULL)
      ((Panel*)(VJ.Panel))->Dibuja();

    if (VJ.Autobus != NULL)
      ((Guagua*)VJ.Autobus)->Dibuja();

    if (Config->VerDinamic() && Mariano->VerTipo() == MARIANO &&
       (Mariano->VerEst() == TRANSFOR || Mariano->VerEst() == TRANSFOR + 1) &&
       !((int)rand() % 5))
          H->Overlay (0, 255, 0, 128);

    Marcador->Show(curSkip, afps, Temporizador);

    if (AttractMode == 2)
       if (++ContTextAM > TEMPORIZADOR / 2) {
          H->SetFont_af ("TTF_CAPITAN", 50);
          H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(89), ANCHOPANTALLA / 2 + 6, 31, 252, 88, 0);
          H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(89), ANCHOPANTALLA / 2 + 2, 27, 0, 0, 0);
          H->TextoPartido ((char*) SMS->ReadMens(89), ANCHOPANTALLA / 2, 25, 255,255,255, 252,224,168);
          H->ResizeFont_af (25);
          H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(90), ANCHOPANTALLA / 2 + 3, 79, 252, 88, 0);
          H->TextOutCentre_aa_ex ((char*) SMS->ReadMens(90), ANCHOPANTALLA / 2 + 1, 76, 0, 0, 0);
          H->TextoPartido ((char*) SMS->ReadMens(90), ANCHOPANTALLA / 2, 75, 255,255,255, 252,224,168);
          if (ContTextAM > TEMPORIZADOR)
             ContTextAM = 0;
          }

    if (AttractMode == 1)
       if (++ContTextAM > TEMPORIZADOR / 2) {
          H->SetFont_af ("TTF_CAPITAN", 50);
          H->TextOutCentre_aa_ex ((char*) "GRABANDO", ANCHOPANTALLA / 2 + 6, 31, 252, 88, 0);
          H->TextOutCentre_aa_ex ((char*) "GRABANDO", ANCHOPANTALLA / 2 + 2, 27, 0, 0, 0);
          H->TextoPartido ((char*) "GRABANDO", ANCHOPANTALLA / 2, 25, 255,255,255, 252,224,168);
          if (ContTextAM > TEMPORIZADOR)
             ContTextAM = 0;
          }

    if (Mariano->VerTipo() == MARIANOMUERTO && CiclosMuerto >= 130)
        H->Circle_cut ((Mariano->VerX() + Mariano->VerX(true)) / 2, Mariano->VerY(true), 4 * (170 - CiclosMuerto));

}

int Partida::Ciclo ()
{
    LogWrite ("\nINICIO DE PARTIDA.\n");
    Fotograma = 0, FSent = 0, CheckFS = 0;
    H->ClearKeyBuf();
    Salir = 0;
    timerCounter = 1;

    do {                                            // Ciclo principal del juego
        maxSkip = 6;
        curSkip = 0;

        if (timerCounter > 0) { // Codigo "logico"
           do {
              Salir = CicloLogico();

               if (++curSkip >= maxSkip) {
                  timerCounter = 0;
                  break;
                  }
              } while (--timerCounter > 0);
           needsRefresh = true;
        }

        /*if (!needsRefresh && curSkip > 1)
           AM->PasaFrame(); // Si no has dibujado el frame, avÃ¡nzalo en los personajes*/

        if (needsRefresh) { // Codigo de visualizacion
           needsRefresh = false;

           if (Salir != GAMEOVER && Salir != TERMINADO) {     // Si el juego contina...
              CicloVisual();           // ... Montamos el bÃºffer y ...
              H->VuelcaBuffer();       // ... volcamos todo a la pantalla!!!

              if (!Salir && !Config->VerDinamic() && !Config->VerScanL())/* && !Config->VerShadow())*/ {
                 AM->Borra();  // DespuÃ©s... borras todos los personajes
                 if (VJ.Autobus != NULL)
                    ((Guagua*)(VJ.Autobus))->Borra();
                 SP->BorraSistema();
                 }
              }

           ++fps;      // Conteo de fps
           if (cfps >= Temporizador) {
              afps = fps;            // Esta variable marca los ciclos x seg que hay ACTUALMENTE
              Ciclos++;
              Media += fps;
              fps = 0;
              cfps = 0;
           }
        }

       } while (!Salir && VJ.Vidas >= 0  && !close_button_pressed);

    StopSounds();

    LogSalida(Salir);


    GMundos->Reset();
    AM->Destroy(true);

    if (AttractMode) {
       GameLogEnd();
       return ABORT;
    }

    if (close_button_pressed)
       return CIERRAVENT;

    if (VJ.Vidas < 0) return GAMEOVER;

    H->CopyFondo();

    if (Salir == ABORT)
      H->Fundido (-1, 2);

    return Salir;
}
