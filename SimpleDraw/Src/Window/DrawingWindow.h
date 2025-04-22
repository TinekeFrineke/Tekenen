#ifndef SIMPLEDRAW_DRAWINGWINDOW_H__
#define SIMPLEDRAW_DRAWINGWINDOW_H__

#include <windows.h>

#include "General/General.h"
#include "General/Observer.h"
#include "Window/Window.h"

class DrawingAttributes;
class DrawingState;
class SpecialToolMenu;

class DrawingWindow : public MijnWindow<DrawingWindow>
{
public:
                          DrawingWindow(Window * aParent, SpecialToolMenu & aMenu,
                                        DrawingAttributes & anAttributes,
                                        HINSTANCE hInstance, const std::tstring & aName = _T("DrawingWindow"));
                          ~DrawingWindow();

  LRESULT                 WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

  HDC                     GetHDC();

  HBITMAP                 GetBitmap() { return mHBitMap; }

  void                    SetBitmap(HBITMAP hBitmap);

  void                    ClearWindow();

  void                    SetState(Tekenen::DRAW_STATE aState);

  static bool             RegisterClass(HINSTANCE hInstance);

protected:
  static TCHAR *          GetClassName() { return _T("MijnDrawingWindow"); }

private:
                          DrawingWindow(const DrawingWindow &);
  DrawingWindow &         operator=(const DrawingWindow &);

  // handle paint request
  LRESULT                 OnPaint();

  LRESULT                 OnChangeSize(int iWidth, int iHeight);

  LRESULT                 OnWindowPosChanged(const WINDOWPOS & oPos);

  // Screen DC
  HDC                     mHScreenDC;
  // Memory DC to print upon
  HDC                     mHMemoryDC;
  //
  HBITMAP                 mHBitMap;
  //
  HCURSOR                 mCursor;
  //
  bool                    mDrawing;
  // The drawing state
  Tekenen::DRAW_STATE     mDrawingState;
  ///
  DrawingState *          mState;
  // 
  DrawingAttributes &     mAttributes;
  //
  SpecialToolMenu &       mSpecialToolMenu;
};


#endif // SIMPLEDRAW_DRAWINGWINDOW_H__
