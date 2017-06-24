#include "logger.h"
#include "filehandling.h"

int LogInit ()
{
    char fname[512];

#ifndef WINDOWS__
    snprintf(fname, 511, "%s/.capitan/capitan.log", getenv("HOME"));
    mkdirp(fname, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#else
    snprintf (fname, 511, "%s\\CapitanSevilla", getenv ("APPDATA"));
    mkdir (fname);
    snprintf (fname, 511, "%s\\CapitanSevilla\\capitan.log", getenv ("APPDATA"));
#endif

    Logger.open (fname, ios::out);
    return (int) Logger.fail();
}

void LogWrite(const char *format,...)
{
     char Temp[256];
     va_list args;
     va_start(args,format);
     vsprintf (Temp, format, args);
     Logger.write (Temp, strlen(Temp));
     Logger.flush();
     va_end(args);
}

int LogEnd ()
{
    Logger.close();
    return (int)Logger.fail();
}

int GameLogInit (int AMode, int &Fase)
{
    char Titulo[128];
    int Ret;
    if (AMode == 1)
        sprintf (Titulo, "./data/capitan.at%i", Fase);
      else
        sprintf (Titulo, "./data/capitan.at%i", (int) (rand() % 2) + 1);
    LogWrite ("Abriendo fichero %s... ", Titulo);

    if (AMode == 1) {
        GameLogger.open (Titulo, ios::out | ios::binary);
        GameLogWrite (Fase);
        }
      else {
        GameLogger.open (Titulo, ios::in | ios::binary);
        Fase = GameLogRead();
      }

    Ret = (int) GameLogger.fail();
    if (Ret)
        LogWrite ("ERROR!!!\n");
      else
        LogWrite ("OK!\n");
    return Ret;
}

void GameLogWrite(int data)
{
     Dato[0] = (char)data;
     GameLogger.write (Dato, 1);
     GameLogger.flush();
}

int GameLogRead()
{
     GameLogger.read (Dato, 1);
     if (GameLogger.fail())
        return 0;
     return (int)Dato[0];
}
int GameLogEnd ()
{
    GameLogger.close();
    return (int)GameLogger.fail();
}
