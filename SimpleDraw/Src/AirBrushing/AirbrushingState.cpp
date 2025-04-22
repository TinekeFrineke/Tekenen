#include "AirbrushingState.h"
#include "Resource/resource.h"

#include "AirbrushingSubMenu.h"
#include "Window/SpecialToolMenu.h"
#include "Window/DrawingWindow.h"

#define USE_TIMER

#ifdef USE_TIMER
static const int sTimerID = 54321;
static const int sTimerInterval = 10;
#endif


std::vector<std::vector<bool> > sUsedVector;

void Clear(std::vector<std::vector<bool> > & aUsedVector, int iElements)
{
  sUsedVector.resize(iElements);
  for (size_t i = 0; i < aUsedVector.size(); ++i) {
    sUsedVector[i].resize(iElements);
    for (size_t j = 0; j < aUsedVector[i].size(); ++j)
      aUsedVector[i][j] = false;
  }
}

AirbrushingState::AirbrushingState(DrawingWindow &      aWindow,
                                   HINSTANCE            anInstance,
                                   Controller &         aController,
                                   SpecialToolMenu &    aMenu,
                                   DrawingAttributes &  anAttributes)
: DrawingStateWithCursor(aWindow, anInstance, aController, anAttributes, IDC_POTLOOD),
  mDrawing              (false)
{
  aMenu.SetSubMenu(new AirbrushingSubMenu(aMenu, *this, anInstance, anAttributes));
}


AirbrushingState::~AirbrushingState()
{
}


LRESULT AirbrushingState::ProcessTimer(int aTimer)
{
#ifdef USE_TIMER
  if (aTimer == sTimerID) {
    POINT pos;
    GetCursorPos(&pos);
    ScreenToClient(GetWindow().GetHwnd(), &pos);

    UpdateWindow(pos);
    SetTimer(GetWindow().GetHwnd(), sTimerID, sTimerInterval, NULL);
    return 0;
  }

  return -1;
#else
  (void)aTimer;
  return 0;
#endif
}


LRESULT AirbrushingState::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  (void)wParam;
  switch (uMsg) {
    case WM_LBUTTONDOWN:
      return ProcessLButtonDown((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));

    case WM_MOUSEMOVE:
      return ProcessMouseMove((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));

    case WM_LBUTTONUP:
      return ProcessLButtonUp((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));

#ifdef USE_TIMER
    case WM_TIMER:
      return ProcessTimer(int(wParam));
#endif
  }

  return -1;
}


LRESULT AirbrushingState::ProcessLButtonDown(int anX, int anY)
{
  FlushRedo();

#ifdef USE_TIMER
  SetTimer(GetWindow().GetHwnd(), sTimerID, sTimerInterval, NULL);
#endif

  Clear(sUsedVector, GetAttributes().mThickness);
  if (mDrawing)
    return -1;

  mDrawing = true;

  SetCapture(GetWindow().GetHwnd());

  RECT rect;
  GetClientRect(GetWindow().GetHwnd(), &rect);
  POINT point = { anX, anY };

  // Draw at least a dot
  UpdateWindow(point);

  return 0;
}




LRESULT AirbrushingState::ProcessLButtonUp(int anX, int anY)
{
  (void)anX;
  (void)anY;
  if (mDrawing) {
    ReleaseCapture();
    mDrawing = false;
#ifdef USE_TIMER
    KillTimer(GetWindow().GetHwnd(), sTimerID);
#endif
    Store();
    return 0;
  }

  return -1;
}


LRESULT AirbrushingState::ProcessMouseMove(int anX, int anY)
{
#ifndef USE_TIMER
  if (!mDrawing)
#endif
    return DrawingStateWithCursor::ProcessMouseMove(anX, anY);

#ifndef USE_TIMER
  POINT point = { anX, anY };
  UpdateWindow(point);

  return 0;
#endif
}


void AirbrushingState::SetColour(COLORREF aColour)
{
  GetAttributes().mColour = aColour;
}


void AirbrushingState::SetThickness(int aThickness)
{
  GetAttributes().mThickness = aThickness;
}


void AirbrushingState::UpdateWindow(const POINT & aP)
{
  RECT rect;
  GetClientRect(GetWindow().GetHwnd(), &rect);

  Circle(GetWindow().GetHDC(), aP, GetAttributes().mThickness / 2, 5);
  RECT irect = { aP.x - GetAttributes().mThickness / 2,
                 aP.y - GetAttributes().mThickness / 2,
                 aP.x + GetAttributes().mThickness / 2,
                 aP.y + GetAttributes().mThickness / 2 };

  InvalidateRect(GetWindow().GetHwnd(), &irect, false);
}


void AirbrushingState::Circle(HDC hDC, const POINT & aCenter, int aRadius, int aWeightPercentage)
{
  SYSTEMTIME systemtime;
  GetSystemTime(&systemtime);
  srand(systemtime.wMilliseconds);
  //srand(int(time(NULL)));
  for (int row = aCenter.y - aRadius; row <= aCenter.y + aRadius; ++row) {
    for (int column = aCenter.x - aRadius; column <= aCenter.x + aRadius; ++column) {
      int distance = (column - aCenter.x) * (column - aCenter.x) + (row - aCenter.y) * (row - aCenter.y);
      float lengthfactor = float(distance) / (aRadius * aRadius);
      if (lengthfactor < 1) {
        if (rand() % 107 < aWeightPercentage * (1 - lengthfactor)) {
          sUsedVector[column - aCenter.x + aRadius][row - aCenter.y + aRadius] = true;
          SetPixel(hDC, column, row, GetAttributes().mColour);
        }
      }
    }
  }
}
