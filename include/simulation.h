#pragma once
#include <cell_world.h>
#include <cell_world_tools.h>
#include <static_data.h>
#include <reward.h>
#include <predator.h>
#include <poi_prey.h>


struct Simulation_parameters : cell_world::Parameters_builder {
    std::string id;
    cell_world::World world;
    std::string path_type;
    Planner_parameters planner;
    Particle_filter_parameters particle_filter;
    Poi_prey_parameters prey;
    Predator_parameters predator;
    cell_world::Coordinates predator_location;
    json_cpp::Json_vector<unsigned int> seeds;
    bool predator_present;
    std::string result_file;
    Parameters_definitions({
        Add_value(id);
        Add_web_resource(world, ({"world"}));
        Add_value(path_type);
        Add_web_resource(planner,({"planner"}));
        Add_web_resource(particle_filter,({"particle_filter"}));
        Add_web_resource(prey,({"prey"}));
        Add_web_resource(predator,({"predator"}));
        Add_value(predator_location);
        Add_value(seeds);
        Add_value(predator_present);
        Add_value(result_file);
   })
};

struct Simulation {
    explicit Simulation(Simulation_parameters &);
    unsigned int run();
    void show_map();
    Simulation_parameters parameters;
    Static_data data;
    cell_world::Model model;
    Poi_prey prey;
    Predator predator;
    cell_world::Stateless_agent ghost;
    static std::string format(const std::string &, unsigned int);

};