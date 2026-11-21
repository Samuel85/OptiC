
#include <stdio.h>
#include <stdlib.h>
#include "lista.h"

void Acomodar_Elemento(struct ELEMENTO *nuevo)
{
	struct ELEMENTO *tmp;
	struct ELEMENTO *puntero;
	int CONTINUAR = 1;

	 if (primero == NULL){
	    primero = nuevo;
            ultimo = nuevo;
         }
         else {
            puntero = primero;
          if (nuevo->x < primero->x){
                  nuevo->siguiente = puntero;
                  nuevo->anterior = NULL;
                  puntero->anterior = nuevo;
                  primero = nuevo;
                  }
          else {
               while (puntero->siguiente != NULL && CONTINUAR){
                     puntero = puntero->siguiente;
                     if (nuevo->x < puntero->x){
                        tmp = puntero->anterior;
                        nuevo->siguiente = puntero;
                        puntero->anterior = nuevo;
                        puntero = tmp;
                        puntero->siguiente = nuevo;
                        nuevo->anterior = tmp;
                                 
                        CONTINUAR = 0;
                     }
               }
                     if (CONTINUAR){  
			puntero = ultimo;
                        ultimo->siguiente = nuevo;
                        nuevo->anterior = puntero;
                        ultimo = nuevo;
                     }
               }
	       }

}

int Agregar_Elemento(int tipo, int x, int y, int foco, int forma, int Ancho)
{
	/*struct ELEMENTO *nuevo;

	nuevo = (struct ELEMENTO *)malloc(sizeof(struct ELEMENTO));
	 if (nuevo == NULL) return -1;
	
	    nuevo->tipo = tipo;
        nuevo->x = x;
        nuevo->y = y;
        nuevo->foco = foco;
        nuevo->forma = forma;
        nuevo->Ancho = Ancho;
	    
        nuevo->anterior = NULL;
     	nuevo->siguiente = NULL;

	Acomodar_Elemento(nuevo);*/
	struct ELEMENTO *nuevo;

	if (disponible_ultimo == NULL && disponible_primero ==NULL)
	{
		nuevo = (struct ELEMENTO *)malloc(sizeof(struct ELEMENTO));
		//printf("Elemento agregado reservando memoria dinamicamente\n");
	}
	else
	{
		nuevo = disponible_ultimo;
		if (disponible_primero != disponible_ultimo)
		{
			disponible_ultimo = disponible_ultimo->anterior;
			disponible_ultimo->siguiente = NULL;
		}
		else
		{
			disponible_ultimo = NULL;
			disponible_primero = NULL;			
		}
		//printf("Elemento agregado reciclando memoria\n");		
	}
	
	if (nuevo == NULL) return -1;
	
    nuevo->tipo = tipo;
    nuevo->x = x;
    nuevo->y = y;
    nuevo->foco = foco;
    nuevo->forma = forma;
    nuevo->Ancho = Ancho;
	nuevo->anterior = NULL;
	nuevo->siguiente = NULL;

	Acomodar_Elemento(nuevo);
	return 0;
}

void Borrar_Elemento(struct ELEMENTO *puntero, int liberar)
{
	struct ELEMENTO *tmp, *tmp2;
	
	if (puntero == primero){
	 primero = puntero->siguiente;
	 puntero->anterior = NULL;
	}
	else if (puntero == ultimo){
	 tmp = puntero->anterior;
	 tmp->siguiente = NULL;
	 ultimo = tmp;
	}
	else {
	 tmp = puntero->anterior;
	 tmp2 = puntero->siguiente;

	 tmp->siguiente = tmp2;
	 tmp2->anterior = tmp;
	}

	puntero->anterior = NULL;
	puntero->siguiente = NULL;
	
	//if (liberar)
	// free (puntero);
	if (liberar)
	{
		if (disponible_ultimo == NULL)
		{
			disponible_primero = puntero;
			disponible_ultimo = puntero;
			//printf ("Si entra a liberar memoria 1\n");
		}
		else
		{
			disponible_ultimo->siguiente = puntero;
			puntero->anterior = disponible_ultimo;
			disponible_ultimo = puntero;
			//printf ("Si entra a liberar memoria 2\n");
		}
	}	


}
