#ifndef DIALOG_DIALOG_H__
#define DIALOG_DIALOG_H__


#include "Window/Window.h"

#include "Window/FrameWindow.h"

class Dialog : public MijnWindow<Dialog>
{
public:
  // WS_POPUP by default
                              Dialog(Window * aParent, HINSTANCE hInstance,
                                     const std::tstring & aName = _T("Dialog"),
                                     DWORD style = 0);

                              // 0 means error
  int                         Execute();

  static bool                 RegisterClass(HINSTANCE hInstance);
  //FrameWindow *               mFrame;

protected:
  void                        SetReady() { mReady = true; }

  void                        SetResult(int aResult) { mResult = aResult; }

  virtual void                ShowModal() = 0;

  static TCHAR *              GetClassName() { return const_cast<TCHAR *>(_T("MijnDialog")); }

private:
  bool                        mReady;
  int                         mResult;
};


#endif // DIALOG_DIALOG_H__
