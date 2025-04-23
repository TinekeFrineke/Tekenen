
#include "DrawingWindow.h"

#include <stdio.h>

#include "DrawingState/DrawingState.h"
#include "Filling/FillState.h"
#include "Drawing/FreeDrawingState.h"
#include "Stamping/StampState.h"
#include "Writing/LetterState.h"
#include "Smudging/SmudgingState.h"
#include "Airbrushing/AirbrushingState.h"
#include "Resource/resource.h"


template<>
bool MijnWindow<DrawingWindow>::mRegistered = false;

DrawingWindow::DrawingWindow(Window * aParent, SpecialToolMenu & aMenu,
                             DrawingAttributes & anAttributes,
                             HINSTANCE hInstance, const std::string & aName)
: MijnWindow<DrawingWindow> (aParent, hInstance, aName),
  mSpecialToolMenu          (aMenu),
  mAttributes               (anAttributes),
  mState                    (NULL),
  mDrawingState             (Tekenen::DRAW_STATE::None),
  mHBitMap                  (NULL)
{
  Create(GetClassName(), "TekenWindow", WS_CHILD);
  mHScreenDC = GetDC(GetHwnd());
  mHMemoryDC = CreateCompatibleDC(mHScreenDC);

  SetState(Tekenen::DRAW_STATE::FreeDraw);
#if PRINT_MESSAGES > 0
  SetDebug(true);
#endif
}


DrawingWindow::~DrawingWindow()
{
  DestroyCursor(mCursor);
  if (mHBitMap != NULL)
    DeleteObject(mHBitMap);

  DeleteDC(mHMemoryDC);
  ReleaseDC(GetHwnd(), mHScreenDC);
}


void DrawingWindow::SetState(Tekenen::DRAW_STATE aState)
{
  RECT rect;
  GetClientRect(rect);
  if (aState == mDrawingState)
    return;

  mDrawingState = aState;

  DrawingState * pState = NULL;
  switch (aState)
  {
  case Tekenen::DRAW_STATE::FreeDraw:
    pState = new FreeDrawingState(*this, GetInstance(), Controller::GetInstance(), mSpecialToolMenu, mAttributes);
    break;

  case Tekenen::DRAW_STATE::Smudge:
    pState = new SmudgingState(*this, GetInstance(), Controller::GetInstance(), mSpecialToolMenu, mAttributes);
    break;

  case Tekenen::DRAW_STATE::Stamp:
    pState = new StampState(*this, GetInstance(), Controller::GetInstance(), mSpecialToolMenu, mAttributes);
    break;

  case Tekenen::DRAW_STATE::Letter:
    pState = new LetterState(*this, GetInstance(), Controller::GetInstance(), mSpecialToolMenu, mAttributes);
    break;

  case Tekenen::DRAW_STATE::Fill:
    pState = new FillState(*this, GetInstance(), Controller::GetInstance(), mSpecialToolMenu, mAttributes);
    break;
  case Tekenen::DRAW_STATE::Airbrush:
    pState = new AirbrushingState(*this, GetInstance(), Controller::GetInstance(), mSpecialToolMenu, mAttributes);
    break;
  }

  delete mState;
  mState = pState;
}


void DrawingWindow::ClearWindow()
{
  RECT rect;
  GetClientRect(rect);
  FillRect(mHMemoryDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));
  InvalidateRect(GetHwnd(), &rect, false);
}


HDC DrawingWindow::GetHDC()
{
  return mHMemoryDC;
}


LRESULT DrawingWindow::OnPaint()
{
  PAINTSTRUCT ps;
  HDC dc = BeginPaint(GetHwnd(), &ps);

  BitBlt(dc, ps.rcPaint.left, ps.rcPaint.top,
         ps.rcPaint.right - ps.rcPaint.left, ps.rcPaint.bottom - ps.rcPaint.top,
         mHMemoryDC, ps.rcPaint.left, ps.rcPaint.top, SRCCOPY);

  EndPaint(GetHwnd(), &ps);
  return 0;
}


LRESULT DrawingWindow::OnWindowPosChanged(const WINDOWPOS & oPos)
{
  (void)oPos;
  RECT rect;
  GetClientRect(rect);

  if (mHBitMap == NULL)
  {
    mHBitMap = CreateCompatibleBitmap(mHScreenDC, rect.right - rect.left, rect.bottom - rect.top);
    SelectObject(mHMemoryDC, mHBitMap);
    ClearWindow();
  }

  return 0;
}


LRESULT DrawingWindow::OnChangeSize(int iWidth, int iHeight)
{
  if (mHBitMap != NULL)
    DeleteObject(mHBitMap);
  mHBitMap = CreateCompatibleBitmap(mHScreenDC, iWidth, iHeight);
  SelectObject(mHMemoryDC, mHBitMap);

  ClearWindow();

  return 0;
}


LRESULT DrawingWindow::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg) {
    case WM_SIZE:
      return OnChangeSize(LOWORD(lParam), HIWORD(lParam));

    case WM_LBUTTONDOWN:
      if (mState != NULL && mState->ProcessMessage(uMsg, wParam, lParam) == 0)
        return 0;
      break;

    default:
      if (mState != NULL && mState->ProcessMessage(uMsg, wParam, lParam) == 0)
        return 0;
      break;
  }

  return MijnWindow<DrawingWindow>::WindowProc(uMsg, wParam, lParam);
}


bool DrawingWindow::RegisterClass(HINSTANCE hInstance)
{
  WNDCLASS sWndClass;
  memset(&sWndClass, 0, sizeof(WNDCLASS));
  sWndClass.hbrBackground = HBRUSH(GetStockObject(WHITE_BRUSH));
  sWndClass.lpszClassName = GetClassName(); // L"TekenWindow";
  sWndClass.lpfnWndProc = MijnWindow<DrawingWindow>::WindowProc;
  //sWndClass.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND));
  sWndClass.hInstance = hInstance;

  return ::RegisterClass(&sWndClass) != NULL;
}


void DrawingWindow::SetBitmap(HBITMAP hBitmap)
{
  RECT rect;
  GetClientRect(rect);
  FillRect(mHMemoryDC, &rect, (HBRUSH)GetStockObject(WHITE_BRUSH));

  HDC memdc = CreateCompatibleDC(mHMemoryDC);
  HGDIOBJ obj = SelectObject(memdc, hBitmap);

  BITMAP bitmap;
  GetObject(hBitmap, sizeof(bitmap), &bitmap);

  int srcwidth = bitmap.bmWidth;
  int srcheight = bitmap.bmHeight;

  GetObject(mHBitMap, sizeof(bitmap), &bitmap);
  int dstwidth = bitmap.bmWidth;
  int dstheight = bitmap.bmHeight;

  float hscale = float(dstwidth) / srcwidth;
  float vscale = float(dstheight) / srcheight;

  float scale = std::min(hscale, vscale);

  int hormargin = int((dstwidth - srcwidth * scale) / 2);
  int vermargin = int((dstheight - srcheight * scale) / 2);

  StretchBlt(mHMemoryDC, hormargin, vermargin,
                         int(srcwidth * scale), int(srcheight * scale),
             memdc,      0, 0,
                         srcwidth, srcheight,
             SRCCOPY);

  SelectObject(memdc, obj);
  DeleteObject(hBitmap);
  DeleteDC(memdc);
//  float horscale = GetDeviceCaps(memdc,

  //HGDIOBJ obj = SelectObject(mHMemoryDC, hBitmap);
  //mHBitMap = hBitmap;
  //if (obj)
  //  DeleteObject(obj);

  InvalidateRect(GetHwnd(), NULL, false);
}
