#include "actor.h"

/**********************************************************************************************
Constructor de la clase

Recibe:
Actor* Pointer -> Puntero al objeto anterior en la lista
int Clase -> "Tipo" del sprite (sprite concreto que es)
int Est -> Estado del sprite
int Sent -> Sentido del sprite
char Equi -> "Equipo" del sprite: AMIGO, ENEMIGO o PLATAFORMA
int X, Y -> Coordenadas iniciales
int Power -> Energía inicial
int ref -> Nmero de referencia: Índice del objeto en la matriz de objetos dentro de los datos de la habitación

Ajusta, entre otras, las variables Xi e Yi, que son las coordenadas iniciales del objeto.
Sirve, p. ej., para mover al contenedor de basura.
**********************************************************************************************/
Actor::Actor(Actor *Pointer, int Clase, int Est, int Sent, char Equi, int X, int Y, int Power, int ref, int CiclEsp, int Cap)
{
    LogWrite ("Creando actor (%i) ...", Clase);
    Activo = true;
    Vidas = 1;
    Colisionable = true;
    Anterior = Pointer;
    Siguiente = NULL;
    Pertenencia = NULL;
    SetTipo(Clase);
    SetSent(Sent);
    SetVelX(0);
    SetVelY(0);
    SetEst(Est);
    Equipo = Equi;
    Ref = ref;
    Message = 0;
    CiclosQuieto = 0;
    CiclosRad = 0;
    CiclosCayendo = 0;
    Message = 0;
    SetY (Y); Yi = Y;
    CiclosEspera = CiclEsp;
    Energia = Power;
    if (!CiclEsp) {
       Activo = true;
       SetX (X); Xi = X;
       }
     else {
       Activo = false;
       if (X > ANCHOPANTALLA / 2) SetX(ANCHOPANTALLA); else SetX(-VerLongX());
       Xi = (int) VerX();
       if (VerX() > 0) SetSent(1); else SetSent(0);
     }

    Senti = Sentido;
    Rebote = 0;
    ClrFotog();

    if ((VerTipo()>= HALOG && VerTipo() <= ONOMAT)
        || VerTipo () == BUS || VerTipo() == CUORE || VerTipo() == DMORCILLA)
            SetGrav(false); // Actores no afectados x la gravedad
       else
            SetGrav(true);

    if (Cap >= 0)
       Capa = Cap;
     else
       Capa = 2;

    Ajustes2();

    LogWrite ("Nuevo Actor: %i (%p)\n", Tipo, this);
}

Actor::Actor(Actor *Pointer, Objeto* Obj, int ref)
{
    LogWrite ("Creando actor (%i) ...", Obj->Tipo + FAROLG);
    Activo = true;
    Vidas = 1;
    Colisionable = true;
    Anterior = Pointer;
    Siguiente = NULL;
    Pertenencia = NULL;
    SetTipo(FAROLG + Obj->Tipo);
    SetSent(Obj->Sent);
    SetVelX(0);
    SetVelY(0);
    Equipo = Obj->Equipo;
    Ref = ref;
    Message = 0;
    CiclosQuieto = 0;
    CiclosCayendo = 0;
    CiclosRad = 0;
    Message = 0;
    SetY (Obj->Y * TAMTILE + Obj->OffY); Yi = VerY(false);
    CiclosEspera = Obj->CiclosEspera;
    Energia = Obj->byte4;
    SetGrav (Obj->Grav);
    Capa = Obj->Capa + 1;
    Equipo = Obj->Equipo;
    SetKF (Obj->KeyF);
    Senti = Sentido;
    Rebote = 0;
    SetX (Obj->X * TAMTILE + Obj->OffX); Xi = VerX(false);
    Ajustes1(Obj);
    Ajustes2();
    LogWrite ("Nuevo Actor: %i (%p)\n", Tipo, this);
}

Actor::~Actor()
{
    switch (VerTipo()) {
        case GRUA:
        case HORMI:
            SFx->StopFX (DF_SRALENTI);
            break;
        case SHORTCIRCUIT:
        case RODADOR:
            SFx->StopFX (DF_SROBOCHAT);
            break;
        case SIERRA:
            SFx->StopFX (DF_SSIERRA);
            break;

    }

    LogWrite ("Destruído Actor: %i (%p)\n", Tipo, this);
}

void Actor::Ajustes1(Objeto *Obj)
{
  switch (VerTipo()) {
           case SEMAFORO:
           case BARRIL:
                SetEst (Obj->Sent);
                Sentido = 0;
                break;
           case VENTANA:
                SetSent(VENTANA1 + (int(rand() % MAXVENTANA)));
                SetEst (PARADO);
                Equipo = NEUTRO;
                break;
           case CAMARERO:
                SetEst (PARADO);
                if (Obj->byte1)
	            SetTipo (CAMARSIN);
	            break;
           case ABEJAS:
                SetEst (0);
                break;
           case CURRO:
                SetTipo(CURRO1 + ((int)(rand() % MAXCURRO)));
                SetEst (0);
                Obj->Sent = !Obj->Sent;
                if (VerY() < 0)
                   SetY (-VerLongY());
                 else
                   SetEst (NACIENDO);
                break;
           case ASTRON:
                SetTipo(CHEPPY1 + ((int)(rand() % MAXCHEPPY)));
                SetEst(0);
                Colisionable = false;
                break;
           case NEGRO:
           case GANGSTER:
           case NINO1:
           case TORO:
                SetEst (ANDANDO);
                Activo = false;
                if (VerX(false) > ANCHOPANTALLA / 2) SetX(ANCHOPANTALLA); else SetX(-VerLongX());
                if (VerX() > 0) SetSent(1); else SetSent(0);
                if (VerSent()) SetVelX (-2); else SetVelX(2);  // Pon la VelX que corresponda segn el sentido
                Xi = (int) VerX();
                break;
           case NINO2:
                SetEst (BABEANDO);
                break;
           case HORMI:
                SFx->PlayFX (DF_SRALENTI, true);
                SetEst (ANDANDO);
                ClrFotog();
                break;
           case SIERRA:
                SetEst(ANDANDO);
                if (!Sentido)
                   SetVelX(2);
                  else
                   SetVelX(-2);
                SFx->PlayFX (DF_SSIERRA);
                break;
           case COCO:
                SetSent (1);
                SetVelY(-2);
                SetEst (VOLANDO);
                Rebote = Obj->byte1;
                break;
           case BURGUER:
           case ASPIRADOR:
                Colisionable = false;
                SetEst (VOLANDO);
                if (VerX() < ANCHOPANTALLA / 2) {
                   SetVelX(2);
                   Sentido = 0;
                   }
                 else {
                   SetVelX(-2);
                   Sentido = 1;
                   }
                break;
           case JERING:
                Colisionable = false;
           case ASOLDIER:
                SetEst (VOLANDO);
                break;
           case HUMOJ:
                SetEst(PARADO);
                SetTrans (1);
                CiclosQuieto = 0;
                break;

           case SALTARIN:
           case ASTRONAUTA2:
                SetEst (PARADO);
                CiclosQuieto = 0;
                break;
           case ROBOTIN:
                SetEst (VOLANDO);
                break;
           case MONSTRUO:
                SetEst (NACIENDO);
                Equipo = NEUTRO;
                SetX(100 + ((int) (rand() % 440)));
                if ((VerX() + VerX(true)) / 2 > (ANCHOPANTALLA / 2))
                   SetSent (1);
                 else
                   SetSent (0);
                break;
           case TORRES:
               Vidas = 2;
               SetEst (PARADO);
               ClrFotog();
               VJ.Torrebruno = (void*)this;
               break;

           case GRUA:
               SFx->PlayFX (DF_SRALENTI, true);
               SetEst (ANDANDO);
               ClrFotog();
               break;

           case RODADOR:
           case SHORTCIRCUIT:
                SFx->PlayFX (DF_SROBOCHAT, 1);
                SetKF(4);
                Rebote = 0;
                if (VerSent() == 0)
                   SetVelX (2);
                  else
                   SetVelX (-2);
                CiclosQuieto = 0;
           default:
                SetEst(0);
       }
    ClrFotog();
    if (Equipo == ENEMIGO && Config->VerDif() == 2)
        IncPow (1);
}

void Actor::Ajustes2 ()
{
     switch (VerTipo()) {
        case MORCILLA:
             Capa = 1;
             SetGrav (false);
             break;
        case CUBO:
             if (Sentido == 1)
                IncX (9 * TAMTILE);
             break;
        case ONOMAT:
             SetTrans (255);
             SetEst ((int) (rand() % 6));
             break;
        case ALMA:
             SetTrans(250);
             SetGrav(false);
             break;
        case HALOMORCI:
             Capa = 1;
             SetTrans (220);
             SetGrav (false);
             break;
        case DMORCILLA:
             SetHalo (true);
             break;
        case BOLITA:
             SetEst (PARADO);
             SetGrav (false);
             break;

        case LASERJ:
             SetEst (Capa);
             Capa = 3;
             SetGrav(false);
             SetTrans (170);
             if (Estado < 2)
                SetVelX (6 * (1 - 2 * Sentido));
             switch (Estado) {
                    case 0:
                         SetVelY (0);
                         Colisionable = true;
                         break;
                    default:
                         SetVelY (6);
                         Colisionable = false;
                         break;
                         }
             break;
        case CEMENTO:
             SetGrav (false);
             SetVelX (-4);
             SetVelY (-4);
             Capa = 2;
             break;
        case LANZADERA:
        case CIERRE1:
        case CIERRE2:
             Capa = 1;
             SetGrav(false);
             Colisionable = false;
             break;
        case CRCABINA:
        case HUMOJ:
        case COLUMNON:
             SetGrav (false);
             break;
        case METEORITO:
            SetVelX (8 + (VerX() / (-30)));
            break;
        case PANTALLA:
            SetEst((int)(rand() % MAXPANTALLA));
            SetTrans (255);
            SetGrav (false);
            break;
        case LUCECITA:
            SetEst (0);
            SetKF (2);
            SetGrav (0);
            break;
        case RAYOTORRE:
            SetGrav (false);
            if (VerSent())
               SetVelX (-10);
             else
               SetVelX (10);
             break;
        case ESCAFANDRAM:
            SetEst (0);
            ClrFotog();
        case MESACAPI:
        case SIRFRED:
        case MARIANOBEBE:
            SetGrav (0);
        case GLOBO:
        case TORO:
        case ELEFANT:
        case COCO:
        case ALIEN:
             Rebote = 0;
             Colisionable = false;
             break;
        }
}

