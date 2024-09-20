#include "Cfg.h"

void add(HDC hdc, RECT client_rect) {
    if (size > masSize - 1)
        return;
    mas[size].left = (int)client_rect.right / 2 - SIZEX;
    mas[size].top = (int)client_rect.bottom / 2 - SIZEY;
    mas[size].right = (int)client_rect.right / 2 + SIZEX;
    mas[size].bottom = (int)client_rect.bottom / 2 + SIZEY;
    size++;

}

void drawRECT(HDC hdc) {
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, RGB(255, 255, 255));
    Rectangle(hdc, 0, 0, client_rect.right, client_rect.bottom);


    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, RGB(0, 255, 0));
    SelectObject(hdc, GetStockObject(DC_PEN));
    SetDCPenColor(hdc, RGB(0, 0, 0));
    for (int i = 0; i < size; ++i) {
        Rectangle(hdc, mas[i].left, mas[i].top, mas[i].right, mas[i].bottom);
    }

}