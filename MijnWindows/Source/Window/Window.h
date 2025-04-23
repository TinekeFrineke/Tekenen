#ifndef SIMPLEDRAW_WINDOW_H__
#define SIMPLEDRAW_WINDOW_H__


#include <windows.h>

#include <list>
#include <string>

class Window
{
public:
    enum {
        WM_USER_BUTTON_CLICKED = WM_USER + 1,
        WM_LAST_USER_MESSAGE = WM_USER + 100
    };


    Window(Window* aParent, HINSTANCE hInstance,
           const std::string& aName);
    virtual                 ~Window();

    virtual bool            Show(int nCmdShow);

    bool                    SetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx, int cy,
                                         UINT uFlags);

    bool                    SetParent(Window* aParent);
    Window* GetParent() { return mParent; }
    HWND                    GetHwnd() { return mHwnd; }

    bool                    GetClientRect(RECT& oRect);
    bool                    GetWindowRect(RECT& oRect);

    LRESULT                 SendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT                 PostMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    std::string            GetName() const { return mName; }

protected:
    void                    SetHwnd(HWND hWnd) { mHwnd = hWnd; }
    HINSTANCE               GetInstance() { return mHinstance; }

    bool                    Create(const std::string& aClassName,
                                   const std::string& aName,
                                   DWORD aStyle, DWORD anExStyle = 0,
                                   int aLeft = CW_USEDEFAULT, int aTop = 0,
                                   int aWidth = CW_USEDEFAULT, int aHeight = 0,
                                   HMENU hMenu = NULL,
                                   LPVOID aParam = 0);

    virtual LRESULT         OnWindowPosChanged(const WINDOWPOS& oPos);
    virtual LRESULT         OnPaint();

#if PRINT_MESSAGES > 0
    void                    SetDebug(bool bDebug) { mDebug = bDebug; }
#endif

    virtual LRESULT         WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    void                    AddChild(Window* aWindow);
    void                    RemoveChild(Window* aWindow);
#if PRINT_MESSAGES > 0
    bool                    mDebug;
#endif

    Window* mParent;
    std::list<Window*>     mChildren;
    HINSTANCE               mHinstance;
    HWND                    mHwnd;
    std::string            mName;
};


template <class Derived>
class MijnWindow: public Window
{
public:
    MijnWindow(Window* aParent, HINSTANCE hInstance,
               const std::string& aName);

private:
    static bool             mRegistered;
};


template <class Derived>
MijnWindow<Derived>::MijnWindow(Window* aParent, HINSTANCE hInstance,
                                const std::string& aName)
    : Window(aParent, hInstance, aName)
{
    if (!mRegistered)
        mRegistered = Derived::RegisterClass(hInstance);
}


class GenericWindow: public MijnWindow<GenericWindow>
{
public:
    GenericWindow(Window* aParent, HINSTANCE hInstance,
                  const std::string& aName, DWORD dwStyle, DWORD dwExStyle = 0);

    static bool RegisterClass(HINSTANCE hInstance);

protected:
    static char* GetClassName() { return const_cast<char*>("MijGenericWindow"); }
};

#endif // SIMPLEDRAW_WINDOW_H__
