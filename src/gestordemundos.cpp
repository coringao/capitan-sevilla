#include "gestordemundos.h"

GestorDeMundos::GestorDeMundos()
{
 NumMundos = 0;
 Inicial = NULL;
 Final = NULL;
 LogWrite ("Creado Gestor de mapeado...OK\n");
}

GestorDeMundos::~GestorDeMundos()
{
 Reset();
}

void GestorDeMundos::Reset () {
    LogWrite ("Iniciando borrado datos del mapeado...\n");
    Temp = Final;
    Inicial = NULL;
    Final = NULL;
    if (!NumMundos)
       return;
    while (Temp) {
        Temp2 = Temp->VerAnterior();
        delete Temp;
        Temp = Temp2;
        };
    NumMundos = 0;
    LogWrite ("Datos del mapeado borrados OK\n");
}

Mundo* GestorDeMundos::BuscaMundo (int Num) {
    Mundo* Busq;
    Busq = Inicial;
    while (Busq) {
          if (Busq->VerNumero() == Num) return Busq;
          Busq = Busq->VerSiguiente();
          }
    return NULL;
}

Mundo* GestorDeMundos::CreaMundo (unsigned char Anc, unsigned char Alt)
{
    Temp = new Mundo (Anc, Alt, Final, ++NumMundos);
    if (NumMundos == 1) Inicial = Temp;
       else Final->SetSiguiente (Temp);
    Final = Temp;
    return Temp;
}

int GestorDeMundos::Load () {
    LogWrite ("Inicializando el mapa.\n");

    int n, NumMundosL, MAncho, MAlto, MNum, MInitX, MInitY;
    Mundo *Temp;

    if (H->MapLoadInit()) return 1;

    if (NumMundos)
       Reset ();

    H->MapLoadChar (); // TilesAncho
    H->MapLoadChar (); // TilesAlto
    H->MapLoadChar (); // AnchoTile
    H->MapLoadChar (); // AltoTile
    H->MapLoadChar (); // Layers

    NumMundosL = H->MapLoadChar();

    for (n = 0; n < NumMundosL; n++) {
        MNum = H->MapLoadChar ();
        MAncho = H->MapLoadChar();
        MAlto = H->MapLoadChar();
        MInitX = H->MapLoadChar();
        MInitY = H->MapLoadChar();
//        if (MNum + 1 != n) return 1;
        Temp = CreaMundo (MAncho, MAlto);
        Temp->SetInitRoom(MInitX, MInitY);
        Temp->Load();
        }

    H->MapLoadClose();

    LogWrite ("Datos del mapa cargados OK.\n");
    return 0;
}
