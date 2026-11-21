#include <windows.h>
#include <math.h>
#include <stdio.h>
#include "lista.h"
#include "window.h"
#include "optic.h"

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
const char szClassName[ ] = "WindowsApp";

void Escribe_Lista()
{
 /*    FILE *archivo;
     struct ELEMENTO *puntero;
 
     archivo = fopen ("datos.txt", "w");
     
     puntero = primero;
     
     while (puntero!= NULL)
     {
      fprintf (archivo,"%d %d %d %d %d %d\n",puntero->tipo, puntero->x, 
              puntero->y,puntero->forma,puntero->foco, puntero->Ancho);
      puntero = puntero->siguiente;  
     }
     fclose (archivo);*/
 
}

int Trazo_Analitico(int x1, int y1, float m1, int *x2, int *y2)
{         
     long pixel;
     int Continuar = 1;
     int x;
     int converge = 0;
     float y;
     
     x = x1;
     y = (float) y1;
     
     while (Continuar)
     {     
           pixel = (long)GetPixel (memoria, x, floor(y + 0.5));
           
           if (pixel == RGB(255,255,255))
           {
            Continuar = 0;
            converge = 1;
            }
           else {x++;y += m1;}
           
           if (x>640) {x = 640; Continuar = 0; converge = 0;}
     }
     *(x2) = x;
     *(y2) = (int)floor(y+0.5);
     
     if ( floor (y+0.5) > 350) converge = 0;
     
     return converge;
}

