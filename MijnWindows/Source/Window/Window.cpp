#include "Window.h"

#include <stdio.h>

Window::Window(Window * aParent, HINSTANCE hInstance, const std::tstring & aName)
: mParent     (aParent),
  mHinstance  (hInstance),
  mName       (aName),
#if PRINT_MESSAGES > 1
  mDebug      (true),
#elif PRINT_MESSAGES > 0
  mDebug      (false),
#endif
  mHwnd       (NULL)
{
#if PRINT_MESSAGES > 1
  TCHAR msg[1024];
  _stprintf(msg, _T("Constructing window %s address = 0x%08x parent == %s\n"),
            aName.c_str(), this, aParent ? aParent->mName.c_str() : _T("NULL"));
  OutputDebugString(msg);
#endif
  if (aParent)
    aParent->AddChild(this);
}


Window::~Window()
{
#if PRINT_MESSAGES > 1
  TCHAR msg[1024];
  _stprintf(msg, _T("Destructor window %s 0x%08x 1\n"), mName.c_str(), this);
  OutputDebugString(msg);
#endif

  if (mHwnd != NULL)
    SetWindowLong(mHwnd, GWL_USERDATA, NULL);

  while (!mChildren.empty())
    delete *mChildren.begin();

  if (GetParent())
    GetParent()->RemoveChild(this);

  if (mHwnd)
  {
    DestroyWindow(mHwnd);
    mHwnd = NULL;
  }
#if PRINT_MESSAGES > 1
  _stprintf(msg, _T("Destructor window %s 2\n"), mName.c_str());
  OutputDebugString(msg);
#endif
}


bool Window::Show(int nCmdShow)
{
  ShowWindow(mHwnd, nCmdShow);
  return true;
}


bool Window::GetClientRect(RECT & oRect)
{
  return ::GetClientRect(GetHwnd(), &oRect) != FALSE;
}


bool Window::GetWindowRect(RECT & oRect)
{
  return ::GetWindowRect(GetHwnd(), &oRect) != FALSE;
}


LRESULT Window::SendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  return ::SendMessage(GetHwnd(), uMsg, wParam, lParam);
}


LRESULT Window::PostMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  return ::PostMessage(GetHwnd(), uMsg, wParam, lParam);
}


bool Window::SetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx, int cy,
                          UINT uFlags)
{
  return ::SetWindowPos(mHwnd, hWndInsertAfter, X, Y, cx, cy, uFlags) != FALSE;
}


void Window::AddChild(Window * aWindow)
{
  mChildren.push_back(aWindow);
}


void Window::RemoveChild(Window * aWindow)
{
  mChildren.remove(aWindow);
}


bool Window::SetParent(Window * aParent)
{
  if (mParent != NULL)
    mParent->RemoveChild(this);

  mParent = aParent;
  if (mParent != NULL)
    mParent->AddChild(this);

  if (mHwnd)
    ::SetParent(mHwnd, aParent ? aParent->GetHwnd() : NULL);

  return true;
}


bool Window::Create(const std::tstring & aClassName,
                    const std::tstring & aName,
                    DWORD aStyle, DWORD anExStyle,
                    int aLeft, int aTop, int aWidth, int aHeight,
                    HMENU hMenu,
                    LPVOID aParam)
{
  FILE* fp(NULL);
  errno_t err(fopen_s(&fp, "d:\\tineke.txt", "a")); (void)err;
  if (!fp)
    ::MessageBox(0, _T("fp == NULL"), L"ERROR", MB_OK);

  if (fp) {
    _ftprintf(fp, _T("Window::Create(\"%s\", \"%s\", 0x%08x, 0x%08x, %d, %d, %d, %d, NULL 0x%08p)\n"),
                 aClassName.c_str(), aName.c_str(), aStyle, anExStyle, aLeft, aTop, aWidth, aHeight, aParam);
  }
  if (mHwnd != NULL)
  {
    ::MessageBox(0, (_T("Double create for window ") + mName).c_str(), L"ERROR", MB_OK);
    throw 0;
  }

  if (fp) {
    _ftprintf(fp, _T("CreateWindowEx(0x%08x %s %s 0x%08x %d %d %d %d 0x%08p)\n"),
                      anExStyle, aClassName.c_str(), aName.c_str(), aStyle, aLeft, aTop, aWidth, aHeight,
                      GetParent() ? GetParent()->GetHwnd() : NULL);
    fclose(fp);
  }

  mHwnd = CreateWindowEx(anExStyle, aClassName.c_str(), aName.c_str(), aStyle,
                         aLeft, aTop, aWidth, aHeight,
                         GetParent() ? GetParent()->GetHwnd() : NULL,
                         hMenu,
                         GetInstance(), NULL);

#if PRINT_MESSAGES > 1
  TCHAR msg[1024];
  _stprintf(msg, _T("Creating window %s HWND == 0x%08x\n"), mName.c_str(), mHwnd);
  OutputDebugString(msg);
#endif

  if (mHwnd == NULL)
  {
    ::MessageBox(0, (_T("Error creating window ") + mName).c_str(), L"ERROR", MB_OK);
  }

#pragma warning (push)
#pragma warning (disable:4311)
  SetWindowLong(mHwnd, GWL_USERDATA, LONG(this));
#pragma warning (pop)

#if PRINT_MESSAGES > 1
  TCHAR smsg[1024];
  _stprintf(smsg, _T("Created window HWND == 0x%08x class == %s name == %s\n"),
            GetHwnd(), aClassName.c_str(), aName.c_str());
  OutputDebugString(smsg);
#endif

  return true;
}


