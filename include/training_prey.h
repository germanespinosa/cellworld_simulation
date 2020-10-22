#pragma once
#include <cell_world.h>
#include <predator.h>
#include <static_data.h>
#include <off_line_knowledge.h>

struct Training_prey_state : cell_world::Agent_internal_state {
    Training_prey_state():
            status(),
            move(),
            coordinates(){}
    cell_world::Agent_status_code status;
    cell_world::Move move;
    cell_world::Coordinates coordinates;
    Json_object_members({
                            Add_member(status);
                            Add_member(move);
                            Add_member(coordinates);
                        })
};

struct Training_prey : cell_world::Stateful_agent<Training_prey_state> {
    Training_prey(const Static_data &,
                  const cell_world::Cell &,
                  const std::string &,
                  const std::string &);
    ~Training_prey();
    const cell_world::Cell &start_episode() override;
    cell_world::Move get_move(const cell_world::Model_public_state &) override;
    cell_world::Agent_status_code update_state(const cell_world::Model_public_state &) override;
    void update_knowledge_map(bool);
    const Static_data &data;
    const cell_world::Cell &goal;
    const cell_world::Cell_group start_cells;
    cell_world::Cell start;
    Off_line_knowledge_map knowledge_map;
    Off_line_knowledge_key key{0,0,0};
    const std::string &output_file;
};