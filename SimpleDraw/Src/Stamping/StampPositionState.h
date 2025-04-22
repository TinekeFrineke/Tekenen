#ifndef STAMPSTATE_STAMPPOSITIONSTATE_H__
#define STAMPSTATE_STAMPPOSITIONSTATE_H__


#include "StampSubState.h"

class StampState;


class StampPositionState : public StampSubState
{
public:
                          StampPositionState(StampState & aState, HoverWindow & aWindow);
                          ~StampPositionState();

  virtual LRESULT         WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
  LRESULT                 OnLeftButtonUp(int aVirtualKey, int anX, int anY);
  LRESULT                 OnMouseMove(int aVirtualKey, int anX, int anY);

  StampPositionState &    operator=(const StampPositionState &);
                          StampPositionState(const StampPositionState &);

  HCURSOR                 mOldCursor;
};



#endif // STAMPSTATE_STAMPPOSITIONSTATE_H__