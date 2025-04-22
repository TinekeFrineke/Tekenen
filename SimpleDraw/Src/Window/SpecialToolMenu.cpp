#include "SpecialToolMenu.h"

#include "Drawing/DrawingSubMenu.h"


SpecialToolMenu::SpecialToolMenu(Window * aParent, DrawingAttributes & anAttributes,
                                 HINSTANCE hInstance, const std::tstring & aName)
: FrameWindow   (aParent, hInstance, aName),
  mAttributes   (anAttributes),
  mSubMenu      (NULL)
{
}


SpecialToolMenu::~SpecialToolMenu()
{
  delete mSubMenu;
}


void SpecialToolMenu::SetSubMenu(SubMenu * aSubMenu)
{
  SubMenu * pMenu = mSubMenu;
  mSubMenu = aSubMenu;
  delete pMenu;

  if (mSubMenu != NULL)
  {
    RECT rect;
    GetClientRect(rect);
    mSubMenu->LayoutWindow(rect);
  }
}


LRESULT SpecialToolMenu::OnWindowPosChanged(const WINDOWPOS & aPos)
{
  (void)aPos;
  if (mSubMenu == 0)
    return -1;

  RECT rect;
  GetClientRect(rect);
  return mSubMenu->LayoutWindow(rect);
}


SubMenu::SubMenu(SpecialToolMenu & aWindow, DrawingAttributes & anAttributes)
: mWindow     (aWindow),
  mAttributes (anAttributes)
{
}

