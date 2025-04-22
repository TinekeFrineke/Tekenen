#ifndef BUTTON_BUTTONSTRATEGY_H__
#define BUTTON_BUTTONSTRATEGY_H__


class Button;

typedef long LRESULT;

class ButtonStrategy
{
public:
                        ButtonStrategy(Button & aButton) : mButton(aButton) {}
  virtual               ~ButtonStrategy() {}

  virtual LRESULT       HandleButtonDown(int aVirtualKey, int anX, int anY) = 0;
  virtual LRESULT       HandleButtonUp(int aVirtualKey, int anX, int anY) = 0;

protected:
  Button &              GetButton() { return mButton; }

private:
  ButtonStrategy &      operator=(const ButtonStrategy &);
                        ButtonStrategy(const ButtonStrategy &);

  Button &              mButton;
};


class PushButtonStategy : public ButtonStrategy
{
public:
                        PushButtonStategy(Button & aButton) : ButtonStrategy(aButton) {}

  virtual LRESULT       HandleButtonDown(int aVirtualKey, int anX, int anY);
  virtual LRESULT       HandleButtonUp(int aVirtualKey, int anX, int anY);
};


class ToggleButtonStrategy : public ButtonStrategy
{
public:
                        ToggleButtonStrategy(Button & aButton) : ButtonStrategy(aButton) {}

  virtual LRESULT       HandleButtonDown(int aVirtualKey, int anX, int anY);
  virtual LRESULT       HandleButtonUp(int aVirtualKey, int anX, int anY);
};


class CustomButtonStrategy : public ButtonStrategy
{
public:
                        CustomButtonStrategy(Button & aButton) : ButtonStrategy(aButton) {}

  virtual LRESULT       HandleButtonDown(int aVirtualKey, int anX, int anY);
  virtual LRESULT       HandleButtonUp(int aVirtualKey, int anX, int anY);
};


#endif // BUTTON_BUTTONSTRATEGY_H__
