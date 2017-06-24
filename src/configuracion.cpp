#include "configuracion.h"
#include "filehandling.h"

Configuracion::Configuracion() {
 LogWrite ("Iniciando gestor de configuración...");
 Idioma = 127;
 Sound = true;
 Windowed = false;
 ScanL = true;
 Resol = 1;
 GraphDet = 3;
 VolFX = 153;
 VolMus = 153;
 Intro = true;
 Fase = 1;
 Passwd = 0;
 Dif = 1;
 Joystick = 0;

 /*
 Passwd --> Nº fases pasadas
 */

 Tecla[0] = KEY_UP;
 Tecla[1] = KEY_DOWN;
 Tecla[2] = KEY_LEFT;
 Tecla[3] = KEY_RIGHT;
 Tecla[4] = KEY_LCONTROL;
 Tecla[5] = KEY_ENTER;
 Tecla[6] = KEY_ESC;


 char Version[1];
 char config[255];
 char fname[512];
 ifstream fichin;

#ifndef WINDOWS__
    snprintf(fname, 511, "%s/.capitan/capitan.cfg", getenv("HOME"));
#else
    snprintf (fname, 511, "%s\\CapitanSevilla\\capitan.cfg", getenv ("APPDATA"));
#endif

 fichin.open (fname);

 if (fichin.good()) {
     fichin.read (Version, 1);
     if (Version[0] != CfgFileVersion)
        Save();
      else {
        fichin.read (config,255);
        fichin.close();
        Idioma = config[0];
        Sound = config[1] & 1;
        Windowed = config[1] & (1 << 1);
        ScanL = config[1] & (1 << 2);
        Intro = config[1] & (1 << 3);
        Passwd = config[1] & (1 << 4);
        Resol = config[2];
        GraphDet = config[3];
        VolFX = (unsigned char) config[4];
        VolMus = (unsigned char) config[5];
        Fase = config[6];
        Passwd = config [7];
        Dif = config [8];
        for (int n = 0; n < 7; n++)
           Tecla[n] = config [9 + n];
       }
     }
  else
   Save();

#ifdef GP2X
  GraphDet = 0;
#endif

  AdjustDet();

  LogWrite ("OK\n");

}

Configuracion::~Configuracion()
{
  LogWrite ("Borrando gestor de configuración... OK\n");
}

