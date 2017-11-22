#include <stdio.h>
#include <unistd.h>

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WPaintDevice>
#include <Wt/WPaintedWidget>
#include <Wt/WPainter>

#include <Wt/WBrush>
#include <Wt/WColor>
#include <Wt/WPen>
#include <Wt/WPointF>

// teveel?
#include <Wt/WCssDecorationStyle>
#include <Wt/WEvent>
#include <Wt/WPainterPath>
#include <Wt/WPushButton>
#include <Wt/WRectF>
#include <Wt/WTemplate>
#include <Wt/WText>
#include <Wt/WToolBar>

#include "caf/all.hpp"

//#define LOCAL_DRAW

using meldaan_atom    = caf::atom_constant<caf::atom("meldaan")>;
using meldaf_atom     = caf::atom_constant<caf::atom("meldaf")>;
using destructor_atom = caf::atom_constant<caf::atom("destructor")>;
using aangemeld_atom  = caf::atom_constant<caf::atom("aangemeld")>;
using nieuw_atom      = caf::atom_constant<caf::atom("nieuw")>;
using weg_atom        = caf::atom_constant<caf::atom("weg")>;
using muissleep_atom  = caf::atom_constant<caf::atom("muissleep")>;
using toonlijn_atom   = caf::atom_constant<caf::atom("toonlijn")>;

#ifdef LOCAL_DRAW
#else
using lijn            = std::tuple<double, double, double, double>;
#endif

caf::actor_system_config cfg;
caf::actor_system        asystem{cfg};


class Register
{
private:
   caf::actor self;
   int        teller;
   int        aantal;
   std::mutex mu;
   std::map<caf::actor, int> lijst;
   
protected:
	static caf::behavior actor_fun(caf::event_based_actor* self, Register *zelf)
	{
	   zelf->aantal = 0;
	   zelf->teller = 0;
	   printf("zelf1a %p %d\n", zelf, zelf->aantal);
	   std::cout << "actor_fun " << zelf->aantal << "\n";
	   printf("zelf1b %p %d\n", zelf, zelf->aantal);
      return
      {
         [=](meldaan_atom, const caf::actor &zender)
         {
            std::lock_guard<std::mutex> lock(zelf->mu);
            std::cout << "ontvangen meldaan\n";
       	   printf("zelf2a %p %d\n", zelf, zelf->aantal);
            zelf->aantal++;
            std::cout << "zelf->aantal " << zelf->aantal << "\n";
            
            zelf->teller++;
            int nr = zelf->teller;
            //zelf->lijst.insert(std::pair<caf::actor, int>(zender, nr));
            //zelf->lijst.insert(std::make_pair(zender, nr));
            zelf->lijst[zender] = nr;
            for (auto &kv: zelf->lijst)
            {
               if (kv.first == zender)
               {
                  std::cout << "gelijk aan zender\n";

                  // Geef een aangemeld antwoord terug
                  self->send(zender, aangemeld_atom::value, zelf->aantal, zelf->teller);
               }
               else
               {
                  std::cout << "niet gelijk aan zender\n";
                  self->send(kv.first, nieuw_atom::value, zelf->aantal);
               }
            }
         },
         [=](meldaf_atom, const caf::actor &zender)
         {
            std::cout << "ontvangen meldaf\n";

            std::lock_guard<std::mutex> lock(zelf->mu);
       	   printf("zelf2b %p %d\n", zelf, zelf->aantal);
            zelf->aantal--;
            int erased = zelf->lijst.erase(zender);
            
            for (auto &kv: zelf->lijst)
            {
               self->send(kv.first, weg_atom::value, zelf->aantal);
            }

            std::cout << "zelf->aantal " << zelf->aantal << "erased " << erased << "\n";
         },
         [=](muissleep_atom, lijn l, const caf::actor &zender)
         {
            std::cout << "ontvangen muissleep\n";

            std::lock_guard<std::mutex> lock(zelf->mu);
            
            for (auto &kv: zelf->lijst)
            {
               std::cout << "zend toonlijn " << std::to_string(kv.second) << "\n";
               self->send(kv.first, toonlijn_atom::value, l);
            }
         }
      };
   }


public:
   Register() : self(asystem)
   {
      //Wt::log("teken") << "Acteur() " << aantal << "\n";
	   printf("Acteur() zelf3 %p %d\n", this, this->aantal);
      
      caf::scoped_actor se(asystem);
      self = se->spawn(actor_fun, this);

      //Wt::log("teken") << "Acteur() na spawn\n";
      //int na = asystem.registry().running();
      //Wt::log("teken") << "Acteur() #actors running " << na << "\n";
   }

   const caf::actor &getSelf()
   {
      return self;
   }
   
   int getAantal()
   {
      std::lock_guard<std::mutex> lock(mu);
      return aantal;
   }
};

Register reg;

class TekenWidget : public Wt::WPaintedWidget
{
public:
   TekenWidget(Wt::WContainerWidget *parent);
   ~TekenWidget();

