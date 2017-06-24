#include "menuplayer.h"

volatile int tCounterMenu = 0;
volatile int tAttractMode = 0;
static void tCounterUpdaterMenu() {
    tCounterMenu++;
    tAttractMode++;
}
END_OF_STATIC_FUNCTION(tCounterUpdaterMenu);
LOCK_FUNCTION(tCounterUpdaterMenu);
LOCK_VARIABLE(tCounterMenu);

MenuPlayer::MenuPlayer()
{
    LogWrite ("Creando gestor de menús (%p)... ", this);

    Fondo = new Background;

    Ppal = new Menu (432, 270, 5, 25, 0, "Menú Ppal", Fondo);
    Conf = new Menu (432, 270, 5, 25, 0, "Configuración", Fondo);
    ConfJ = new Menu (432, 294, 3, 25, 0, "Configuración juego", Fondo);
    ConfV = new Menu (432, 270, 5, 25, 0, "Configurar vídeo", Fondo);
    ConfA = new Menu (432, 294, 3, 25, 0, "Configurar audio", Fondo);
    ConfC = new Menu (432, 294, 3, 25, 0, "Configurar controles", Fondo);
    Redefine = new Menu (432, 244, 7, 25, 0, "Redefinir teclas", Fondo);
    Extras = new Menu (432, 294, 3, 25, 0, "Extras", Fondo);

    LogWrite ("Gestor de menús creado correctamente.\n");
}

MenuPlayer::~MenuPlayer()
{
    LogWrite ("Borrando gestor de menús (%p)... ", this);
    delete Ppal;
    delete Conf;
    delete ConfJ;
    delete ConfA;
    delete ConfV;
    delete ConfC;
    delete Redefine;
    delete Extras;
    delete Fondo;

    LogWrite ("Gestor de menus borrado correctamente.\n");
}

void MenuPlayer::PrepMenuPpal ()
{
     Ppal->SetTituloOp (0, (char*) SMS->ReadMens(3));
     Ppal->SetTituloOp (1, (char*) SMS->ReadMens(0));
     Ppal->SetTituloOp (2, (char*) SMS->ReadMens(4));
     Ppal->SetTituloOp (3, (char*) SMS->ReadMens(6));
     Ppal->SetTituloOp (4, (char*) SMS->ReadMens(7));
}

void MenuPlayer::PrepMenuExtras ()
{
     Extras->SetTituloOp (0, (char*) SMS->ReadMens(24));
     Extras->SetTituloOp (1, (char*) SMS->ReadMens(25));
     Extras->SetTituloOp (2, (char*) SMS->ReadMens(13));
}

