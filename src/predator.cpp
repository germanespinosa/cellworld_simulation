//
// Created by german on 7/31/20.
//

#include <agents/predator.h>

Predator::Predator(
        const cell_world::Graph &world_graph,
        const cell_world::Graph &visibility,
        const cell_world::Paths &paths,
        const cell_world::Cell &start_cell):
        world_graph(world_graph),
        visibility(visibility),
        paths(paths),
        start_cell(start_cell){

}
