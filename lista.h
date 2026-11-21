#ifndef _LISTA_H
#define _LISTA_H

struct ELEMENTO {
        int    tipo;/*    1. Cañon
                          2. Objeto
                          3. Fuente
                          4. Lente
                          5. Espejo
                          6. Apertura */
        int    x;
        int    y;
        int    foco;
        int    forma;     /*0 - Convexo         
                            1 - Concavo*/
        int    Ancho;
        
        struct ELEMENTO   *anterior;
        struct ELEMENTO   *siguiente;
        };

struct  ELEMENTO       *primero, *ultimo, *puntero, *actual, *tmp;
struct  ELEMENTO       *disponible_primero, *disponible_ultimo;

void Acomodar_Elemento(struct ELEMENTO *nuevo);
int Agregar_Elemento(int tipo, int x, int y, int foco, int forma, int Ancho);
void Borrar_Elemento(struct ELEMENTO *puntero, int liberar);

enum enmTYPE { CANNON = 1, OBJECT, SOURCE, LEN, MIRROR, APERTURE};

#endif
