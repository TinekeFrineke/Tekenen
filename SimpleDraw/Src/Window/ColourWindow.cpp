#include "ColourWindow.h"

#include <assert.h>

#include "General/Controller.h"
#include "Button/ColourButton.h"


template<>
bool MijnWindow<ColourWindow>::mRegistered = false;


///*
static COLORREF lColours[2][12] =
{
  {
    RGB(0x00, 0x00, 0x00), // Black
    RGB(0x80, 0x80, 0x80), // Dark Grey
    RGB(0x80, 0x20, 0x00), // Dark Red
    RGB(0xa0, 0x80, 0x20), // Ocre
    RGB(0x00, 0x80, 0x00), // Dark Apple Green
    RGB(0x00, 0x80, 0x80), // Dark Turqoise
    RGB(0x00, 0x00, 0x80), // Dark Blue
    RGB(0x80, 0x00, 0x80), // Dark Magenta
    RGB(0x00, 0x40, 0x40), // Dark Green
    RGB(0x40, 0x00, 0x80), // Dark Purple
    RGB(0xd0, 0x70, 0x00), // Dark Orange
    RGB(0x40, 0x40, 0x40), // Darker grey
  },
  {
    RGB(0xff, 0xff, 0xff), // White
    RGB(0xb0, 0xb0, 0xb0), // Light gray
    RGB(0xff, 0x00, 0x00), // Light red
    RGB(0xff, 0xff, 0x00), // Light yellow
    RGB(0x00, 0xff, 0x00), // Light green
    RGB(0x00, 0xff, 0xff), // Light Turqoise
    RGB(0x00, 0x00, 0xff), // Light blue
    RGB(0xff, 0x00, 0xff), // Light Magenta
    RGB(0xff, 0x90, 0xc0), // Pink
    RGB(0xc0, 0x80, 0xc0), // Light Purple
    RGB(0xff, 0xc0, 0x40), // Light orange
    RGB(0xa0, 0xd0, 0xff), // Light blue
  }
};
//*/

int nrrows = sizeof(lColours) / sizeof(lColours[0]);
int nrcols = sizeof(lColours[0]) / sizeof(lColours[0][0]);

ColourWindow::ColourWindow(Window * aParent, DrawingAttributes & anAttributes,
                           HINSTANCE hInstance, const std::string & aName)
: GenericWindow(aParent, hInstance, aName, WS_CHILD),
  mAttributes             (anAttributes),
  mPressedButton          (NULL)
{
  //Create(GetClassName(), "KleurenWindow"), WS_CHILD);

  for (int row = 0; row < nrrows; ++row)
    for (int column = 0; column < nrcols; ++column) {
      TCHAR name[64];
      sprintf(name, "ColourButton %02d", column * nrrows + row);
      ColourButton * button = new ColourButton(this, hInstance, name, Button::BUTTON_STRATEGY::ToggleButton);
      RECT rect = { 0, 0, 1, 1 };
      button->Create(rect, lColours[row][column]);
      if (lColours[row][column] == anAttributes.mColour)
      {
        mPressedButton = button;
        mPressedButton->SetPressed(true);
      }

      mButtonList.push_back(button);
    }
}


ColourWindow::~ColourWindow()
{
}


COLORREF ColourWindow::GetColour() const
{
  if (mPressedButton != NULL)
    return mPressedButton->GetColour();

  assert(false);
  return 0x000000;
}


LRESULT ColourWindow::OnWindowPosChanged(const WINDOWPOS & oPos)
{
  int iLeft   = 2;
  int iWidth  = oPos.cx / nrcols;
  int iTop    = 2;
  int iHeight = oPos.cy / nrrows;

  for (int row = 0; row < nrrows; ++row) {
    for (int column = 0; column < nrcols; ++column) {
      ColourButton * button = mButtonList[column + row * nrcols];
      RECT brect = { iLeft + iWidth *  column      + 2, iTop + iHeight *  row      + 2,
                     iLeft + iWidth * (column + 1) - 4, iTop + iHeight * (row + 1) - 4 };
      button->SetWindowPos(0, brect.left, brect.top, brect.right -  brect.left, brect.bottom - brect.top, SWP_SHOWWINDOW);
    }
  }

  return 0;
}


LRESULT ColourWindow::OnButtonClicked(ColourButton * aButton)
{
  mAttributes.mColour = aButton->GetColour();

  if (mPressedButton)
    mPressedButton->SetPressed(false);
  mPressedButton = aButton;
  if (mPressedButton)
    mPressedButton->SetPressed(true);

  // Communicate the change
  Notify();

  return 0;
}


LRESULT ColourWindow::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  switch (uMsg) {
    case WM_USER_BUTTON_CLICKED:
      return OnButtonClicked((ColourButton *)lParam);

    default:
      return GenericWindow::WindowProc(uMsg, wParam, lParam);
  }
}
//
//
//bool ColourWindow::RegisterClass(HINSTANCE hInstance)
//{
//  WNDCLASS sWndClass;
//  memset(&sWndClass, 0, sizeof(WNDCLASS));
//  sWndClass.hbrBackground = GetSysColorBrush(COLOR_APPWORKSPACE);
//  sWndClass.lpszClassName = GetClassName();
//  sWndClass.lpfnWndProc = Window::WindowProc;
//  sWndClass.hInstance = hInstance;
//  sWndClass.hCursor = LoadCursor(0, IDC_CROSS);
//
//  return ::RegisterClass(&sWndClass) != NULL;
//}
