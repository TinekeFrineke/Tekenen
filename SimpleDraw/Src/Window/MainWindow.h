#ifndef SIMPLEDRAW_MAINWINDOW_H__
#define SIMPLEDRAW_MAINWINDOW_H__

#include <windows.h>

#include "Window/Window.h"

class Button;
class ButtonBar;
class Controller;
class DoubleBitmapButton;
class DrawingWindow;
class GeneralToolMenu;
class SpecialToolMenu;
class UserButtonBar;

class MainWindow: public MijnWindow<MainWindow>
{
public:
    MainWindow(HINSTANCE    hInstance,
               Controller& aController);
    ~MainWindow();

    LRESULT                 WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

    static bool             RegisterClass(HINSTANCE hInstance);

    DrawingWindow* GetDrawingWindow() { return mDrawingWindow; }
    GeneralToolMenu* GetGeneralToolMenu() { return mGeneralToolMenu; }
    SpecialToolMenu* GetSpecialToolMenu() { return mSpecialToolMenu; }

    void                    UpdateUser();

protected:
    static char* GetClassName() { return const_cast<char*>("MijnMainWindow"); }

private:
    MainWindow& operator=(const MainWindow&) = delete;
    MainWindow(const MainWindow&) = delete;

    bool                    HandleCreateWindow();

    bool                    HandleCharacter(UINT aCharacter, UINT aRepeatCount, UINT aFlags);

    bool                    HandleMoveWindow(UINT aLeft, UINT aTop);
    LRESULT                 OnSizeWindow(UINT aWidth, UINT aHeight);
    virtual LRESULT         OnWindowPosChanged(const WINDOWPOS& oPos);
    LRESULT                 OnButtonClicked(Button* aButton);

    Controller& mController;
    UserButtonBar* mUserButtonBar;
    ButtonBar* mFileButtonBar;
    DoubleBitmapButton* mEraseButton;
    DoubleBitmapButton* mSaveButton;
    DoubleBitmapButton* mLoadButton;
    DoubleBitmapButton* mCloseButton;

    ButtonBar* mUndoButtonBar;

    DoubleBitmapButton* mUndoButton;
    DoubleBitmapButton* mRedoButton;

    DrawingWindow* mDrawingWindow;
    GeneralToolMenu* mGeneralToolMenu;
    SpecialToolMenu* mSpecialToolMenu;
};


#endif // SIMPLEDRAW_MAINWINDOW_H__