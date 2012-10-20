#include <cstdlib>
#include <cstdio>
#include <cstring>

#include "map.h"

extern path_t bfs_solve(const map_t &map);
extern path_t dfs_solve(const map_t &map);
extern path_t ids_solve(const map_t &map);

int main(int argc, char **argv)
{
  using namespace map_printer;
  map_t map;

  // parse opts if given
  if (argc > 1) {
    map = map_reader::read(argv[argc - 1]);
    if (argc > 2) {
      if (strchr(argv[argc - 2], '-') != NULL) {
        if (strchr(argv[argc - 2], 'v') != NULL) {
          enable_trace_print();
        }
        if (strchr(argv[argc - 2], 's') != NULL) {
          enable_stdout_write();
        }
      }
    }
  }
  // no opts given
  else {
    printf("Usage: %s [-sv] mapfile.txt\n\n", argv[0]);
    printf("    -s     Print output to stdout (console)\n");
    printf("    -v     Print graph trace\n");
    printf("    -sv    Both -s and -v combined\n");
    printf("\nExample: %s -sv maps/map_1.txt\n", argv[0]);
    printf("Default output file is output.txt on program working directory\n");
    return 0;
  }

  init_printer();

  path_t bfs_path = bfs_solve(map);
  path_t dfs_path = dfs_solve(map);
  path_t ids_path = ids_solve(map);

  mprintf("> BFS:\n");
  print_map(map, bfs_path); print_direction(bfs_path);

  mprintf("\n> DFS:\n");
  print_map(map, dfs_path); print_direction(dfs_path);

  mprintf("\n> IDS:\n");
  print_map(map, ids_path); print_direction(ids_path);

  close_printer();
  free_2d_array(map.height, map.map_array)
  return 0;
}
