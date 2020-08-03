#include <cell_world.h>
#include <cell_world_tools.h>
#include <poi_prey.h>

struct Simulation_parameters : cell_world_tools::Parameters_builder {
    cell_world::World world;
    cell_world::Path_builder paths;
    cell_world::Coordinates prey_start;
    cell_world::Coordinates predator_start;
    cell_world::Coordinates goal;
    Parameters_definitions({
        Add_web_resource(world, ({"world"}));
        Add_web_resource(paths, ({"world","paths"}));
//        Add_value(prey_start);
//        Add_value(goal);
//        Add_value(predator_start);
    })
};

struct Simulation {
    Simulation(Simulation_parameters &);
    cell_world::World world;
    cell_world::Graph world_graph;
    cell_world::Cell_group cells;
    cell_world::Map map;
    cell_world::Paths paths;
    cell_world::Graph visibility;
    cell_world::Model model;
    Poi_prey prey;
    Predator predator;
};