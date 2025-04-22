
#include "ButtonBarTemplate.h"

#include "Resource/resource.h"
#include "Button/BitmapButton.h"
#include "Control/ButtonBar.h"


ButtonBarBase::ButtonBarBase(Window * aParent, HINSTANCE hInstance,
                             const std::tstring & aName,
                             int aRows, int aColumns,
                             bool aScroll)
: FrameWindow(aParent, hInstance, aName, WS_CHILD/* | WS_VISIBLE*/),
  mScrolling    (aScroll),
  mRows         (aRows),
  mColumns      (aColumns),
  mBack         (NULL),
  mForward      (NULL)
{
  if (mScrolling)
  {
    mBack     = new SingleBitmapButton(this, hInstance, true, _T("BackButton"));
    mForward  = new SingleBitmapButton(this, hInstance, true, _T("ForwardButton"));
    
    mBack->SetBitmap(IDB_ARROW_LEFT);
    mForward->SetBitmap(IDB_ARROW_RIGHT);
  }

  for (int row = 0; row < aRows; ++row)
  {
    ButtonBar * buttonbar = new ButtonBar(this, hInstance,
                                          _T("ButtonBarTemplateButtonBar"),
                                          ButtonBar::LS_HorizontalFit);
    mButtonBars.push_back(buttonbar);
  }
}


LRESULT ButtonBarBase::OnButtonClicked(Button * aButton)
{
  if (aButton == 0)
    return -1;

  if (aButton == mForward)
    ScrollRight();
  else if (aButton == mBack)
    ScrollLeft();
  else
    return -1;

  return 0;
}


LRESULT ButtonBarBase::OnWindowPosChanged(const WINDOWPOS & oPos)
{
  (void)oPos;

  RECT rect;
  GetClientRect(rect);
  int iHeight = (rect.bottom - rect.top) / (int)mButtonBars.size();
  int iWidth = 0;
  if (mBack != NULL && mForward != NULL)
  {
    iWidth = int((rect.right - rect.left) * 0.94);
    mBack->SetWindowPos(NULL, rect.left, rect.top, int((rect.right - rect.left) * 0.03), rect.bottom - rect.top, SWP_SHOWWINDOW);
    mForward->SetWindowPos(NULL, rect.left + int((rect.right - rect.left) * 0.97), rect.top, int((rect.right - rect.left) * 0.03), rect.bottom - rect.top, SWP_SHOWWINDOW);
  }
  else
  {
    iWidth = int((rect.right - rect.left)/* * 0.94*/);
  }

  for (size_t i = 0; i < mButtonBars.size(); ++i)
    mButtonBars[i]->SetWindowPos(NULL, rect.left + int((rect.right - rect.left) * 0.03), rect.top + iHeight * (int)i,
                                       iWidth, iHeight,
                                       SWP_SHOWWINDOW);

  return 0;
}
