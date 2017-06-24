#include "sonido.h"

Sonido::Sonido( )
{
 LogWrite ("Creado Gestor de sonido...OK\n");
 dfMusic = NULL;
 ActFx = NULL;
}

Sonido::~Sonido()
{
 LogWrite ("Destruido Gestor de sonido...");
 alogg_destroy_ogg (ActFx);
 LogWrite ("OK\n");
}

int Sonido::PlayFX (int FX, int loop)
{
    int Vol;
    DATAFILE *SFx;

    SFx = (DATAFILE*) H->DatSFx();
    Vol = Config->VerVolFX();

    if (!Vol)
       return -1;

    switch (FX) {
        case DF_SDISPCEMENTO:
        case DF_SSOPLA:
        case DF_SDISPLASER2:
            Vol /= 2;
            break;
        case DF_SCANON:
        case DF_SSIERRA:
            Vol *= 2;
            Vol /= 3;
            break;
        case DF_SROBOCHAT:
            Vol *= 2;
            break;
    }

    if (Config->VerSonido())
       return play_sample ((SAMPLE*)SFx[FX].dat, Vol, 127, 1000, loop);
     else
       return -1;
}

void Sonido::StopFX (int FX)
{
    DATAFILE *SFx;

    SFx = (DATAFILE*) H->DatSFx();

    stop_sample ((SAMPLE*)SFx[FX].dat);
}

int Sonido::PlayMusic (const char *Nombre, bool Loop, int Vol)
{
    if (!Config->VerSonido()) return -1;

    loop = Loop;

    if (dfMusic) {
       unload_datafile_object (dfMusic);
       dfMusic = NULL;
       }
    if (ActFx) {
       alogg_stop_autopoll_ogg(ActFx);
       alogg_stop_ogg(ActFx);
       alogg_destroy_ogg (ActFx);
       }

    dfMusic = load_datafile_object("./data/music.dat", Nombre);

    if (!dfMusic) {
       LogWrite ("ERROR: No se ha podido cargar fichero de música: %s\n", Nombre);
       return 1;
       }

    set_volume (Config->VerVolMus(), 0);

    if (Vol < 0)
        Vol = Config->VerVolMus();

  ActFx = alogg_create_ogg_from_buffer ((char*)dfMusic->dat, dfMusic->size);
  alogg_play_ex_ogg(ActFx, 65536, Vol, 127, 1000, Loop);
  alogg_start_autopoll_ogg(ActFx, 25);

  return 0;
}

int Sonido::StopSound ()
{
    alogg_destroy_ogg(ActFx);
    ActFx = NULL;

    return 0;
}

void Sonido::ActVolume (int Vol)
{
   if (Vol < 0)
    Vol = 0;
   if (Vol > Config->VerVolMus())
    Vol = Config->VerVolMus();
   alogg_adjust_ogg(ActFx, Vol, 127, 1000, loop);
}
