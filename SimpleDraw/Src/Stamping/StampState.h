#ifndef DRAWINGSTATE_STAMPSTATE_H__
#define DRAWINGSTATE_STAMPSTATE_H__


#include "DrawingState/DrawingState.h"

class HoverWindow;
class SpecialToolMenu;

class StampState : public DrawingState
{
public:
                        StampState(DrawingWindow &      aWindow,
                                   HINSTANCE            anInstance,
                                   Controller &         aController,
                                   SpecialToolMenu &    aMenu,
                                   DrawingAttributes &  anAttributes);
  virtual               ~StampState();

  LRESULT               OnHoverWindowClicked(HoverWindow * aButton);

  virtual LRESULT       ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

  DrawingWindow &       GetDrawingWindow() { return GetWindow(); }

  void                  SetHoverWindow(HoverWindow * aWindow);

  virtual void          Store() { DrawingState::Store(); }

private:
  HoverWindow *         mHoverWindow;  
};


#endif // DRAWINGSTATE_STAMPSTATE_H__