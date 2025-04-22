#ifndef THICKNESS_THICKNESSSIZEWINDOW_H__
#define THICKNESS_THICKNESSSIZEWINDOW_H__

#include "ThicknessControl.h"
#include "Custom/SizeWindow.h"


class DrawingAttributes;

class ThicknessPreview : public Preview
{
public:
                        ThicknessPreview(Window *aParent, HINSTANCE anInstance,
                                         DrawingAttributes & anAttributes);

  void                  SetThickness(int aThickness);
  virtual void          Larger();
  virtual void          Smaller();
  virtual void          SetColour(COLORREF aRef);

protected:
  virtual LRESULT       OnPaint();

private:
  // Prohibit copying
                        ThicknessPreview(const ThicknessPreview &);
  // Prohibit assignment
  ThicknessPreview &    operator=(const ThicknessPreview &);

  void                  UpdatePen();

  DrawingAttributes &   mAttributes;
  HPEN                  mPen;
};

class ThicknessSizeWindow : public ThicknessControl, public SizeWindow
{
public:
                        ThicknessSizeWindow(Window * aParent, HINSTANCE hInstance,
                                            DrawingAttributes & anAttributes);
  virtual               ~ThicknessSizeWindow() {}

  virtual int           GetThickness() const;
  virtual void          SetThickness(int aThickness);
  void                  SetColour(COLORREF aColour);

  virtual bool          SetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx, int cy,
                                     UINT uFlags);

private:
  // Prohibit copying
                        ThicknessSizeWindow(const ThicknessSizeWindow &);
  // Prohibit assignment
  ThicknessSizeWindow & operator=(const ThicknessSizeWindow &);
  ThicknessPreview *    mThicknessPreview;
  DrawingAttributes &   mAttributes;
};


#endif // THICKNESS_THICKNESSSIZEWINDOW_H__