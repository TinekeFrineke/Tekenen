#ifndef BUTTON_H__
#define BUTTON_H__


#include "Window/Window.h"


class ButtonStrategy;

class Button : public MijnWindow<Button>
{
public:
  enum BUTTON_STRATEGY { BS_PushButton, BS_ToggleButton, BS_CustomButton };

                          Button(Window * aParent, HINSTANCE hInstance,
                                 const std::tstring & aName, BUTTON_STRATEGY aStrategy);

                          ~Button();

  LRESULT                 WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

  bool                    IsPressed() const { return mPressed; }
  void                    SetPressed(bool aPressed);

  static bool             RegisterClass(HINSTANCE hInstance);

protected:
  // Method was provided because strategy constructor needs button reference on construction
  void                    SetStrategy(ButtonStrategy * aStrategy);

  static TCHAR *          GetClassName() { return const_cast<TCHAR *>(_T("MijnButton")); }

private:
  /// handle paint request
  virtual LRESULT         OnPaint() = 0;
  virtual LRESULT         OnLeftButtonDown(int aVirtualKey, int anX, int anY);
  virtual LRESULT         OnLeftButtonUp(int aVirtualKey, int anX, int anY);
  virtual LRESULT         OnWindowPosChanged(const WINDOWPOS & oPos) { (void)oPos; return 0;}

  ButtonStrategy *        MakeStrategy(BUTTON_STRATEGY aStrategy);

  ButtonStrategy *        mStrategy;
  bool                    mPressed;
};


#endif // BUTTON_H__
