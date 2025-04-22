#ifndef WINDOW_DEFINITIONS_H__
#define WINDOW_DEFINITIONS_H__


#include "Window/Window.h"

namespace WinDefs
{

  
enum UserMessages
{
  WM_HOVERWINDOW_CLICKED = Window::WM_LAST_USER_MESSAGE + 1,
  WM_LETTERWINDOW_READY
};


} // namespace WinDefs


#endif // WINDOW_DEFINITIONS_H__