#pragma once
#include <cell_world.h>
#include <static_data.h>
#include <reward.h>
#include <predator.h>
#include <training_prey.h>


struct Training_parameters {
    cell_world::World_info world_info;
    std::string path_type;
    Predator_parameters predator;
    cell_world::Coordinates goal;
    unsigned int episodes;
    std::string input_file;
    std::string output_file;
};

struct Training {
    explicit Training(Training_parameters &);

    unsigned int run();
    Training_parameters parameters;
    Static_data data;
    cell_world::Model model;
    Training_prey prey;
    Predator predator;
};