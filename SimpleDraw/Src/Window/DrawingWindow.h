#ifndef SIMPLEDRAW_DRAWINGWINDOW_H__
#define SIMPLEDRAW_DRAWINGWINDOW_H__

#include <windows.h>

#include "General/General.h"
#include "General/Observer.h"
#include "Window/Window.h"

class DrawingAttributes;
class DrawingState;
class SpecialToolMenu;

class DrawingWindow: public MijnWindow<DrawingWindow>
{
public:
    DrawingWindow(Window* aParent, SpecialToolMenu& aMenu,
                  DrawingAttributes& anAttributes,
                  HINSTANCE hInstance, const std::string& aName = "DrawingWindow");
    ~DrawingWindow();

    LRESULT                 WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
    HDC                     GetHDC();
    HBITMAP                 GetBitmap() { return mHBitMap; }
    void                    SetBitmap(HBITMAP hBitmap);
    void                    ClearWindow();
    void                    SetState(Tekenen::DRAW_STATE aState);

    static bool             RegisterClass(HINSTANCE hInstance);

protected:
    static char* GetClassName() { return const_cast<char*>("MijnDrawingWindow"); }

private:
    DrawingWindow(const DrawingWindow&) = delete;
    DrawingWindow& operator=(const DrawingWindow&) = delete;

    // handle paint request
    LRESULT                 OnPaint();

    LRESULT                 OnChangeSize(int iWidth, int iHeight);

    LRESULT                 OnWindowPosChanged(const WINDOWPOS& oPos);

    HDC                     mHScreenDC;
    HDC                     mHMemoryDC;
    HBITMAP                 mHBitMap;
    HCURSOR                 mCursor;
    bool                    mDrawing;
    Tekenen::DRAW_STATE     mDrawingState;

    DrawingState* mState;
    DrawingAttributes& mAttributes;
    SpecialToolMenu& mSpecialToolMenu;
};


#endif // SIMPLEDRAW_DRAWINGWINDOW_H__