/**********************************************************************************************
Método PreparaMov

Recibe:
Disparo &Retorno: Valor que se pasa por referencia, para poder devolver todos los datos de un "disparo" en caso de q se produzca
Actor *Inicial: Puntero al primer actor de la lista enlazada

Sirve para "preparar el movimiento" de un sprite: Recibe el teclado si es un jugador, o calcula
el movimiento en función de la IA correspondiente si es un PNJ

Devuelve 0 si no errores
**********************************************************************************************/
int Actor::PreparaMov (Actor *Inicial, Disparo &Retorno, int Tecla)
{
    Actor *Jugador;
    Coord2D Coordenada;
    int Salida;

    Retorno.Tipo = -1; // En ppio. no hay disparo ...
    Jugador = ((Actor*)(VJ.Jugador));

    if (VerEst () == MUERTE) {
      AjustaKF();
      if (FinAnim() || VerTipo() == MONSTRUO)
         IncTrans(-7);
      if (VerTipo() != BURBUJA)
         return TODOOK;
    }

   if (VerTipo() == MORCILLA) {
      Pertenencia->SetX (VerX());
      Pertenencia->SetY (VerY());
      }

   if (CiclosRad-- == 0)
       SetHalo (false);

    switch (VerTipo()) {
           case MARIANO:
           case CAPITAN:
                if (Estado == MUERTE) {
                   return TODOOK;
                   }

                if ((Estado == PARADO) && !Tecla)
                   return TODOOK;

                if (Estado == POSTSALTO || Estado == LEVANTANDO)
                   return TODOOK;

                if (Tecla & FUEGO) {

                   if (Tipo == MARIANO &&  // Pegar puñetazos
                    (Estado == PARADO || Estado == ANDANDO) ) {
                       SetEst (DISPARANDO);
                       SetVelX(0);
                       ClrFotog();
                   }

                   if (VerTipo() == CAPITAN) {

                      if (VJ.QueHase == VOLAR &&
                         (Estado == SUBIENDO || Estado == CAYENDO)) {
                         IncVelY (-2.25);
                         if (VelY < -2.25) SetVelY (-2.25);
                         SetEst (DESPEGANDO);
                         ClrFotog();
                         CiclosCayendo = 0;
                         }

                      if (VJ.QueHase == VOLAR && Estado == DESPEGANDO) {
                         IncVelY (-2.25);
                         if (VelY < -2.25) SetVelY (-2.25);
                         CiclosCayendo = 0;
                         }

                      if (Estado == VOLANDO) {
                         SFx->PlayFX(DF_SVUELA);
                         IncVelY (-2.25);
                         if (VelY < -2.25) SetVelY (-2.25);
                         CiclosCayendo = 0;
                         }

                      if (VJ.QueHase == SOPLAR &&
                        (VerEst() == ANDANDO || VerEst() == PARADO)) {
                         SetEst (SOPLO);
                         ClrFotog();
                         SetVelX(0);
                         switch (VerSent()) {
                           case 0:
                             IncX (4);
                             break;
                           case 1:
                             IncX (-4);
                             break;
                         }
                      }

                       if (VJ.QueHase == MORCILLAS &&
                         (VerEst() == ANDANDO || VerEst() == PARADO)) {
                          SetEst (DISPARANDO);
                          ClrFotog();
                          SetVelX(0);
                          switch (VerSent()) {
                            case 0:
                              IncX (4);
                              break;
                            case 1:
                              IncX (-4);
                              break;
                          }
                        }

                      if (VJ.QueHase == RADIACTIVO && !VerRad() && VerEst() != TRANSFOR) {
                         SetVelX(0);
                         SFx->PlayFX (DF_SRADIAC);
                         CiclosRad = 4 * 40;
                         SetHalo (true);
                         }
                   } // Fin del if (Tipo == CAPITAN)
                } // Fin del if (Tecla & FUEGO)

                if (Tecla & SELEC && VerTipo() == MARIANO && !(VerEst() == TRANSFOR || VerEst() == TRANSFOR + 1) && VJ.Morcillas) {
                   SetEst (TRANSFOR);
                   if ((int) rand() % 2) Estado++;
                   SetParpadeo (0);
                   SetVelX (0);
                   VJ.Morcillas--;
                   ClrFotog();
                   }

                if (Tecla & SELEC && VerTipo() == CAPITAN && VerEst() != TRANSFOR && VJ.QueHase != VOLAR && !VerRad()) {
                   if (++VJ.QueHase > RADIACTIVO)
                      VJ.QueHase = MORCILLAS;
                   VJ.QueHasia = VJ.QueHase;
                   }

                if (Tecla & ARRIBA && Estado == AGACHADO) {
                   SetEst(PRESALTO);
                   CiclosQuieto = 255;
                   ClrFotog();
                   }

                if ((Tecla & ARRIBA) && !VerVelY() && (Estado == ANDANDO || Estado == PARADO)) {
                   SetEst(PRESALTO);
                   CiclosQuieto = 0;
                   SetVelX(0);
                   ClrFotog();
                }

                if (Tecla & ABAJO && Estado == PARADO) {
                   SetEst(AGACHADO);
                   ClrFotog();
                   CiclosQuieto = 0;
                }

                if (Tecla & ABAJO && Estado == AGACHADO)
                   CiclosQuieto = 0;

                if (Estado == AGACHADO && (Tecla & IZQUIERDA || Tecla & DERECHA || Tecla & ARRIBA)) {
                   SetEst(LEVANTANDO);
                   CiclosQuieto = 0;
                }

                if (Tecla & IZQUIERDA) {

                   if (VerSent() == 1) { // Mira hacia la izquierda
                      if (Tipo == CAPITAN && (Estado == PARADO || (Estado == GIRANDO && FinAnim()))) {
                         SetEst (ARRANCANDO);
                         ClrFotog();
                         SetVelX (-2);
                         CiclosQuieto = 0;
                         }
                      if (Estado == ARRANCANDO && VerTipo() == CAPITAN && FinAnim()) {
                         SetEst (ANDANDO);
                         CiclosQuieto = 0;
                         }

                      if (Estado == PARADO) {
                         SetEst (ANDANDO);
                         SetVelX(-2);
                         CiclosQuieto = 0;
                         }

                      if (Estado == SUBIENDO || Estado == CAYENDO || Estado == VOLANDO || Estado == DESPEGANDO) {
                         SetVelX(-2);
                         CiclosQuieto = 0;
                         }
                   }

                   if (VerSent() == 0) { // Mira hacia la derecha

                      if (Estado == ANDANDO || Estado == PARADO) {
                         SetEst(GIRANDO);
                         SetVelX(0);
                         ClrFotog();
                         CiclosQuieto = 0;
                         }
                      if (Estado == SUBIENDO || Estado == CAYENDO || Estado == VOLANDO || Estado == DESPEGANDO) {
                         Reverse();
                         SetVelX(-2);
                         CiclosQuieto = 0;
                         }
                   }
                }

                if (Tecla & DERECHA) {

                   if (VerSent() == 0) { // Mira hacia la derecha
                      if (Tipo == CAPITAN && (Estado == PARADO || (Estado == GIRANDO && FinAnim()))) {
                         SetEst (ARRANCANDO);
                         ClrFotog();
                         CiclosQuieto = 0;
                         SetVelX (2);
                         CiclosQuieto = 0;
                         }

                      if (Estado == ARRANCANDO && VerTipo() == CAPITAN && FinAnim()) {
                         SetEst (ANDANDO);
                         CiclosQuieto = 0;
                         }

                      if (Estado == PARADO) {
                         SetEst (ANDANDO);
                         SetVelX(2);
                         CiclosQuieto = 0;
                         }

                      if (Estado == SUBIENDO || Estado == CAYENDO|| Estado == VOLANDO || Estado == DESPEGANDO) {
                         SetVelX(2);
                         CiclosQuieto = 0;
                         }
                   }

                   if (VerSent() == 1) { // Mira hacia la izquierda

                      if (Estado == ANDANDO || Estado == PARADO) {
                         SetEst(GIRANDO);
                         SetVelX(0);
                         ClrFotog();
                         CiclosQuieto = 0;
                         }
                      if (Estado == SUBIENDO || Estado == CAYENDO || Estado == VOLANDO || Estado == DESPEGANDO) {
                         Reverse();
                         SetVelX(2);
                         CiclosQuieto = 0;
                         }
                   }
                }

                if (Estado == ARRANCANDO && VerTipo () == CAPITAN &&
                   !(Tecla & IZQUIERDA && VerSent() == 1) && !(Tecla & DERECHA && VerSent() == 0)) {
                   SetEst (PARADO);
                   ClrFotog();
                   SetVelX(0);
                   CiclosQuieto = 0;
                }

                if (Estado != SUBIENDO && Estado != CAYENDO && Estado != DESPEGANDO && Estado != VOLANDO &&
                    !(Tecla & IZQUIERDA) && !(Tecla & DERECHA) && ++CiclosQuieto > 4)
                            SetVelX(0);

                if ((Estado == DESPEGANDO || Estado == VOLANDO) &&
                    !(Tecla & IZQUIERDA) && !(Tecla & DERECHA) &&
                    ++ CiclosQuieto > 8) {
                    if (VerVelX() >= 1.25) IncVelX (-1.25);
                    if (VerVelX() <= -1.25) IncVelX (1.25);
                    }

                if (VerEst() == SOPLO && VerFotog() >= 6 && VerFotog() <= 10) {
                   Actor *Bucle = Inicial;
                   while (Bucle) {
                         if (Bucle->VerEquipo() == ENEMIGO &&
                             Bucle->VerTipo() != VENTANA && Bucle->VerTipo() != LANZAOBJETOS && Bucle->VerEspera() == 0 &&
                             Bucle->VerTipo() != LASERJ && Bucle->VerTipo() != HUMOJ &&
                            ((Bucle->VerX() > VerX() && VerSent() == 0) || (Bucle->VerX() < VerX() && VerSent() == 1))
                            )
                             Bucle->SendMessage(DESPLAZA);
                         if (Bucle->VerTipo() == ASTRONAUTA2)
                             Bucle->SendMessage(DESPLAZA);
                         Bucle = Bucle->VerSiguiente();
                         }
                   }

                if (VerEst() == DISPARANDO && VerTipo() == CAPITAN && VerFotog() == 8 && Frames == 1) {
                  Retorno.Tipo = DMORCILLA;
                  Retorno.Estado = 0;
                  Retorno.Sentido = VerSent();
                  Retorno.Equipo = AMIGO;
                  if (!Sentido)
                    Retorno.X = VerX(true);
                  else
                    Retorno.X = VerX() - 24;
                  Retorno.Y = VerY() + 26;
                  Retorno.Capa = 2;
                  IncPow (-2);
                  }
                break;

           case DMORCILLA:
                if (!Sentido) SetVelX(6); else SetVelX(-6);
                SetVelY (0);
                break;

           case CONTENEDOR:
                if ((VerX() - Xi >= TAMTILE * 8 && VerSent() == 0) ||
                    (VerX() <= Xi && VerSent() == 1))
                   Reverse();
                if (!Sentido) SetVelX(2); else SetVelX(-2);
                break;
           case CAMARERO:
	       case CAMARSIN:
		        int FuturSent;
                Equipo = AMIGO;
		        SetHalo (false);
                if (Jugador->VerTipo() == CAPITAN && Jugador->VerEst() != TRANSFOR && VerEst() != MUERTE) {
		           Equipo = ENEMIGO;
		           SetHalo(true);
		           }
                if (VerEquipo() == ENEMIGO && !Jugador->VerHalo)
		           if (Jugador->VerX() < VerX())
			          FuturSent = 1;
                     else
                      FuturSent = 0;
                 else
                   FuturSent = (int)(rand() % 2);
                if (VerEst() == PARADO && !(int)(rand()%40)) {  // Si esta quieto y toca moverse
                   if (!FuturSent) {
			          if (VerSent() == 0)
			             SetVelX(2);
                       else
			             SetEst (GIRANDO);
                      }
                    else {
			          if (VerSent() == 1)
			             SetVelX(-2);
                       else
			             SetEst (GIRANDO);
                      }
		           if (VerVelX()) {
                      CiclosQuieto = 0;
                      SetEst (ANDANDO);
                      }
                   ClrFotog();
                   }

                if (VerEst() == GIRANDO && FinAnim()) {
		           Reverse();

		           SetEst(ANDANDO);
                   if (VerSent()) SetVelX(-2);
		            else SetVelX (2);
                   CiclosQuieto = 0;
		           ClrFotog();
		           }

                if (VerEst() == ANDANDO && ++CiclosQuieto > 24 && !(int)(rand()%5)) { // Si lleva andando 20 ciclos y "te da por ahí"...
                   SetEst (FRENANDO);
                   ClrFotog();
                   }

                if (VerEst() == FRENANDO && VerFotog() == 0 && Frames == KeyFrame - 1)
                   SetVelX(0);

                if (VerEst() == FRENANDO && FinAnim()) {
                   SetEst (PARADO);
                   ClrFotog();
                   }

                if (VerEst() == ANDANDO && (VerX() + VerVelX() < 4 || VerX(true) + VerVelX() > (ANCHOPANTALLA - 4))) { // Si te vas a salir de la pantalla, vuélvete
                   SetEst (GIRANDO);
		           ClrFotog();
                   SetVelX (0);
                   }
                if (VerEst() != ANDANDO)
                  SetVelX(0);
                break;
           case VENTANA:
                if (VerEst() == PARADO && ++CiclosQuieto > 10 && !((int) rand()% (120 - 40 * Config->VerDif()))) {
                   SetEst (SALIENDO);
                   Equipo = ENEMIGO;
                   CiclosQuieto = 0;
                   }
                if (VerEst() == SALIENDO && FinAnim()) {
                   if (++CiclosQuieto > 6) {
                      SetEst (METIENDOSE);
                      CiclosQuieto = 0;
                      }
                   if (CiclosQuieto == 3) {
                      Retorno.Tipo = OBJVENTANA;
                      Retorno.Estado = 0;
                      Retorno.Sentido = (int(rand()%4));
                      Retorno.Equipo = ENEMIGO;
                      Retorno.X = (VerX(true) + VerLongX() / 2) - 56;
                      Retorno.Y = VerY(true) - 8;
                      Retorno.Capa = 2;
                      SFx->PlayFX(DF_SCAIDA);
                      }
                   }
                if (VerEst() == METIENDOSE && FinAnim()) {
                      SetEst (PARADO);
                      SetSent(VENTANA1 + (int(rand() % MAXVENTANA)));
                      Equipo = NEUTRO;
                      CiclosQuieto = 0;
                      }
                break;

           case NEGRO:
           case GANGSTER:
           case CHEPPY1:
           case CHEPPY2:
           case CHEPPY3:
                if (!Activo && --CiclosEspera > 0)
                   return 0;
                Activo = true;
                if (!VerVelX() && VerEst() != GIRANDO) { // Por si se ha parado por chocar con un tile (pantalla del parque)
                   SetEst (GIRANDO);
                   ClrFotog();
                   }
                if (((VerX() + VerVelX() <= 0 && VerVelX() < 0) ||
                     (VerX(true) + VerVelX() >= ANCHOPANTALLA && VerVelX() > 0))) { // ... y te vas a salir de la pantalla, vuélvete
                        SetEst (GIRANDO);
                        ClrFotog();
                   }
                if (!(Jugador->VerEst() == TRANSFOR || (Jugador->VerTipo() == MARIANO && Jugador->VerEst() == TRANSFOR + 1)) && // A no ser que se esté transformando
                    !Jugador->VerHalo && // y el jugador no está "radiactivo"
                    VerEst() != GIRANDO && // Y no estás ya girando, claro
                    VerY(true) == Jugador->VerY(true) &&  // Si estás a la misma altura del jugador...
                    VerX() > 0 && VerX(true) < ANCHOPANTALLA) // Y no estás medio-fuera de la pantalla
                        if ((VerX() > Jugador->VerX() && VerSent() == 0) ||
                            (VerX() < Jugador->VerX() && VerSent() == 1)) {
                                SetEst (GIRANDO);
                                ClrFotog();
                                }
                if (Estado == GIRANDO && FinAnim()) {
                   Reverse();
                   SetEst (ANDANDO);
                   ClrFotog();
                   }
                if (VerEst() == ANDANDO)
                   if (VerSent())
                      SetVelX (-2);
                     else
                      SetVelX(2);  // Pon la VelX que corresponda según el sentido
                 else
                    SetVelX (0);
                break;
           case NINO1:
                if (!Activo && --CiclosEspera > 0)
                   return 0;
                Activo = true;
                if (VerEst() == PARADO && VerFotog() == 2 && Frames == 1) {
                    Retorno.Tipo = GLOBO;
                    Retorno.Estado = 0;
                    Retorno.Sentido = 0;
                    if (Jugador->VerTipo() == CAPITAN && Jugador->VerEst() != TRANSFOR)
                       Retorno.Equipo = ENEMIGO;
                     else
                       Retorno.Equipo = NEUTRO;
                    Retorno.X = VerX();
                    Retorno.Y = VerY();
                    Retorno.Capa = 3;
                    CiclosQuieto = 0;
                    }
                if (VerEst() == PARADO && FinAnim()) {
                   SetEst(GIRANDO);
                   ClrFotog();
                   }
                if (VerEst() == GIRANDO && FinAnim()) {
                   Reverse();
                   SetEst(ANDASIN);
                   ClrFotog();
                   if (VerSent() == 0)
                      IncX(8);
                   }
                if (VerEst() == ANDANDO &&
                   (VerX() >= Jugador->VerX() && VerX(true) <= Jugador->VerX(true))) {
                       SetVelX(0);
                       SetEst(PARADO);
                       ClrFotog();
                       }
                if (VerEst() == ANDANDO &&
                    ((VerSent() == 0 && VerX() > 400) || (VerSent() == 1 && VerX()<100))) {
                       SetVelX(0);
                       SetEst(PARADO);
                       ClrFotog();
                       }
                if (VerEst() == ANDANDO || VerEst() == ANDASIN)
                   if (VerSent())
                      SetVelX (-2);
                     else
                      SetVelX(2);  // Pon la VelX que corresponda según el sentido
                 else
                    SetVelX (0);
                break;

           case CURRO1:
           case CURRO2:
           case CURRO3:
             if ((int)VerVelY() >= 1) {
               SetVelX(0);
               Rebote = 0;
               }

             if ((VerEst() == NACIENDO || VerEst() == LEVANTANDO) &&
                 FinAnim()) {
               SetEst(ANDANDO);
               ClrFotog();
               }

             if (VerEst () == GIRANDO && FinAnim()) {
               Reverse();
               SetEst(ANDANDO);
               ClrFotog();
               if (++Rebote > 4)
                  Muerte();
               }

             if (((VerX() + VerVelX() < 0 && VerVelX() < 0) || (VerX(true) + VerVelX() > ANCHOPANTALLA && VerVelX() > 0)) &&
                 VerEst() != GIRANDO) {
               SetEst(GIRANDO);
               ClrFotog();
               SetVelX(0);
               }
             if (VerEst() == ANDANDO)
               if (VerSent()) SetVelX(-2);
                else SetVelX (2);
             break;
           case LANZAOBJETOS:
                if (++CiclosQuieto > 30 && !((int) rand()% (120 - 40 * Config->VerDif()))) {
                   if (VJ.Fase == 1) {
                      Retorno.X = VerX();
                      Retorno.Tipo = OBJVENTANA;
                      Retorno.Estado = 0;
                      Retorno.Sentido = 4 + (int(rand()%3));
                      if (((Hab*) (VJ.HAct))->VerNum() > 20)
                         Retorno.Sentido = 6;
                      }
                    else {
                      Retorno.X = (int)(rand() % 480);
                      Retorno.Tipo = METEORITO;
                      Retorno.Estado = 0;
                      Retorno.Sentido = 0;
                    }
                   Retorno.Y = -20;
                   Retorno.Equipo = ENEMIGO;
                   Retorno.Capa = 2;
                   CiclosQuieto = 0;
                   SFx->PlayFX(DF_SCAIDA);
                      }
                break;
           case METEORITO:
                if (VerY() >= 0 && !VerVelY()) {
                   SendMessage (COLISION);
                   SetGrav (false);
                   Equipo = NEUTRO;
                   ClrFotog();
                   }
                break;


           case HORMI:
                if (++CiclosQuieto > 70 - 10 * Config->VerDif()) {
                   SFx->PlayFX (DF_SDISPCEMENTO);
                   CiclosQuieto = 0;
                   Retorno.Tipo = CEMENTO;
                   Retorno.Estado = 0;
                   Retorno.Sentido = 0;
                   Retorno.Equipo = ENEMIGO;
                   Retorno.X = VerX() + 15;
                   Retorno.Y = VerY();
                   Retorno.Capa = 1;
                   }
                break;
           case CEMENTO:
                IncX (2 * (Jugador->VerX() > VerX()) - 1);
                IncY (2 * (Jugador->VerY() > VerY()) - 1);
                IncVelY (.25);
                break;
           case CUBO:
                if (VerEst() == ANDANDO && !VerVelY()) {
                   SetEst (POSTSALTO);
                   SetVelX(0);
                   ClrFotog();
                   }
                if (VerEst() == SUBIENDO || VerEst() == CAYENDO) {
                   if (!Sentido) SetVelX(2);
                      else SetVelX(-2);
                   }
                 else
                   SetVelX(0);

                if (VerEst() == POSTSALTO && FinAnim()) {
                   SetEst (PRESALTO);
                   ClrFotog();
                   }

                if ((VerX() - Xi >= TAMTILE * 8 && VerSent() == 0) ||
                    (VerX() <= Xi && VerSent() == 1))
                   Reverse();
                break;
           case MUJER:
                if (VerEst() != PARADO && ((((Actor*)(VJ.Jugador))->VerTipo() == MARIANO) ||
                    (Jugador->VerTipo() == CAPITAN && Jugador->VerEst() == TRANSFOR))) {
                     SetEst (PARADO);
                     Equipo = NEUTRO;
                     ClrFotog();
                     SetRepeat (true);
                     SetVelX (0);
                     }

                if (Jugador->VerTipo() == CAPITAN && Jugador->VerEst() != TRANSFOR) { // Si es el Capi, y no se está transformando
                   if (Equipo == NEUTRO) {
                      CiclosQuieto = 0;
                      Equipo = ENEMIGO; // Entonces te vuelves "mala"
                      }
                   if (VerEst() == PARADO && !Jugador->VerHalo && Jugador->VerY(true) >= VerY() - 10) { // y si estás parada y "ves" al jugador
                      if ((VerX() >= Jugador->VerX() && VerSent() == 1) ||
                          (VerX() < Jugador->VerX() && VerSent() == 0))
                          SetEst (ARRANCANDO); // síguelo
                        else
                          SetEst (GIRAQUIETA);
                      ClrFotog();
                      }

                   if ((VerEst() == ARRANCANDO || VerEst() == GIRAQUIETA) && FinAnim()) {
                      if (VerEst() == GIRAQUIETA)
                         Reverse();
                      SetEst (ANDANDO);
                      ClrFotog();
                      }

                   if (VerEst() == ANDANDO &&
                       ((VerX() + VerVelX() <= 0 && VerVelX() < 0) ||
                       (VerX(true) + VerVelX() >= ANCHOPANTALLA && VerVelX() > 0))) { // ... Si te vas a salir de la pantalla, vuélvete
                          SetEst (GIRANDO);
                          SetVelX(0);
                          ClrFotog();
                          }

                   if (VerEst() == ANDANDO && Jugador->VerY(true) < VerY() - 10) { // Si estás andando y dejas de ver al jugador...
                      SetEst (PARADO); // ... párate
                      ClrFotog();
                      SetVelX(0);
                      CiclosQuieto = 4000; // Para que lance antes el primer corazón
                      }

                   if (VerEst() == ANDANDO && // Si miras hacia donde no está el jugador, vuélvete
                       ((VerX() > Jugador->VerX() && VerSent() == 0) ||
                       (VerX() < Jugador->VerX() && VerSent() == 1))) {
                             SetEst (GIRANDO);
                             SetVelX(0);
                             ClrFotog();
                             }
                   if (Estado == GIRANDO && FinAnim()) {
                      Reverse();
                      SetEst (ANDANDO);
                      ClrFotog();
                      if (VerSent()) IncX(4);
                         else IncX(-4);
                      }
                   if (VerEquipo() == ENEMIGO && VerY() > Jugador->VerY(true) + 20 && ++CiclosQuieto >= 60 &&
                      (Jugador->VerX(true) + 30 >= VerX() && Jugador->VerX() - 30 <= VerX(true))) {
                       SFx->PlayFX (DF_SBESO);
                       Retorno.Tipo = CUORE;
                       Retorno.Estado = 0;
                       Retorno.Sentido = 0;
                       Retorno.Equipo = ENEMIGO;
                       Retorno.X = VerX();
                       Retorno.Y = VerY();
                       Retorno.Capa = 3;
                       CiclosQuieto = 0;
                       }
                   if (VerEst() == ANDANDO)
                     if (VerSent())
                       SetVelX (-2);
                     else
                       SetVelX(2);  // Pon la VelX que corresponda según el sentido

                   if  (VerEst() == GIRAQUIETA && VerFotog() >= 8 && !VerVelX())
                     if (!VerSent())
                       SetVelX (-2);
                     else
                       SetVelX(2);  // Pon la VelX que corresponda según el sentido
                }
                if (VerEst() == PARADO && VerEst() == GIRANDO)
                  SetVelX(0);
                break;
           case CUORE:
           case GLOBO:
                SetVelY (-1);
                if (!VerVelX())
                   SetVelX ((int)(rand() % 2) * 2 - 1);
                  else
                   SetVelX(0);
                if (Jugador->VerTipo() == CAPITAN && Jugador->VerEst() != TRANSFOR)
                   Equipo = ENEMIGO;
                 else
                   Equipo = NEUTRO;
                break;
           case ABEJAS:
                if ((int(rand() % (4 + Config->VerDif()))) || Jugador->VerHalo) {
                     SetVelX ((int(rand()%2)) * 2 - 1);
                     SetVelY ((int(rand()%2)) * 2 - 1);
                     }
                 else {
                     SetVelX ((VerX() < Jugador->VerX()) * 2 - 1);
                     SetVelY ((VerY() < Jugador->VerY()) * 2 - 1);
                     }
                 break;
           case ELEFANT:
           case OVNIJ:
                if (VerEquipo() == ENEMIGO) {
                   if (!VerVelX() && VerEst() != GIRANDO)
                      if (VerSent()) SetVelX(-1);
                        else SetVelX (1);

                   if (!(int(rand()%50))) {
                      if (VerEst() != GIRANDO && int(rand()%2)) {
                         SetVelX (0);
                         SetEst(GIRANDO);
                         ClrFotog();
                         }
                      if (int(rand()%2)) SetVelY(-1); else SetVelY(1);
                      }
                    if (VerEst() != GIRANDO && ((VerX() + 2 * VerVelX() < 0 && VerSent() == 1) ||
                        (VerX(true) + 2 * VerVelX() > ANCHOPANTALLA && VerSent() == 0))) {
                       SetEst (GIRANDO);
                       ClrFotog();
                       SetVelX(0);
                       }
                    if (VerY() + 2 * VerVelY() < 0)
                       SetVelY(1);
                    if (VerY(true) + 2 * VerVelY() > TILESALTO * TAMTILE)
                       SetVelY(-1);

                   if (Estado == GIRANDO && FinAnim()) {
                      Reverse();
                      SetEst (ANDANDO);
                      ClrFotog();
                      }

                   if (VerEst() == ANDANDO)
                      if (VerSent()) SetVelX(-1);
                        else SetVelX (1);
                   }

                if (VerTipo() == OVNIJ) {
                   if (Estado == GIRANDO) {
                      Reverse();
                      SetEst (0);
                      ClrFotog();
                      if (VerSent()) SetVelX(-1);
                         else SetVelX (1);
                      }
                   if (Estado == CAYENDO && FinAnim() && VerTipo () == OVNIJ && !VerVelY())
                      if (VerGrav() == false)
                         SetGrav(true);
                        else
                          SendMessage(COLISION);
                   if (Estado == CAYENDO && (int)(VerY(true) + VerVelY() + 1) >= TAMTILE * TILESALTO) {
                      while (VerY(true) >= TAMTILE * TILESALTO)
                            IncY(-1);
                      SetVelY(0);
                      SetGrav(false);
                      SendMessage(COLISION);
                      }
                   }
                 break;
           case SIERRA:
                if (
                   (VerX() - Xi >= TAMTILE * 4 && VerSent() == 0) ||
                   (VerX() <= Xi && VerSent() == 1)) {
                   Reverse();
                   CiclosQuieto = 0;
                   SetVelX(0);
                   }
                if (VerVelX() == 0 && ++CiclosQuieto > 20) {
                   if (!Sentido)
                      SetVelX(2);
                    else
                      SetVelX(-2);
                   SFx->PlayFX (DF_SSIERRA);
                   }
                break;
           case TORO:
                if (!Activo && --CiclosEspera > 0)
                   return 0;
                Activo = true;
                if (VerVelX() &&
                   ((VerX() + VerVelX() <= 0 && VerSent() > 0 ) || (VerX(true) + VerVelX() >= TILESANCHO * TAMTILE && VerSent() == 0))) {
                   SetEst (GIRANDO);
                   ClrFotog();
                   SetVelX(0);
                   }
                if (VerEst() == GIRANDO && FinAnim()) {
                   Reverse();
                   SetEst(ANDANDO);
                   }
                if (VerEst() == ANDANDO) // Va fuera del if de antes para que cuando le termine de afectar el soplido ande bien
                   if (VerSent())
                      SetVelX (-2);
                    else
                      SetVelX(2);  // Pon la VelX que corresponda según el sentido
                break;
           case NINO2:
               if (VerEst() == COGEBAL && FinAnim()) {
                  SetEst (TIRABAL);
                  ClrFotog();
                  }
               if (VerEst() == TIRABAL && FinAnim()) {
                  SetEst (BABEANDO);
                  ClrFotog();
                  }
               if (VerEst() == TIRABAL && VerFotog() == 5 && Frames == 1) {
                  SFx->PlayFX (DF_SBALON);
                  Retorno.Tipo = BALON;
                  Retorno.Estado = VOLANDO;
                  Retorno.Sentido = 1;
                  Retorno.Equipo = NEUTRO;
                  Retorno.X = VerX() - 6;
                  Retorno.Y = VerY(true) - 20;
                  Retorno.Capa = 2;
                  }
               break;
           case NINO3:
               if (VerEst() == COGEBAL && FinAnim()) {
                  SetEst (TIRABAL);
                  ClrFotog();
                  }
               if (VerEst() == TIRABAL && FinAnim()) {
                  SetEst (BABEANDO);
                  ClrFotog();
                  }
               if (VerEst() == TIRABAL && VerFotog() == 5 && Frames == 1) {
                  SFx->PlayFX (DF_SBALON);
                  Retorno.Tipo = BALON;
                  Retorno.Estado = ANDANDO;
                  Retorno.Sentido = 0;
                  Retorno.Equipo = NEUTRO;
                  Retorno.X = VerX(true) - 14;
                  Retorno.Y = VerY(true) - 14;
                  Retorno.Capa = 2;
                  }
               break;

          case BALON:
               if (!VerVelY() && VerEst () == VOLANDO && !Rebote) // Movimiento "inicial"
                  SetVelY (-11);
               if (VerVelX())
                  CiclosQuieto = -1;
               if (!VerVelX() && !Rebote)
                  if (VerSent())
                     SetVelX(-5);
                   else
                     SetVelX(10.5);  // Pon la VelX que corresponda según el sentido
               if (!Rebote && (VerVelX() || VerVelY()))
                  Rebote = 1;

               if (VerY() > 242)
                SetY(242);

               if (Jugador->VerTipo() == MARIANO)
                  Equipo = NEUTRO;
                 else
                  Equipo = ENEMIGO;

               if ((int)VerVelX() == 0) {
                  SetEst(PARADO);
                  ClrFotog();
                  }

               if (VerSent() == 0) // Cuando vayas para la derecha, que vas rodando, frénate
                  if (VerVelX() > .125)
                      IncVelX(-.125);  // Pon la VelX que corresponda según el sentido
               if ((int)VerVelX() == 0 && (int)VerVelY() == 0)
                  SetEquipo (AMIGO);
               break;
           case COCO:
                 if (VerX() <= Xi - Rebote * TAMTILE)
                    Sentido = 0;
                 if (VerX() >= Xi || VerX(true) >= ANCHOPANTALLA) {
                    Sentido = 1;
                    if (ReadMessage() != DESPLAZA)
                       SetX(Xi);
                    }
                 if (ReadMessage() == DESPLAZA && VerX() >= Xi)
                    Xi = VerX();

                 if (!VerVelY() && !(VerX() % TAMTILE)) {
                    SetVelX(0);
                    if (VerY() >= Yi + 2* TAMTILE)
                       SetVelY (-2);
                     else
                       SetVelY(2);
                    }
                 if (VerY() >= Yi + 2 * TAMTILE && VerVelY() > 0) {
                    SetY (Yi + 2 * TAMTILE);
                    SetVelY(0);
                    if (VerSent())
                       SetVelX (-2);
                     else
                       SetVelX(2);  // Pon la VelX que corresponda según el sentido
                    }
                 if (VerY() <= Yi && VerVelY() < 0) {
                    SetY (Yi);
                    SetVelY(0);
                    if (VerSent())
                       SetVelX (-2);
                     else
                       SetVelX(2);  // Pon la VelX que corresponda según el sentido
                    }
                 if (!VerVelX() && !VerVelY())
                    if (VerSent())
                       SetVelX (-2);
                     else
                       SetVelX(2);  // Pon la VelX que corresponda según el sentido

                 break;
           case ALIEN:
                if (++Rebote >= FramesCirculo)
                   Rebote = 0;
               SetVelY (2 * FuncionSeno[Rebote]);
               if (VerX(true) < Jugador->VerX()) {
                  if (VerFotog() == 5 && Frames == 1)
                     SetVelX (5);
                  else
                     if (VerVelX() > 0.5)
                        IncVelX(-.5);
                  if (VerSent() == 1 && VerEst() != GIRANDO) {
                     SetEst(GIRANDO);
                     ClrFotog();
                     }
                  }
               if (VerX() > Jugador->VerX(true)) {
                  if (VerFotog() == 5 && Frames == 1)
                     SetVelX (-5);
                  else
                     if (VerVelX() < 0.5)
                        IncVelX(.5);
                  if (VerSent() == 0 && VerEst() != GIRANDO) {
                     SetEst(GIRANDO);
                     ClrFotog();
                     }
                  }
               if (!((int)VerVelX()))
                  SetVelX (2 * FuncionCoseno[Rebote]);
               if (VerY(true) < Jugador->VerY())
                  IncVelY (1);
               if (VerY() > Jugador->VerY(true))
                  IncVelY (-1);
               if (ReadMessage() != DESPLAZA && (
                   (VerX() + VerVelX() <= 0 && VerVelX() < 0) || (VerX(true) + VerVelX() >= TILESANCHO * TAMTILE && VerVelX() > 0)))
                  SetVelX(0);
               if (VerEst() == GIRANDO && FinAnim()) {
                  SetEst (VOLANDO);
                  Reverse();
                  ClrFotog();
                  }
                 break;
           case BURGUER:
                if (Estado == CAYENDO) {
                   if (FinAnim() && !VerVelY())
                      if (VerGrav() == false)
                         SetGrav(true);
                        else
                          SendMessage(COLISION);

                   if ((int)(VerY(true) + VerVelY() + 1) >= TAMTILE * TILESALTO) {
                      while (VerY(true) >= TAMTILE * TILESALTO)
                            IncY(-1);
                      SetVelY(0);
                      SetGrav(false);
                      SendMessage(COLISION);
                      }
                   }
                 else {

                   if ((VerX() + VerVelX() <= 0 && VerVelX() < 0) ||
                       (VerX(true) + VerVelX() >= TILESANCHO * TAMTILE && VerVelX() > 0)) {
                      Rebote = 0;
                      SetVelX(0);
                      if (VerY(true) < (TILESALTO - 1) * TAMTILE)
                         SetVelY(2);
                       else
                         SetVelY(-2);
                      }
                   if (VerVelY()) {
                      Rebote += (int) VerVelY();
                      if ((VerVelY() > 0 && Rebote >= 20) ||
                          (VerVelY() + VerY() < 0)) {
                         SetVelY(0);
                         if (VerX() < (TILESANCHO / 2) * TAMTILE)
                            SetVelX(2);
                          else
                            SetVelX(-2);
                          }
                      }
                   }
                break;
           case JERING:
                   if (Estado == CAYENDO && FinAnim() && !VerVelY())
                      if (VerGrav() == false)
                         SetGrav(true);
                        else
                          SendMessage(COLISION);

                   if (Estado == CAYENDO && (int)(VerY(true) + VerVelY() + 1) >= TAMTILE * TILESALTO) {
                      while (VerY(true) >= TAMTILE * TILESALTO)
                            IncY(-1);
                      SetVelY(0);
                      SetGrav(false);
                      SendMessage(COLISION);
                      }

                if (VerEst() != DISPARANDO && VerEst() != CAYENDO) {
                   if (VerX() < Jugador->VerX())
                      Sentido = 1;
                     else
                      Sentido = 0;
                   SetVelX(0);
                   SetVelY(0);
                   if (VerX() < (TILESANCHO * TAMTILE - Jugador->VerX(true)))
                      SetVelX(1);
                   if (VerX(true) > (TILESANCHO * TAMTILE - Jugador->VerX()))
                      SetVelX(-1);
                   if (VerY(true) < Jugador->VerY(true))
                      SetVelY (1);
                   if (VerY(true) > Jugador->VerY(true))
                      SetVelY (-1);
                   }
                if (VerEst() == DISPARANDO && FinAnim()) {
                   SetEst (VOLANDO);
                   CiclosQuieto = 0;
                   }
                if (VerEst() == DISPARANDO && VerFotog() == 3 && Frames == 1) {
                   SFx->PlayFX (DF_SDISPLASER2);
                   Retorno.Tipo = LASERJ;
                   Retorno.Estado = 0;
                   Retorno.Sentido = !Sentido;
                   Retorno.Equipo = ENEMIGO;
                   Retorno.X = VerX(Sentido) - 20 * (!Sentido);
                   Retorno.Y = VerY() + 4;
                   Retorno.Capa = 0;
                   CiclosQuieto = 0;
                   }
                if (VerY() >= Jugador->VerY() && VerY(true) <= Jugador->VerY(true) && VerEst() != CAYENDO)
                   if (VerEst() != DISPARANDO && ++CiclosQuieto > (35 - 5 * Config->VerDif())) {
                      SetEst (DISPARANDO);
                      ClrFotog();
                      SetVelX(0);
                      SetVelY(0);
                      }
                break;
           case ASOLDIER:
                SetVelX(0);
                SetVelY(0);
                if (VerEst() != DISPARANDO)
                   if (VerEst() != GIRANDO &&
                      ((VerX() < Jugador->VerX() && Sentido == 1) || (VerX() > Jugador->VerX() && Sentido == 0))) {
                       SetEst(GIRANDO);
                       ClrFotog();
                       }
                     else {
                       SetVelX(0);
                       SetVelY(0);
                       if (VerX(true) < (TILESANCHO * TAMTILE - Jugador->VerX()))
                          SetVelX(2);
                       if (VerX(true) > (TILESANCHO * TAMTILE - Jugador->VerX()))
                          SetVelX(-2);
                       if (VerY(true) - 20 < Jugador->VerY(true))
                          SetVelY (1);
                       if (VerY(true) - 20 > Jugador->VerY(true))
                          SetVelY (-1);
                       }
                if (VerEst() == GIRANDO && FinAnim()) {
                   SetEst(VOLANDO);
                   Reverse();
                   ClrFotog();
                   }
                if (VerEst() == DISPARANDO && FinAnim()) {
                   SetEst (VOLANDO);
                   CiclosQuieto = 0;
                   }
                if (VerEst() == DISPARANDO && VerFotog() == 4 && Frames == 1) {
                   SFx->PlayFX (DF_SDISPLASER2);
                   Retorno.Tipo = LASERJ;
                   Retorno.Estado = 0;
                   Retorno.Sentido = Sentido;
                   Retorno.Equipo = ENEMIGO;
                   if (!
                   Sentido)
                      Retorno.X = VerX(true) - 2;
                    else
                      Retorno.X = VerX() + 2;
                   Retorno.Y = VerY() + 38;
                   Retorno.Capa = 0;
                   CiclosQuieto = 0;
                   }
                if (VerY(true) >= Jugador->VerY() && VerY() <= Jugador->VerY(true))
                   if (VerEst() == VOLANDO && ++CiclosQuieto > (30 - 5 * Config->VerDif())) {
                      SetEst (DISPARANDO);
                      ClrFotog();
                      SetVelX(0);
                      SetVelY(0);
                      }
                break;
           case HUMOJ:
                if (VerEst() == ANDANDO && ++CiclosQuieto > 40 && !((int)rand() % 40))
                   SetEst (CAYENDO);
                if (VerEst() == PARADO && ++CiclosQuieto > 40 && !((int)rand() % 40))
                   SetEst (SUBIENDO);
                if (VerEst() == CAYENDO) {
                   IncTrans (-4);
                   if (VerTrans() < 5) {
                      SetEst(PARADO);
                      ClrFotog();
                      CiclosQuieto = 0;
                      }
                   }
                if (VerEst() == SUBIENDO) {
                   IncTrans (4);
                   if (VerTrans() > 127) {
                      SetEst(ANDANDO);
                      ClrFotog();
                      CiclosQuieto = 0;
                      }
                   }
                break;

           case BOLITA:
                if (!VerVelX() || (Capa == 2 && !VerVelY()))
                   if (VerEst() == PARADO)
                      SetEst(ANDANDO);
                    else
                      SendMessage (COLISION);
                if (!VerSent())
                   SetVelX(6);
                 else
                   SetVelX(-6);
                if (Capa == 2)
                   SetVelY (4);
                 break;

           case ROBOTIN:
                   if (Estado == CAYENDO && FinAnim() && !VerVelY())
                      if (VerGrav() == false)
                         SetGrav(true);
                        else
                          SendMessage(COLISION);

                   if (Estado == CAYENDO && (int)(VerY(true) + VerVelY() + 1) >= TAMTILE * TILESALTO) {
                      while (VerY(true) >= TAMTILE * TILESALTO)
                            IncY(-1);
                      SetVelY(0);
                      SetGrav(false);
                      SendMessage(COLISION);
                      }

                   if (VerEst() != GIRANDO &&
                      ((VerX() > Jugador->VerX(true) && VerSent() == 0) ||
                       (VerX(true) < Jugador->VerX() && VerSent() == 1))) {
                       SetEst(GIRANDO);
                       ClrFotog();
                       }

                if (VerEst() == GIRANDO && FinAnim()) {
                   Reverse();
                   SetEst (VOLANDO);
                   ClrFotog();
                   }

                if (VerEst() != DISPARANDO && VerEst() != CAYENDO && VerEst() != GIRANDO) {
                   SetVelX(0);
                   SetVelY(0);
                   if (VerX() < (TILESANCHO * TAMTILE - Jugador->VerX(true)))
                      SetVelX(1);
                   if (VerX(true) > (TILESANCHO * TAMTILE - Jugador->VerX()))
                      SetVelX(-1);
                   if (VerY(true) < Jugador->VerY(true) && !((int)(rand()) % 10))
                      SetVelY (1);
                   if (VerY(true) > Jugador->VerY(true) && !((int)(rand()) % 10))
                      SetVelY (-1);
                   }
                if (VerEst() == DISPARANDO && FinAnim()) {
                   SetEst (VOLANDO);
                   CiclosQuieto = 0;
                   }
                if (VerEst() == DISPARANDO && VerFotog() == 4 && Frames == 1) {
                   SFx->PlayFX(DF_SCANON);
                   Retorno.Tipo = BOLITA;
                   Retorno.Estado = 0;
                   Retorno.Sentido = Sentido;
                   Retorno.Equipo = ENEMIGO;
                   if (Sentido)
                      Retorno.X = VerX();
                    else
                      Retorno.X = VerX(true);
                   Retorno.Y = VerY(true) - 18;
                   Retorno.Capa = 3;
                   CiclosQuieto = 0;
                   }
                if (VerEst() != CAYENDO && VerEst() != DISPARANDO && ++CiclosQuieto > 25 && !((int) rand()% (30 - 5 * Config->VerDif()))) {
                   SetEst (DISPARANDO);
                   ClrFotog();
                   SetVelX(0);
                   SetVelY(0);
                   }
                break;


           case SALTARIN:
                if (ReadMessage() != DESPLAZA)
                   CiclosQuieto = 0;
                 else
                   if (++CiclosQuieto > 5) {
                      SendMessage (0);
                      SetVelX (0);
                      }

                if (VerEst() == PARADO && ReadMessage() == GIRAR) {
                   SetEst (GIRANDO);
                   SendMessage (0);
                   ClrFotog();
                   }

                if (VerEst() == PARADO &&
                   ((VerX(true) < Jugador->VerX() && VerSent() == 1) ||
                   (VerX () > Jugador->VerX(true) && VerSent() == 0))) {
                         SetEst (GIRANDO);
                         ClrFotog();
                         }

                if (VerEst () == GIRANDO && FinAnim()) {
                   Reverse();
                   SetEst(PRESALTO);
                   ClrFotog();
                   }

                if (VerEst() == PARADO && FinAnim())
                   SetEst(PRESALTO);

                if (VerEst() == SUBIENDO && !VerVelX())
                   if (VerSent())
                      SetVelX (-3);
                    else
                      SetVelX(3);  // Pon la VelX que corresponda según el sentido

                if (((VerX() + VerVelX() <= 0 && VerVelX() < 0) ||
                     (VerX(true) + VerVelX() >= ANCHOPANTALLA && VerVelX() > 0))) { // ... y te vas a salir de la pantalla, vuélvete
                        SetVelX(0);
                        ClrFotog();
                        SendMessage (GIRAR);
                   }

                 break;

           case ASTRONAUTA2:
                if (VerEst() == PARADO && !int(rand()%40)) {  // Si esta quieto y toca moverse
                   if ((Jugador->VerX() < VerX() && VerSent() == 0) ||
                       (Jugador->VerX() > VerX() && VerSent() == 1))
                      SetEst (GIRANDO);
		            else {
                      CiclosQuieto = 0;
                      SetEst (ANDANDO);
                      if (VerSent()) SetVelX(-2);
     		            else SetVelX (2);
                      }
                   ClrFotog();
                   }

                if (VerEst() == GIRANDO && FinAnim()) {
		           Reverse();

		           SetEst(ANDANDO);
                   if (VerSent()) SetVelX(-2);
		            else SetVelX (2);
                   CiclosQuieto = 0;
		           ClrFotog();
		           }

                if (VerEst() == ANDANDO && ++CiclosQuieto > 24 && !(int) (rand()%20)) { // Si lleva andando 20 ciclos y "te da por ahí"...
                   SetEst (PARADO);
                   ClrFotog();
                   }

                if (VerEst() == ANDANDO && (VerX() + VerVelX() < 4 || VerX(true) + VerVelX() > (ANCHOPANTALLA - 4))) { // Si te vas a salir de la pantalla, vuélvete
                   SetEst (GIRANDO);
		           ClrFotog();
                   SetVelX (0);
                   }
                if (VerEst() != ANDANDO)
                  SetVelX(0);

                if (VerEst() == PARADO && Jugador->VerTipo() == CAPITAN && !(int)(rand() % (35 - 5 * Config->VerDif()))) {
                   SetEst (DISPARANDO);
                   ClrFotog();
                   }

                if (VerEst() == DISPARANDO && VerFotog() == 4 && Frames == 1) {
                   SFx->PlayFX (DF_SDISPLASER2);
                   Retorno.Tipo = LASERJ;
                   Retorno.Estado = 0;
                   Retorno.Sentido = Sentido;
                   Retorno.Equipo = ENEMIGO;
                   if (!Sentido)
                      Retorno.X = VerX(true);
                    else
                      Retorno.X = VerX() - 6;
                   Retorno.Y = VerY() + 44;
                   Retorno.Capa = 0;
                   CiclosQuieto = 0;
                   }
                if (VerEst() == DISPARANDO && FinAnim()) {
                   SetEst (PARADO);
                   ClrFotog();
                   }

                break;

           case MONSTRUO:
                if (VerEst() == NACIENDO && FinAnim() && ++CiclosQuieto >= 10 && !((int)(rand() % 10))) {
                   SetEst (SURGIENDO);
                   SFx->PlayFX (DF_SAPARECE);
                   ClrFotog();
                   }
                if (VerEst() == SURGIENDO && FinAnim()) {
                   SetEst (PARADO);
                   Equipo = ENEMIGO;
                   SFx->PlayFX (DF_SROAR);
                   ClrFotog();
                   }
                if (VerEst() == PARADO && FinAnim()) {
                   SetEst (ANDANDO);
                   ClrFotog();
                   }
                if (VerEst() == ANDANDO &&
                   ((VerX() + VerVelX() < 0 && VerSent() == 1) ||
                    (VerX(true) + VerVelX() > ANCHOPANTALLA && VerSent() == 0))) {
                    SetEst (ENTERRANDOSE);
                    SFx->PlayFX (DF_SDESAPARECE);
                    Equipo = NEUTRO;
                    ClrFotog();
                    SetVelX (0);
                    }

                if (VerEst() == ENTERRANDOSE && FinAnim())
                   SetTrans (-1); // Cuando vaya a moverse retorna SALEPANTALLA pq cree que está muerto

                if (VerEst() == ANDANDO)
                   if (VerSent())
                      SetVelX (-2);
                    else
                      SetVelX(2);  // Pon la VelX que corresponda según el sentido
                if (ReadMessage() == DESPLAZA && VerEst() != ANDANDO)
                   SendMessage (0);

                break;

           case ASPIRADOR:
                if (Estado == CAYENDO) {
                   if (FinAnim() && !VerVelY())
                      if (VerGrav() == false)
                         SetGrav(true);
                        else
                          SendMessage(COLISION);

                   if ((int)(VerY(true) + VerVelY() + 1) >= TAMTILE * TILESALTO) {
                      while (VerY(true) >= TAMTILE * TILESALTO)
                            IncY(-1);
                      SetVelY(0);
                      SetGrav(false);
                      SendMessage(COLISION);
                      }
                   }
                 else {
                   if (++CiclosCayendo >= FramesCirculo)
                      CiclosCayendo = 0;
                      SetVelY (FuncionSeno[CiclosCayendo]);
                      }


                 if (VerEst() == GIRANDO && FinAnim()) {
                    SetEst(VOLANDO);
                    CiclosQuieto = 0;
                    ClrFotog();
                    Reverse();
                    if (VerSent() == 0)
                       SetVelX (2);
                     else
                       SetVelX (-2);
                    }

                   if (VerEst() == VOLANDO && Jugador->VerTipo() == MARIANO) {
                      if ((VerX() + VerVelX() <= 0 && VerVelX() < 0) ||
                         (VerX(true) + VerVelX() >= TILESANCHO * TAMTILE && VerVelX() > 0)) {
                            Rebote = 0;
                            SetVelX(0);
                            if (VerY(true) < (TILESALTO - 1) * TAMTILE)
                               SetVelY(6);
                             else
                               SetVelY(-6);
                            }
                      if (VerVelX() == 0 && VerEst() == VOLANDO) {
                         Rebote++; //= (int) (VerVelY() - FuncionSeno[CiclosQuieto]);
                         if ((VerVelY() > 0 && Rebote >= 20) ||
                             (VerVelY() + VerY() < 0)) {
//                               SetVelY(FuncionSeno[CiclosQuieto]);
                               SetEst (GIRANDO);
                               ClrFotog();
                               }
                         }
                      }

                   if (VerEst() == VOLANDO && Jugador->VerTipo() == CAPITAN) {
                       if (Jugador->VerX() < VerX())
                          if (VerSent() == 0) {
                             SetEst(GIRANDO);
                             ClrFotog();
                             }
                           else
                             SetVelX(-1);
                       if (Jugador->VerX(true) > VerX(true))
                          if (VerSent() == 1) {
                             SetEst(GIRANDO);
                             ClrFotog();
                             }
                           else
                             SetVelX(1);
                       if (Jugador->VerY() < VerY() && !((int) (rand() % 3)))
                          IncVelY(-1);
                       if (Jugador->VerY() > VerY() && !((int) (rand() % 3)))
                          IncVelY(1);
                       }

                if (VerEst() == GIRANDO)
                   SetVelX(0);

                if (VerVelX() && ++CiclosQuieto > 25 && !((int)(rand()) % (15 - 5 * Config->VerDif()))) {
                   SetEst (DISPARANDO);
                   ClrFotog();
                   SetVelX(0);
                   SetVelY(0);
                   }
                if (VerEst() == DISPARANDO && FinAnim()) {
                   SetEst (VOLANDO);
                   CiclosQuieto = 0;
                    if (VerSent() == 0)
                       SetVelX (2);
                     else
                       SetVelX (-2);
                   }

                if (VerEst() == DISPARANDO && VerFotog() == 4 && Frames == 1) {
                   SFx->PlayFX(DF_SCANON);
                   Retorno.Tipo = BOLITA;
                   Retorno.Estado = 0;
                   Retorno.Sentido = Sentido;
                   Retorno.Equipo = ENEMIGO;
                   if (Sentido)
                      Retorno.X = VerX(true)-60;
                    else
                      Retorno.X = VerX()+55;
                   Retorno.Y = VerY() + 45;
                   Retorno.Capa = 2;
                   CiclosQuieto = 0;
                   }
                break;

          case SHORTCIRCUIT:
               if (!(int (rand() % 80)))
                   if (!int (rand() % 2))
                        SFx->PlayFX (DF_SINTRUDER);
                     else
                        SFx->PlayFX (DF_SDESTROYH);
          case RODADOR:
               if (VerEst() == ANDANDO) {
                  if (VerSent() == 0)
                     SetVelX (2);
                    else
                     SetVelX (-2);
                  if ((VerSent() == 0 && VerX() > Jugador->VerX()) ||
                      (VerSent() == 1 && VerX() < Jugador->VerX()))
                          if (!(int)(rand() % 20))
                             Rebote = 1;
                  if (Rebote == 0)
                     if (++CiclosQuieto > (20 + 10 * (4 - KeyFrame)) && KeyFrame > 1) {
                        KeyFrame--;
                        CiclosQuieto = 0;
                        }
                  if (Rebote == 1)
                     if (++CiclosQuieto > (20 + 10 * (4 - KeyFrame)) && KeyFrame < 5) {
                        KeyFrame++;
                        CiclosQuieto = 0;
                        }
                  if ((VerX() < 50 && VerSent() == 1) ||
                      (VerX(true) > 462 && VerSent() == 0))
                          Rebote = 1;
                  if (Rebote == 1 && KeyFrame == 3) {
                      Rebote = 0;
                      if ((VerX() < Jugador->VerX() && VerSent() == 1) ||
                          (VerX() > Jugador->VerX() && VerSent() == 0)) {
                       SetEst (GIRANDO);
                       SetKF (2);
                       SetVelX (0);
                       ClrFotog();
                       }
                  }}
               if (VerEst() == GIRANDO && FinAnim()) {
                  SetEst (ANDANDO);
                  Reverse();
                  ClrFotog();
                  CiclosQuieto = 0;
                  SetKF (4);
                  }
               break;

          case GRUA:
               if (VerEst() == PARADO && FinAnim()) {
                  if ((VerSent() == 0 && Jugador->VerX(true) < VerX()) ||
                      (VerSent() == 1 && Jugador->VerX() > VerX(true)))
                          SetEst (GIRANDO);
                      else
                          SetEst (ANDANDO);
                  ClrFotog();
                  CiclosQuieto = 0;
                  }
               if (VerEst() == GIRANDO && FinAnim()) {
                  SetVelX(0);
                  SetEst (ANDANDO);
                  ClrFotog();
                  CiclosQuieto = 0;
                  Reverse();
                  }
               if (VerEst() == ANDANDO) {
                  if (VerSent() == 0)
                     SetVelX (2);
                   else
                     SetVelX (-2);

                   if ((VerX(true) + VerVelX() >= ANCHOPANTALLA) ||
                       (VerX() + VerVelX() <= 0)) {
                       SetVelX(0);
                       SetEst(GIRANDO);
                       ClrFotog();
                       CiclosQuieto = 0;
                       }

                   if (++CiclosQuieto > 20 && !((int)rand() % 30)) {
                      SetEst (PARADO);
                      ClrFotog();
                      CiclosQuieto = 0;
                      SetVelX (0);
                      }
                   }
               if (VerEst() == PARADO && VerFotog() == 3 && Frames == 1)
                    SFx->PlayFX (DF_SPINZA);
               break;

          case PC:
               if (ReadMessage() != GIRAR)
                  Rebote = 1;
                 else {
                  if (Rebote) {
                     (VerPertenencia())->SetEstDest ((VerPertenencia())->VerEst() + 1);
                     Rebote = 0;
                     SFx->PlayFX (DF_SKEYBOARD);
                     }
                  SendMessage (TODOOK);
                     }
               break;
          case PANTALLA:
               if (VerEstDest() >= 0) {
                  if (VerEstDest() >= MAXPANTALLA)
                     SetEstDest (0);
                  IncTrans (-10);
                  if (!VerTrans()) {
                     SetEst (VerEstDest());
                     SetEstDest (-1);
                     SetTrans (255);
                     }
                  }

                if (VerEst() == 3) {
                   if ((VerPertenencia())->VerEquipo() != NEUTRO)
                       SFx->PlayFX (DF_SPUERTA);
                   ((VerPertenencia())->VerPertenencia())->SetEst (1);
                   (VerPertenencia())->SetEquipo(NEUTRO);
                   }
                 else {
                   if ((VerPertenencia())->VerEquipo() == NEUTRO)
                       SFx->PlayFX (DF_SPUERTA);
                   ((VerPertenencia())->VerPertenencia())->SetEst (0);
                   (VerPertenencia())->SetEquipo(OBSTACULO);
                   }
                 break;
           case TORRETA:
                SetKF (3);
                if (VerFotog() < 6) {
                   Estado = 1;
                   if ((Sentido && Jugador->VerX(true) >= VerX()) ||
                      (!Sentido && Jugador->VerX() <= VerX(true)))
                        Estado = 2;
                   if (Jugador->VerY() <= VerY(true))
                      Estado = 0;
                      }

                if (VerFotog() < 6 && !((int)rand() % (10 + 15 * Config->VerDif())))
                    ClrFotog();

                if (VerFotog() == 10 && Frames == 1) {
                   SFx->PlayFX (DF_SDISPLASER2);
                   Retorno.Tipo = LASERJ;
                   Retorno.Estado = 0;
                   Retorno.Sentido = Sentido;
                   Retorno.Equipo = ENEMIGO;
                   Retorno.Capa = Estado;
                   switch (Estado) {
                          case 0:
                               Retorno.Y = VerY() + 7;
                               Retorno.X = VerX(!Sentido) - Sentido * 20;
                               break;
                          case 1:
                               Retorno.Y = VerY(true);
                               Retorno.X = VerX(!Sentido) + 3 * (1 - 2 * Sentido) - Sentido * 20;
                               break;
                          case 2:
                               Retorno.Y = VerY(true);
                               Retorno.X = VerX(Sentido) + 7 * (1 - 2 * Sentido) - Sentido * 20;
                               break;
                          }
                   ClrFotog();
                   }
                break;
           case ASTRONAUTA3:
                if (VerEst() == ANDANDO) {
                  if (VerSent() == 0)
                     SetVelX (2);
                   else
                     SetVelX (-2);
                  if (((VerX() + VerVelX() < 0 && VerSent() == 1) || (VerX(true) + VerVelX() > ANCHOPANTALLA && VerSent() == 0)) &&
                       VerEst() != GIRANDO) {
                     SetEst (GIRANDO);
                     ClrFotog();
                     SetVelX(0);
                     }
                  }
                if (VerEst() == GIRANDO && FinAnim()) {
                   SetEst (ANDANDO);
                   Reverse();
                   ClrFotog();
                   }
                break;

           case TORRES:
                if (VerEst() == PARADO) {
                    if ((VerX()+50 > Jugador->VerX(true) && VerSent() == 0) ||
                        (VerX(true)-50 < Jugador->VerX() && VerSent() == 1)) {
                            SetEst(GIRANDO);
                            ClrFotog();
                        }
                    }
                if (VerEst() == GIRANDO && FinAnim()) {
                    Reverse();
                    SetEst(PARADO);
                    ClrFotog();
                    CiclosQuieto = 0;
                    }
                if (VerEst() == PARADO && ++CiclosQuieto > 80) {
                    if ((VerY() + 26 > Jugador->VerY() && VerY() + 26 < Jugador->VerY (true)) || !(int)(rand() % 3))
                       SetEst(DESENFUNDA);
                     else
                       SetEst(DESAPARECE);
                    ClrFotog();
                    CiclosQuieto = 0;
                    }

                if ((VerX() < 0 || VerX(true) > TILESANCHO * TAMTILE) && VerEst() != DESAPARECE && VerEst() != OCULTO) {
                   SetEst (DESAPARECE);
                   ClrFotog();
                   CiclosQuieto = 0;
                   }

                if (VerEst() == DESENFUNDA && FinAnim()) {
                   SetTipo (TORREC);
                   SetEst (STAND + ((int)(rand() % 2)));
                   ClrFotog();
                   CiclosQuieto = 0;
                   }

                if (VerEst() == DESAPARECE && FinAnim()) {
                    SetEst (OCULTO);
                    ClrFotog();
                    }

                if (VerEst() == OCULTO && ++CiclosQuieto > 40) {
                    SetX ((int)(rand() % (TILESANCHO * TAMTILE - VerLongX())));
                    SetY (0);
                    SetSent ((int)(rand() % 2));
                    SetEst(PARADO);
                    ClrFotog();
                    do {
                        Salida = 0;
                        IncY (2);
                        Coordenada.X = (VerX()+48) / TAMTILE;
                        Coordenada.Y = (VerY(true)) / TAMTILE;
                        for (int n = 0; n < ((VerLongX()-48) / TAMTILE); n++) {
                            Coordenada.X += n;
                            Salida += ColisionPP (M->Mask(TILE,0,0,((Hab*)VJ.HAct)->CContenido (Coordenada, false)),
                                      Coordenada.X * TAMTILE, Coordenada.Y * TAMTILE - 2, M->Mask(TORRES,Sentido,PARADO,0));
                            }
                        }
                        while (!Salida);

                    SetEst (APARECE);
                    SFx->PlayFX (DF_SAPATORRE);
                    ClrFotog();
                    CiclosQuieto = 0;
                    }
                if (Energia < 125) {
                    CiclosQuieto = 0;
                    Energia = 126;
                    ClrFotog();
                    if (Vidas--) {
                       SetEst (ATURDIDO);
                       SFx->PlayFX (DF_SBIRDS);
                     }
                     else
                       SendMessage (TERMINADO);
                    }

                if (VerEst() == ATURDIDO && ++ CiclosQuieto > 120) {
                    SetEst (DESAPARECE);
                    ClrFotog();
                    CiclosQuieto = 0;
                }

                if (VerEst() == APARECE && FinAnim()) {
                    SetEst (PARADO);
                    ClrFotog();
                    }
                if (VerEst() != APARECE && VerEst() != DESAPARECE && VerEst() != OCULTO && !VerGrav())
                    SetGrav (true);
                if (VerEst() == APARECE || VerEst() == DESAPARECE || VerEst() == OCULTO && VerGrav())
                    SetGrav (false);

                SetEquipo (ENEMIGO);
                if (VerEst() == ATURDIDO || (VerEst() == APARECE && !FinAnim()) ||
                   (VerEst() == DESAPARECE && VerFotog() > 1) || (VerEst() == GIRANDO && !FinAnim()))
                    SetEquipo (NEUTRO);

                if (VerEst() == DESAPARECE && VerFotog() == 0 && Frames == 1)
                    SFx->PlayFX (DF_SDESAPATORRE);

                break;

           case TORREC:
                CiclosQuieto++;

                if (ReadMessage() == RIENDO && VerEst() != RIENDO) {
                   SetEst(RIENDO);
                   SendMessage (0);
                   ClrFotog();
                   break;
                   }

                if (Energia < 125) {
                    CiclosQuieto = 0;
                    Energia = 126;
                    ClrFotog();
                    if (Vidas--) {
                       SetEst (ATURDIDO);
                       SFx->PlayFX (DF_SBIRDS);
                    }
                     else
                       SendMessage (TERMINADO);
                    }

                if (VerEst() == ATURDIDO && ++ CiclosEspera > 120) {
                    SetEst (PARADO);
                    ClrFotog();
                    CiclosEspera = 0;
                }

                if ((VerX() < 0 || VerX(true) > TILESANCHO * TAMTILE) && VerEst() != ENFUNDA) {
                   SetEst (ENFUNDA);
                   ClrFotog();
                   }

                if (FinAnim() && VerEst() != ATURDIDO) {

                    if (VerEst() == RIENDO) {
                       ClrFotog();
                       if (++Rebote > 3) {
                          SetEst (PARADO);
                          ClrFotog();
                          Rebote = 0;
                          }
                       break;
                       }

                    if (VerEst() == ENFUNDA) {
                       SetTipo(TORRES);
                       SetEst (DESAPARECE);
                       CiclosQuieto = 0;
                       ClrFotog();
                       break;
                       }

                    if (VerEst() == GIRANDO) {
                       Reverse();
                       CiclosEspera = 0;
                       SetEst (PARADO);
                       ClrFotog();
                       break;
                       }

                    if ((VerX()+50 > Jugador->VerX(true) && VerSent() == 0) ||
                        (VerX(true)-50 < Jugador->VerX() && VerSent() == 1)) {
                            SetEst(GIRANDO);
                            ClrFotog();
                            break;
                            }

                    if (VerY() + 26 > Jugador->VerY() && VerY() + 26 < Jugador->VerY (true) && CiclosQuieto < 240) {
                       SetEst (DISPARANDO);
                       CiclosEspera = 0;
                       ClrFotog();
                       break;
                       }

                    if (CiclosQuieto > 180) {
                       SetEst (ENFUNDA);
                       ClrFotog();
                       break;
                       }

                    if (CiclosQuieto > 90 && !((int) rand() % 10)) {
                       SetEst (DISPARANDO);
                       ClrFotog();
                       break;
                       }

                    if (VerEst() == DISPARANDO) {
                       CiclosEspera = 0;
                       SetEst (PARADO);
                       ClrFotog();
                       break;
                       }

                    }

                if (VerEst() == PARADO && ++CiclosEspera > 20 && !((int) rand() % 10)) {
                   SetEst (STAND + ((int)(rand() % 2)));
                   ClrFotog();
                   }

                if (VerEst() == DISPARANDO && VerFotog() == 1 && Frames == 1) {
                   if (++Rebote > 3 + Config->VerDif()) {
                      SetEst (ENFUNDA);
                      ClrFotog();
                      }
                   SFx->PlayFX (DF_SDISPLASER);

                   Retorno.Tipo = RAYOTORRE;
                   Retorno.Estado = 0;
                   Retorno.Sentido = Sentido;
                   Retorno.Equipo = ENEMIGO;
                   if (Sentido)
                      Retorno.X = VerX()+6;
                    else
                      Retorno.X = VerX()+115;
                   Retorno.Y = VerY() + 27;
                   Retorno.Capa = 2;
                   CiclosQuieto = 0;
                   }

                if (VerEst() != DISPARANDO && VerEst() != RIENDO)
                   Rebote = 0;

                SetEquipo (ENEMIGO);
                if (VerEst() == ATURDIDO || (VerEst() == APARECE && !FinAnim()) ||
                   (VerEst() == DESAPARECE && VerFotog() > 1) || (VerEst() == GIRANDO && !FinAnim()))
                    SetEquipo (NEUTRO);

                if (VerEst() == DESAPARECE && VerFotog() == 0 && Frames == 1)
                    SFx->PlayFX (DF_SDESAPATORRE);

               break;

           case BURBUJA:
                if (++CiclosQuieto > 6 && Config->VerDinamic()) {
                   CiclosQuieto = 0;
                   IncX(2);}
                break;
           case MARIANOMUERTO:
                if (VerEst() == POSTSALTO && VerFotog() == 13) {
                   Retorno.Tipo = ALMA;
                   Retorno.Estado = SUBIENDO;
                   Retorno.Sentido = 0;
                   Retorno.Equipo = NEUTRO;
                   Retorno.X = VerX();
                   Retorno.Y = VerY();
                   Retorno.Capa = 3;
                   }
                if (VerEst() == POSTSALTO && VerFotog() == 1) {
                   H->Mamporro (16);
                   if (!Suena[DF_SMAMPORRO]) {
                      SFx->PlayFX (DF_SMAMPORRO);
                      Suena[DF_SMAMPORRO] = true;
                      }
                   }
                if (VerEst() == POSTSALTO && VerFotog() > 1)
                   Suena [DF_SMAMPORRO] = false;

                if (!VerVelY() && (VerEst() == SUBIENDO || VerEst() == CAYENDO))
                   IncY (2);

                if (H->Tecla() & ABORT)
                   return ABORT;
                break;
           case ALMA:
                if (VerEst() == SUBIENDO) {
                   SetVelX(0);
                   SetVelY(0);
                   if (FinAnim()) {
                      SetEst (VOLANDO);
                      SetVelY (-1);
                      ClrFotog();
                      }
                   }
                 else
                   IncTrans (-2);
                break;
           case OBJVENTANA:
                SetVelX(0);
                break;
           case ONOMAT:
                SetVelY (-.5);
                IncTrans (-7);
                break;
           case PANTALLA8B:
                if (FinAnim()) {
                   SetEst ((int) (rand() % 3));
                   ClrFotog();
                   }
                 break;
           case ESCAFANDRAM:
                if (FinAnim()) {
                   Explota();
                   SendMessage (COLISION);
                   }
                break;
           case LASERJ:
                if (!VerVelX() && !VerVelY())
                   SendMessage (COLISION);
                break;
           case MESACAPI:
           case MARIANOBEBE:
                SetKF (10);
                if (FinAnim() && !((int)rand() % 20))
                    ClrFotog();
                break;
           case SIRFRED:
                SetKF (8);
                if (FinAnim()) {
                    ClrFotog();
                    if (++Estado > 4)
                        Estado = 0;
                    }
                break;
    }

    if (ReadMessage() == DESPLAZA && VerTipo() != TORRES && VerTipo() != TORREC && VerTipo() != RAYOTORRE)
       if (Jugador->VerY() + 26 <= VerY(true) && Jugador->VerY() + 32 >= VerY()) {
           if (Jugador->VerEst() == PARADO)
              Jugador->SetEst (ANDANDO);
           if (Jugador->VerSent() == 0) {
              if (VerTipo() == BALON && VerVelX() < 0)
                 Reverse();
              if (VerVelX() <= 4)
                 SetVelX (4);
               else
                 IncVelX (2);
              if (VerTipo() == NEGRO || VerTipo() == GANGSTER)
                 IncVelX (1);
              }
            else {
              if (VerTipo() == BALON && VerVelX() > 0)
                 Reverse();
              if (VerVelX() >= -4)
                 SetVelX (-4);
               else
                 IncVelX (-2);
              if (VerTipo() == NEGRO || VerTipo() == GANGSTER)
                 IncVelX (-1);
              }
            }

    AjustaKF();
    Actualiza();
    return 0;
}

