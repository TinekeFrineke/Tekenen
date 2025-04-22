#include "SmudgingSubMenu.h"

#include "General/Profile.h"
#include "SmudgingState.h"
#include "Window/ColourWindow.h"
#include "Thickness/ThicknessSizeWindow.h" /*ThicknessPanel.h*/


SmudgingSubMenu::SmudgingSubMenu(SpecialToolMenu & aWindow, SmudgingState & aState,
                                 HINSTANCE hInstance, DrawingAttributes &  anAttributes)
: SubMenu       (aWindow, anAttributes),
  mState        (aState),
  mHInstance    (hInstance)/*,
  mColourWindow (NULL)*/
{
  //mColourWindow = new ColourWindow(&aWindow, anAttributes, mHInstance);
#ifdef USE_THICKNESS
  mThicknessPanel = new ThicknessSizeWindow(&aWindow, hInstance, anAttributes);// ThicknessPanel(&aWindow, hInstance);
#endif

  //mColourWindow->Attach(this);
#ifdef USE_THICKNESS
  mThicknessPanel->Attach(this);
#endif

  aState.SetColour(anAttributes.mColour);
#ifdef USE_THICKNESS
  mThicknessPanel->SetColour(anAttributes.mColour);
#endif
}


SmudgingSubMenu::~SmudgingSubMenu()
{
  //mColourWindow->Detach(this);
#ifdef USE_THICKNESS
  mThicknessPanel->Detach(this);
#endif

  //delete mColourWindow;
#ifdef USE_THICKNESS
  delete mThicknessPanel;
#endif
}


LRESULT SmudgingSubMenu::LayoutWindow(const RECT & aClientRect)
{
  (void)aClientRect;
  //mColourWindow  ->SetWindowPos(NULL, 2,                             2,
  //                              aClientRect.right / 2, aClientRect.bottom / 2, SWP_SHOWWINDOW);
#ifdef USE_THICKNESS
  mThicknessPanel->SetWindowPos(NULL, 2 + 5 * aClientRect.right / 6,
                                2, aClientRect.right / 6, aClientRect.bottom,     SWP_SHOWWINDOW);
#endif
  return 0;
}


void SmudgingSubMenu::Update(Subject * aSubject)
{
  (void)aSubject;
#ifdef USE_THICKNESS
  if (aSubject == mThicknessPanel)
  {
    mState.SetThickness(mThicknessPanel->GetThickness());
  }
  else
#endif
  //if (aSubject == mColourWindow)
  //{
  //  mState.SetColour(mColourWindow->GetColour());
#ifdef USE_THICKNESS
    mThicknessPanel->SetColour(mColourWindow->GetColour());
#endif
  //  GetAttributes().mColour = mColourWindow->GetColour();
  //}
}


void SmudgingSubMenu::Destroyed(Subject * aSubject)
{
  (void)aSubject;
#ifdef USE_THICKNESS
  if (aSubject == mThicknessPanel)
    mThicknessPanel = NULL;
  else
#endif
  //if (aSubject == mColourWindow)
  //  mColourWindow = NULL;
}
