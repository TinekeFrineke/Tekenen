
#include "SmudgingState.h"

#include <windef.h>

#include "General/Profile.h"
#include "Resource/resource.h"

#include "SmudgingSubMenu.h"
#include "Window/SpecialToolMenu.h"
#include "Window/DrawingWindow.h"
class SubToolMenu;


SmudgingState::SmudgingState(DrawingWindow &      aWindow,
                             HINSTANCE            anInstance,
                             Controller &         aController,
                             SpecialToolMenu &    aMenu,
                             DrawingAttributes &  anAttributes)
: DrawingStateWithCursor(aWindow, anInstance, aController, anAttributes, Profile::GetInstance().GetResourceDirectory() + "\\Smudge.cur"),
  mDrawing              (false)
{
  aMenu.SetSubMenu(new SmudgingSubMenu(aMenu, *this, anInstance, anAttributes));
}


SmudgingState::~SmudgingState()
{
}


LRESULT SmudgingState::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  (void)wParam;
  switch (uMsg) {
    case WM_LBUTTONDOWN:
      return ProcessLButtonDown((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));

    case WM_MOUSEMOVE:
      return ProcessMouseMove((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));

    case WM_LBUTTONUP:
      return ProcessLButtonUp((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));

    case WM_ACTIVATEAPP:
      mDrawing = false;
      return 0;
  }

  return -1;
}


static int type = 0;


LRESULT SmudgingState::ProcessLButtonDown(int anX, int anY)
{
  type++;
  if (type > 10)
    type = 1;

  FlushRedo();

  if (mDrawing)
    return -1;

  mDrawing = true;

  SetCapture(GetWindow().GetHwnd());

  RECT rect;
  GetClientRect(GetWindow().GetHwnd(), &rect);
  POINT point = { anX, anY };

  mLastPosition = point;

  return 0;
}


LRESULT SmudgingState::Smudge5(int anX, int anY)
{
  //TCHAR smsg[1024];
  //_stprintf(smsg, "### Smudge4: anX == %d anY == %d mLastPosition == %d, %d\n"), anX, anY, mLastPosition.x, mLastPosition.y);
  //OutputDebugString(smsg);
  Vector point(anX, anY);
  Vector direction(point - mLastPosition);

  int dlength(direction.GetLength());
  if (dlength /*< 2*/== 0)
    return -1;

  bool bLeft = anX - mLastPosition.x < 0;
  bool bUp   = anY - mLastPosition.y < 0;

  int startx;
  int endx;
  int hIncrement;
  int starty;
  int endy;
  int vIncrement;

//#define USE_THICKNESS

#ifdef USE_THICKNESS
  const int halfwidth = GetAttributes().mThickness / 2;
#else
  const int halfwidth = 8;
#endif

  if (bLeft)
  {
    startx      = point.GetX() - halfwidth;
    endx        = point.GetX() + halfwidth;
    hIncrement  = 1/* * (dlength + 3 / 4)*/;
  }
  else
  {
    startx      = point.GetX() + halfwidth;
    endx        = point.GetX() - halfwidth;
    hIncrement  = -1/* * (dlength + 3 / 4)*/;
  }

  if (bUp)
  {
    starty      = point.GetY() - halfwidth;
    endy        = point.GetY() + halfwidth;
    vIncrement  = 1/* * (dlength + 3 / 4)*/;
  }
  else
  {
    starty      = point.GetY() + halfwidth;
    endy        = point.GetY() - halfwidth;
    vIncrement  = -1/* * (dlength + 3 / 4)*/;
  }

  //_stprintf(smsg, "###          hIncrement == %d vIncrement %d\n"), hIncrement, vIncrement);
  //OutputDebugString(smsg);

  for (int i = startx; bLeft ? i <= endx : i >= endx; i += hIncrement)
  {
    for (int j = starty; bUp ? j <= endy : j >= endy; j += vIncrement)
    {
      int squaredistance = point.SquareDistance(Vector(i, j));
      double weight = squaredistance / (halfwidth * halfwidth);
      if (weight < 1)
      {
        int red     = 0;
        int green   = 0;
        int blue    = 0;
        int nrcols  = 0;

        for (int length = 1; length <= dlength; ++length)
        {
          red   += length * GetRValue(GetPixel(GetWindow().GetHDC(), i - direction.GetX() / length, j - direction.GetY() / length));
          green += length * GetGValue(GetPixel(GetWindow().GetHDC(), i - direction.GetX() / length, j - direction.GetY() / length));
          blue  += length * GetBValue(GetPixel(GetWindow().GetHDC(), i - direction.GetX() / length, j - direction.GetY() / length));
          nrcols += length;
        }

        SetPixel(GetWindow().GetHDC(), i, j, RGB(red / nrcols, green / nrcols, blue / nrcols));
      }
    }
  }

  mLastPosition.x = point.GetX();
  mLastPosition.y = point.GetY();

  RECT rect;
  rect.top    = anY - halfwidth;
  rect.bottom = anY + halfwidth;
  rect.left   = anX - halfwidth;
  rect.right  = anX + halfwidth;

  InvalidateRect(GetWindow().GetHwnd(), &rect, false);
  UpdateWindow(GetWindow().GetHwnd());
  // Determine the pixels that must be adapted:

  //OutputDebugString("### Smudge4 Done\n"));

  return 0;
}


