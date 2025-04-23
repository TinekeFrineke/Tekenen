#include "HoverButton.h"

#include <wingdi.h>

#include "Window/HoverWindow.h"

HoverButton::HoverButton(Window * aParent, HINSTANCE hInstance,
                         StampState & aState,
                         int aWidth, int aHeight,
                         const std::string & aFilename)
: Button        (aParent, hInstance, aFilename, BUTTON_STRATEGY::CustomButton),
  mState        (aState),
  mBitmapWidth  (aWidth),
  mBitmapHeight (aHeight),
  mHBitmap      (NULL)
{
}


HoverButton::~HoverButton()
{
  DeleteObject(mHBitmap);
}


void HoverButton::SetFile(const std::string & aFile)
{
  if (mHBitmap != NULL)
    DeleteObject(mHBitmap);

  mHBitmap = (HBITMAP)LoadImage(GetInstance(), aFile.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  BITMAP bitmap;
  GetObject(mHBitmap, sizeof(bitmap), &bitmap);
  mBitmapWidth  = bitmap.bmWidth;
  mBitmapHeight = bitmap.bmHeight;

  float fhorscale = float(mButtonWidth)  / mBitmapWidth;
  float fverscale = float(mButtonHeight) / mBitmapHeight;
  mScale = fhorscale < fverscale ? fhorscale : fverscale;
}


LRESULT HoverButton::OnPaint()
{
  PAINTSTRUCT ps;
  HDC dc = BeginPaint(GetHwnd(), &ps);

  if (!IsRectEmpty(&ps.rcPaint))
    ps = ps;

  FillRect(dc, &ps.rcPaint, (HBRUSH)GetStockObject(WHITE_BRUSH));

  HDC hMemDC = CreateCompatibleDC(dc);
  SelectObject(hMemDC, mHBitmap);

  int iHorOffset = int((mButtonWidth -  mBitmapWidth * mScale) / 2);
  int iVerOffset = int((mButtonHeight - mBitmapHeight * mScale) / 2);

  ::TransparentBlt(dc, iHorOffset, iVerOffset,
                   int(mBitmapWidth * mScale), int(mBitmapHeight * mScale),
                   hMemDC, 0, 0, mBitmapWidth, mBitmapHeight, GetPixel(hMemDC, 0, 0));

  DeleteDC(hMemDC);
  EndPaint(GetHwnd(), &ps);
  return 0;
}


LRESULT HoverButton::OnWindowPosChanged(const WINDOWPOS & oPos)
{
  mButtonWidth  = oPos.cx;
  mButtonHeight = oPos.cy;

  float fhorscale = float(mButtonWidth)  / mBitmapWidth;
  float fverscale = float(mButtonHeight) / mBitmapHeight;
  mScale = fhorscale < fverscale ? fhorscale : fverscale;

  return 0;
}


HoverWindow * HoverButton::CreateImage()
{
  if (mHBitmap == NULL)
    return NULL;

  return new HoverWindow(GetParent(), GetInstance(), mState, mHBitmap,
                         int(mBitmapWidth * mScale), int(mBitmapHeight * mScale));
}
