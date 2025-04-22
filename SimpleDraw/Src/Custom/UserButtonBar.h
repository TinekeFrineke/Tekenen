#ifndef CONTROL_USERBUTTONBAR_H__
#define CONTROL_USERBUTTONBAR_H__


#include "Control/RadioButtonBar.h"


//class UserButton : public BitmapButtonBase
//{
//};

class UserProfile;

class UserButtonBar : public RadioButtonBar
{
public:
                          UserButtonBar(Window * aParent, HINSTANCE hInstance,
                                        const std::tstring & aName = _T("UserButtonBar"));

  void                    Update();

private:
  LRESULT                 OnButtonClick(Button * aButton);

  void                    AddButton(const UserProfile & aProfile, int iID);
};


#endif // CONTROL_USERBUTTONBAR_H__