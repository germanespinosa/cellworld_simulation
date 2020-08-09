#pragma once
#include <cell_world.h>

struct Static_data{
    Static_data(cell_world::World &world,
                std::string path_type );
    const cell_world::Graph world_graph;
    const cell_world::Cell_group &cells;
    const cell_world::Map map;
    const cell_world::Paths paths;
    const cell_world::Graph visibility;
    const cell_world::Graph inverted_visibility;
    const cell_world::Cell_group pois;
    const cell_world::Graph pois_graph;
};