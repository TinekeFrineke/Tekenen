#ifndef BUTTON_HOVERBUTTON_H__
#define BUTTON_HOVERBUTTON_H__


#include "Button/Button.h"
#include "Button/ButtonStrategy.h"


class HoverWindow;
class StampState;

class HoverButton : public Button
{
public:
                          HoverButton(Window * aParent, HINSTANCE hInstance,
                                      StampState & mState,
                                      int aWidth, int aHeight,
                                      const std::tstring & aName = _T("HoverButton"));
                          ~HoverButton();

  void                    SetFile(const std::tstring & aFile);
  HoverWindow *           CreateImage();

protected:

  LRESULT                 OnWindowPosChanged(const WINDOWPOS & oPos);
  LRESULT                 OnPaint();

private:
  HoverButton &           operator=(const HoverButton &);
                          HoverButton(const HoverButton &);

  int                     mButtonWidth;
  int                     mButtonHeight;
  int                     mBitmapWidth;
  int                     mBitmapHeight;
  float                   mScale;
  HBITMAP                 mHBitmap;
  StampState &            mState;
};


#endif