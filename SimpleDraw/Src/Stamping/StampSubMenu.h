#ifndef SUBMENU_STAMPSUBMENU_H__
#define SUBMENU_STAMPSUBMENU_H__


#include "Window/HoverWindow.h"
#include "Window/SpecialToolMenu.h"
#include "General/Observer.h"

#include <vector>


class HoverButtonBar;
class HoverButton;
class StampState;
template <class Target> class SizeBar;

class StampSubMenu : public SubMenu, public Observer
{
public:
                              StampSubMenu(SpecialToolMenu & aMenu, StampState & aState,
                                           HINSTANCE hInstance, DrawingAttributes &  anAttributes);
  virtual                     ~StampSubMenu();

  void                        ScrollLeft();
  void                        ScrollRight();

protected:
  virtual LRESULT             LayoutWindow(const RECT & aClientRect);

  // Observer methods
  virtual void                Update(Subject * aSubject);
  virtual void                Destroyed(Subject * aSubject);

private:
  void                        GetFiles();

  StampState &                mState;
  HINSTANCE                   mHInstance;

  HoverButtonBar *            mButtonBar;
  std::vector<std::tstring>   mFiles;
  std::vector<StampSubMenu *> mCells;

  static const int            mNumberOfRows = 1;
  static const int            mNumberOfColumns = 8;
};


#endif // SUBTOOLWINDOW_DRAWINGSTATE_H__