#include <queue>

#include "point.h"
#include "map.h"

// [bfs] ////
BEGIN_NAMESPACE(bfs)
static const map_t *map;
static int **visited; // 0 == unvisited; 1 == visited;
static point **prev_node;
static std::queue<point> nodes;

static void add_node(const point &next, const point &prev)
{
  // if next node is walkable and haven't visited then enqueue it
  if (visited[next.y][next.x] == 0 && map->map_array[next.y][next.x] == 0) {
    nodes.push(next);
    prev_node[next.y][next.x] = prev;
  }
}

#define add_next_node(valid_condition, x, y, node) \
  if (valid_condition) { point next = point(node.x, node.y); add_node(next, node); }

static void visit(const point &node)
{
  visited[node.y][node.x] = 1;

  add_next_node((node.y != 0), x, y - 1, node) // top
  add_next_node((node.x != map->width - 1), x + 1, y, node) // right
  add_next_node((node.y != map->height - 1), x, y + 1, node) // bottom
  add_next_node((node.x != 0), x - 1, y, node) // left
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
END_NAMESPACE(bfs)
// [/bfs] ////

path_t bfs_solve(const map_t &map)
{
  bfs::init_properties(map);
  bfs::nodes.push(map.start);

  map_printer::mprintf_trace("BFS Trace:\n");
  point current;
  do {
    current = bfs::nodes.front();
    bfs::nodes.pop();
    map_printer::mprintf_trace("(%d, %d)\n", current.x, current.y);
    bfs::visit(current);
  } while (!current.equal_to(map.finish) && !bfs::nodes.empty());
  map_printer::mprintf_trace("\n");

  path_t path; path_t empty_path;

  // Fail
  if (bfs::nodes.empty() && !current.equal_to(map.finish))
    path = empty_path;
  // Success
  else
    path = bfs::create_path();

  free_2d_array(map.height, bfs::visited)
  free_2d_array(map.height, bfs::prev_node)
  return path;
}