/**********************************************************************************************
Método Mueve

Recibe:
Mundo *MAct: Puntero al "Mundo" (fase) actual (para comprobar las conexiones de la hab actual)
Hab *HAct: Puntero a la habitación actual;
Actor *Inicial: Puntero al primer actor de la lista de actores (para valorar colisiones con Objetos - PLATAFORMA)

Se encarga de aplicar el movimiento "pendiente", recibido del método "PreparaMov". Aplica la "física",
detecta colisiones con los tiles, vigila la salida de la habitación actual del Spr, y acta en consecuencia.

Tb. activa "automáticamente" el estado "PARADO" en caso pertinente, y gestiona las animaciones de "inactividad".

Devuelve:
0 - TODOOK si no pasa nada
SALEPANTALLA si sale de la pantalla
**********************************************************************************************/
int Actor::Mueve (Mundo *MAct, Hab *HAct, Actor *Inicial)
{
   /* Este método va a haber que subdividirlo según tipo de actor.
   Que se llame aquí, pero que luego haya distintos métodos "privados"
   para mover distintos actores: Autobus, ObjetoVentana, ..., y General*/

    Coord2D Temp = CAct, Temp2, Temp3;
    int Final, Sent, n, TileAct, TileProx, VelMov, Altura;
    mask *Mand;
    Hab *HTemp;
    Actor *Bucle;

    if (VerTipo() >= MESACAPI)
        return TODOOK;

    if (VerTrans() <= 0)
       return SALEPANTALLA;
   if (VerEst() == MUERTE)
      return TODOOK;
   if (VerTipo() == EXPLPOLVO && FinAnim())
      return SALEPANTALLA;

    if (CiclosEspera > 0) return 0; // Los sprites que están "esperando" tampoco se mueven

    if (ReadMessage() == DESPLAZA) {
       Message = 0;
       if (VerX(true) < 0 || VerX() > ANCHOPANTALLA)
          return SALEPANTALLA;
       }

    if (!VerVelX() && !VerVelY() && !VerGrav()) return 0; // Si no se tiene que mover... vuelvete...

    Altura = 0;

/*
 La variable Senti sirve para que cuando estamos en el aire, al caer, si nos metemos en una plataforma,
 avance automáticamente en el sentido que iba (giremos o no) hasta que se salga de la plataforma.
 Representa el Sentido en el que se iba. Cuando sube es -1, para que no funcione lo de avanzar automáticamente:
 si no, al chocar con una plataforma al subir, la pasa "mágicamente", en lugar de golpearse y caer.
*/

    if (Estado != CAYENDO) Senti = Sentido;
    if (Estado == SUBIENDO) Senti = -1;

    if (VerGrav() && VerVelY() < 16 && (VerTipo() != CAPITAN || (VerEst() != VOLANDO && VerEst() != DESPEGANDO)))
       IncVelY(.5);
    if (VerTipo() == BOLITA)
       IncVelY (.15);
    if ((VerTipo() == CAPITAN) && (Estado == VOLANDO || Estado == DESPEGANDO))
       IncVelY ((double) ((1.7 + Config->VerDif()) / 10)); // Gravedad al volar
    if (VerVelY() > 8 && VerTipo() == OBJVENTANA)
       SetVelY(8);

    if (VerEst() == PRESALTO && (FinAnim() || ++CiclosQuieto > 30)) {
       SetVelY(-8);      // SALTO <----------------------------------------
//       if (VerTipo() == SALTARIN) IncVelY (-1);
       if (VerTipo() == CAPITAN) IncVelY (-1);
       SetEst(SUBIENDO);
       ClrFotog();
    }

    if (VerEst() == TRANSFOR || (VerTipo() == MARIANO && VerEst() == TRANSFOR + 1))
       SetVelY (0);

// Comprobar si sale de la pantalla
    if ((VerTipo() <= CAPITAN && VerX() + VerVelX() < 0 && VerVelX() < 0) || // Hace falta comprobar el "sentido" de la velocidad pq los anchos del Capitán y Mariano son distintos
        (VerTipo() > CAPITAN && VerX(true) + VerVelX() < 0)) {
      if (VerTipo() > CAPITAN)
        return SALEPANTALLA;
      Temp = CAct;
       Temp.X -= 1;
       if (MAct->ExisteHab(Temp)) { // Existe la nueva habitación?
          if (Tipo <= CAPITAN) {
             Temp = CAct;
             Temp.X--; // Ahora hay que comprobar que no choque con nada en la nueva habitación al pasar
             HTemp = MAct->IrHab(Temp);
             Temp.X = TILESANCHO - 1;
             for (Temp.Y = (int)(VerY() / TAMTILE); Temp.Y < (int)(VerY(true) / TAMTILE); Temp.Y ++)
                 if (HTemp->CContenido(Temp)) {
                    VelX = 0;
                    if (Estado == ANDANDO || Estado == ARRANCANDO) {
                       SetEst(PARADO);
                       AcabaAnim();
                       }
                    }
             if (VerVelX()) {
                SetX (TILESANCHO * TAMTILE - VerLongX());// + (VerLongX() / 2));
                CAct.X--;
                }
             }
          if (VerVelX()) return SALEPANTALLA;
          }
       else {
          SetVelX(0);
          if (Estado == ANDANDO || Estado == ARRANCANDO) {
             SetEst(PARADO);
             AcabaAnim();
             }
          }
    }

    if ((VerTipo() <= CAPITAN && VerX(true) + VerVelX() > TILESANCHO * TAMTILE && VerVelX() > 0) || // Hace falta comprobar el "sentido" de la velocidad pq los anchos del Capitán y Mariano son distintos
        (VerTipo() > CAPITAN && VerX() + VerVelX() > TILESANCHO * TAMTILE)) {
      if (VerTipo() > CAPITAN)
        return SALEPANTALLA;
      Temp = CAct;
       Temp.X += 1;
       if (MAct->ExisteHab(Temp)) {
          if (Tipo <= CAPITAN) {
             Temp = CAct;
             Temp.X++;
             HTemp = MAct->IrHab(Temp);
             Temp.X = 0;
             for (Temp.Y = (int)(VerY() / TAMTILE);(int) (Temp.Y < VerY(true) / TAMTILE); Temp.Y ++)
                 if (HTemp->CContenido(Temp)) {
                    VelX = 0;
                    if (Estado == ANDANDO || Estado == ARRANCANDO) {
                       SetEst(PARADO);
                       AcabaAnim();
                       }
                    }
             if (VerVelX()) {
                SetX (0);//-(VerLongX() / 2));
                CAct.X++;
                }
             }
          if (VerVelX()) return SALEPANTALLA;
          }
        else {
          SetVelX(0);
          if (Estado == ANDANDO || Estado == ARRANCANDO) {
             SetEst(PARADO);
             AcabaAnim();
             }
          }
    }

    if ((VerTipo() <= CAPITAN && (VerY() + VerVelY() + 12 + (12 * (Tipo == CAPITAN))) < 0 && VerVelY() < 0) ||
        (VerTipo() > CAPITAN && VerY(true) + VerVelY() < 0)) {
       Temp = CAct;
       Temp.Y -= 1;
       if (MAct->ExisteHab(Temp)) { // Existe la nueva habitación?
          if (Tipo <= CAPITAN) {
             Temp = CAct;
             Temp.Y--; // Ahora hay que comprobar que no choque con nada en la nueva habitación al pasar
             HTemp = MAct->IrHab(Temp);
             Temp.Y = TILESALTO - 1;
             for (Temp.X = (VerX() / TAMTILE); Temp.X < (VerX(true) / TAMTILE); Temp.X ++)
                 if (HTemp->CContenido(Temp)) VelY = 0;
             if (VerVelY()) {
                SetY (TILESALTO * TAMTILE - VerLongY());
                if (Estado == VOLANDO || Estado == DESPEGANDO || Estado == SUBIENDO)
                   IncY(-10);
                CAct.Y--;
                }
             }
          if (VerVelY()) return SALEPANTALLA;
          }
       else {
          if (VerTipo() <= CAPITAN) {
             if (Estado == VOLANDO || Estado == DESPEGANDO)
                SetVelY(-VerVelY());
              else
                SetVelY(0);
                }
          }
    }

    if ((VerTipo() <= CAPITAN && (VerY(true) + VerVelY()) >= TILESALTO * TAMTILE && VerVelY() > 0) ||
        (VerTipo() > CAPITAN && VerY() + VerVelY() >= TILESANCHO * TAMTILE)) {
        if (VerTipo() > CAPITAN)
           return SALEPANTALLA;
       Temp = CAct;
       Temp.Y += 1;
       if (MAct->ExisteHab(Temp)) {
          if (Tipo <= CAPITAN)
             if (VerVelY()) {
                SetY (-20);
                if (Estado == VOLANDO || Estado == DESPEGANDO || Estado == CAYENDO || Estado == SUBIENDO)
                   IncY(-30);
                CAct.Y++;
                }
          if (VerVelY()) return SALEPANTALLA;
             else {
               SetVelY(0);
               SetEst (POSTSALTO);
               ClrFotog();
               }
          }
    }

// Comprobar si choca con algn tile

   if (VerTipo() == OBJVENTANA || VerTipo() == CEMENTO) { // Si un disparo de una ventana cae al suelo...
      Temp.X = VerX() / TAMTILE;
      Temp.Y = VerY(true) / TAMTILE;
      if (HAct->CContenido(Temp, true)) {
            Explota();
            return SALEPANTALLA;
      }
      IncX((int)VerVelX());
      IncY((int)VerVelY());
      return TODOOK;
      }

   if (Tipo <= CAPITAN)
      Mand = M->Mask(Tipo, Sentido, PARADO, 0, 0);
     else
      Mand = Mascara;

   if (VerVelX() &&               // ******************* DESPLAZAMIENTO HORIZONTAL <----------------------------------
      (Frames == KeyFrame || KeyFrame == 1 || !FotoMax ||
       (Estado != ANDANDO && Estado != ARRANCANDO && Estado != GIRAQUIETA && Estado != ANDASIN))) {

      Final = VerVelX() > 0;
      Sent = Final * 2 - 1;
      VelMov = abs (VerX() - VerXConVel());
      for (n = 0; n < VelMov; n++) {

          for (Temp.X = (VerX (false) + Sent) / TAMTILE + 2 * (Sent > 0) ; Colisionable && Temp.X <= (VerX (true) + Sent) / TAMTILE - 2 * (Sent < 0); Temp.X++)

          for (Temp.Y = VerY (false) / TAMTILE + (Estado == VOLANDO || Estado == DESPEGANDO); Temp.Y <= VerY (true) / TAMTILE; Temp.Y++) {

              TileAct = HAct->CContenido(Temp);

              if (TileAct &&
                  ColisionPP (M->Mask(TILE,0,0,TileAct), Temp.X * TAMTILE - Sent, Temp.Y * TAMTILE, Mand))
                 Frena (VelX);
          }

               Bucle = Inicial;  // Ahora vamos a ver si encuentra un "objeto plataforma" al avanzar que haya que "escalar"
               if (VerTipo() <= CAPITAN) do { // Solo el jugador puede subir a las plataformas

                   if (Temp.Y >= VerY(true) / TAMTILE - 1 &&
                       Bucle!= this && Bucle->VerEquipo() == PLATAFORMA && VerVelY() > 0 &&       // Si caemos y este objeto es una plataforma
                      !ColisionPP (Bucle->VerMascara(), Bucle->VerX() - (int) Bucle->VerVelX(), Bucle->VerY(), Mand) &&
                       ColisionPP (Bucle->VerMascara(), Bucle->VerX() - (int) Bucle->VerVelX() - Sent, Bucle->VerY(),Mand))
                       {
                        if (!ColisionPP (Bucle->VerMascara(), Bucle->VerX() - (int) Bucle->VerVelX() - Sent, Bucle->VerY() + MAXCUESTA,Mand))
                           while (ColisionPP (Bucle->VerMascara(), Bucle->VerX() - (int) Bucle->VerVelX() - Sent, Bucle->VerY(),Mand))
                                 IncY (-1);
                       }

                   if (Bucle->VerEquipo() == OBSTACULO && Colision (Bucle, false) && (
                      ((Bucle->VerX() > VerX() && VerSent() == 0) || (Bucle->VerX() < VerX() && VerSent() == 1)))) {//, Bucle->VerX() - (int)VerVelX(), Bucle->VerY())) {
                      if (Estado == ANDANDO || Estado == ARRANCANDO) {
                         SetEst(PARADO);
                         AcabaAnim();
                         }
                      SetVelX(0);
//                      while (Colision (Bucle, false))
//                            IncX (-2);
                      }

                   Bucle = Bucle->VerSiguiente();
               } while (Bucle);

          if (VerVelX())
             IncX(Sent);
           else {
             if (Tipo >= CURRO1 && Tipo <= CURRO1 + MAXCURRO) {
                SetVelX(0);
                SetEst (GIRANDO);
                ClrFotog();
             }
             if (VerTipo() == DMORCILLA)
               return SALEPANTALLA;
             if (VerTipo() == CAPITAN &&
                (Estado == ANDANDO || Estado == ARRANCANDO)) {
                SetEst(PARADO);
                AcabaAnim();
                }
             }
      }
   }

   if  (VerVelY() > 0 && Gravedad && VerTipo() != OVNIJ && VerTipo() != JERING && VerTipo() != CEMENTO && VerTipo() != ROBOTIN) // Si está cayendo,
       Altura = VerLongY() - 2; // para pararlo fíjate únicamente en los ocho últimos píxeles

   if (VerVelY()) {               // ******************* DESPLAZAMIENTO VERTICAL <----------------------------------
      Final = VerVelY() > 0;
      Sent = Final * 2 - 1;
      //VelMov = abs ( (int) VerVelY() ) + (VerVelY() != (int) VerVelY());
      VelMov = abs (VerY() - VerYConVel());
      for (n = 0; n < VelMov; n++) {

          Temp.Y = (VerY (Final)) / TAMTILE;
          Temp2.Y = (VerY (Final) + Sent) / TAMTILE;
          Temp3.Y = Temp.Y + 1;

          if (Sent < 0 && Tipo == CAPITAN && (Estado == VOLANDO || Estado == DESPEGANDO || Estado == SUBIENDO)) {
             Temp.Y++;
             Temp2.Y++;
             }

          for (Temp.X = VerX (false) / TAMTILE; Colisionable && Temp.X <= VerX (true) / TAMTILE; Temp.X++) {

              Temp2.X = Temp.X;
              Temp3.X = Temp.X;

              TileAct = HAct->CContenido(Temp, Sent < 0);
              TileProx = HAct->CContenido (Temp2, Sent < 0);

              if (Tipo == CAPITAN && Sent < 0 && // Esto sirve para que si nos convertimos a Capitán pegados a una viga, no choque al saltar (por la diferencia de tamaño entre Mariano y el Capitan)
                  ColisionPP (M->Mask(TILE,0,0,TileProx), Temp2.X * TAMTILE, Temp2.Y * TAMTILE - Sent, Mand) &&
                  !ColisionPP (M->Mask(TILE,0,0,TileProx), Temp2.X * TAMTILE - 2 * ((VerSent() > 0) * 2 - 1), Temp2.Y * TAMTILE - Sent, Mand))
                      IncX (2 * ((VerSent() > 0) * 2 - 1));

              if (!ColisionPP (M->Mask(TILE,0,0,TileAct), Temp.X * TAMTILE, Temp.Y * TAMTILE, Mand, Altura) &&
                  (ColisionPP (M->Mask(TILE,0,0,TileProx), Temp2.X * TAMTILE, Temp2.Y * TAMTILE - Sent, Mand, Altura)  ||
                  ColisionPP (M->Mask(TILE,0,0,HAct->CContenido(Temp3, Sent < 0)), Temp3.X * TAMTILE, Temp3.Y * TAMTILE - Sent, Mand))) // Esta última condición sirve para ver si hay colisión con los píxeles 17 a 32 del sprite, pq si no la "chepa" de Mariano no colisiona con las vigas
                  if (VerTipo() == BALON && VerVelY() >= 1) {
                     SetVelY (-VerVelY() / 1.5);
                     IncVelX(.5);  // Frénate
                     SFx->PlayFX (DF_SBALON);
                     }
                   else
                     Frena (VelY);

// Esto de ahora "empuja" al personaje en el Sentido en el que iba si se mete de lleno en un tile al caer. De esta manera, si se cae
// por el borde de una viga, p.ej., el personaje se desplaza hasta que no cause ninguna colisión pixel perfect con los tiles...
// Salvo en las pantallas del cohete y tal (Num >= 19 y Num <= 23), donde se tiene que caer al suelo

              if (VerTipo() <= CAPITAN && VerEst() == CAYENDO && Senti >= 0)
                 if (ColisionPP (M->Mask(TILE,0,0,TileAct), Temp.X * TAMTILE, Temp.Y * TAMTILE) && (HAct->VerNum() < 19 || HAct->VerNum() > 23)) {
                       if (Senti == 0) IncX(2);
                       if (Senti == 1) IncX(-2);
                       }

// Esto de ahora sirve por si nos metemos de lleno en un tile al cambiar de pantalla

              if (Sent > 0 && !VerVelX() && !VerVelY() &&
                  (VerTipo() <= CAPITAN || VerTipo() == MARIANOMUERTO)&&
                  VerX(false) < 0 || VerX(true) > ANCHOPANTALLA) {
                 if (!(VerY(true) % TAMTILE))
                    Temp.Y--;
                 TileAct = HAct->CContenido (Temp, true);
                 while (TileAct && ColisionPP (M->Mask(TILE,0,0,TileAct), Temp.X * TAMTILE, Temp.Y * TAMTILE, Mand, Altura))
                       IncY (-1);
                 if (!(VerY(true) % TAMTILE))
                    Temp.Y++;
                 }

          }

          if (Sent > 0 && Temp.Y >= VerY(true) / TAMTILE - 1 && VerTipo() <= CAPITAN) { // Solo el jugador puede subir a las plataformas
               Bucle = Inicial;  // Ahora vamos a ver si encuentra un "objeto plataforma" en su caída
               do {

                   if (Bucle != this && Bucle->VerTipo() == SACOS && Estado == GIRANDO &&
                       !ColisionPP (Bucle->VerMascara(), Bucle->VerX() - (int) Bucle->VerVelX(), Bucle->VerY() + MAXCUESTA))
                           while (ColisionPP (Bucle->VerMascara(), Bucle->VerX() - (int) Bucle->VerVelX(), Bucle->VerY() - 2 * Sent, M->Mask(Tipo, !Sentido, Estado, 0), Altura)) {
                                IncY (-1);
                                Frena (VelY);
                                }

                   if (Bucle != this && Bucle->VerTipo() == CVIDRIO && Estado == GIRANDO)
                      if (ColisionPP (Bucle->VerMascara(), Bucle->VerX(), Bucle->VerY(), M->Mask(Tipo, - (Sentido + 1 - 2), PARADO, 0)) &&
                          !ColisionPP (Bucle->VerMascara(), Bucle->VerX() - 16 * (-2 * Sentido + 1), Bucle->VerY(), M->Mask(Tipo, - (Sentido + 1 - 2), PARADO, 0)))
                                while (ColisionPP (Bucle->VerMascara(), Bucle->VerX(), Bucle->VerY(), M->Mask(Tipo, - (Sentido + 1 - 2), PARADO, 0))) {
                                      IncX (Sentido * (-2) + 1);
                                      Frena (VelY);
                                      }

                   if (Bucle!= this && Bucle->VerEquipo() == PLATAFORMA && VerVelY() > 0 &&       // Si caemos y este objeto es una plataforma
                      !ColisionPP (Bucle->VerMascara(), Bucle->VerX() - (int) Bucle->VerVelX(), Bucle->VerY(), Mand, Altura) &&
                       ColisionPP (Bucle->VerMascara(), Bucle->VerX() - (int) Bucle->VerVelX(), Bucle->VerY() - Sent,Mand, Altura))
                       {
                           Frena (VelY);
                           IncX((int)Bucle->VerVelX());
                       }
                   Bucle = Bucle->VerSiguiente();
               } while (Bucle);
          }

          if (VerVelY())
             IncY(Sent);
           else {
             if (VerTipo() <= CAPITAN &&
                (Estado == SUBIENDO || Estado == VOLANDO || Estado == DESPEGANDO)) {
                ClrFotog();
                SetEst (POSTSALTO);
                VJ.QueHase = VJ.QueHasia;
                CiclosQuieto = 0;
                Frena (VelX);
                }
             }
      }
   }

// Cambios de Estado ***********************************************************************

    if (VerTipo() == ABEJAS || VerTipo() == COCO || VerTipo() == ALMA ||
        VerTipo() == BALON || VerTipo() == ELEFANT || VerTipo() == OVNIJ ||
        VerTipo() == ALIEN || VerTipo() == JERING || VerTipo() == CEMENTO ||
        VerTipo() == ASOLDIER || VerTipo() == HUMOJ || VerTipo() == LASERJ ||
        VerTipo() == BURGUER || VerTipo() == LUZVERDE || VerTipo() == ANUNCIOWIN ||
        VerTipo() == ROBOTIN || VerTipo() == COLUMNON || VerTipo() == ASPIRADOR ||
        VerTipo() == SHORTCIRCUIT || VerTipo() == TORRES || VerTipo() == TORREC)// || VerTipo() == RODADOR)
       return TODOOK;

    AjustaEstado();

    GeneraFX();

    return TODOOK;
}

