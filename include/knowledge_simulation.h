#pragma once
#include <cell_world.h>
#include <static_data.h>
#include <reward.h>
#include <predator.h>
#include <knowledge_prey.h>


struct Knowledge_simulation_parameters{
    std::string id;
    cell_world::World_info world_info;
    std::string path_type;
    Particle_filter_parameters particle_filter;
    Predator_parameters predator;
    cell_world::Coordinates predator_location;
    cell_world::Coordinates prey_location;
    cell_world::Coordinates goal;
    json_cpp::Json_vector<unsigned int> seeds;
    unsigned int steps;
    std::string knowledge_file;
    std::string result_file;
};

struct Knowledge_simulation {
    explicit Knowledge_simulation(Knowledge_simulation_parameters &);
    unsigned int run();
    void show_map();
    static std::string format(const std::string &, unsigned int);
    Knowledge_simulation_parameters parameters;
    Static_data data;
    cell_world::Model model;
    Knowledge_prey prey;
    Predator predator;
};