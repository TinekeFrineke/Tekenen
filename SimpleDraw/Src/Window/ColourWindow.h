#ifndef SIMPLEDRAW_COLOURWINDOW_H__
#define SIMPLEDRAW_COLOURWINDOW_H__


#include <windows.h>

#include <vector>

#include "General/Subject.h"
#include "Window/Window.h"

class ColourButton;
class DrawingAttributes;

class ColourWindow: public GenericWindow, public virtual Subject
{
public:
    ColourWindow(Window* aParent, DrawingAttributes& anAttributes, HINSTANCE hInstance,
                 const std::string& aName = "ColourWindow");
    ~ColourWindow();

    LRESULT                 WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

    COLORREF                GetColour() const;

    static bool             RegisterClass(HINSTANCE hInstance);

protected:
    static char* GetClassName() { return const_cast<char*>("MijnColourWindow"); }

private:
    ColourWindow(const ColourWindow& aWindow) = delete;
    ColourWindow& operator=(const ColourWindow&) = delete;

    LRESULT                 OnButtonClicked(ColourButton* aButton);
    LRESULT                 OnWindowPosChanged(const WINDOWPOS& oPos);

    std::vector<ColourButton*> mButtonList;
    ColourButton* mPressedButton;
    DrawingAttributes& mAttributes;
};


#endif // SIMPLEDRAW_COLOURWINDOW_H__