LRESULT SmudgingState::ProcessLButtonUp(int anX, int anY)
{
  (void)anX;
  (void)anY;
  if (mDrawing)
  {
    ReleaseCapture();
    mDrawing = false;
    Store();
    return 0;
  }

  return -1;
}


LRESULT SmudgingState::ProcessMouseMove(int anX, int anY)
{
  if (!mDrawing)
    return DrawingStateWithCursor::ProcessMouseMove(anX, anY);

  //if (type % 2 == 0)
    return Smudge5(anX, anY);
  //else
  //  return Smudge8(anX, anY);
  //switch (type)
  //{
  ////case 1:
  ////  return Smudge1(anX, anY);
  ////case 2:
  ////  return Smudge2(anX, anY);
  ////case 3:
  ////  return Smudge3(anX, anY);
  //case 4:
  //default:
  //  return Smudge8(anX, anY);
  ////case 5:
  ////  return Smudge5(anX, anY);
  ////case 6:
  ////  return Smudge6(anX, anY);
  ////case 7:
  ////default:
  ////  return Smudge7(anX, anY);
  //}
}


void SmudgingState::SetColour(COLORREF aColour)
{
  GetAttributes().mColour = aColour;
}


void SmudgingState::SetThickness(int aThickness)
{
  GetAttributes().mThickness = aThickness;
}


void SmudgingState::Circle(int anX, int anY)
{
  HGDIOBJ oldpen   = SelectObject(GetWindow().GetHDC(), CreatePen(PS_SOLID, 1, GetAttributes().mColour));
  HGDIOBJ oldbrush = SelectObject(GetWindow().GetHDC(), CreateSolidBrush(GetAttributes().mColour));
  ::Ellipse(GetWindow().GetHDC(), anX - (GetAttributes().mThickness / 2),
                                  anY - (GetAttributes().mThickness / 2),
                                  anX + (GetAttributes().mThickness / 2),
                                  anY + (GetAttributes().mThickness / 2));
  DeleteObject(SelectObject(GetWindow().GetHDC(), oldpen));
  DeleteObject(SelectObject(GetWindow().GetHDC(), oldbrush));

  RECT rect;
  GetClientRect(GetWindow().GetHwnd(), &rect);
  RECT irect = { anX - (GetAttributes().mThickness / 2),
                 anY - (GetAttributes().mThickness / 2),
                 anX + (GetAttributes().mThickness / 2),
                 anY + (GetAttributes().mThickness / 2) };

  irect.left   = std::max(rect.left, irect.left - GetAttributes().mThickness);
  irect.top    = std::max(rect.top, irect.top - GetAttributes().mThickness);
  irect.right  = std::min(rect.right, irect.right + GetAttributes().mThickness);
  irect.bottom = std::min(rect.bottom, irect.bottom + GetAttributes().mThickness);

  InvalidateRect(GetWindow().GetHwnd(), &irect, false);
}


LRESULT SmudgingState::Smudge1(int anX, int anY)
{
  POINT point = { anX, anY };

  bool bLeft = anX - mLastPosition.x < 0;
  bool bUp   = anY - mLastPosition.y < 0;

  int startx;
  int endx;
  int hIncrement;
  int starty;
  int endy;
  int vIncrement;

#ifdef USE_THICKNESS
  const int halfwidth = GetAttributes().mThickness / 2;
#else
  const int halfwidth = 8;
#endif
  if (bLeft)
  {
    startx      = anX - halfwidth;
    endx        = anX + halfwidth;
    hIncrement  = 1;
  }
  else
  {
    startx      = anX + halfwidth;
    endx        = anX - halfwidth;
    hIncrement  = -1;
  }

  if (bUp)
  {
    starty      = anY - halfwidth;
    endy        = anY + halfwidth;
    vIncrement  = 1;
  }
  else
  {
    starty      = anY + halfwidth;
    endy        = anY - halfwidth;
    vIncrement  = -1;
  }

  for (int i = startx; bLeft ? i <= endx : i >= endx; i += hIncrement)
  {
    for (int j = starty; bUp ? j <= endy : j >= endy; j += vIncrement)
    {
      double sqdistance = double(i - anX) * double(i - anX) + double(j - anY) * double(j - anY);
      if (sqdistance < halfwidth * halfwidth)
      {
        int red2   = GetRValue(GetPixel(GetWindow().GetHDC(), i - hIncrement, j));
        int red3   = GetRValue(GetPixel(GetWindow().GetHDC(), i, j - vIncrement));
        int red    = (red2 + red3) / 2;
        int green2 = GetGValue(GetPixel(GetWindow().GetHDC(), i - hIncrement, j));
        int green3 = GetGValue(GetPixel(GetWindow().GetHDC(), i, j - vIncrement));
        int green  = (green2 + green3) / 2;
        int blue2  = GetBValue(GetPixel(GetWindow().GetHDC(), i - hIncrement, j));
        int blue3  = GetBValue(GetPixel(GetWindow().GetHDC(), i, j - vIncrement));
        int blue   = (blue2 + blue3) / 2;
        SetPixel(GetWindow().GetHDC(), i, j, RGB(red, green, blue));
      }
    }
  }

  mLastPosition.x = point.x;
  mLastPosition.y = point.y;

  RECT rect;
  rect.top    = anY - halfwidth;
  rect.bottom = anY + halfwidth;
  rect.left   = anX - halfwidth;
  rect.right  = anX + halfwidth;

  InvalidateRect(GetWindow().GetHwnd(), &rect, false);
  UpdateWindow(GetWindow().GetHwnd());
  // Determine the pixels that must be adapted:
  return 0;
}


