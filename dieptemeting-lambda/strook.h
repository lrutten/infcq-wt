#ifndef STROOK_H
#define STROOK_H

#include <vector>

#include <qpainter.h>
#include "driehoek.h"

// $Date$
// $Author$
// $Revision$


class Strook
{
friend class Vaart;

private:
   std::vector<Driehoek *> driehoeken;
   
public:
   Strook();
   void voegdriehoekbij(Driehoek *d);
   void toon(int d = 0);
   void teken(QPainter *qp, double minz, double maxz);
};

#endif
