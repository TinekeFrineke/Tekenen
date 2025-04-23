#include "FileControl.h"

#include <windef.h>

#include "General/Profile.h"
#include "Button/BitmapButton.h"


FileControl::FileControl(Window * aParent, HINSTANCE hInstance, bool bExistingOnly, const std::string & aName)
: FrameWindow     (aParent, hInstance, aName),
  mExistingOnly   (bExistingOnly),
  mSelectedButton (NULL),
  mIndex          (0)
{
  for (int i = 0; i < 9; ++i)
  {
    VariableBitmapButton * button = new VariableBitmapButton(this, hInstance,
                                                             false, "FileControlButton",
                                                             Button::BUTTON_STRATEGY::ToggleButton);
    button->SetSelectedColour(RGB(255, 127, 0));
    mCells.push_back(button);
  }
}


void FileControl::SetFiles(const std::vector<std::string> & aFiles)
{
  mFiles = aFiles;
  mIndex = 0;
  mSelectedButton = NULL;

  UpdateFiles();
}


void FileControl::UpdateFiles()
{
  for (size_t i = 0; i < 9; ++i) {
    if (i + mIndex < mFiles.size())
      mCells[i]->SetBitmap(mFiles[i + mIndex]);
    else
      mCells[i]->SetBitmap("");
    InvalidateRect(mCells[i]->GetHwnd(), NULL, true);
  }
}


LRESULT FileControl::OnButtonClicked(Button * aButton)
{
  if (aButton == mSelectedButton)
    return 0;

  if (mSelectedButton != NULL)
    mSelectedButton->SetPressed(false);

  mSelectedButton = aButton;

  if (mSelectedButton != NULL)
    mSelectedButton->SetPressed(true);

  return 0;
}


#ifndef min
#define min(a,b) ((a)<(b)?(a):(b))
#endif
#ifndef max
#define max(a,b) ((a)>(b)?(a):(b))
#endif


LRESULT FileControl::OnWindowPosChanged(const WINDOWPOS & oPos)
{
  (void)oPos;

  RECT rect;
  GetClientRect(rect);

  int iHBorderWidth = (rect.right - rect.left) / 30;
  int iVBorderWidth = (rect.bottom - rect.top) / 30;

  int iWidth  = int(((rect.right - rect.left) * 0.9) / 3) - iHBorderWidth * 2;
  int iHeight = int(((rect.bottom - rect.top) * 0.9) / 3) - iVBorderWidth * 2;

  for (size_t i = 0; i < min(3, (mFiles.size() + 2) / 3); ++i)
  {
    for (size_t j = 0; i * 3 + j < mFiles.size() && j < 3; ++j)
    {
      int iHorOffset = iHBorderWidth + int(i) * (iWidth  + iHBorderWidth);
      int iVerOffset = iVBorderWidth + int(j) * (iHeight + iVBorderWidth);

      VariableBitmapButton * button = mCells[i * 3 + j];
      button->SetWindowPos(NULL, iHorOffset, iVerOffset, iWidth, iHeight, SWP_SHOWWINDOW);
    }
  }

  return 0;
}


LRESULT FileControl::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg)
  {
    case Window::WM_USER_BUTTON_CLICKED:
      return OnButtonClicked((Button *)lParam);
  }

  return FrameWindow::WindowProc(uMsg, wParam, lParam);
}


std::string FileControl::GetSelectedFile() const
{
  for (int i = 0; i < 9; ++i)
    if (mCells[i] == mSelectedButton)
      return mFiles[mIndex + i];

  return "";
}


void FileControl::ScrollUp()
{
  mIndex = max(0, mIndex - 9);
  UpdateFiles();
}


void FileControl::ScrollDown()
{
  mIndex = min(int(mFiles.size()) - 8, mIndex + 9);
  UpdateFiles();
}

