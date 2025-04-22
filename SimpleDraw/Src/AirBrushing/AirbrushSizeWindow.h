#ifndef AIRBRUSH_AIRBRUSHSIZEWINDOW_H__
#define AIRBRUSH_AIRBRUSHSIZEWINDOW_H__

#include "Thickness/ThicknessControl.h"
#include "Custom/SizeWindow.h"


class DrawingAttributes;

class AirbrushPreview : public Preview
{
public:
                        AirbrushPreview(Window *aParent, HINSTANCE anInstance,
                                         DrawingAttributes & anAttributes);

  void                  SetThickness(int aThickness);
  virtual void          Larger();
  virtual void          Smaller();
  virtual void          SetColour(COLORREF aRef);

protected:
  virtual LRESULT       OnPaint();

private:
  // Prohibit copying
                        AirbrushPreview(const AirbrushPreview &);
  // Prohibit assignment
  AirbrushPreview &    operator=(const AirbrushPreview &);

  void                  UpdatePen();

  DrawingAttributes &   mAttributes;
  HPEN                  mPen;
};

class AirbrushSizeWindow : public ThicknessControl, public SizeWindow
{
public:
                        AirbrushSizeWindow(Window * aParent, HINSTANCE hInstance,
                                            DrawingAttributes & anAttributes);
  virtual               ~AirbrushSizeWindow() {}

  virtual int           GetThickness() const;
  virtual void          SetThickness(int aThickness);
  void                  SetColour(COLORREF aColour);

  virtual bool          SetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx, int cy,
                                     UINT uFlags);

private:
  // Prohibit copying
                        AirbrushSizeWindow(const AirbrushSizeWindow &);
  // Prohibit assignment
  AirbrushSizeWindow & operator=(const AirbrushSizeWindow &);
  AirbrushPreview *    mThicknessPreview;
  DrawingAttributes &   mAttributes;
};


#endif // AIRBRUSH_AIRBRUSHSIZEWINDOW_H__