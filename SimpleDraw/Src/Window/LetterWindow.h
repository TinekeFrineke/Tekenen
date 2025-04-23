#ifndef WINDOW_LETTERWINDOW_H__
#define WINDOW_LETTERWINDOW_H__


#include "General/Font.h"
#include "Window/Window.h"


class DrawingAttributes;
class DrawingWindow;

class LetterWindow: public GenericWindow
{
public:
    LetterWindow(DrawingWindow& aParent, HINSTANCE hInstance, const Font& aFont,
                 const DrawingAttributes& anAttributes);
    ~LetterWindow();

    virtual LRESULT         WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void                    SetFont(const Font& aFont);
    void                    Finalize();

protected:
    LRESULT                 OnPaint();
    LRESULT                 OnChar(char c);

    void                    UpdateWindowSize();
    void                    DrawText(HDC aDc);

private:
    LetterWindow(const LetterWindow&) = delete;
    LetterWindow& operator=(const LetterWindow&) = delete;

    DrawingWindow& mWindow;
    const DrawingAttributes& mAttributes;

    HWND                    mOldFocusWnd;
    Font                    mFont;
    std::string            mText;
};


#endif // WINDOW_LETTERWINDOW_H__