void Actor::AjustaEstado()
{
    if (VerTipo() == CAPITAN && !VerRad())
       if (VerEst() == SUBIENDO || VerEst() == CAYENDO ||
          VerEst() == DESPEGANDO || VerEst() == VOLANDO)
               VJ.QueHase = VOLAR;

    if (VerTipo() == MARIANOMUERTO && VerEst() == POSTSALTO)
       return; // Esto no haría falta, pero a veces pasa que vuelve a "caer", y al entrar en estado "cayendo", se queda "atascado" sin terminar de "reconocer" la muerte de Mariano

    if (Tipo == MARIANO && Estado == PARADO && !Animacion && ++CiclosQuieto > 3 * TEMPORIZADOR) {
       Animacion = 1 + ((int) (rand() % AnimMariano));
       Actualiza();
       ClrFotog();
       }

    if (Tipo == CAPITAN && Estado == PARADO && !Animacion && ++CiclosQuieto > 3 * TEMPORIZADOR) {
       Animacion = 1 + ((int) (rand() % AnimCapitan));
       Actualiza();
       ClrFotog();
       }

    if (Animacion && FinAnim()) {
       SetEst (PARADO);
       CiclosQuieto = 0;
       AcabaAnim();
       Animacion = 0;
       }

    if (Estado == GIRANDO && FinAnim() && Tipo <= CAPITAN) {
       Reverse();
       SetEst(PARADO);
       CiclosQuieto = 0;
       if (Tipo == CAPITAN)
          AcabaAnim();
        else
          ClrFotog();
       }

    if (Estado == CAYENDO && !VerVelY()) {
       SetEst(POSTSALTO);
       Frena (VelX);
       CiclosQuieto = 0;
       ClrFotog();
       if (VerTipo() <= CAPITAN && CiclosCayendo > 25 - 5 * Config->VerDif())
          IncPow(-CiclosCayendo / 5);
       if (VerTipo() == CAPITAN)
          VJ.QueHase = VJ.QueHasia;
       if (VerTipo() == MARIANOMUERTO) {
          SetGrav (false);
          IncY(1);
          }
       }

    if (/*Tipo <= CAPITAN && */VerVelY() > 0 && // Si eres Capitán o Mariano, y vas hacia abajo y...
       ((Estado != DESPEGANDO && Estado != VOLANDO && ++CiclosCayendo > 5) || // o no estás volando y llevas 5 ciclos cayendo...
       ((Estado == DESPEGANDO || Estado == VOLANDO) && ++CiclosCayendo > 10) || Estado == SUBIENDO) && // ... o estás volando y llevas mas de 10 ciclos cayendo, o estabas subiendo...
        Estado != TRANSFOR && !(VerTipo() == MARIANO && VerEst() == TRANSFOR + 1) && // ... y no te estás transformando
        Estado != CAYENDO) { // ... y, por supuesto, no está ya "cayendo"...
             SetEst(CAYENDO);
             ClrFotog();
             }

    if (/*Tipo <= CAPITAN && */!VerVelX() && !VerVelY() && Estado == ANDANDO) { // Párate cuando termines de andar
       SetEst(PARADO);
       CiclosQuieto = 0;
       ClrFotog();
       }

    if (Tipo != CAMARERO && Estado == POSTSALTO && FinAnim() && (++CiclosQuieto > 3 || Tipo != MARIANO)) {
       if (VerTipo() == SALTARIN)
          SetEst (PARADO);
        else
          SetEst(LEVANTANDO);
       CiclosQuieto = 0;
       ClrFotog();
       }

    if (Tipo != CAMARERO && Estado == AGACHADO && ++CiclosQuieto > 50) {
       SetEst(LEVANTANDO);
       ClrFotog();
       CiclosQuieto = 0;
       }

    if (Tipo != CAMARERO && Estado == LEVANTANDO && FinAnim())
       if (Tipo <= CAPITAN) {
          SetEst(PARADO);
          AcabaAnim();
          CiclosQuieto = 0;
          }

    if ((Estado == TRANSFOR || (Estado == TRANSFOR + 1 && Tipo == MARIANO)) && FinAnim()) {
       if (Tipo == CAPITAN) {
          Tipo = MARIANO;
          SetParpadeo (2 * TEMPORIZADOR);
          SetHalo (false);
          IncX (4);
          IncY (4);
          VJ.QueHase = PUNO;
          }
        else {
          Tipo = CAPITAN;
          IncX (-4);
          IncY (-4);
          SetPow (140 - 20 * Config->VerDif());
          VJ.QueHase = SOPLAR;
          VJ.QueHasia = SOPLAR;
          }
       SetEst (PARADO);
       Frena (VelX);
       AcabaAnim();
       CiclosQuieto = 0;
       }

       if (VerEst() == SOPLO && FinAnim()) {
         SetEst (PARADO);
         AcabaAnim();
         SetVelX(0);
         CiclosQuieto = 0;
         switch (VerSent()) {
           case 0:
             IncX (-4);
             break;
           case 1:
             IncX (4);
             break;
         }
       }

       if (VerTipo() == CAPITAN && VerEst() == DISPARANDO && FinAnim()) {
         SetEst (PARADO);
         AcabaAnim();
         Frena (VelX);
         CiclosQuieto = 0;
         switch (VerSent()) {
           case 0:
             IncX (-4);
             break;
           case 1:
             IncX (4);
             break;
         }
       }

       if (VerTipo() == MARIANO && Estado == DISPARANDO && FinAnim()) {
         SetEst (PARADO);
         CiclosQuieto = 0;
       }

       if (Tipo == CAPITAN && Estado == DESPEGANDO && FinAnim()) {
          SetEst (VOLANDO);
          ClrFotog();
          SetVelX(0);
          }

    if (!VerVelY()) CiclosCayendo = 0;

    AjustaKF();
}

