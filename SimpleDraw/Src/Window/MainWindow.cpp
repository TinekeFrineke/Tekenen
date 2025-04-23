
#include "MainWindow.h"

#include <windows.h>

#include "General/Controller.h"
#include "DrawingWindow.h"
#include "GeneralToolMenu.h"
#include "Resource/resource.h"
#include "General/Profile.h"
#include "SpecialToolMenu.h"

#include "Button/BitmapButton.h"
#include "Custom/UserButtonBar.h"

static bool REDO = false;

// Relative sies for child windows
int cbBorderSize = 2;
int cbDrawingSize = 70;

template<>
bool MijnWindow<MainWindow>::mRegistered = false;

MainWindow::MainWindow(HINSTANCE    hInstance,
                       Controller& aController)
    : MijnWindow<MainWindow>(NULL, hInstance, "The MainWindow"),
    mController(aController),
    mUserButtonBar(NULL),
    mFileButtonBar(NULL),
    mEraseButton(NULL),
    mSaveButton(NULL),
    mLoadButton(NULL),
    mCloseButton(NULL),
    mDrawingWindow(NULL),
    mGeneralToolMenu(NULL),
    mSpecialToolMenu(NULL),
    mUndoButtonBar(NULL),
    mUndoButton(NULL),
    mRedoButton(NULL)
{
    Create(GetClassName(), "Ik Teken",
#ifdef NDEBUG
           WS_POPUP
#else
           WS_BORDER
#endif
    );

    mUserButtonBar = new UserButtonBar(this, hInstance);
    mFileButtonBar = new ButtonBar(this, hInstance, "FileButtonBar",
                                   ButtonBar::LAYOUT_STRATEGY::HorizontalFixedSize);

    mEraseButton = new DoubleBitmapButton(mFileButtonBar, hInstance, true, "EraseButton");
    mSaveButton = new DoubleBitmapButton(mFileButtonBar, hInstance, true, "SaveButton");
    mLoadButton = new DoubleBitmapButton(mFileButtonBar, hInstance, true, "LoadButton");

    mUndoButtonBar = new ButtonBar(this, hInstance, "UndoButtonBar",
                                   ButtonBar::LAYOUT_STRATEGY::HorizontalFixedSize);
    mUndoButton = new DoubleBitmapButton(mUndoButtonBar, hInstance, true, "UndoButton");
    if (REDO)
        mRedoButton = new DoubleBitmapButton(mUndoButtonBar, hInstance, true, "RedoButton");

    mCloseButton = new DoubleBitmapButton(this, hInstance, true, "CloseButton");

    mEraseButton->SetBitmap(Profile::GetInstance().GetResourceDirectory() + "\\Erase.bmp", Profile::GetInstance().GetResourceDirectory() + "\\ErasePressed.bmp");
    mSaveButton->SetBitmap(Profile::GetInstance().GetResourceDirectory() + "\\Save.bmp", Profile::GetInstance().GetResourceDirectory() + "\\SavePressed.bmp");
    mLoadButton->SetBitmap(Profile::GetInstance().GetResourceDirectory() + "\\Load.bmp", Profile::GetInstance().GetResourceDirectory() + "\\LoadPressed.bmp");
    mCloseButton->SetBitmap(Profile::GetInstance().GetResourceDirectory() + "\\Cross.bmp", Profile::GetInstance().GetResourceDirectory() + "\\CrossPressed.bmp");

    mFileButtonBar->AddButton(mEraseButton, IDB_ERASE);
    mFileButtonBar->AddButton(mSaveButton, IDB_SAVE);
    mFileButtonBar->AddButton(mLoadButton, IDB_LOAD);

    mUndoButton->SetBitmap(Profile::GetInstance().GetResourceDirectory() + "\\undo.bmp", Profile::GetInstance().GetResourceDirectory() + "\\UndoPressed.bmp");
    if (REDO)
        mRedoButton->SetBitmap(Profile::GetInstance().GetResourceDirectory() + "\\Redo.bmp", Profile::GetInstance().GetResourceDirectory() + "\\RedoPressed.bmp");

    mUndoButtonBar->AddButton(mUndoButton, IDB_UNDO);
    if (REDO)
        mUndoButtonBar->AddButton(mRedoButton, IDB_REDO);

    mGeneralToolMenu = new GeneralToolMenu(this, aController, hInstance);
    mSpecialToolMenu = new SpecialToolMenu(this, aController.GetAttributes(), hInstance);
    mDrawingWindow = new DrawingWindow(this, *mSpecialToolMenu, mController.GetAttributes(), hInstance);

    ShowWindow(GetHwnd(), SW_SHOWMAXIMIZED);

    if (mGeneralToolMenu == NULL || mSpecialToolMenu == NULL || mDrawingWindow == NULL)
    {
        ::MessageBox(0, "Error creating main window", "ERROR", MB_OK);
        throw 0;
    }

    RECT rect;
    GetClientRect(rect);

    //SetWindowPos(NULL, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, SWP_SHOWWINDOW);
    OnSizeWindow(rect.right - rect.left, rect.bottom - rect.top);
}


MainWindow::~MainWindow()
{
}


bool MainWindow::HandleCreateWindow()
{
    return true;
}


