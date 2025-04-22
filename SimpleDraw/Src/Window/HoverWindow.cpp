
#include "HoverWindow.h"

#include <assert.h>

#include "Stamping/StampState.h"
#include "Stamping/StampPositionState.h"
#include "Stamping/StampSizeState.h"
#include "Window/Definitions.h"
#include "Window/DrawingWindow.h"


HoverWindow::HoverWindow(Window * aParent, HINSTANCE hInstance, StampState & aState,
                         HBITMAP hBitmap, int aWidth, int aHeight)
: GenericWindow (aParent, hInstance, _T("HoverWindow"), WS_POPUP, WS_EX_TRANSPARENT),
  mState        (aState),
  mTarget       (aState.GetDrawingWindow()),
  mHBitmap      (0),
  mHSrcBitmap   (hBitmap),
#if USE_OLD_DC == 1
  mHOldBitmap   (0),
#endif
  mWidth        (aWidth),
  mHeight       (aHeight),
  mBitmapWidth  (0),
  mBitmapHeight (0),
  mOldCursor    (SetCursor(NULL)),
  mSubState     (NULL)
{
#if PRINT_MESSAGES == 1
  SetDebug(true);
#endif
  mSubState = new StampPositionState(mState, *this);

  mHBitmap = (HBITMAP)CopyImage(hBitmap, IMAGE_BITMAP, 0, 0, 0);
  BITMAP bitmap;
  GetObject(mHBitmap, sizeof(bitmap), &bitmap);
  mBitmapWidth  = bitmap.bmWidth;
  mBitmapHeight = bitmap.bmHeight;

  mHMemoryDC = CreateCompatibleDC(0);
  SelectObject(mHMemoryDC, mHBitmap);

#if USE_OLD_DC == 1
  mHOldDC = CreateCompatibleDC(0);
#endif

  //ShowWindow(GetHwnd(), SW_SHOW);
  InvalidateRect(GetHwnd(), NULL, false);
}


HoverWindow::~HoverWindow()
{
  delete mSubState;
  mSubState = NULL;

  DeleteObject(mHBitmap);
  DeleteDC(mHMemoryDC);

#if USE_OLD_DC == 1
  DeleteObject(mHOldDC);
  if (mHOldBitmap != NULL)
    DeleteObject(mHOldBitmap);
#endif

  SetCursor(mOldCursor);
}


void HoverWindow::SetState(STATE aState)
{
  if (mSubState != NULL)
  {
    delete mSubState;
    mSubState = NULL;
  }

  switch (aState)
  {
    case ST_Position:
      mSubState = new StampPositionState(mState, *this);
      OutputDebugString(_T("Set ST_Position\n"));
      break;

    case ST_Size:
      mSubState = new StampSizeState(mState, *this);
      OutputDebugString(_T("Set ST_Size\n"));
      break;

    case ST_Ready:
      mSubState = NULL;
      mState.SetHoverWindow(NULL);
      OutputDebugString(_T("Set ST_Ready\n"));
      break;
    default:
      assert(false);
      mSubState = new StampPositionState(mState, *this);
  }
}


bool HoverWindow::CopyBitmap(HDC aDC, const POINT &p)
{
  RECT rect;
  GetClientRect(rect);
  int width = rect.right - rect.left;
  int height = rect.bottom - rect.top;
  bool bResult = TransparentBlt(aDC, p.x, p.y, width/*mWidth*/, height/*mHeight*/,
                                mHMemoryDC, 0, 0, mBitmapWidth, mBitmapHeight, GetPixel(mHMemoryDC, 0, 0)) != FALSE;
  mState.Store();
  return bResult;
}


LRESULT HoverWindow::Paint(HDC aDc, const PAINTSTRUCT & aPaintStruct)
{
  (void)aPaintStruct;
#if USE_OLD_DC == 1
  if (mHOldBitmap != NULL)
  {
    HGDIOBJ oldbmp = SelectObject(mHOldDC, mHOldBitmap);
    POINT p1 = { mOldRect.left, mOldRect.top };
    ScreenToClient(GetHwnd(), &p1);
    BitBlt(aDc, p1.x, p1.y, mOldRect.right - p1.x, mOldRect.bottom - p1.y,
           mHMemoryDC, 0, 0, SRCCOPY);
    SelectObject(mHOldDC, oldbmp);
    DeleteObject(mHOldBitmap);
  }
#endif

  RECT rect;
  GetClientRect(rect);

#if USE_OLD_DC == 1
  mOldRect = rect;
  POINT p1 = { mOldRect.left, mOldRect.top };
  ClientToScreen(GetHwnd(), &p1);
  POINT p2 = { mOldRect.right, mOldRect.bottom };
  ClientToScreen(GetHwnd(), &p2);
  mOldRect.left   = p1.x;
  mOldRect.top    = p1.y;
  mOldRect.right  = p2.x;
  mOldRect.bottom = p2.x;

  mHOldBitmap = CreateCompatibleBitmap(aDc, rect.right - rect.left,
                                           rect.bottom - rect.top);
  SelectObject(mHOldDC, mHOldBitmap);
  BitBlt(mHOldDC, 0, 0, rect.right - rect.left, rect.bottom - rect.top,
         aDc, rect.left, rect.top, SRCCOPY);
#endif

  //ShowWindow(GetHwnd(), SW_SHOW);
  TransparentBlt(aDc, rect.left, rect.top, 
                 rect.right - rect.left, rect.bottom - rect.top,
                 mHMemoryDC, 0, 0, mBitmapWidth, mBitmapHeight, GetPixel(mHMemoryDC, 0, 0));


  return 0;
}


LRESULT HoverWindow::OnPaint()
{
  PAINTSTRUCT ps;
  HDC dc = BeginPaint(GetHwnd(), &ps);

  LRESULT lResult = Paint(dc, ps);

  EndPaint(GetHwnd(), &ps);
  return lResult;
}


LRESULT HoverWindow::SetPosition(int anX, int anY)
{
  POINT p = {anX, anY};
  ClientToScreen(GetHwnd(), &p);
  SetWindowPos(HWND_TOP, p.x, p.y, mWidth, mHeight, SWP_SHOWWINDOW);
  InvalidateRect(GetHwnd(), NULL, false);
  return 0;
}


LRESULT HoverWindow::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_DESTROY)
  {
    delete mSubState;
    mSubState = NULL;
    return 0;
  }

  if (mSubState == NULL)
    return -1;

  if (mSubState->WindowProc(uMsg, wParam, lParam) != 0)
    GenericWindow::WindowProc(uMsg, wParam, lParam);

  return 0;
}
