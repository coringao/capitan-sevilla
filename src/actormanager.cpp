#include "actormanager.h"

/**********************************************************************************************
Constructor
Inicializa la lista enlazada
***********************************************************************************************/
ActorManager::ActorManager()
{
 LogWrite ("Creando nuevo gestor de actores...");
 Inicial = NULL;
 Final = NULL;
 Numero = 0;
 LogWrite ("OK\n");
}

/**********************************************************************************************
Destructor
Destruye la lista enlazada
**********************************************************************************************/
ActorManager::~ActorManager()
{
 LogWrite ("Borrando gestor de actores...\n");
 Destroy (true);
 LogWrite ("Gestor de actores borrado.\n");
}

/**********************************************************************************************
Método NuevoAct

Recibe:
int Clase, int Estado, int Sent, Equi, X, Y, Power, ref: Todos los datos para pasar al constructor del nuevo actor

Crea un nuevo actor, aumenta el contador de actores, y corrige los punteros de la lista

Devuelve:
Actor *NuevoAct: Puntero al nuevo actor creado
**********************************************************************************************/
Actor* ActorManager::NuevoAct (int Clase,int Estado, int Sent, char Equi, int X, int Y, int Power, int Cap, int ref, int CiclEsp)
{
    Actor *Retorno;
    Retorno = new Actor (Final, Clase, Estado, Sent, Equi, X, Y, Power, ref, CiclEsp, Cap);
    if (++Numero == 1) Inicial = Retorno;
       else Final->SetSiguiente(Retorno);
    Final = Retorno;
    return Retorno;
}

Actor* ActorManager::NuevoAct (Objeto *Obj, int ref)
{
    Actor *Retorno;
    Retorno = new Actor (Final, Obj, ref);
    if (++Numero == 1) Inicial = Retorno;
       else Final->SetSiguiente(Retorno);
    Final = Retorno;
    return Retorno;
}

/**********************************************************************************************
Método Destroy
Recibe:
bool Todos = false: Si tiene que destruir todos los actores, o "salvar" al jugador

Destruye los objetos de la lista enlazada

Devuelve el nmero de actores que quedan en la lista
**********************************************************************************************/
int ActorManager::Destroy (bool Todos)
{
    Actor *Bucle, *ActAnt, *ActSig;
    Bucle = Inicial;
    if (!Bucle || !Numero) return -1;
    do {
        ActAnt = Bucle->VerAnterior();
        ActSig = Bucle->VerSiguiente();
        if (Todos || (Bucle->VerTipo() > CAPITAN && Bucle->VerTipo() != MARIANOMUERTO)) {
           if (ActAnt) ActAnt->SetSiguiente (ActSig);
              else if (ActSig) Inicial = ActSig;
                   else Inicial = NULL;
           if (ActSig) ActSig->SetAnterior (ActAnt);
              else if (ActAnt) Final = ActAnt;
                   else Final = NULL;
           delete Bucle;
           Numero--;
           }
        Bucle = ActSig; } while (Bucle);
    return Numero;
}

/**********************************************************************************************
Método DestroyEnemies

Destruye los enemigos de la lista enlazada enviándoles la señal para que "mueran"

**********************************************************************************************/
void ActorManager::DestroyEnemies ()
{
    Actor *Bucle;
    Bucle = Inicial;

    if (!Bucle || !Numero)
      return;

    do {
        if (Bucle->VerEquipo() == ENEMIGO) {
            if (Bucle->VerTipo() == BURGUER) {
               Bucle->Explota();
               BorraAct(Bucle);
               }
            if (Bucle->VerTipo() == LASERJ)
               BorraAct(Bucle);
             else
               Bucle->Muerte();
            }
        if (Bucle->VerTipo() == PASAJE)
            BorraAct (Bucle);
        Bucle = Bucle->VerSiguiente(); } while (Bucle);
   Colisiones();
}

