#ifndef DRAWINGSTATE_FREEDRAWINGSTATE_H__
#define DRAWINGSTATE_FREEDRAWINGSTATE_H__

#include "DrawingState/DrawingState.h"

class SpecialToolMenu;

class FreeDrawingState : public DrawingStateWithCursor
{
public:
                        FreeDrawingState(DrawingWindow &      aWindow,
                                         HINSTANCE            anInstance,
                                         Controller &         aController,
                                         SpecialToolMenu &    aMenu,
                                         DrawingAttributes &  anAttributes);
                        ~FreeDrawingState();

  virtual LRESULT       ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
  virtual void          SetColour(COLORREF aColour);
  virtual void          SetThickness(int aThickness);

private:
  LRESULT               ProcessLButtonDown(int anX, int anY);
  LRESULT               ProcessMouseMove(int anX, int anY);
  LRESULT               ProcessLButtonUp(int anX, int anY);

  void                  UpdateWindow(const POINT & aP1, const POINT & aP2);

  bool                  mDrawing;
  POINT                 mLastPosition;

  HPEN                  mHPen;
};


#endif // DRAWINGSTATE_FREEDRAWINGSTATE_H__