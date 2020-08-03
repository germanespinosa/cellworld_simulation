#pragma once
#include <cell_world.h>

struct Predator_state : cell_world::Agent_internal_state {
};

struct Predator : cell_world::Stateful_agent<Predator_state> {
    Predator (const cell_world::Graph &,const cell_world::Graph &, const cell_world::Paths &, const cell_world::Cell &);
    Predator (const cell_world::Graph &,const cell_world::Graph &, const cell_world::Paths &);
    const cell_world::Cell &start_episode() override;
    cell_world::Move get_move(const cell_world::Model_public_state &) override;
    cell_world::Agent_status_code update_state(const cell_world::Model_public_state &) override;
    void end_episode(const cell_world::Model_public_state &) override;
    const cell_world::Graph &world_graph;
    const cell_world::Graph &visibility;
    const cell_world::Paths &paths;
    cell_world::Cell start_cell;
};