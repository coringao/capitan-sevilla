#include "filehandling.h"

#ifndef WINDOWS__
// make (sub)directories including multiple subdirs
int mkdirp(const char *fqfn, mode_t mode)
{
   char *t, str[STRLEN];

   struct stat stbuf;
   int len;

   t = (char *)fqfn;
   memset(str, '\0', STRLEN);

   while (*t) {
      if (*t == '/') {
         len = t - fqfn;

         if ((len < STRLEN) && (len > 0)) {
            strncpy(str, fqfn, len);

            if (stat(str, &stbuf) != 0)
               if (mkdir(str, mode) != 0) return(-1);
         }
      }

      t++;
   }
   return(0);
}
#endif

