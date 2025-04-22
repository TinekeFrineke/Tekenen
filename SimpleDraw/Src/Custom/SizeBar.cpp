
#include "SizeBar.h"

#include "Resource/resource.h"
#include "Button/BitmapButton.h"


SizeBarBase::SizeBarBase(Window * aParent, HINSTANCE anInstance)
: ButtonBar     (aParent, anInstance, _T("SizeBarBase"), ButtonBar::LS_VerticalFixedSize),
  mLargerButton (NULL),
  mSmallerButton(NULL)
{
  SingleBitmapButton * larger   = new SingleBitmapButton(this, anInstance, true, _T("SizeWindowLargerButton"));
  SingleBitmapButton * smaller  = new SingleBitmapButton(this, anInstance, true, _T("SizeWindowSmallerButton"));

  larger->SetBitmap(IDB_ARROW_UP);
  smaller->SetBitmap(IDB_ARROW_DOWN);

  mLargerButton   = larger;
  mSmallerButton  = smaller;

  AddButton(mLargerButton, IDB_ARROW_UP);
  AddButton(mSmallerButton, IDB_ARROW_DOWN);
}