/**********************************************************************************************
Método Creation ()

Lee el mapa, y crea los actores que haya en la habitación

Devuelve el número de objetos creados;
**********************************************************************************************/
int ActorManager::Creation ()
{
    int n, NumObjs = HAct->NumObj();
    if (!NumObjs) return -1;
    Actor *Temp = NULL, *Temp2 = NULL, *Temp3 = NULL, *Temp4 = NULL;

    for (n = 0; n < NumObjs; n++){

        while (Objetos[n].Tipo + FAROLG == MORCILLA &&
               (Objetos[n].byte3 != 0 && Objetos[n].byte3 <= Config->VerDif()))
               if (++n >= NumObjs)
                  return NumObjs;

        while (Objetos[n].Tipo + FAROLG != MORCILLA &&
               Objetos[n].byte3 && Objetos[n].byte3 >= Config->VerDif())
               if (++n >= NumObjs)
                  return NumObjs;

        if (Objetos[n].Tipo + FAROLG == FAROLG) {
           Temp = NuevoAct (HALOG, 0, 0, NEUTRO, (Objetos[n].X * TAMTILE) + Objetos[n].OffX - 44, (Objetos[n].Y * TAMTILE) + Objetos[n].OffY - 44, -1, 3);
           Temp->SetKF (1);
           }
        if (Objetos[n].Tipo + FAROLG == FAROLP && !Objetos[n].byte1) {
           Temp = NuevoAct (HALOP, 0, Objetos[n].Sent, NEUTRO, (Objetos[n].X * TAMTILE) + Objetos[n].OffX - (38 * Objetos[n].Sent), (Objetos[n].Y * TAMTILE) + Objetos[n].OffY - 40, -1, 1);
           Temp->SetKF (1);
           }
        if (Objetos[n].Tipo + FAROLG == FAROLPC ||
           (Objetos[n].Tipo + FAROLG == FAROLP && Objetos[n].byte1)) {
           Temp = NuevoAct (HALOPC, 0, Objetos[n].Sent, NEUTRO, (Objetos[n].X * TAMTILE) + Objetos[n].OffX - 36, (Objetos[n].Y * TAMTILE) + Objetos[n].OffY - 40, -1, 1);
           Temp->SetKF (1);
           }
        if (Objetos[n].Tipo + FAROLG == FAROLA) {
           Temp = NuevoAct (HALOGB, 0, 0, NEUTRO, (Objetos[n].X * TAMTILE) + Objetos[n].OffX - 50, (Objetos[n].Y * TAMTILE) + Objetos[n].OffY - 50, -1, 3);
           Temp->SetKF (1);
           }
        if (Objetos[n].Tipo + FAROLG == CABINA)
           Temp = NuevoAct (CRCABINA, 0, 0, NEUTRO, (Objetos[n].X * TAMTILE) + Objetos[n].OffX, (Objetos[n].Y * TAMTILE) + Objetos[n].OffY, -1, 3);

        if (Objetos[n].Tipo + FAROLG == NINO2)
           Temp = NuevoAct (NINO3, COGEBAL, 0, NEUTRO, (Objetos[n].X * TAMTILE) + Objetos[n].OffX - TAMTILE * 24, (Objetos[n].Y * TAMTILE) + Objetos[n].OffY, -1, Objetos[n].Capa);

        if (Objetos[n].Tipo + FAROLG == MORCILLA) {
           Temp2 = NuevoAct (HALOMORCI, 0, 0, NEUTRO, (Objetos[n].X * TAMTILE) + Objetos[n].OffX, (Objetos[n].Y * TAMTILE) + Objetos[n].OffY - 2, -1, Objetos[n].Capa);
           Temp2->SetKF (4);
           }
        if (Objetos[n].Tipo + FAROLG == FOCAZO)
           for (int j = 0; j < 6; j++)
               for (int k = 0; k < 2; k++)
                   if (Objetos[n].X * TAMTILE + Objetos[n].OffX + j * 20 < ANCHOPANTALLA &&
                       Objetos[n].Y * TAMTILE + Objetos[n].OffY + k * 20 < ALTOPANTALLA) {
                           Temp = NuevoAct (HALOGB, 0, 0, NEUTRO, ((Objetos[n].X * TAMTILE + Objetos[n].OffX) + j * 20) - 50, ((Objetos[n].Y * TAMTILE + Objetos[n].OffY) + k * 20) - 50, -1);
                           Temp->SetKF (1);
                           Temp->SetTrans (40);
                           }

        if (Objetos[n].Tipo + FAROLG == BUSSTOP)
           if (VJ.Panel == NULL)
              VJ.Panel = new Panel (Objetos[n].X * TAMTILE + Objetos[n].OffX, Objetos[n].Y * TAMTILE + Objetos[n].OffY);

        if (Objetos[n].Tipo + FAROLG == MONSTRUO && !Objetos[n].byte1)
           Temp2 = NuevoAct (COLUMNON, 0, 0, OBSTACULO, ANCHOPANTALLA - 28, 0, -1, 2);

        if (Objetos[n].Tipo + FAROLG == PC) {
           Temp2 = NuevoAct (PANTALLA, 0, 0, NEUTRO, 14 + 316 * Objetos[n].Sent, 0, -1, 2, 0);
           Temp3 = NuevoAct (COLUMNON, 0, 0, OBSTACULO, ANCHOPANTALLA-6, 0, -1, 2);
           Temp4 = NuevoAct (LUCECITA, 0, 0, NEUTRO, 58 + 316 * Objetos[n].Sent, 154, 2, 2);
           Temp2->SetPertenencia (Temp3);
           Temp3->SetPertenencia (Temp4);
           }


 	if (Objetos[n].Tipo >= 0) // Si es una morcilla ya cogida, el tipo es -1... así que no hay que dibujarla
	   Temp = NuevoAct (&Objetos[n], n);

    if (Objetos[n].Tipo + FAROLG == MORCILLA || Objetos[n].Tipo + FAROLG == MONSTRUO || Objetos[n].Tipo + FAROLG == PC)
       Temp->SetPertenencia (Temp2);
    }
    return NumObjs;
}

