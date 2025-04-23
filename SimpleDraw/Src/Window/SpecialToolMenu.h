#pragma once

#include "Window/FrameWindow.h"
#include "General/Subject.h"

class ColourWindow;
class DrawingAttributes;
class SpecialToolMenu;


class SubMenu: public virtual Subject
{
public:
    SubMenu(SpecialToolMenu& aMenu, DrawingAttributes& anAttributes);
    virtual                 ~SubMenu() {}

    virtual LRESULT         LayoutWindow(const RECT& aClientRect) = 0;

protected:
    SpecialToolMenu& GetMenu() { return mWindow; }
    const SpecialToolMenu& GetMenu() const { return mWindow; }

    DrawingAttributes& GetAttributes() { return mAttributes; }
    const DrawingAttributes&
        GetAttributes() const { return mAttributes; }

private:
    // Prohibit assignment
    SubMenu& operator=(const SubMenu&);
    // Prohibit copying
    SubMenu(const SubMenu&);

    SpecialToolMenu& mWindow;
    DrawingAttributes& mAttributes;
};


class SpecialToolMenu: public FrameWindow
{
public:
    SpecialToolMenu(Window* aParent, DrawingAttributes& anAttributes,
                    HINSTANCE hInstance, const std::string& aName = "SpecialToolMenu");
    ~SpecialToolMenu();

    void                    SetSubMenu(SubMenu* aSuMenu);

protected:
    friend class            SubMenu;

private:
    LRESULT                 OnWindowPosChanged(const WINDOWPOS& aPos);

    SpecialToolMenu& operator=(const SpecialToolMenu&) = delete;
    SpecialToolMenu(const SpecialToolMenu&) = delete;

    SubMenu* mSubMenu;
    DrawingAttributes& mAttributes;
};
