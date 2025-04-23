#include "BitmapButton.h"


BitmapButtonBase::BitmapButtonBase(Window* aParent, HINSTANCE hInstance,
                                   bool bDeformAllowed, const std::string& aName,
                                   BUTTON_STRATEGY aStrategy)
    : Button(aParent, hInstance, aName, aStrategy),
    mDeformAllowed(bDeformAllowed)
{
}


BitmapButtonBase::~BitmapButtonBase()
{
}


bool BitmapButtonBase::MyLoadBitmap(LPCTSTR bitmapname, BmpInfo& anInfo)
{
    if (anInfo.mHBitmap != NULL)
        DeleteObject(anInfo.mHBitmap);

    anInfo.mHBitmap = (HBITMAP)LoadImage(GetInstance(), bitmapname, IMAGE_BITMAP,
                                          0, 0,
                                          LR_LOADFROMFILE);

    //anInfo.mHBitmap = ::LoadBitmap(GetInstance(), bitmapname);
    if (anInfo.mHBitmap == NULL) {
        OutputDebugString((std::string("Could not load bitmap ") + bitmapname + '\n').c_str());
        return false;
    }
    else {
        OutputDebugString((std::string("Loaded bitmap ") + bitmapname + '\n').c_str());
    }

    BITMAP bitmap;
    GetObject(anInfo.mHBitmap, sizeof(bitmap), &bitmap);

    anInfo.mWidth = bitmap.bmWidth;
    anInfo.mHeight = bitmap.bmHeight;

    return true;
}


bool BitmapButtonBase::Show(HDC aDc, const BmpInfo& anInfo)
{
    RECT rect;
    GetClientRect(rect);

    HDC hDCMem = CreateCompatibleDC(aDc);

    SelectObject(hDCMem, anInfo.mHBitmap);

    if (!IsDeformAllowed())
    {
        float hscale = float(rect.right - rect.left) / anInfo.mWidth;
        float vscale = float(rect.bottom - rect.top) / anInfo.mHeight;

        if (hscale < vscale)
        {
            // scale vertically
            int height = int(hscale * anInfo.mHeight);
            rect.top += (rect.bottom - rect.top - height) / 2;
            rect.bottom = rect.top + height;
        }
        else
        {
            int width = int(vscale * anInfo.mWidth);
            rect.left += (rect.right - rect.left - width) / 2;
            rect.right = rect.left + width;
        }
    }

    StretchBlt(aDc, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
               hDCMem, 0, 0, anInfo.mWidth, anInfo.mHeight, SRCCOPY);

    DeleteObject(hDCMem);
    return true;
}


void BitmapButtonBase::DrawRectangle(HDC aDC, const RECT& aRect, COLORREF aColour)
{
    HPEN hPen = CreatePen(PS_SOLID, 2, aColour);
    HGDIOBJ oldpen = SelectObject(aDC, hPen);
    MoveToEx(aDC, aRect.left + 1, aRect.top + 1, NULL);
    LineTo(aDC, aRect.right - 1, aRect.top + 1);
    LineTo(aDC, aRect.right - 1, aRect.bottom - 1);
    LineTo(aDC, aRect.left + 1, aRect.bottom - 1);
    LineTo(aDC, aRect.left + 1, aRect.top + 1);
    SelectObject(aDC, oldpen);
    DeleteObject(hPen);
}


SingleBitmapButton::SingleBitmapButton(Window* aParent, HINSTANCE hInstance,
                                       bool bDeformAllowed, const std::string& aName,
                                       BUTTON_STRATEGY aStrategy)
    : BitmapButtonBase(aParent, hInstance, bDeformAllowed, aName, aStrategy),
    mSelectedColour(RGB(0xff, 0xff, 0x00))
{
}


SingleBitmapButton::~SingleBitmapButton()
{
    if (mBitmap.mHBitmap != NULL)
        DeleteObject(mBitmap.mHBitmap);
}


void SingleBitmapButton::SetBitmap(const std::string& bitmap)
{
    MyLoadBitmap(bitmap.c_str(), mBitmap);
    InvalidateRect(GetHwnd(), NULL, false);
}


