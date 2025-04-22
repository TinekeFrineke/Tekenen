#ifndef SUBMENU_LETTERSUBMENU_H__
#define SUBMENU_LETTERSUBMENU_H__


#include "Window/SpecialToolMenu.h"
#include "General/Observer.h"

#include <vector>

#include "General/Font.h"

class Font;
class FontButtonBar;
class LetterPreview;
class LetterState;
class SizeWindow;


class LetterSubMenu : public SubMenu, public Observer
{
public:
                              LetterSubMenu(SpecialToolMenu & aMenu, LetterState & aState,
                                            HINSTANCE hInstance,
                                            DrawingAttributes &  anAttributes);
  virtual                     ~LetterSubMenu();

  const Font &                GetFont() const { return mFont; }

protected:
  void                        FillFonts();

  void                        SetColour(COLORREF aColour);
  void                        SetFont(const Font & aFont);

  virtual LRESULT             LayoutWindow(const RECT & aClientRect);

  // Observer methods
  virtual void                Update(Subject * aSubject);
  virtual void                Destroyed(Subject * aSubject);

private:
  Font                        mFont;

  LetterState &               mState;
  HINSTANCE                   mInstance;

  ColourWindow *              mColourWindow;
  FontButtonBar *             mButtonBar;
  SizeWindow *                mSizeWindow;
  LetterPreview *             mPreview;
};


#endif // SUBMENU_LETTERSUBMENU_H__