
#include "SizeBar.h"

#include "General/Profile.h"
#include "Resource/resource.h"
#include "Button/BitmapButton.h"


SizeBarBase::SizeBarBase(Window * aParent, HINSTANCE anInstance)
: ButtonBar     (aParent, anInstance, "SizeBarBase", ButtonBar::LAYOUT_STRATEGY::VerticalFixedSize),
  mLargerButton (nullptr),
  mSmallerButton(nullptr)
{
  SingleBitmapButton * larger   = new SingleBitmapButton(this, anInstance, true, Profile::GetInstance().GetResourceDirectory() + "\\SizeWindowLargerButton");
  SingleBitmapButton * smaller  = new SingleBitmapButton(this, anInstance, true, Profile::GetInstance().GetResourceDirectory() + "\\SizeWindowSmallerButton");

  larger->SetBitmap(Profile::GetInstance().GetResourceDirectory() + "\\ArrowUp.bmp");
  smaller->SetBitmap(Profile::GetInstance().GetResourceDirectory() + "\\ArrowDown.bmp");

  mLargerButton   = larger;
  mSmallerButton  = smaller;

  AddButton(mLargerButton, IDB_ARROW_UP);
  AddButton(mSmallerButton, IDB_ARROW_DOWN);
}