/**********************************************************************************************
Método BorraAct
Recibe:
Actor *Obj: Puntero al objeto que se quiere eliminar

Borra un actor de la lista enlazada, restaurando los punteros correspondientes

Devuelve el nmero de objetos que quedan
**********************************************************************************************/
int ActorManager::BorraAct (Actor *Obj)
{
    Actor *ActAnt, *ActSig;
    if (!Obj || !Numero) return -1;
    ActAnt = Obj->VerAnterior();
    ActSig = Obj->VerSiguiente();
    if (ActAnt)
      ActAnt->SetSiguiente (ActSig);
    else
      if (ActSig) Inicial = ActSig;
    else
      Inicial = NULL;
    if (ActSig)
      ActSig->SetAnterior (ActAnt);
    else
      if (ActAnt) Final = ActAnt;
    else
      Final = NULL;
    if (Obj->VerTipo() == MORCILLA)
       Objetos[Obj->VerRef()].Tipo = -1;
    delete Obj;
    Obj = NULL;
    return --Numero;
}

/**********************************************************************************************
Método ActualizaMov()
Mueve los actores, llamando a los Métodos PreparaMov y Mueve.

Devuelve 0 si no hay novedades, y si el mover el JUGADOR retorna algo, ese valor.
También se encarga de borrar los actores que sea necesario tras moverse
**********************************************************************************************/
int ActorManager::ActualizaMov (int Tecla)
{
     Actor *Bucle, *Temp, *Temp2, *Temp3;
     Objeto Morci;
     Disparo Retorno;
     int ResMov, Ret = 0, Tipo;
     Retorno.byte = 0;
     Bucle = Final;
     while (Bucle) {
         ResMov = Bucle->PreparaMov (Inicial, Retorno, Tecla);

         Tipo = Bucle->VerTipo();
         if (Tipo == MARIANOMUERTO)
            Tipo = MARIANO;

         if (Retorno.Tipo > 0) {
            NuevoAct (Retorno.Tipo, Retorno.Estado, Retorno.Sentido , Retorno.Equipo, Retorno.X, Retorno.Y, 1, Retorno.Capa);
            }
         ResMov = Bucle->Mueve(MAct, HAct, Inicial);

         if (!Ret && ResMov && (Tipo <= CAPITAN || Tipo == MARIANOMUERTO)) // Si estamos moviendo al jugador y ...
            Ret = ResMov; // Si al moverlo retorna != 0, devuelve a Partida lo que haya retornado

         if (ResMov && Tipo > CAPITAN) { // Si retorna algo y no es jugador... es que muere o desaparece ;-)
            Temp = Bucle->VerAnterior();
            Bucle->Borra();
            if (Tipo >= CURRO1 && Tipo < CURRO1 + MAXCURRO) // Si es un obrero el que "muere"
                NuevoAct(&Objetos[Bucle->VerRef()], Bucle->VerRef()); // Encárgate de crear uno nuevo
            if (Tipo == NINO1) // Si es un niño el que "muere"
                NuevoAct(&Objetos[Bucle->VerRef()], Bucle->VerRef()); // Encárgate de crear uno nuevo

            if (Tipo == MONSTRUO) { // Si es un monstruo el que "muere"
                Temp3 = Bucle->VerPertenencia();
                Temp2 = NuevoAct(&Objetos[Bucle->VerRef()], Bucle->VerRef()); // Encárgate de crear uno nuevo
                Bucle->SetPertenencia (NULL);
                Temp2->SetPertenencia (Temp3);
                }

            if (Tipo == TORO && !Objetos[Bucle->VerRef()].byte1) { // Si sale el toro y su byte1 es 0...
                Objetos[Bucle->VerRef()].byte1 = 1; // ... ponlo en uno...
                Morci.Tipo = MORCILLA - FAROLG; // ... y crea una morcilla!
                Morci.Equipo = AMIGO;
                if (Bucle->VerX() < 100) // 100 por ejemplo, es para ver que sale por la izquierda
                   Morci.X = 0;
                 else
                   Morci.X = TILESANCHO - 3;
                Morci.Y = TILESALTO - 3;
                Morci.OffX = 0;
                Morci.OffY = 0;
                Morci.Sent = 0;
                Morci.CiclosEspera = 0;
                Morci.KeyF = 4;
                Morci.Capa = 1;
                Morci.Grav = true;
                Morci.byte1 = 0;
                Morci.byte2 = 0;
                Morci.byte3 = 0;
                Morci.byte4 = 0;
                Temp2 = NuevoAct (MORCILLA, 0, 0, AMIGO, Morci.X * TAMTILE, Morci.Y * TAMTILE, 1, 1, HAct->InsertaObj (&Morci)); // ... y crea una Morcilla :)
                Temp3 = NuevoAct (HALOMORCI, 0, 0, NEUTRO, Morci.X * TAMTILE, Morci.Y * TAMTILE, 1, 1); // ... y crea un halo para la Morcilla
                Temp2->SetPertenencia(Temp3);
                }
            if (Tipo == COLUMNON)
                H->Terremoto (0);
            BorraAct (Bucle); // Cárgate al actor que ha muerto o desaparecido
            Bucle = Temp;
            }
         Bucle = Bucle->VerAnterior();
     }
     return Ret;
}

