#ifndef GENERAL_H__
#define GENERAL_H__

#include <vector>
#include "Window/Window.h"


namespace Tekenen
{
  enum ORIENTATION { OR_Horizontal, OR_Vertical };

  enum DRAW_STATE
  {
    DS_None,
    DS_FreeDraw,
    DS_Smudge,
    DS_Stamp,
    DS_Letter,
    DS_Fill,
    DS_Airbrush
  };

  void Randomize(std::vector<std::tstring> & aStrings);
}


#endif // GENERAL_H__