int Trazar_Rayo(int x, int y, struct ELEMENTO *Fuente)
{ 
    int             Continuar = 1;
    int             forma;
	      
    int             x1[10];
    int             y1[10];
    int             x2[10];
    int             y2[10];
     
    int             r[10]; /* 0 - Detener
                              1 - Derecha 
                              2 - Proximo a diverger */
    int             Derecha = 1;
     
    int             n_rayos;  /* Numero de rayos
                               3 - objeto
                               6 - cañon
                               10 - objeto*/ 
    float           m[10];
//    float           m_temp;
     
    int             i = 0;
    //int             dx
	int				dy;
    int             Foco; 
    int             valor;
    int             saltar = 0;
    struct ELEMENTO *puntero, *puntero2;
    
//	float	b[3];
	float	my_x = 0;


    puntero  = Fuente;
    if (Fuente->siguiente == NULL) return 0;
    else puntero2 = Fuente->siguiente;
     
    pen_tmp = SelectObject (memoria, pen_light);
	         
    for (i=0;i<3;i++)
    {   
    	x1[i] = x;
        y1[i] = y-50;
        r[i] = 1;
    }
         
    dy = 180-y+50;         
    
	/*CAÑON*/
	if (puntero->tipo == CANNON)
	{
		n_rayos = 10;
		while (Continuar)
		{
			if ((puntero2->tipo == LEN) | (puntero2->tipo == MIRROR))
			{
				for (i=0;i<n_rayos;i++)
				{
				  m[i] = (float)puntero->foco/30;
				  x1[i] = puntero->x;
				  y1[i] = puntero->y - 95 + i*10;
				}
				
				for (i=0;i<n_rayos;i++)
				{
				  x2[i] = puntero2->x;
				  y2[i] = (int)floor(m[i]*(x2[i]-x1[i]))+ y1[i];

				  MoveToEx (memoria, x1[i], y1[i], NULL);
				  LineTo (memoria, x2[i], y2[i]);
				  
				  x1[i]=x2[i];
				  y1[i]=y2[i];
				   
				  if (puntero2->tipo == MIRROR)
				  {
					 r[i] = Trazo_Analitico (x1[i], y1[i], m[i], &x2[i], &y2[i]);               
					 MoveToEx (memoria, x1[i], y1[i], NULL);
					 LineTo (memoria, x2[i], y2[i]);
					 x1[i]=x2[i];
					 y1[i]=y2[i];
				  }
				}  

			  Continuar = 0;
			  if (puntero2->siguiente != NULL)
				  puntero2 = puntero2->siguiente;
			  else saltar = 2;

			  if (saltar==1){
				  if (puntero2->anterior != NULL)
					puntero = puntero2->anterior;
				  saltar = 0;
				 }
			  else if (saltar == 2){
				  if (puntero2 != NULL){
					puntero = puntero2;
				  }
				  puntero2 = NULL;
				  saltar = 0;
				 }
			  else{
				  if (puntero->siguiente != NULL)
				  {
					  puntero = puntero->siguiente;
				  }
			  }
			}
			else
			{
			 if (puntero2->siguiente != NULL)
			 puntero2 = puntero2->siguiente;
			 else return 0;
			 saltar = 1;
			}    			   
			}
	}
         
	/* OBJETO */
	else if (puntero->tipo == OBJECT)   
	{
	n_rayos = 3;
	while (Continuar)
	{
	  if (puntero2->tipo == LEN)
	  {

	   Foco = puntero2->foco - puntero2->x;
	  
	   m[0] = 0.0;
	   m[1] = (float) dy/(puntero2->x - x);
	   m[2] = (float) dy/(puntero2->x -Foco-x);

	   for (i=0;i<n_rayos;i++){
		   x2[i] = puntero2->x;
		   y2[i] = (int)floor(m[i]*(x2[i]-x1[i]))+ y1[i];
	  
		   //aaaaa
		   //b[i] = y2[i] - x2[i]*m[i];

		   MoveToEx (memoria, x1[i], y1[i], NULL);
		   LineTo (memoria, x2[i], y2[i]);		   
		  
		   x1[i]=x2[i];
		   y1[i]=y2[i];
		   }              
			 
	   Continuar = 0;
	   if (puntero2->siguiente != NULL)
		  puntero2 = puntero2->siguiente;
	   else saltar = 2;

	   if (saltar==1){
		   if (puntero2->anterior != NULL){
			   puntero = puntero2->anterior;
		   }
		  saltar = 0;
		  }
	   else if (saltar == 2){
		   if (puntero2 != NULL){
			   puntero = puntero2;
		   }
			puntero2 = NULL;
			saltar = 0;
			}
	   else {
		   if (puntero->siguiente != NULL)
		   {
			   puntero = puntero->siguiente;
		   }
	   }
	  }
	  
	  else if (puntero2->tipo == MIRROR)
	  {
	   m[0] = 0.0;
	   m[1] = (float) dy/(puntero2->x - x+50);
	   m[2] = (float) dy/(puntero2->foco -x);
	   
	   for (i=0;i<n_rayos;i++){
		   r[i] = Trazo_Analitico (x1[i], y1[i], m[i], &x2[i], &y2[i]);
		   MoveToEx (memoria, x1[i], y1[i], NULL);
		   LineTo (memoria, x2[i], y2[i]);

		   x1[i]=x2[i];
		   y1[i]=y2[i];
		   }
	   Continuar = 0;
	   if (puntero2->anterior != NULL){
		   puntero2 = puntero2->anterior;
	   }
	   if (puntero2->siguiente != NULL){
		   puntero = puntero2->siguiente;
	   }
	  }
	  
	  else
	  {
		  if (puntero2->siguiente != NULL)
		  puntero2 = puntero2->siguiente;
		  else return 0;
		  saltar = 1;
	  }          
	  }
	}  
	/* FUENTE */
	else if (puntero->tipo == SOURCE)
	{
	while (Continuar)
	{
	if ((puntero2->tipo == LEN) | (puntero2->tipo == MIRROR))
	{
	   n_rayos = 10;
	   for (i=0;i<n_rayos;i++){
		   m[i] = (float) (-1.0 +0.25*i);
		   x1[i] = puntero->x+10;
		   y1[i] = puntero->y - 60+ 2*i;
		   }
			   
	   for (i=0;i<n_rayos;i++){
	   	   x2[i] = puntero2->x;
		   y2[i] = (int)floor(m[i]*(x2[i]-x1[i]))+ y1[i];
		   
		   MoveToEx (memoria, x1[i], y1[i], NULL);
		   LineTo (memoria, x2[i], y2[i]);
		  
		   x1[i]=x2[i];
		   y1[i]=y2[i];
		   
		   if (puntero2->tipo == MIRROR){
			  r[i] = Trazo_Analitico (x1[i], y1[i], m[i], &x2[i], &y2[i]);               
			  MoveToEx (memoria, x1[i], y1[i], NULL);
			  LineTo (memoria, x2[i], y2[i]);
			  x1[i]=x2[i];
			  y1[i]=y2[i];
			  }
		   }
		   
		   Continuar = 0;
		   if (puntero2->siguiente != NULL)
			  puntero2 = puntero2->siguiente;
		   else saltar = 2;

		   if (saltar==1){
			   if (puntero2->anterior != NULL){
				   puntero = puntero2->anterior;
			   }
			  saltar = 0;
			  }
		   else if (saltar == 2){
			   if (puntero2 != NULL){
				   puntero = puntero2;
			   }			  
			  puntero2 = NULL;
			  saltar = 0;
			}
		   else{
			   if (puntero->siguiente != NULL){
				   puntero = puntero->siguiente;
			   }
		   }

		   
	 }
	 else
		 {
		  if (puntero2->siguiente != NULL)
		  puntero2 = puntero2->siguiente;
		  else return 0;
		  saltar = 1;
		 }
	 } 
	 }
	     
          Continuar = 1;
          /*puntero = puntero->siguiente;*/
                    
           do{
               
           if (puntero->tipo == LEN)
           {            
           forma = puntero->forma;
           
           if (puntero == primero)
              puntero2 = NULL;
           
           if (puntero2 != NULL)
              { 
                Foco = puntero->foco - puntero->x;
           
                for (i=0;i<n_rayos;i++)
                    x2[i] = puntero2->x;
                if (Derecha){
					if (puntero2->siguiente != NULL){
						puntero2 = puntero2->siguiente;
					}
					if (puntero->siguiente != NULL){
						puntero = puntero->siguiente;
					}
                   }
                else{
					if (puntero->anterior != NULL){
						puntero = puntero->anterior;
						puntero2 = puntero->anterior;  
					}                   
                   /*Continuar = 0;*/
                   }
              }
           else
              {
                if (Derecha)
                for (i=0;i<n_rayos;i++)
                {   x2[i] = 640;
                    r[i] = 2;
                }
                else {
                for (i=0;i<n_rayos;i++)
                {    x2[i] = 0;
                     r[i] = 2;
                }
                }
                Continuar = 0;
                Foco = puntero->foco - puntero->x;
              }

           for (i=0;i<n_rayos;i++)
           { 
           if (r[i] != 0){
           if (!Derecha) forma = !forma;
              if (abs(180-y2[i])< 80)
               if (!forma) m[i] =  (float) m[i]+(float)(-y1[i]+180)/Foco;
               else        m[i] =  (float) m[i]-(float)(-y1[i]+180)/Foco;
              
              /*if (m[i] < 0.1)   m[i] = (float) 0.0;*/
              
              if (r[i] == 2) r[i] = 0;
              
              y2[i] = (int)floor(m[i]*(x2[i]-x1[i]))+ y1[i];  
                 
              MoveToEx (memoria, x1[i], y1[i], NULL);
              LineTo (memoria, x2[i], y2[i]);
              
              x1[i]=x2[i];
              y1[i]=y2[i];
              
              if (puntero->tipo == MIRROR){
               r[i] = Trazo_Analitico (x1[i], y1[i], m[i], &x2[i], &y2[i]);               
               MoveToEx (memoria, x1[i], y1[i], NULL);
               LineTo (memoria, x2[i], y2[i]);
               x1[i]=x2[i];
               y1[i]=y2[i];
               }
              }
           }           
           }
           
		   else if (puntero->tipo == MIRROR){
			   if (puntero->anterior != NULL){
				   puntero2 = puntero->anterior;
			   }
           
		   for (i=0;i<n_rayos;i++){            
				if (r[i] != 0){                
					m[i] = (float) m[i]*(-1)+(float)(y1[i]-180)/(x1[i]-puntero->foco);
                
					x2[i] = puntero2->x;                
					y2[i] = (int)(ceil(m[i]*(x2[i]-x1[i])+ y1[i]));  
                                           
					MoveToEx (memoria, x1[i], y1[i], NULL);                
					LineTo (memoria, x2[i], y2[i]);
                                
					x1[i]=x2[i];                
					y1[i]=y2[i];
                }
            }
		   if (puntero->anterior != NULL){
			   puntero = puntero->anterior;
			   puntero2 = puntero->anterior;
		   }            
		   Derecha = 0;
           }
           else {

                 if (puntero == primero){
                    valor = 0;
                    Continuar= 0;
                    }
                 else {
                      valor = puntero2->x;
                      puntero = puntero2;
                      
                      if (!Derecha)
                         puntero2 = puntero2->anterior;
                      }
                
                for (i=0;i<n_rayos;i++)
                    x2[i] = valor;      

                for (i=0;i<n_rayos;i++){
                    if (r[i] !=0){
                    y2[i] = (int)floor(m[i]*(x2[i]-x1[i]))+ y1[i];  
                 
                    MoveToEx (memoria, x1[i], y1[i], NULL);
                    LineTo (memoria, x2[i], y2[i]);
                    
                    x1[i]=x2[i];
                    y1[i]=y2[i];
                    
                    if (puntero->tipo == MIRROR){
                    r[i] = Trazo_Analitico (x1[i], y1[i], m[i], &x2[i], &y2[i]);               
                    MoveToEx (memoria, x1[i], y1[i], NULL);
                    LineTo (memoria, x2[i], y2[i]);
                    x1[i]=x2[i];
                    y1[i]=y2[i];
                    }                    
                    }
                 }
                 
           }

           }while (Continuar);
     
     
     SelectObject (memoria, pen_tmp);
	 return 0;
}


