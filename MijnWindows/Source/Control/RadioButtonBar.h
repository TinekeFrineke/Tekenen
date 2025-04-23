#ifndef CONTROL_RADIOBUTTONBAR_H__
#define CONTROL_RADIOBUTTONBAR_H__

#include "ButtonBar.h"

class RadioButtonBar: public ButtonBar
{
public:
    RadioButtonBar(Window* aParent, HINSTANCE hInstance,
                   const std::string& aName,
                   LAYOUT_STRATEGY aStrategy);

    virtual void AddButton(Button* aButton, int id) override;

    void SelectButton(Button* aButton);
    void SelectButton(int anID);

    const Button* GetSelectedButton() const { return mSelected; }
    Button* GetSelectedButton() { return mSelected; }
    int                         GetSelectedID() const;

protected:
    virtual LRESULT             OnButtonClick(Button* aButton);

    std::map<Button*, int>& GetButtonMap() { return  mButtonMap; }
    const std::map<Button*, int>& GetButtonMap() const { return  mButtonMap; }

private:
    Button* mSelected;
    std::map<Button*, int> mButtonMap;
};


#endif // CONTROL_RADIOBUTTONBAR_H__