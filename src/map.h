#ifndef MAP_READER_H_
#define MAP_READER_H_

#include <vector>
#include "point.h"

#define BUFF_SIZE 4096

#define BEGIN_NAMESPACE(_namespace) namespace _namespace {
#define END_NAMESPACE(_namespace) }

#define init_2d_array(h, w, VAR, TYPE, VAL) \
  VAR = new TYPE*[h]; \
  for (int y = 0; y < h; y++) { \
    VAR[y] = new TYPE[w]; \
    for (int x = 0; x < w; x++) { \
      VAR[y][x] = VAL; \
    } \
  }

#define free_2d_array(h, VAR) \
  for (int y = 0; y < h; y++) \
    delete [] VAR[y]; \
  delete [] VAR;

typedef std::vector<point> path_t;

struct map_t {
  int width;
  int height;

  point start;
  point finish;

  int **map_array;
};

BEGIN_NAMESPACE(map_reader)
map_t read(const char *map_file);
END_NAMESPACE(map_reader)

BEGIN_NAMESPACE(map_printer)
void init_printer();
void close_printer();

void mprintf(const char *format, ...);
void mprintf_trace(const char *format, ...);

void print_map(const map_t &map, const std::vector<point> &path);
void print_direction(const std::vector<point> &path, bool with_new_line = false);

void enable_trace_print();
void enable_stdout_write();
END_NAMESPACE(map_printer)

#endif