void Limpiar_Pantalla()
{     
     rc.left = 0;
     rc.top = 0;
     rc.right = 640;
     rc.bottom = 350;
     FillRect (memoria, &rc, (HBRUSH) (COLOR_BACKGROUND+6)); 

     MoveToEx (memoria, 0,180,NULL);             
     LineTo(memoria, 640,180);
     
     BitBlt(hdc, 0, 50, 640,350, memoria, 0, 0, SRCCOPY);    
}

void Actualizar ()
{
     struct ELEMENTO *puntero;   
     puntero = primero;
     
     BitBlt(hdc, 0, 50, 640,350, memoria, 0, 0, SRCCOPY);    
     
     while (puntero != NULL)
     {
     	if (puntero->tipo == 2)
     	{
     		Trazar_Rayo (puntero->x, puntero->y, puntero);
     	}
	  	puntero = puntero->siguiente;
     }
}

void Actualizar_Pantalla()
{
     struct ELEMENTO *puntero;
     puntero = primero;
     
     /* LIMPIAR MEMORIA */
     rc.left = 0;
     rc.top = 0;
     rc.right = 640;
     rc.bottom = 350;
     FillRect (memoria, &rc, (HBRUSH) (COLOR_BACKGROUND+6)); 
     
     /* DIBUJAR ESPEJOS */
     while (puntero != NULL)
     {
     	if (puntero->tipo == 5)
     	{
			Ancho = puntero->Ancho;
			
			SelectObject (memoria, pen_solid);   
			SelectObject (memoria, brush_color);
			Rectangle (memoria, puntero->x, 180-(80+Ancho),
								puntero->x+61,180+(81+Ancho));
			SelectObject (memoria, brush_black);
			SelectObject (memoria, pen_black);
			Chord (memoria, puntero->x-50,180-(80+Ancho),puntero->x+50,
								180+(80+Ancho),puntero->x,180+(80+Ancho),
								puntero->x,180-(80+Ancho));
			SelectObject (memoria, pen_solid);
							 
			MoveToEx (memoria, puntero->foco, 175, NULL);
			LineTo(memoria, puntero->foco, 185);
			
			Rectangle (memoria, puntero->foco - 3, 180 + 3,
								puntero->foco + 3, 180 - 3);
							 
			MoveToEx (memoria, 0,180,NULL);             
			LineTo(memoria, 640,180);
     	}
     	puntero = puntero->siguiente;
     }
     
     puntero = primero;
     
     /* DIBUJAR LENTES CONCAVAS */
     while (puntero != NULL)
     {
		 Ancho = puntero->Ancho;
		 Foco = puntero->foco - puntero->x;
		 if ( (puntero->tipo == 4) & (puntero->forma ))
		 {
			SelectObject (memoria, pen_solid);   
			SelectObject (memoria, brush_color);

			Rectangle (memoria, puntero->x-2*Ancho, 180-80,
						puntero->x+2*Ancho,180+81);
			SelectObject (memoria, brush_black);
			pen_tmp = SelectObject (memoria, pen_black);
			Chord (memoria, puntero->x+Ancho,180-80,puntero->x+3*Ancho,180+80,
							puntero->x+2*Ancho,180-80,puntero->x+2*Ancho,180+80);
			Chord (memoria, puntero->x-Ancho,180-80,puntero->x-3*Ancho,180+80,
							puntero->x-2*Ancho,180+80,puntero->x-2*Ancho,180-80);
			SelectObject (memoria, pen_tmp);
			
			MoveToEx (memoria, puntero->x-Foco, 175, NULL);
			LineTo(memoria, puntero->x-Foco, 185);
			MoveToEx (memoria, puntero->x+Foco, 175, NULL);
			LineTo(memoria, puntero->x+Foco, 185);
			
			Rectangle (memoria, puntero->x +Foco - 3, 180 + 3,
								puntero->x +Foco + 3, 180 - 3);

			Rectangle (memoria, puntero->x -Foco - 3, 180 + 3,
								puntero->x -Foco + 3, 180 - 3);
							
     	}
     puntero = puntero->siguiente;
     }

     puntero = primero;     
     
     /* DIBUJAR EL RESTO */
     while (puntero != NULL){
     
     switch (puntero->tipo)
            {
                    case 1: /*CAÑON*/
                         tmp_brush = SelectObject (memoria, brush_black);
                         Ellipse(memoria, puntero->x - 10, puntero->y - 60 - 50, 
                                          puntero->x + 10, puntero->y + 60 - 50);
                         SelectObject (memoria, tmp_brush); 
                         
                         Rectangle (memoria, puntero->x +30 - 3, puntero->y+puntero->foco + 3 - 50,
                                             puntero->x +30 + 3, puntero->y+puntero->foco - 3 - 50);
 
                         OPCION = 0;              
                         break;
                    case 2: /*OBJETO*/                  
                         MoveToEx (memoria, puntero->x,180,NULL);             
                         LineTo(memoria, puntero->x,puntero->y-50);

                         if (puntero->y < 240){
                         MoveToEx (memoria, puntero->x,puntero->y-50,NULL);
                         LineTo(memoria, puntero->x+5,puntero->y-50+5);
                         MoveToEx (memoria, puntero->x,puntero->y-50,NULL);
                         LineTo(memoria, puntero->x-5,puntero->y-50+5);}
                         else{
                         MoveToEx (memoria, puntero->x,puntero->y-50,NULL);
                         LineTo(memoria, puntero->x+5,puntero->y-50-5);
                         MoveToEx (memoria, puntero->x,puntero->y-50,NULL);
                         LineTo(memoria, puntero->x-5,puntero->y-50-5);
                         }
                         
                         SelectObject (memoria, pen_solid);   
                         SelectObject (memoria, brush_color);

                         OPCION = 0;
                         break;
                    case 3: /*FUENTE*/
                         tmp_brush = SelectObject (memoria, brush_green);
                         Rectangle (memoria, puntero->x - 10, puntero->y - 10 - 50, 
                                             puntero->x + 10, puntero->y + 10 - 50);
                         SelectObject (memoria, tmp_brush); 

                         OPCION = 0;
                         break;
                    case 4: /*LENTES*/
                         if (puntero->forma == 0){
                         Ancho = puntero->Ancho;
                         Foco = puntero->foco - puntero->x;
                         
                         SelectObject (memoria, pen_solid);   
                         SelectObject (memoria, brush_color);
                                                 
                         Chord (memoria, puntero->x-Ancho,180-80,puntero->x+Ancho,180+80,
                                   puntero->x,180+80,puntero->x,180-80);    
                         Chord (memoria, puntero->x-Ancho,180-80,puntero->x+Ancho,180+80,
                                   puntero->x,180-80,puntero->x,180+80);
                         
                         MoveToEx (memoria, puntero->x-Foco, 175, NULL);
                         LineTo(memoria, puntero->x-Foco, 185);
                         MoveToEx (memoria, puntero->x+Foco, 175, NULL);
                         LineTo(memoria, puntero->x+Foco, 185);
                         
                         Rectangle (memoria, puntero->x +Foco - 3, 180 + 3,
                                             puntero->x +Foco + 3, 180 - 3);
                         Rectangle (memoria, puntero->x -Foco - 3, 180 + 3,
                                             puntero->x -Foco + 3, 180 - 3);
                    
                                   }
                         break;
                    }
     
     puntero = puntero->siguiente;
     }
     
     puntero = primero;
     /* TRAZAR RAYOS */
      while (puntero != NULL){
      if (puntero->tipo == 2 || puntero->tipo == 1 || puntero->tipo == 3){
         Trazar_Rayo (puntero->x, puntero->y, puntero);
         }
      puntero = puntero->siguiente;
      }
      
     MoveToEx (memoria, 0,180,NULL);             
     LineTo(memoria, 640,180);         
}

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nFunsterStil)

