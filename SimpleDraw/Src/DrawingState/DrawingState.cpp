#include "DrawingState.h"

#include <assert.h>
#include <tchar.h>

#include "General/Controller.h"
#include "General/RectUtilities.h"

#include "Window/DrawingWindow.h"

//#define DRAW_LINE

DrawingState::DrawingState(DrawingWindow & aWindow,
                           HINSTANCE anInstance,
                           Controller & aController,
                           DrawingAttributes & anAttributes)
: mWindow     (aWindow),
  mInstance   (anInstance),
  mAttributes (anAttributes),
  mController (aController)
{
}


void DrawingState::Store()
{
  mController.StoreBitmap(mWindow.GetBitmap());
}


void DrawingState::FlushRedo()
{
  mController.FlushRedo();
}


DrawingStateWithCursor::DrawingStateWithCursor(DrawingWindow &    aWindow,
                                               HINSTANCE          anInstance,
                                               Controller &       aController,
                                               DrawingAttributes &anAttributes,
                                               int                aCursor)
: DrawingState(aWindow, anInstance, aController, anAttributes),
  mCursor     (LoadCursor(anInstance, MAKEINTRESOURCE(aCursor))),
  mOldCursor  (NULL)
{
}


DrawingStateWithCursor::~DrawingStateWithCursor()
{
  if (GetCursor() == mCursor)
    SetCursor(mOldCursor);

  if (mCursor != NULL)
    DestroyCursor(mCursor);
}


bool DrawingStateWithCursor::ProcessMouseMove(int anX, int anY)
{
  (void)anX;
  (void)anY;
  if (GetCursor() != mCursor)
    mOldCursor = SetCursor(mCursor);

  return 0;
}


LRESULT DrawingStateWithCursor::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  (void)wParam;
  switch (uMsg) {
    case WM_MOUSEMOVE:
      return ProcessMouseMove((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
  }

  return -1;
}



SimpleDrawingState::SimpleDrawingState(DrawingWindow &      aWindow,
                                       HINSTANCE            anInstance,
                                       Controller &         aController,
                                       DrawingAttributes &  anAttributes)
: DrawingState(aWindow, anInstance, aController, anAttributes)
{
}


SimpleDrawingState::~SimpleDrawingState()
{
}


LRESULT SimpleDrawingState::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

  return false;
}


void SimpleDrawingState::SetColour(COLORREF aColour)
{
  GetAttributes().mColour = aColour;
}


void SimpleDrawingState::SetThickness(int aThickness)
{
  GetAttributes().mThickness = aThickness;
}


LRESULT SimpleDrawingState::ProcessLButtonDown(int anX, int anY)
{
  mDrawing = true;
  SetPixel(GetWindow().GetHDC(), anX, anY, GetAttributes().mColour);
  RECT rect = { anX, anY, anX + 1, anY + 1 };
  InvalidateRect(GetWindow().GetHwnd(), &rect, false);
  return 0;
}


LRESULT SimpleDrawingState::ProcessMouseMove(int anX, int anY)
{
  if (mDrawing) {
    SetPixel(GetWindow().GetHDC(), anX, anY, GetAttributes().mColour);
    RECT rect = { anX, anY, anX + 1, anY + 1 };
    InvalidateRect(GetWindow().GetHwnd(), &rect, false);
  }

  return 0;
}


LRESULT SimpleDrawingState::ProcessLButtonUp(int anX, int anY)
{
  SetPixel(GetWindow().GetHDC(), anX, anY, GetAttributes().mColour);
  RECT rect = { anX, anY, anX + 1, anY + 1 };
  InvalidateRect(GetWindow().GetHwnd(), &rect, false);
  mDrawing = false;
  return 0;
}