LRESULT SmudgingState::Smudge2(int anX, int anY)
{
  Vector point(anX, anY);
  Vector direction = point - mLastPosition;

  bool bLeft = anX - mLastPosition.x < 0;
  bool bUp   = anY - mLastPosition.y < 0;

  int startx;
  int endx;
  int hIncrement;
  int starty;
  int endy;
  int vIncrement;

  if (bLeft)
  {
    startx      = anX - (GetAttributes().mThickness / 3);
    endx        = anX + (GetAttributes().mThickness / 3);
    hIncrement  = 1;
  }
  else
  {
    startx      = anX + (GetAttributes().mThickness / 3);
    endx        = anX - (GetAttributes().mThickness / 3);
    hIncrement  = -1;
  }

  if (bUp)
  {
    starty      = anY - (GetAttributes().mThickness / 3);
    endy        = anY + (GetAttributes().mThickness / 3);
    vIncrement  = 1;
  }
  else
  {
    starty      = anY + (GetAttributes().mThickness / 3);
    endy        = anY - (GetAttributes().mThickness / 3);
    vIncrement  = -1;
  }

  for (int i = startx; bLeft ? i <= endx : i >= endx; i += hIncrement)
  {
    for (int j = starty; bUp ? j <= endy : j >= endy; j += vIncrement)
    {
      if (point.SquareDistance(Vector(i, j)) < (GetAttributes().mThickness * GetAttributes().mThickness) * 0.25)
      {
        int red1   = GetRValue(GetPixel(GetWindow().GetHDC(), i - hIncrement, j));
        int red2   = GetRValue(GetPixel(GetWindow().GetHDC(), i, j - vIncrement));
        int red    = (red1 + red2) >> 1;
        int green1 = GetGValue(GetPixel(GetWindow().GetHDC(), i - hIncrement, j));
        int green2 = GetGValue(GetPixel(GetWindow().GetHDC(), i, j - vIncrement));
        int green  = (green1 + green2) >> 1;
        int blue1  = GetBValue(GetPixel(GetWindow().GetHDC(), i - hIncrement, j));
        int blue2  = GetBValue(GetPixel(GetWindow().GetHDC(), i, j - vIncrement));
        int blue   = (blue1 + blue2) >> 1;
        SetPixel(GetWindow().GetHDC(), i, j, RGB(red, green, blue));
      }
    }
  }

  mLastPosition.x = point.GetX();
  mLastPosition.y = point.GetY();

  RECT rect;
  rect.top    = anY - (GetAttributes().mThickness / 2);
  rect.bottom = anY + (GetAttributes().mThickness / 2);
  rect.left   = anX - (GetAttributes().mThickness / 2);
  rect.right  = anX + (GetAttributes().mThickness / 2);

  InvalidateRect(GetWindow().GetHwnd(), &rect, false);
  UpdateWindow(GetWindow().GetHwnd());
  // Determine the pixels that must be adapted:
  return 0;
}


