
#include "LetterSubMenu.h"


#include <assert.h>
#include "General/Controller.h"
#include "General/Profile.h"
#include "Custom/FontButtonBar.h"
#include "LetterSizeWindow.h"
#include "Custom/SizeWindow.h"
#include "Window/ColourWindow.h"


LetterSubMenu::LetterSubMenu(SpecialToolMenu & aMenu, LetterState & aState,
                             HINSTANCE hInstance, DrawingAttributes &  anAttributes)
: SubMenu     (aMenu, anAttributes),
  mState      (aState),
  mInstance   (hInstance),
  mButtonBar  (new FontButtonBar(&aMenu, hInstance, aState, 2, 5)),
  mFont       (_T("Arial New"), 16),
  mSizeWindow (new SizeWindow(&aMenu, hInstance)),
  mPreview    (NULL)
{
  mColourWindow = new ColourWindow(&aMenu, anAttributes, mInstance);
  FillFonts();
  mPreview = new LetterPreview(mSizeWindow, hInstance, mFont, anAttributes);
  mSizeWindow->SetPreview(mPreview);

  // Attach to the subjects
  mColourWindow->Attach(this);
  mButtonBar->Attach(this);
  mSizeWindow->Attach(this);
}


LetterSubMenu::~LetterSubMenu()
{
  mSizeWindow->Detach(this);
  mColourWindow->Detach(this);
  mButtonBar->Detach(this);

  delete mButtonBar;
  delete mColourWindow;
  delete mSizeWindow;
  //for (size_t i = 0; i < mFonts.size(); ++i)
  //  delete mFonts[i];
}


void LetterSubMenu::FillFonts()
{
  std::vector<std::tstring> fontnames;

  Profile::GetInstance().GetFontNames(fontnames);

  mButtonBar->SetFontNames(fontnames);
}


LRESULT LetterSubMenu::LayoutWindow(const RECT & aClientRect)
{
  mColourWindow->SetWindowPos(NULL, 0, 0,
                              aClientRect.right / 2, aClientRect.bottom / 2, SWP_SHOWWINDOW);
  mButtonBar->SetWindowPos(NULL, aClientRect.left, (aClientRect.bottom - aClientRect.top) / 2,
                                 ((aClientRect.right - aClientRect.left) * 5) / 8,
                                 (aClientRect.bottom - aClientRect.top) / 2, SWP_SHOWWINDOW);
  mSizeWindow->SetWindowPos(NULL, ((aClientRect.right - aClientRect.left) * 5) / 8 + 2,
                                  0,
                                  aClientRect.right - ((aClientRect.right - aClientRect.left) * 5) / 8,
                                  aClientRect.bottom,
                                  SWP_SHOWWINDOW);

  return 0;
}


void LetterSubMenu::SetColour(COLORREF aColour)
{
  GetAttributes().mColour = aColour;
  InvalidateRect(mPreview->GetHwnd(), NULL, false);

  Notify();
}


void LetterSubMenu::SetFont(const Font & aFont)
{
  mFont.SetName(aFont.GetName());
  InvalidateRect(mPreview->GetHwnd(), NULL, false);

  Notify();
}


void LetterSubMenu::Update(Subject * aSubject)
{
  if (aSubject == mColourWindow)
  {
    SetColour(mColourWindow->GetColour());
  }
  else if (aSubject == mButtonBar)
  {
    if (mButtonBar->GetFont() != NULL)
      SetFont(*mButtonBar->GetFont());
  }
  else if (aSubject == mSizeWindow)
  {
    Notify();
  }
}


void LetterSubMenu::Destroyed(Subject * aSubject)
{
  (void)aSubject;
}
