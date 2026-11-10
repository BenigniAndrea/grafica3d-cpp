#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <conio.h>
#include <math.h>
#include <fstream>
#include "graphics2.h"
using namespace std;

const float Scala=4, CostH=0.866, ResX=1280, ResY=720, Ripeti=100; 

struct angoli{
	double a;
	double b;
	double c;
};
struct cordinate3D{
	double x;
	double y;
	double z;
};

struct cordinate2D{
	double x;
	double y;
};

struct linea{
	int a;
	int b;
};

//Avvia il programma e la sua interfaccia, l'argv è il percorso del file trascinato .g3d
void eseguiInterfaccia(char* argv); 
//Converte un punto 3d in 2d (punto su schermo con coordiante xy) considerando i gradi degl'assi e la dim (zoom)
cordinate2D convertiPunti3D(cordinate3D assi, angoli lati,int dim); 
//Disegna gli assi sullo schermo considerando i gradi degl'assi
void disegnaSfondo(angoli lati);
//Disegna la forma su schermo passandogli: 
//i punti, quantità punti,gradi assi,collegamenti punti,quantità collegamenti e dim (zoom) 
void disegnaForma(cordinate3D A[],int qp,angoli lati,linea L[],int ql,int dim);
//Ruota un punto. Gli si passano gli assi da modificare, il coseno e il seno
cordinate2D ruotaPunti(cordinate2D punto,float coseno,float seno);
//Ruota un punto con formula matematica non semplificata (vecchia versione)
cordinate2D ruotaPuntiOld(cordinate2D punto,double rotazione);
string intString(int n);
void mostraMenu();

void menu()
{
	setbkcolor(WHITE);	        //Imposta il colore dello sfondo
	setcolor(DARKGRAY);
	cleardevice();
	settextstyle(10,0,3);
	settextstyle(10,0,9);
	outtextxy(ResX/3.8,ResY/4.5,"Grafica 3D");
    settextstyle(10,0,5);
	outtextxy(ResX/3.4,ResY/2.2,"Scegli assonometria");
	outtextxy(ResX/2.27,ResY/1.6,"Esci");
	settextstyle(10,0,2);
	outtextxy(ResX/14,ResY/1.3,"Istruzioni");
	outtextxy(ResX/14,ResY/1.2,"Crediti");
}

void mostraIstruzioni()
{
	setbkcolor(WHITE);	        //Imposta il colore dello sfondo
	settextstyle(10,0,7);
	cleardevice();
	outtextxy(ResX/2.9,ResY/6,"Istruzioni:");
	settextstyle(10,0,3);
	outtextxy(ResX/5,ResY/2.5,"-Schiaccia su 'scegli assonometria' per iniziare a");
	outtextxy(ResX/5,ResY/2.2,"-Schiaccia su esci....");
	outtextxy(ResX/5,ResY/2,"-...............");
	outtextxy(ResX/2.27,ResY/1.3,"<-Indietro");
	while(1)
	{
		while(!mousedown())
	    {
		}
		
		if(mousecurrentx()<(int)(ResX/1.83) && mousecurrentx()>(int)(ResX/2.28) && mousecurrenty()<(int)(ResY/1.25) && mousecurrenty()>(int)(ResY/1.29))
		     return;
	}	
}

void mostraCrediti()
{
	setbkcolor(WHITE);	        //Imposta il colore dello sfondo
	settextstyle(10,0,7);
	cleardevice();
	outtextxy(ResX/2.6,ResY/6,"Crediti:");
	settextstyle(10,0,3);
	outtextxy(ResX/2.5,ResY/3,"  Sviluppato da: ");
	outtextxy(ResX/6.7,ResY/2.6,"Rinaldi Michele, Giambalvo Luca, Benigni Andrea, Airoldi Maurizio");
	outtextxy(ResX/2.7,ResY/2,"IISS E.Majorana, Seriate");
	outtextxy(ResX/2.27,ResY/1.3,"<-Indietro");
	while(1)
	{
		while(!mousedown())
	    {
		}
		
		if(mousecurrentx()<(int)(ResX/1.83) && mousecurrentx()>(int)(ResX/2.28) && mousecurrenty()<(int)(ResY/1.25) && mousecurrenty()>(int)(ResY/1.29))
		    return;
	}	
}

