#ifndef PUNT_H
#define PUNT_H

// $Date$
// $Author$
// $Revision$


class Punt
{
friend class Vaart;

public:
   double x;
   double y;
   double z;

   int nr;
   static int teller;
   
   Punt(double ix, double iy, double iz);
   void toon(int d = 0);
};

#endif
