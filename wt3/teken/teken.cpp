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

class TekenWidget : public Wt::WPaintedWidget
{
public:
   TekenWidget(Wt::WContainerWidget *parent);

protected:
   void paintEvent(Wt::WPaintDevice *paintDevice);
   void mouseDown(const Wt::WMouseEvent& e);
   void mouseUp(const Wt::WMouseEvent& e);
   void mouseDrag(const Wt::WMouseEvent& e);
   void touchStart(const Wt::WTouchEvent& e);
   void touchMove(const Wt::WTouchEvent& e);

private:
   bool             down;
   Wt::WPainterPath pad;
};


TekenWidget::TekenWidget(Wt::WContainerWidget *parent)
	   : Wt::WPaintedWidget(parent)
{
   resize(800, 600);   // Provide a default size.

   down = false;

   /*
      Kies voor SVG, canvas of een png afbeelding
    */
   setPreferredMethod(InlineSvgVml);
   //setPreferredMethod(HtmlCanvas);
   //setPreferredMethod(PngImage);

   Wt::log("info") << "TekenWidget() " << this << "\n";
   
	decorationStyle().setCursor("icons/pencil.cur", Wt::Cursor::CrossCursor);
   
   // Muisafhandeling
   mouseWentDown().connect(this, &TekenWidget::mouseDown);
   mouseWentUp().connect(this, &TekenWidget::mouseUp);
	mouseDragged().connect(this, &TekenWidget::mouseDrag);
	touchStarted().connect(this, &TekenWidget::touchStart);
	touchMoved().connect(this, &TekenWidget::touchMove);
	touchMoved().preventDefaultAction();
}

void TekenWidget::paintEvent(Wt::WPaintDevice *paintDevice)
{
   std::cout << "paintEvent()\n";
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
	painter.drawPath(pad);

   pad = Wt::WPainterPath(pad.currentPosition());
}

void TekenWidget::mouseDown(const Wt::WMouseEvent& e)
{
   Wt::Coordinates c = e.widget();
   pad = Wt::WPainterPath(Wt::WPointF(c.x, c.y));
   
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
	pad.lineTo(c.x, c.y);
	update(Wt::PaintFlag::PaintUpdate);
}

void TekenWidget::touchStart(const Wt::WTouchEvent& e) 
{
   Wt::Coordinates c = e.touches()[0].widget();
   pad = Wt::WPainterPath(Wt::WPointF(c.x, c.y));
}

void TekenWidget::touchMove(const Wt::WTouchEvent& e)
{
   Wt::Coordinates c = e.touches()[0].widget();
   pad.lineTo(c.x, c.y);
   update(Wt::PaintFlag::PaintUpdate);
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
   TekenWidget *painting = new TekenWidget(container);
   
   root()->addWidget(container);
   Wt::log("info") << "TekenApplication() " << teller << "\n";
}

TekenApplication::~TekenApplication()
{
   teller--;
   Wt::log("info") << "~TekenApplication() " << teller << "\n";
}

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
   return new TekenApplication(env);
}

int main(int argc, char **argv)
{
   return Wt::WRun(argc, argv, &createApplication);
}

