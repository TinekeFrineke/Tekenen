#ifndef WINDOW_FRAMEWINDOW_H__
#define WINDOW_FRAMEWINDOW_H__

#include "Window/Window.h"


class FrameWindow : public MijnWindow<FrameWindow>
{
public:
                          FrameWindow(Window * aParent, HINSTANCE hInstance,
                                      const std::tstring & aName,
                                      DWORD aStyle = WS_CHILD | WS_BORDER/* | WS_VISIBLE*/);
                          ~FrameWindow();

  void                    SetColour(COLORREF aColour);

  static bool             RegisterClass(HINSTANCE hInstance);

protected:
  static TCHAR *          GetClassName() { return const_cast<TCHAR *>(_T("MijnFrameWindow")); }

private:
  /// handle paint request
  LRESULT                 OnPaint();

 ///
  COLORREF                mColour;
};


#endif // WINDOW_FRAMEWINDOW_H__