LRESULT SmudgingState::Smudge3(int anX, int anY)
{
  Vector point(anX, anY);
  Vector direction = point - mLastPosition;

  bool bLeft = anX - mLastPosition.x < 0;
  bool bUp   = anY - mLastPosition.y < 0;

  int startx;
  int endx;
  int hIncrement;
  int starty;
  int endy;
  int vIncrement;

  if (bLeft)
  {
    startx      = anX - (GetAttributes().mThickness / 2);
    endx        = anX + (GetAttributes().mThickness / 2);
    hIncrement  = 1;
  }
  else
  {
    startx      = anX + (GetAttributes().mThickness / 2);
    endx        = anX - (GetAttributes().mThickness / 2);
    hIncrement  = -1;
  }

  if (bUp)
  {
    starty      = anY - (GetAttributes().mThickness / 3);
    endy        = anY + (GetAttributes().mThickness / 3);
    vIncrement  = 1;
  }
  else
  {
    starty      = anY + (GetAttributes().mThickness / 3);
    endy        = anY - (GetAttributes().mThickness / 3);
    vIncrement  = -1;
  }

  for (int i = startx; bLeft ? i <= endx : i >= endx; i += hIncrement)
  {
    for (int j = starty; bUp ? j <= endy : j >= endy; j += vIncrement)
    {
      int squaredistance = point.SquareDistance(Vector(i, j));
      double weight = (GetAttributes().mThickness * GetAttributes().mThickness * 0.25) / squaredistance;
      if (weight < 1)
      {
        weight = sqrt(weight);
        int red1   = GetRValue(GetPixel(GetWindow().GetHDC(), i, j));
        int red2   = GetRValue(GetPixel(GetWindow().GetHDC(), i - hIncrement, j));
        int red3   = GetRValue(GetPixel(GetWindow().GetHDC(), i, j - vIncrement));
        int red    = int(red1 * (1-weight) + ((red2 + red3) * (weight * 0.5)));
        int green1 = GetGValue(GetPixel(GetWindow().GetHDC(), i, j));
        int green2 = GetGValue(GetPixel(GetWindow().GetHDC(), i - hIncrement, j));
        int green3 = GetGValue(GetPixel(GetWindow().GetHDC(), i, j - vIncrement));
        int green  = int(green1 * (1-weight) + ((green2 + green3) * (weight * 0.5)));
        int blue1  = GetBValue(GetPixel(GetWindow().GetHDC(), i, j));
        int blue2  = GetBValue(GetPixel(GetWindow().GetHDC(), i - hIncrement, j));
        int blue3  = GetBValue(GetPixel(GetWindow().GetHDC(), i, j - vIncrement));
        int blue   = int(blue1 * (1-weight) + ((blue2 + blue3) * (weight * 0.5)));
        SetPixel(GetWindow().GetHDC(), i, j, RGB(red, green, blue));
      }
    }
  }

  mLastPosition.x = point.GetX();
  mLastPosition.y = point.GetY();

  RECT rect;
  rect.top    = anY - (GetAttributes().mThickness / 2);
  rect.bottom = anY + (GetAttributes().mThickness / 2);
  rect.left   = anX - (GetAttributes().mThickness / 2);
  rect.right  = anX + (GetAttributes().mThickness / 2);

  InvalidateRect(GetWindow().GetHwnd(), &rect, false);
  UpdateWindow(GetWindow().GetHwnd());
  // Determine the pixels that must be adapted:
  return 0;
}


LRESULT SmudgingState::Smudge(const Vector & aTarget, const Vector & aWith)
{
  bool bLeft = aTarget.GetX() - mLastPosition.x < 0;
  bool bUp   = aTarget.GetY() - mLastPosition.y < 0;

  Vector direction = aTarget - aWith;
  int startx;
  int endx;
  int hIncrement;
  int starty;
  int endy;
  int vIncrement;

  if (bLeft)
  {
    startx      = aTarget.GetX() - (GetAttributes().mThickness / 2);
    endx        = aTarget.GetX() + (GetAttributes().mThickness / 2);
    hIncrement  = 1;
  }
  else
  {
    startx      = aTarget.GetX() + (GetAttributes().mThickness / 2);
    endx        = aTarget.GetX() - (GetAttributes().mThickness / 2);
    hIncrement  = -1;
  }

  if (bUp)
  {
    starty      = aTarget.GetY() - (GetAttributes().mThickness / 2);
    endy        = aTarget.GetY() + (GetAttributes().mThickness / 2);
    vIncrement  = 1;
  }
  else
  {
    starty      = aTarget.GetY() + (GetAttributes().mThickness / 2);
    endy        = aTarget.GetY() - (GetAttributes().mThickness / 2);
    vIncrement  = -1;
  }

  for (int i = startx; bLeft ? i <= endx : i >= endx; i += hIncrement)
  {
    for (int j = starty; bUp ? j <= endy : j >= endy; j += vIncrement)
    {
      int squaredistance = aTarget.SquareDistance(Vector(i, j));
      double weight = squaredistance / (GetAttributes().mThickness * GetAttributes().mThickness * 0.25);
      if (weight < 1)
      {
        //weight = sqrt(weight);
        int red1   = GetRValue(GetPixel(GetWindow().GetHDC(), i, j));
        int red2   = GetRValue(GetPixel(GetWindow().GetHDC(), i - direction.GetX(), j - direction.GetY()));
        int red    = int(red1 * weight + (red2 * (1-weight)));
        int green1 = GetGValue(GetPixel(GetWindow().GetHDC(), i, j));
        int green2 = GetGValue(GetPixel(GetWindow().GetHDC(), i - direction.GetX(), j - direction.GetY()));
        int green  = int(green1 * weight + (green2 * (1-weight)));
        int blue1  = GetBValue(GetPixel(GetWindow().GetHDC(), i, j));
        int blue2  = GetBValue(GetPixel(GetWindow().GetHDC(), i - direction.GetX(), j - direction.GetY()));
        int blue   = int(blue1 * weight + (blue2 * (1-weight)));
        SetPixel(GetWindow().GetHDC(), i, j, RGB(red, green, blue));
      }
    }
  }

  return 0;
}


