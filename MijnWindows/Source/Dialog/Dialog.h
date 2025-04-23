#pragma once


#include "Window/Window.h"

#include "Window/FrameWindow.h"

class Dialog: public MijnWindow<Dialog>
{
public:
    // WS_POPUP by default
    Dialog(Window* aParent, HINSTANCE hInstance,
           const std::string& aName = "Dialog",
           DWORD style = 0);

    // 0 means error
    int                         Execute();

    static bool                 RegisterClass(HINSTANCE hInstance);
    //FrameWindow *               mFrame;

protected:
    void                        SetReady() { mReady = true; }

    void                        SetResult(int aResult) { mResult = aResult; }

    virtual void                ShowModal() = 0;

    static char* GetClassName() { return const_cast<char*>("MijnDialog"); }

private:
    bool                        mReady;
    int                         mResult;
};
