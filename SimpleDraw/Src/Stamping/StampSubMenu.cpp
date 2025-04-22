#include "StampSubMenu.h"

#include "Button/HoverButton.h"
#include "Custom/HoverButtonBar.h"
#include "Custom/SizeBar.h"
#include "General/Profile.h"
#include "StampState.h"
#include "Window/ColourWindow.h"
#include "Thickness/ThicknessPanel.h"


StampSubMenu::StampSubMenu(SpecialToolMenu & aWindow, StampState & aState,
                           HINSTANCE hInstance, DrawingAttributes &  anAttributes)
: SubMenu       (aWindow, anAttributes),
  mState        (aState),
  mHInstance    (hInstance),
  mButtonBar    (new HoverButtonBar(&aWindow, hInstance, aState, 2, 10))
{
  GetFiles();

  mButtonBar->SetFiles(mFiles);
}


StampSubMenu::~StampSubMenu()
{
  delete mButtonBar;
}


void StampSubMenu::GetFiles()
{
  mFiles.clear();
  WIN32_FIND_DATA finddata;
  HANDLE hFind = FindFirstFile((Profile::GetInstance().GetStampDirectory() + _T("\\*.bmp")).c_str(), &finddata);
  if (hFind == NULL)
    return;

  if (finddata.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
    mFiles.push_back(Profile::GetInstance().GetStampDirectory() + _T("\\") + finddata.cFileName);
  while (FindNextFile(hFind, &finddata) != FALSE)
    if (finddata.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
      mFiles.push_back(Profile::GetInstance().GetStampDirectory() + _T("\\") + finddata.cFileName);

  FindClose(hFind);

  Tekenen::Randomize(mFiles);
}


LRESULT StampSubMenu::LayoutWindow(const RECT & aClientRect)
{
  mButtonBar->SetWindowPos(NULL, aClientRect.left, aClientRect.top,
                                 aClientRect.right - aClientRect.left,
                                 aClientRect.bottom - aClientRect.top, SWP_SHOWWINDOW);

  return 0;
}


void StampSubMenu::Update(Subject * aSubject)
{
  (void)aSubject;
}


void StampSubMenu::Destroyed(Subject * aSubject)
{
  (void)aSubject;
}
