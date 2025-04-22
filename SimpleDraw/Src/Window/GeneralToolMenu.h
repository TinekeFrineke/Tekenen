#ifndef WINDOW_MAINTOOLWINDOW_H__
#define WINDOW_MAINTOOLWINDOW_H__


//#include "FrameWindow.h"
#include "Control/RadioButtonBar.h"

class Button;
class Controller;
class SingleBitmapButton;

class GeneralToolMenu : public RadioButtonBar
{
public:
                          GeneralToolMenu(Window * aParent, Controller & aController, HINSTANCE hInstance,
                                          const std::tstring & aName = _T("GeneralToolMenu"));
                          ~GeneralToolMenu();

protected:

private:
  virtual LRESULT         OnButtonClick(Button * aButton);

  GeneralToolMenu &       operator=(const GeneralToolMenu &);
                          GeneralToolMenu(const GeneralToolMenu &);

  SingleBitmapButton *    mDrawButton;
  SingleBitmapButton *    mStampButton;
  SingleBitmapButton *    mLetterButton;
  SingleBitmapButton *    mFillButton;
  SingleBitmapButton *    mSmudgeButton;
  SingleBitmapButton *    mAirbrushButton;
  Controller &            mController;
};


#endif // WINDOW_MAINTOOLWINDOW_H__