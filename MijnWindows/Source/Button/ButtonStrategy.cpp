#include "ButtonStrategy.h"

#include "Button.h"


LRESULT PushButtonStategy::HandleButtonDown(int aVirtualKey, int anX, int anY)
{
  (void)aVirtualKey;
  (void)anX;
  (void)anY;
  GetButton().SetPressed(true);
  //InvalidateRect(GetButton().GetHwnd(), NULL, false);
  return 0;
}


LRESULT PushButtonStategy::HandleButtonUp(int aVirtualKey, int anX, int anY)
{
  (void)aVirtualKey;
  (void)anX;
  (void)anY;
  GetButton().SetPressed(false);
  //InvalidateRect(GetButton().GetHwnd(), NULL, false);
  return 0;
}


LRESULT ToggleButtonStrategy::HandleButtonDown(int aVirtualKey, int anX, int anY)
{
  (void)aVirtualKey;
  (void)anX;
  (void)anY;
  //GetButton().SetPressed(!GetButton().IsPressed());
  //InvalidateRect(GetButton().GetHwnd(), NULL, false);
  return 0;
}


LRESULT ToggleButtonStrategy::HandleButtonUp(int aVirtualKey, int anX, int anY)
{
  (void)aVirtualKey;
  (void)anX;
  (void)anY;
  return 0;
}


LRESULT CustomButtonStrategy::HandleButtonDown(int aVirtualKey, int anX, int anY)
{
  (void)aVirtualKey;
  (void)anX;
  (void)anY;
  return 0;
}


LRESULT CustomButtonStrategy::HandleButtonUp(int aVirtualKey, int anX, int anY)
{
  (void)aVirtualKey;
  (void)anX;
  (void)anY;
  return 0;
}
