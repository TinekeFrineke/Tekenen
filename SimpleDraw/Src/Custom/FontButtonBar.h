#ifndef CUSTOM_FONTBUTTONBAR_H__
#define CUSTOM_FONTBUTTONBAR_H__


#include "ButtonBarTemplate.h"
//#include "Window/FrameWindow.h"

#include <vector>

class FontButton;
class Font;
//class HoverButton;
//class SingleBitmapButton;
class LetterState;


class FontButtonBar : public ButtonBarTemplate<FontButton>
{
public:
                              FontButtonBar(Window * aParent, HINSTANCE hInstance,
                                            LetterState & aState,
                                            int aRows, int aColumns);

  void                        SetFontNames(const std::vector<std::tstring> & aFontNames);

  virtual void                ScrollLeft();
  virtual void                ScrollRight();

  const Font *                GetFont() const;

protected:
  virtual LRESULT             WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

  void                        FillButtons();
  LRESULT                     OnButtonClicked(Button * aButton);

private:
                              FontButtonBar(const FontButtonBar &);
  FontButtonBar &             operator=(const FontButtonBar &);

  LetterState &               mState;
  
  std::vector<std::tstring>   mFontNames;
  int                         mCurrentIndex;

  FontButton *                mSelected;
};


#endif // CUSTOM_FONTBUTTONBAR_H__