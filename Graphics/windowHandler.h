#pragma once
#include "framework.h"
#include "Graphics.h"
#include <windowsx.h>
#include <utility>
#include <string>
#include "MoveIt.h"
#include <commdlg.h>


class WindowHandler {
private:
    HWND hWnd;
    bool isDragging;
    POINT prevMousePos;
    MoveIt* rects;
    int speed = 10;
    float angle = 0.5;
public:
    WindowHandler(HWND windowHandle, MoveIt* rectHandler);

    void closeWin();

    void ButtonDown(LPARAM lParam);

    void ButtonUp();

    void MouseMove(LPARAM lParam);

    void MouseWheel(WPARAM wParam);

    void moveUp();

    void moveDown();

    void moveLeft();

    void moveRight();

    void rotate();

    void setSpeed(int newSpeed);

    std::wstring OpenFileDialog(HWND hwnd);

    void setImage();

    void OnPaint(HDC hdc, RECT clientRect);

};
