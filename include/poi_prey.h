#pragma once
#include <cell_world.h>
#include <predator.h>
#include <particle_filter.h>
#include <planner.h>

struct Poi_prey_state : cell_world::Agent_internal_state {
    Poi_prey_state():
        status(),
        move(),
        coordinates(),
        estimated_reward(0){}
    cell_world::Agent_status_code status;
    cell_world::Move move;
    cell_world::Coordinates coordinates;
    double estimated_reward;
};

struct Poi_prey : cell_world::Stateful_agent<Poi_prey_state> {
    Poi_prey(const cell_world::Cell_group &,
             const cell_world::Graph &,
             const cell_world::Cell_group &,
             const cell_world::Graph &,
             const cell_world::Graph &,
             const cell_world::Paths &,
             const cell_world::Cell &,
             const cell_world::Cell &,
             const Planning_parameters &);
    const cell_world::Cell &start_episode() override;
    cell_world::Move get_move(const cell_world::Model_public_state &) override;
    cell_world::Agent_status_code update_state(const cell_world::Model_public_state &) override;

    bool process_state(const cell_world::Model_public_state &);

    const cell_world::Cell_group &cells;
    const cell_world::Cell_group &pois;
    const cell_world::Graph &pois_graph;
    const cell_world::Graph &world_graph;
    const cell_world::Graph &visibility;
    const cell_world::Cell &start_cell;
    const cell_world::Cell &goal;
    const Planning_parameters &parameters;

    Particle_filter particle_filter;
    Planner planner;
};