#ifndef MESSAGES_H
#define MESSAGES_H

#include "constantes.h"
#include "configuracion.h"

extern Configuracion *Config;

const int MaxMens = 330;

class Messages{

public:
    Messages();
    ~Messages();
    int InitMens(int Lang);
    int Init(int Lang);
    const char* ReadMens(int Num);
    const char* ReadName (int Num);
    const char* ReadDedic (int Num);
    const char* ReadNameS (int Num);
private:
    string Mensajes[MaxMens];

};

#endif