/**********************************************************************************************
Método Dibuja
Recibe:
int CapaVis: Indica qué capas debe dibujarse
bool Actualiza: Si tiene que actualizar el frame del actor

Llama al Método Dibuja de los actores
**********************************************************************************************/
void ActorManager::Dibuja (int CapaVis, bool Actualiza) {
     Actor *Bucle = Final; // Se empieza a dibujar por el final porque el Inicial siempre va a ser el jugador
     while (Bucle) { // y hay que dibujarlo encima del resto de objetos
           if (CapaVis == Bucle->VerCapa()-1)
               if (Bucle->VerTipo() != VENTANA || Bucle->VerEst() != PARADO) // Las ventanas "paradas" hay que dibujarlas en la primera capa, para que las morcillas de los disparos pasen por "encima"
               Bucle->Dibuja(Actualiza);
           if (CapaVis == 0 && Bucle->VerTipo() == VENTANA && Bucle->VerEst() == PARADO)
               Bucle->Dibuja(Actualiza);


           Bucle = Bucle->VerAnterior();
           }
}

/**********************************************************************************************
Método Dibuja_Sombra
Llama al Método Dibuja_Sombra de los actores
**********************************************************************************************/

void ActorManager::Dibuja_Sombra (int CapaVis) {
     Actor *Bucle = Inicial;
     int SombraX, SombraY, SombraT;
     while (Bucle) {

           if (Bucle->VerSombra())
              if (CapaVis == Bucle->VerCapa()-1) {

                 if (VJ.Autobus && ((Guagua*)VJ.Autobus)->VerX() - 100 < Bucle->VerX()) { // Si hay un autobus en la pantalla
                    SombraX = ((Guagua*)VJ.Autobus)->VerX() - 10; // Define nuevo punto de luz
                    if (SombraX < 1)
                       SombraX = 1;
                    SombraY = 114 + 158 - 40;
                    SombraT = 120 - ((Bucle->VerX(true) - (((Guagua*)VJ.Autobus)->VerX() - 10)) * 50) / ANCHOPANTALLA; // Define nueva intensidad de sombra
                    if (SombraT < 70) //  Si la nueva intensidad te ha salido más tenue que la intensidad normal...
                       SombraT = 70;  // ... ajústala
                    Bucle->Dibuja_Sombra(); // Dibuja la sombra del actor
                    H->Vuelca_Sombra (HAct->VerNum(), SombraX, SombraY, SombraT); // Vuélcala
                    Bucle->Borra_Sombra(); // Y bórrala
                    }
                  else
                    Bucle->Dibuja_Sombra(); // Si no le "pilla el autobús", solo dibuja la sombra
                 }

           Bucle = Bucle->VerSiguiente();
           }
}