LRESULT Window::OnWindowPosChanged(const WINDOWPOS & oPos)
{
  (void)oPos;
  return 0;
}


LRESULT Window::OnPaint()
{
  return DefWindowProc(mHwnd, WM_PAINT, 0, 0);
}


LRESULT Window::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
    case WM_WINDOWPOSCHANGED:
      return OnWindowPosChanged(*(WINDOWPOS *)lParam);

    case WM_PAINT:
      return OnPaint();

    default:
      return DefWindowProc(GetHwnd(), uMsg, wParam, lParam);
  }
}


LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  if (uMsg == WM_LBUTTONDOWN)
    uMsg = uMsg;

#pragma warning (push)
#pragma warning (disable:4312)
  Window * wnd = (Window *)GetWindowLong(hwnd, GWLP_USERDATA);
#pragma warning (pop)
  if (wnd != NULL)
  {
#if PRINT_MESSAGES > 0
    if (wnd->mDebug)
    {
      TCHAR msgname[128];
      switch (uMsg)
      {
      case WM_CHAR:
        _stprintf(msgname, _T("WM_CHAR"));
        break;
      case WM_PAINT:
        _stprintf(msgname, _T("WM_PAINT"));
        break;
      case WM_LBUTTONDOWN:
        _stprintf(msgname, _T("WM_LBUTTONDOWN"));
        break;
      case WM_LBUTTONUP:
        _stprintf(msgname, _T("WM_LBUTTONUP"));
        break;
      case WM_PARENTNOTIFY:
        _stprintf(msgname, _T("WM_PARENTNOTIFY"));
        break;
      case WM_SETCURSOR:
        _stprintf(msgname, _T("WM_SETCURSOR"));
        break;
      case WM_NCHITTEST:
        _stprintf(msgname, _T("WM_NCHITTEST"));
        break;
      case WM_MOUSEACTIVATE:
        _stprintf(msgname, _T("WM_MOUSEACTIVATE"));
        break;
      case WM_MOUSEMOVE:
        _stprintf(msgname, _T("WM_MOUSEMOVE"));
        break;
      case WM_KEYUP:
        _stprintf(msgname, _T("WM_KEYUP"));
        break;
      case WM_SYSKEYDOWN:
        _stprintf(msgname, _T("WM_SYSKEYDOWN"));
        break;
      case WM_CANCELMODE:
        _stprintf(msgname, _T("WM_CANCELMODE"));
        break;
      case WM_SYSKEYUP:
        _stprintf(msgname, _T("WM_SYSKEYUP"));
        break;
      case WM_NCACTIVATE:
        _stprintf(msgname, _T("WM_NCACTIVATE"));
        break;
      case WM_GETTEXT:
        _stprintf(msgname, _T("WM_GETTEXT"));
        break;
      case WM_ACTIVATE:
        _stprintf(msgname, _T("WM_ACTIVATE"));
        break;
      case WM_ACTIVATEAPP:
        _stprintf(msgname, _T("WM_ACTIVATEAPP"));
        break;
      case WM_WINDOWPOSCHANGING:
        _stprintf(msgname, _T("WM_WINDOWPOSCHANGING"));
        break;
      case WM_WINDOWPOSCHANGED:
        _stprintf(msgname, _T("WM_WINDOWPOSCHANGED"));
        break;
      case WM_NCCALCSIZE:
        _stprintf(msgname, _T("WM_NCCALCSIZE"));
        break;
      case WM_CHILDACTIVATE:
        _stprintf(msgname, _T("WM_CHILDACTIVATE"));
        break;
      case WM_KILLFOCUS:
        _stprintf(msgname, _T("WM_KILLFOCUS"));
        break;
      case WM_SETFOCUS:
        _stprintf(msgname, _T("WM_SETFOCUS"));
        break;
      case WM_ERASEBKGND:
        _stprintf(msgname, _T("WM_ERASEBKGND"));
        break;
      case WM_NCPAINT:
        _stprintf(msgname, _T("WM_NCPAINT"));
        break;
      case WM_CAPTURECHANGED:
        _stprintf(msgname, _T("WM_CAPTURECHANGED"));
        break;
      default:
        _stprintf(msgname, _T("0x%08x"), uMsg);
      }

      TCHAR msg[1024];
      _stprintf(msg, _T("WindowProc for message %s window %s address = 0x%08x\n"),
                msgname, wnd->GetName().c_str(), wnd);
      OutputDebugString(msg);
    }
#endif
    return wnd->WindowProc(uMsg, wParam, lParam);
  }

  return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

template<>
bool MijnWindow<GenericWindow>::mRegistered = false;

GenericWindow::GenericWindow(Window * aParent, HINSTANCE hInstance,
                             const std::tstring & aName, DWORD dwStyle, DWORD dwExStyle)
: MijnWindow<GenericWindow>(aParent, hInstance, aName)
{
  Create(GetClassName(), aName, dwStyle, dwExStyle);
}



bool GenericWindow::RegisterClass(HINSTANCE hInstance)
{
  WNDCLASS sWndClass;
  memset(&sWndClass, 0, sizeof(WNDCLASS));
  sWndClass.lpszClassName = GetClassName();
  sWndClass.lpfnWndProc = Window::WindowProc;
  sWndClass.hInstance = hInstance;

  return ::RegisterClass(&sWndClass) != NULL;
}
