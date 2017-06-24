#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <allegro.h>

using namespace std;

fstream Logger;
fstream GameLogger;
char Dato[1];

int LogInit ();
void LogWrite(const char *format,...);
int LogEnd ();

int GameLogInit (int AMode, int &Fase);
void GameLogWrite(int data);
int GameLogRead();
int GameLogEnd ();

#endif
