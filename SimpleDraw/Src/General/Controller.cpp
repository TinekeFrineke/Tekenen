#include "Controller.h"

#include <tchar.h>

#include "Bitmap.h"
#include "Window/DrawingWindow.h"
#include "Window/MainWindow.h"
#include "Dialog/FileDialog.h"
#include "IniProfile.h"

//#include "resource.h"


DrawingAttributes::DrawingAttributes()
: mColour   (0x000000),
  mThickness(20)
{
}


Controller * Controller::mInstance = NULL;


Controller::Controller(HINSTANCE hInstance)
: mHInstance  (hInstance),
  mProfile    (new IniProfile(_T("simpledraw.ini")))
{
  if (mProfile->NumberOfUsers() <= 0)
  {
    std::tstring error = _T("Voeg users toe in ");
    error += mProfile->GetWorkDirectory() + _T("\\simpledraw.ini:\n");
    error += _T("[users]\n");
    error += _T("user1=<username>\n\n");
    error += _T("[<username>]\n");
    error += _T("bitmap=<bitmapname>\n");
    ::MessageBox(0, error.c_str(), _T("ERROR"), MB_OK);
    exit(0);
  }

  mInstance = this;

  mMainWindow       = new MainWindow(hInstance, *this);
  mDrawingWindow    = mMainWindow->GetDrawingWindow();
  mGeneralToolMenu  = mMainWindow->GetGeneralToolMenu();
  mSpecialToolMenu  = mMainWindow->GetSpecialToolMenu();
}


Controller::~Controller()
{
  while (!mUndoBitmaps.empty())
  {
    DeleteObject(*mUndoBitmaps.begin());
    mUndoBitmaps.pop_front();
  }

  delete mMainWindow;
  delete mProfile;

  mInstance = NULL;
}


Controller & Controller::GetInstance()
{
  if (mInstance == NULL)
    throw _T("No controller instance present");

  return *mInstance;
}


void Controller::SetState(Tekenen::DRAW_STATE aState)
{
  mDrawingWindow->SetState(aState);
}


void Controller::ClearWindow()
{
  mDrawingWindow->ClearWindow();
}


std::tstring Controller::GetNewFilename() const
{
  std::tstring mask = GetProfile().GetDataDirectory() + _T("\\") + GetProfile().GetUser().GetName() + _T("%d.bmp");

  TCHAR fname[_MAX_PATH];
  int iIndex = 0;
  _stprintf_s(fname, mask.c_str(), iIndex);
  WIN32_FIND_DATA data;
  HANDLE hFind = FindFirstFile(fname, &data);
  while (hFind != INVALID_HANDLE_VALUE)
  {
    iIndex++;
    _stprintf_s(fname, mask.c_str(), iIndex);
    FindClose(hFind);
    hFind = FindFirstFile(fname, &data);
  }

  return fname;
}


void Controller::LoadBitmap()
{
  std::tstring smask = PickFile(true);
  HBITMAP bmp = (HBITMAP)LoadImage(mHInstance, smask.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
  if (bmp)
    mDrawingWindow->SetBitmap(bmp);
}


std::tstring Controller::PickFile(bool bExistingOnly) const
{
  FileDialog dialog(mMainWindow, mHInstance, bExistingOnly);
  bool bResult = dialog.Execute() != 0;

  mMainWindow->UpdateUser();

  if (bResult != 0)
    return dialog.GetFilename();

  return _T("");
}


void Controller::SaveBitmap()
{
  mName = GetNewFilename();
  ::SaveBitmap(mDrawingWindow->GetHDC(), mDrawingWindow->GetBitmap(), mName);
}


void Controller::StoreBitmap(HBITMAP aBitmap)
{
  OutputDebugString(_T("StoreBitmap()\n"));
  if (mUndoBitmaps.size() > 10)
  {
    DeleteObject(*mUndoBitmaps.begin());
    mUndoBitmaps.pop_front();
  }

  HDC memdc = CreateCompatibleDC(mDrawingWindow->GetHDC());

  BITMAP bitmap;
  GetObject(aBitmap, sizeof(bitmap), &bitmap);

  HBITMAP newbitmap = CreateBitmapIndirect(&bitmap);
  SelectObject(memdc, newbitmap);
  BitBlt(memdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, mDrawingWindow->GetHDC(), 0, 0, SRCCOPY);
  DeleteDC(memdc);

  mUndoBitmaps.push_back(newbitmap);
}


void Controller::FlushRedo()
{
  OutputDebugString(_T("FlushRedo()\n"));
  while (!mRedoBitmaps.empty())
  {
    DeleteObject(*mRedoBitmaps.begin());
    mRedoBitmaps.pop_front();
  }
}


bool Controller::CanUndo() const
{
  return !mUndoBitmaps.empty();
}


bool Controller::CanRedo() const
{
  return !mRedoBitmaps.empty();
}


void Controller::Undo()
{
  OutputDebugString(_T("Undo()\n"));
  if (!CanUndo())
    return;

  OutputDebugString(_T("Can be undone\n"));

  mDrawingWindow->SetBitmap(mUndoBitmaps.back());
  mRedoBitmaps.push_back(mUndoBitmaps.back());
  mUndoBitmaps.pop_back();

  InvalidateRect(mDrawingWindow->GetHwnd(), NULL, false);
}


void Controller::Redo()
{
  OutputDebugString(_T("Redo()\n"));
  if (!CanRedo())
    return;

  OutputDebugString(_T("Can be redone\n"));

  mDrawingWindow->SetBitmap(mRedoBitmaps.back());
  mUndoBitmaps.push_back(mRedoBitmaps.back());
  mRedoBitmaps.pop_back();

  InvalidateRect(mDrawingWindow->GetHwnd(), NULL, false);
}
