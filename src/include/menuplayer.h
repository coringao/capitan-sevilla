#ifndef MENUPLAYER_H
#define MENUPLAYER_H

#include "menu.h"
#include "hardware.h"
#include "configuracion.h"
#include "messages.h"
#include "sonido.h"
#include "sprmanager.h"
#include "background.h"
#include "presentacion.h"
#include "creditos.h"


extern Hardware *H;
extern SprManager *M;
extern Configuracion *Config;
extern Messages *SMS;
extern Sonido *SFx;
extern Presentacion *Presen;
extern int Fase;
extern volatile int close_button_pressed;

class MenuPlayer{

public:
    MenuPlayer();
    ~MenuPlayer();
    int Run ();
private:
    Menu *Ppal, *Conf, *ConfV, *ConfA, *ConfC, *ConfJ, *Redefine, *Extras, *Extras1, *Extras2;
    Background *Fondo;
    Creditos Credits;
    void PrepMenuPpal ();
    void PrepMenuConf ();
    void PrepMenuRedef ();
    void PrepMenuExtras ();
    void Musiquita (int VolInic = 1);
    void Titulo ();
//    void ActualizaDisableds ();
    void PrepMenus();
};

#endif
