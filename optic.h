int            numero_cursor = 0;

int            OPCION;
int            GRABAR;
int            DIBUJAR;
int            CONTINUAR;
int            ARRASTRAR;
int            PEGAR;

int            FOCO = 0;
int            LONGITUD = 0;

int            Ancho = 15;
int            Foco = 30;
int            Convexo = 0;

int            PRIMERAVEZ = 0;



struct RAYO {
       int  x;
       int  y;
       float m;
       int diverge;
       } rayo_1, rayo_2, rayo_3;

int xPos, yPos, xActual;
int tmp2;
int tmp_x, tmp_y;
int Alto;
