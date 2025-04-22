
#include "SizeWindow.h"

#include <assert.h>

//#include "resource.h"

#include "Button/BitmapButton.h"
//#include "Control/ButtonBar.h"
#include "Custom/SizeBar.h"


SizeWindow::SizeWindow(Window * aParent, HINSTANCE anInstance)
: FrameWindow (aParent, anInstance, _T("SizeWindow")),
  mPreview    (NULL),
  mSizeBar    (NULL)
{
  mSizeBar = new SizeBar<SizeWindow>(this, anInstance, this);
}


void SizeWindow::SetPreview(Preview * aPreview)
{
  if (mPreview == aPreview)
    return;

  if (mPreview != NULL)
    delete mPreview;

  mPreview = aPreview;
}


void SizeWindow::Larger()
{
  if (mPreview != NULL)
  {
    mPreview->Larger();
    Notify();
  }
}


void SizeWindow::Smaller()
{
  if (mPreview != NULL)
  {
    mPreview->Smaller();
    Notify();
  }
}


LRESULT SizeWindow::OnWindowPosChanged(const WINDOWPOS & oPos)
{
  (void)oPos;

  assert(mPreview != NULL);
  if (mPreview == NULL)
    return -1;

  RECT rect;
  GetClientRect(rect);

  int previewwidth   = ((rect.right - rect.left) * 2) / 3;
  int buttonbarwidth = rect.right - rect.left - previewwidth - 6;

  mPreview->SetWindowPos(NULL, 2, 2,
                         previewwidth, rect.bottom - rect.top - 4,
                         SWP_SHOWWINDOW);
  mSizeBar->SetWindowPos(NULL, previewwidth + 4, 2,
                                 buttonbarwidth, rect.bottom - rect.top - 4,
                                 SWP_SHOWWINDOW);

  return 0;
}
