
#include "LetterWindow.h"

#include "General/Controller.h"
#include "Window/Definitions.h"
#include "Window/DrawingWindow.h"


LetterWindow::LetterWindow(DrawingWindow & aParent, HINSTANCE hInstance, const Font & aFont,
                           const DrawingAttributes & anAttributes)
: GenericWindow (&aParent, hInstance, _T("LetterWindow"), WS_CHILD, WS_EX_TRANSPARENT),
  mWindow       (aParent),
  mAttributes   (anAttributes),
  mFont         (aFont)
{
  mOldFocusWnd = SetFocus(GetHwnd());
  ShowWindow(GetHwnd(), SW_SHOW);

#if PRINT_MESSAGES > 0
  SetDebug(true);
#endif
}


LetterWindow::~LetterWindow()
{
  //SetCursor(mOldCursor);
  SetFocus(mOldFocusWnd);
}


void LetterWindow::SetFont(const Font & aFont)
{
  if (mFont == aFont)
    return;

  mFont = aFont;
  UpdateWindowSize();
  InvalidateRect(GetHwnd(), NULL, false);
}


void LetterWindow::Finalize()
{
  // Convert coordinates to drawing window coordinates
  RECT rect;
  GetClientRect(rect);
  POINT upperleft  = { rect.left,  rect.top };

  ClientToScreen(GetHwnd(), &upperleft);
  ScreenToClient(mWindow.GetHwnd(), &upperleft);

  // Select the font handle into the dc
  HDC hDC = mWindow.GetHDC();
  HFONT hFont = mFont.GetHandle();
  if (hFont == 0)
  {
    ::MessageBox(GetHwnd(), (_T("Error creating font ") + mFont.GetName()).c_str(), _T("ERROR"), MB_OK);
    return;
  }

  HGDIOBJ oldfont = SelectObject(hDC, hFont);

  // Determine the size of rectangle needed
  RECT rect2 = { 0, 0, 1, 1 };
  ::DrawText(hDC, mText.c_str(), -1, &rect2, DT_CALCRECT | DT_SINGLELINE);

  SetTextColor(hDC, mAttributes.mColour);
  SetBkMode(hDC, TRANSPARENT);

  rect2.left += upperleft.x;
  rect2.right += upperleft.x;
  rect2.top += upperleft.y;
  rect2.bottom += upperleft.y;

  ::DrawText(hDC, mText.c_str(), -1, &rect2, DT_SINGLELINE);

  InvalidateRect(mWindow.GetHwnd(), &rect2, true);
  SelectObject(hDC, oldfont);
}


LRESULT LetterWindow::OnPaint()
{
  PAINTSTRUCT ps;
  HDC dc = BeginPaint(GetHwnd(), &ps);

  if (mFont.GetHandle() == NULL)
    ::MessageBox(NULL, _T("Ongeldige tekenset"), _T("FOUT"), MB_OK);

  HGDIOBJ oldfont = mFont.GetHandle() ? SelectObject(dc, mFont.GetHandle()) : 0;

  RECT rect = { 0, 0, 1, 1 };
  ::DrawText(dc, mText.c_str(), -1, &rect, DT_CALCRECT | DT_SINGLELINE);
  DrawText(dc);
  if (mFont.GetHandle())
    SelectObject(dc, oldfont);

  EndPaint(GetHwnd(), &ps);
  return 0;
}


void LetterWindow::UpdateWindowSize()
{
  if (mText.empty())
  {
    SetWindowPos(NULL, 0, 0, 0, mFont.GetSize(), SWP_NOMOVE);
  }
  else
  {
    HDC dc = GetDC(GetHwnd());
    HGDIOBJ oldfont = mFont.GetHandle()
                        ? SelectObject(dc, mFont.GetHandle())
                        : NULL;

    RECT rect = { 0, 0, 1, 1 };
    ::DrawText(dc, mText.c_str(), -1, &rect, DT_CALCRECT | DT_SINGLELINE);

    SetWindowPos(NULL, 0, 0, rect.right, rect.bottom, SWP_NOMOVE);

    if (mFont.GetHandle())
      SelectObject(dc, oldfont);
  }
}


void LetterWindow::DrawText(HDC aDc)
{
  HPEN dotpen = CreatePen(PS_DOT, 1, mAttributes.mColour);
  HGDIOBJ oldpen = SelectObject(aDc, dotpen);

  RECT rect;
  GetClientRect(rect);
  if (mText.empty())
  {
    MoveToEx(aDc, 0, 0, NULL);
    LineTo(aDc, 0, rect.bottom);
  }
  else
  {
    SetTextColor(aDc, mAttributes.mColour);
    SetBkMode(aDc, TRANSPARENT);
    ::DrawText(aDc, mText.c_str(), -1, &rect, DT_SINGLELINE);

    HGDIOBJ oldbrush = SelectObject(aDc, GetStockObject(HOLLOW_BRUSH));
    Rectangle(aDc, rect.left, rect.top, rect.right, rect.bottom);
    SelectObject(aDc, oldbrush);
  }

  SelectObject(aDc, oldpen);
  DeleteObject(dotpen);
}


LRESULT LetterWindow::OnChar(char c)
{
  switch (c)
  {
    case '\b':
      if (mText.length() > 0)
        mText.resize(mText.length() - 1);
      break;

      case '\r':
      case '\n':
        return GetParent()->PostMessage(WinDefs::WM_LETTERWINDOW_READY, 0, (LPARAM)this);

    default:
      mText += wchar_t(c);
      break;
  }

  UpdateWindowSize();
  InvalidateRect(GetHwnd(), NULL, false);
  return 0;
}


LRESULT LetterWindow::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  (void)uMsg;
  (void)wParam;
  (void)lParam;

  switch (uMsg)
  {
  case WM_CHAR:
    return OnChar(char(wParam));

  case WM_KILLFOCUS:
    return GetParent()->PostMessage(WinDefs::WM_LETTERWINDOW_READY, 0, (LPARAM)this);

  default:
    return GenericWindow::WindowProc(uMsg, wParam, lParam);
  }
}
