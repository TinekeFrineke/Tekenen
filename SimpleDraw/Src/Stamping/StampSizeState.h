#ifndef STAMPSTATE_STAMPSIZESTATE_H__
#define STAMPSTATE_STAMPSIZESTATE_H__


#include "StampSubState.h"

class Window;

class StampSizeState : public StampSubState
{
public:
                          StampSizeState(StampState & aState, HoverWindow & aWindow);
                          ~StampSizeState();

  virtual LRESULT         WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
  virtual LRESULT         OnPaint();
  virtual LRESULT         OnLeftButtonDown(int aVirtualKey, int anX, int anY);
  virtual LRESULT         OnLeftButtonUp(int aVirtualKey, int anX, int anY);
  virtual LRESULT         OnMouseMove(int aVirtualKey, int anX, int anY);

private:
  enum DRAGGING_STATE
  {
    DS_None, DS_Left, DS_Right, DS_Top, DS_Bottom,
    DS_LeftTop, DS_RightTop, DS_LeftBottom, DS_RightBottom, DS_Move
  };

  static void             OutputDebugState(DRAGGING_STATE aState);

  StampSizeState &        operator=(const StampSizeState &);
                          StampSizeState(const StampSizeState &);

  DRAGGING_STATE          StateFromPoint(const RECT & aRect, const POINT & aPoint);
  void                    AdaptCursor(const RECT & aRect, const POINT & aPoint);

  bool                    mDragging;
  DRAGGING_STATE          mState;
  int                     mOldX;
  int                     mOldY;
  HPEN                    mPen;
  HCURSOR                 mOldCursor;
  Window *                mOldParent;
};


#endif // STAMPSTATE_STAMPSIZESTATE_H__