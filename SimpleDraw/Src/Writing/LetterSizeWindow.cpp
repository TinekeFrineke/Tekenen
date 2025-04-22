
#include "LetterSizeWindow.h"

#include "General/Controller.h"
#include "General/Colours.h"

#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif



LetterPreview::LetterPreview(Window * aParent, HINSTANCE anInstance,
                             Font & aFont, const DrawingAttributes & anAttributes)
: Preview     (aParent, anInstance, _T("LetterPreview")),
  mFont       (aFont),
  mAttributes (anAttributes)
{
}


void LetterPreview::Larger()
{
  mFont.SetSize(int(mFont.GetSize() * 1.2));
  InvalidateRect(GetHwnd(), NULL, false);
}


void LetterPreview::Smaller()
{
  if (mFont.GetSize() > 8)
  {
    int newsize = int(mFont.GetSize() * 0.8);
    mFont.SetSize(max(newsize, 8));
    InvalidateRect(GetHwnd(), NULL, false);
  }
}


LRESULT LetterPreview::OnPaint()
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

  int cwidth = crect.right - crect.left;
  int cheight = crect.bottom - crect.top;

  HFONT hFont = mFont.GetHandle();
  HGDIOBJ oldfont = hFont ? SelectObject(dc, mFont.GetHandle()) : 0;

  RECT rect = { 0, 0, -1, -1 };
  DrawText(dc, _T("ABCDE"), -1, &rect, DT_CALCRECT | DT_SINGLELINE);

  int hmargin = (cwidth - rect.right + rect.left) / 2;
  int vmargin = (cheight - rect.bottom + rect.top) / 2;

  rect.left += hmargin;
  rect.right += hmargin;
  rect.top += vmargin;
  rect.bottom += vmargin;

  SetBkMode(dc, TRANSPARENT);
  COLORREF oldcolour = SetTextColor(dc, mAttributes.mColour);
  DrawText(dc, _T("ABCDE"), -1, &rect, DT_SINGLELINE);
  SetTextColor(dc, oldcolour);

  if (hFont)
    SelectObject(dc, oldfont);

  EndPaint(GetHwnd(), &ps);
  return 0;
}
