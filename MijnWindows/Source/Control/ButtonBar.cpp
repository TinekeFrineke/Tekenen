#include "ButtonBar.h"

#include "Button/Button.h"


std::vector<Button *> & LayoutStrategy::GetButtons()
{
  return mPanel.GetButtons();
}


const std::vector<Button *> & LayoutStrategy::GetButtons() const
{
  return mPanel.GetButtons();
}


bool HorizontalFixedSizeStrategy::Layout(const RECT & aRect)
{
  (void)aRect;
  for (size_t i = 0; i < GetButtons().size(); ++i)
    GetButtons()[i]->SetWindowPos(NULL, 2 + int(i) * (GetButtonHeight() + 2), 2,
                                  GetButtonWidth(), GetButtonHeight(), SWP_SHOWWINDOW);
  return true;
}


int HorizontalFixedSizeStrategy::GetWantedWidth() const
{
  return (int)GetButtons().size() * (GetButtonWidth() + 2) + 2;
}


int HorizontalFixedSizeStrategy::GetWantedHeight() const
{
  return GetButtonHeight() + 4;
}


bool VerticalFixedSizeStrategy::Layout(const RECT & aRect)
{
  (void)aRect;
  for (size_t i = 0; i < GetButtons().size(); ++i)
    GetButtons()[i]->SetWindowPos(NULL, 2, 2 + int(i) * (GetButtonWidth() + 2),
                                  GetButtonWidth(), GetButtonHeight(), SWP_SHOWWINDOW);
  return true;
}


int VerticalFixedSizeStrategy::GetWantedWidth() const
{
  return GetButtonWidth() + 4;
}


int VerticalFixedSizeStrategy::GetWantedHeight() const
{
  return (int)GetButtons().size() * (GetButtonHeight() + 2) + 2;
}


int HorizontalFitStrategy::GetWantedWidth() const
{
  return 0;
}


int HorizontalFitStrategy::GetWantedHeight() const
{
  return 0;
}


bool HorizontalFitStrategy::Layout(const RECT & aRect)
{
  if (GetButtons().size() == 0)
    return false;

  int iWidth  = (aRect.right - aRect.left - (int(GetButtons().size()) + 1) * 2) / int(GetButtons().size());
  int iHeight  = aRect.bottom - aRect.top - 4;

  for (size_t i = 0; i < GetButtons().size(); ++i)
    GetButtons()[i]->SetWindowPos(NULL, 2 + int(i) * (iWidth + 2), 2,
                                  iWidth, iHeight, SWP_SHOWWINDOW);

  return true;
}


int VerticalFitStrategy::GetWantedWidth() const
{
  return 0;
}


int VerticalFitStrategy::GetWantedHeight() const
{
  return 0;
}


bool VerticalFitStrategy::Layout(const RECT & aRect)
{
  int iWidth  = aRect.right - aRect.left - 4;
  int iHeight = (aRect.bottom - aRect.top - (int(GetButtons().size()) + 1) * 2) / int(GetButtons().size());

  for (size_t i = 0; i < GetButtons().size(); ++i)
    GetButtons()[i]->SetWindowPos(NULL, 2, 2 + int(i) * (iHeight + 2),
                                  iWidth, iHeight, SWP_SHOWWINDOW);
  return true;
}


template<>
bool MijnWindow<ButtonPanel>::mRegistered = false;

ButtonPanel::ButtonPanel(Window * aParent, HINSTANCE hInstance,
                         const std::tstring & aName)
: MijnWindow<ButtonPanel> (aParent, hInstance, aName)
{
}


ButtonPanel::~ButtonPanel()
{
}


bool ButtonPanel::RegisterClass(HINSTANCE hInstance)
{
  WNDCLASS wndclass;
  memset(&wndclass, 0, sizeof(WNDCLASS));

  wndclass.hInstance = hInstance;
  wndclass.lpfnWndProc = MijnWindow<ButtonBar>::WindowProc;
  wndclass.lpszClassName = GetClassName();
  wndclass.style = CS_HREDRAW | CS_VREDRAW;
  wndclass.hbrBackground = (HBRUSH)GetSysColorBrush(COLOR_BACKGROUND);
  //wndclass.hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));

  return ::RegisterClass(&wndclass) != NULL;
}


void ButtonPanel::AddButton(Button * aButton, int iID)
{
  mButtons.push_back(aButton);
  mButtonMap[aButton] = iID;
}


LRESULT ButtonPanel::OnWindowPosChanged(const WINDOWPOS & oPos)
{
  for (size_t i = 0; i < GetButtons().size(); ++i)
    GetButtons()[i]->SetWindowPos(NULL, 2 + int(i) * 50, 2,
                                    48, 48, oPos.flags);

  return 0;
}


LRESULT ButtonPanel::OnButtonClick(Button * aButton)
{
  GetParent()->SendMessage(Window::WM_USER_BUTTON_CLICKED, 0, LPARAM(aButton));
  return 0;
}


LRESULT ButtonPanel::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg) {
    case WM_USER_BUTTON_CLICKED:
      return OnButtonClick((Button *)lParam);

    default:
      return MijnWindow<ButtonPanel>::WindowProc(uMsg, wParam, lParam);
  }
}


ButtonBar::ButtonBar(Window * aParent, HINSTANCE hInstance,
                     const std::tstring & aName, LAYOUT_STRATEGY aStrategy)
: ButtonPanel   (aParent, hInstance, aName),
  mStrategy     (NULL)
{
  Create(GetClassName(), L"MijnButtonBar", WS_CHILD);

  switch (aStrategy)
  {
  case LS_HorizontalFixedSize:
    mStrategy = new HorizontalFixedSizeStrategy(*this, 48, 48);
    break;

  case LS_VerticalFixedSize:
    mStrategy = new VerticalFixedSizeStrategy(*this, 48, 48);
    break;

  case LS_HorizontalFit:
    mStrategy = new HorizontalFitStrategy(*this);
    break;

  case LS_VerticalFit:
    mStrategy = new VerticalFitStrategy(*this);
    break;
  }
}


ButtonBar::~ButtonBar()
{
  delete mStrategy;
}


LRESULT ButtonBar::OnWindowPosChanged(const WINDOWPOS & oPos)
{
  (void)oPos;
  RECT rect;
  GetClientRect(rect);
  mStrategy->Layout(rect);

  return 0;
}
//
//
//LRESULT ButtonBar::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//  switch (uMsg) {
//    //case WM_WINDOWPOSCHANGED:
//    //  return OnWindowPosChanged(*(WINDOWPOS *)lParam);
//
//    default:
//      return ButtonPanel::WindowProc(uMsg, wParam, lParam);
//  }
//}
