#include <stdio.h>
#include "punt.h"
#include "insprong.h"

// $Date$
// $Author$
// $Revision$


int Punt::teller = 0;

Punt::Punt(double ix, double iy, double iz) : x(ix), y(iy), z(iz)
{
   nr = teller++;
}

void Punt::toon(int d)
{
   Insprong::springin(d);
   printf("Punt %d: %lf %lf %lf\n", nr, x, y, z);
}
