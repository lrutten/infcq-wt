#include <stdio.h>
#include "strook.h"
#include "insprong.h"

// $Date$
// $Author$
// $Revision$


Strook::Strook()
{
}

void Strook::voegdriehoekbij(Driehoek *d)
{
   driehoeken.push_back(d);  
}

void Strook::toon(int d)
{
   Insprong::springin(d);
   printf("Strook\n");
   for (int i = 0; i<driehoeken.size(); i++)
   {
      driehoeken[i]->toon(d + 1);
   }
}

