#include "StampState.h"

#include <assert.h>

#include "StampSubMenu.h"
#include "Window/Definitions.h"
#include "Window/DrawingWindow.h"
#include "Window/HoverWIndow.h"
#include "Window/SpecialToolMenu.h"

StampState::StampState(DrawingWindow &      aWindow,
                       HINSTANCE            anInstance,
                       Controller &         aController,
                       SpecialToolMenu &    aMenu,
                       DrawingAttributes &  anAttributes)
: DrawingState(aWindow, anInstance, aController, anAttributes),
  mHoverWindow(NULL)
{
  aMenu.SetSubMenu(new StampSubMenu(aMenu, *this, anInstance, anAttributes));
}


StampState::~StampState()
{
  if (mHoverWindow != NULL)
    delete mHoverWindow;
}


void StampState::SetHoverWindow(HoverWindow * aWindow)
{
  FlushRedo();

  if (mHoverWindow == aWindow)
    return;

  if (mHoverWindow != NULL)
    delete mHoverWindow;

  mHoverWindow = aWindow;
}


LRESULT StampState::OnHoverWindowClicked(HoverWindow * aWindow)
{
  if (aWindow == mHoverWindow)
  {
    POINT p;
    GetCursorPos(&p);
    RECT rect;
    GetWindow().GetWindowRect(rect);
    if (PtInRect(&rect, p))
    {
      aWindow->SetState(HoverWindow::ST_Size);
    }
    else
    {
      delete mHoverWindow;
      mHoverWindow = NULL;
    }
  }

  return 0;
}


LRESULT StampState::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  (void)wParam;
  switch (uMsg)
  {
    case WinDefs::WM_HOVERWINDOW_CLICKED:
      return OnHoverWindowClicked((HoverWindow *)lParam);
  }

  return -1;
}
