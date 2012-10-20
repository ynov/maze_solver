#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>

#include "point.h"
#include "map.h"

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //

// [map_reader] ////
BEGIN_NAMESPACE(map_reader)
static map_t map;

static void init_map()
{
  map.width = -1;
  map.height = 0;
  map.start = point(-1, -1);
  map.finish = point(-1, -1);
  map.map_array = (int**) 0;
}

static char *read_to_string(const char *map_file)
{
  char map_buff[BUFF_SIZE];
  FILE *file;
  int c;
  int i;
  int len;
  char *map_str;

  i = 0;
  file = fopen(map_file, "r");

  do {
    c = getc(file);
    if (c == '\n') {
      if (map.width == -1)
        map.width = i;
      map.height++;
    } else {
      map_buff[i++] = c;
    }
  } while (c != EOF);

  map_buff[i - 1] = '\0';

  fclose(file);

  len = strlen(map_buff);
  map_str = new char[len];
  strncpy(map_str, map_buff, len);

  return map_str;
}

map_t read(const char *map_file)
{
  int i;
  char *map_str;
  int **map_array;

  init_map();
  map_str = read_to_string(map_file);

  i = 0;
  map_array = new int*[map.height];
  for (int y = 0; y < map.height; y++) {
    map_array[y] = new int[map.width];
    for (int x = 0; x < map.width; x++) {
      map_array[y][x] = map_str[i++] - 48;
    }
  }

  sscanf(&map_str[i], "(%d, %d), (%d, %d)",
    &map.start.x, &map.start.y,
    &map.finish.x, &map.finish.y
  );

  map.map_array = map_array;

  delete [] map_str;
  return map;
}
END_NAMESPACE(map_reader)
// [/map_reader] ////

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //

// [map_printer] ////
BEGIN_NAMESPACE(map_printer)
static bool stdout_write = false;
static bool trace_print = false;

static FILE *output = NULL;

void init_printer()
{
  if (!stdout_write)
    output = fopen("output.txt", "w+");
  else
    output = stdout;
}

void close_printer()
{
  if (!stdout_write)
    fclose(output);
}

void mprintf(const char *format, ...)
{
  va_list args;
  va_start(args, format);

  vfprintf(output, format, args);

  va_end(args);
}

void mprintf_trace(const char* format, ...)
{
  va_list args;
  va_start(args, format);

  if (trace_print)
    vfprintf(output, format, args);

  va_end(args);
}

#define strcat_direction(x_y, SIGN, STR, DEST, index) \
  if (path[index - 1].x_y SIGN path[index].x_y) { strcat(DEST, STR); }

void print_map(const map_t &map, const std::vector<point> &path)
{
  int **map_array;

  if (path.empty()) {
    mprintf("No path to finish!\n");
    return;
  }

  init_2d_array(map.height, map.width, map_array, int, map.map_array[y][x])

  for (int i = path.size() - 1; i >= 0; i--) {
    map_array[path[i].y][path[i].x] = 5;
  }

  for (int y = 0; y < map.height; y++) {
    for (int x = 0; x < map.width; x++) {
      if (map_array[y][x] == 5)
        mprintf(" * ");
      else if (map_array[y][x] == 0)
        mprintf("   ");
      else if (map_array[y][x] == 1)
        mprintf("MWM");
      else
        mprintf("%d  ", map_array[y][x]);
    }
    mprintf("\n");
  }

  free_2d_array(map.height, map_array)
}

void print_direction(const std::vector<point> &path, bool with_new_line)
{
  char strd[BUFF_SIZE];
  strcpy(strd, "");

  if (path.empty()) {
    mprintf("No path to finish!\n");
    return;
  }

  int j = 0;
  // Create Map + Path
  for (int i = path.size() - 1; i >= 0; i--) {
    if (i != 0) {
      strcat_direction(x, >, "Right", strd, i)
      strcat_direction(y, >, "Down", strd, i)
      strcat_direction(x, <, "Left", strd, i)
      strcat_direction(y, <, "Up", strd, i)

      if (i != 1) {
        if (with_new_line)
          strcat(strd, "\n");
        else {
          strcat(strd, ", ");
          if (++j % 12  == 0)
            strcat(strd, "\n");
        }
      }
    }
  }

  mprintf("%s\n", strd);
}

void enable_trace_print()
{
  trace_print = true;
}

void enable_stdout_write()
{
  stdout_write = true;
}
END_NAMESPACE(map_printer)
// [/map_printer] ////

// -------------------------------------------------------------------------- //
// -------------------------------------------------------------------------- //
