#include "UserButtonBar.h"

#include "General/Profile.h"
#include "Resource/resource.h"
#include "Button/BitmapButton.h"


UserButtonBar::UserButtonBar(Window * aParent, HINSTANCE hInstance, const std::tstring & aName)
: RadioButtonBar(aParent, hInstance, aName, ButtonBar::LS_HorizontalFixedSize)
{
  std::tstring selected = Profile::GetConstInstance().GetUser().GetName();
  for (int i = 0; i < Profile::GetConstInstance().NumberOfUsers(); ++i)
  {
    AddButton(Profile::GetConstInstance().GetUser(i), i);
    if (selected == Profile::GetConstInstance().GetUser(i).GetName())
      SelectButton(i);
  }
}


void UserButtonBar::Update()
{
  for (int i = 0; i < Profile::GetConstInstance().NumberOfUsers(); ++i)
  {
    if (Profile::GetConstInstance().GetUser(i).GetName() != Profile::GetConstInstance().GetUser().GetName())
      continue;

    SelectButton(i);
    return;
  }
}


LRESULT UserButtonBar::OnButtonClick(Button * aButton)
{
  LRESULT result = RadioButtonBar::OnButtonClick(aButton);
  Profile::GetInstance().SetUser(Profile::GetConstInstance().GetUser(GetSelectedID()).GetName());
  return result;
}


void UserButtonBar::AddButton(const UserProfile & aProfile, int iID)
{
  VariableBitmapButton * button = new VariableBitmapButton(this, GetInstance(), true, aProfile.GetName(), Button::BS_ToggleButton);
  button->SetBitmap(aProfile.GetBitmapName());
  RadioButtonBar::AddButton(button, iID);
}