/**********************************************************************************************
Método ValoraColisiones

Valora las colisiones entre actores, desde "this" actor, hasta el actor "Final"

Usa, a su vez, el método INLINE Read/Send Message, que sirve para enviar / recibir un mensaje al objeto
con que se produzca "colisión"

Devuelve:
0 - TODOOK:   Si no hay colisión
COLISION  :   Si hay una colisión q destruya el objeto
**********************************************************************************************/
int Actor::ValoraColisiones (Hab *HAct)
{
    Actor *Bucle, *Jugador;
    int Mess = ReadMessage();
    if (Mess == TERMINADO)
        return Mess;
    if (Mess == COLISION) {
      Message = 0;
      if (--Energia <= 0)
          return COLISION; // Si tienes un "mensaje de colisión" pendiente... devuélvelo
      }

    Jugador = ((Actor*)(VJ.Jugador));
    Bucle =  VerSiguiente();

    if (VerTipo() <= CAPITAN && // Si eres el jugador
        HAct->VerNum()> 15 && HAct->VerNum() < 18 && VerY(true) >= (TILESALTO - 1) * TAMTILE) // Y te toca la lava...
      return COLISION; // ... estás "colisionando"

    if (VerTipo() == CAPITAN && VerPow() <= 0) // Si eres el Capi y no te queda energía (por los saltos grandes, morcillazos, etc...)
       return COLISION;  // Devuelve colision

    while (Bucle) {
          if (Colision (Bucle,true) && VerEst() != MUERTE && Bucle->VerEst() != MUERTE)
             switch (VerEquipo()) {
                    case AMIGO: // "Nuestro" equipo es AMIGO
                      if (Bucle->VerEquipo() == ENEMIGO && !Bucle->VerEspera() &&  // "El otro" equipo es ENEMIGO y ya está activo (no está esperando)
                         !((Bucle->VerTipo() == LASERJ || Bucle->VerTipo() == CEMENTO ||
                            Bucle->VerTipo() == BOLITA || Bucle->VerTipo() == METEORITO ||
                            Bucle->VerTipo() == RAYOTORRE) && VerEst() == AGACHADO)) { // Ni el láser ni el cemento te dan si estás agachado

                         if (VerTipo() <= CAPITAN &&
                             (Bucle->VerTipo() == OBJVENTANA || Bucle->VerTipo() == CUBO ||
                              Bucle->VerTipo() == CUORE || Bucle->VerTipo() == GLOBO ||
                              Bucle->VerTipo() == LASERJ || Bucle->VerTipo() == RAYOTORRE ||
                              Bucle->VerTipo () == CEMENTO || Bucle->VerTipo() == BOLITA)) {
                               SFx->PlayFX (DF_SIMALO);

                         if (VerTipo() <= CAPITAN && Bucle->VerTipo() == RAYOTORRE)
                            ((Actor*)VJ.Torrebruno)->SendMessage(RIENDO);

                            Bucle->SetPow(0);  // Hay que destruir el objeto que choca contra el jugador
                            Bucle->SendMessage (COLISION);
                            if (!VerRad())
                               IncPow (-(9 + 5 * Config->VerDif()));
                            return COLISION;
                         }

                         if (VerTipo() == MARIANO && VerEst() == DISPARANDO && VerFotog() < 4&&
                             (
                             (VerSent() == 0 && VerX(true) < Bucle->VerX(true) ||
                              VerSent() == 1 && VerX() > Bucle->VerX())
                             ))
                                return TODOOK;

                         if (VerTipo() == MARIANO && VerEst() == DISPARANDO && VerFotog() >= 4 && VerFotog() <= 6 &&
                             Bucle->VerTipo() != ABEJAS &&
                             (
                             (VerSent() == 0 && VerX(true) < Bucle->VerX(true) ||
                              VerSent() == 1 && VerX() > Bucle->VerX())
                             )) {
                              SFx->PlayFX (DF_SGOLPPUNO);
                              Bucle->SendMessage (COLISION);
                              return TODOOK;
                              }
                         if (VerTipo() == MARIANO) return COLISION; // Colisión Mariano Vs Enemigo : Siempre muere Mariano
                         if (VerTipo() == CAPITAN) { // Colisión Capitán Vs Enemigo:
                            if (!VerRad()) {         // Si no está radiactivo
                               if (Bucle->VerTipo() == BALON)
                                  IncPow (-Config->VerDif());             // Resta energía
                               SFx->PlayFX (DF_SIMALO);
                               }
                            return COLISION;}
                         if (VerTipo() == DMORCILLA && Bucle->VerTipo() != ABEJAS && Bucle->VerTipo() != HUMOJ) {
                            Bucle->SendMessage (COLISION);
                            SFx->PlayFX (DF_SIMALO);
                            if (Bucle->VerTipo() != VENTANA)
                               if (Jugador->VerSent() == 0)
                                 Bucle->IncX(6);
                               else
                                 Bucle->IncX(-6);
                            return COLISION;
                            }
                       }
                       else { // "El otro" equipo NO es ENEMIGO
                         if (VerTipo() <= CAPITAN && Bucle->VerTipo() == MORCILLA) {
                            Bucle->SendMessage (COLISION);
                            Bucle->SetPow(0);
                            VJ.Morcillas++;
                            }

                         if (VerTipo() == DMORCILLA && Bucle->VerEquipo() == PLATAFORMA)
                            return COLISION;

                         if (VerTipo() == MORCILLA && Bucle->VerTipo() <= CAPITAN) {
                            VJ.Morcillas++;
                            return COLISION;
                            }
                         if (VerTipo() == HALOMORCI && Bucle->VerTipo() <= CAPITAN) {
                            return COLISION;
                            }
                         if (VerTipo() == MARIANO && VerEst() == DISPARANDO && VerFotog() == 5 && Bucle->VerTipo() == ASTRONAUTA2)
                            Bucle->SendMessage (COLISION);
                         if (Bucle->VerTipo() == MARIANO && Bucle->VerEst() == DISPARANDO &&
                             Bucle->VerFotog() == 5 && VerTipo() == ASTRONAUTA2)
                             return COLISION;

                         if ((VerTipo() == DMORCILLA && Bucle->VerTipo() == ASTRONAUTA2) ||
                             (Bucle->VerTipo() == DMORCILLA && VerTipo() == ASTRONAUTA2)) {
                             Bucle->SendMessage (COLISION);
                             return COLISION;
                             }

                         if (VerTipo() == DMORCILLA && Bucle->VerTipo() == PC) {
                            Bucle->SendMessage (GIRAR);
                            return COLISION;
                            }

                         if (VerTipo() == MARIANO && (Bucle->VerTipo() == CAMARERO || Bucle->VerTipo() == CAMARSIN)&&
                             VerEst() == DISPARANDO && VerFotog() == 5 &&
                             (
                             (VerSent() == 0 && Bucle->VerSent() == 0 && VerX(true) - 40 <= Bucle->VerX()) ||
                             (VerSent() == 1 && Bucle->VerSent() == 1 && VerX() + 40 >= Bucle->VerX(true)) || // Si estï¿½ enfrentados, hay que evitar la bandeja
                             (VerSent() == 0 && Bucle->VerSent() == 1 && VerX(true) - 50 <= Bucle->VerX() && VerX(true) - 30 >= Bucle->VerX()) ||
                             (VerSent() == 1 && Bucle->VerSent() == 0 && VerX() + 50 >= Bucle->VerX(true) && VerX() + 30 <= Bucle->VerX(true))
                             )) {
                             Bucle->IncPow (-1);
                             Bucle->SendMessage (COLISION);
                             }
                         if (Bucle->VerTipo() == MARIANO && (VerTipo() == CAMARERO || VerTipo() == CAMARSIN)&&
                             Bucle->VerEst() == DISPARANDO && VerFotog() == 5 &&
                             (
                             (Bucle->VerSent() == 0 && VerSent() == 0 && Bucle->VerX(true) - 40 <= VerX()) ||
                             (Bucle->VerSent() == 1 && VerSent() == 1 && Bucle->VerX() + 40 >= VerX(true)) || // Si estï¿½ enfrentados, hay que evitar la bandeja
                             (Bucle->VerSent() == 0 && VerSent() == 1 && Bucle->VerX(true) - 50 <= VerX() && Bucle->VerX(true) - 30 >= VerX()) ||
                             (Bucle->VerSent() == 1 && VerSent() == 0 && Bucle->VerX() + 50 >= VerX(true) && Bucle->VerX() + 30 <= VerX(true))
                             )) {
                             IncPow (-1);
                             if (VerPow() <= 0)
                                return COLISION;
                             }
                         if ((VerTipo() <= CAPITAN && Bucle->VerTipo() == FINFASE) ||
                             (VerTipo() == FINFASE && Bucle->VerTipo() <= CAPITAN)) {
                                return TERMINADO;
                                }
                         if (VerTipo() <= CAPITAN && Bucle->VerTipo() == PC)
                            Bucle->SendMessage (GIRAR);
                      }
                      break;
                    case ENEMIGO: // "Nuestro" equipo es ENEMIGO
                      if (!CiclosEspera && Bucle->VerEquipo() == AMIGO) {// "El otro" equipo es AMIGO
                         if (Bucle->VerTipo() <= CAPITAN &&  // Si es el jugador
                         !((VerTipo() == CEMENTO || VerTipo() == LASERJ || VerTipo() == BOLITA ||
                            VerTipo() == METEORITO || VerTipo() == RAYOTORRE) && VerEst() == AGACHADO)){ // Ni el láser ni el cemento te dan si estás agachado
                               if (!Bucle->VerRad()) { // Si no está radiactivo
                                   if (VerTipo() == BALON)
                                       Bucle->IncPow(-1); // Restale energía
                                   SFx->PlayFX (DF_SIMALO);
                                   Bucle->SendMessage (COLISION);
                                   }

                         if (Bucle->VerTipo() <= CAPITAN && VerTipo() == RAYOTORRE)
                            ((Actor*)VJ.Torrebruno)->SendMessage(RIENDO);

                            Bucle->SendMessage (COLISION);

                         if (Bucle->VerTipo() == MARIANO && Bucle->VerEst() == DISPARANDO && Bucle->VerFotog() < 4 &&
                             (
                             (Bucle->VerSent() == 0 && VerX(true) < VerX(true) ||
                              Bucle->VerSent() == 1 && VerX() > VerX())
                             ))
                                 return TODOOK;

                         if (Bucle->VerTipo() == MARIANO && Bucle->VerEst() == DISPARANDO && Bucle->VerFotog() >= 4 && Bucle->VerFotog() <= 6 &&
                             VerTipo() != ABEJAS &&
                             (
                             (Bucle->VerSent() == 0 && VerX(true) < VerX(true) ||
                              Bucle->VerSent() == 1 && VerX() > VerX())
                             )) {
                                 SFx->PlayFX (DF_SGOLPPUNO);
                                 return COLISION;
                              }

                            if (VerTipo() == OBJVENTANA || VerTipo() == CUBO || VerTipo() == CUORE || VerTipo() == GLOBO ||
                                (VerTipo() == LASERJ && Bucle->VerEst() != AGACHADO) || VerTipo() == CEMENTO || VerTipo() == BOLITA) {
                               if (!Bucle->VerRad()) {// Si no está radiactivo
                                   SFx->PlayFX (DF_SIMALO);
                                   Bucle->IncPow (-(9 + 5 * Config->VerDif()));
                                   Bucle->SendMessage (COLISION);
                                   }
                               return COLISION;
                               }
                         }
                         if (Bucle->VerTipo() == DMORCILLA && VerTipo() != ABEJAS && VerTipo() != HUMOJ) {
                           Bucle->SendMessage(COLISION);
                           SFx->PlayFX (DF_SIMALO);
                           if (--Energia <= 0)
                              return COLISION;
                           if (VerTipo() != VENTANA)
                             if (Jugador->VerSent() == 0)
                                IncX(6);
                              else
                                IncX(-6);
                           }
                         }
                         if (VerTipo() == BALON)
                            if (Bucle->VerTipo() == NINO2 || Bucle->VerTipo() == NINO3) {
                               Bucle->SetEst (COGEBAL);
                               Bucle->ClrFotog();
                               return COLISION;
                               }
                         break;
                    case NEUTRO:
                         if (VerTipo() == BALON)
                            if (Bucle->VerTipo() == NINO2 || Bucle->VerTipo() == NINO3) {
                               Bucle->SetEst (COGEBAL);
                               Bucle->ClrFotog();
                               return COLISION;
                               }
                         if (VerTipo() == NINO2 || VerTipo() == NINO3)
                            if (Bucle->VerTipo() == BALON) {
                              Bucle->SendMessage (COLISION);
                              Bucle->SetPow(0);
                              SetEst(COGEBAL);
                              ClrFotog();
                              }
                         if (VerTipo() == PC && Bucle->VerTipo() <= CAPITAN)
                            SendMessage (GIRAR);

                         if (VerTipo() == PC && Bucle->VerTipo() == DMORCILLA){
                            SendMessage (GIRAR);
                            Bucle->SendMessage (COLISION);
                            }
                         break;
                    case PLATAFORMA:
                         if (Bucle->VerTipo () == DMORCILLA)
                            Bucle->SendMessage (COLISION);
                         break;
             }
          Bucle = Bucle->VerSiguiente();
    }
    return TODOOK;
}