/**********************************************************************************************
Método PasaFrame

Llama al Método PasaFrame de los actores. Es para saltarse los frames cuando está activo el frameskipping
**********************************************************************************************/

void ActorManager::PasaFrame () {
     Actor *Bucle = Final;
     while (Bucle) {
           Bucle->PasaFrame();
           Bucle = Bucle->VerAnterior();
           }
}

/**********************************************************************************************
Método Borra
Llama al Método Borra de los actores
**********************************************************************************************/
void ActorManager::Borra () {
     Actor *Bucle = Inicial;
     while (Bucle) {
           Bucle->Borra();
           Bucle = Bucle->VerSiguiente();
           }
}

/**********************************************************************************************
Método Borra_Sombra
Llama al Método Borra_Sombra de los actores
**********************************************************************************************/
void ActorManager::Borra_Sombra (int CapaVis) {
     Actor *Bucle = Inicial;
     while (Bucle) {
           if (Bucle->VerTipo() != FAROLP && Bucle->VerTipo() != FAROLG && Bucle->VerTipo() != FAROLPC &&
               Bucle->VerTipo() != HALOP && Bucle->VerTipo() != HALOG && Bucle->VerTipo() != HALOPC)
              if (CapaVis == Bucle->VerCapa()-1 && // Para pintar la sombra tiene que tratarse de la capa que estamos "trabajando ahora" Y
                 (!VJ.Autobus || // ... O bien que no haya autobús
                  (VJ.Autobus && ((Guagua*)VJ.Autobus)->VerX() - 100 >= Bucle->VerX())))
                 // Or if there is, the shadow is not "altered" by the bus, because it was erased in due course
                      Bucle->Borra_Sombra();
           Bucle = Bucle->VerSiguiente();
           }
}

