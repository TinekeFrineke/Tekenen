#include "RectUtilities.h"

#include <vector>


namespace Teken
{


bool IntersectionWithX(const POINT & aP1, const POINT & aP2, int anX, POINT & anIntersect)
{
  if (aP2.x == aP1.x)
    return false;

  float t = (float(anX) - float(aP1.x)) / (float(aP2.x) - float(aP1.x));
  if (t < 0 || t > 1)
    return false;

  anIntersect.x = anX;
  anIntersect.y = LONG((1 - t) * aP1.y + t * aP2.y);
  return true;
}


bool IntersectionWithY(const POINT & aP1, const POINT & aP2, int anY, POINT & anIntersect)
{
  if (aP2.y == aP1.y)
    return false;

  float t = (float(anY) - float(aP1.y)) / (float(aP2.y) - float(aP1.y));
  if (t < 0 || t > 1)
    return false;

  anIntersect.y = anY;
  anIntersect.x = LONG((1 - t) * aP1.x + t * aP2.x);
  return true;
}


bool LineSectionInsideRect(const RECT & aRect, const POINT & aP1, const POINT & aP2, POINT & anIntersect1, POINT & anIntersect2)
{
  bool p1inrect = PtInRect(&aRect, aP1) != FALSE;
  bool p2inrect = PtInRect(&aRect, aP2) != FALSE;

  if (p1inrect && p2inrect)
  {
    anIntersect1 = aP1;
    anIntersect2 = aP2;
    return true;
  }

  std::vector<POINT> lIntersect;
  POINT intersect;
  if (IntersectionWithX(aP1, aP2, aRect.left, intersect))
    if (intersect.y >= aRect.top && intersect.y <= aRect.bottom)
      lIntersect.push_back(intersect);
  if (IntersectionWithX(aP1, aP2, aRect.right, intersect))
    if (intersect.y >= aRect.top && intersect.y <= aRect.bottom)
      lIntersect.push_back(intersect);
  if (IntersectionWithY(aP1, aP2, aRect.top, intersect))
    if (intersect.x >= aRect.left && intersect.x <= aRect.right)
      lIntersect.push_back(intersect);
  if (IntersectionWithY(aP1, aP2, aRect.bottom, intersect))
    if (intersect.x >= aRect.left && intersect.x <= aRect.right)
      lIntersect.push_back(intersect);

  if (lIntersect.empty())
    return false;

  if (p1inrect || p2inrect)
  {
    anIntersect1 = (p1inrect ? aP1 : aP2);
    anIntersect2 = lIntersect[0];
  }
  else if (lIntersect.size() >= 2)
  {
    anIntersect1 = lIntersect[0];
    anIntersect2 = lIntersect[1];
  }
  else
  {
    return false;
  }

  return true;
}


}