LRESULT SmudgingState::Smudge4(int anX, int anY)
{
  //TCHAR smsg[1024];
  //_stprintf(smsg, "### Smudge4: anX == %d anY == %d mLastPosition == %d, %d\n"), anX, anY, mLastPosition.x, mLastPosition.y);
  //OutputDebugString(smsg);
  Vector point(anX, anY);
  Vector direction(point - mLastPosition);

  int dlength(direction.GetLength());
  if (dlength /*< 2*/== 0)
    return -1;

  bool bLeft = anX - mLastPosition.x < 0;
  bool bUp   = anY - mLastPosition.y < 0;

  int startx;
  int endx;
  int hIncrement;
  int starty;
  int endy;
  int vIncrement;

//#define USE_THICKNESS

#ifdef USE_THICKNESS
  const int halfwidth = GetAttributes().mThickness / 2;
#else
  const int halfwidth = 8;
#endif

  if (bLeft)
  {
    startx      = point.GetX() - halfwidth;
    endx        = point.GetX() + halfwidth;
    hIncrement  = 1/* * (dlength + 3 / 4)*/;
  }
  else
  {
    startx      = point.GetX() + halfwidth;
    endx        = point.GetX() - halfwidth;
    hIncrement  = -1/* * (dlength + 3 / 4)*/;
  }

  if (bUp)
  {
    starty      = point.GetY() - halfwidth;
    endy        = point.GetY() + halfwidth;
    vIncrement  = 1/* * (dlength + 3 / 4)*/;
  }
  else
  {
    starty      = point.GetY() + halfwidth;
    endy        = point.GetY() - halfwidth;
    vIncrement  = -1/* * (dlength + 3 / 4)*/;
  }

  //_stprintf(smsg, "###          hIncrement == %d vIncrement %d\n"), hIncrement, vIncrement);
  //OutputDebugString(smsg);

  for (int i = startx; bLeft ? i <= endx : i >= endx; i += hIncrement)
  {
    for (int j = starty; bUp ? j <= endy : j >= endy; j += vIncrement)
    {
      int squaredistance = point.SquareDistance(Vector(i, j));
      double weight = squaredistance / (halfwidth * halfwidth);
      if (weight < 1)
      {
        int red     = 0;
        int green   = 0;
        int blue    = 0;
        int nrcols  = 0;

        for (int length = 1; length <= dlength; ++length)
        {
          red   += length * GetRValue(GetPixel(GetWindow().GetHDC(), i - direction.GetX() / length, j - direction.GetY() / length));
          green += length * GetGValue(GetPixel(GetWindow().GetHDC(), i - direction.GetX() / length, j - direction.GetY() / length));
          blue  += length * GetBValue(GetPixel(GetWindow().GetHDC(), i - direction.GetX() / length, j - direction.GetY() / length));
          nrcols += length;
        }

        SetPixel(GetWindow().GetHDC(), i, j, RGB(red / nrcols, green / nrcols, blue / nrcols));
      }
    }
  }

  mLastPosition.x = point.GetX();
  mLastPosition.y = point.GetY();

  RECT rect;
  rect.top    = anY - halfwidth;
  rect.bottom = anY + halfwidth;
  rect.left   = anX - halfwidth;
  rect.right  = anX + halfwidth;

  InvalidateRect(GetWindow().GetHwnd(), &rect, false);
  UpdateWindow(GetWindow().GetHwnd());
  // Determine the pixels that must be adapted:

  //OutputDebugString("### Smudge4 Done\n"));

  return 0;
}


