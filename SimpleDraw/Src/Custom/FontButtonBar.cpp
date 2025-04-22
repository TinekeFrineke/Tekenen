#include "FontButtonBar.h"

#include <assert.h>

//#include "resource.h"

#include "Button/FontButton.h"
#include "Button/BitmapButton.h"
#include "Control/ButtonBar.h"
#include "Stamping/StampState.h"


#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif



FontButtonBar::FontButtonBar(Window * aParent, HINSTANCE hInstance,
                              LetterState & aState,
                              int aRows, int aColumns)
: ButtonBarTemplate<FontButton> (aParent, hInstance, aRows, aColumns, true),
  mState                        (aState),
  mSelected                     (NULL)
{
}


void FontButtonBar::FillButtons()
{
  for (size_t i = 0; i < GetButtons().size(); ++i)
  {
    if (i + mCurrentIndex < mFontNames.size())
      GetButtons()[i]->SetFont(Font(mFontNames[i + mCurrentIndex], 24));
    else
      GetButtons()[i]->SetFont(Font(_T(""), 0));

    InvalidateRect(GetButtons()[i]->GetHwnd(), NULL, false);
  }
}


void FontButtonBar::ScrollRight()
{
  mCurrentIndex = min(mCurrentIndex + GetRows() * GetColumns(), int(mFontNames.size()) - GetRows() * GetColumns());
  mCurrentIndex = max(mCurrentIndex, 0);

  FillButtons();
}


void FontButtonBar::ScrollLeft()
{
  mCurrentIndex -= GetRows() * GetColumns();
  mCurrentIndex = max(mCurrentIndex, 0);

  FillButtons();
}


LRESULT FontButtonBar::OnButtonClicked(Button * aButton)
{
  if (aButton == 0)
    return -1;

  FontButton * button = dynamic_cast<FontButton *>(aButton);
  if (button == NULL)
    return ButtonBarBase::OnButtonClicked(aButton);

  if (aButton == mSelected)
    return 0;

  if (mSelected != NULL)
    mSelected->SetPressed(false);

  mSelected = button;
  if (mSelected != NULL)
    mSelected->SetPressed(true);

  Notify();

  return 0;
}


const Font * FontButtonBar::GetFont() const
{
  if (mSelected == NULL)
    return NULL;

  return mSelected->GetFont();
}


LRESULT FontButtonBar::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg) {
    case WM_USER_BUTTON_CLICKED:
      return OnButtonClicked((Button *)lParam);

    default:
      return ButtonBarTemplate<FontButton>::WindowProc(uMsg, wParam, lParam);
  }
}


void FontButtonBar::SetFontNames(const std::vector<std::tstring> & aFontNames)
{
  mFontNames = aFontNames;
  mCurrentIndex = 0;

  FillButtons();
}
