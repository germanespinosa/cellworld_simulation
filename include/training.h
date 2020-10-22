#pragma once
#include <cell_world.h>
#include <cell_world_tools.h>
#include <static_data.h>
#include <reward.h>
#include <predator.h>
#include <training_prey.h>


struct Training_parameters : cell_world::Parameters_builder {
    cell_world::World world;
    std::string path_type;
    Predator_parameters predator;
    cell_world::Coordinates predator_location;
    cell_world::Coordinates goal;
    unsigned int episodes;
    std::string input_file;
    std::string output_file;
    Parameters_definitions({
                               Add_web_resource(world, ({"world"}));
                               Add_value(path_type);
                               Add_web_resource(predator,({"predator"}));
                               Add_value(predator_location);
                               Add_value(goal);
                               Add_value(episodes);
                               Add_value(input_file);
                               Add_value(output_file);
                           })
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