LRESULT SmudgingState::Smudge6(int anX, int anY)
{
  HBITMAP hBitmap = GetWindow().GetBitmap();

  BITMAP bitmap;
  GetObject(hBitmap, sizeof(bitmap), &bitmap);

  int size = bitmap.bmWidthBytes * bitmap.bmHeight;
  BYTE * bitmapdata = new BYTE[size];

  GetBitmapBits(hBitmap, size, bitmapdata);

  int bpp = bitmap.bmBitsPixel >> 3;

  (void)bpp;

  //TCHAR smsg[1024];
  //_stprintf(smsg, "### Smudge4: anX == %d anY == %d mLastPosition == %d, %d\n"), anX, anY, mLastPosition.x, mLastPosition.y);
  //OutputDebugString(smsg);

  Vector point(anX, anY);
  Vector direction(point - mLastPosition);

  int dlength(direction.GetLength());
  if (dlength /*< 2*/== 0)
    return -1;

  bool bLeft = anX - mLastPosition.x < 0;
  bool bUp   = anY - mLastPosition.y < 0;

  int startx;
  int endx;
  int hIncrement;
  int starty;
  int endy;
  int vIncrement;

//#define USE_THICKNESS
//#ifdef USE_THICKNESS
  const int halfwidth = GetAttributes().mThickness / 2;
//#else
//  const int halfwidth = 8;
//#endif

  if (bLeft)
  {
    startx      = std::max(0, (point.GetX() - halfwidth) * bpp);
    endx        = std::min(bitmap.bmWidth - 1, LONG(point.GetX() + halfwidth)) * bpp;
    hIncrement  = 1 * bpp/* * (dlength + 3 / 4)*/;
  }
  else
  {
    startx      = std::min(bitmap.bmWidth - 1, LONG(point.GetX() + halfwidth)) * bpp;
    endx        = std::max(0, (point.GetX() - halfwidth) * bpp);
    hIncrement  = -1 * bpp/* * (dlength + 3 / 4)*/;
  }

  if (bUp)
  {
    starty      = std::max(0, point.GetY() - halfwidth);
    endy        = std::min(bitmap.bmHeight, LONG(point.GetY() + halfwidth));
    vIncrement  = 1/* * (dlength + 3 / 4)*/;
  }
  else
  {
    starty      = point.GetY() + halfwidth;
    endy        = point.GetY() - halfwidth;
    vIncrement  = -1/* * * (dlength + 3 / 4)*/;
  }

  //_stprintf(smsg, "###          hIncrement == %d vIncrement %d\n"), hIncrement, vIncrement);
  //OutputDebugString(smsg);

  for (int i = startx; bLeft ? i <= endx : i >= endx; i += hIncrement)
  {
    if (i < 0 || i > bitmap.bmWidthBytes)
      continue;

    for (int j = starty; bUp ? j <= endy : j >= endy; j += vIncrement)
    {
      if (j < 0 || j > bitmap.bmHeight)
        continue;
      int squaredistance = point.SquareDistance(Vector(i / bpp, j));
      double weight = squaredistance / halfwidth * halfwidth; //1;///*point.GetX() + point.GetY();*/ squaredistance / (halfwidth * halfwidth * bpp * bpp);
      if (weight <= 1)
      {
        int red     = 0;
        int green   = 0;
        int blue    = 0;
        int nrcols  = 0;

        for (int length = 1; length <= dlength; ++length)
        {
          int index = i - (direction.GetX() / length) * bpp + j * bitmap.bmWidthBytes - (direction.GetY() / length) * bpp;
          if (index >= size - 3)
            index = size - 4;
          red   += length * bitmapdata[index + 2];
          green += length * bitmapdata[index + 1];
          blue  += length * bitmapdata[index + 0];
          nrcols += length;
        }

        if (i + j * bitmap.bmWidthBytes >= size)
          continue;

        ////TCHAR smsg[1024];
        //if (i == startx && j == starty)
        //{
        //  _stprintf(smsg, "### Smudge6: *in* red == %d green == %d blue == %d nrcols == %d *out* red == %d green == %d blue === %d\n"),
        //                  bitmapdata[i + j * bitmap.bmWidthBytes + 2],
        //                  bitmapdata[i + j * bitmap.bmWidthBytes + 1],
        //                  bitmapdata[i + j * bitmap.bmWidthBytes + 0],
        //                  nrcols,
        //                  red / nrcols,
        //                  green / nrcols,
        //                  blue / nrcols);
        //  OutputDebugString(smsg);
        //}

        bitmapdata[i + j * bitmap.bmWidthBytes + 2] = static_cast<BYTE>(red / nrcols);
        bitmapdata[i + j * bitmap.bmWidthBytes + 1] = static_cast<BYTE>(green / nrcols);
        bitmapdata[i + j * bitmap.bmWidthBytes + 0] = static_cast<BYTE>(blue / nrcols);
      }
    }
  }

  SetBitmapBits(hBitmap, size, bitmapdata);
  mLastPosition.x = point.GetX();
  mLastPosition.y = point.GetY();

  RECT rect;
  rect.top    = anY - halfwidth;
  rect.bottom = anY + halfwidth;
  rect.left   = anX - halfwidth;
  rect.right  = anX + halfwidth;

  InvalidateRect(GetWindow().GetHwnd(), NULL/*&rect*/, false);
  UpdateWindow(GetWindow().GetHwnd());
  // Determine the pixels that must be adapted:

  //OutputDebugString("### Smudge4 Done\n"));

  delete[] bitmapdata;

  return 0;
}


class BitMapCell
{
public:
                  BitMapCell(BYTE * aByte, int aWidth) : mCell(aByte), mWidth(aWidth) {}

  BYTE            GetRed() const    { return *(mCell + 2); }
  BYTE            GetGreen() const  { return *(mCell + 1); }
  BYTE            GetBlue() const   { return *(mCell + 0); }

  void            SetRed(BYTE aValue)   { *(mCell + 2) = aValue; }
  void            SetGreen(BYTE aValue) { *(mCell + 1) = aValue; }
  void            SetBlue(BYTE aValue)  { *(mCell + 0) = aValue; }

private:
  BYTE *          mCell;
  int             mWidth;
};


class BitMapRow
{
public:
                  BitMapRow(BYTE * aRow, int aWidth, int aBytesPerPixel) : mRow(aRow), mWidth(aWidth), mBytesPerPixel(aBytesPerPixel) {}