/*void cambiaColoreSfondo()
{
	setbkcolor(WHITE);	        //Imposta il colore dello sfondo
	settextstyle(10,0,7);
	cleardevice();
	outtextxy(ResX/3.7,ResY/6,"Scegli il colore:");
	settextstyle(10,0,3);
//	outtextxy(ResX/ ,ResY/ ,"ROSSO");
//	outtextxy(ResX/ ,ResY/ ,"VERDE");
//	outtextxy(ResX/ ,ResY/ ,"NERO");
//	outtextxy(ResX/ ,ResY/ ,"GIALLO"); 
	outtextxy(ResX/2.27,ResY/1.3,"<-Indietro");
	while(1)
	{
		while(!mousedown())
	    {
		}
		
		if(mousecurrentx()<(int)(ResX/1.83) && mousecurrentx()>(int)(ResX/2.28) && mousecurrenty()<(int)(ResY/1.25) && mousecurrenty()>(int)(ResY/1.29))
		    return;
	}	
} */

int main(int argc,char* argv[])
{
	int GraphDriver=0, GraphMode=0;		//Avvia la finestra grafica
	initgraph(&GraphDriver,&GraphMode,"",ResX,ResY);
	if(argc<2)
	{
		cout<<"Manca la forma .g3d";
		//argv[1]="C:\Users\pollo\OneDrive\Desktop\Linea.g3d";
		//eseguiInterfaccia("C:\\Users\\pollo\\OneDrive\\Desktop\\Linea.g3d");
		return 1;
	}
	while(1)
	  eseguiInterfaccia(argv[1]);
	return 0;
}

