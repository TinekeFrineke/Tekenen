#ifndef COLORBUTTON_H__
#define COLORBUTTON_H__


#include "Button/Button.h"

class ColourButton : public Button
{
public:
                          ColourButton(Window * aParent, HINSTANCE hInstance,
                                       const std::tstring & aName, BUTTON_STRATEGY aStrategy);

  void                    Create(const RECT & oRect, COLORREF aColour);

  virtual void            SetColour(COLORREF aRef);
  COLORREF                GetColour() const { return mColour; }

protected:
  // handle paint request
  virtual LRESULT         OnPaint();
  virtual LRESULT         OnPaint(HDC hDC, const RECT & aRect);

private:
  LRESULT                 OnSize(int iWidth, int iHeight);
  virtual LRESULT         OnWindowPosChanged(const WINDOWPOS & oPos);
  HBRUSH                  mColourBrush;
  COLORREF                mColour;
  int                     mThickness;
};


#endif // COLORBUTTON_H__