/**********************************************************************************************
Mï¿½odo SetGrav

Recibe:
bool Grav: Nuevo valor para "Gravedad"

Ajusta el valor "Gravedad": Si afecta o no a este actor la Gravedad
**********************************************************************************************/
void Actor::SetGrav (bool Grav)
{
     Gravedad = Grav;
}

/**********************************************************************************************
Mï¿½odo VerGrav

Devuelve el valor "Gravedad": Si afecta o no la gravedad a este actor
**********************************************************************************************/

const bool Actor::VerGrav ()
{
      return Gravedad;
}

/**********************************************************************************************
Mï¿½odo Dibuja_Sombra

Dibuja la sombra de este actor en el fondo
**********************************************************************************************/

void Actor::Dibuja_Sombra ()
{
  if (VerSombra())
    H->Dibuja_Sombra ((int) x, (int) y, M->Sprite(Tipo,Sentido,Estado,Fotograma,Animacion));
}

/**********************************************************************************************
Mï¿½odo Borra_Sombra

Borra la sombra de este actor en el fondo
**********************************************************************************************/

void Actor::Borra_Sombra ()
{
  if (VerSombra())
    H->Borra_Sombra ((int) x , (int) y, VerLongX(), VerLongY());
}

void Actor::GeneraFX()
{
     if (VerTipo() <= CAPITAN) {

        if (VerEst() == POSTSALTO) {
           if (VerFotog() == 0 && !Suena[DF_SPOSTSALTO]) {
              SFx->PlayFX (DF_SPOSTSALTO);
              Suena[DF_SPOSTSALTO] = true;
              }
           if (VerFotog() > 0)
              Suena[DF_SPOSTSALTO] = false;
           }

        if (VerTipo() == MARIANO && (VerEst() == TRANSFOR || VerEst() == TRANSFOR + 1)) {
           if (VerFotog() == 0 && !Suena[DF_SCONV]) {
              SFx->PlayFX (DF_SCONV);
              Suena[DF_SCONV] = true;
              }
           if (VerFotog() > 0)
              Suena[DF_SCONV] = false;
           }

        if (VerTipo() == CAPITAN && VerEst() == TRANSFOR) {
           if (VerFotog() == 0 && !Suena [DF_SDECONV]) {
              SFx->PlayFX (DF_SDECONV);
              Suena[DF_SDECONV] = true;
              }
           if (VerFotog() > 0)
              Suena[DF_SDECONV] = false;
           }

        if (VerTipo() == CAPITAN && VerEst() == SOPLO) {
           if (VerFotog() == 0 && !Suena [DF_SSOPLA]) {
              SFx->PlayFX (DF_SSOPLA);
              Suena[DF_SSOPLA] = true;
              }
           if (VerFotog() > 0)
              Suena[DF_SSOPLA] = false;
           }

        if (VerTipo() == MARIANO && VerEst() == DISPARANDO) {
           if (VerFotog() == 0 && !Suena[DF_SGOLPE]) {
              SFx->PlayFX (DF_SGOLPE);
              Suena[DF_SGOLPE] = true;
              }
           if (VerFotog() > 0)
              Suena[DF_SGOLPE] = false;
           }

        if (VerTipo() == CAPITAN && VerEst() == DISPARANDO) {
           if (VerFotog() == 2 && !Suena[DF_SDISPMORC]) {
              SFx->PlayFX (DF_SDISPMORC);
              Suena[DF_SDISPMORC] = true;
              }
           if (VerFotog() > 2)
              Suena[DF_SDISPMORC] = false;
           }

        if (VerTipo() == MARIANO && Animacion)
           switch (Animacion) {
                  case 1:
                       if (!VerFotog() && !Suena[DF_SRASCA]) {
                          SFx->PlayFX (DF_SRASCA);
                          Suena[DF_SRASCA] = true;
                          }
                       if (VerFotog())
                          Suena[DF_SRASCA] = false;
                       break;
                  case 2:
                       if (!VerFotog() && !Suena[DF_SPIS]) {
                          SFx->PlayFX (DF_SPIS);
                          Suena[DF_SPIS] = true;
                          }
                       if (VerFotog())
                          Suena[DF_SPIS] = false;
                       break;
                  case 3:
                       if (VerFotog() == 22 && !Suena[DF_SCIG]) {
                          SFx->PlayFX (DF_SCIG);
                          Suena[DF_SCIG] = true;
                          }
                       if (VerFotog() > 22)
                          Suena[DF_SRASCA] = false;
                       break;
                  case 5:
                       if (!VerFotog() && !Suena [DF_SSILBA]) {
                          SFx->PlayFX (DF_SSILBA);
                          Suena[DF_SSILBA] = true;
                          }
                       if (VerFotog())
                          Suena[DF_SSILBA] = false;
                       break;
                  case 6:
                       if (VerFotog() == 8 && !Suena [DF_SPEDO]) {
                          SFx->PlayFX (DF_SPEDO);
                          Suena[DF_SPEDO] = true;
                          }
                       if (VerFotog() > 8)
                          Suena[DF_SPEDO] = false;
                       break;
           }

        if (VerTipo() == CAPITAN && Animacion)
           switch (Animacion) {
                  case 3:
                       if (VerFotog() == 6 && !Suena [DF_STIN]) {
                          SFx->PlayFX (DF_STIN);
                          Suena[DF_STIN] = true;
                          }
                       if (VerFotog() > 6)
                          Suena[DF_STIN] = false;
                       break;
           }
     }

     switch (VerTipo()) {

         case CUBO:
            if (VerEst() == SUBIENDO && VerFotog() == 0 && Frames == 1)
                    SFx->PlayFX (DF_SBOING);
            break;
         case SALTARIN:
            if (VerEst() == PRESALTO && VerFotog() == 1 && Frames == 1)
                    SFx->PlayFX (DF_SBOING);
            break;
        }
}