  int             Size() const { return mWidth; }
  BitMapCell      operator[](int anIndex) { return BitMapCell(mRow + anIndex * mBytesPerPixel, mBytesPerPixel); }

private:
  BYTE *          mRow;
  int             mWidth;
  int             mBytesPerPixel;
};


class BitMap
{
public:
  BitMap(HBITMAP hBitmap);
  ~BitMap();

  int             Rows() const { return mHeight; }
  BitMapRow       operator[](int anIndex) { return BitMapRow(mBytes + mWidth * mBytesPerPixel * anIndex, mWidth, mBytesPerPixel); }

  int             GetWidth() const { return mWidth; }
  int             GetHeight() const { return mHeight; }
  int             GetBytesPerPixel() const { return mBytesPerPixel; }
  void            Commit();

private:
  HBITMAP         mBitMap;
  BYTE *          mBytes;
  int             mWidth; // In pixels
  int             mHeight; // in pixels
  int             mBytesPerPixel;
};


void BitMap::Commit()
{
  if (SetBitmapBits(mBitMap, mWidth * mHeight * mBytesPerPixel, mBytes) == 0)
  {
    DWORD derr = GetLastError();
    derr = derr;
  }
}


BitMap::BitMap(HBITMAP hBitmap)
: mBytes        (0),
  mBytesPerPixel(0),
  mBitMap       (hBitmap)
{
  BITMAP bitmap;
  GetObject(hBitmap, sizeof(bitmap), &bitmap);

  mWidth = bitmap.bmWidth;
  mHeight = bitmap.bmHeight;
  mBytesPerPixel = bitmap.bmBitsPixel >> 3;

  int size = mWidth * mHeight * mBytesPerPixel;
  mBytes = new BYTE[mWidth * mHeight * mBytesPerPixel];
  GetBitmapBits(hBitmap, size, mBytes);
}


BitMap::~BitMap()
{
  delete[] mBytes;
}


LRESULT SmudgingState::Smudge7(int anX, int anY)
{
  HBITMAP hBitmap = GetWindow().GetBitmap();

  BitMap bitmap(hBitmap);

  //TCHAR smsg[1024];
  //_stprintf(smsg, "### Smudge7: anX == %d anY == %d mLastPosition == %d, %d\n"), anX, anY, mLastPosition.x, mLastPosition.y);
  //OutputDebugString(smsg);

  Vector point(anX, anY);
  Vector direction(point - mLastPosition);

  int dlength(direction.GetLength());
  if (dlength /*< 2*/== 0)
    return -1;

  bool bLeft = anX - mLastPosition.x < 0;
  bool bUp   = anY - mLastPosition.y < 0;

  int startx;
  int endx;
  int hIncrement;
  int starty;
  int endy;
  int vIncrement;

  const int halfwidth = 200;

  if (bLeft)
  {
    startx      = std::max(0, (point.GetX() - halfwidth));
    endx        = std::min(bitmap.GetWidth() - 1, point.GetX() + halfwidth);
    hIncrement  = 1;
  }
  else
  {
    startx      = std::min(bitmap.GetWidth() - 1, point.GetX() + halfwidth);
    endx        = std::max(0, (point.GetX() - halfwidth));
    hIncrement  = -1;
  }

  if (bUp)
  {
    starty      = std::max(0, point.GetY() - halfwidth);
    endy        = std::min(bitmap.GetHeight(), point.GetY() + halfwidth);
    vIncrement  = 1;
  }
  else
  {
    starty      = std::min(bitmap.GetHeight(), point.GetY() + halfwidth);
    endy        = std::max(0, point.GetY() - halfwidth);
    vIncrement  = -1;
  }

  //_stprintf(smsg, "###          hIncrement == %d vIncrement %d\n"), hIncrement, vIncrement);
  //OutputDebugString(smsg);

  for (int irow = starty; bUp ? irow <= endy : irow >= endy; irow += vIncrement)
  {
    BitMapRow row(bitmap[irow]);

    for (int icolumn = startx; bLeft ? icolumn <= endx : icolumn >= endx; icolumn += hIncrement)
    {
      BitMapCell cell(row[icolumn]);

      int squaredistance = point.SquareDistance(Vector(icolumn, irow));
      double weight = squaredistance / halfwidth * halfwidth;
      if (weight <= 1)
      {
        int red     = 0;
        int green   = 0;
        int blue    = 0;
        int nrcols  = 0;

        for (int length = 1; length <= dlength; ++length)
        {
          int trow    = irow - direction.GetY() / length;
          int tcolumn = icolumn - direction.GetX() / length;

          if (trow >= 0 && trow < bitmap.GetHeight() && tcolumn >= 0 && tcolumn < bitmap.GetWidth())
          {
            red   += bitmap[trow][tcolumn].GetRed() * length;
            green += bitmap[trow][tcolumn].GetGreen() * length;
            blue  += bitmap[trow][tcolumn].GetBlue() * length;
            nrcols += length;
          }
        }

        ////TCHAR smsg[1024];
        //if (i == startx && j == starty)
        //{
        //  _stprintf(smsg, "### Smudge6: *in* red == %d green == %d blue == %d nrcols == %d *out* red == %d green == %d blue === %d\n"),
        //                  bitmapdata[i + j * bitmap.bmWidthBytes + 2],
        //                  bitmapdata[i + j * bitmap.bmWidthBytes + 1],
        //                  bitmapdata[i + j * bitmap.bmWidthBytes + 0],
        //                  nrcols,
        //                  red / nrcols,
        //                  green / nrcols,
        //                  blue / nrcols);
        //  OutputDebugString(smsg);
        //}

        if (nrcols > 0)
        {
          cell.SetRed(static_cast<BYTE>(red / nrcols));
          cell.SetGreen(static_cast<BYTE>(green / nrcols));
          cell.SetBlue(static_cast<BYTE>(blue / nrcols));
        }
      }
    }
  }

  bitmap.Commit();
  mLastPosition.x = point.GetX();
  mLastPosition.y = point.GetY();

  RECT rect;
  rect.top    = anY - halfwidth;
  rect.bottom = anY + halfwidth;
  rect.left   = anX - halfwidth;
  rect.right  = anX + halfwidth;

  InvalidateRect(GetWindow().GetHwnd(), NULL/*&rect*/, false);
  UpdateWindow(GetWindow().GetHwnd());
  //OutputDebugString("### Smudge4 Done\n"));

  return 0;
}


