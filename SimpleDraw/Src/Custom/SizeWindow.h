#ifndef CUSTOM_SIZEWINDOW_H__
#define CUSTOM_SIZEWINDOW_H__


#include "General/Subject.h"
#include "Window/FrameWindow.h"


class Button;
//class ButtonBar;
template <class Target> class SizeBar;
class SingleBitmapButton;

class Preview : public FrameWindow
{
public:
                      Preview(Window *aWindow, HINSTANCE anInstance,
                              const std::string & aName = "Preview",
                              DWORD dwStyle = WS_CHILD | WS_BORDER)
                      : FrameWindow(aWindow, anInstance, aName, dwStyle) {}

  virtual void        Larger() = 0;
  virtual void        Smaller() = 0;
  
private:
};


class SizeWindow : public FrameWindow, public virtual Subject
{
public:
                      SizeWindow(Window * aParent, HINSTANCE anInstance);

  void                SetPreview(Preview * aPreview);
  void                Larger();
  void                Smaller();

protected:
  //virtual LRESULT     WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
  virtual LRESULT     OnWindowPosChanged(const WINDOWPOS & oPos);

  //LRESULT             OnButtonClick(Button * aButton);

private:
  Preview *           mPreview;
  //ButtonBar *         mButtonBar;
  SizeBar<SizeWindow> *
                      mSizeBar;
  //SingleBitmapButton *mLargerButton;
  //SingleBitmapButton *mSmallerButton;
};


#endif // CUSTOM_SIZEWINDOW_H__