   const caf::actor &getSelf()
   {
      return self;
   }

protected:
   void paintEvent(Wt::WPaintDevice *paintDevice);
   void mouseDown(const Wt::WMouseEvent& e);
   void mouseUp(const Wt::WMouseEvent& e);
   void mouseDrag(const Wt::WMouseEvent& e);
   void touchStart(const Wt::WTouchEvent& e);
   void touchMove(const Wt::WTouchEvent& e);
	static caf::behavior actor_fun(caf::event_based_actor* self, TekenWidget *zelf);
	void refresh();

private:
   caf::actor        self;   
   bool              down;
   int               aantal;
   std::mutex        mu;
   Wt::WApplication *app;
   int               nummer;
#ifdef LOCAL_DRAW
   Wt::WPainterPath  pad;
#else
   Wt::WPointF       start;
   std::vector<lijn> lijnen;
#endif
};

TekenWidget::TekenWidget(Wt::WContainerWidget *parent)
	   : Wt::WPaintedWidget(parent), self(asystem), aantal(0)
{
   app = Wt::WApplication::instance();
   app->enableUpdates();
   
   resize(800, 600);   // Provide a default size.

   down = false;

   /*
      Kies voor SVG, canvas of een png afbeelding
    */
   setPreferredMethod(InlineSvgVml);
   //setPreferredMethod(HtmlCanvas);
   //setPreferredMethod(PngImage);

   Wt::log("teken") << "TekenWidget() " << this << "\n";
   
	decorationStyle().setCursor("icons/pencil.cur", Wt::Cursor::CrossCursor);
   
   // Muisafhandeling
   mouseWentDown().connect(this, &TekenWidget::mouseDown);
   mouseWentUp().connect(this, &TekenWidget::mouseUp);
	mouseDragged().connect(this, &TekenWidget::mouseDrag);
	touchStarted().connect(this, &TekenWidget::touchStart);
	touchMoved().connect(this, &TekenWidget::touchMove);
	touchMoved().preventDefaultAction();

   caf::scoped_actor se(asystem);
   self = se->spawn(actor_fun, this);
	
   Wt::log("teken") << "TekenWidget() einde\n";
}

TekenWidget::~TekenWidget()
{
   Wt::log("teken") << "~TekenWidget() " << this << "\n";
   caf::scoped_actor se(asystem);
   se->send(self, destructor_atom::value);
}

// function-based, dynamically typed, blocking API

caf::behavior TekenWidget::actor_fun(caf::event_based_actor* slf, TekenWidget *zelf)
{
   slf->send(reg.getSelf(), meldaan_atom::value, slf);
   return
   {
      [=](destructor_atom)
      {
         std::cout << "destructor\n";
         slf->send(reg.getSelf(), meldaf_atom::value, slf);
      },
      [=](aangemeld_atom, int ntal, int nr)
      {
         {
            std::lock_guard<std::mutex> lock(zelf->mu);
            zelf->aantal = ntal;
            zelf->nummer = nr;
         }
         zelf->refresh();

         std::cout << "aangemeld aantal " << ntal << " nr " << nr << "\n";
      },
      [=](nieuw_atom, int ntal)
      {
         std::cout << "nieuw aantal " << ntal << "\n";
         {
            std::lock_guard<std::mutex> lock(zelf->mu);
            zelf->aantal = ntal;
         }
         
         zelf->refresh();
      },
      [=](weg_atom, int ntal)
      {
         std::cout << "weg aantal " << ntal << "\n";
         {
            std::lock_guard<std::mutex> lock(zelf->mu);
            zelf->aantal = ntal;
         }
         
         // De refresh moet buiten de lock!
         zelf->refresh();
      },
      [=](muissleep_atom, lijn l)
      {
         std::cout << "muissleep\n";

         slf->send(reg.getSelf(), muissleep_atom::value, l, slf);
      },
      [=](toonlijn_atom, lijn l)
      {
         std::cout << "toonlijn\n";

         {
            std::lock_guard<std::mutex> lock(zelf->mu);
            zelf->lijnen.push_back(l);
         }

         zelf->refresh();
      }
   };
}

void TekenWidget::refresh()
{
   std::cout << "refresh\n";
   if (app != nullptr)
   {
      std::cout << "app refresh\n";

      Wt::WApplication::UpdateLock lock(app);
      if (lock)
      {
         std::cout << "app lock\n";

         // Nu gaat update() zonder segmentation fout
         update();
         app->triggerUpdate();
      }
   }
   else
   {
      std::cout << "app nullptr\n";
   }
   std::cout << "refresh einde\n";
}

