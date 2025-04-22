#ifndef SUBTOOLWINDOW_DRAWINGSTATE_H__
#define SUBTOOLWINDOW_DRAWINGSTATE_H__


#include "Window/SpecialToolMenu.h"
#include "General/Observer.h"


class FreeDrawingState;
class ThicknessControl;


class DrawingSubMenu : public SubMenu, public Observer
{
public:
                          DrawingSubMenu(SpecialToolMenu & aMenu, FreeDrawingState & aState,
                                         HINSTANCE hInstance, DrawingAttributes &  anAttributes);
  virtual                 ~DrawingSubMenu();

protected:
  virtual LRESULT         LayoutWindow(const RECT & aClientRect);

  // Observer methods
  virtual void            Update(Subject * aSubject);
  virtual void            Destroyed(Subject * aSubject);

private:
  FreeDrawingState &      mState;

  ColourWindow *          mColourWindow;
  ThicknessControl *      mThicknessPanel;
  HINSTANCE               mHInstance;
};


#endif // SUBTOOLWINDOW_DRAWINGSTATE_H__