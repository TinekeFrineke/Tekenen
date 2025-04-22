#ifndef CONTROL_BUTTONBAR_H__
#define CONTROL_BUTTONBAR_H__


#include "Window/Window.h"

#include <map>
#include <vector>


class Button;
class ButtonPanel;

class LayoutStrategy
{
public:
                              LayoutStrategy(ButtonPanel & aPanel)
                                : mPanel(aPanel) {}
                              virtual ~LayoutStrategy() {}

  virtual bool                HasFixedSize() const = 0;
  virtual int                 GetWantedWidth() const = 0;
  virtual int                 GetWantedHeight() const = 0;
  virtual bool                Layout(const RECT & aRect) = 0;

protected:
  std::vector<Button *> &     GetButtons();
  const std::vector<Button *> &
                              GetButtons() const;

private:
  LayoutStrategy &      operator=(const LayoutStrategy &);
                        LayoutStrategy(const LayoutStrategy &);

  ButtonPanel &         mPanel;
};


class FixedSizeStrategy : public LayoutStrategy
{
public:
                        FixedSizeStrategy(ButtonPanel & aPanel,
                                          int iButtonWidth,
                                          int iButtonHeight)
                        : LayoutStrategy(aPanel),
                          mButtonWidth  (iButtonWidth),
                          mButtonHeight (iButtonHeight)
                        {}

  virtual bool          HasFixedSize() const { return true; }

protected:
  int                   GetButtonWidth()  const { return mButtonWidth; }
  int                   GetButtonHeight() const { return mButtonHeight; }

private:
  int                   mButtonWidth;
  int                   mButtonHeight;
};


class HorizontalFixedSizeStrategy : public FixedSizeStrategy
{
public:
                        HorizontalFixedSizeStrategy(ButtonPanel & aPanel,
                                                  int iButtonWidth,
                                                  int iButtonHeight)
                        : FixedSizeStrategy(aPanel, iButtonWidth, iButtonHeight)
                        {}

  virtual int           GetWantedWidth() const;
  virtual int           GetWantedHeight() const;
  virtual bool          Layout(const RECT & aRect);

private:
  HorizontalFixedSizeStrategy &
                        operator=(const HorizontalFixedSizeStrategy &);
                        HorizontalFixedSizeStrategy(const HorizontalFixedSizeStrategy &);
};


class VerticalFixedSizeStrategy : public FixedSizeStrategy
{
public:
                        VerticalFixedSizeStrategy(ButtonPanel & aPanel,
                                                  int iButtonWidth,
                                                  int iButtonHeight)
                        : FixedSizeStrategy(aPanel, iButtonWidth, iButtonHeight)
                        {}

  virtual int           GetWantedWidth() const;
  virtual int           GetWantedHeight() const;
  virtual bool          Layout(const RECT & aRect);

private:
  VerticalFixedSizeStrategy &
                        operator=(const VerticalFixedSizeStrategy &);
                        VerticalFixedSizeStrategy(const VerticalFixedSizeStrategy &);
};


class FitSizeStrategy : public LayoutStrategy
{
public:
                        FitSizeStrategy(ButtonPanel & aPanel)
                        : LayoutStrategy(aPanel)
                        {}

  virtual bool          HasFixedSize() const { return false; }

protected:

private:
};


class HorizontalFitStrategy : public FitSizeStrategy
{
public:
                        HorizontalFitStrategy(ButtonPanel & aPanel)
                        : FitSizeStrategy(aPanel)
                        {}

  virtual int           GetWantedWidth() const;
  virtual int           GetWantedHeight() const;
  virtual bool          Layout(const RECT & aRect);

private:
  HorizontalFitStrategy &
                        operator=(const HorizontalFitStrategy &);
                        HorizontalFitStrategy(const HorizontalFitStrategy &);
};


class VerticalFitStrategy : public FitSizeStrategy
{
public:
                        VerticalFitStrategy(ButtonPanel & aPanel)
                        : FitSizeStrategy(aPanel)
                        {}

  virtual int           GetWantedWidth() const;
  virtual int           GetWantedHeight() const;
  virtual bool          Layout(const RECT & aRect);

private:
  VerticalFitStrategy &
                        operator=(const VerticalFitStrategy &);
                        VerticalFitStrategy(const VerticalFitStrategy &);
};


class ButtonPanel : public MijnWindow<ButtonPanel>
{
public:
                              ButtonPanel(Window * aParent, HINSTANCE hInstance,
                                          const std::tstring & aName);
  virtual                     ~ButtonPanel();

  static TCHAR *              GetClassName() { return const_cast<TCHAR *>(_T("MijnButtonPanel")); }
  static bool                 RegisterClass(HINSTANCE hInstance);

  void                        AddButton(Button * aButton, int anID);

protected:
  std::vector<Button *> &     GetButtons()  { return  mButtons; }
  const std::vector<Button *> &
                              GetButtons()  const { return  mButtons; }
  std::map<Button *, int> &   GetButtonMap()  { return  mButtonMap; }
  const std::map<Button *, int> &
                              GetButtonMap()  const { return  mButtonMap; }

  virtual LRESULT             WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
  virtual LRESULT             OnButtonClick(Button * aButton);
  virtual LRESULT             OnWindowPosChanged(const WINDOWPOS & oPos);

private:
  friend class LayoutStrategy;

  std::vector<Button *>       mButtons;
  std::map<Button *, int>     mButtonMap;
};

// Always sets buttons to size 48 * 48.



class ButtonBar : public ButtonPanel
{
public:
  enum LAYOUT_STRATEGY { LS_HorizontalFixedSize, LS_VerticalFixedSize,
                         LS_HorizontalFit, LS_VerticalFit};

                              ButtonBar(Window * aParent, HINSTANCE hInstance,
                                        const std::tstring & aName, LAYOUT_STRATEGY aStrategy);
  virtual                     ~ButtonBar();

  bool                        HasFixedSize() const { return mStrategy->HasFixedSize(); }
  int                         GetWantedWidth() const { return mStrategy->GetWantedWidth(); }
  int                         GetWantedHeight() const { return mStrategy->GetWantedHeight(); }

protected:
  //virtual LRESULT             WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
  virtual LRESULT             OnWindowPosChanged(const WINDOWPOS & oPos);

private:
  LayoutStrategy *            mStrategy;
};


#endif // CONTROL_BUTTONBAR_H__
