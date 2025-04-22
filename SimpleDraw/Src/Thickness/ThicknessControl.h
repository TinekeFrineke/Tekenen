#ifndef THICKNESS_THICKNESSCONTROL_H__
#define THICKNESS_THICKNESSCONTROL_H__


#include "Window/Window.h"
#include "General/Subject.h"


class ThicknessControl : public virtual Subject
{
public:
  virtual               ~ThicknessControl() {}

  virtual bool          SetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx, int cy,
                                     UINT uFlags) = 0;
  virtual int           GetThickness() const = 0;
  virtual void          SetThickness(int aThickness) = 0;
  virtual void          SetColour(COLORREF aColour) = 0;
};


#endif // THICKNESS_THICKNESSCONTROL_H__
