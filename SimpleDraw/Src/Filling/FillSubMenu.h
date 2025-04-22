#ifndef SUBMENU_FILLSUBMENU_H__
#define SUBMENU_FILLSUBMENU_H__


#include "Window/SpecialToolMenu.h"
#include "General/Observer.h"

#include <vector>

#include "General/Font.h"

class Font;
class FontButtonBar;
class FillState;


class FillSubMenu : public SubMenu, public Observer
{
public:
                              FillSubMenu(SpecialToolMenu & aMenu, FillState & aState,
                                          HINSTANCE hInstance,
                                          DrawingAttributes &  anAttributes);
  virtual                     ~FillSubMenu();

protected:
  void                        SetColour(COLORREF aColour);

  virtual LRESULT             LayoutWindow(const RECT & aClientRect);

  // Observer methods
  virtual void                Update(Subject * aSubject);
  virtual void                Destroyed(Subject * aSubject);

private:
  FillState &                 mState;
  HINSTANCE                   mInstance;

  ColourWindow *              mColourWindow;
};



#endif // SUBMENU_FILLSUBMENU_H__