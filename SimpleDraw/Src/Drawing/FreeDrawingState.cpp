#include "FreeDrawingState.h"
#include "Resource/resource.h"

#include "DrawingSubMenu.h"
#include "General/Profile.h"
#include "Window/SpecialToolMenu.h"
#include "Window/DrawingWindow.h"

#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif


class SubToolMenu;

FreeDrawingState::FreeDrawingState(DrawingWindow &      aWindow,
                                   HINSTANCE            anInstance,
                                   Controller &         aController,
                                   SpecialToolMenu &    aMenu,
                                   DrawingAttributes &  anAttributes)
: DrawingStateWithCursor(aWindow, anInstance, aController, anAttributes, Profile::GetInstance().GetResourceDirectory() + "\\potlood.cur"),
  mDrawing              (false),
  mHPen                 (CreatePen(PS_SOLID, anAttributes.mThickness, anAttributes.mColour))
{
  SelectObject(GetWindow().GetHDC(), HGDIOBJ(mHPen));

  aMenu.SetSubMenu(new DrawingSubMenu(aMenu, *this, anInstance, anAttributes));
}


FreeDrawingState::~FreeDrawingState()
{
  DeleteObject(mHPen);
}


LRESULT FreeDrawingState::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  (void)wParam;
  switch (uMsg) {
    case WM_LBUTTONDOWN:
      return ProcessLButtonDown((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));

    case WM_MOUSEMOVE:
      return ProcessMouseMove((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));

    case WM_LBUTTONUP:
      return ProcessLButtonUp((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
  }

  return -1;
}


LRESULT FreeDrawingState::ProcessLButtonDown(int anX, int anY)
{
  FlushRedo();

  if (mDrawing)
    return -1;

  mDrawing = true;

  SetCapture(GetWindow().GetHwnd());

  RECT rect;
  GetClientRect(GetWindow().GetHwnd(), &rect);
  POINT point = { anX, anY };

  mLastPosition = point;
  // Draw at least a dot
  UpdateWindow(mLastPosition, mLastPosition);

  return 0;
}


LRESULT FreeDrawingState::ProcessLButtonUp(int anX, int anY)
{
  (void)anX;
  (void)anY;
  if (mDrawing) {
    ReleaseCapture();
    mDrawing = false;
    Store();
    return 0;
  }

  return -1;
}


LRESULT FreeDrawingState::ProcessMouseMove(int anX, int anY)
{
  if (!mDrawing)
    return DrawingStateWithCursor::ProcessMouseMove(anX, anY);

  POINT point = { anX, anY };

  UpdateWindow(mLastPosition, point);

  mLastPosition = point;

  return 0;
}


void FreeDrawingState::SetColour(COLORREF aColour)
{
  GetAttributes().mColour = aColour;

  mHPen = CreatePen(PS_SOLID, GetAttributes().mThickness, GetAttributes().mColour);
  HGDIOBJ oldpen = SelectObject(GetWindow().GetHDC(), mHPen);

  DeleteObject(oldpen);
}


void FreeDrawingState::SetThickness(int aThickness)
{
  GetAttributes().mThickness = aThickness;

  mHPen = CreatePen(PS_SOLID, GetAttributes().mThickness, GetAttributes().mColour);
  HGDIOBJ oldpen = SelectObject(GetWindow().GetHDC(), mHPen);

  DeleteObject(oldpen);
}


void FreeDrawingState::UpdateWindow(const POINT & aP1, const POINT & aP2)
{
  RECT rect;
  GetClientRect(GetWindow().GetHwnd(), &rect);

//  if (aP1.x == aP2.x && aP1.y == aP2.y)
//  {
////    Ellipse(GetWindow().GetHDC(),
    MoveToEx(GetWindow().GetHDC(), aP1.x, aP1.y, NULL);
    LineTo(GetWindow().GetHDC(), aP2.x, aP2.y);
  //}
  //else
  //{
  //  MoveToEx(GetWindow().GetHDC(), aP1.x, aP1.y, NULL);
  //  LineTo(GetWindow().GetHDC(), aP2.x, aP2.y);
  //}
  RECT irect = { aP1.x <  aP2.x ? aP1.x : aP2.x,
                 aP1.y <  aP2.y ? aP1.y : aP2.y,
                 aP1.x >= aP2.x ? aP1.x + 1 : aP2.x + 1,
                 aP1.y >= aP2.y ? aP1.y + 1 : aP2.y + 1 };

  irect.left   = max(rect.left, irect.left - GetAttributes().mThickness);
  irect.top    = max(rect.top, irect.top - GetAttributes().mThickness);
  irect.right  = min(rect.right, irect.right + GetAttributes().mThickness);
  irect.bottom = min(rect.bottom, irect.bottom + GetAttributes().mThickness);

  InvalidateRect(GetWindow().GetHwnd(), NULL, false);
}
