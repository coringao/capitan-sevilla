#include "hab.h"

Hab::Hab(int Anc, int Alt, int Num)
{
  int n, m, o;
  LogWrite ("Creando habitación (%p)...", this);
  NumObjs = 0;
  Ancho = Anc;
  Alto = Alt;
  Datos = new unsigned char** [Ancho];
  MiNum = Num;
  for (n = 0; n < Ancho; n++)
      Datos[n] = new unsigned char* [Alto];
  for (n = 0; n < Ancho; n++)
      for (m = 0; m < Alto; m++)
          Datos [n][m] = new unsigned char [LAYERS * 3];

  for (n = 0; n < Ancho; n++)
      for (m = 0; m < Alto; m++)
          for (o = 0; o < LAYERS * 3; o++)
              Datos [n][m][o] = 0;

  LogWrite ("OK.\n");
}

Hab::~Hab()
{
  LogWrite ("Borrando habitación (%p)...", this);
  int n, m;
  for (n = 0; n < Ancho; n++)
      for (m = 0; m < Alto; m++)
          delete [] Datos[n][m];
  for (n = 0; n < Ancho; n++)
      delete [] Datos [n];
  delete [] Datos;
  LogWrite ("OK.\n");
}

int Hab::CContenido (Coord2D &Coord, bool Nube)
{
    if (Coord.X < 0 || Coord.X >= TILESANCHO ||
        Coord.Y < 0 || Coord.Y >= TILESALTO)
            return 0;
    if (Nube && Datos[(unsigned char)Coord.X][(unsigned char)Coord.Y][5] & 1 << 2)
       return 0;
    return Datos[(unsigned char)Coord.X][(unsigned char)Coord.Y][3];
}

void Hab::Dibuja (int CapaVis) {
    int n, m, o;
/*    for (n = 0; n < LAYERS * 3; n += 3)
         if (CapaVis * 3 == n)
*/
    n = CapaVis * 3;

            for (m = 0; m < TILESANCHO; m++)
                for (o = 0; o < TILESALTO; o++)
                    if (Datos[m][o][n]) // Si es 0, no dibujes nada
/*                       if (TileTrans(Datos[n][m][o]))
                          H->Dibuja_Trans(m * TAMTILE, o * TAMTILE, M->Sprite (TILE, Datos[m][o][n], Datos[m][o][n + 1], Datos[m][o][n + 2]));
                        else */
                          H->Dibuja (m * TAMTILE, o * TAMTILE, M->Sprite (TILE, Datos[m][o][n], Datos[m][o][n + 1], Datos[m][o][n + 2]));
}

void Hab::Dibuja_Sombra ()
{
     int m, o;
            for (m = 0; m < TILESANCHO; m++)
                for (o = 0; o < TILESALTO; o++)
                    if (Datos[m][o][6]) // Si es 0, no dibujes nada
                          H->Dibuja_Sombra (m * TAMTILE, o * TAMTILE, M->Sprite (TILE, Datos[m][o][6], Datos[m][o][7], Datos[m][o][8]));
}

int Hab::Load () {
     int n, m, o;
     for (n = 0; n < Ancho; n++)
         for (m = 0; m < Alto; m++)
             for (o = 0; o < LAYERS * 3; o++)
                 Datos[n][m][o] = H->MapLoadChar();
     NumObjs = H->MapLoadChar ();
     for (n = 0; n < NumObjs; n++) {
         Objects[n].Tipo = H->MapLoadChar ();
         Objects[n].X = H->MapLoadChar ();
         Objects[n].Y = H->MapLoadChar ();
         Objects[n].OffX = H->MapLoadChar ();
         Objects[n].OffY = H->MapLoadChar ();
         Objects[n].Capa = H->MapLoadChar ();
         Objects[n].Equipo = H->MapLoadChar ();
         Objects[n].KeyF = H->MapLoadChar ();
         Objects[n].Sent = H->MapLoadChar ();
         Objects[n].CiclosEspera = H->MapLoadChar ();
         Objects[n].Grav = H->MapLoadChar ();
         Objects[n].byte1 = H->MapLoadChar ();
         Objects[n].byte2 = H->MapLoadChar ();
         Objects[n].byte3 = H->MapLoadChar ();
         Objects[n].byte4 = H->MapLoadChar ();
         }
     return 0;
}

int Hab::InsertaObj (Objeto *ObjInsert)
{
    if (NumObjs == 255) return -1;
    Objects[NumObjs++] = *ObjInsert;
    return (NumObjs-1); // Devuelve el nmero que ocupa en el vector de objetos, es decir, su nmero de "refencia"
}

unsigned char Hab::VerNum ()
{
    return MiNum;
}

unsigned char Hab::SetNum (unsigned char Num)
{
    MiNum = Num;
    return MiNum;
}

mask * Hab::CTileMask (Coord2D &Coord, bool Nube)
{
    mask *Ret;
    int Tile, Pag, Byte;
    unsigned char x = (unsigned char) Coord.X;
    unsigned char y = (unsigned char) Coord.Y;
    Tile = Datos [x][y][3];
    Pag = Datos [x][y][4];
    Byte = Datos [x][y][5];
    if (Coord.X < 0 || Coord.X >= TILESANCHO ||
        Coord.Y < 0 || Coord.Y >= TILESALTO) {
            Tile = 0;
            Pag = 0;
            }
    if (Nube && Datos[x][y][5] & 1 << 2){
            Tile = 0;
            Pag = 0;
            }
    Ret = M->Mask(TILE, Tile, Pag, Byte);
    return Ret;
}
