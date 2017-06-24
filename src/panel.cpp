#include "panel.h"

Panel::Panel(int X, int Y)
{
    LogWrite ("Nuevo Panel (%p)\n", this);

    x = X + 4;
    y = Y + 16;
    Offset = -30;
    Ciclos = 0;
    CTiempo = 0;
    SegAnterior = 0;
    MaxSec = (int)(rand() % 30) + 30;

    Texto = create_bitmap (640, 14);
    Pantalla = create_bitmap (30, 14);

    MostrarTiempo = true;
    CreaMensaje();
    DosPuntos = true;
}

Panel::~Panel()
{
    destroy_bitmap (Texto);
    destroy_bitmap (Pantalla);
    LogWrite ("Destruido Panel (%p)\n", this);
}

void Panel::CreaMensaje()
{
  if (!MostrarTiempo)
     return;
  sprintf (Mensaje,"%s",SMS->ReadMens (61 + (int) rand() % MensPanel));
  clear_to_color (Texto, makecol (255,0,255));
  textout_ex (Texto, font, Mensaje, 0, 0, makecol (252,184,0), -1);
  MostrarTiempo = false;
}

void Panel::AjustaCiclos()
{
     Segundos = Ciclos++ / 40;

     if (Segundos < MaxSec - 10 && Segundos >= SegAnterior + 5)
       CreaMensaje();

     if (Segundos >= MaxSec) {
       Ciclos = 0;
       CreaMensaje();
       MaxSec = (int)(rand() % 30) + 30;
       VJ.Autobus = new Guagua (x + 60);
       }
     if (Segundos >= MaxSec - 5) {
        MostrarTiempo = true;
        SegAnterior = 0;
        Offset = -30;
        }
}

void Panel::Dibuja ()
{
     char PreMens[4];
     clear_to_color (Pantalla, makecol (255,0,255));

     if (!MostrarTiempo) {
       blit (Texto, Pantalla, Offset++, 0, 0, 0, 30, 14);
       if (Offset >= text_length (font, Mensaje)) {
           Offset = -30;
           MostrarTiempo = true;
           SegAnterior = Segundos;
           }
        }
      else {
        if (DosPuntos)
          sprintf (PreMens, "0:");
         else
          sprintf (PreMens, "0_");
        if (!(Ciclos % 10))
          DosPuntos = !DosPuntos;
        if (Segundos > MaxSec - 10)
           sprintf (Mensaje, "%s0%i", PreMens, MaxSec - Segundos);
         else
           sprintf (Mensaje,"%s%i", PreMens, MaxSec - Segundos);
        clear_to_color (Texto, makecol (255,0,255));
        textout_ex (Texto, font, Mensaje, -1, 0, makecol (252,184,0), -1);
        blit (Texto, Pantalla, 0, 0, 2, 0, 30, 14);
      }

     H->Dibuja (x, y, (void*) Pantalla);
}
