#pragma once


#include "Window/Window.h"

#include <map>
#include <vector>


class Button;
class ButtonPanel;

class LayoutStrategy
{
public:
    LayoutStrategy(ButtonPanel& aPanel)
        : mPanel(aPanel) {
    }
    virtual ~LayoutStrategy() {}

    virtual bool                HasFixedSize() const = 0;
    virtual int                 GetWantedWidth() const = 0;
    virtual int                 GetWantedHeight() const = 0;
    virtual bool                Layout(const RECT& aRect) = 0;

protected:
    std::vector<Button*>& GetButtons();
    const std::vector<Button*>&
        GetButtons() const;

private:
    LayoutStrategy& operator=(const LayoutStrategy&) = delete;
    LayoutStrategy(const LayoutStrategy&) =  delete;

    ButtonPanel& mPanel;
};


class FixedSizeStrategy: public LayoutStrategy
{
public:
    FixedSizeStrategy(ButtonPanel& aPanel,
                      int iButtonWidth,
                      int iButtonHeight)
        : LayoutStrategy(aPanel),
        mButtonWidth(iButtonWidth),
        mButtonHeight(iButtonHeight)
    {
    }

    virtual bool          HasFixedSize() const { return true; }

protected:
    int                   GetButtonWidth()  const { return mButtonWidth; }
    int                   GetButtonHeight() const { return mButtonHeight; }

private:
    int                   mButtonWidth;
    int                   mButtonHeight;
};


class HorizontalFixedSizeStrategy: public FixedSizeStrategy
{
public:
    HorizontalFixedSizeStrategy(ButtonPanel& aPanel,
                                int iButtonWidth,
                                int iButtonHeight)
        : FixedSizeStrategy(aPanel, iButtonWidth, iButtonHeight)
    {
    }

    virtual int           GetWantedWidth() const;
    virtual int           GetWantedHeight() const;
    virtual bool          Layout(const RECT& aRect);

private:
    HorizontalFixedSizeStrategy&
        operator=(const HorizontalFixedSizeStrategy&);
    HorizontalFixedSizeStrategy(const HorizontalFixedSizeStrategy&);
};


class VerticalFixedSizeStrategy: public FixedSizeStrategy
{
public:
    VerticalFixedSizeStrategy(ButtonPanel& aPanel,
                              int iButtonWidth,
                              int iButtonHeight)
        : FixedSizeStrategy(aPanel, iButtonWidth, iButtonHeight)
    {
    }

    virtual int           GetWantedWidth() const;
    virtual int           GetWantedHeight() const;
    virtual bool          Layout(const RECT& aRect);

private:
    VerticalFixedSizeStrategy&
        operator=(const VerticalFixedSizeStrategy&);
    VerticalFixedSizeStrategy(const VerticalFixedSizeStrategy&);
};


class FitSizeStrategy: public LayoutStrategy
{
public:
    FitSizeStrategy(ButtonPanel& aPanel)
        : LayoutStrategy(aPanel)
    {
    }

    virtual bool          HasFixedSize() const { return false; }

protected:

private:
};


class HorizontalFitStrategy: public FitSizeStrategy
{
public:
    HorizontalFitStrategy(ButtonPanel& aPanel)
        : FitSizeStrategy(aPanel)
    {
    }

    virtual int           GetWantedWidth() const;
    virtual int           GetWantedHeight() const;
    virtual bool          Layout(const RECT& aRect);

private:
    HorizontalFitStrategy&
        operator=(const HorizontalFitStrategy&);
    HorizontalFitStrategy(const HorizontalFitStrategy&);
};


class VerticalFitStrategy: public FitSizeStrategy
{
public:
    VerticalFitStrategy(ButtonPanel& aPanel)
        : FitSizeStrategy(aPanel)
    {
    }

    virtual int           GetWantedWidth() const;
    virtual int           GetWantedHeight() const;
    virtual bool          Layout(const RECT& aRect);

private:
    VerticalFitStrategy& operator=(const VerticalFitStrategy&) = delete;
    VerticalFitStrategy(const VerticalFitStrategy&) = delete;
};


class ButtonPanel: public MijnWindow<ButtonPanel>
{
public:
    ButtonPanel(Window* aParent, HINSTANCE hInstance,
                const std::string& aName);
    virtual                     ~ButtonPanel();

    static char* GetClassName() { return const_cast<char*>("MijnButtonPanel"); }
    static bool                 RegisterClass(HINSTANCE hInstance);

    virtual void AddButton(Button* aButton, int id);

protected:
    std::vector<Button*>& GetButtons() { return  mButtons; }
    const std::vector<Button*>& GetButtons()  const { return  mButtons; }

    virtual LRESULT             WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT             OnButtonClick(Button* aButton);
    virtual LRESULT             OnWindowPosChanged(const WINDOWPOS& oPos);

private:
    friend class LayoutStrategy;

    std::vector<Button*> mButtons;
};

// Always sets buttons to size 48 * 48.



class ButtonBar: public ButtonPanel
{
public:
    enum class LAYOUT_STRATEGY {
        HorizontalFixedSize, VerticalFixedSize,
        HorizontalFit, VerticalFit
    };

    ButtonBar(Window* aParent, HINSTANCE hInstance,
              const std::string& aName, LAYOUT_STRATEGY aStrategy);
    virtual                     ~ButtonBar();

    bool                        HasFixedSize() const { return mStrategy->HasFixedSize(); }
    int                         GetWantedWidth() const { return mStrategy->GetWantedWidth(); }
    int                         GetWantedHeight() const { return mStrategy->GetWantedHeight(); }

protected:
    //virtual LRESULT             WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual LRESULT             OnWindowPosChanged(const WINDOWPOS& oPos);

private:
    LayoutStrategy* mStrategy;
};
