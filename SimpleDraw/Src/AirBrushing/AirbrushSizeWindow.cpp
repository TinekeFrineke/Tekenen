
#include "AirbrushSizeWindow.h"

#include "General/Controller.h"

#include "General/colours.h"


AirbrushPreview::AirbrushPreview(Window *aParent, HINSTANCE anInstance,
                                 DrawingAttributes & anAttributes)
: Preview     (aParent, anInstance, "AirbrushPreview", WS_CHILD | WS_BORDER),
  mAttributes (anAttributes)/*,
  mPen        (CreatePen(PS_SOLID, mAttributes.mThickness, mAttributes.mColour))*/
{
}


void AirbrushPreview::UpdatePen()
{
  //DeleteObject(mPen);
  //mPen = CreatePen(PS_SOLID, mAttributes.mThickness, mAttributes.mColour);

  InvalidateRect(GetHwnd(), NULL, false);
}


void AirbrushPreview::Larger()
{
  mAttributes.mThickness = (int)(mAttributes.mThickness * 1.1) + 1;
  UpdatePen();
}


void AirbrushPreview::Smaller()
{
  mAttributes.mThickness = (int)((mAttributes.mThickness / 1.1));
  UpdatePen();
}


void AirbrushPreview::SetThickness(int aThickness)
{
  mAttributes.mThickness = aThickness;
  UpdatePen();
}


void AirbrushPreview::SetColour(COLORREF aColour)
{
  mAttributes.mColour = aColour;

  //DeleteObject(mPen);
  //mPen = CreatePen(PS_SOLID, mAttributes.mThickness, mAttributes.mColour);

  InvalidateRect(GetHwnd(), NULL, false);
}


LRESULT AirbrushPreview::OnPaint()
{
  PAINTSTRUCT ps;
  HDC dc = BeginPaint(GetHwnd(), &ps);

  RECT crect;
  GetClientRect(crect);

  {
    HBRUSH backbrush = CreateSolidBrush(GetBackgroundColour(mAttributes.mColour));
    FillRect(dc, &crect, backbrush);
    DeleteObject(backbrush);
  }

  //SYSTEMTIME systemtime;
  //GetSystemTime(&systemtime);
  //srand(systemtime.wMilliseconds);
  //srand(int(time(NULL)));
  int radius = mAttributes.mThickness >> 1;
  POINT center = { (crect.left + crect.right) >> 1, (crect.top + crect.bottom) >> 1 };
  for (int row = center.y - radius; row <= center.y + radius; ++row) {
    for (int column = center.x - radius; column <= center.x + radius; ++column) {
      int distance = (column - center.x) * (column - center.x) + (row - center.y) * (row - center.y);
      float lengthfactor = float(distance) / (radius * radius);
      if (lengthfactor < 1)
        if (rand() % 107 < 50 * (1 - lengthfactor))
          SetPixel(dc, column, row, mAttributes.mColour);
    }
  }

  EndPaint(GetHwnd(), &ps);
  return 0;
}


AirbrushSizeWindow::AirbrushSizeWindow(Window * aParent, HINSTANCE anInstance,
                                       DrawingAttributes & anAttributes)
: SizeWindow        (aParent, anInstance),
  mThicknessPreview (NULL),
  mAttributes       (anAttributes)
{
  mThicknessPreview = new AirbrushPreview(this, anInstance, mAttributes);

  SetPreview(mThicknessPreview);
}


int AirbrushSizeWindow::GetThickness() const
{
  return mAttributes.mThickness;
}


void AirbrushSizeWindow::SetThickness(int aThickness)
{
  mThicknessPreview->SetThickness(aThickness);
}


void AirbrushSizeWindow::SetColour(COLORREF aColour)
{
  mThicknessPreview->SetColour(aColour);
}


bool AirbrushSizeWindow::SetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx, int cy,
                                       UINT uFlags)
{
  return SizeWindow::SetWindowPos(hWndInsertAfter, X, Y, cx, cy, uFlags);
}

