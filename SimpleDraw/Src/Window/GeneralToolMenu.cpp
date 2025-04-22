#include "GeneralToolMenu.h"

#include "Button/BitmapButton.h"

#include "General/Controller.h"
#include "Resource/resource.h"

GeneralToolMenu::GeneralToolMenu(Window * aParent, Controller & aController,
                               HINSTANCE hInstance, const std::tstring & aName)
: RadioButtonBar  (aParent, hInstance, aName, LS_VerticalFixedSize),
  mController     (aController),
  mDrawButton     (NULL),
  mStampButton    (NULL),
  mLetterButton   (NULL),
  mFillButton     (NULL),
  mSmudgeButton   (NULL),
  mAirbrushButton (NULL)
{
  mDrawButton     = new SingleBitmapButton(this, hInstance, true, _T("DrawButton"),     Button::BS_ToggleButton);
  mStampButton    = new SingleBitmapButton(this, hInstance, true, _T("StampButton"),    Button::BS_ToggleButton);
  mLetterButton   = new SingleBitmapButton(this, hInstance, true, _T("LetterButton"),   Button::BS_ToggleButton);
  mFillButton     = new SingleBitmapButton(this, hInstance, true, _T("FillButton"),     Button::BS_ToggleButton);
  mSmudgeButton   = new SingleBitmapButton(this, hInstance, true, _T("SmudgeButton"),   Button::BS_ToggleButton);
  mAirbrushButton = new SingleBitmapButton(this, hInstance, true, _T("AirbrushButton"), Button::BS_ToggleButton);

  mDrawButton->SetBitmap(IDB_PENCIL);
  mStampButton->SetBitmap(IDB_STAMP);
  mLetterButton->SetBitmap(IDB_FONT);
  mFillButton->SetBitmap(IDB_EMMER);
  mSmudgeButton->SetBitmap(IDB_SMUDGE);
  mAirbrushButton->SetBitmap(IDB_AIRBRUSH);

  AddButton(mDrawButton, IDB_PENCIL);
  AddButton(mStampButton, IDB_STAMP);
  AddButton(mLetterButton, IDB_FONT);
  AddButton(mFillButton, IDB_EMMER);
  AddButton(mSmudgeButton, IDB_SMUDGE);
  AddButton(mAirbrushButton, IDB_AIRBRUSH);

  RadioButtonBar::SelectButton(mDrawButton);
}


GeneralToolMenu::~GeneralToolMenu()
{
}


LRESULT GeneralToolMenu::OnButtonClick(Button * aButton)
{
  if (aButton == mDrawButton)
    mController.SetState(Tekenen::DS_FreeDraw);
  else if (aButton == mStampButton)
    mController.SetState(Tekenen::DS_Stamp);
  else if (aButton == mLetterButton)
    mController.SetState(Tekenen::DS_Letter);
  else if (aButton == mFillButton)
    mController.SetState(Tekenen::DS_Fill);
  else if (aButton == mSmudgeButton)
    mController.SetState(Tekenen::DS_Smudge);
  else if (aButton == mAirbrushButton)
    mController.SetState(Tekenen::DS_Airbrush);

  RadioButtonBar::SelectButton(aButton);
  return 0;
}
