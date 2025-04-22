#include "FrameWindow.h"

#include <stdio.h>
#include <tchar.h>

//#include "resource.h"

template<>
bool MijnWindow<FrameWindow>::mRegistered = false;

FrameWindow::FrameWindow(Window * aParent, HINSTANCE hInstance,
                         const std::tstring & aName, DWORD aStyle)
: MijnWindow<FrameWindow> (aParent, hInstance, aName),
  mColour                 (0xffffff)
{
  aStyle &= ~WS_POPUP;
  aStyle |= WS_CHILD;
  Create(GetClassName(), _T("FrameWindow"), aStyle);
  Show(SW_SHOW);
}


FrameWindow::~FrameWindow()
{
}


LRESULT FrameWindow::OnPaint()
{
  PAINTSTRUCT ps;
  HDC dc = BeginPaint(GetHwnd(), &ps);

  HBRUSH hBrush = CreateSolidBrush(mColour);
  FillRect(dc, &ps.rcPaint, hBrush);
  DeleteObject(hBrush);

  EndPaint(GetHwnd(), &ps);
  return 0;
}


bool FrameWindow::RegisterClass(HINSTANCE hInstance)
{
  WNDCLASS sWndClass;
  memset(&sWndClass, 0, sizeof(WNDCLASS));
  sWndClass.hbrBackground = HBRUSH(GetStockObject(WHITE_BRUSH));
  sWndClass.lpszClassName = GetClassName();
  sWndClass.lpfnWndProc = MijnWindow<FrameWindow>::WindowProc;
  //sWndClass.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND));
  sWndClass.hInstance = hInstance;

  return ::RegisterClass(&sWndClass) != NULL;
}


void FrameWindow::SetColour(COLORREF aColour)
{
  mColour = aColour;
}
