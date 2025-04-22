#ifndef SUBTOOLWINDOW_DRAWINGSTATE_H__
#define SUBTOOLWINDOW_DRAWINGSTATE_H__


#include "Window/SpecialToolMenu.h"
#include "General/Observer.h"


class AirbrushingState;
//class ThicknessControl;
class AirbrushSizeWindow;


class AirbrushingSubMenu : public SubMenu, public Observer
{
public:
                          AirbrushingSubMenu(SpecialToolMenu & aMenu, AirbrushingState & aState,
                                             HINSTANCE hInstance, DrawingAttributes &  anAttributes);
  virtual                 ~AirbrushingSubMenu();

protected:
  virtual LRESULT         LayoutWindow(const RECT & aClientRect);

  // Observer methods
  virtual void            Update(Subject * aSubject);
  virtual void            Destroyed(Subject * aSubject);

private:
  AirbrushingState &      mState;

  ColourWindow *          mColourWindow;
  AirbrushSizeWindow *    mThicknessPanel;
  HINSTANCE               mHInstance;
};


#endif // SUBTOOLWINDOW_DRAWINGSTATE_H__