int Actor::AjustaKF ()
{
    switch (VerTipo()) {
         case PANTALLA8B:
              SetKF(6);
              break;
         case MORCILLA:
         case HALOMORCI:
              SetKF(4);
              break;
         case ASTRONAUTA2:
         case CAMARERO:
         case CAMARSIN:
              switch (Estado) {
                     case ANDANDO:
                          SetKF (1);
                          break;
                     case DISPARANDO:
                          SetKF (3);
                          break;
                     case GIRANDO:
                     case MUERTE:
                          SetKF (2);
                          break;
                     }
              break;
         case METEORITO:
              SetKF (1);
              break;
         case VENTANA:
         case GRUA:
              SetKF (2);
              break;
         case NEGRO:
         case ASTRONAUTA3:
              switch (Estado) {
                     case ANDANDO:
                     case MUERTE:
                          SetKF (2);
                          break;
                     case GIRANDO:
                          SetKF (1);
                          break;
                     }
              break;
         case OBJVENTANA:
              switch (Sentido) {
                     case 0: // Maceta
                          SetKF(2);
                          break;
                     case 2: // Botella
                          SetKF(1);
                          break;
                     default:
                          SetKF(4);
                          break;
                     }
              break;
         case CURRO1:
         case CURRO2:
         case CURRO3:
              switch (Estado) {
                case ANDANDO:
                case CAYENDO:
                case GIRANDO:
                    SetKF (1);
                    break;
                case POSTSALTO:
                    SetKF (3);
                    break;
                case LEVANTANDO:
                case MUERTE:
                case NACIENDO:
                    SetKF (2);
                    break;
              }
              break;
         case GANGSTER:
           switch (Estado) {
                     case ANDANDO:
                     case GIRANDO:
                          SetKF (1);
                          break;
                     case MUERTE:
                          SetKF (2);
                          break;
                     }
          break;
         case CUBO:
              switch (Estado) {
                     case PRESALTO:
                     case POSTSALTO:
                          SetKF (4);
                          break;
                     case SUBIENDO:
                     case CAYENDO:
                          SetKF (2);
                          break;
                     case MUERTE:
                          SetKF(3);
                          break;
                     }
              break;
         case MUJER:
              switch (Estado) {
                     case PARADO:
                          SetKF (2);
                          SetRepeat (true);
                          break;
                     case ANDANDO:
                     case ARRANCANDO:
                     case GIRAQUIETA:
                     case GIRANDO:
                          SetKF (1);
                          break;
                     case MUERTE:
                          SetKF (2);
                          break;
                          }
              break;
         case ABEJAS:
         case LUZVERDE:
              SetKF(3);
              break;
         case MARIANOMUERTO:
         case ESCAFANDRAM:
              SetKF (1);
              if (VerEst() == POSTSALTO && VerFotog() <= 12)
                 SetKF (2);
              break;
         case ALMA:
              SetKF (2);
              break;
         case DMORCILLA:
              SetKF (1);
              break;
         case ELEFANT:
              switch (Estado) {
                     case GIRANDO:
                          SetKF(1);
                          break;
                     default:
                          SetKF(2);
                          break;
                     }
              break;
         case NINO1:
         case OVNIJ:
         case BURBUJA:
         case ASPIRADOR:
              SetKF(2);
              break;
         case BALON:
         case TORO:
              SetKF(1);
              break;
         case NINO2:
         case NINO3:
              switch (Estado) {
                     case BABEANDO:
                          SetKF (3);
                          break;
                     case COGEBAL:
                     case TIRABAL:
                          SetKF (2);
                          break;
                          }
              break;
         case COCO:
         case SHORTCIRCUIT:
         case RODADOR:
              if (Estado == MUERTE)
                 SetKF(2);
               else
                 SetKF(3);
              break;
         case CHEPPY1:
         case CHEPPY2:
         case CHEPPY3:
           switch (Estado) {
                     case ANDANDO:
                     case GIRANDO:
                          SetKF (1);
                          break;
                     case MUERTE:
                          SetKF (2);
                          break;
                     }
          break;
         case ALIEN:
              SetKF(3);
              break;
         case BURGUER:
              SetKF(2);
              break;
         case JERING:
              SetKF(4);
              break;
         case ROBOTIN:
              switch (Estado) {
                     case GIRANDO:
                          SetKF(2);
                          break;
                     default:
                          SetKF (4);
                          break;
                     }

         case ASOLDIER:
              switch (Estado) {
                     case GIRANDO:
                          SetKF(2);
                          break;
                     default:
                          SetKF (3);
                          break;
                     }
              break;

         case SALTARIN:
              SetKF (4);
              switch (Estado) {
                     case GIRANDO:
                     case MUERTE:
                          SetKF (2);
                          break;
                     }
              break;

         case MONSTRUO:
              SetKF (2);
              switch (Estado) {
                     case NACIENDO:
                          SetKF(4);
                          break;
                     case ANDANDO:
                          SetKF (1);
                          break;
                     }
              break;
        case TORRES:
        case TORREC:
            switch (Estado) {
                case ATURDIDO:
                case RIENDO:
                    SetKF(4);
                    break;
                case MUERTE:
                    SetKF (6);
                    break;
                default:
                    SetKF(2);
            }
            break;

         case CAPITAN:
         case MARIANO:
             if (!Animacion)
                switch (Estado) {
                       case LEVANTANDO:
                       case GIRANDO:
                       case ANDANDO:
                            SetKF(1);
                            break;

                       case SUBIENDO:
                       case CAYENDO:
                       case VOLANDO:
                       case DESPEGANDO:
                       case TRANSFOR:
                            SetKF(3);
                            break;

                       case SOPLO:
                            if (VerFotog() < 5)
                               SetKF (2);
                             else
                               SetKF(3);
                            break;

                       case ARRANCANDO:
                            if (Tipo == MARIANO) SetKF (3); // La segunda transformación de Mariano
                             else SetKF (1);
                            break;

                       case POSTSALTO:
                            if (Tipo == MARIANO) SetKF (2);
                               else SetKF (3);
                            break;
                       case DISPARANDO:
                            if (Tipo == MARIANO || Config->VerDif() != 0) SetKF (2);
                               else SetKF (1);
                            break;

                       default:
                            SetKF (2);
                }
              else  // Es decir, si está en una animación de espera ==> if (Animacion)
                if (Tipo == MARIANO)
                   switch (Animacion) {
                          case 1:
                          case 4:
                          case 7:
                               SetKF(3);
                               break;
                          case 2:
                          case 3:
                          case 5:
                          case 6:
                               SetKF(4);
                               break;
                   }
                 else // Es decir, si Tipo == CAPITAN
                   switch (Animacion) {
                          default:
                               SetKF(4);
                               break;
                  }
             if (VerRad())
                SetKF (VerKF() + 1);
             break;
    }
    return VerKF();
}

