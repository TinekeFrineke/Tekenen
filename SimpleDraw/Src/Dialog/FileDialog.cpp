#include "FileDialog.h"

#include <tchar.h>

#include "General/Profile.h"
#include "Resource/resource.h"
#include "Button/BitmapButton.h"
#include "Custom/UserButtonBar.h"
#include "Custom/FileControl.h"


static const float sBorderWidth     = 2.5;

// Window is the place where the drawings are
static const float sWindowWidth     = 100 - 2 * sBorderWidth;
static const float sArrowWidth      = 100 - 2 * sBorderWidth;
static const float sHButtonPosition = sBorderWidth;

static const float sVButtonPosition = sBorderWidth;
static const float sArrowHeight     = 4;
static const float sVArrowDnPosition= 100 - sBorderWidth - sArrowHeight;

static const int   sColumns = 3;
static const int   sRows = 3;

FileDialog::FileDialog(Window * aParent, HINSTANCE hInstance,
                       bool bExistingOnly)
: Dialog        (aParent, hInstance/*, _T("FileDialig"), WS_THICKFRAME*/),
  mOKButton     (NULL),
  mButtonUp     (NULL),
  mButtonDown   (NULL),
  mButtonBar    (NULL),
  mFileControl  (NULL),
  mFirstIndex   (0)
{
  mButtonBar = new UserButtonBar(this, hInstance);

  mButtonUp   = new DoubleBitmapButton(this, hInstance, true, _T("ButtonUp"));
  mButtonDown = new DoubleBitmapButton(this, hInstance, true, _T("ButtonDown"));
  mButtonUp->SetBitmap(IDB_ARROW_UP, IDB_ARROW_UP);
  mButtonDown->SetBitmap(IDB_ARROW_DOWN, IDB_ARROW_DOWN);

  mOKButton = new DoubleBitmapButton(this, hInstance, true, _T("ButtonOK"));
  mOKButton->SetBitmap(IDB_LOAD, IDB_LOAD_PRESSED);

  mCloseButton = new DoubleBitmapButton(this, hInstance, true, _T("ButtonClose"));
  mCloseButton->SetBitmap(IDB_CROSS, IDB_CROSS_PRESSED);

  mFileControl = new FileControl(this, hInstance, bExistingOnly, _T("FileControl"));

  SelectUser(Profile::GetConstInstance().GetUser().GetName());
}


FileDialog::~FileDialog()
{
  //delete mButtonBar;
}


void FileDialog::ShowModal()
{
  ShowWindow(GetHwnd(), SW_SHOW);
}


LRESULT FileDialog::OnButtonClicked(Button * aButton)
{
  if (aButton == mCloseButton)
  {
    SetResult(0);
    SetReady();
  }
  else if (aButton == mButtonBar->GetSelectedButton())
  {
    SelectUser(Profile::GetConstInstance().GetUser(mButtonBar->GetSelectedID()).GetName());
  }
  else if (aButton == mOKButton)
  {
    SetResult(1);
    SetReady();
  }
  else if (aButton == mButtonDown)
  {
    mFileControl->ScrollDown();
  }
  else if (aButton == mButtonUp)
  {
    mFileControl->ScrollUp();
  }

  return 0;
}


void FileDialog::SelectUser(const std::tstring & aUser)
{
  mFileMask = Profile::GetInstance().GetDataDirectory() + _T("\\") + aUser + _T("*.bmp");
  mEntries.clear();

  WIN32_FIND_DATA data;
  HANDLE hFile = FindFirstFile(mFileMask.c_str(), &data);
  if (hFile != NULL)
    if (data.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
      mEntries.push_back(Profile::GetInstance().GetDataDirectory() + _T("\\") + data.cFileName);

  while (FindNextFile(hFile, &data))
    if (data.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
      mEntries.push_back(Profile::GetInstance().GetDataDirectory() + _T("\\") + data.cFileName);

  FindClose(hFile);

  //for (int i = 0; i < 100; ++i) {
  //  TCHAR filename[_MAX_FNAME];
  //  _stprintf(filename, mFileMask.c_str(), i);

  //  FILE * fp = _tfopen(filename, _T("r"));
  //  if (fp != NULL)
  //    mEntries.push_back(filename);
  //}

  mFileControl->SetFiles(mEntries);
}

LRESULT FileDialog::OnWindowPosChanged(const WINDOWPOS & /*aPos*/)
{
  RECT rect;
  GetClientRect(rect);
  int iWidth = rect.right - rect.left;
  int iHeight = rect.bottom - rect.top;
  float horscale = iWidth / 100.0f;
  float verscale = iHeight / 100.0f;
  const int borderWidth     = static_cast<int>(sBorderWidth * horscale);
  const int buttonBarWidth  = mButtonBar->GetWantedWidth();
  const int arrowWidth      = iWidth - (2 * borderWidth);
  const int windowWidth     = iWidth - (2 * borderWidth);
  const int closeWidth      = 48;
  const int hClosePosition  = iWidth - closeWidth - borderWidth;

  const int hButtonBarPosition  = borderWidth;
  const int hArrowPosition      = borderWidth;
  const int hWindowPosition     = borderWidth;

  const int hOKPosition       = (hButtonBarPosition + buttonBarWidth + hClosePosition) / 2 - 24;

  const int borderHeight      = static_cast<int>(sBorderWidth * verscale);
  const int buttonBarHeight   = mButtonBar->GetWantedHeight();
  const int arrowHeight       = static_cast<int>(sArrowHeight * verscale);
  const int closeHeight       = 48;

  const int vButtonBarPosition= borderHeight;
  const int vArrowUpPosition  = vButtonBarPosition + buttonBarHeight + borderHeight;
  const int vWindowPosition   = vArrowUpPosition + arrowHeight + borderHeight;
  const int vArrowDnPosition  = iHeight - arrowHeight - borderWidth;

  const int windowHeight      = vArrowDnPosition - 2 * borderHeight - vWindowPosition;
  const int vClosePosition    = borderHeight;
  const int vOKPosition       = borderHeight;

  mButtonBar->SetWindowPos(NULL, hButtonBarPosition, vButtonBarPosition,
                                 buttonBarWidth, buttonBarHeight, SWP_SHOWWINDOW);
  mButtonUp->SetWindowPos(NULL, hArrowPosition, vArrowUpPosition,
                                arrowWidth, arrowHeight, SWP_SHOWWINDOW);
  mButtonDown->SetWindowPos(NULL, hArrowPosition, vArrowDnPosition,
                                  arrowWidth, arrowHeight, SWP_SHOWWINDOW);
  mFileControl->SetWindowPos(NULL, hWindowPosition, vWindowPosition,
                                   windowWidth, windowHeight, SWP_SHOWWINDOW);
  mCloseButton->SetWindowPos(NULL, hClosePosition, vClosePosition,
                                   closeWidth, closeHeight, SWP_SHOWWINDOW);
  mOKButton->SetWindowPos(NULL, hOKPosition, vOKPosition,
                                48, 48, SWP_SHOWWINDOW);

  return 0;
}


LRESULT FileDialog::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg) {
    case WM_USER_BUTTON_CLICKED:
      return OnButtonClicked((Button *)lParam);

    default:
      return Dialog::WindowProc(uMsg, wParam, lParam);
  }
}


std::tstring FileDialog::GetFilename() const
{
  return mFileControl->GetSelectedFile();
}
