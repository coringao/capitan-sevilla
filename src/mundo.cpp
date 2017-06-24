#include "mundo.h" // class's header file

Mundo::Mundo (unsigned char Anc, unsigned char Alt, Mundo *Ant, unsigned char Num)
{
    LogWrite ("Creando mundo (%p)...", this);
    int n, m;
    Ancho = Anc;
    Alto = Alt;
    MiNumero = Num;
    Habs = new bool*[Ancho];
    Habitacion = new Hab **[Ancho];
    for (n = 0; n < Ancho; n++) {
        Habs[n] = new bool [Alto];
        Habitacion[n] = new Hab *[Alto];
        }
    for (n = 0; n < Ancho; n++)
        for (m = 0; m < Alto; m++)
            Habs[n][m] = false;
    Anterior = Ant;
    Siguiente = NULL;
    LogWrite ("OK\n");
}

Mundo::~Mundo()
{
 Reset ();
}

void Mundo::Reset ()
{
    int n, m;

    LogWrite ("Borrando mundo %i (%p)...\n", MiNumero, this);

    for (n = 0; n < Ancho; n++)
        for (m = 0; m < Alto; m++)
            if (Habs [n][m])
               delete Habitacion[n][m];

    for (n = 0; n < Ancho; n++)
        delete [] Habs [n];
    delete [] Habs;

    for (n = 0; n < Ancho; n++)
        delete [] Habitacion [n];
    delete [] Habitacion;

    LogWrite ("Mundo (%p) borrado OK.\n", this);
}

Hab* Mundo::IrHab (Coord2D Habit) {
     if (Habit.X >= Ancho || Habit.Y >= Alto || Habit.X < 0 || Habit.Y < 0) return NULL;
     if (!Habs[Habit.X][Habit.Y]) return NULL;
     return Habitacion[Habit.X][Habit.Y];
}

void Mundo::SetInitRoom (int x, int y)
{
     Inic.X = x;
     Inic.Y = y;
}

Hab* Mundo::CreateRoom (int x, int y, int w, int h)
{
      Habs[x][y] = true;
      Habitacion [x][y] = new Hab (w, h, 0);
      return Habitacion[x][y];
}

const bool Mundo::ExisteHab (Coord2D &Coord)
{
      if (Coord.X >= Ancho || Coord.Y >= Alto || Coord.X < 0 || Coord.Y < 0)
         return 0;
       else
         return Habs[Coord.X][Coord.Y];
}

int Mundo::Load ()
{
    int n, m, HAncho, HAlto, HNum;
    Hab *Temp;

    LogWrite ("Cargando datos de mundo...\n");

    for (n = 0; n < Ancho; n++)
        for (m = 0; m < Alto; m++) {
            Habs[n][m] = H->MapLoadChar();
            if (Habs[n][m]) {
               HAncho = H->MapLoadChar();
               HAlto = H->MapLoadChar();
               HNum = H->MapLoadChar();
               Temp = CreateRoom (n, m, HAncho, HAlto);
               Temp->SetNum(HNum);
               Temp->Load();
            }
        }

    LogWrite ("Mundo nº %i - OK.\n", MiNumero);
    return 0;
}
