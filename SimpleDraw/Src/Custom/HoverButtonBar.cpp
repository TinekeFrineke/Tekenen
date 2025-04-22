#include "HoverButtonBar.h"

#include <assert.h>

#include "Resource/resource.h"

#include "Button/HoverButton.h"
#include "Button/BitmapButton.h"
#include "Control/ButtonBar.h"
#include "Stamping/StampState.h"

#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif



HoverButtonBar::HoverButtonBar(Window * aParent, HINSTANCE hInstance,
                               StampState & aState,
                               int aRows, int aColumns)
: FrameWindow (aParent, hInstance, _T("HoverButtonBar")),
  mState      (aState),
  mRows       (aRows),
  mColumns    (aColumns),
  mBack       (NULL),
  mForward    (NULL)
{
  mBack     = new SingleBitmapButton(this, hInstance, true, _T("BackButton"));
  mForward  = new SingleBitmapButton(this, hInstance, true, _T("ForwardButton"));

  mBack->SetBitmap(IDB_ARROW_LEFT);
  mForward->SetBitmap(IDB_ARROW_RIGHT);

  for (int row = 0; row < aRows; ++row)
  {
    ButtonBar * buttonbar = new ButtonBar(this, hInstance,
                                          _T("HoverButtonBarButtonBar"),
                                          ButtonBar::LS_HorizontalFit);
    mButtonBars.push_back(buttonbar);

    for (int column = 0; column < aColumns; ++column)
      AddButton(buttonbar, new HoverButton(buttonbar, hInstance, mState, 0, 0));
  }
}


void HoverButtonBar::FillButtons()
{

  for (size_t i = 0; i < mButtons.size(); ++i)
  {
    if (i + mCurrentIndex < mFiles.size())
      mButtons[i]->SetFile(mFiles[i + mCurrentIndex]);
    else
      mButtons[i]->SetFile(_T(""));

    InvalidateRect(mButtons[i]->GetHwnd(), NULL, false);
  }
}


void HoverButtonBar::ScrollForward()
{
  mCurrentIndex = min(mCurrentIndex + mRows * mColumns, int(mFiles.size()) - mRows * mColumns);
  mCurrentIndex = max(mCurrentIndex, 0);

  FillButtons();
}


void HoverButtonBar::ScrollBack()
{
  mCurrentIndex -= mRows * mColumns;
  mCurrentIndex = max(mCurrentIndex, 0);

  FillButtons();
}


LRESULT HoverButtonBar::OnButtonClicked(Button * aButton)
{
  if (aButton == 0)
    return -1;

  if (aButton == mForward)
  {
    ScrollForward();
    return 0;
  }
  else if (aButton == mBack)
  {
    ScrollBack();
    return 0;
  }

  HoverButton * button = dynamic_cast<HoverButton *>(aButton);
  if (button == NULL)
    return -1;

  HoverWindow * window = button->CreateImage();
  if (window)
    mState.SetHoverWindow(window);

  return 0;
}


LRESULT HoverButtonBar::OnWindowPosChanged(const WINDOWPOS & oPos)
{
  (void)oPos;

  RECT rect;
  GetClientRect(rect);
  int iHeight = (rect.bottom - rect.top) / (int)mButtonBars.size();
  int iWidth = int((rect.right - rect.left) * 0.94);

  mBack->SetWindowPos(NULL, rect.left, rect.top, int((rect.right - rect.left) * 0.03), rect.bottom - rect.top, SWP_SHOWWINDOW);
  mForward->SetWindowPos(NULL, rect.left + int((rect.right - rect.left) * 0.97), rect.top, int((rect.right - rect.left) * 0.03), rect.bottom - rect.top, SWP_SHOWWINDOW);

  for (size_t i = 0; i < mButtonBars.size(); ++i)
    mButtonBars[i]->SetWindowPos(NULL, rect.left + int((rect.right - rect.left) * 0.03), rect.top + iHeight * (int)i,
                                       iWidth, iHeight,
                                       SWP_SHOWWINDOW);

  return 0;
}


LRESULT HoverButtonBar::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg) {
    case WM_USER_BUTTON_CLICKED:
      return OnButtonClicked((Button *)lParam);

    default:
      return FrameWindow::WindowProc(uMsg, wParam, lParam);
  }
}


void HoverButtonBar::AddButton(ButtonBar * aButtonBar, HoverButton * aButton)
{
  assert(aButtonBar != NULL);
  assert(aButton != NULL);

  aButtonBar->AddButton(aButton, int(mButtons.size()) - 1);
  mButtons.push_back(aButton);
}


void HoverButtonBar::SetFiles(const std::vector<std::tstring> & aFiles)
{
  mFiles = aFiles;
  mCurrentIndex = 0;

  FillButtons();
}
