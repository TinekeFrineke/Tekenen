#ifndef STAMPSTATE_STAMPSTATEBASE_H__
#define STAMPSTATE_STAMPSTATEBASE_H__


#include <windows.h>

class HoverWindow;
class StampState;
class DrawingWindow;


class StampSubState
{
public:
                          StampSubState(StampState & aState, HoverWindow & aWindow);
  virtual                 ~StampSubState() {}

  virtual LRESULT         WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
  virtual LRESULT         OnPaint();

  HoverWindow &           GetWindow() { return mWindow; }
  DrawingWindow &         GetTarget() { return mTarget; }
  StampState &            GetState() { return mState; }
  HCURSOR                 mOldCursor;

private:
  StampSubState &         operator=(const StampSubState &);
                          StampSubState(const StampSubState &);

  StampState &            mState;
  HoverWindow &           mWindow;
  DrawingWindow &         mTarget;
};



#endif // STAMPSTATE_STAMPSTATEBASE_H__
