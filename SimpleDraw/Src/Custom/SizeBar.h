#ifndef CUSTOM_SIZEBAR_H__
#define CUSTOM_SIZEBAR_H__

#include "Control/ButtonBar.h"


class SingleBitmapButton;


class SizeBarBase : public ButtonBar
{
public:
                      SizeBarBase(Window * aParent, HINSTANCE anInstance);

protected:
  const Button *      GetLargerButton()  const { return mLargerButton; }
  const Button *      GetSmallerButton() const { return mSmallerButton; }

private:
  Button *            mLargerButton;
  Button *            mSmallerButton;
};

// Target should have methods Smaller() and Larger().
template<class Target>
class SizeBar : public SizeBarBase
{
public:
                      SizeBar(Window * aParent, HINSTANCE anInstance,
                              Target * aTarget = NULL);

  void                SetTarget(Target * aTarget) { mTarget = aTarget; }
  virtual LRESULT     OnButtonClick(Button * aButton);

private:
  Target *            mTarget;
};


template <class Target>
inline SizeBar<Target>::SizeBar(Window * aParent, HINSTANCE anInstance,
                                Target * aTarget)
: SizeBarBase (aParent, anInstance),
  mTarget     (aTarget)
{
}


template<class Target>
inline LRESULT SizeBar<Target>::OnButtonClick(Button * aButton)
{
  if (aButton == GetLargerButton())
  {
    if (mTarget != NULL)
      mTarget->Larger();
    return 0;
  }
  else if (aButton == GetSmallerButton())
  {
    if (mTarget != NULL)
      mTarget->Smaller();
    return 0;
  }

  return ButtonBar::OnButtonClick(aButton);
}


#endif // CUSTOM_SIZEBAR_H__