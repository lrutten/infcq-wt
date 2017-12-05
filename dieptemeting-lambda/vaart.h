#ifndef VAART_H
#define VAART_H

#include <vector>
#include <functional>

#include <qpainter.h>
#include "meting.h"
#include "strook.h"

// $Date$
// $Author$
// $Revision$


class Vaart
{
private:
   std::vector<Meting *> metingen;
   std::vector<Strook *> stroken;
   
   double minx;
   double maxx;
   double miny;
   double maxy;
   double minz;
   double maxz;
   
   
public:
   Vaart();
   void voegmetingbij(Meting *m);
   void voegstrookbij(Strook *m);
   void toon(int d = 0);
   void leesbestand(char *naam);
   int isleeg(char *bf);
   void maakstroken();
   void berekenminmax();
   void teken(QPainter *qp, double minz, double maxz);
   void voorAlleDriehoeken(std::function<void(Driehoek*)> doe);
   
   double getminx()
   {
      return minx;
   }
   double getmaxx()
   {
      return maxx;
   }
   double getminy()
   {
      return miny;
   }
   double getmaxy()
   {
      return maxy;
   }
   double getminz()
   {
      return minz;
   }
   double getmaxz()
   {
      return maxz;
   }
};

#endif
