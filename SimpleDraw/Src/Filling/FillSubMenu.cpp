
#include "FillSubMenu.h"


#include <assert.h>
#include "General/Controller.h"
#include "General/Profile.h"
#include "Custom/FontButtonBar.h"
#include "FillState.h"
#include "Window/ColourWindow.h"


FillSubMenu::FillSubMenu(SpecialToolMenu & aMenu, FillState & aState,
                             HINSTANCE hInstance, DrawingAttributes &  anAttributes)
: SubMenu   (aMenu, anAttributes),
  mState    (aState),
  mInstance (hInstance)
{
  mColourWindow = new ColourWindow(&aMenu, anAttributes, mInstance);

  // Attach to the subjects
  mColourWindow->Attach(this);
}


FillSubMenu::~FillSubMenu()
{
  mColourWindow->Detach(this);

  delete mColourWindow;
}


LRESULT FillSubMenu::LayoutWindow(const RECT & aClientRect)
{
  mColourWindow->SetWindowPos(NULL, 0, 0,
                                aClientRect.right / 2, aClientRect.bottom / 2, SWP_SHOWWINDOW);
  return 0;
}


void FillSubMenu::SetColour(COLORREF aColour)
{
  GetAttributes().mColour = aColour;
  mState.SetColour(aColour);
}


void FillSubMenu::Update(Subject * aSubject)
{
  if (aSubject == mColourWindow)
    SetColour(mColourWindow->GetColour());
}


void FillSubMenu::Destroyed(Subject * aSubject)
{
  (void)aSubject;
}
