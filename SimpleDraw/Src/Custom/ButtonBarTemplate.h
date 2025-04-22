#ifndef CUSTOM_BUTTONBARTEMPLATE_H__
#define CUSTOM_BUTTONBARTEMPLATE_H__

#include "Control/ButtonBar.h"
#include "Window/FrameWindow.h"
#include "General/Subject.h"

#include <assert.h>
#include <vector>

class Button;
class ButtonBar;
class HoverButton;
class SingleBitmapButton;
class StampState;


class ButtonBarBase : public FrameWindow, public virtual Subject
{
public:
                              ButtonBarBase(Window * aParent, HINSTANCE hInstance,
                                            const std::tstring & aName,
                                            int aRows, int aColumns,
                                            bool bScroll);
                              ~ButtonBarBase() {}

  // Override for scrolling
  virtual void                ScrollLeft() {}
  // Override for scrolling
  virtual void                ScrollRight() {}

protected:
  int                         GetRows() const { return mRows; }
  int                         GetColumns() const { return mColumns; }
  std::vector<ButtonBar *> &  GetButtonBars() { return mButtonBars; }

  LRESULT                     OnWindowPosChanged(const WINDOWPOS & oPos);
  LRESULT                     OnButtonClicked(Button * aButton);

private:
  SingleBitmapButton *        mBack;
  SingleBitmapButton *        mForward;

  std::vector<ButtonBar *>    mButtonBars;

  int                         mRows;
  int                         mColumns;
  bool                        mScrolling;
};


template <class ButtonType>
class ButtonBarTemplate : public ButtonBarBase
{
public:
                              ButtonBarTemplate(Window * aParent, HINSTANCE hInstance,
                                                int aRows, int aColumns,
                                                bool bScroll);

protected:
  std::vector<ButtonType *> & GetButtons() { return mButtons; }
  const std::vector<ButtonType *> &
                              GetButtons() const { return mButtons; }

private:
  void                        AddButton(ButtonBar * aButtonBar, ButtonType * aButton);

  SingleBitmapButton *        mBack;
  SingleBitmapButton *        mForward;

  std::vector<ButtonType *>   mButtons;
};


template <class ButtonType>
ButtonBarTemplate<ButtonType>::ButtonBarTemplate(Window * aParent, HINSTANCE hInstance,
                                                 int aRows, int aColumns,
                                                 bool aScroll)
: ButtonBarBase (aParent, hInstance, _T("ButtonBarTemplate"), aRows, aColumns, aScroll)
{
  for (size_t row = 0; row < GetButtonBars().size(); ++row)
  {
    ButtonBar * buttonbar = GetButtonBars()[row];

    for (int column = 0; column < aColumns; ++column)
      AddButton(buttonbar, new ButtonType(buttonbar, hInstance));
  }
}


template <class ButtonType>
void ButtonBarTemplate<ButtonType>::AddButton(ButtonBar * aButtonBar, ButtonType * aButton)
{
  assert(aButtonBar != NULL);
  assert(aButton != NULL);

  aButtonBar->AddButton(aButton, int(mButtons.size()) - 1);
  mButtons.push_back(aButton);
}


#endif // CUSTOM_BUTTONBARTEMPLATE_H__
