#include "GeneralToolMenu.h"

#include "Button/BitmapButton.h"

#include "General/Controller.h"
#include "General/Profile.h"
#include "Resource/resource.h"

GeneralToolMenu::GeneralToolMenu(Window* aParent, Controller& aController,
                                 HINSTANCE hInstance, const std::string& aName)
    : RadioButtonBar(aParent, hInstance, aName, LAYOUT_STRATEGY::VerticalFixedSize),
    mController(aController),
    mDrawButton(nullptr),
    mStampButton(nullptr),
    mLetterButton(nullptr),
    mFillButton(nullptr),
    mSmudgeButton(nullptr),
    mAirbrushButton(nullptr)
{
    mDrawButton = new SingleBitmapButton(this, hInstance, true, "DrawButton", Button::BUTTON_STRATEGY::ToggleButton);
    mStampButton = new SingleBitmapButton(this, hInstance, true, "StampButton", Button::BUTTON_STRATEGY::ToggleButton);
    mLetterButton = new SingleBitmapButton(this, hInstance, true, "LetterButton", Button::BUTTON_STRATEGY::ToggleButton);
    mFillButton = new SingleBitmapButton(this, hInstance, true, "FillButton", Button::BUTTON_STRATEGY::ToggleButton);
    mSmudgeButton = new SingleBitmapButton(this, hInstance, true, "SmudgeButton", Button::BUTTON_STRATEGY::ToggleButton);
    mAirbrushButton = new SingleBitmapButton(this, hInstance, true, "AirbrushButton", Button::BUTTON_STRATEGY::ToggleButton);

    mDrawButton->SetBitmap(Profile::GetInstance().GetResourceDirectory() + "\\Pencil.bmp");
    mStampButton->SetBitmap(Profile::GetInstance().GetResourceDirectory() + "\\Stamp.bmp");
    mLetterButton->SetBitmap(Profile::GetInstance().GetResourceDirectory() + "\\font.bmp");
    mFillButton->SetBitmap(Profile::GetInstance().GetResourceDirectory() + "\\emmer.bmp");
    mSmudgeButton->SetBitmap(Profile::GetInstance().GetResourceDirectory() + "\\Smudge.bmp");
    mAirbrushButton->SetBitmap(Profile::GetInstance().GetResourceDirectory() + "\\Airbrush.bmp");

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


LRESULT GeneralToolMenu::OnButtonClick(Button* aButton)
{
    if (aButton == mDrawButton)
        mController.SetState(Tekenen::DRAW_STATE::FreeDraw);
    else if (aButton == mStampButton)
        mController.SetState(Tekenen::DRAW_STATE::Stamp);
    else if (aButton == mLetterButton)
        mController.SetState(Tekenen::DRAW_STATE::Letter);
    else if (aButton == mFillButton)
        mController.SetState(Tekenen::DRAW_STATE::Fill);
    else if (aButton == mSmudgeButton)
        mController.SetState(Tekenen::DRAW_STATE::Smudge);
    else if (aButton == mAirbrushButton)
        mController.SetState(Tekenen::DRAW_STATE::Airbrush);

    RadioButtonBar::SelectButton(aButton);
    return 0;
}
