#ifndef BUTTON_FONTBUTTON_H__
#define BUTTON_FONTBUTTON_H__


#include "Button/Button.h"
#include "Button/ButtonStrategy.h"
#include "General/Font.h"


class HoverWindow;

class FontButton : public Button
{
public:
                          FontButton(Window * aParent, HINSTANCE hInstance,
                                     const std::string & aName = "FontButton");
                          ~FontButton();

  bool                    SetFont(const Font & aFont);
  const Font *            GetFont() const {return mFont; }

protected:
  LRESULT                 OnPaint();

private:
  Font *                  mFont;

  static const TCHAR *    mTestString[2];
};


#endif // BUTTON_FONTBUTTON_H__