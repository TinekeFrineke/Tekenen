#ifndef DRAWINGSTATE_FILLSTATE_H__
#define DRAWINGSTATE_FILLSTATE_H__


#include "DrawingState/DrawingState.h"

class SpecialToolMenu;

class FillState : public DrawingStateWithCursor
{
public:
                        FillState(DrawingWindow &     aWindow,
                                  HINSTANCE           anInstance,
                                  Controller &        aController,
                                  SpecialToolMenu &   aMenu,
                                  DrawingAttributes & anAttributes);
                        ~FillState();

  virtual LRESULT       ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
  virtual void          SetColour(COLORREF aColour);

private:
  void                  ColourNeighbours(HDC aDc, const RECT & aClientRect, int anX, int anY, COLORREF aWithColour, COLORREF aToColour);

  LRESULT               ProcessLButtonDown(int anX, int anY);

  HBRUSH                mBrush;
};


#endif //DRAWINGSTATE_FILLSTATE_H__