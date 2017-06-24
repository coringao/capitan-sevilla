#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define STRLEN 512

#ifndef WINDOWS__
int mkdirp(const char *fqfn, mode_t mode);
#endif

