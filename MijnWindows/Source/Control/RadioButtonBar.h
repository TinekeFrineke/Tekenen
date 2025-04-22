#ifndef CONTROL_RADIOBUTTONBAR_H__
#define CONTROL_RADIOBUTTONBAR_H__

#include "ButtonBar.h"

class RadioButtonBar : public ButtonBar
{
public:
                              RadioButtonBar(Window * aParent, HINSTANCE hInstance,
                                             const std::tstring & aName,
                                             LAYOUT_STRATEGY aStrategy);

  void                        SelectButton(Button * aButton);
  void                        SelectButton(int anID);

  const Button *              GetSelectedButton() const { return mSelected; }
  Button *                    GetSelectedButton() { return mSelected; }
  int                         GetSelectedID() const;

protected:
  virtual LRESULT             OnButtonClick(Button * aButton);

private:
  Button *                    mSelected;
};


#endif // CONTROL_RADIOBUTTONBAR_H__