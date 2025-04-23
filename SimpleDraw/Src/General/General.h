#ifndef GENERAL_H__
#define GENERAL_H__

#include <vector>
#include "Window/Window.h"


namespace Tekenen
{
  enum class ORIENTATION { Horizontal, Vertical };

  enum class DRAW_STATE
  {
    None,
    FreeDraw,
    Smudge,
    Stamp,
    Letter,
    Fill,
    Airbrush
  };

  void Randomize(std::vector<std::string> & aStrings);
}


#endif // GENERAL_H__