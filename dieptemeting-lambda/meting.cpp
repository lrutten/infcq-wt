#include <stdio.h>
#include "meting.h"
#include "insprong.h"

Meting::Meting()
{
}

void Meting::voegpuntbij(Punt *p)
{
   punten.push_back(p);  
}

void Meting::toon(int d)
{
   Insprong::springin(d);
   printf("Meting\n");
   for (unsigned int i = 0; i<punten.size(); i++)
   {
      punten[i]->toon(d + 1);
   }
}

double Meting::berekenminx()
{
   double min;
   
   min = punten[0]->x;
   
   for (unsigned int i=1; i<punten.size(); i++)
   {
      if (punten[i]->x < min)
      {
         min = punten[i]->x;
      }
   }
   return min;
}

double Meting::berekenmaxx()
{
   double max;
   
   max = punten[0]->x;
   
   for (unsigned int i=1; i<punten.size(); i++)
   {
      if (punten[i]->x > max)
      {
         max = punten[i]->x;
      }
   }
   return max;
}

double Meting::berekenminy()
{
   double min;
   
   min = punten[0]->y;
   
   for (unsigned int i=1; i<punten.size(); i++)
   {
      if (punten[i]->y < min)
      {
         min = punten[i]->y;
      }
   }
   return min;
}

double Meting::berekenmaxy()
{
   double max;
   
   max = punten[0]->y;
   
   for (unsigned int i=1; i<punten.size(); i++)
   {
      if (punten[i]->y > max)
      {
         max = punten[i]->y;
      }
   }
   return max;
}

double Meting::berekenminz()
{
   double min;
   
   min = punten[0]->z;
   
   for (unsigned int i=1; i<punten.size(); i++)
   {
      if (punten[i]->z < min)
      {
         min = punten[i]->z;
      }
   }
   return min;
}

double Meting::berekenmaxz()
{
   double max;
   
   max = punten[0]->z;
   
   for (unsigned int i=1; i<punten.size(); i++)
   {
      if (punten[i]->z > max)
      {
         max = punten[i]->z;
      }
   }
   return max;
}



