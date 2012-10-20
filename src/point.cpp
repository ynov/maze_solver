#include "point.h"

point::point(int _x, int _y) : x(_x), y(_y) {}
point::point(const point& p) : x(p.x), y(p.y) {}

void point::operator=(const point& p)
{
  x = p.x;
  y = p.y;
}

bool point::equal_to(const point& p) const
{
  return (x == p.x && y == p.y);
}
