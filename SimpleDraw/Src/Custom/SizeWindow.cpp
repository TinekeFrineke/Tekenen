
#include "SizeWindow.h"

#include <assert.h>

//#include "resource.h"

#include "Button/BitmapButton.h"
//#include "Control/ButtonBar.h"
#include "Custom/SizeBar.h"


SizeWindow::SizeWindow(Window * aParent, HINSTANCE anInstance)
: FrameWindow (aParent, anInstance, "SizeWindow"),
  mPreview    (nullptr),
  mSizeBar    (nullptr)
{
  mSizeBar = new SizeBar<SizeWindow>(this, anInstance, this);
}


void SizeWindow::SetPreview(Preview * aPreview)
{
  if (mPreview == aPreview)
    return;

  if (mPreview != nullptr)
    delete mPreview;

  mPreview = aPreview;
}


void SizeWindow::Larger()
{
  if (mPreview != nullptr)
  {
    mPreview->Larger();
    Notify();
  }
}


void SizeWindow::Smaller()
{
  if (mPreview != nullptr)
  {
    mPreview->Smaller();
    Notify();
  }
}


LRESULT SizeWindow::OnWindowPosChanged(const WINDOWPOS & oPos)
{
  (void)oPos;

  assert(mPreview != nullptr);
  if (mPreview == nullptr)
    return -1;

  RECT rect;
  GetClientRect(rect);

  int previewwidth   = ((rect.right - rect.left) * 2) / 3;
  int buttonbarwidth = rect.right - rect.left - previewwidth - 6;

  mPreview->SetWindowPos(nullptr, 2, 2,
                         previewwidth, rect.bottom - rect.top - 4,
                         SWP_SHOWWINDOW);
  mSizeBar->SetWindowPos(nullptr, previewwidth + 4, 2,
                                 buttonbarwidth, rect.bottom - rect.top - 4,
                                 SWP_SHOWWINDOW);

  return 0;
}
