#include "Controller.h"


#include "Bitmap.h"
#include "Window/DrawingWindow.h"
#include "Window/MainWindow.h"
#include "Dialog/FileDialog.h"
#include "IniProfile.h"

//#include "resource.h"


DrawingAttributes::DrawingAttributes()
    : mColour(0x000000),
    mThickness(20)
{
}


Controller* Controller::mInstance = NULL;


Controller::Controller(HINSTANCE hInstance)
    : mHInstance(hInstance),
    mProfile(new IniProfile("simpledraw.ini"))
{
    if (mProfile->NumberOfUsers() <= 0)
    {
        std::string error = "Voeg users toe in ";
        error += mProfile->GetWorkDirectory() + "\\simpledraw.ini:\n";
        error += "[users]\n";
        error += "user1=<username>\n\n";
        error += "[<username>]\n";
        error += "bitmap=<bitmapname>\n";
        ::MessageBox(0, error.c_str(), "ERROR", MB_OK);
        exit(0);
    }

    mInstance = this;

    mMainWindow = new MainWindow(hInstance, *this);
    mDrawingWindow = mMainWindow->GetDrawingWindow();
    mGeneralToolMenu = mMainWindow->GetGeneralToolMenu();
    mSpecialToolMenu = mMainWindow->GetSpecialToolMenu();
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


Controller& Controller::GetInstance()
{
    if (mInstance == NULL)
        throw "No controller instance present";

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


std::string Controller::GetNewFilename() const
{
    std::string mask = GetProfile().GetDataDirectory() + "\\" + GetProfile().GetUser().GetName() + "%d.bmp";

    TCHAR fname[_MAX_PATH];
    int iIndex = 0;
    sprintf_s(fname, mask.c_str(), iIndex);
    WIN32_FIND_DATA data;
    HANDLE hFind = FindFirstFile(fname, &data);
    while (hFind != INVALID_HANDLE_VALUE)
    {
        iIndex++;
        sprintf_s(fname, mask.c_str(), iIndex);
        FindClose(hFind);
        hFind = FindFirstFile(fname, &data);
    }

    return fname;
}


void Controller::LoadBitmap()
{
    std::string smask = PickFile(true);
    HBITMAP bmp = (HBITMAP)LoadImage(mHInstance, smask.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (bmp)
        mDrawingWindow->SetBitmap(bmp);
}


std::string Controller::PickFile(bool bExistingOnly) const
{
    FileDialog dialog(mMainWindow, mHInstance, bExistingOnly);
    bool bResult = dialog.Execute() != 0;

    mMainWindow->UpdateUser();

    if (bResult != 0)
        return dialog.GetFilename();

    return "";
}


void Controller::SaveBitmap()
{
    mName = GetNewFilename();
    ::SaveBitmap(mDrawingWindow->GetHDC(), mDrawingWindow->GetBitmap(), mName);
}


void Controller::StoreBitmap(HBITMAP aBitmap)
{
    OutputDebugString("StoreBitmap()\n");
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
    OutputDebugString("FlushRedo()\n");
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
    OutputDebugString("Undo()\n");
    if (!CanUndo())
        return;

    OutputDebugString("Can be undone\n");

    mDrawingWindow->SetBitmap(mUndoBitmaps.back());
    mRedoBitmaps.push_back(mUndoBitmaps.back());
    mUndoBitmaps.pop_back();

    InvalidateRect(mDrawingWindow->GetHwnd(), NULL, false);
}


void Controller::Redo()
{
    OutputDebugString("Redo()\n");
    if (!CanRedo())
        return;

    OutputDebugString("Can be redone\n");

    mDrawingWindow->SetBitmap(mRedoBitmaps.back());
    mUndoBitmaps.push_back(mRedoBitmaps.back());
    mRedoBitmaps.pop_back();

    InvalidateRect(mDrawingWindow->GetHwnd(), NULL, false);
}
