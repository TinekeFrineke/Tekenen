#ifndef RECTUTILITIES_H__
#define RECTUTILITIES_H__

#include <windows.h>

namespace Teken
{


// Whether RECt and the line from point 1 to 2 intersect and where
bool LineSectionInsideRect(const RECT & oRect, const POINT & p1, const POINT & p2, POINT & anIntersect1, POINT & anIntersect2);


} // namespace Teken

#endif