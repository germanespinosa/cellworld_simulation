#pragma once
#include <cell_world.h>
#include <planner.h>
#include <particle_filter.h>
#include <predator.h>
#include <static_data.h>

struct Poi_prey_parameters : json_cpp::Json_object {
    cell_world::Coordinates start;
    cell_world::Coordinates goal;
    unsigned int steps;
    Json_object_members({
        Add_member(start);
        Add_member(goal);
        Add_member(steps);
    })
};

struct Poi_prey_state : cell_world::Agent_internal_state {
    Poi_prey_state():
        status(),
        move(),
        coordinates(),
        option(),
        estimated_reward(0){}
    cell_world::Agent_status_code status;
    cell_world::Move move;
    cell_world::Coordinates coordinates;
    cell_world::Coordinates option;
    cell_world::Cell_group belief_state;
    double estimated_reward;
    std::string tree_mode;
    Json_object_members({
        Add_member(status);
        Add_member(move);
        Add_member(coordinates);
        Add_member(option);
        Add_member(estimated_reward);
        Add_member(tree_mode);
    })
};

struct Poi_prey : cell_world::Stateful_agent<Poi_prey_state> {
    Poi_prey(const Poi_prey_parameters &,
             const Planner_parameters &,
             const Particle_filter_parameters &,
             const Predator_parameters &,
             const Static_data &);
    const cell_world::Cell &start_episode() override;
    cell_world::Move get_move(const cell_world::Model_public_state &) override;
    cell_world::Agent_status_code update_state(const cell_world::Model_public_state &) override;

    const Poi_prey_parameters &parameters;
    const Static_data &data;
    const cell_world::Cell &start;
    const cell_world::Cell &goal;
    Planner planner;
};