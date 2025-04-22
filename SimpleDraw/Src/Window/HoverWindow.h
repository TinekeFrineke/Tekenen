#ifndef WINDOW_HOVERWINDOW_H__
#define WINDOW_HOVERWINDOW_H__

#include "Window/Window.h"

class StampState;
class StampSubState;

#define USE_OLD_DC 0

class HoverWindow : public GenericWindow
{
public:
  enum STATE { ST_Position, ST_Size, ST_Ready };

                          HoverWindow(Window * aParent, HINSTANCE hInstance,
                                      StampState & mState,
                                      HBITMAP hBitmap,
                                      int aWidth, int aHeight);
                          ~HoverWindow();

  void                    SetState(STATE aState);

  virtual LRESULT         WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

  LRESULT                 SetPosition(int anX, int anY);
  //bool                    SetTarget(Window * aTarget);
  bool                    CopyBitmap(HDC aDC, const POINT &p);
  void                    Larger();
  void                    Smaller();

  LRESULT                 OnPaint();
  LRESULT                 Paint(HDC aDc, const PAINTSTRUCT & aPaintStruct);
  //LRESULT                 OnPaint();
  //LRESULT                 OnLeftButtonUp(int aVirtualKey, int anX, int anY);
  //LRESULT                 OnMouseMove(int aVirtualKey, int anX, int anY);

private:
  HoverWindow &           operator=(const HoverWindow &);
                          HoverWindow(const HoverWindow &);

  int                     mWidth;
  int                     mHeight;
  int                     mBitmapWidth;
  int                     mBitmapHeight;
  HBITMAP                 mHBitmap;
  HDC                     mHMemoryDC;
  Window &                mTarget;

  HBITMAP                 mHSrcBitmap;
#if USE_OLD_DC == 1
  RECT                    mOldRect;
  HDC                     mHOldDC;
  HBITMAP                 mHOldBitmap;
#endif
  HCURSOR                 mOldCursor;
  StampState &            mState;
  StampSubState *         mSubState;
};


#endif // WINDOW_HOVERWINDOW_H__