#include <agents/poi_prey.h>

Poi_prey::Poi_prey(const cell_world::Graph &world_graph,
                   const cell_world::Paths &paths,
                   const cell_world::Cell &start_cell,
                   const cell_world::Cell &goal):
                   world_graph(world_graph),
                   paths(paths),
                   start_cell(start_cell),
                   goal(goal){ }