{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_CROSS);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default color as the background of the window */
    wincl.hbrBackground = (HBRUSH) (COLOR_BACKGROUND);

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;
        
    cursor_normal = LoadCursor (NULL, IDC_ARROW);
    cursor_mano   = LoadCursor (NULL, IDC_HAND);
    cursor_cruz   = LoadCursor (NULL, IDC_CROSS);
        
    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
           0,                   /* Extended possibilites for variation */
           szClassName,         /* Classname */
           "OPTICA",           /* Title Text */
           WS_OVERLAPPEDWINDOW, /* default window */
           CW_USEDEFAULT,       /* Windows decides the position */
           CW_USEDEFAULT,       /* where the window ends up on the screen */
           640,                 /* The programs width */
           480,                 /* and height in pixels */
           HWND_DESKTOP,        /* The window is a child-window to desktop */
           NULL,                /* No menu */
           hThisInstance,       /* Program Instance handler */
           NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nFunsterStil);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)                  /* handle the messages */
    {
           case WM_SETCURSOR:
                switch (numero_cursor)
                {
                       case 0:
                            SetCursor(cursor_normal);
                            break;
                       case 1:
                            SetCursor(cursor_mano);
                            break;
                       case 2:
                            SetCursor(cursor_cruz);
                            break;
                       default:
                               break;
                               
				}
                break;
           case WM_PAINT:
			   hdc = BeginPaint (hwnd, &ps);
             
               if (PRIMERAVEZ == 0)
               {
				   memoria = CreateCompatibleDC(hdc);
				   hMemBmp = CreateCompatibleBitmap(hdc, 640,400);
				   hOldBmp = SelectObject(memoria, hMemBmp);
				   Limpiar_Pantalla();
				   PRIMERAVEZ = 1;
                
					black.lbStyle = BS_SOLID;
					black.lbColor = RGB(0,0,0);
	                                
					color.lbStyle = BS_SOLID;
					color.lbColor = RGB (255,255,255);
					green.lbStyle = BS_SOLID;
					green.lbColor = RGB (255,0,0);
	                
					brush_green = CreateBrushIndirect (&green);
					brush_black = CreateBrushIndirect (&black);
					brush_color = CreateBrushIndirect (&color);
	                
					pen_solid = CreatePen (PS_SOLID, 1, RGB(100, 200, 2));
					pen_light = CreatePen (PS_SOLID, 1, RGB(255,255,0));
					SelectObject(memoria,pen_solid);
	                
               		primero = NULL;
            		ultimo = NULL;
            		disponible_primero = NULL;
            		disponible_ultimo = NULL;
             }
             
             pen_black = CreatePen(PS_SOLID,1, RGB(0,0,0));
             pen_dot = CreatePen (PS_DOT, 1, RGB(100, 100, 100));
             SelectObject(hdc,pen_dot);
             
             MoveToEx (memoria, 0,180,NULL);             
             LineTo(memoria, 640,180);
             
             BitBlt(hdc, 0, 50, 640, 350, memoria, 0, 0, SRCCOPY);
             
             GRABAR = 0;
             
             break;
           case WM_CREATE:
			   GetClientRect (hwnd, &rc);
             
               Botones_xPos = (int)((rc.right-rc.left)-440)/2;
               Botones_yPos = (int)(rc.top+10);
             
			   btnCannon    = CREATE_BUTTON (L"Cañon",Botones_xPos, Botones_yPos, 100, 30);
			   btnObject    = CREATE_BUTTON (L"Objeto",Botones_xPos+110, Botones_yPos,100, 30);
			   btnSource    = CREATE_BUTTON (L"Fuente",Botones_xPos+220, Botones_yPos, 100, 30);
			   btnClearAll  = CREATE_BUTTON (L"Limpiar Todo",Botones_xPos+330, Botones_yPos, 100, 30);
			   btnLens      = CREATE_BUTTON (L"Lentes",Botones_xPos, (int)(Botones_yPos+rc.bottom-50), 100, 30);
			   btnMirror    = CREATE_BUTTON (L"Espejo",Botones_xPos+110, (int)(Botones_yPos+rc.bottom-50), 100, 30);
			   btnClearLast = CREATE_BUTTON (L"Limpiar",Botones_xPos+220, (int)(Botones_yPos+rc.bottom-50), 100, 30);
			   btnExit		= CREATE_BUTTON (L"Salir",Botones_xPos+330, (int)(Botones_yPos+rc.bottom-50),100, 30);             
               OPCION = 0;
             break;             
        case WM_LBUTTONUP:
             xPos = LOWORD (lParam);
             yPos = HIWORD (lParam);                                                    
             if ((PEGAR == 1)||(LONGITUD == 1))
             {
				 if (PEGAR)
				 {
					 Foco = abs(actual->foco - actual->x);
                     actual->x = xPos;
                     actual->y = yPos;
                         
                     Borrar_Elemento(actual, 0);
                     Acomodar_Elemento(actual);
                                                  
                     if (actual->tipo == 2)               /* OBJETO */						
                        Actualizar();
                     else if (actual->tipo == 4)          /* LENTE */
                        actual->foco = xPos+Foco;
                     else if (actual->tipo == 5)          /* ESPEJO */
                        actual->foco = xPos-Foco;
                 }
                 else if (LONGITUD)
				 {
					 Foco = abs(xPos - actual->x);
                     xPos = actual->x;
                     yPos = actual->y;
                     actual->Ancho = Ancho;
                       
                     if (actual->tipo == 1)
					 {
						 actual->foco = Alto;
                     }        
                     if (actual->tipo == 4)
					 {
						 actual->foco = xPos + Foco;
                         actual->forma = Convexo;
                     }                       
                     if (actual->tipo == 5)
					 {
                          actual->foco = LOWORD (lParam);                          
                          if (actual->foco > actual->x)
                             actual->foco = actual->x;}
                     }
					 OPCION = DIBUJAR;
					 switch (OPCION)
					 {
						 case 1: /*CAÑON*/
							 OPCION = 0;						
							 break;
						 case 2: /*OBJETO*/                                           
							 MoveToEx (memoria, xPos,180,NULL);             
							 LineTo(memoria, xPos,yPos-50);
							 if (yPos < 240)
							 {                         
								 MoveToEx (memoria, xPos,yPos-50,NULL);                         
								 LineTo(memoria, xPos+5,yPos-50+5);                         
								 MoveToEx (memoria, xPos,yPos-50,NULL);                         
								 LineTo(memoria, xPos-5,yPos-50+5);
							 }
							 else
							 {                         
								 MoveToEx (memoria, xPos,yPos-50,NULL);                         
								 LineTo(memoria, xPos+5,yPos-50-5);                         
								 MoveToEx (memoria, xPos,yPos-50,NULL);                         
								 LineTo(memoria, xPos-5,yPos-50-5);
							 }
							OPCION = 0;
							break;                    
						 case 3: /*FUENTE*/
							OPCION = 0;                         
							break;                    
						 case 5: /*LENTES*/						
							 if (Convexo == 0)						
							 {							
								 Chord (memoria, xPos-Ancho,180-80,xPos+Ancho,180+80,								
												 xPos,180+80,xPos,180-80);    
								 Chord (memoria, xPos-Ancho,180-80,xPos+Ancho,180+80,
									             xPos,180-80,xPos,180+80);
							 }
							 else 
							 {
								 Rectangle (memoria, xPos-2*Ancho, 180-80,xPos+2*Ancho,180+81);
								 SelectObject (memoria, brush_black);
								 pen_tmp = SelectObject (memoria, pen_black);
								 Chord (memoria, xPos+Ancho,180-80,xPos+3*Ancho,180+80,
											     xPos+2*Ancho,180-80,xPos+2*Ancho,180+80);
								 Chord (memoria, xPos-Ancho,180-80,xPos-3*Ancho,180+80,
												 xPos-2*Ancho,180+80,xPos-2*Ancho,180-80);
								 SelectObject (memoria, pen_tmp);                        
							 }
							 MoveToEx (memoria, xPos-Foco, 175, NULL);
							 LineTo(memoria, xPos-Foco, 185);
							 MoveToEx (memoria, xPos+Foco, 175, NULL);
							 LineTo(memoria, xPos+Foco, 185);                       
							 MoveToEx (memoria, 0,180,NULL);             
							 LineTo(memoria, 640,180);                        							
							 OPCION = 0;                    
							 break;                    
						 case 6: /*ESPEJO*/                         
							 Rectangle (memoria, xPos, 180-(80+Ancho),xPos+61,180+(81+Ancho));                         
							 SelectObject (memoria, brush_black);                     
							 SelectObject (memoria, pen_black);                     
							 Chord (memoria, xPos-50,180-(80+Ancho),xPos+50,180+(80+Ancho),                     
											 xPos,180+(80+Ancho),xPos,180-(80+Ancho));                     
							 SelectObject (memoria, pen_solid);                                                
							 MoveToEx (memoria, actual->foco, 175, NULL);                       
							 LineTo(memoria, actual->foco, 185);                                                
							 MoveToEx (memoria, 0,180,NULL);                                    
							 LineTo(memoria, 640,180);                                                
							 OPCION = 0;                        
							 break;                    
						 case 7: /*Apertura*/                                                 
							 OPCION = 0;                        
							 break;                    
						 default:                          
							 break;             
					 }                                 
					 SelectObject (memoria, pen_solid);               
					 SelectObject (memoria, brush_color);
				}                          
				Convexo = 0;             
				FOCO = 0;             
				LONGITUD = 0;                      
				ARRASTRAR = 0;             
				PEGAR = 0;            
				DIBUJAR = 0;             
				puntero = NULL;          

				Actualizar_Pantalla();                          
				BitBlt(hdc, 0, 50, 640,350, memoria, 0, 0, SRCCOPY);             
				break;
        case WM_LBUTTONDOWN:
			xPos = LOWORD (lParam);
            yPos = HIWORD (lParam);
             
            xActual = xPos;
             
            if (FOCO==1)
            {
				LONGITUD = 1; 
                SelectObject (memoria, pen_black);
                SelectObject (memoria, brush_black);
                xPos = actual->x;
                yPos = actual->y;
                tmp_x = actual->foco;
                GRABAR = 0;
                /*actual = puntero;*/
                puntero = NULL;
                OPCION = tmp2;               
			}
                                              
             if (ARRASTRAR == 1)
             {
				 SelectObject (memoria, pen_black);
                 SelectObject (memoria, brush_black);
                 xPos = puntero->x;
                 yPos = puntero->y;
                 tmp_x = puntero->foco;
                 OPCION = DIBUJAR;
                 GRABAR = 0;
                 actual = puntero;
                 puntero = NULL;                 
			 }
             
             switch (OPCION)
             {
				 case 1: /*CAÑON*/
					 if (GRABAR == 1){
						 Agregar_Elemento (1, xPos, yPos, 0, 0,0);
                         GRABAR = 0;
					 }
                         
                     OPCION = 0;
                     /*Agregar_Elemento (1, xPos, yPos, yPos+30);*/                                                  
                     break;
                 case 2: /*OBJETO*/
					 if (GRABAR == 1){
                         Agregar_Elemento (2, xPos, yPos, 0, 0,0);
                         GRABAR = 0;
					 }
                         
                     MoveToEx (memoria, xPos,180,NULL);             
                     LineTo(memoria, xPos,yPos-50);

                     if (yPos < 290){
                         MoveToEx (memoria, xPos,yPos-50,NULL);
                         LineTo(memoria, xPos+5,yPos-50+5);
                         MoveToEx (memoria, xPos,yPos-50,NULL);
                         LineTo(memoria, xPos-5,yPos-50+5);
					 }
					 else{
                         MoveToEx (memoria, xPos,yPos-50,NULL);
                         LineTo(memoria, xPos+5,yPos-50-5);
                         MoveToEx (memoria, xPos,yPos-50,NULL);
                         LineTo(memoria, xPos-5,yPos-50-5);                         
					 }                         
                         
					 OPCION = 0;                         
					 break;
                 case 3: /*FUENTE*/
					 if (GRABAR == 1){
						 Agregar_Elemento (3, xPos, yPos, 0, 0,0);
                         GRABAR = 0;
					 }
                     /*Agregar_Elemento (3, xPos, yPos, yPos+30);*/                   
                     OPCION = 0;
                     break;
                 case 5: /*LENTES*/
					 if (GRABAR == 1){
                         Agregar_Elemento (4, xPos, yPos, xPos+30,0,15);
                         GRABAR = 0;Ancho = 15;Foco = 30;
					 }
                     if (Convexo == 0){
						 Chord (memoria, xPos-Ancho,180-80,xPos+Ancho,180+80,
                             xPos,180+80,xPos,180-80);
                         Chord (memoria, xPos-Ancho,180-80,xPos+Ancho,180+80,
                                   xPos,180-80,xPos,180+80);
                     }
                     else {                                                       
                         Rectangle (memoria, xPos-2*Ancho, 180-80,xPos+2*Ancho,180+81);
                         SelectObject (memoria, brush_black);
                         pen_tmp = SelectObject (memoria, pen_black);
                         Chord (memoria, xPos+Ancho,180-80,xPos+3*Ancho,180+80,
                                   xPos+2*Ancho,180-80,xPos+2*Ancho,180+80);

                         Chord (memoria, xPos-Ancho,180-80,xPos-3*Ancho,180+80,
                                   xPos-2*Ancho,180+80,xPos-2*Ancho,180-80);
                         SelectObject (memoria, pen_tmp);                         
					 }
                     MoveToEx (memoria, xPos-Foco, 175, NULL);
                     LineTo(memoria, xPos-Foco, 185);
                     MoveToEx (memoria, xPos+Foco, 175, NULL);
                     LineTo(memoria, xPos+Foco, 185);
                         
                     OPCION = 0;
                     break;
                 case 6: /*ESPEJO*/
					 if (GRABAR == 1){
						 Agregar_Elemento (5, xPos, yPos, xPos,0, 0);
                         GRABAR = 0;Ancho = 0;tmp_x = xPos;
					 }
                                                  
                         Rectangle (memoria, xPos, 180-(80+Ancho),xPos+61,180+(81+Ancho));
                         SelectObject (memoria, brush_black);
                         pen_tmp = SelectObject (memoria, pen_black);
                         Chord (memoria, xPos-50,180-(80+Ancho),xPos+50,180+(80+Ancho),
                                   xPos,180+(80+Ancho),xPos,180-(80+Ancho));

                         SelectObject (memoria, pen_tmp);
                         MoveToEx (memoria, tmp_x, 175, NULL);
                         LineTo(memoria, tmp_x, 185);
                         SelectObject (memoria, brush_color);
                         
                         OPCION = 0;
                         break;
                    case 7: /*Apertura*/
                         /*Agregar_Elemento (6, xPos, yPos, 0);*/
                         
                         OPCION = 0;
                         break;
                    default:
                            break;
             }
             if (ARRASTRAR == 1)
             {  SelectObject (memoria, pen_solid); ARRASTRAR = 0; PEGAR = 1;
                SelectObject (memoria, brush_color);}
             else{ DIBUJAR = 0; ARRASTRAR = 0;PEGAR = 0;}
             
             if (FOCO == 1)
             { SelectObject (memoria, pen_solid); DIBUJAR = 0; PEGAR = 0;
               SelectObject (memoria, brush_color);}

             SelectObject (memoria, brush_color);
             SelectObject (memoria, pen_solid);
             MoveToEx (memoria, 0,180,NULL);             
             LineTo(memoria, 640,180);
             
             Actualizar_Pantalla();
             BitBlt(hdc, 0, 50, 640,350, memoria, 0, 0, SRCCOPY);

             tmp_x = xPos;
             tmp_y = yPos;
             
             break;
       case WM_COMMAND:
			if (BUTTON(btnCannon))
            { OPCION = 1; GRABAR = 1;}
            else if (BUTTON(btnObject))
            {OPCION = 2; GRABAR = 1;}
            else if (BUTTON(btnSource))
            {  OPCION = 3; GRABAR = 1;}
            else if (BUTTON(btnClearAll))
			{ 
				 Limpiar_Pantalla(); 
                 MoveToEx (memoria, 0,180,NULL);             
                 LineTo(memoria, 640,180);
                 BitBlt(hdc, 0, 50, 640,350, memoria, 0, 0, SRCCOPY);
                 
                 primero = NULL;
                 ultimo = NULL;
                 puntero = NULL;
                 actual = NULL;
            }          
			else if (BUTTON(btnLens))
            {	OPCION = 5; DIBUJAR = 5;Ancho = 15;Convexo = 0;	}
            else if (BUTTON(btnMirror))
            {	OPCION = 6; DIBUJAR = 6;Ancho = 0;}			
			else if (BUTTON(btnExit))
            { 
                 SelectObject(memoria, hOldBmp);
                 DeleteObject(hMemBmp);
                 DeleteDC(memoria);
                 ReleaseDC(hwnd,hdc);
                 PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            }
            else if (BUTTON(btnClearLast))
            {
				if (actual != NULL)
				{
					tmp = actual;       
					Borrar_Elemento(tmp,1); 
					actual = NULL;                       
					Actualizar_Pantalla();                     
				}
                else {MessageBox (hwnd, L"Solo se presiono borrar", L"Optica", MB_OK);}
            }               
            break;
       case WM_MOUSEMOVE:
             xPos = LOWORD (lParam);
             yPos = HIWORD (lParam);

             if (LONGITUD == 1){
                 if (xActual < actual->x){
                 if (xPos > actual->x) Convexo = 1;
                 else Convexo = 0;
                }
                else {
                 if (xActual > actual->x){
                 if (xPos < actual->x) Convexo = 1;
                 else Convexo = 0;}
                 
                 Alto = yPos - actual->y;
                 
                 }
                                 
                if (abs(xPos - actual->x)<45)
                {  
                   if (Ancho<15) Ancho++;}
                   else {if (Ancho>7)Ancho--;}
                   
                   if (actual->tipo == 5){
                   Ancho = actual->foco + actual->Ancho - xPos;
                   if (Ancho < 0) Ancho = 0;
                   else if (Ancho > 50) Ancho = 50;                   
                   }

                DIBUJAR = tmp2;
                xPos = actual->x;
                yPos = actual->y;
                }
             
             if ((OPCION == 0)&(PEGAR == 0)&(LONGITUD == 0))
             if (primero != NULL)
             {             puntero = primero;
             
                           if ((yPos>50)&(yPos<390))
                           {CONTINUAR = 1;}
                           else {DIBUJAR = 0; CONTINUAR = 0; ARRASTRAR = 0;numero_cursor = 0;}
                           
                           while (CONTINUAR)
                           {
                           /* CAÑON */
                           if (puntero->tipo == 1){
                           
                           if ( (xPos <= puntero->x + 33)&&(xPos >= puntero->x +27)
                              &&(yPos <= puntero->y+puntero->foco+3)
                              &&(yPos >= puntero->y+puntero->foco-3))
                           {
                                    FOCO = 1;
                                    numero_cursor = 1;
                                    CONTINUAR = 0;
                                    DIBUJAR = 0;
                                    tmp2 = 1;
                                    Ancho = puntero->Ancho;
                                    actual = puntero;
                           }
                           else {FOCO = 0;numero_cursor = 2;}
                           
                           if (((xPos <= puntero->x + 10)&&(xPos >= puntero->x - 10))&&
                            ((yPos <= puntero->y + 60)&&(yPos >= puntero->y - 60)))
                            {
                           
                                     xPos = puntero->x;
                                     yPos = puntero->y;
                                     DIBUJAR = 1;
                                     CONTINUAR = 0;
                                     ARRASTRAR = 1;
                                     numero_cursor = 1;
                                     actual = puntero;
                            }
                           else if (DIBUJAR == 1) { DIBUJAR = 0; ARRASTRAR = 0; numero_cursor = 0;}
                           }
                                 
                           /*OBJETO*/
                           else if (puntero->tipo == 2){
                           if ((xPos <= puntero->x + 10)&&(xPos >= puntero->x-10))
                           {
                                     xPos = puntero->x;
                                     yPos = puntero->y;
                                     DIBUJAR = 2;
                                     CONTINUAR = 0;
                                     ARRASTRAR = 1;
                                     numero_cursor = 1;
                                     }
                           else if (DIBUJAR == 2) { DIBUJAR = 0; ARRASTRAR = 0;numero_cursor = 0;}
                           }
                           
                           /* FUENTE */
                           else if (puntero->tipo == 3){
                           if ((xPos <= puntero->x + 10)&&(xPos >= puntero->x - 10))
                           {
                                     xPos = puntero->x;
                                     yPos = puntero->y;
                                     DIBUJAR = 3;
                                     CONTINUAR = 0;
                                     ARRASTRAR = 1;
                                     numero_cursor = 1;
                                     actual = puntero;
                           }
                           else if (DIBUJAR == 3) { DIBUJAR = 0; ARRASTRAR = 0; numero_cursor = 0;}
                           }
                           
                           /*LENTES*/
                           else if (puntero->tipo == 4){
                           
                           Foco = abs(puntero->x - puntero->foco);
                           Convexo = puntero->forma;
                           
                           if (yPos == 229)
                           if ((xPos > puntero->foco -3)&&(xPos < puntero->foco +3)
                              ||(xPos > (puntero->x - Foco -3))&&(xPos<(puntero->x - Foco +3)))
                           {  FOCO = 1;
                              numero_cursor = 1;
                              CONTINUAR = 0;
                              DIBUJAR = 0;
                              tmp2 = 5;
                              Ancho = puntero->Ancho;
                              actual = puntero;
                              
                              
                           }else {FOCO = 0;numero_cursor = 2;}
                           if ((xPos <= puntero->x + 10)&&(xPos >= puntero->x-10)||(LONGITUD == 1))
                           {
                                     xPos = puntero->x;
                                     DIBUJAR = 5;
                                     CONTINUAR = 0;
                                     ARRASTRAR = 1;
                                     numero_cursor = 1;
                                     Ancho = puntero->Ancho;
                                     actual = puntero;
                                     }
                           else if (DIBUJAR == 5) { DIBUJAR = 0; ARRASTRAR = 0; numero_cursor = 2;}
                           }

                           /*ESPEJO*/             
                           else if (puntero->tipo == 5){
                           if (yPos == 229)
                           if ((xPos > puntero->foco -3)&&(xPos < puntero->foco +3)){
                                     FOCO = 1;
                                     numero_cursor = 1;
                                     CONTINUAR = 0;
                                     DIBUJAR = 0;
                                     tmp2 = 6;
                                     Ancho = puntero->Ancho;
                                     actual = puntero;
                                     }
                           else {FOCO = 0;numero_cursor = 2;}
                           if ((xPos <= puntero->x +60)&&(xPos >= puntero->x+50))
                           {           xPos = puntero->x;
                                       DIBUJAR = 6;
                                       CONTINUAR = 0;
                                       ARRASTRAR = 1;
                                       numero_cursor = 1;
                                       Ancho = puntero->Ancho;
                                       }
                           else if (DIBUJAR == 6) { DIBUJAR = 0;ARRASTRAR = 0;numero_cursor = 2;}
                           }
                           
                           if ((puntero->siguiente != NULL) && (CONTINUAR == 1))
                                {  puntero = puntero->siguiente;}
                           else {  CONTINUAR = 0;}
                           }
             }
                        
            if (OPCION != 0)
            {DIBUJAR = OPCION;}
             
            if (!ARRASTRAR)                  
             BitBlt(hdc, 0, 50, 640,350, memoria, 0, 0, SRCCOPY);
             /*Actualizar();*/

             if ((yPos>50)&(yPos<390))
             switch (DIBUJAR)
             {
                    case 1: /*CAÑON*/
                         if ((yPos>120)&&(yPos < 340)){
                         tmp_brush = SelectObject (hdc, brush_black);
                         Ellipse(hdc, xPos - 10, yPos - 60, xPos + 10, yPos + 60);
                         SelectObject (hdc, tmp_brush);                       
                         }
                         GRABAR = 1;
                         break;
                    case 2: /*OBJETO*/
                         MoveToEx (hdc, xPos,230,NULL);             
                         LineTo(hdc, xPos,yPos);
                         
                         if (yPos < 230){
                         LineTo(hdc, xPos+5,yPos+5);
                         MoveToEx (hdc, xPos,yPos,NULL);
                         LineTo(hdc, xPos-5,yPos+5);
                         }
                         GRABAR = 1;
                         break;
                    case 3: /*FUENTE*/
                         if ((yPos>60)&(yPos < 390)){
                         tmp_brush = SelectObject (hdc, brush_green);
                         Rectangle (hdc, xPos - 10, yPos - 10, xPos + 10, yPos + 10);
                         SelectObject (hdc, tmp_brush);
                         }
                         GRABAR = 1;
                         break;
                    case 4: /*LIMPIAR TODO*/
                         break;
                    case 5: /*LENTES*/
                         if (Convexo == 0){
                         Arc (hdc, xPos-Ancho,230-80,xPos+Ancho,230+80,
                                   xPos,230+80,xPos,230-80);    
                         Arc (hdc, xPos-Ancho,230-80,xPos+Ancho,230+80,
                                   xPos,230-80,xPos,230+80);
                                   }
                         else {          
                         Arc (hdc, xPos+Ancho,230-80,xPos+3*Ancho,230+80,
                                   xPos+2*Ancho,230-80,xPos+2*Ancho,230+80);

                         Arc (hdc, xPos-Ancho,230-80,xPos-3*Ancho,230+80,
                                   xPos-2*Ancho,230+80,xPos-2*Ancho,230-80);
                                   
                         MoveToEx (hdc, xPos-2*Ancho, 230-80, NULL);
                         LineTo(hdc, xPos+2*Ancho,230-80);
                         MoveToEx (hdc, xPos-2*Ancho, 230+80, NULL);
                         LineTo(hdc, xPos+2*Ancho,230+80);
                                         }

                         GRABAR = 1;
                         break;
                    case 6: /*ESPEJO*/
                         Arc (hdc, xPos-50,230-(80+Ancho),xPos+50,230+(80+Ancho),
                                   xPos,230+(80+Ancho),xPos,230-(80+Ancho));    
                         MoveToEx (hdc, xPos, 230-(80+Ancho), NULL);
                         LineTo (hdc, xPos+60,230-(80+Ancho));
                         LineTo (hdc, xPos+60,230+(80+Ancho));
                         LineTo (hdc, xPos,230+(80+Ancho));
                         
                         GRABAR = 1;
                         break;
                    case 7: /*Apertura*/

                         break;
                    case 8: /*LIMPIAR ULTIMO*/

                         break;
                    default:
                            break;
             }

            break;
       case WM_DESTROY:
            SelectObject(memoria, hOldBmp);
            DeleteObject(hMemBmp);
            DeleteDC(memoria);
            ReleaseDC(hwnd,hdc);
            PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
            break; 
        default:                      /* for messages that we don't deal with */
            return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
