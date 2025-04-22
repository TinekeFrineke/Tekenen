#include "LetterState.h"

#include<assert.h>

#include "LetterSubMenu.h"
#include "Window/Definitions.h"
#include "Window/DrawingWindow.h"
#include "Window/LetterWIndow.h"
#include "Window/SpecialToolMenu.h"

LetterState::LetterState(DrawingWindow &      aWindow,
                         HINSTANCE            anInstance,
                         Controller &         aController,
                         SpecialToolMenu &    aMenu,
                         DrawingAttributes &  anAttributes)
: DrawingState(aWindow, anInstance, aController, anAttributes),
  mState      (ST_Idle),
  mWindow     (NULL),
  mSubMenu    (NULL)
{
  mSubMenu = new LetterSubMenu(aMenu, *this, anInstance, anAttributes);
  aMenu.SetSubMenu(mSubMenu);

  mSubMenu->Attach(this);
}


LetterState::~LetterState()
{
}


LRESULT LetterState::OnLeftButtonUp(int aVirtualKey, int anX, int anY)
{
  (void) aVirtualKey;
  (void)anX;
  (void)anY;
  if (mState == ST_Idle)
  {
    mOldCursor = SetCursor(NULL);

    mWindow = new LetterWindow(GetWindow(), GetInstance(), mSubMenu->GetFont(), GetAttributes());
    mWindow->SetWindowPos(NULL, anX, anY, 1, mSubMenu->GetFont().GetSize(), SWP_SHOWWINDOW);
    mState = ST_Writing;
  }
  else
  {
    KillLetterWindow();
    SetCursor(mOldCursor);
  }

  return 0;
}


void LetterState::KillLetterWindow()
{
  if (mWindow == NULL)
    return;

  mWindow->Finalize();
  delete mWindow;
  mWindow = NULL;
  mState = ST_Idle;

  // Copy the state up to now
  Store();
}


LRESULT LetterState::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  (void)wParam;
  switch (uMsg)
  {
    case WM_LBUTTONDOWN:
      FlushRedo();
      return -1;

    case WM_LBUTTONUP:
      return OnLeftButtonUp((int)wParam, (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)) == 0;

    case WinDefs::WM_LETTERWINDOW_READY:
      KillLetterWindow();
      return 0;
  }

  return -1;
}


void LetterState::Update(Subject * aSubject)
{
  if (aSubject == mSubMenu)
  {
    if (mWindow != NULL)
    {
      mWindow->SetFont(mSubMenu->GetFont());
      InvalidateRect(mWindow->GetHwnd(), NULL, true);
    }
  }
}


void LetterState::Destroyed(Subject * aSubject)
{
  (void)aSubject;
}
