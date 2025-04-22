#include "ThicknessPanel.h"


ThicknessButton::ThicknessButton(Window * aParent, HINSTANCE hInstance,
                                 const std::tstring & aName,
                                 Button::BUTTON_STRATEGY aStrategy)
: ColourButton(aParent, hInstance, aName, aStrategy),
  mPen        (NULL)
{
  mPen = CreatePen(PS_SOLID, 1, 0xffffff);
}


void ThicknessButton::SetThickness(int aThickness)
{
  if (mThickness == aThickness)
    return;

  mThickness = aThickness;

  DeleteObject(mPen);
  mPen = CreatePen(PS_SOLID, mThickness, mLineColour);
}


void ThicknessButton::SetColour(COLORREF aRef)
{
  if (aRef == mLineColour)
    return;

  mLineColour = aRef;
  DeleteObject(mPen);
  mPen = CreatePen(PS_SOLID, mThickness, mLineColour);

  if (GetRValue(aRef) + GetGValue(aRef) * 2 + GetBValue(aRef) > 225 * 3)
    // Very light colour:
    ColourButton::SetColour(RGB(0x8f, 0x8f, 0x8f));
  else
    ColourButton::SetColour(RGB(0xff, 0xff, 0xff));

  InvalidateRect(GetHwnd(), NULL, false);
}


LRESULT ThicknessButton::OnPaint(HDC hDC, const RECT & aRect)
{
  if (ColourButton::OnPaint(hDC, aRect) != 0)
    return -1;

  // Draw a horizontal line
  int iHBorder = (aRect.right - aRect.left) / 16;

  HGDIOBJ hObject = SelectObject(hDC, mPen);
  MoveToEx(hDC, aRect.left + iHBorder, (aRect.bottom - aRect.top) / 2, NULL);
  LineTo(hDC, aRect.right - iHBorder, (aRect.bottom - aRect.top) / 2);
  SelectObject(hDC, hObject);
  return 0;
}


ThicknessPanel::ThicknessPanel(Window * aParent, HINSTANCE hInstance,
                               const std::tstring & aName)
: RadioButtonBar(aParent, hInstance, aName, ButtonBar::LS_VerticalFit)
{
  for (int i = 0; i < 6; ++i)
  {
    ThicknessButton * button = new ThicknessButton(this, hInstance, _T("ThicknessButton"), Button::BS_ToggleButton);
    button->SetColour(RGB(0x00, 0xff, 0xff));
    button->SetThickness(i);
    AddButton(button, i);
  }

  SelectButton(GetButtons()[0]);
}


bool ThicknessPanel::SetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx, int cy,
                                  UINT uFlags)
{
  return RadioButtonBar::SetWindowPos(hWndInsertAfter, X, Y, cx, cy, uFlags);
}


LRESULT ThicknessPanel::OnButtonClick(Button * aButton)
{
  ThicknessButton * button = dynamic_cast<ThicknessButton *>(aButton);
  if (button != NULL)
  {
    mSelected = button;
    Notify();
  }

  return RadioButtonBar::OnButtonClick(aButton);
}


ThicknessPanel::~ThicknessPanel()
{
}


int ThicknessPanel::GetThickness() const
{
  return mSelected->GetThickness();
}


void ThicknessPanel::SetThickness(int aThickness)
{
  if (aThickness < 1)
  {
    SelectButton(GetButtons()[0]);
  }
  else if ((unsigned int)aThickness > GetButtons().size())
  {
    SelectButton(GetButtons()[GetButtons().size() - 1]);
  }
  else
  {
    SelectButton(GetButtons()[aThickness - 1]);
  }
}


void ThicknessPanel::SetColour(COLORREF aRef)
{
  for (size_t i = 0; i < GetButtons().size(); ++i)
    dynamic_cast<ThicknessButton *>(GetButtons()[i])->SetColour(aRef);
}
