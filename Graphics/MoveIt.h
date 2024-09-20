#pragma once
#include "framework.h"
#include <array>
#define masSize 3

class MoveIt
{
public:
    MoveIt(RECT client_rect);

    BOOL checkMouseCapture(POINT mousePos);


    void moveRECT(int x, int y);

    void drawRECT(HDC hdc, RECT client_rect);
  
    void RotateSprite(float _angle);

    void setBitmap(const HBITMAP bitmap);
    
private:
    const unsigned size_X = 150;
    const unsigned size_Y = 100;
    float angle = 0;
    HBITMAP hBitmap;
	std::array<POINT, 4> mas;
    std::array<POINT, 4> start_condition;
    BOOL PtInPolygon( POINT mousePos);


};