void SingleBitmapButton::SetSelectedColour(COLORREF aColour)
{
    mSelectedColour = aColour;
    if (IsPressed())
        InvalidateRect(GetHwnd(), NULL, false);
}


LRESULT SingleBitmapButton::OnPaint()
{
    PAINTSTRUCT ps;
    HDC dc = BeginPaint(GetHwnd(), &ps);

    Show(dc, mBitmap);

    RECT rect;
    GetClientRect(rect);

    if (IsPressed())
        DrawRectangle(dc, rect, mSelectedColour);

    EndPaint(GetHwnd(), &ps);
    return 0;
}


DoubleBitmapButton::DoubleBitmapButton(Window* aParent, HINSTANCE hInstance,
                                       bool bDeformAllowed, const std::string& aName,
                                       BUTTON_STRATEGY aStrategy)
    : BitmapButtonBase(aParent, hInstance, bDeformAllowed, aName, aStrategy)
{
}


DoubleBitmapButton::~DoubleBitmapButton()
{
    if (mBitmap.mHBitmap != NULL)
        DeleteObject(mBitmap.mHBitmap);
    if (mPressedBitmap.mHBitmap != NULL)
        DeleteObject(mPressedBitmap.mHBitmap);
}


void DoubleBitmapButton::SetBitmap(const std::string& bitmap, const std::string& bitmapPressed)
{
    MyLoadBitmap(bitmap.c_str(), mBitmap);
    MyLoadBitmap(bitmapPressed.c_str(), mPressedBitmap);

    InvalidateRect(GetHwnd(), NULL, false);
}


LRESULT DoubleBitmapButton::OnPaint()
{
    PAINTSTRUCT ps;
    HDC dc = BeginPaint(GetHwnd(), &ps);

    if (IsPressed())
        Show(dc, mPressedBitmap);
    else
        Show(dc, mBitmap);

    EndPaint(GetHwnd(), &ps);
    return 0;
}


VariableBitmapButton::VariableBitmapButton(Window* aParent, HINSTANCE hInstance,
                                           bool bDeformAllowed, const std::string& aName,
                                           BUTTON_STRATEGY aStrategy)
    : BitmapButtonBase(aParent, hInstance, bDeformAllowed, aName, aStrategy),
    mSelectedColour(RGB(0xff, 0xff, 0x00))
{
}


VariableBitmapButton::~VariableBitmapButton()
{
    if (mBitmap.mHBitmap != NULL)
        DeleteObject(mBitmap.mHBitmap);
}


void VariableBitmapButton::SetBitmap(const std::string& aBitmap)
{
    RECT rect;
    GetClientRect(rect);

    if (mBitmap.mHBitmap != NULL)
        DeleteObject(mBitmap.mHBitmap);

    mBitmap.mHBitmap = (HBITMAP)LoadImage(GetInstance(), aBitmap.c_str(), IMAGE_BITMAP,
                                          0, 0,
                                          LR_LOADFROMFILE);
    if (mBitmap.mHBitmap != NULL)
    {
        BITMAP bitmap;
        GetObject(mBitmap.mHBitmap, sizeof(bitmap), &bitmap);

        mBitmap.mWidth = bitmap.bmWidth;
        mBitmap.mHeight = bitmap.bmHeight;
    }
    else
    {
        mBitmap.mWidth = 0;
        mBitmap.mHeight = 0;
    }

    InvalidateRect(GetHwnd(), NULL, false);
}


void VariableBitmapButton::SetSelectedColour(COLORREF aColour)
{
    mSelectedColour = aColour;
    if (IsPressed())
        InvalidateRect(GetHwnd(), NULL, false);
}


LRESULT VariableBitmapButton::OnPaint()
{
    PAINTSTRUCT ps;
    HDC dc = BeginPaint(GetHwnd(), &ps);

    RECT rect;
    GetClientRect(rect);

    DrawRectangle(dc, rect, RGB(0xff, 0xff, 0xff));

    Show(dc, mBitmap);

    if (IsPressed())
        DrawRectangle(dc, rect, mSelectedColour);

    EndPaint(GetHwnd(), &ps);
    return 0;
}
