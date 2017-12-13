#include <stdio.h>
#include "vaart.h"
#include "insprong.h"

// $Date$
// $Author$
// $Revision$

Vaart::Vaart()
{
   minx = 0.0;
   maxx = 0.0;
   miny = 0.0;
   maxy = 0.0;
   minz = 0.0;
   maxz = 0.0;
}

void Vaart::voegmetingbij(Meting *m)
{
   metingen.push_back(m);  
}

void Vaart::voegstrookbij(Strook *s)
{
   stroken.push_back(s);  
}

void Vaart::toon(int d)
{
   Insprong::springin(d);
   printf("Vaart\n");

   Insprong::springin(d);
   printf("minx %lf\n", minx);
   Insprong::springin(d);
   printf("maxx %lf\n", maxx);

   Insprong::springin(d);
   printf("miny %lf\n", miny);
   Insprong::springin(d);
   printf("maxy %lf\n", maxy);

   Insprong::springin(d);
   printf("minz %lf\n", minz);
   Insprong::springin(d);
   printf("maxz %lf\n", maxz);

   for (unsigned int i = 0; i<metingen.size(); i++)
   {
      metingen[i]->toon(d + 1);
   }
   for (unsigned int i = 0; i<stroken.size(); i++)
   {
      stroken[i]->toon(d + 1);
   }
}

void Vaart::voorAlleDriehoeken(std::function<void(Driehoek *)> doe)
{
   for (unsigned int i = 0; i<stroken.size(); i++)
   {
      Strook *s = stroken[i];
      for (unsigned int j = 0; j<s->driehoeken.size(); j++)
      {
         Driehoek *d = s->driehoeken[j];
         doe(d);
      }
   }
}


int Vaart::isleeg(char *bf)
{
	int ntekens = 0;
	while (*bf != '\0')
	{
		if (*bf != ' ' && *bf != '\n' && *bf != '\t')
			ntekens++;
		bf++;
	}
	return ntekens == 0;
}


void Vaart::leesbestand(char *naam)
{
	char buf[1000];
	FILE *f;
	int i,n,temp;
	float x,y;
	int xx,yy;

	f=fopen(naam,"r");
	if(f==NULL)
	{
		printf("\nFout bij inlezing !\n\n");
		//exit(1);
	}

	for(i=0;i<8;i++)
	{
		fgets(buf,1000,f);
	}

   fgets(buf,1000,f);
	while(!feof(f))
	{
		if(isleeg(buf))
			continue;

		Meting *m = new Meting();

		sscanf(buf,"%d",&n);

		sscanf(&buf[25],"%f",&x);xx=(int)(x*10000);
		sscanf(&buf[34],"%f",&y);yy=(int)(y*10000);

		for(i=0;i<n;i++)
		{
			sscanf(&buf[42+(i*4)],"%d",&temp);

			Punt *p = new Punt(xx,yy+i*6,temp*10);
			m->voegpuntbij(p);
		}

		voegmetingbij(m);

		fgets(buf,1000,f);
	}
}


void Vaart::maakstroken()
{
   for (unsigned int im = 0; im < metingen.size() - 1; im++)
   {
      Meting *m1 = metingen[im];
      Meting *m2 = metingen[im + 1];
      
      Strook *s = new Strook();
      voegstrookbij(s);
      
      for (unsigned int ip = 0; ip < m1->punten.size() - 1; ip++)
      {
         Punt *p1 = m1->punten[ip];
         Punt *p2 = m1->punten[ip + 1];
         Punt *p3 = m2->punten[ip + 1];
         Punt *p4 = m2->punten[ip];
         
         s->voegdriehoekbij(new Driehoek(p1, p2, p4));
         s->voegdriehoekbij(new Driehoek(p2, p3, p4));
      }
   }
}

void Vaart::berekenminmax()
{
   minx = metingen[0]->berekenminx();
   for (unsigned int i = 1; i<metingen.size(); i++)
   {
      double temp = metingen[i]->berekenminx();
      
      if (temp < minx)
      {
         minx = temp;
      }
   }

   maxx = metingen[0]->berekenmaxx();
   for (unsigned int i = 1; i<metingen.size(); i++)
   {
      double temp = metingen[i]->berekenmaxx();
      
      if (temp > maxx)
      {
         maxx = temp;
      }
   }

   miny = metingen[0]->berekenminy();
   for (unsigned int i = 1; i<metingen.size(); i++)
   {
      double temp = metingen[i]->berekenminy();
      
      if (temp < miny)
      {
         miny = temp;
      }
   }

   maxy = metingen[0]->berekenmaxy();
   for (unsigned int i = 1; i<metingen.size(); i++)
   {
      double temp = metingen[i]->berekenmaxy();
      
      if (temp > maxy)
      {
         maxy = temp;
      }
   }
   
   
   minz = metingen[0]->berekenminz();
   for (unsigned int i = 1; i<metingen.size(); i++)
   {
      double temp = metingen[i]->berekenminz();
      
      if (temp < minz)
      {
         minz = temp;
      }
   }

   maxz = metingen[0]->berekenmaxz();
   for (unsigned int i = 1; i<metingen.size(); i++)
   {
      double temp = metingen[i]->berekenmaxz();
      
      if (temp > maxz)
      {
         maxz = temp;
      }
   }
}