int Actor::VerEspera ()
{
    return CiclosEspera;
}

void Actor::Explota ()
{
     SP->CreaSistema ((int) x, (int)y, Dibujo);
     if (VerTipo() == CEMENTO)
        return;
     switch (Sentido) {
            case 0: // Maceta
            case 2: // Botella
                 SFx->PlayFX(DF_SROTO_000);
                 break;
            default:
                 SFx->PlayFX (DF_SROTO_001);
            }
}

void Actor::Muerte ()
{
     if (VerTipo() == GRUA)
        SFx->StopFX (DF_SRALENTI);
     if (VerTipo() == SHORTCIRCUIT || VerTipo() == RODADOR)
        SFx->StopFX (DF_SROBOCHAT);

     if (VerTipo() == METEORITO)
        SFx->PlayFX (DF_SPIEDRAS);
      else
        SFx->PlayFX (DF_SMMALO);
     SetEst (MUERTE);
     SetHalo (false);
     Equipo = NEUTRO;
     SetVelX (0);
     SetVelY (0);
     SetTrans (255);
     ClrFotog();
}

void Actor::TiraAbajo()
{
     SFx->PlayFX (DF_SMMALO);
     Equipo = NEUTRO;
     SetEst (CAYENDO);
     ClrFotog();
     SetVelX(0);
     SetVelY(0);
     SetKF(2);
}

void Actor::ClrCiclos ()
{
     CiclosQuieto = 0;
}

bool Actor::VerSombra()
{
  if (!Mascara->Transp &&
      VerTipo() != FAROLG && VerTipo() != FAROLP && VerTipo() != FAROLPC && VerTipo() != FAROLA &&
      VerTipo() != ONOMAT && VerTipo() != VENTANA && VerTipo () != ABEJAS && VerTipo() != CUORE)
          return true;
  return false;
}

bool Actor::VerRad()
{
     return (CiclosRad > 0);
}

void Actor::Frena (long double &Variable)
{
     if (Colisionable)
        Variable = 0;
}

void Actor::SetCapa(int Cap)
{
    Capa = Cap;
}
