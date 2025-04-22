#include "FillState.h"

#include "Resource/resource.h"
#include "FillSubMenu.h"
#include "Window/SpecialToolMenu.h"
#include "Window/DrawingWindow.h"


class SubToolMenu;

FillState::FillState(DrawingWindow &      aWindow,
                     HINSTANCE            anInstance,
                     Controller &         aController,
                     SpecialToolMenu &    aMenu,
                     DrawingAttributes &  anAttributes)
: DrawingStateWithCursor(aWindow, anInstance, aController, anAttributes, IDC_EMMER),
  mBrush                (NULL)
{
  LOGBRUSH logbrush;
  logbrush.lbColor = anAttributes.mColour;
  logbrush.lbStyle = BS_SOLID;
  logbrush.lbHatch = 0;
  mBrush = CreateBrushIndirect(&logbrush);

  SelectObject(GetWindow().GetHDC(), HGDIOBJ(mBrush));

  aMenu.SetSubMenu(new FillSubMenu(aMenu, *this, anInstance, anAttributes));
}


FillState::~FillState()
{
  DeleteObject(mBrush);
}


LRESULT FillState::ProcessMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
  (void)wParam;
  switch (uMsg) {
    case WM_LBUTTONDOWN:
      return ProcessLButtonDown((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam));
  }

  return DrawingStateWithCursor::ProcessMessage(uMsg, wParam, lParam);
}


LRESULT FillState::ProcessLButtonDown(int anX, int anY)
{
  FlushRedo();

  COLORREF colour = GetPixel(GetWindow().GetHDC(), anX, anY);
  ExtFloodFill(GetWindow().GetHDC(), anX, anY, colour, FLOODFILLSURFACE);
  InvalidateRect(GetWindow().GetHwnd(), NULL, false);

  Store();

  return 0;
}


void FillState::SetColour(COLORREF aColour)
{
  GetAttributes().mColour = aColour;

  LOGBRUSH logbrush;
  logbrush.lbColor = GetAttributes().mColour;
  logbrush.lbStyle = BS_SOLID;
  logbrush.lbHatch = 0;
  mBrush = CreateBrushIndirect(&logbrush);
  HGDIOBJ oldbrush = SelectObject(GetWindow().GetHDC(), mBrush);

  DeleteObject(oldbrush);
}
