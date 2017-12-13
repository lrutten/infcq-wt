#include <stdio.h>

#include <QtGui>
//#include <qapplication.h>
//#include <qpushbutton.h>
//#include <qfont.h>
//#include <qpainter.h>

#include "vaart.h"

// $Date$
// $Author$
// $Revision$

/*
  In deze versie wordt de vaart in een venster getoond
  als een balk die volledig in het venster past.
  De schaalfactor wordt hiervoor aangepast.
  Er wordt ook rekeningen gehouden met de verschillen tussen
  de aspectratio's van de vaart en het venster.
*/

class DVenster : public QWidget
{
public:
    DVenster(QWidget *parent=0);
    void maakvaart();
    
protected:
    void paintEvent( QPaintEvent * );
    
private:
   Vaart *v;    
};


DVenster::DVenster(QWidget *parent)
        : QWidget(parent)
{
   // setMinimumSize( 200, 120 );
   // setMaximumSize( 200, 120 );

   // bouw alle objecten
   maakvaart();
}



void DVenster::maakvaart()
{
   printf("start\n");

   v = new Vaart();
   v->leesbestand("OELG_B06.DAT");
//   v->leesbestand("DATA.DAT");
   v->maakstroken();
   v->berekenminmax();
//   v->toon();
   int n = 0;
   v->voorAlleDriehoeken(
      
      [&n](Driehoek *s)
      {
         if (n < 5)
         {
            s->toon(0);
         }
         
         n++;
      }
   );
   
}



void DVenster::paintEvent( QPaintEvent * )
{
    QPainter p( this );
   
    // sla de instellingen van de qpainter op
    p.save();
    
    // haal de oorsprong, breedte en hoogte op van het venster
    // de waarden zijn in pixel; xv en yv zijn beide 0
    QRect r = p.viewport();
    int xv = r.left();
    int yv = r.top();
    int bv = r.width();
    int hv = r.height();
    printf("viewport %d %d %d %d\n", xv, yv, bv, hv);

    // haal de oorsprong, breedte en hoogte op van het kanaal
    // deze waarden zijn in km
    int xk = (int) v->getminx();
    int yk = (int) v->getminy();
    int bk = (int) (v->getmaxx() - v->getminx());
    int hk = (int) (v->getmaxy() - v->getminy());
    printf("window %d %d %d %d\n", xk, yk, bk, hk);

    // bereken de aspectratio's
    double av = ((double) hv)/bv;
    double ak = (v->getmaxy() - v->getminy())/(v->getmaxx() - v->getminx());
    
    printf("aspectratio's window %lf  kanaal %lf\n", av, ak);
    
    // Stel in hoe de rechthoek waarbinnen getekend wordt,
    // zichtbaar wordt in het venster.
    // Dit is afhankelijk van de aspectratio a=h/b.
    // Met setViewport wordt ingesteld waar de kader met de vaart 
    // binnen het venster verschijnt.
    // De parameters voor setViewport zijn in pixel uitgedrukt.
    if (av > ak)
    {
       // de voorgestelde vaart is minder hoog dan het venster
       // de viewportkader past in de volledige breedte van het venster
       int b = bv;
       int h = (int) (ak * b);
       p.setViewport(xv, yv + (hv - h)/2, b, h);
    }
    else
    {
       // de voorgestelde vaart is smaller dan het venster
       // de viewportkader past in de volledige hoogte van het venster
       int h = hv;
       int b = (int) (h/ak);
       p.setViewport(xv + (bv - b)/2, yv , b ,h);
    }

    // hiermee stellen we de wereldcoordinaten (in km) in    
    p.setWindow(xk, yk, bk, hk);
    
    
    // de vier hoekpunten
    /*
      worden niet gebruikt
    int x1 = xk;
    int y1 = yk;
    int x2 = xk + bk;
    int y2 = yk;
    int x3 = xk + bk;
    int y3 = yk + hk;
    int x4 = xk;
    int y4 = yk + hk;
     */
    
    // diagonaal, alleen voor de test van viewport
  /*  
    p.drawLine(x1, y1, x3, y3);
    p.drawLine(x2, y2, x4, y4);
    
    // rechthoek
    p.drawLine(x1, y1, x2, y2);
    p.drawLine(x2, y2, x3, y3);
    p.drawLine(x3, y3, x4, y4);
    p.drawLine(x4, y4, x1, y1);
    printf("lijn %d %d %d %d\n", xk, yk, xk + bk, yk + hk);
  */  

    // haal de min. en max. diepte voor de kleurbepaling  
    double minz = v->getminz();
    double maxz = v->getmaxz();

   // teken het kanaal    
   //v->teken(&p, minz, maxz);

   v->voorAlleDriehoeken(
      [&p, minz, maxz](Driehoek *s)
      {
         // s->toon(0);
         s->teken(&p, minz, maxz);
      }
   );
    
    // herstel de instellingen van de qpainter
    p.restore();
}



void Vaart::teken(QPainter *qp, double minz, double maxz)
{
   // teken alle stroken
   for (unsigned int i=0; i<stroken.size(); i++)
   {
      stroken[i]->teken(qp, minz, maxz);
   }
}

void Strook::teken(QPainter *qp, double minz, double maxz)
{
   // teken alle driehoeken
   for (unsigned int i=0; i<driehoeken.size(); i++)
   {
      driehoeken[i]->teken(qp, minz, maxz);
   }
   
}

// teken een driehoek met een vulkleur volgens diepte

void Driehoek::teken(QPainter *qp, double minz, double maxz)
{
   int x1 = (int) p1->x;
   int y1 = (int) p1->y;
   int x2 = (int) p2->x;
   int y2 = (int) p2->y;
   int x3 = (int) p3->x;
   int y3 = (int) p3->y;
   
   // teken niet gevulde driehoek
   //qp->drawLine(x1, y1, x2, y2);
   //qp->drawLine(x2, y2, x3, y3);
   //qp->drawLine(x3, y3, x1, y1);


   // kleur voor de grootste diepte
   int rmax = 0;
   int gmax = 0;
   int bmax = 255;
   
   // kleur voor de kleinste diepte
   int rmin = 0;
   int gmin = 255;
   int bmin = 0;
   
   // bereken gemiddelde diepte
   double z = (p1->z + p2->z + p3->z)/3.0;
   
   // u varieert van 0.0 (min. diepte) tot 1.0 (maximale diepte) 
   double u = (z - minz)/(maxz - minz);

   // dit is de interpolatie   
   int r = int (rmax * u + rmin * (1.0 - u));
   int g = int (gmax * u + gmin * (1.0 - u));
   int b = int (bmax * u + bmin * (1.0 - u));

   // stel de lijnkleur in
   qp->setPen( QColor(r,g,b));
   
   // stel de vulkleur in
   qp->setBrush( QColor(r,g,b));
   
   
   // teken gevulde driehoek
   QPolygon polygon(3);
   polygon.putPoints(0, 3, x1, y1, x2, y2, x3, y3);
   qp->drawPolygon( polygon );
}



int main( int argc, char **argv )
{
    QApplication a( argc, argv );

    DVenster w;
    w.setGeometry( 100, 100, 400, 300 );
    //a.setMainWidget( &w );
    w.show();
    return a.exec();
}

