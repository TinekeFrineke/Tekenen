#ifndef WINDOW_FRAMEWINDOW_H__
#define WINDOW_FRAMEWINDOW_H__

#include "Window/Window.h"


class FrameWindow: public MijnWindow<FrameWindow>
{
public:
    FrameWindow(Window* aParent, HINSTANCE hInstance,
                const std::string& aName,
                DWORD aStyle = WS_CHILD | WS_BORDER/* | WS_VISIBLE*/);
    ~FrameWindow() override;

    void                    SetColour(COLORREF aColour);

    static bool             RegisterClass(HINSTANCE hInstance);

protected:
    static char* GetClassName() { return const_cast<char*>("MijnFrameWindow"); }

private:
    LRESULT  OnPaint();
    COLORREF mColour;
};


#endif // WINDOW_FRAMEWINDOW_H__
