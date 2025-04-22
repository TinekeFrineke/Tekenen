#include "AirbrushingSubMenu.h"

#include "General/Profile.h"
#include "AirbrushingState.h"
#include "Window/ColourWindow.h"
#include "AirbrushSizeWindow.h" /*ThicknessPanel.h*/


AirbrushingSubMenu::AirbrushingSubMenu(SpecialToolMenu & aWindow, AirbrushingState & aState,
                                       HINSTANCE hInstance, DrawingAttributes &  anAttributes)
: SubMenu       (aWindow, anAttributes),
  mState        (aState),
  mHInstance    (hInstance),
  mColourWindow (NULL)
{
  mColourWindow = new ColourWindow(&aWindow, anAttributes, mHInstance);
  mThicknessPanel = new AirbrushSizeWindow(&aWindow, hInstance, anAttributes);// ThicknessPanel(&aWindow, hInstance);

  mColourWindow->Attach(this);
  mThicknessPanel->Attach(this);

  aState.SetColour(anAttributes.mColour);
  mThicknessPanel->SetColour(anAttributes.mColour);
}


AirbrushingSubMenu::~AirbrushingSubMenu()
{
  mColourWindow->Detach(this);
  mThicknessPanel->Detach(this);

  delete mColourWindow;
  delete mThicknessPanel;
}


LRESULT AirbrushingSubMenu::LayoutWindow(const RECT & aClientRect)
{
  mColourWindow  ->SetWindowPos(NULL, 2,                             2,
                                aClientRect.right / 2, aClientRect.bottom / 2, SWP_SHOWWINDOW);
  mThicknessPanel->SetWindowPos(NULL, 2 + 5 * aClientRect.right / 6,
                                2, aClientRect.right / 6, aClientRect.bottom,     SWP_SHOWWINDOW);

  return 0;
}


void AirbrushingSubMenu::Update(Subject * aSubject)
{
  if (aSubject == mThicknessPanel)
  {
    mState.SetThickness(mThicknessPanel->GetThickness());
  }
  else if (aSubject == mColourWindow)
  {
    mState.SetColour(mColourWindow->GetColour());
    mThicknessPanel->SetColour(mColourWindow->GetColour());
    GetAttributes().mColour = mColourWindow->GetColour();
  }
}


void AirbrushingSubMenu::Destroyed(Subject * aSubject)
{
  if (aSubject == mThicknessPanel)
    mThicknessPanel = NULL;
  else if (aSubject == mColourWindow)
    mColourWindow = NULL;
}
