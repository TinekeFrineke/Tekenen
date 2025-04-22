#ifndef SUBMENU_LETTERWINDOW_H__
#define SUBMENU_LETTERWINDOW_H__


#include "General/Font.h"
#include "Custom/SizeWindow.h"


class DrawingAttributes;

class LetterPreview : public Preview
{
public:
                      LetterPreview(Window * aWindow,
                                    HINSTANCE anInstance,
                                    Font & aFont,
                                    const DrawingAttributes & anAttributes);

protected:
  virtual void        Larger();
  virtual void        Smaller();
  virtual LRESULT     OnPaint();

private:
  // Prohibit copying
                      LetterPreview(const LetterPreview &);
  // Prohibit assignment
  LetterPreview &     operator=(const LetterPreview &);
  Font &              mFont;

  const DrawingAttributes &
                      mAttributes;
};


#endif