bool MainWindow::HandleCharacter(UINT aCharacter, UINT aRepeatCount, UINT aFlags)
// Returns true if the character was handled
{
    (void)aRepeatCount;
    (void)aFlags;
    if (aCharacter == VK_ESCAPE) {
        PostMessage(WM_QUIT, 0, 0);
        return true;
    }

    return false;
}


bool MainWindow::HandleMoveWindow(UINT aLeft, UINT aTop)
{
    (void)aLeft;
    (void)aTop;

    return true;
}


LRESULT MainWindow::OnWindowPosChanged(const WINDOWPOS& oPos)
{
    (void)oPos;

    RECT rect;
    GetClientRect(rect);
    return OnSizeWindow(rect.right - rect.left, rect.bottom - rect.top);

}

LRESULT MainWindow::OnSizeWindow(UINT aWidth, UINT aHeight)
{
    const int iHBorder = static_cast<int>((aWidth * cbBorderSize) / 100);
    const int iVBorder = static_cast<int>((aHeight * cbBorderSize) / 100);

    const int iButtonBarWidtht = mUserButtonBar->GetWantedWidth();
    const int iButtonBarHeight = mUserButtonBar->GetWantedHeight();

    const int iUndoButtonBarWidth = mUndoButtonBar->GetWantedWidth();
    const int iUndoButtonBarHeight = mUndoButtonBar->GetWantedHeight();

    const int iDrawingWindowWidth = static_cast<int>((aWidth * cbDrawingSize) / 100);
    const int iDrawingWindowHeight = static_cast<int>((aHeight * cbDrawingSize) / 100);

    const int iVToolWindowWidth = aWidth - 2 * iHBorder;

    const int iHButtonBar = iHBorder;
    const int iVButtonBar = iVBorder;
    const int iHClose = aWidth - iHBorder - 48;
    const int iVClose = iVBorder;
    const int iHDrawingWindow = iHBorder;
    const int iVDrawingWindow = iVButtonBar + iButtonBarHeight + iVBorder;
    const int iHPatternWindow = iHBorder * 2 + iDrawingWindowWidth;
    const int iVPatternWindow = iVDrawingWindow;
    const int iHToolWindow = iHBorder;
    const int iVToolWindow = iVDrawingWindow + iDrawingWindowHeight + iVBorder;
    const int iVToolWindowHeight = aHeight - iVToolWindow - iVBorder;

    mUserButtonBar->SetWindowPos(NULL, iHButtonBar, iVButtonBar, iButtonBarWidtht, iButtonBarHeight, SWP_SHOWWINDOW);
    mFileButtonBar->SetWindowPos(NULL, (iHButtonBar + iHClose) / 3, iVClose, mFileButtonBar->GetWantedWidth(), mFileButtonBar->GetWantedHeight(), SWP_SHOWWINDOW);
    mUndoButtonBar->SetWindowPos(NULL, 2 * (iHButtonBar + iHClose) / 3, iVButtonBar, iUndoButtonBarWidth, iUndoButtonBarHeight, SWP_SHOWWINDOW);
    mCloseButton->SetWindowPos(NULL, iHClose, iVClose, 48, 48, SWP_SHOWWINDOW);
    mDrawingWindow->SetWindowPos(NULL, iHDrawingWindow, iVDrawingWindow, iDrawingWindowWidth, iDrawingWindowHeight, SWP_SHOWWINDOW);
    mGeneralToolMenu->SetWindowPos(NULL, iHPatternWindow, iVPatternWindow,
                                   aWidth - iHPatternWindow - iHBorder, iDrawingWindowHeight,
                                   SWP_SHOWWINDOW);
    mSpecialToolMenu->SetWindowPos(NULL, iHToolWindow, iVToolWindow,
                                   iVToolWindowWidth, iVToolWindowHeight, SWP_SHOWWINDOW);

    return true;
}


LRESULT MainWindow::OnButtonClicked(Button* aButton)
{
    if (aButton == mCloseButton)
        PostMessage(WM_QUIT, 0, 0);
    else if (aButton == mEraseButton)
        mController.ClearWindow();
    else if (aButton == mSaveButton)
        mController.SaveBitmap();
    else if (aButton == mLoadButton)
        mController.LoadBitmap();
    else if (aButton == mUndoButton)
        mController.Undo();
    else if (aButton == mRedoButton)
        mController.Redo();

    return 0;
}


LRESULT MainWindow::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
        case WM_SIZE:
            return OnSizeWindow(LOWORD(lParam), HIWORD(lParam));
        case WM_USER_BUTTON_CLICKED:
            return OnButtonClicked((Button*)lParam);
        default:
            return MijnWindow<MainWindow>::WindowProc(uMsg, wParam, lParam);
    }
}


void MainWindow::UpdateUser()
{
    mUserButtonBar->Update();
}


bool MainWindow::RegisterClass(HINSTANCE hInstance)
{
    WNDCLASS wndclass;
    memset(&wndclass, 0, sizeof(WNDCLASS));

    wndclass.hInstance = hInstance;
    wndclass.lpfnWndProc = MijnWindow<MainWindow>::WindowProc;
    wndclass.lpszClassName = GetClassName();// = L"TinekeWindow";
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.hbrBackground = (HBRUSH)GetSysColorBrush(COLOR_INACTIVECAPTION);
    //wndclass.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));

    if (!::RegisterClass(&wndclass)) {
        DWORD err = GetLastError();
        err = err;
        return false;
    }

    return true;
}
