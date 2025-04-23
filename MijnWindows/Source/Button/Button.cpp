#include "Button.h"

#include <assert.h>
#include <stdio.h>

#include "buttonStrategy.h"

template<>
bool MijnWindow<Button>::mRegistered = false;


Button::Button(Window* aParent, HINSTANCE hInstance, const std::string& aName,
               BUTTON_STRATEGY aStrategy)
    : MijnWindow<Button>(aParent, hInstance, aName),
    mStrategy(NULL),
    mPressed(false)
{
    Create(GetClassName(), "Button", WS_CHILD);
    mStrategy = MakeStrategy(aStrategy);
}


void Button::SetStrategy(ButtonStrategy* aStrategy)
{
    if (aStrategy == NULL)
        return;

    ButtonStrategy* oldStrategy = mStrategy;
    mStrategy = aStrategy;
    delete oldStrategy;
}


Button::~Button()
{
    delete mStrategy;
}


LRESULT Button::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_LBUTTONDOWN:
            if (OnLeftButtonDown((int)wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)))
                return 0;
            break;

        case WM_LBUTTONUP:
            if (OnLeftButtonUp((int)wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)))
                return 0;
            break;
    }

    return MijnWindow<Button>::WindowProc(uMsg, wParam, lParam);
}


LRESULT Button::OnLeftButtonDown(int aVirtualKey, int anX, int anY)
{
    SetCapture(GetHwnd());
    return mStrategy->HandleButtonDown(aVirtualKey, anX, anY);
}


LRESULT Button::OnLeftButtonUp(int aVirtualKey, int anX, int anY)
{
    ReleaseCapture();
    GetParent()->SendMessage(WM_USER_BUTTON_CLICKED, 0, LPARAM((Button*)this));
    return mStrategy->HandleButtonUp(aVirtualKey, anX, anY);
}


bool Button::RegisterClass(HINSTANCE hInstance)
{
    LOGBRUSH lbrush = { BS_SOLID, RGB(0xff, 0xff, 0xff), 0 };

    WNDCLASS sWndClass;
    memset(&sWndClass, 0, sizeof(WNDCLASS));
    sWndClass.hbrBackground = CreateBrushIndirect(&lbrush);
    sWndClass.lpszClassName = GetClassName(); //L"MijnButton";
    sWndClass.lpfnWndProc = MijnWindow<Button>::WindowProc;
    sWndClass.hInstance = hInstance;
    sWndClass.hCursor = LoadCursor(0, IDC_HAND);

    return ::RegisterClass(&sWndClass) != NULL;
}


ButtonStrategy* Button::MakeStrategy(BUTTON_STRATEGY aStrategy)
{
    switch (aStrategy) {
        case BUTTON_STRATEGY::PushButton:
            return new PushButtonStategy(*this);
        case BUTTON_STRATEGY::ToggleButton:
            return new ToggleButtonStrategy(*this);
        case BUTTON_STRATEGY::CustomButton:
            return new CustomButtonStrategy(*this);
        default:
            assert(false);
            return new PushButtonStategy(*this);
    }
}


void Button::SetPressed(bool aPressed)
{
    if (mPressed == aPressed)
        return;

    mPressed = aPressed;
    InvalidateRect(GetHwnd(), NULL, false);
}
