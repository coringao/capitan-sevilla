/**********************************************************************
Capitan Sevilla / Captain "S" - PC Remake
Original code by Hi Score
PC Remake code by Luis I. Garcia Ventura - aka "Benway"
Contact: benway@computeremuzone.com

(c) 1988 Microdigital soft (Dinamic Software)
(c) 2009 Computeremuzone Games Studio


    Este programa es software libre: usted puede redistribuirlo y/o modificarlo
    bajo los términos de la Licencia Pública General GNU publicada
    por la Fundación para el Software Libre, ya sea la versión 3
    de la Licencia, o (a su elección) cualquier versión posterior.

    Este programa se distribuye con la esperanza de que sea útil, pero
    SIN GARANTÍA ALGUNA; ni siquiera la garantía implícita
    MERCANTIL o de APTITUD PARA UN PROPÓSITO DETERMINADO.
    Consulte los detalles de la Licencia Pública General GNU para obtener
    una información más detallada.

    Debería haber recibido una copia de la Licencia Pública General GNU
    junto a este programa.
    En caso contrario, consulte <http://www.gnu.org/licenses/>.



    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

***********************************************************************/

#include "main.h"

int main(int argc, char *argv[])
{
    int ResMenu;

    if (Creacion(argc, argv)) {
       LogWrite ("INICIALIZACIÓN ABORTADA.\nFINALIZANDO APLICACIÓN...\n");
       delete H;
       LogWrite ("APLICACIÓN FINALIZADA CORRECTAMENTE.\n");
       LogEnd();
       return 1;
       }

    do { // Mientras el menú no dé la orden de salir
         ResMenu = Menus->Run();
         Game = new Partida; // Nueva partida

         switch (ResMenu) {
                case ATTRACTMODE:
                     if (Game->Init(2)) // Algún problema en la inicialización?
                        return 1;
                     Game->Ciclo();   // Ciclo del juego
                     break;
                case 1:
                     if (Game->Init(0)) // Algún problema en la inicialización?
                        return 1;
                     Game->Ciclo();   // Ciclo del juego
                     break;
                }

         delete Game;  // Se acabó esta partida
         } while (ResMenu > 0);

    Destruccion();

    return 0;
}
END_OF_MAIN();

int Creacion (int argc, char *argv[])
{
    int Ret;

    LogInit();
    LogWrite ("Capitán Sevilla, El Remake\nComputerEmuZone Games Studio\n\nINICIANDO APLICACIÓN...\n");

    Config = new Configuracion;
    H = new Hardware;
    SMS = new Messages;
    Menus = new MenuPlayer;

    Ret = H->PreConfig(argc, argv);
    if (!Ret) Ret = H->InitGraph();
    if (!Ret) Ret = H->Init();

    if (!Ret && Config->VerIdioma() == 127)
        Ret = H->SelectLang();

    if (!Ret)
        Ret = SMS->InitMens (Config->VerIdioma());

    if (Ret) return Ret;

    textout_centre_ex (screen, font, SMS->ReadMens(1), screen->w / 2, screen->h / 2 - 4, makecol (255,255,255), 0);

    if (H->LoadDF1())
       return 1;
    M = new SprManager;

    SFx = new Sonido;

    Presen = new Presentacion;
    if (Presen->Run(true))
       return CIERRAVENT;

    H->LoadDF2();

    M->InitGraph();
    M->InitMasks();
    GMundos = new GestorDeMundos;

    SP = new SistParticulas;

    for (int i = 0; i < FramesCirculo; i++) {
        FuncionCoseno[i] = cos((10 * (i+1) * 2 * PI) / FramesCirculo);
        FuncionSeno[i] = sin((10 * (i+1) * 2 * PI) / FramesCirculo);
        }

    LogWrite ("APLICACIÓN INICIADA CORRECTAMENTE.\n\n");
    return 0;
}

void Destruccion ()
{
    LogWrite ("\nFINALIZANDO APLICACIÓN...\n");
    delete SP;
    delete Presen;
    delete SFx;
    delete GMundos;
    delete M;
    delete Menus;
    delete H;
    delete SMS;
    delete Config;
    LogWrite ("APLICACIÓN FINALIZADA CORRECTAMENTE.");
    LogEnd();
}

void IndicadorCarga (DATAFILE *dat_obj)
{
     extern Hardware *H;
     void *p;
     p = dat_obj;
     H->Indicador();
}

void close_button_handler(void)
{
     close_button_pressed = true;
}
END_OF_FUNCTION(close_button_handler)
