#ifndef SIMPLEDRAW_COLOURWINDOW_H__
#define SIMPLEDRAW_COLOURWINDOW_H__


#include <windows.h>

#include <vector>

#include "General/Subject.h"
#include "Window/Window.h"

class ColourButton;
class DrawingAttributes;

class ColourWindow : public GenericWindow, public virtual Subject
{
public:
                          ColourWindow(Window * aParent, DrawingAttributes &  anAttributes, HINSTANCE hInstance,
                                       const std::tstring & aName = _T("ColourWindow"));
                          ~ColourWindow();

  LRESULT                 WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

  COLORREF                GetColour() const;

  static bool             RegisterClass(HINSTANCE hInstance);

protected:
  static TCHAR *          GetClassName() { return _T("MijnColourWindow"); }

private:
  // Disable copying
                          ColourWindow(const ColourWindow & aWindow);
  // Disable assignment
  ColourWindow &          operator=(const ColourWindow &);

  LRESULT                 OnButtonClicked(ColourButton * aButton);
  LRESULT                 OnWindowPosChanged(const WINDOWPOS & oPos);

  std::vector<ColourButton *> mButtonList;
  ColourButton *          mPressedButton;
  DrawingAttributes &     mAttributes;
};


#endif // SIMPLEDRAW_COLOURWINDOW_H__
