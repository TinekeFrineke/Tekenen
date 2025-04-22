#ifndef SUBTOOLWINDOW_THICKNESSPANEL_H__
#define SUBTOOLWINDOW_THICKNESSPANEL_H__


#include "Button/ColourButton.h"
#include "Control/RadioButtonBar.h"
#include "ThicknessControl.h"


class Controller;

class ThicknessButton : public ColourButton
{
public:
                              ThicknessButton(Window * aParent, HINSTANCE hInstance,
                                              const std::tstring & aName,
                                              Button::BUTTON_STRATEGY aStrategy);

  virtual void                SetThickness(int aThickness);
  int                         GetThickness() const { return mThickness; }
  virtual void                SetColour(COLORREF aRef);

protected:
  virtual LRESULT             OnPaint(HDC hDC, const RECT & aRect);

private:
  COLORREF                    mLineColour;
  int                         mThickness;
  HPEN                        mPen;
};


class ThicknessPanel : public RadioButtonBar, public ThicknessControl
{
public:
                              ThicknessPanel(::Window * aParent, HINSTANCE hInstance,
                                             const std::tstring & aName = _T("ThicknessPanel"));
                              ~ThicknessPanel();

  virtual int                 GetThickness() const;
  virtual void                SetThickness(int aThickness);
  void                        SetColour(COLORREF aRef);

  virtual bool                SetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx, int cy,
                                           UINT uFlags);

protected:
  virtual LRESULT             OnButtonClick(Button * aButton);

private:
  ThicknessButton *           mSelected;
};


#endif // SUBTOOLWINDOW_THICKNESSPANEL_H__