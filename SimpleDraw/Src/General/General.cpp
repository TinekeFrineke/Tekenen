#include "General.h"

#include <algorithm>

namespace Tekenen
{

void Randomize(std::vector<std::tstring> & aStrings)
{
  random_shuffle(aStrings.begin(), aStrings.end());
}


}
