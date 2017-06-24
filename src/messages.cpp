#include "messages.h"

Messages::Messages()
{
    LogWrite ("Creado Gestor de mensajes.\n");
}


Messages::~Messages()
{
    LogWrite ("Destruido Gestor de mensajes.\n");
}

int Messages::Init (int Lang)
{
    char FileName[20];
    ifstream Source;
    int Ret = 0, Cont = 0;
    string Buffer;

    if (!MaxMens)
	   return 0;

    sprintf (FileName, "./lang/lang%i.txt", Lang);
    LogWrite ("Intentando cargar mensajes de %s...", FileName);
    Source.open (FileName);
    if (Source.fail()) {
        LogWrite ("ERROR.\nNo se pueden cargar mensajes de texto.\n");
        return -1;
	    }

    while (Cont < MaxMens && !Source.eof() ) {
	getline (Source, Buffer);
	while (Buffer.length() && Buffer.substr (0,1) == " ")
	      Buffer = Buffer.substr (1, Buffer.length() - 1);
	if (Buffer.length() > 1 && Buffer.substr (0, 1) != "#") {
	    Mensajes[Cont++] = Buffer;
	    if (*((char*) (Mensajes[Cont-1].substr (Mensajes[Cont-1].length() - 1, 1)).c_str()) == 13) // If you enter the carriage return as a character in the string
		    Mensajes [Cont-1] = Mensajes [Cont-1].substr (0, Mensajes[Cont-1].length () - 1); // Asi que hay que quitarlo :)
	    }
	}

    LogWrite ("\n %i mensajes cargados correctamente de %i esperados.\n", Cont, MaxMens);

    Source.close();

    if (Cont < MaxMens)
	Ret = 1;

    return Ret;
}

int Messages::InitMens(int Lang)
{
    if (!Init (Lang))
       return 0;
    Config->ToggleIdioma (0);
    return Init(0);
}

const char* Messages::ReadMens(int Num)
{
     if (Num < MaxMens)
        return Mensajes[Num].c_str();
     return NULL;
}

const char* Messages::ReadName (int Num)
{
/* Meter a
.- Litos.net
*/

      const char* Nombres[] = {
            "CEZ Team",
            "Alejandro Layunta Villarejo - Alx",
            "Ángel Ló Pérez - Anjuel",
            "Augusto Ruiz - AugustoRuiz",
            "David Cañadas Mazo - DavidCM",
            "Daniel Celemín García - LordFred",
            "David Donaire - DadMan",
            "Iván Sánchez Jordá - IvanZX",
            "Luis I. García Ventura - Benway",
            "Miguel Á. González Álvarez - Zikitrake",
            "Jaime Tejedor Gómez - Metalbrain",
            "José Andrés Gil Ayube - Konamito",
            "José Juan Ródenas - Sejuan",
            "José Luis Veiga Sierra - Karnevi",
            "José Vicente Masó - Wyz",
            "Xavier Ortiz Carrasco - TBrazil",
            "Jose Ignacio Ramos - Kendroock",
            "José Luis Castaño - Na_th_an",
            "Raúl Molina Guerrero - Zemman",
            "Raúl Montón - The Punisher",
            "Fran Crespo - Black Hole",
            "Francisco Javier Velasco - Pagantipaco",
	        "Manuel Sagra - Edge",
	        "Alberto Riera - [[c|-|e]]",
            "Fernando Díaz Jordán - Onofre",
            "Nacho Esperidón - Mandrake el Mago",
            "Álvaro Mateos",
            "Ángel Tirado",
            "José Ramón Pérez",
            "Manuél Gascó",
            "Rafael García",
            "David Brioso",
            "Ángel Macedo",
            "Manuel Guillén",
            "Francisco Gómez",
            "Francisco Rodríguez",
            "Traperic",
            "Santiago Ontañón Villar - Popolon",
            "Lennart Steinke",
            "Marta Rodríguez Iglesias",
            "Manuel Pazos - Guillian",
            "Patsie"
            };
      return Nombres [Num];
}

const char* Messages::ReadDedic (int Num)
{

      const char* Nombres[] = {
            "María Rengel Arribas",
            "Sin tí hubiera sido imposible",
            "terminar esto nunca.",
            "Gracias por estar a mi lado.",
            "Emma",
            "Finalmente consiguió",
            "abstraerse de mi",
            "alter ego."
            };
      return Nombres [Num];
}

const char* Messages::ReadNameS (int Num)
{

      const char* Nombres[] = {
            "Achintya",
            "Anton",
            "Banco de imágenes y sonidos - Ministerio de Educación",
            "batchku",
            "Benboncan",
            "bullshit99",
            "Connum",
            "fonogeno",
            "HardPCM",
            "harri",
            "ignotus",
            "jobro",
            "Jovica",
            "melack",
            "memexikon",
            "mich3d",
            "patchen",
            "reinsamba",
            "roscoetoon",
            "Sea Fury",
            "thannvanispen",
            "THE_bizniss",
            "tigersound",
            "UATaudio",
            "zippi1"
            };

      return Nombres [Num];
}

