#include "Dialog.h"

#include <assert.h>


template<>
bool MijnWindow<Dialog>::mRegistered = false;


Dialog::Dialog(Window * aParent, HINSTANCE hInstance,
               const std::string & aName, DWORD style)
: MijnWindow<Dialog>(aParent, hInstance, aName),
  mResult           (-1),
  mReady            (false)
{
  assert((style & WS_CHILD) == 0);
  Create(GetClassName(), aName.c_str(), WS_DLGFRAME | WS_THICKFRAME/*WS_OVERLAPPED*//*WS_POPUP*/ | (style & ~WS_CHILD));
  //mFrame = new FrameWindow(aParent, hInstance);
}


int Dialog::Execute()
{
  EnableWindow(GetParent()->GetHwnd(), false);

  ShowModal();

	MSG msg;
  for (; !mReady; WaitMessage())
  {
    while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
    {
      if (msg.message == WM_QUIT)
      {
        mReady = TRUE;
        ::PostMessage(NULL,WM_QUIT,0,0);
        break;
      }

#if PRINT_MESSAGES > 0
      wchar_t smsg[1024] = {0};
      wchar_t wndname[1024] = {0};
      swprintf(wndname, L"0x%08x", msg.hwnd);

      if (msg.message == WM_PAINT)
        swprintf(smsg, L"HWND == %s Message == WM_PAINT\n", wndname);
      else if (msg.message == WM_LBUTTONUP)
        swprintf(smsg, L"HWND == %s Message == WM_LBUTTONUP\n", wndname);
      else if (msg.message == WM_LBUTTONDOWN)
        swprintf(smsg, L"HWND == %s Message == WM_LBUTTONDOWN\n", wndname);
      if (_tcslen(smsg) > 0)
        OutputDebugString(smsg);
#endif // PRINT_MESSAGES > 0

      if (!IsDialogMessage(GetHwnd(), &msg))
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
  }

  EnableWindow(GetParent()->GetHwnd(), true);
  DestroyWindow(GetHwnd());

  return mResult;
}


bool Dialog::RegisterClass(HINSTANCE hInstance)
{
  WNDCLASS wndclass;
  memset(&wndclass, 0, sizeof(WNDCLASS));

  wndclass.hInstance = hInstance;
  wndclass.lpfnWndProc = WindowProc;
  wndclass.lpszClassName = GetClassName(); // L"Dialog";
  wndclass.style = CS_HREDRAW | CS_VREDRAW;
  wndclass.hbrBackground = (HBRUSH)GetSysColorBrush(COLOR_HIGHLIGHT);
  //wndclass.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));

  if (!::RegisterClass(&wndclass)) {
    DWORD err = GetLastError();
    err = err;
    return false;
  }

  return true;
}
