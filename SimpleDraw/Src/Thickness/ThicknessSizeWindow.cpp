
#include "ThicknessSizeWindow.h"

#include "General/Controller.h"

#include "General/colours.h"


ThicknessPreview::ThicknessPreview(Window *aParent, HINSTANCE anInstance,
                                   DrawingAttributes & anAttributes)
: Preview     (aParent, anInstance, _T("ThicknessPreview"), WS_CHILD | WS_BORDER),
  mAttributes (anAttributes),
  mPen        (CreatePen(PS_SOLID, mAttributes.mThickness, mAttributes.mColour))
{
}


void ThicknessPreview::UpdatePen()
{
  DeleteObject(mPen);
  mPen = CreatePen(PS_SOLID, mAttributes.mThickness, mAttributes.mColour);

  InvalidateRect(GetHwnd(), NULL, false);
}


void ThicknessPreview::Larger()
{
  mAttributes.mThickness = (int)(mAttributes.mThickness * 1.1) + 1;
  UpdatePen();
}


void ThicknessPreview::Smaller()
{
  mAttributes.mThickness = (int)((mAttributes.mThickness / 1.1));
  UpdatePen();
}


void ThicknessPreview::SetThickness(int aThickness)
{
  mAttributes.mThickness = aThickness;
  UpdatePen();
}


void ThicknessPreview::SetColour(COLORREF aColour)
{
  mAttributes.mColour = aColour;

  DeleteObject(mPen);
  mPen = CreatePen(PS_SOLID, mAttributes.mThickness, mAttributes.mColour);

  InvalidateRect(GetHwnd(), NULL, false);
}


LRESULT ThicknessPreview::OnPaint()
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

  HGDIOBJ oldpen = SelectObject(dc, mPen);
  MoveToEx(dc, crect.left  + 4, (crect.top + crect.bottom) / 2, NULL);
  BOOL bresult = LineTo  (dc, crect.right - 4, (crect.top + crect.bottom) / 2);
  (void)bresult;
  SelectObject(dc, oldpen);

  EndPaint(GetHwnd(), &ps);
  return 0;
}


ThicknessSizeWindow::ThicknessSizeWindow(Window * aParent, HINSTANCE anInstance,
                                         DrawingAttributes & anAttributes)
: SizeWindow        (aParent, anInstance),
  mThicknessPreview (NULL),
  mAttributes       (anAttributes)
{
  mThicknessPreview = new ThicknessPreview(this, anInstance, mAttributes);

  SetPreview(mThicknessPreview);
}


int ThicknessSizeWindow::GetThickness() const
{
  return mAttributes.mThickness;
}


void ThicknessSizeWindow::SetThickness(int aThickness)
{
  mThicknessPreview->SetThickness(aThickness);
}


void ThicknessSizeWindow::SetColour(COLORREF aColour)
{
  mThicknessPreview->SetColour(aColour);
}


bool ThicknessSizeWindow::SetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx, int cy,
                                       UINT uFlags)
{
  return SizeWindow::SetWindowPos(hWndInsertAfter, X, Y, cx, cy, uFlags);
}