void eseguiInterfaccia(char* argv)
{
	mostraMenu();
	//Mostra menu mauri.
	setbkcolor(WHITE);	        //Imposta il colore dello sfondo
	settextstyle(10,0,1);
	cleardevice();
	fstream f1(argv,ios::in);	//Apre il file .g3d trascinato sul programma
	int qpunti, qlinee;	        //Quanti punti ci sono, quanti linee ci sono
	f1>>qpunti;
	cordinate3D* Punti = new cordinate3D[qpunti];	    //Array che contiene i punti 3D in maniera statica
	cordinate3D* Puntitmp = new cordinate3D[qpunti];	//Array che contiene i punti 3D in maniera dinamica
	for(int i=0;i<qpunti;i++)	                        //Inserisce nell'array le informazioni da file
	{
		f1>>Punti[i].x;
		f1>>Punti[i].y;
		f1>>Punti[i].z;	
	}
	for(int i=0;i<qpunti;i++)	//Inserisce nell'array le informazioni da file
	{
		Puntitmp[i].x=Punti[i].x;
		Puntitmp[i].y=Punti[i].y;
		Puntitmp[i].z=Punti[i].z;
	}
	f1>>qlinee;
	linea* Linee = new linea[qlinee];
	for(int i=0;i<qlinee;i++)
	{
		f1>>Linee[i].a;
		f1>>Linee[i].b;
	}
	angoli lati={330,90,210};
	int dim=50,y=0;
	bool exit=false;
	char opz=0;
	angoli tmplati=lati;
	double rotazione=15;
	float rotCoseno=cos(rotazione*(M_PI/180)); 
	float rotSeno=sin(rotazione*(M_PI/180));
	cordinate2D tmp={0,0};
	disegnaSfondo(tmplati);
	disegnaForma(Puntitmp,qpunti,lati,Linee,qlinee,dim);
	while(1)
	{
		opz=0;
		string x=intString(mousecurrentx()),y=intString(mousecurrenty());
		outtextxy(0,0,&x[1]);
		outtextxy(ResX/2,0,&y[1]);
		if(kbhit())
			opz=getchg();
		else
			continue;
		switch(opz)
		{
			case '+':
			{
				if(dim<80)
					dim+=5;
				break;
			}
			case '-':
			{
				if(dim>20)
					dim-=5;
				break;
			}
			case 'x':
			{
				cout<<"X: "<<mousecurrentx()<<"Y: "<<mousecurrenty()<<endl;
				for(int i=0;i<qpunti;i++)
				{
					tmp=ruotaPunti({Puntitmp[i].z,Puntitmp[i].y},rotCoseno,rotSeno);
					Puntitmp[i].z=tmp.x;
					Puntitmp[i].y=tmp.y;
				}
				break;
			}
			case 'y':
			{
				for(int i=0;i<qpunti;i++)
				{
					tmp=ruotaPunti({Puntitmp[i].x,Puntitmp[i].z},rotCoseno,rotSeno);
					Puntitmp[i].x=tmp.x;
					Puntitmp[i].z=tmp.y;
				}
				break;
			}
			case 'z':
			{
				for(int i=0;i<qpunti;i++)
				{
					tmp=ruotaPunti({Puntitmp[i].x,Puntitmp[i].y},rotCoseno,rotSeno);
					Puntitmp[i].x=tmp.x;
					Puntitmp[i].y=tmp.y;
				}
				break;
			}
			case 'a':
			{
				rotazione=0.01;
				rotCoseno=cos(rotazione*(M_PI/180)); 
				rotSeno=sin(rotazione*(M_PI/180));
				for(int n=0; n<100000; n++)
				{
					for(int i=0;i<qpunti;i++)
					{
						tmp=ruotaPunti({Puntitmp[i].z,Puntitmp[i].y},rotCoseno,rotSeno);
						Puntitmp[i].z=tmp.x;
						Puntitmp[i].y=tmp.y;
						tmp=ruotaPunti({Puntitmp[i].x,Puntitmp[i].z},rotCoseno,rotSeno);
						Puntitmp[i].x=tmp.x;
						Puntitmp[i].z=tmp.y;
						tmp=ruotaPunti({Puntitmp[i].x,Puntitmp[i].y},rotCoseno,rotSeno);
						Puntitmp[i].x=tmp.x;
						Puntitmp[i].y=tmp.y;
					}	
					if(n%10==0)
						cleardevice();
					disegnaSfondo(tmplati);
					disegnaForma(Puntitmp,qpunti,lati,Linee,qlinee,dim);	
				}
				break;
			}
			case 'e':
			{
				return;
				break;
			}
			break;
		}
		cleardevice();
		disegnaSfondo(tmplati);
		disegnaForma(Puntitmp,qpunti,lati,Linee,qlinee,dim);
	}
	return;
}

cordinate2D convertiPunti3D(cordinate3D assi, angoli lati, int dim)
{
	cordinate2D punto;
	lati.a = lati.a * M_PI / 180 ;
	lati.b = lati.b * M_PI / 180 ;
	lati.c = lati.c * M_PI / 180 ;	
	punto.x = (assi.x * dim) * cos(lati.a) + (assi.z *dim) * cos(lati.c) + (assi.y *dim) * cos(lati.b);
	punto.y = (assi.z * dim) * sin(lati.c) + (assi.y *dim) * sin(lati.b) + (assi.x *dim) * sin(lati.a);
	return punto;
}

void disegnaSfondo(angoli lati)
{
	cordinate2D punti;
	setcolor(DARKGRAY);
	punti=convertiPunti3D({1,0,0},lati, ResY/2);
	line(ResX/2,ResY/2,punti.x + ResX/2, ResY/2 - punti.y);
	punti=convertiPunti3D({0,1,0},lati, ResY/2);
	line(ResX/2,ResY/2,punti.x + ResX/2, ResY/2 - punti.y);
	punti=convertiPunti3D({0,0,1},lati,ResY/2);
	line(ResX/2,ResY/2,punti.x + ResX/2, ResY/2 - punti.y);
}