LRESULT SmudgingState::Smudge8(int anX, int anY)
{
  BitMap bitmap(GetWindow().GetBitmap());
  //TCHAR smsg[1024];
  //_stprintf(smsg, "### Smudge4: anX == %d anY == %d mLastPosition == %d, %d\n"), anX, anY, mLastPosition.x, mLastPosition.y);
  //OutputDebugString(smsg);
  Vector point(anX, anY);
  Vector direction(point - mLastPosition);

  int dlength(direction.GetLength());
  if (dlength /*< 2*/== 0)
    return -1;

  bool bLeft = anX - mLastPosition.x < 0;
  bool bUp   = anY - mLastPosition.y < 0;

  int startx;
  int endx;
  int hIncrement;
  int starty;
  int endy;
  int vIncrement;

//#define USE_THICKNESS

#ifdef USE_THICKNESS
  const int halfwidth = GetAttributes().mThickness / 2;
#else
  const int halfwidth = 8;
#endif

  if (bLeft)
  {
    startx      = point.GetX() - halfwidth;
    endx        = point.GetX() + halfwidth;
    hIncrement  = 1;
  }
  else
  {
    startx      = point.GetX() + halfwidth;
    endx        = point.GetX() - halfwidth;
    hIncrement  = -1;
  }

  if (bUp)
  {
    starty      = point.GetY() - halfwidth;
    endy        = point.GetY() + halfwidth;
    vIncrement  = 1;
  }
  else
  {
    starty      = point.GetY() + halfwidth;
    endy        = point.GetY() - halfwidth;
    vIncrement  = -1;
  }

  //_stprintf(smsg, "###          hIncrement == %d vIncrement %d\n"), hIncrement, vIncrement);
  //OutputDebugString(smsg);

  for (int j = starty; bUp ? j <= endy : j >= endy; j += vIncrement)
  {
    BitMapRow row(bitmap[j]);
    for (int i = startx; bLeft ? i <= endx : i >= endx; i += hIncrement)
    {
      BitMapCell cell(row[i]);
      int squaredistance = point.SquareDistance(Vector(i, j));
      double weight = squaredistance / (halfwidth * halfwidth);
      if (weight < 1)
      {
        int red     = 0;
        int green   = 0;
        int blue    = 0;
        int nrcols  = 0;

        for (int length = 1; length <= dlength; ++length)
        {
          int ncol = i - direction.GetX() / length;
          int nrow = j - direction.GetY() / length;
          red   += length * bitmap[nrow][ncol].GetRed();
          green += length * bitmap[nrow][ncol].GetGreen();
          blue  += length * bitmap[nrow][ncol].GetBlue();
          nrcols += length;
        }

        if (nrcols > 0)
        {
          cell.SetRed(static_cast<BYTE>(red / nrcols));
          cell.SetGreen(static_cast<BYTE>(green / nrcols));
          cell.SetBlue(static_cast<BYTE>(blue / nrcols));
        }
      }
    }
  }

  bitmap.Commit();

  mLastPosition.x = point.GetX();
  mLastPosition.y = point.GetY();

  RECT rect;
  rect.top    = anY - halfwidth;
  rect.bottom = anY + halfwidth;
  rect.left   = anX - halfwidth;
  rect.right  = anX + halfwidth;

  InvalidateRect(GetWindow().GetHwnd(), &rect, false);
  UpdateWindow(GetWindow().GetHwnd());

  return 0;
}
