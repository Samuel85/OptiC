#include <windows.h>

HWND           btnCannon;
HWND           btnObject;
HWND           btnSource;
HWND           btnClearAll;
HWND           btnLens;
HWND           btnMirror;
HWND           btnClearLast;
HWND           btnExit;

HDC            hdc;
HDC            memoria;
PAINTSTRUCT    ps;
RECT           rc;
HPEN           pen_solid;
HPEN           pen_dot;
HPEN           pen_black;
HPEN           pen_light;
HPEN           pen_tmp;

HBRUSH         brush_color;
HBRUSH         brush_black;
HBRUSH         tmp_brush;
HBRUSH         brush_green;

LOGBRUSH       black;
LOGBRUSH       color;
LOGBRUSH       green;

HBITMAP        hMemBmp;
HBITMAP        hOldBmp;

HCURSOR        cursor_normal;
HCURSOR        cursor_cruz;
HCURSOR        cursor_mano;

int            Botones_xPos = 100;
int            Botones_yPos = 20;

#define BUTTON(x) (LOWORD(wParam) == 1 && HIWORD(wParam) == BN_CLICKED && (HWND) lParam == x)
#define CREATE_BUTTON(title, x1, y1, x2, y2) (CreateWindow (L"button",title,WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,x1, y1,	x2, y2,hwnd, (HMENU) 1,((LPCREATESTRUCT) lParam)->hInstance,NULL))

