#pragma once
#include <cell_world.h>
#include <particle_filter/particle_filter.h>

struct Poi_prey_state : cell_world::Agent_internal_state {

};

struct Poi_prey : cell_world::Stateful_agent<Poi_prey_state> {
    Poi_prey(const cell_world::Graph &, const cell_world::Graph &, const cell_world::Paths &, const cell_world::Cell
    &, const cell_world::Cell &);
    const cell_world::Cell &start_episode() override;
    cell_world::Move get_move(const cell_world::Model_public_state &) override;
    cell_world::Agent_status_code update_state(const cell_world::Model_public_state &) override;
    void end_episode(const cell_world::Model_public_state &) override;
    const cell_world::Graph &world_graph;
    const cell_world::Graph &visibility;
    const cell_world::Paths &paths;
    const cell_world::Cell &start_cell;
    const cell_world::Cell &goal;
    Particle_filter particle_filter;
};