#pragma once
#include <cell_world.h>
#include <particle_filter.h>
#include <predator.h>
#include <static_data.h>
#include <off_line_knowledge.h>


struct Knowledge_prey_state : cell_world::Agent_internal_state {
    Knowledge_prey_state():
            status(),
            move(),
            coordinates(),
            estimated_reward(0){}
    cell_world::Agent_status_code status;
    cell_world::Move move;
    cell_world::Coordinates coordinates;
    double estimated_reward;
    Json_object_members({
                            Add_member(status);
                            Add_member(move);
                            Add_member(coordinates);
                            Add_member(estimated_reward);
                        })
};

struct Knowledge_prey : cell_world::Stateful_agent<Knowledge_prey_state> {
    Knowledge_prey(const cell_world::Cell &,
                   const cell_world::Cell &,
                   const Particle_filter_parameters &,
                   const Predator_parameters &,
                   const Static_data &,
                   const std::string &);
    const cell_world::Cell &start_episode() override;
    cell_world::Move get_move(const cell_world::Model_public_state &) override;
    cell_world::Agent_status_code update_state(const cell_world::Model_public_state &) override;

    const Static_data &data;
    const cell_world::Cell &start;
    const cell_world::Cell &goal;

    Off_line_knowledge_map knowledge_map;
    Particle_filter filter;
};