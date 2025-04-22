#include "StampSizeState.h"

#include "StampState.h"
#include "Window/DrawingWindow.h"
#include "Window/HoverWindow.h"


StampSizeState::StampSizeState(StampState & aState, HoverWindow & aWindow)
: StampSubState (aState, aWindow),
  mState        (DS_None),
  mOldCursor    (SetCursor(LoadCursor(0, IDC_HAND))),
  mOldParent    (aWindow.GetParent()),
  mPen          (CreatePen(PS_DOT, 1, const_cast<const StampState &>(aState).GetAttributes().mColour)),
  mDragging     (false)
{
  SetCapture(aWindow.GetHwnd());
  RECT windowrect;
  GetWindowRect(aWindow.GetHwnd(), &windowrect);
  POINT lefttop = { windowrect.left, windowrect.top };
  ScreenToClient(GetTarget().GetHwnd(), &lefttop);

  LONG style = GetWindowLong(aWindow.GetHwnd(), GWL_STYLE);
  style &= ~WS_POPUP;
  style |= WS_CHILD;
  SetWindowLong(aWindow.GetHwnd(), GWL_STYLE, style);
  aWindow.SetParent(&GetTarget());

  SetWindowPos(aWindow.GetHwnd(), HWND_TOP, lefttop.x, lefttop.y, windowrect.right - windowrect.left, windowrect.bottom - windowrect.top, SWP_SHOWWINDOW/*SWP_NOMOVE | SWP_NOSIZE*/);
  InvalidateRect(aWindow.GetHwnd(), NULL, true);
}


StampSizeState::~StampSizeState()
{
  if (GetCapture() == GetWindow().GetHwnd())
    ReleaseCapture();

  DeleteObject(mPen);
  DestroyCursor(SetCursor(mOldCursor));
  //Parent is reset by someone else; otherwise crash on quitting app
  //GetWindow().SetParent(mOldParent);
}


void StampSizeState::OutputDebugState(DRAGGING_STATE aState)
{
  switch (aState)
  {
  case DS_None:
    OutputDebugString(_T("state == DS_None\n"));
    break;
  case DS_Left:
    OutputDebugString(_T("state == DS_None\n"));
    break;
  case DS_Right:
    OutputDebugString(_T("state == DS_None\n"));
    break;
  case DS_Top:
    OutputDebugString(_T("state == DS_None\n"));
    break;
  case DS_Bottom:
    OutputDebugString(_T("state == DS_None\n"));
    break;
  case DS_LeftTop:
    OutputDebugString(_T("state == DS_None\n"));
    break;
  case DS_RightTop:
    OutputDebugString(_T("state == DS_None\n"));
    break;
  case DS_LeftBottom:
    OutputDebugString(_T("state == DS_None\n"));
    break;
  case DS_RightBottom:
    OutputDebugString(_T("state == DS_None\n"));
    break;
  case DS_Move:
    OutputDebugString(_T("state == DS_None\n"));
    break;
  default:
    OutputDebugString(_T("state == <Unknown>\n"));
  }
}


StampSizeState::DRAGGING_STATE StampSizeState::StateFromPoint(const RECT & aRect, const POINT & aPoint)
{
  const int cMargin = 10;
  enum HORZ { left, hcenter, right, hnone };
  enum VERT { top, vcenter, bottom, vnone };
  HORZ horz = hnone;
  if (aPoint.x >= aRect.left - cMargin && aPoint.x <= aRect.left + cMargin)
    horz = left;
  else if (aPoint.x >= aRect.right - cMargin && aPoint.x <= aRect.right + cMargin)
    horz = right;
  else if (aPoint.x >= aRect.left && aPoint.x <= aRect.right)
    horz = hcenter;

  VERT vert = vnone;
  if (aPoint.y >= aRect.top - cMargin && aPoint.y <= aRect.top + cMargin)
    vert = top;
  else if (aPoint.y >= aRect.bottom - cMargin && aPoint.y <= aRect.bottom + cMargin)
    vert = bottom;
  else if (aPoint.y >= aRect.top && aPoint.y <= aRect.bottom)
    vert = vcenter;

  switch (horz)
  {
    case left:
      switch (vert)
      {
        case top:     return DS_LeftTop;
        case vcenter: return DS_Left;
        case bottom:  return DS_LeftBottom;
        default:      return DS_None;
      }
    case hcenter:
      switch (vert)
      {
        case top:     return DS_Top;
        case vcenter: return DS_Move;
        case bottom:  return DS_Bottom;
        default:      return DS_None;
      }
    case right:
      switch (vert)
      {
        case top:     return DS_RightTop;
        case vcenter: return DS_Right;
        case bottom:  return DS_RightBottom;
        default:      return DS_None;
      }
    default:
      return DS_None;
  }
}


LRESULT StampSizeState::OnLeftButtonDown(int aVirtualKey, int anX, int anY)
{
  mDragging = true;
  (void)aVirtualKey;
  RECT rect;
  GetWindow().GetClientRect(rect);

  POINT point = { anX, anY };
  mState = StateFromPoint(rect, point);

//  OutputDebugState(mState);

  if (mState == DS_Move)
  {
    // Store the absolute cursor position
    POINT npoint = { anX, anY };
    ClientToScreen(GetWindow().GetHwnd(), &npoint);
    mOldX = npoint.x;
    mOldY = npoint.y;
  }
  else if (mState == DS_None)
  {
    POINT npoint = { rect.left, rect.top };
    ClientToScreen(GetWindow().GetHwnd(), &npoint);
    ScreenToClient(GetTarget().GetHwnd(), &npoint);
    GetWindow().CopyBitmap(GetTarget().GetHDC(), npoint);
    InvalidateRect(GetTarget().GetHwnd(), NULL, false);

    // See whether we are inside the drawing window
    RECT dwrect;
    GetTarget().GetClientRect(dwrect);
    if (PtInRect(&dwrect, npoint))
      GetWindow().SetState(HoverWindow::ST_Position);
    else
      GetWindow().SetState(HoverWindow::ST_Ready);
  }

  return 0;
}