void disegnaForma(cordinate3D A[],int qp,angoli lati,linea L[],int ql, int dim)
{	
	cordinate2D P2d[qp]; //Punti in 2 dimensioni
	for(int i = 0; i < qp; i++)
		P2d[i] = convertiPunti3D(A[i],lati,dim);
	setcolor(BLUE);
	for(int i = 0; i < qp; i++)
		circle(P2d[i].x + ResX/2,ResY/2 - P2d[i].y,5);
	for(int i = 0; i < ql; i++)
		line(P2d[L[i].a-1].x + ResX/2,ResY/2 - P2d[L[i].a-1].y, P2d[L[i].b-1].x + ResX/2,ResY/2 - P2d[L[i].b-1].y);	
}

cordinate2D ruotaPunti(cordinate2D punto,float coseno,float seno)
{
	float x = (punto.x*coseno)-(punto.y*seno);
	float y = (punto.x*seno)+(punto.y*coseno);
	punto.x=x;
	punto.y=y;
	return punto;
}

//Versione deprecata della funzione ruotaPunti
cordinate2D ruotaPuntiOld(cordinate2D punto,double rotazione)
{
	float raggio,alpha;
	raggio=sqrt(punto.x*punto.x+punto.y*punto.y);
	if(!(punto.x==0 && punto.y==0))
		alpha=atan(punto.y/punto.x);
	else
		alpha=0;
	if(punto.x<0)
		alpha=alpha-M_PI;
	punto.x=raggio*cos(alpha + rotazione*M_PI/180);
	punto.y=raggio*sin(alpha + rotazione*M_PI/180);
	return punto;
}

string intString(int n)
{
	string s,s1;
	while(n!=0)
	{
		s.push_back(char(n%10)+'0');	//Converti intero in stringa
		n/=10;
	}
	for(int i=s.length();i>=0;i--)	//Inverti la stringa 
		s1.push_back(s[i]);
	return s1;	//Dai in return il valore + il carattere %
	
}

void mostraMenu()
{
	ShowWindow(GetConsoleWindow(),SW_HIDE);
	setbkcolor(WHITE);	        //Imposta il colore dello sfondo
	setcolor(DARKGRAY);
	cleardevice();
	settextstyle(10,0,3);
	settextstyle(10,0,9);
	outtextxy(ResX/3.8,ResY/4.5,"Grafica 3D");
    settextstyle(10,0,5);
	outtextxy(ResX/3.4,ResY/2.2,"Scegli assonometria");
	outtextxy(ResX/2.27,ResY/1.6,"Esci");
	settextstyle(10,0,2);
	outtextxy(ResX/14,ResY/1.3,"Istruzioni");
	outtextxy(ResX/14,ResY/1.2,"Crediti");
	//outtextxy(ResX/14,ResY/1.1,"Cambia colore sfondo");
	//CREA FIGURA
	while(1)
	{
		while(!mousedown())
	    {
		}
		
		if(mousecurrentx()<(int)(ResX/1.7) && mousecurrentx()>(int)(ResX/2.8) && mousecurrenty()<(int)(ResY/1.47) && mousecurrenty()>(int)(ResY/1.6))
		     exit(1);
        else
	      if(mousecurrentx()<(int)(ResX/1.4) && mousecurrentx()>(int)(ResX/3.4) && mousecurrenty()<(int)(ResY/1.95) && mousecurrenty()>(int)(ResY/2.15))
	         return;
	    else
	      if(mousecurrentx()<(int)(ResX/6) && mousecurrentx()>(int)(ResX/16) && mousecurrenty()<(int)(ResY/1.20) && mousecurrenty()>(int)(ResY/1.33))
	      {
	      	mostraIstruzioni();
	      	menu();
		  }
	    else
	      if(mousecurrentx()<(int)(ResX/7) && mousecurrentx()>(int)(ResX/16) && mousecurrenty()<(int)(ResY/1.11) && mousecurrenty()>(int)(ResY/1.23))
	      {
	      	mostraCrediti();
	      	menu();
		  }
	         
	   /* else
	      if(mousecurrentx()<(int)(ResX/3) && mousecurrentx()>(int)(ResX/16) && mousecurrenty()<(int)(ResY/1.01) && mousecurrenty()>(int)(ResY/1.13))
	         cambiaColoreSfondo(); */
	}	
}

