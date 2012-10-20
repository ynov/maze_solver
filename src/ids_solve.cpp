#include "point.h"
#include "map.h"

#define DEPTH_INCREMENT 2
#define MAX_DEPTH_ALLOWED 999

// [ids] ////
BEGIN_NAMESPACE(ids)
static const map_t *map;
static int **visited; // 0 == unvisited; 1 == visited;
static point **prev_node;
static bool finish_found = false;

static void reset_visited()
{
  for (int y = 0; y < map->height; y++)
    for (int x = 0; x < map->width; x++)
      visited[y][x] = 0;
}

static void search(const point &node, int depth);
static void add_node(const point &next, const point &prev, int depth)
{
  if (visited[next.y][next.x] == 0 && map->map_array[next.y][next.x] == 0) {
    if (next.equal_to(map->finish))
      finish_found = true;
    else
      search(next, depth - 1);

    prev_node[next.y][next.x] = prev;
  }
}

#define add_next_node(valid_condition, x, y, node, depth) \
  if (valid_condition && !finish_found) { point next = point(node.x, node.y); add_node(next, node, depth); }

static void search(const point &node, int depth)
{
  visited[node.y][node.x] = 1;
  map_printer::mprintf_trace("(%d, %d)\n", node.x, node.y);

  add_next_node((depth >= 0 && node.y != 0), x, y - 1, node, depth) // top
  add_next_node((depth >= 0 && node.x != map->width - 1), x + 1, y, node, depth) // right
  add_next_node((depth >= 0 && node.y != map->height - 1), x, y + 1, node, depth) // bottom
  add_next_node((depth >= 0 && node.x != 0), x - 1, y, node, depth) // left
}

static path_t create_path()
{
  path_t path;
  point prev = prev_node[map->finish.y][map->finish.x];

  // create path by backtracking through nodes from finish to start
  path.push_back(map->finish);
  while(!prev.equal_to(map->start)) {
    path.push_back(prev);
    prev = prev_node[prev.y][prev.x];
  }
  path.push_back(map->start);

  return path;
}

static void init_properties(const map_t &r_map)
{
  map = &r_map;
  init_2d_array(map->height, map->width, prev_node, point, point(-1, -1))
  init_2d_array(map->height, map->width, visited, int, 0)
}
END_NAMESPACE(ids)
// [/ids] ////

path_t ids_solve(const map_t &map)
{
  ids::init_properties(map);

  map_printer::mprintf_trace("IDS Trace:\n");

  int max_depth = 0;
  do {
    ids::reset_visited();
    ids::search(map.start, max_depth);
    max_depth += DEPTH_INCREMENT;
  } while (!ids::finish_found && max_depth < MAX_DEPTH_ALLOWED);

  map_printer::mprintf_trace("\n");

  path_t path; path_t empty_path;

  // Fail
  if (!ids::finish_found)
    path = empty_path;
  // Success
  else
    path = ids::create_path();

  free_2d_array(map.height, ids::visited)
  free_2d_array(map.height, ids::prev_node)
  return path;
}
