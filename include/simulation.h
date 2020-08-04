#pragma once
#include <cell_world.h>
#include <cell_world_tools.h>
#include <rewards.h>
#include <predator.h>
#include <poi_prey.h>

struct Simulation_parameters : cell_world_tools::Parameters_builder {
    cell_world::World world;
    cell_world::Path_builder path_type;
    cell_world::Coordinates prey_start;
    cell_world::Coordinates predator_start;
    cell_world::Coordinates goal;
    Reward reward;
    Parameters_definitions({
        Add_web_resource(world, ({"world"}));
        Add_web_resource_from_table(path_type,"paths" ,({"world","path_type"}));
        Add_value(prey_start);
        Add_value(goal);
        Add_value(predator_start);
        Add_web_resource(reward, ({"reward"}));
   })
};

struct Simulation {
    explicit Simulation(Simulation_parameters &);

    unsigned int run();

    cell_world::World world;
    cell_world::Cell_group pois;
    cell_world::Graph world_graph;
    cell_world::Graph poi_graph;
    cell_world::Cell_group cells;
    cell_world::Map map;
    cell_world::Paths paths;
    cell_world::Graph visibility;
    cell_world::Model model;
    Poi_prey prey;
    Predator predator;
};