void TekenWidget::paintEvent(Wt::WPaintDevice *paintDevice)
{
   Wt::log("teken") << "paintEvent()\n";
   Wt::WPainter painter(paintDevice);
   
   if (down)
   {
      painter.setBrush(Wt::WBrush(Wt::WBrush(Wt::green)));
      painter.setPen(Wt::red);
   }
   else
   {
      painter.setBrush(Wt::WBrush(Wt::WBrush(Wt::blue)));
      painter.setPen(Wt::green);
   }
      

   // Teken een driehoek.
   const Wt::WPointF points[] =  
   {
      Wt::WPointF(10, 10), 
      Wt::WPointF(100, 10),
      Wt::WPointF(100, 100) 
   };
   painter.drawPolygon(points, 3);
   
   
   painter.setRenderHint(Wt::WPainter::RenderHint::Antialiasing);
  
   Wt::WPen pen;
	pen.setWidth(3);
	pen.setColor(Wt::green);
	pen.setCapStyle(Wt::PenCapStyle::FlatCap);
	pen.setJoinStyle(Wt::PenJoinStyle::MiterJoin);
	painter.setPen(pen);
	
#ifdef LOCAL_DRAW	
	painter.drawPath(pad);
   pad = Wt::WPainterPath(pad.currentPosition());
#else
   {
	   std::lock_guard<std::mutex> lock(mu);
	   
	   for (lijn l: lijnen)
	   {
	      double x1 = std::get<0>(l);
	      double y1 = std::get<1>(l);
	      double x2 = std::get<2>(l);
	      double y2 = std::get<3>(l);

	      Wt::WPainterPath pad = Wt::WPainterPath(Wt::WPointF(x1, y1));
	      pad.lineTo(x2, y2);
         painter.drawPath(pad);
	   }
   }
#endif

	pen.setColor(Wt::black);
	painter.setPen(pen);
	{
	   std::lock_guard<std::mutex> lock(mu);

	   std::string t1 = "aantal " + std::to_string(aantal); 
      painter.drawText(200, 20, 30, 20, Wt::AlignmentFlag::AlignLeft, t1);

	   std::string t2 = "nummer " + std::to_string(nummer); 
      painter.drawText(200, 50, 30, 20, Wt::AlignmentFlag::AlignLeft, t2);
   }

   const int le = 200;
   char buf[le];
   gethostname(buf, le); 
   std::string t3 = buf; 
   painter.drawText(200, 80, 30, 20, Wt::AlignmentFlag::AlignLeft, t3);
}

void TekenWidget::mouseDown(const Wt::WMouseEvent& e)
{
   Wt::Coordinates c = e.widget();
#ifdef LOCAL_DRAW   
   pad = Wt::WPainterPath(Wt::WPointF(c.x, c.y));
#else
   start = Wt::WPointF(c.x, c.y);
#endif   
   down = true;
   //update();
}

void TekenWidget::mouseUp(const Wt::WMouseEvent& e)
{
   Wt::Coordinates c = e.widget();
   
   down = false;
   //update();
}

void TekenWidget::mouseDrag(const Wt::WMouseEvent& e)
{
   Wt::Coordinates c = e.widget();
#ifdef LOCAL_DRAW
	pad.lineTo(c.x, c.y);
	update(Wt::PaintFlag::PaintUpdate);
#else
   lijn l = std::make_tuple(start.x(), start.y(), c.x, c.y);
   start  = Wt::WPointF(c.x, c.y);

   caf::scoped_actor se(asystem);
   se->send(self, muissleep_atom::value, l);
#endif
}

void TekenWidget::touchStart(const Wt::WTouchEvent& e) 
{
   Wt::Coordinates c = e.touches()[0].widget();
#ifdef LOCAL_DRAW
   pad = Wt::WPainterPath(Wt::WPointF(c.x, c.y));
#endif
}

void TekenWidget::touchMove(const Wt::WTouchEvent& e)
{
   Wt::Coordinates c = e.touches()[0].widget();
#ifdef LOCAL_DRAW
   pad.lineTo(c.x, c.y);
   update(Wt::PaintFlag::PaintUpdate);
#endif
}


// Deze klasse doet dienst als sessie.
// Het begin en einde van de levensduur van de sessie
// kan gevolgd worden door de constructor en destructor.
class TekenApplication : public Wt::WApplication
{
public:
   TekenApplication(const Wt::WEnvironment& env);
   ~TekenApplication();

private:
   static int teller;
};

int TekenApplication::teller = 0;

TekenApplication::TekenApplication(const Wt::WEnvironment& env)
  : Wt::WApplication(env)
{
   teller++;
   std::string titel = "Teken";
   std::stringstream ss;
   ss << teller;
   titel += ss.str();
   setTitle(titel);

   Wt::WContainerWidget *container = new Wt::WContainerWidget();

   // Maak de TekenWidget
   TekenWidget *painting = new TekenWidget(container);
   
   root()->addWidget(container);
   Wt::log("teken") << "TekenApplication() " << teller << "\n";
}

TekenApplication::~TekenApplication()
{
   teller--;
   Wt::log("teken") << "~TekenApplication() " << teller << "\n";
}

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
   return new TekenApplication(env);
}




int main(int argc, char **argv)
{
   Wt::log("teken") << "start\n";

   return Wt::WRun(argc, argv, &createApplication);
}

