#include "StampPositionState.h"

#include "StampState.h"
#include "Window/DrawingWindow.h"
#include "Window/HoverWindow.h"
#include "Window/Definitions.h"

StampPositionState::StampPositionState(StampState & aState, HoverWindow & aWindow)
: StampSubState (aState, aWindow),
  mOldCursor    (SetCursor(NULL))
{
  LONG style = GetWindowLong(aWindow.GetHwnd(), GWL_STYLE);
  style |= WS_POPUP;
  style &= ~WS_CHILD;
  SetWindowLong(aWindow.GetHwnd(), GWL_STYLE, style);
  aWindow.SetParent(0);

  SetCapture(GetWindow().GetHwnd());
}


StampPositionState::~StampPositionState()
{
  if (GetCapture() == GetWindow().GetHwnd())
    ReleaseCapture();

  SetCursor(mOldCursor);
}


LRESULT StampPositionState::OnLeftButtonUp(int aVirtualKey, int anX, int anY)
{
  (void)aVirtualKey, anX, anY;

  RECT crect;
  GetWindow().GetWindowRect(crect);

  // Identify four corner points
  POINT point1 = { anX, anY };
  POINT point2 = { anX, anY + crect.bottom - crect.top };
  POINT point3 = { anX + crect.right - crect.right, anY + crect.bottom - crect.top };
  POINT point4 = { anX + crect.right - crect.right, anY };

  RECT rect;
  GetTarget().GetClientRect(rect);
  if (PtInRect(&rect, point1) || PtInRect(&rect, point2) ||
    PtInRect(&rect, point3) || PtInRect(&rect, point4)) {
    // Stamp overlaps window
    GetWindow().SetState(HoverWindow::ST_Size);
  }
  else {
    ReleaseCapture();
    GetWindow().SetState(HoverWindow::ST_Ready);
  }

  return 0;
}


LRESULT StampPositionState::OnMouseMove(int aVirtualKey, int anX, int anY)
{
  (void)aVirtualKey;
  return GetWindow().SetPosition(anX, anY);
}


LRESULT StampPositionState::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  (void)lParam;
  (void)wParam;
  (void)uMsg;
  switch (uMsg)
  {
  case WM_LBUTTONUP:
    return OnLeftButtonUp((int)wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));

  case WM_MOUSEMOVE:
    return OnMouseMove((int)wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));

  default:
    return StampSubState::WindowProc(uMsg, wParam, lParam);
  }
}
