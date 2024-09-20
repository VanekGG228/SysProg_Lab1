#include "MoveIt.h"
#define M_PI           3.14159265358979323846

MoveIt::MoveIt(RECT client_rect)
{
    int centerX = client_rect.right / 2;
    int centerY = client_rect.bottom / 2;


    start_condition[0].x = mas[0].x = centerX - size_X;
    start_condition[0].y = mas[0].y = centerY - size_Y;

    start_condition[1].x = mas[1].x = centerX + size_X;
    start_condition[1].y = mas[1].y = centerY - size_Y;


    start_condition[3].x = mas[3].x = centerX - size_X;
    start_condition[3].y = mas[3].y = centerY + size_Y;

    start_condition[2].x = mas[2].x = centerX + size_X;
    start_condition[2].y = mas[2].y = centerY + size_Y;
}

BOOL MoveIt::checkMouseCapture(POINT mousePos)
{
        if (PtInPolygon(mousePos)) {

            return true;

        }
    return false;
}

void MoveIt::moveRECT(int x, int y)
{

    for (int i = 0; i < 4;++i) {
        start_condition[i].x += x;
        start_condition[i].y += y;
        mas[i].x += x;
        mas[i].y += y;
    }
}

void MoveIt::drawRECT(HDC hdc, RECT client_rect)
{
    SelectObject(hdc, GetStockObject(DC_BRUSH));
    SetDCBrushColor(hdc, RGB(255, 255, 255));
    Rectangle(hdc, 0, 0, client_rect.right, client_rect.bottom);
    if (hBitmap) {
        HDC hdcMem = CreateCompatibleDC(hdc);
        HBITMAP oldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

        const float centerX = start_condition[0].x +size_X;
        const float centerY = start_condition[0].y + size_Y;
        const float radian = angle;

        XFORM xForm;
        xForm.eM11 = cos(radian);
        xForm.eM12 = sin(radian);
        xForm.eM21 = -sin(radian);
        xForm.eM22 = cos(radian);
        xForm.eDx = centerX;
        xForm.eDy = centerY;

        SetGraphicsMode(hdc, GM_ADVANCED);
        SetWorldTransform(hdc, &xForm);

        const int bitmapX = -1 * size_X;
        const int bitmapY = -1 * size_Y;

        BitBlt(hdc, bitmapX, bitmapY, 2 * size_X, 2 * size_Y, hdcMem, 0, 0, SRCCOPY);
        ModifyWorldTransform(hdc, NULL, MWT_IDENTITY);

        SelectObject(hdcMem, oldBitmap);
        DeleteDC(hdcMem);
    }
    else {
        HBRUSH blueBrush = CreateSolidBrush(RGB(0, 255, 0));
        HPEN bluePen = CreatePen(PS_SOLID, 5, RGB(0, 0, 0));

        HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, blueBrush);
        HPEN oldPen = (HPEN)SelectObject(hdc, bluePen);

        Polygon(hdc, mas.data(), 4);

        SelectObject(hdc, oldBrush);
        SelectObject(hdc, oldPen);

        DeleteObject(blueBrush);
        DeleteObject(bluePen);
    }
    
}

void MoveIt::RotateSprite(float _angle)
{
    angle += _angle;
   
    double centerX = start_condition[0].x + size_X;
    double centerY = start_condition[0].y + size_Y;

    for (int i = 0; i < 4; ++i) {
        double newX = centerX + (start_condition[i].x - centerX) * cos(angle) - (start_condition[i].y - centerY) * sin(angle);
        double newY = centerY + (start_condition[i].x - centerX) * sin(angle) + (start_condition[i].y - centerY) * cos(angle);
        mas[i].x = newX;
        mas[i].y = newY;
    }
    if (angle > 2*M_PI) {
        angle -= 2*M_PI;
    }
}

void MoveIt::setBitmap(const HBITMAP bitmap)
{
        if (hBitmap) {
            DeleteObject(hBitmap);
        }
        hBitmap = bitmap;

        HDC hdcScreen = GetDC(NULL);
        HDC hdcMemSource = CreateCompatibleDC(hdcScreen);
        HDC hdcMemDest = CreateCompatibleDC(hdcScreen);

        BITMAP bitmapInfo;
        GetObject(hBitmap, sizeof(BITMAP), &bitmapInfo);

        HBITMAP hScaledBitmap = CreateCompatibleBitmap(hdcScreen, 2 * size_X, 2 * size_Y);
        HBITMAP oldSourceBitmap = (HBITMAP)SelectObject(hdcMemSource, hBitmap);
        HBITMAP oldDestBitmap = (HBITMAP)SelectObject(hdcMemDest, hScaledBitmap);

        StretchBlt(
            hdcMemDest,
            0, 0, 2 * size_X, 2 * size_Y,
            hdcMemSource,
            0, 0, bitmapInfo.bmWidth, bitmapInfo.bmHeight,
            SRCCOPY
        );

        SelectObject(hdcMemSource, oldSourceBitmap);
        SelectObject(hdcMemDest, oldDestBitmap);
        DeleteDC(hdcMemSource);
        DeleteDC(hdcMemDest);
        ReleaseDC(NULL, hdcScreen);

        hBitmap = hScaledBitmap;
}

BOOL MoveIt::PtInPolygon(POINT mousePos)
{
    int left = start_condition[0].x;
    int right = start_condition[0].x + 2 * size_X;
    int top = start_condition[0].y;
    int bottom = start_condition[0].y + 2 * size_Y;

    if ((mousePos.x <= right) && (mousePos.x >= left) && (mousePos.y >= top) && (mousePos.y <= bottom)) {
        return true;
    }
    return false;
}
