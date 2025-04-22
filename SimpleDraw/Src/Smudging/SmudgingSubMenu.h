#ifndef SMUDGING_SMUDGINGSUBMENU_H__
#define SMUDGING_SMUDGINGSUBMENU_H__


#include "Window/SpecialToolMenu.h"
#include "General/Observer.h"


//#define USE_THICKNESS

class SmudgingState;
#ifdef USE_THICKNESS
class ThicknessControl;
#endif


class SmudgingSubMenu : public SubMenu, public Observer
{
public:
                          SmudgingSubMenu(SpecialToolMenu & aMenu, SmudgingState & aState,
                                          HINSTANCE hInstance, DrawingAttributes &  anAttributes);
  virtual                 ~SmudgingSubMenu();

protected:
  virtual LRESULT         LayoutWindow(const RECT & aClientRect);

  // Observer methods
  virtual void            Update(Subject * aSubject);
  virtual void            Destroyed(Subject * aSubject);

private:
  SmudgingState &         mState;

//  ColourWindow *          mColourWindow;
#ifdef USE_THICKNESS
  ThicknessControl *      mThicknessPanel;
#endif
  HINSTANCE               mHInstance;
};


#endif // SMUDGING_SMUDGINGSUBMENU_H__