void MenuPlayer::PrepMenuConf ()
{
     char Temp[256];
     int Tipo;
     Conf->SetTituloOp (0, (char*) SMS->ReadMens(50)); // Configurar juego
     Conf->SetTituloOp (1, (char*) SMS->ReadMens(81)); // Configurar pantalla
     Conf->SetTituloOp (2, (char*) SMS->ReadMens(82)); // Configurar sonido
     Conf->SetTituloOp (3, (char*) SMS->ReadMens(83)); // Configurar controles
     Conf->SetTituloOp (4, (char*) SMS->ReadMens(13)); // Volver al menú principal

     for (int n = 0; n < 5; n++) {
       Tipo = 0;
       switch (n) {
         case 0:
           sprintf (Temp, "%s: ", (char*) SMS->ReadMens(8)); // Efectos visuales
           for (int n = 0; n < 1 + Config->VerGraphDet(); n++)
             strcat (Temp, "*");
           for (int n = 1 + Config->VerGraphDet(); n < 5; n++)
             strcat (Temp, "-");
           Tipo = 1;
           break;
         case 1:
           sprintf (Temp, "%s: %s", (char*) SMS->ReadMens(9), (char*) SMS->ReadMens(14 + Config->VerScanL())); // Scanlines
           Tipo = 1;
           break;
         case 2:
           sprintf (Temp, "%s: %ix%i", (char*) SMS->ReadMens(11), Config->VerResAncho(), Config->VerResAlto()); // Resolucion
           Tipo = 1;
           break;
         case 3:
           sprintf (Temp, "%s: %s", (char*) SMS->ReadMens(12), (char*) SMS->ReadMens(16 + Config->VerWindowed())); // Modo de pantalla
           Tipo = 1;
           break;
         case 4:
           sprintf (Temp, "%s", (char*) SMS->ReadMens(28)); // Volver al menu anterior
           break;
       }
       ConfV->SetTituloOp (n, Temp, Tipo);
     }

     for (int n = 0; n < 3; n++) {
       Tipo = 0;
       switch (n) {
         case 0:
           sprintf (Temp, "%s: ", (char*) SMS->ReadMens(84)); // Volumen FX
           for (int n = 0; n < (Config->VerVolFX() / 51); n++)
             strcat (Temp, "*");
           for (int n = (Config->VerVolFX() / 51); n < 5; n++)
             strcat (Temp, "-");
           Tipo = 1;
           break;
         case 1:
           sprintf (Temp, "%s: ", (char*) SMS->ReadMens(85)); // Volumen Música
           for (int n = 0; n < (Config->VerVolMus() / 51); n++)
             strcat (Temp, "*");
           for (int n = (Config->VerVolMus() / 51); n < 5; n++)
             strcat (Temp, "-");
           Tipo = 1;
           break;
         case 2:
/*           sprintf (Temp, "%s: ", (char*) SMS->ReadMens(86)); // Búffer sonido
           for (int n = 0; n < (Config->VerBufSnd() / 51); n++)
             strcat (Temp, "*");
           for (int n = (Config->VerBufSnd() / 51); n < 5; n++)
             strcat (Temp, "-");
           Tipo = 1;
           break;
         case 3:*/
           sprintf (Temp, "%s", (char*) SMS->ReadMens(28)); // Volver al menu anterior
           break;
       }
       ConfA->SetTituloOp (n, Temp, Tipo);
     }

     switch (Config->VerJoy()) {
       case 0:
         sprintf (Temp, "%s", (char*) SMS->ReadMens (91));
         break;
       case 1:
         sprintf (Temp, "%s", (char*) SMS->ReadMens (92));
         break;
       case 2:
         sprintf (Temp, "%s + %s", (char*) SMS->ReadMens (91), (char*) SMS->ReadMens (92));
         break;
      }

     ConfC->SetTituloOp (0, (char*) Temp, 1); // Joystick Vs Teclado
     ConfC->SetTituloOp (1, (char*) SMS->ReadMens(5), 0); // Definir teclas
     ConfC->SetTituloOp (2, (char*) SMS->ReadMens(28), 0);


     sprintf (Temp, "%s: %i", SMS->ReadMens(33), Config->VerFase());
     ConfJ->SetTituloOp (0, Temp, 1); // Empezar en fase...
     sprintf (Temp, "%s: %s", (char*) SMS->ReadMens(51), (char*) SMS->ReadMens(58 + Config->VerDif()));
     ConfJ->SetTituloOp (1, Temp, 1); // Dificultad
     ConfJ->SetTituloOp (2, (char*) SMS->ReadMens(13)); // Menú anterior

}

void MenuPlayer::PrepMenuRedef ()
{
     char Temp[256];
     for (int n = 0; n < 6; n++) {
         sprintf (Temp, "%s: %s", (char*) SMS->ReadMens(18 + n), H->NombreTecla (H->VerTecla(TeclaSent[n])));
         Redefine->SetTituloOp (n, Temp);
         }
     Redefine->SetTituloOp (6, (char*) SMS->ReadMens(28)); // Volver al menu anterior
}

void MenuPlayer::Musiquita(int VolInic)
{
   if (VolInic)
    VolInic = Config->VerVolMus();
   SFx->PlayMusic("DFMUS_MENU", 1, VolInic);
}

void MenuPlayer::Titulo()
{
     Ppal->ClrTitulo();
     H->CopyFondo();
}

void MenuPlayer::PrepMenus()
{
   PrepMenuPpal();
   PrepMenuConf();
   PrepMenuRedef();
   PrepMenuExtras();
}


