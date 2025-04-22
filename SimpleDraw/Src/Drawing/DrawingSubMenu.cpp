#include "DrawingSubMenu.h"

#include "General/Profile.h"
#include "Drawing/FreeDrawingState.h"
#include "Window/ColourWindow.h"
#include "Thickness/ThicknessSizeWindow.h" /*ThicknessPanel.h*/


DrawingSubMenu::DrawingSubMenu(SpecialToolMenu & aWindow, FreeDrawingState & aState,
                               HINSTANCE hInstance, DrawingAttributes &  anAttributes)
: SubMenu       (aWindow, anAttributes),
  mState        (aState),
  mHInstance    (hInstance),
  mColourWindow (NULL)
{
  mColourWindow = new ColourWindow(&aWindow, anAttributes, mHInstance);
  mThicknessPanel = new ThicknessSizeWindow(&aWindow, hInstance, anAttributes);// ThicknessPanel(&aWindow, hInstance);

  mColourWindow->Attach(this);
  mThicknessPanel->Attach(this);

  aState.SetColour(anAttributes.mColour);
  mThicknessPanel->SetColour(anAttributes.mColour);
}


DrawingSubMenu::~DrawingSubMenu()
{
  mColourWindow->Detach(this);
  mThicknessPanel->Detach(this);

  delete mColourWindow;
  delete mThicknessPanel;
}


LRESULT DrawingSubMenu::LayoutWindow(const RECT & aClientRect)
{
  mColourWindow  ->SetWindowPos(NULL, 2,                             2,
                                aClientRect.right / 2, aClientRect.bottom / 2, SWP_SHOWWINDOW);
  mThicknessPanel->SetWindowPos(NULL, 2 + 5 * aClientRect.right / 6,
                                2, aClientRect.right / 6, aClientRect.bottom,     SWP_SHOWWINDOW);

  return 0;
}


void DrawingSubMenu::Update(Subject * aSubject)
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


void DrawingSubMenu::Destroyed(Subject * aSubject)
{
  if (aSubject == mThicknessPanel)
    mThicknessPanel = NULL;
  else if (aSubject == mColourWindow)
    mColourWindow = NULL;
}
