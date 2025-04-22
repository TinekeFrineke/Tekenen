#include "RadioButtonBar.h"

#include "Button/Button.h"

RadioButtonBar::RadioButtonBar(Window * aParent, HINSTANCE hInstance,
                               const std::tstring & aName,
                               LAYOUT_STRATEGY aStrategy)
: ButtonBar(aParent, hInstance, aName, aStrategy),
  mSelected(NULL)
{
}


LRESULT RadioButtonBar::OnButtonClick(Button * aButton)
{
  SelectButton(aButton);
  return ButtonBar::OnButtonClick(aButton);
}


void RadioButtonBar::SelectButton(int anID)
{
  for (std::map<Button *, int>::const_iterator miter = GetButtonMap().begin();
       miter != GetButtonMap().end();
       ++miter)
  {
    if (miter->second == anID)
    {
      SelectButton(miter->first);
      break;
    }
  }
}


void RadioButtonBar::SelectButton(Button * aButton)
{
  if (mSelected != aButton) {
    if (mSelected != NULL)
      mSelected->SetPressed(false);

    mSelected = aButton;
    if (mSelected != NULL)
      mSelected->SetPressed(true);
  }
}


int RadioButtonBar::GetSelectedID() const
{
  if (GetSelectedButton() == NULL)
    return -1;

  std::map<Button *, int>::const_iterator citer = GetButtonMap().find(const_cast<Button *>(GetSelectedButton()));
  if (citer == GetButtonMap().end())
    return -1;

  return citer->second;
}
