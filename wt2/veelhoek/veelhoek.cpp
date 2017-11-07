#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WPaintDevice>
#include <Wt/WPaintedWidget>
#include <Wt/WPainter>

#include <Wt/WBrush>
#include <Wt/WColor>
#include <Wt/WPen>
#include <Wt/WPointF>

class VeelhoekWidget : public Wt::WPaintedWidget
{
public:
   VeelhoekWidget(Wt::WContainerWidget *parent = 0)
	   : Wt::WPaintedWidget(parent)
   {
      resize(600, 100);   // Provide a default size.

      /*
         Kies voor SVG, canvas of een png afbeelding
       */
      //setPreferredMethod(InlineSvgVml);
      //setPreferredMethod(HtmlCanvas);
      setPreferredMethod(PngImage);
   }

protected:
   void paintEvent(Wt::WPaintDevice *paintDevice)
   {
      Wt::WPainter painter(paintDevice);
      painter.setBrush(Wt::WBrush(Wt::WBrush(Wt::blue)));
      painter.setPen(Wt::red);

	   // Teken een driehoek.
      const Wt::WPointF points[] =  
      {
         Wt::WPointF(10, 10), 
         Wt::WPointF(100, 10),
         Wt::WPointF(100, 100) 
      };
      painter.drawPolygon(points, 3);
   }

private:
};


class VeelhoekApplication : public Wt::WApplication
{
public:
   VeelhoekApplication(const Wt::WEnvironment& env);

private:
};

VeelhoekApplication::VeelhoekApplication(const Wt::WEnvironment& env)
  : Wt::WApplication(env)
{
   setTitle("Veelhoek");

   Wt::WContainerWidget *container = new Wt::WContainerWidget();
   VeelhoekWidget *painting = new VeelhoekWidget(container);
   
   root()->addWidget(container);
}

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
   return new VeelhoekApplication(env);
}

int main(int argc, char **argv)
{
   return Wt::WRun(argc, argv, &createApplication);
}

