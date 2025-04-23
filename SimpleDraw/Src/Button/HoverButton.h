#ifndef BUTTON_HOVERBUTTON_H__
#define BUTTON_HOVERBUTTON_H__


#include "Button/Button.h"
#include "Button/ButtonStrategy.h"


class HoverWindow;
class StampState;

class HoverButton: public Button
{
public:
    HoverButton(Window* aParent, HINSTANCE hInstance,
                StampState& mState,
                int aWidth, int aHeight,
                const std::string& aName = "HoverButton");
    ~HoverButton();

    void                    SetFile(const std::string& aFile);
    HoverWindow* CreateImage();

protected:

    LRESULT                 OnWindowPosChanged(const WINDOWPOS& oPos);
    LRESULT                 OnPaint();

private:
    HoverButton& operator=(const HoverButton&) = delete;
    HoverButton(const HoverButton&) = delete;

    int                     mButtonWidth;
    int                     mButtonHeight;
    int                     mBitmapWidth;
    int                     mBitmapHeight;
    float                   mScale;
    HBITMAP                 mHBitmap;
    StampState& mState;
};


#endif