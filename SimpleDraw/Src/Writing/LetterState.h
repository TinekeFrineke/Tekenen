#ifndef DRAWINGSTATE_LETTERSTATE_H__
#define DRAWINGSTATE_LETTERSTATE_H__


#include "DrawingState/DrawingState.h"
#include "General/Observer.h"

class LetterSubMenu;
class LetterWindow;
class SpecialToolMenu;

class LetterState : public DrawingState, public Observer
{
public:
  enum LETTER_STATE { ST_Writing, ST_Idle };
                        LetterState(DrawingWindow &     aWindow,
                                    HINSTANCE           anInstance,
                                    Controller &        aController,
                                    SpecialToolMenu &   aMenu,
                                    DrawingAttributes & anAttributes);
  virtual               ~LetterState();

  virtual LRESULT       ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
  void                  KillLetterWindow();
  LRESULT               OnLeftButtonUp(int aVirtualKey, int anX, int anY);

  // Observer inherited method
  virtual void          Update(Subject * aSubject);
  virtual void          Destroyed(Subject * aSubject);

private:
  LetterSubMenu *       mSubMenu;
  LetterWindow *        mWindow;
  LETTER_STATE          mState;
  HCURSOR               mOldCursor;
};


#endif // DRAWINGSTATE_LETTERSTATE_H__