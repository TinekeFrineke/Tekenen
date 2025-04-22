#ifndef AIRBRUSHING_AIRBRUSHINGSTATE_H__
#define AIRBRUSHING_AIRBRUSHINGSTATE_H__

#include "DrawingState/DrawingState.h"

class SpecialToolMenu;

class AirbrushingState : public DrawingStateWithCursor
{
public:
                        AirbrushingState(DrawingWindow &      aWindow,
                                         HINSTANCE            anInstance,
                                         Controller &         aController,
                                         SpecialToolMenu &    aMenu,
                                         DrawingAttributes &  anAttributes);
                        ~AirbrushingState();

  virtual LRESULT       ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
  virtual void          SetColour(COLORREF aColour);
  virtual void          SetThickness(int aThickness);

private:
  LRESULT               ProcessLButtonDown(int anX, int anY);
  LRESULT               ProcessMouseMove(int anX, int anY);
  LRESULT               ProcessLButtonUp(int anX, int anY);
  LRESULT               ProcessTimer(int aTimer);

  void                  UpdateWindow(const POINT & aP);
  void                  Circle(HDC hDC, const POINT & aCenter, int aRadius, int aWeightPercentage);

  bool                  mDrawing;
//  POINT                 mLastPosition;
};


#endif // AIRBRUSHING_AIRBRUSHINGSTATE_H__