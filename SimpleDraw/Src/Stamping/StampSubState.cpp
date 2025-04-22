#include "StampSubState.h"

#include "StampState.h"
#include "Window/DrawingWindow.h"
#include "Window/HoverWindow.h"


StampSubState::StampSubState(StampState & aState, HoverWindow & aWindow)
: mWindow (aWindow),
  mTarget (aState.GetDrawingWindow()),
  mState  (aState)
{
}

LRESULT StampSubState::OnPaint()
{
  PAINTSTRUCT paintstruct;
  HDC dc = BeginPaint(GetWindow().GetHwnd(), &paintstruct);
  LRESULT result = GetWindow().Paint(dc, paintstruct);
  EndPaint(GetWindow().GetHwnd(), &paintstruct);
  return result;
}


LRESULT StampSubState::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  (void)lParam;
  (void)wParam;
  (void)uMsg;

  switch (uMsg)
  {
  case WM_PAINT:
    return OnPaint();

  default:
    return -1;
  }
}