int Configuracion::Save() {
 char config[255];
 config [0] = Idioma;
 config [1] = Sound;
 config [1] += Windowed * (1 << 1);
 config [1] += ScanL * (1 << 2);
 config [1] += Intro * (1 << 3);
 config [1] += Passwd * (1 << 4);
 config [2] = Resol;
 config [3] = GraphDet;
 config [4] = (char) VolFX;
 config [5] = (char) VolMus;
 config [6] = Fase;
 config [7] = Passwd;
 config [8] = Dif;
 for (int n = 0; n < 7; n++)
     config [9 + n] = Tecla[n];
 char fname[512];
 ofstream fichout;

#ifndef WINDOWS__
    snprintf(fname, 511, "%s/.capitan/capitan.cfg", getenv("HOME"));
    mkdirp(fname, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#else
    snprintf (fname, 511, "%s\\CapitanSevilla", getenv ("APPDATA"));
    mkdir (fname);
    snprintf (fname, 511, "%s\\CapitanSevilla\\capitan.cfg", getenv ("APPDATA"));
#endif

 fichout.open (fname);

 if (fichout.good()) {
     fichout.write ((char*) &CfgFileVersion, 1);
     fichout.write (config, 255);
     fichout.close();
     return 0;
     }
  else return 1;
}

bool Configuracion::VerIntro() {
    if (Intro) {
      Intro = false;
      Save();
      return true;
      }
    return false;
}

const char Configuracion::VerIdioma () {
      return Idioma;}

const bool Configuracion::VerSonido (){
      return Sound;}

const char Configuracion::VerJoy (){
      return Joystick;}

const bool Configuracion::VerBlending (){
      return Blending;}

const bool Configuracion::VerWindowed (){
      return Windowed;}

const bool Configuracion::VerDinamic (){
      return Dinamic;}

const bool Configuracion::VerScanL (){
      return ScanL;}

const bool Configuracion::VerShadow (){
      return Shadow;}

const char Configuracion::VerCDepth (){
      return ColorDepth;}

const char Configuracion::VerGraphDet (){
      return GraphDet;}

const unsigned char Configuracion::VerVolFX() {
      if (!Sound) return 0;
      return VolFX;}

const unsigned char Configuracion::VerVolMus() {
      if (!Sound) return 0;
      return VolMus;}

const char Configuracion::VerFase() {
      return Fase;}

const char Configuracion::VerPass() {
      return Passwd;}

const int Configuracion::VerResAlto ()
{
#ifdef GP2X
    return 240;
#endif
      switch (Resol) {
             case 0:
//                  return 384;
             case 1:
                  return 480;
             case 2:
                  return 600;
             case 3:
                  return 768;
      }
      return 480;
}

const int Configuracion::VerResAncho ()
{
#ifdef GP2X
    return 320;
#endif
      switch (Resol) {
             case 0:
//                  return 512;
             case 1:
                  return 640;
             case 2:
                  return 800;
             case 3:
                  return 1024;
      }
      return 640;
}

void Configuracion::ToggleIdioma (char Rec){
     if (Rec < 0)
        Idioma++;
      else
        Idioma = Rec;

     Save();
     return;
}

void Configuracion::ToggleSonido (char Rec){
  if (Rec < 0)
    Sound = !Sound;
  else
    Sound = Rec;
  Save();
}

void Configuracion::ToggleJoy (char Rec){
  if (Rec < 0)
    if (++Joystick >= 3)
      Joystick = 0;

  if (Rec >= 0)
    Joystick = Rec;

  if (Joystick && (!num_joysticks || (joy[0].stick[0].axis[0].pos/40 < 0 && joy[0].stick[0].axis[1].pos/40 < 0)))
    Joystick = 0;


/*
#ifdef GP2X
  Joystick = 1;
#endif
*/
}

void Configuracion::ToggleWindowed (char Rec){
     if (Rec < 0)
        Windowed = !Windowed;
      else
        Windowed = Rec;
     Save();
}

void Configuracion::ToggleBlending (char Rec) {
     if (Rec < 0)
        Blending = !Blending;
      else
        Blending = Rec;
     Save();
}

void Configuracion::ToggleDinamic (char Rec) {
     if (Rec < 0)
        Dinamic = !Dinamic;
      else
        Dinamic = Rec;
     Save();
}

void Configuracion::ToggleScanL (char Rec) {
     if (Rec < 0)
        ScanL = !ScanL;
      else
        ScanL = Rec;
     Save();
}

void Configuracion::ToggleRes (char Rec) {
     if (Rec < 0)
        if (++Resol > 3) Resol = 1;
     if (Rec >= 0)
         Resol = Rec;
     Save();
}

void Configuracion::ToggleShadow (char Rec) {
     if (Rec < 0)
        Shadow = !Shadow;
      else
        Shadow = Rec;
     Save();
}

void Configuracion::ToggleFase(char Rec) {
     if (Rec < 0)
        if (++Fase > 2)
           Fase = 1;
    if (Rec >= 0)
        Fase = Rec;
     if (Fase == 2)
        SetPass (true);
     Save();
}

char Configuracion::IncGraphDetail (char Inc)
{
     if (GraphDet + Inc > 4)
        GraphDet = 4;
      else
        if (GraphDet + Inc < 0)
           GraphDet = 0;
         else
           GraphDet += Inc;
     return AdjustDet();
}

void Configuracion::SetPass(char Pass) {
     Passwd = Pass;
     Save();
     }

char Configuracion::AdjustDet ()
{
     switch (GraphDet) {
            case 0:
                 ToggleShadow(0);
                 ToggleDinamic(0);
                 ToggleBlending(0);
                 ColorDepth = 16;
                 break;
            case 1:
                 ToggleShadow(0);
                 ToggleDinamic(0);
                 ToggleBlending(1);
                 ColorDepth = 16;
                 break;
            case 2:
                 ToggleShadow(1);
                 ToggleDinamic(0);
                 ToggleBlending(1);
                 ColorDepth = 16;
                 break;
            case 3:
                 ToggleShadow(1);
                 ToggleDinamic(1);
                 ToggleBlending(1);
                 ColorDepth = 16;
                 break;
            case 4:
                 ToggleShadow(1);
                 ToggleDinamic(1);
                 ToggleBlending(1);
                 ColorDepth = 32;
                 break;
            }
     return GraphDet;
}

void Configuracion::IncVolFX (int Inc)
{
  if (VolFX + Inc > 255)
    VolFX = 255;
  else
    if (VolFX + Inc < 0)
      VolFX = 0;
  else
    VolFX += Inc;
  Save();
}

void Configuracion::IncVolMus (int Inc)
{
  if (VolMus + Inc > 255)
    VolMus = 255;
  else
    if (VolMus + Inc < 0)
      VolMus = 0;
  else
    VolMus += Inc;
  Save();
}

const char Configuracion::VerTecla (int Sentido)
{
    switch (Sentido) {
           case ARRIBA:
                return Tecla[0];
           case ABAJO:
                return Tecla[1];
           case IZQUIERDA:
                return Tecla[2];
           case DERECHA:
                return Tecla[3];
           case FUEGO:
                return Tecla[4];
           case SELEC:
                return Tecla[5];
           case ABORT:
                return Tecla[6];
           }
    return -1;
}

void Configuracion::GrabaTecla (int Sentido, char Key)
{
    switch (Sentido) {
           case ARRIBA:
                Tecla[0] = Key;
                break;
           case ABAJO:
                Tecla[1] = Key;
                break;
           case IZQUIERDA:
                Tecla[2] = Key;
                break;
           case DERECHA:
                Tecla[3] = Key;
                break;
           case FUEGO:
                Tecla[4] = Key;
                break;
           case SELEC:
                Tecla[5] = Key;
                break;
           case ABORT:
                Tecla[6] = Key;
                break;
           }
    Save();
}

const char Configuracion::VerDif (){
      return Dif;}

void Configuracion::ToggleDif (char Rec) {
     if (Rec < 0)
        if (++Dif > 2)
           Dif = 0;
    if (Rec >= 0 && Rec < 3)
        Dif = Rec;
     Save();
}