int MenuPlayer::Run ()
{
   if (close_button_pressed)
      return CIERRAVENT;

   int RetPpal = 999, RetConf, RetRed, RetConfV, Sent = 0, Anterior, Nuevo, Tecla = -1, RetExtras;

   install_int_ex(tCounterUpdaterMenu, BPS_TO_TIMER(TEMPORIZADOR));
   H->LocateBuffer(640, 480);
   Titulo();
   Fondo->Genera (-1);
   PrepMenus();
   if (Config->VerDinamic())
     Fondo->Actualiza();
   Ppal->DrawWSpr();

   Musiquita(0);

   Ppal->Fade (4);

   tAttractMode = 0;

   while (RetPpal > 1 && !close_button_pressed) {  // Mientras no le des a JUGAR o a SALIR
         Titulo();
         RetPpal = Ppal->Run(Sent, Fondo);

         if (RetPpal == 2) {  // Si le das a Cambiar idioma
            Config->ToggleIdioma();
            SMS->InitMens (Config->VerIdioma());
            PrepMenus();
            Titulo();
            Fondo->Genera (-1);
            }

	 if (RetPpal == 4) { // Extras
	    RetConf = 999;
            while (RetConf > 0 && !close_button_pressed) {
                  PrepMenus();
                  Titulo();
                  RetConf = Extras->Run(Sent, Fondo);
                  if (Config->VerDinamic())
                    Fondo->Actualiza();
                  Extras->DrawWSpr();
                  switch (RetConf) {
                         case 1:     // Repetir intro
                              Extras->Fade (-4);
                              SFx->StopSound();
                              Presen->Run(false);
                              Musiquita();
                              Extras->Fade (3);
                              tAttractMode = 0;
                              break;
                         case 2:    // Ver créditos
                              Extras->Fade (-4);
                              SFx->StopSound();
                              Credits.Run();
                              Musiquita();
                              Extras->Fade (3);
                              tAttractMode = 0;
                              break;
                         }
                  }
            }

        if (RetPpal == 3) {  // Si le das a menú de configuración
            PrepMenus();
            Titulo();
            H->ClearKeyBuf();
            RetConf = Conf->Run(Sent, Fondo);
            while (RetConf > 0 && !close_button_pressed) {
                  switch (RetConf) {
                       case 1: // Configuración de juego
                         RetConfV = 1;
                         while (RetConfV > 0 && !close_button_pressed) {
                           PrepMenus();
                           Titulo();
                           H->ClearKeyBuf();
                           RetConfV = ConfJ->Run (Sent, Fondo, RetConfV - 1);
                           switch (RetConfV) {
                               case 1:  // Fase Inicio
                                   if (!Config->VerPass()) {
                                       Titulo();
                                       H->TextOutCentre_aa_ex (SMS->ReadMens(34) ,432, 255, 252,224,168);
                                       H->CopyFondo();
                                       ConfJ->Input (432, 290, 20, 1, 252, 224, 168, -1,-1,-1);
                                       }
                                    else
                                       Config->ToggleFase();
                                    PrepMenus();
                                    break;
                               case 2:  // Dificultad
                                   if (!Sent)
                                        Config->ToggleDif();
                                     else
                                        if (Config->VerDif() + Sent >= 0)
                                            Config->ToggleDif (Config->VerDif() + Sent);
                                   PrepMenus();
                                   break;
                           } // Del Switch RetConfV
                         } // Del While RetConfV
                         break; // Fin de la configuración de juego

                         case 2: // Configuración Visual
                           RetConfV = 1;
                           while (RetConfV > 0 && !close_button_pressed) {
                             PrepMenus();
                             Titulo();
                             H->ClearKeyBuf();
                             RetConfV = ConfV->Run (Sent, Fondo, RetConfV - 1);
                             switch (RetConfV) {
                                           case 1:  // Nivel de detalle gráfico
                                             Anterior = Config->VerGraphDet();
                                             Nuevo = Config->IncGraphDetail(Sent);
                                             if ((Nuevo == 4 && Anterior == 3) || (Nuevo == 3 && Anterior == 4)) {
                                               SFx->StopSound();
                                               LogWrite ("Cambio de la profundidad de color: %i -> %i\n", Anterior, Nuevo);

                                               delete Fondo;

                                               H->InitGraph();
                                               H->DelFont();
                                               textout_centre_ex (screen, font, SMS->ReadMens(1), screen->w / 2, screen->h / 2 - 4, makecol (255,255,255), 0);
                                               H->ReInit();

                                               M->DeleteGraph();
                                               M->CreateGraph();
                                               M->InitGraph();
                                               Titulo();
                                               Fondo = new Background;
                                               Fondo->Genera (-1);
                                               Musiquita();
                                             }
                                             if (Nuevo == 1 || Anterior == 1)
                                                ConfV->DActivar(1, Nuevo != 0);
                                             break;
                                          case 2:  // Scanlines
                                            Config->ToggleScanL();
                                            break;
                                          case 3:  // Cambiar resolución
                                            Anterior = Config->VerResAncho();
                                            if (!Sent)
                                              Config->ToggleRes();
                                             else
                                               switch (Anterior) {
                                                 case 640:
                                                   if (Sent > 0)
                                                     Config->ToggleRes(2);
                                                   break;
                                                 case 800:
                                                   if (Sent < 0)
                                                     Config->ToggleRes(1);
                                                   if (Sent > 0)
                                                     Config->ToggleRes(3);
                                                   break;
                                                 case 1024:
                                                   if (Sent < 0)
                                                     Config->ToggleRes(2);
                                                   break;
                                               }
                                            if (Anterior != Config->VerResAncho())
                                              H->InitGraph();
                                            break;
                                          case 4:  // Cambiar modo
                                            Config->ToggleWindowed();
                                            H->InitGraph();
                                            break;
                                    } // Del switch RetConfV
                              } // Del While RetConfV
                              break; // fin de la Configuración visual

                       case 3: // Configuración de sonido
                         RetConfV = 1;
                         while (RetConfV > 0 && !close_button_pressed) {
                           PrepMenus();
                           Titulo();
                           H->ClearKeyBuf();
                           RetConfV = ConfA->Run (Sent, Fondo, RetConfV - 1);
                           switch (RetConfV) {
                               case 1:  // Volumen FX
                                   Config->IncVolFX (51 * Sent);
                                   SFx->PlayFX(DF_SDISPMORC);
                                   break;
                               case 2:  // Volumen Música
                                   SFx->StopSound();
                                   Config->IncVolMus (51 * Sent);
                                   Musiquita();
                                   break;
                               case 3: // Buffer
                                   break;
                           } // Del Switch RetConfV
                         } // Del While RetConfV
                         break; // Fin de la configuración de sonido

                       case 4: // Configuración de controles
                         RetConfV = 1;
                         while (RetConfV > 0 && !close_button_pressed) {
                           PrepMenus();
                           Titulo();
                           H->ClearKeyBuf();
                           RetConfV = ConfC->Run (Sent, Fondo, RetConfV - 1);
                           if (!num_joysticks)
                              H->InitJoy();
                           switch (RetConfV) {
                             case 1:  // Joystick Vs Teclado
                                 Config->ToggleJoy();
                                 break;
                             case 2:  // Definir teclado
                                 Titulo();
                                 RetRed = Redefine->Run(Sent, Fondo);
                                 while (RetRed > 0 && !close_button_pressed) {
                                   Redefine->SetTituloOp (RetRed - 1, (char*) SMS->ReadMens(17 + RetRed));
                                   H->GrabarTecla(TeclaSent[RetRed - 1], -1);
                                   Titulo();
                                   H->ClearKeyBuf();
                                   Tecla = Redefine->Run(Sent, Fondo, RetRed-1, true);
                                   H->GrabarTecla(TeclaSent[RetRed-1], Tecla);
                                   Titulo ();
                                   PrepMenuRedef();
                                   RetRed = Redefine->Run(Sent, Fondo, RetRed - 1);
                                   }
                                 tAttractMode = 0;
                                 break;
                              } // Del switch RetConfV
                           } // Del While RetConfV
                           break; // Fin de la configuración de controles

                  } // Del Switch RetConf

                 if (!close_button_pressed) {
                    Titulo();
                    H->ClearKeyBuf();
                    PrepMenuConf();
                    RetConf = Conf->Run(Sent, Fondo, RetConf - 1);
                    }

              } // Del While RetConf
         }
   }

//   SFx->StopSound();

   if (!close_button_pressed) {
      if (Config->VerDinamic())
         Fondo->Actualiza();
      Ppal->Spr();
      Ppal->Fade (-4);
   }

   remove_int (tCounterUpdaterMenu);

   if (close_button_pressed)
      return CIERRAVENT;

   if (tAttractMode > TEMPORIZADOR * ATTRACTWAIT)
      return ATTRACTMODE;

   return RetPpal;
}
