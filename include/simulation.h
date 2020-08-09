#pragma once
#include <cell_world.h>
#include <cell_world_tools.h>
#include <static_data.h>
#include <reward.h>
#include <predator.h>
#include <poi_prey.h>

struct Simulation_parameters : cell_world::Parameters_builder {
    cell_world::World world;
    std::string path_type;
    Planner_parameters planner;
    Particle_filter_parameters particle_filter;
    Poi_prey_parameters prey;
    Predator_parameters predator;
    Parameters_definitions({
        Add_web_resource(world, ({"world"}));
        Add_value(path_type);
        Add_web_resource(planner,({"planning"}));
        Add_web_resource(particle_filter,({"particle_filter"}));
        Add_web_resource(prey,({"prey"}));
        Add_web_resource(predator,({"predator"}));
   })
};

struct Simulation {
    explicit Simulation(Simulation_parameters &);
    unsigned int run();
    Simulation_parameters parameters;
    Static_data data;
    cell_world::Model model;
    Poi_prey prey;
    Predator predator;
};