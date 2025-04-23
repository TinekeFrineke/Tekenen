#include "FontButton.h"

#include <assert.h>


const char* FontButton::mTestString[2] = { "ABCDEF", "abcdef" };


FontButton::FontButton(Window* aParent, HINSTANCE hInstance,
                       const std::string& aFilename)
    : Button(aParent, hInstance, aFilename, BUTTON_STRATEGY::ToggleButton),
    mFont(nullptr)
{
}


FontButton::~FontButton()
{
    if (mFont != NULL)
        delete mFont;
}


bool FontButton::SetFont(const Font& aFont)
{
    if (mFont == NULL)
        mFont = new Font(aFont);
    else
        *mFont = aFont;

    ShowWindow(GetHwnd(), mFont->GetName().empty() ? SW_HIDE : SW_SHOW);
    return true;
}


LRESULT FontButton::OnPaint()
{
    PAINTSTRUCT ps;
    HDC dc = BeginPaint(GetHwnd(), &ps);

    RECT crect;
    GetClientRect(crect);
    HGDIOBJ oldfont = 0;
    if (mFont != NULL && mFont->GetHandle() != NULL)
        oldfont = SelectObject(dc, mFont->GetHandle());

    FillRect(dc, &crect, (HBRUSH)GetStockObject(WHITE_BRUSH));

    RECT rect = { 0, 0, 1, 1 };
    DrawText(dc, mTestString[0], -1, &rect, DT_CALCRECT | DT_SINGLELINE);

    int hmargin = (crect.right - crect.left - rect.right) / 2;
    int vmargin = (crect.bottom - crect.top - rect.bottom) / 2;
    rect.left += hmargin;
    rect.right += hmargin;
    rect.top += vmargin;
    rect.bottom += vmargin;

    DrawText(dc, mTestString[0], -1, &rect, DT_SINGLELINE);

    if (oldfont != 0)
        SelectObject(dc, oldfont);

    if (IsPressed()) {
        HPEN hPen = CreatePen(PS_SOLID, 2, RGB(230, 230, 0));
        HGDIOBJ oldpen = SelectObject(dc, hPen);
        MoveToEx(dc, crect.left, crect.top + 1, NULL);
        LineTo(dc, crect.right - 1, crect.top + 1);
        LineTo(dc, crect.right - 1, crect.bottom - 1);
        LineTo(dc, crect.left + 1, crect.bottom - 1);
        LineTo(dc, crect.left + 1, crect.top);

        HPEN hPen2 = CreatePen(PS_SOLID, 1, RGB(25, 25, 255));
        SelectObject(dc, hPen2);
        MoveToEx(dc, crect.left + 2, crect.top + 2, NULL);
        LineTo(dc, crect.right - 3, crect.top + 2);
        LineTo(dc, crect.right - 3, crect.bottom - 3);
        LineTo(dc, crect.left + 2, crect.bottom - 3);
        LineTo(dc, crect.left + 2, crect.top + 2);
        SelectObject(dc, oldpen);
        DeleteObject(hPen);
        DeleteObject(hPen2);
    }

    EndPaint(GetHwnd(), &ps);
    return 0;
}