LRESULT StampSizeState::OnLeftButtonUp(int aVirtualKey, int anX, int anY)
{
  (void)aVirtualKey;
  (void)anX;
  (void)anY;
  RECT rect;
  GetWindow().GetClientRect(rect);

  mState = DS_None;
  return 0;
}


//#define MAKEINTRESOURCEW(i) (LPWSTR)((DWORD)((WORD)(i)))


void StampSizeState::AdaptCursor(const RECT & aRect, const POINT & aPoint)
{
  DRAGGING_STATE state = StateFromPoint(aRect, aPoint);
  switch (state)
  {
  case DS_Left:
  case DS_Right:
    DestroyCursor(SetCursor(LoadCursor(NULL, IDC_SIZEWE)));
    break;
  case DS_Top:
  case DS_Bottom:
    DestroyCursor(SetCursor(LoadCursor(NULL, IDC_SIZENS)));
    break;
  case DS_LeftBottom:
  case DS_RightTop:
    DestroyCursor(SetCursor(LoadCursor(NULL, IDC_SIZENESW)));
    break;
  case DS_LeftTop:
  case DS_RightBottom:
    DestroyCursor(SetCursor(LoadCursor(NULL, IDC_SIZENWSE)));
    break;
  case DS_Move:
    DestroyCursor(SetCursor(LoadCursor(NULL, IDC_SIZEALL)));
    break;
  case DS_None:
    DestroyCursor(SetCursor(LoadCursor(NULL, IDC_HAND)));
    break;
  }
}


LRESULT StampSizeState::OnMouseMove(int aVirtualKey, int anX, int anY)
{
  (void)aVirtualKey;

  RECT crect;
  GetWindow().GetClientRect(crect);

  switch (mState)
  {
  case DS_Left:
    if (anX < crect.right)
      crect.left = anX;
    break;
  case DS_Right:
    if (anX > crect.left)
      crect.right = anX;
    break;
  case DS_Top:
    if (anY < crect.bottom)
      crect.top = anY;
    break;
  case DS_Bottom:
    if (anY > crect.top)
      crect.bottom = anY;
    break;
  case DS_LeftTop:
    //hdiff = anX
    if (anX < crect.right)
      crect.left = anX;
    if (anY < crect.bottom)
      crect.top = anY;
    break;
  case DS_LeftBottom:
    if (anX < crect.right)
      crect.left = anX;
    if (anY > crect.top)
      crect.bottom = anY;
    break;
  case DS_RightTop:
    if (anX > crect.left)
      crect.right = anX;
    if (anY < crect.bottom)
      crect.top = anY;
    break;
  case DS_RightBottom:
    if (anX > crect.left)
      crect.right = anX;
    if (anY > crect.top)
      crect.bottom = anY;
    break;
  case DS_Move:
    {
      // Determine the extent of the move
      POINT point = { anX, anY };
      ClientToScreen(GetWindow().GetHwnd(), &point);
      crect.left   += point.x - mOldX;
      crect.right  += point.x - mOldX;
      crect.top    += point.y - mOldY;
      crect.bottom += point.y - mOldY;
      mOldX = point.x;
      mOldY = point.y;
    }
    break;
  case DS_None:
    {
      // Not dragging
      POINT newpoint = { anX, anY };
      AdaptCursor(crect, newpoint);
      return 0;
    }
    break;
  }

  POINT lefttop = { crect.left, crect.top };
  ClientToScreen(GetWindow().GetHwnd(), &lefttop);
  ScreenToClient(GetTarget().GetHwnd(), &lefttop);
  GetWindow().SetWindowPos(NULL, lefttop.x/*crect.left*/, lefttop.y/*crect.top*/, crect.right - crect.left, crect.bottom - crect.top, SWP_SHOWWINDOW);
  return 0;
}


LRESULT StampSizeState::OnPaint()
{
  PAINTSTRUCT paintstruct;
  HDC dc = BeginPaint(GetWindow().GetHwnd(), &paintstruct);
  GetWindow().Paint(dc, paintstruct);

  RECT rect;
  GetWindow().GetClientRect(rect);

  POINT lefttop = { rect.left, rect.top };
  ClientToScreen(GetWindow().GetHwnd(), &lefttop);
  HGDIOBJ oldbrush = SelectObject(dc, GetStockObject(HOLLOW_BRUSH));
  HGDIOBJ oldpen = SelectObject(dc, mPen);
  Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);
  SelectObject(dc, oldbrush);
  SelectObject(dc, oldpen);
  EndPaint(GetWindow().GetHwnd(), &paintstruct);
  return 0;
}


LRESULT StampSizeState::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  (void)lParam;
  (void)wParam;
  (void)uMsg;
  switch (uMsg)
  {
  case WM_LBUTTONDOWN:
    return OnLeftButtonDown((int)wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));

  case WM_LBUTTONUP:
    return OnLeftButtonUp((int)wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));

  case WM_MOUSEMOVE:
    return OnMouseMove((int)wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));

  default:
    return StampSubState::WindowProc(uMsg, wParam, lParam);
  }
}
