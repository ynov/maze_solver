#ifndef POINT_H_
#define POINT_H_

class point
{
public:
  point(int _x = 0, int _y = 0);
  point(const point&);
  void operator=(const point&);
  bool equal_to(const point&) const;

  int x;
  int y;
};

#endif
