#ifndef CUSTOM_HOVERBUTTONBAR_H__
#define CUSTOM_HOVERBUTTONBAR_H__

#include "Window/FrameWindow.h"

#include <vector>

class Button;
class ButtonBar;
class HoverButton;
class SingleBitmapButton;
class StampState;


class HoverButtonBar : public FrameWindow
{
public:
                              HoverButtonBar(Window * aParent, HINSTANCE hInstance,
                                             StampState & aState,
                                             int aRows, int aColumns);

  void                        SetFiles(const std::vector<std::tstring> & aFiles);

  void                        ScrollBack();
  void                        ScrollForward();

protected:
  virtual LRESULT             WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

  void                        FillButtons();

  LRESULT                     OnButtonClicked(Button * aButton);
  LRESULT                     OnWindowPosChanged(const WINDOWPOS & oPos);

private:
                              HoverButtonBar(const HoverButtonBar &);
  HoverButtonBar &            operator=(const HoverButtonBar &);

  void                        AddButton(ButtonBar * aButtonBar, HoverButton * aButton);

  StampState &                mState;
  std::vector<HoverButton *>  mButtons;
  std::vector<ButtonBar *>    mButtonBars;
  
  SingleBitmapButton *        mBack;
  SingleBitmapButton *        mForward;

  std::vector<std::tstring>   mFiles;
  int                         mCurrentIndex;
  int                         mRows;
  int                         mColumns;
};


#endif // CUSTOM_HOVERBUTTONBAR_H__