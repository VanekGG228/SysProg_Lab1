#include "windowHandler.h"

WindowHandler::WindowHandler(HWND windowHandle, MoveIt* rectHandler): hWnd(windowHandle), rects(rectHandler), isDragging(false)
{
    prevMousePos.x = 0;
    prevMousePos.y = 0;
}

void WindowHandler::closeWin()
{
    int result = MessageBox(hWnd, L"Вы действительно хотите выйти?", L"Подтверждение", MB_YESNO | MB_ICONQUESTION);
    if (result == IDYES) {
        DestroyWindow(hWnd);
    }
}

void WindowHandler::ButtonDown(LPARAM lParam)
{
    POINT mousePos;
    mousePos.x = LOWORD(lParam);
    mousePos.y = HIWORD(lParam);
    isDragging = rects->checkMouseCapture(mousePos);
    if (isDragging) {
        prevMousePos = mousePos;
    }
}

void WindowHandler::ButtonUp()
{
    if (isDragging) {
        isDragging = false;
    }
}

void WindowHandler::MouseMove(LPARAM lParam)
{
    if (isDragging) {
        POINT mousePos;
        mousePos.x = LOWORD(lParam);
        mousePos.y = HIWORD(lParam);

        int offsetX = mousePos.x - prevMousePos.x;
        int offsetY = mousePos.y - prevMousePos.y;

        rects->moveRECT(offsetX, offsetY);
        prevMousePos = mousePos;
    }
}

void WindowHandler::MouseWheel(WPARAM wParam)
{
    int zDelta = GET_WHEEL_DELTA_WPARAM(wParam);
    bool shiftPressed = (GetKeyState(VK_SHIFT) & 0x8000) != 0;

    if (zDelta > 0) {
        if (!shiftPressed) {
            rects->moveRECT(0, -speed);
        }
        else {
            rects->moveRECT(-speed, 0);
        }
    }
    else {
        if (!shiftPressed) {
            rects->moveRECT(0, speed);
        }
        else {
            rects->moveRECT(speed, 0);
        }
    }
}

void WindowHandler::moveUp()
{
    rects->moveRECT(0, -speed);
}

void WindowHandler::moveDown()
{
    rects->moveRECT(0, speed);
}

void WindowHandler::moveLeft()
{
    rects->moveRECT(-speed, 0);
}

void WindowHandler::moveRight()
{
    rects->moveRECT(speed, 0);
}

void WindowHandler::rotate()
{
    rects->RotateSprite(angle);
}

void WindowHandler::setSpeed(int newSpeed)
{
    speed = newSpeed;
}


std::wstring WindowHandler::OpenFileDialog(HWND hwnd)
{

        OPENFILENAME ofn;
        wchar_t szFile[260] = { 0 };

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFile = szFile;
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = L"Image Files\0*.bmp\0All Files\0*.*\0";
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;


        if (GetOpenFileName(&ofn) == TRUE)
        {
            return ofn.lpstrFile;
        }

        return L"";
}

void WindowHandler::setImage()
{
    std::wstring filePath = OpenFileDialog(hWnd);
    if (!filePath.empty()) {
        auto hNewBitmap = (HBITMAP)LoadImage(NULL, filePath.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        if (hNewBitmap != nullptr) {
            rects->setBitmap(hNewBitmap);
        }
    }
}

void WindowHandler::OnPaint(HDC hdc, RECT clientRect)
{
    GetClientRect(hWnd, &clientRect);
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP hBmp = CreateCompatibleBitmap(hdc, clientRect.right, clientRect.bottom);
    HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, hBmp);

    FillRect(memDC, &clientRect, (HBRUSH)(COLOR_WINDOW + 1));
    rects->drawRECT(memDC, clientRect);
    BitBlt(hdc, 0, 0, clientRect.right, clientRect.bottom, memDC, 0, 0, SRCCOPY);

    SelectObject(memDC, oldBmp);
    DeleteObject(hBmp);
    DeleteDC(memDC);
}
