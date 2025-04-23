#include "ColourButton.h"


ColourButton::ColourButton(Window* aParent, HINSTANCE hInstance,
                           const std::string& aName, BUTTON_STRATEGY aStrategy)
    : Button(aParent, hInstance, aName, aStrategy),
    mColour(RGB(0xff, 0xff, 0xff))
{
}


void ColourButton::Create(const RECT& oRect, COLORREF aColour)
{
    SetWindowPos(NULL, oRect.left, oRect.top, oRect.right - oRect.left, oRect.bottom - oRect.top, SWP_SHOWWINDOW);
    SetColour(aColour);
    InvalidateRect(GetHwnd(), NULL, false);
}


void ColourButton::SetColour(COLORREF aColour)
{
    if (mColour == aColour)
        return;

    mColour = aColour;
    LOGBRUSH lbrush = { BS_SOLID, mColour, 0 };
    DeleteObject(mColourBrush);
    mColourBrush = CreateBrushIndirect(&lbrush);

    RECT crect;
    GetClientRect(crect);
    InvalidateRect(GetHwnd(), &crect, false);
}


LRESULT ColourButton::OnPaint()
{
    PAINTSTRUCT ps;
    HDC dc = BeginPaint(GetHwnd(), &ps);

    RECT crect;
    GetClientRect(crect);

    LRESULT bResult = OnPaint(dc, crect);

    EndPaint(GetHwnd(), &ps);
    return bResult;
}


LRESULT ColourButton::OnPaint(HDC hDC, const RECT& aRect)
{
    FillRect(hDC, &aRect, mColourBrush);

    if (IsPressed()) {
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(230, 230, 0));
        HGDIOBJ oldpen = SelectObject(hDC, hPen);
        MoveToEx(hDC, aRect.left, aRect.top + 1, NULL);
        LineTo(hDC, aRect.right - 1, aRect.top + 1);
        LineTo(hDC, aRect.right - 1, aRect.bottom - 1);
        LineTo(hDC, aRect.left + 1, aRect.bottom - 1);
        LineTo(hDC, aRect.left + 1, aRect.top);

        HPEN hPen2 = CreatePen(PS_SOLID, 1, RGB(25, 25, 255));
        SelectObject(hDC, hPen2);
        MoveToEx(hDC, aRect.left + 2, aRect.top + 2, NULL);
        LineTo(hDC, aRect.right - 3, aRect.top + 2);
        LineTo(hDC, aRect.right - 3, aRect.bottom - 3);
        LineTo(hDC, aRect.left + 2, aRect.bottom - 3);
        LineTo(hDC, aRect.left + 2, aRect.top + 2);
        SelectObject(hDC, oldpen);
        DeleteObject(hPen);
        DeleteObject(hPen2);
    }

    return 0;
}


LRESULT ColourButton::OnSize(int iWidth, int iHeight)
{
    (void)iWidth;
    (void)iHeight;
    UpdateWindow(GetHwnd());
    return 0;
}


LRESULT ColourButton::OnWindowPosChanged(const WINDOWPOS& oPos)
{
    (void)oPos;
    return 0;
}
