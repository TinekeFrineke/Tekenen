#include "General.h"

#include <algorithm>
#include <random>

namespace Tekenen
{

void Randomize(std::vector<std::string> & aStrings)
{
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(aStrings.begin(), aStrings.end(), g);
}


}