/**********************************************************************************************
Método Colisiones

Valora las colisiones entre actores, y actúa en consecuencia

Devuelve COLISION si colisiona el CAPITAN SEVILLA, y TODOOK en otro caso
**********************************************************************************************/
int ActorManager::Colisiones () {
    int Ret = TODOOK, IX, Resultado;
    Objeto Morci;
    Actor *Bucle = Inicial, *Temp = NULL, *Temp2, *Temp3;
    while (Bucle) {
          Temp = Bucle->VerSiguiente();
          Resultado = Bucle->ValoraColisiones(HAct);
          if (Resultado == TERMINADO)
              return TERMINADO;
          if (Resultado == COLISION)
              switch (Bucle->VerTipo()) { // Como se comportan ante la colisión los distintos actores
                    case CAPITAN:
                    case MARIANO:
                         Ret = COLISION;
                         break;
                    case HALOMORCI:
                         break;
                    case MORCILLA:
                         BorraAct (Bucle->VerPertenencia());
                         BorraAct (Bucle);
                         break;
                    case CAMARSIN:
                    case CAMARERO:
                        if (!Objetos[Bucle->VerRef()].byte1) { // Si el byte 1 de este Camarero es 0...
                            Objetos[Bucle->VerRef()].byte1 = 1; // ... ponlo en uno...
                            if (!Inicial->VerSent()) IX = 10; else IX = - 10; //... calcula la coordenada X ...
                            Morci.Tipo = MORCILLA - FAROLG;
			                Morci.Equipo = AMIGO;
                            Morci.X = Bucle->VerX() / TAMTILE;
                            Morci.Y = TILESALTO - 3;
                            Morci.OffX = IX;
			                Morci.OffY = 0;
			                Morci.Sent = 0;
			                Morci.CiclosEspera = 0;
			                Morci.KeyF = 4;
			                Morci.Capa = 1;
                            Morci.Grav = true;
			                Morci.byte1 = 0;
			                Morci.byte2 = 0;
			                Morci.byte3 = 0;
			                Morci.byte4 = 0;
                            Temp2 = NuevoAct (MORCILLA, 0, 0, AMIGO, Morci.X * TAMTILE + IX, Morci.Y * TAMTILE, 1, 1, HAct->InsertaObj (&Morci)); // ... y crea una Morcilla :)
                            Temp3 = NuevoAct (HALOMORCI, 0, 0, NEUTRO, Morci.X * TAMTILE + IX, Morci.Y * TAMTILE, 1, 1); // ... y crea un halo para la Morcilla
                            Temp2->SetPertenencia(Temp3);
                            }
                            NuevoAct (ONOMAT, 0, 0, NEUTRO, Bucle->VerX(), Bucle->VerY() + 15, 1, 2);
                            Bucle->Muerte();
                            Ret = MUERTEENEMIGO;
                         break;
                    case MONSTRUO:
                        if (Bucle->VerPertenencia()) { //  Si el byte 1 de este Monstruo es 0...
                            Objetos[Bucle->VerRef()].byte1 = 1; // ... ponlo en uno...
                            (Bucle->VerPertenencia())->Explota();
                            //BorraAct (Bucle->VerPertenencia());
                            (Bucle->VerPertenencia())->SetGrav(true);
                            Bucle->SetPertenencia (NULL);
                            H->Terremoto (1);
                            }
                        SFx->PlayFX (DF_SDESAPARECE);

                    case CURRO1:
                    case CURRO2:
                    case CURRO3:
                    case CUBO:
                    case NEGRO:
                    case GANGSTER:
                    case MUJER:
                    case ELEFANT:
                    case COCO:
                    case ALIEN:
                    case CHEPPY1:
                    case CHEPPY2:
                    case CHEPPY3:
                    case ASOLDIER:
                    case ASTRONAUTA2:
                    case SHORTCIRCUIT:
                    case RODADOR:
                    case GRUA:
                    case ASTRONAUTA3:
                         Bucle->Muerte();
                         NuevoAct (ONOMAT, 0, 0, NEUTRO, Bucle->VerX(), Bucle->VerY() + 15, 1, 2);
                         if (Ret == TODOOK)
                            Ret = MUERTEENEMIGO;
                         break;
                    case SALTARIN:
                         if (Bucle->VerEst() == SUBIENDO || Bucle->VerEst() == CAYENDO)
                            Bucle->SendMessage (COLISION);
                          else {
                            Bucle->Muerte();
                            NuevoAct (ONOMAT, 0, 0, NEUTRO, Bucle->VerX(), Bucle->VerY() + 15, 1, 2);
                         if (Ret == TODOOK)
                            Ret = MUERTEENEMIGO;
                            }
                          break;
                    case OVNIJ:
                    case JERING:
                    case BURGUER:
                    case ROBOTIN:
                    case ASPIRADOR:
                         if (Bucle->VerEst() != CAYENDO && Bucle->VerEst() != POSTSALTO)
                            Bucle->TiraAbajo();
                          else {
                            Bucle->Explota();
                            NuevoAct (ONOMAT, 0, 0, NEUTRO, Bucle->VerX(), Bucle->VerY() + 15, 1, 2);
                            BorraAct (Bucle);
                            }
                         if (Ret == TODOOK)
                            Ret = MUERTEENEMIGO;
                          break;
                    case OBJVENTANA:
                    case CEMENTO:
                         Bucle->Explota();
                         BorraAct (Bucle);
                         break;
                    case VENTANA:
                         if (Ret == TODOOK)
                            Ret = MUERTEENEMIGO;
                    case METEORITO:
                         Bucle->Muerte();
                         break;
                    default:
                      BorraAct (Bucle);
               } // Del Switch
          Bucle = Temp;
    }
    return Ret;
}
